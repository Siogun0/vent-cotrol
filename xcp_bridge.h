#include <cstring>
#ifndef XCP_BRIDGE_H
#define XCP_BRIDGE_H

#include <IPAddress.h>
#include <AsyncUDP.h>
#include <AsyncTCP.h>
#include "can_platform.h"

enum Xcp_connect_type
{
  XCP_OFF,
  XCP_UDP,
  XCP_TCP,
  XCP_UDP_TCP
};

class XcpBridge
{
public:
  // Init before first constructor run
  uint32_t NEXT_FREE_MBN = 5;

  XcpBridge(Xcp_connect_type type, uint32_t udp_port, uint32_t tcp_port,  uint32_t device_rx_id, uint32_t device_tx_id, uint32_t daq = 0)
  {
    _rx_id = device_rx_id;
    _tx_id = device_tx_id;
    // daq_number = daq;
    _rx_mbn = NEXT_FREE_MBN++;
    _tx_mbn = NEXT_FREE_MBN++;

    platform_can_init_rx_mb(CAN0, _tx_mbn, _tx_id, 8);
    platform_can_init_tx_mb(CAN0, _rx_mbn, _rx_id, 8);

    if (type == XCP_TCP || type == XCP_UDP_TCP)
    {
      Serial.println("Создаём TCP server");
      _tcp_server = new AsyncServer(tcp_port);
      _tcp_server->onClient([this](void* arg, AsyncClient* client){
        Serial.printf("Клиент подключился: %s\n", client->remoteIP().toString().c_str());

        // Назначаем обработчик данных для этого клиента
        client->onData(&XcpBridge::receiveTcpPacket, this);

        // Коллбэк на отключение
        client->onDisconnect([this](void* arg, AsyncClient* client) {
          Serial.printf("Клиент отключился\n");
          delete client; // Освобождаем память
          _tcp_server = nullptr;
        }, this);
            
      }, this);

      _tcp_server->begin();
      Serial.printf("Сервер XCP<=>TCP запущен на порту %d для ID >0x%03X <0x%03X\n", tcp_port, _rx_id, _tx_id);
    }

    if (type == XCP_UDP || type == XCP_UDP_TCP)
    {
      _udp_server = new AsyncUDP;
      _udp_server->listen(udp_port);
      _udp_server->onPacket(&XcpBridge::receiveUdpPacket, this);
      Serial.printf("Сервер XCP<=>UDP запущен на порту %d для ID >0x%03X <0x%03X\n", udp_port, _rx_id, _tx_id);
    }

    if (_tcp_server == nullptr && _udp_server == nullptr)
    {
      Serial.println("Сервер XCP не запущен");
    }
  }

  ~XcpBridge()
  {
    if (_tcp_server)
    {
      Serial.println("Удаляем TCP server");
      _tcp_server->end();
      delete _tcp_server;
    }
    if (_udp_server)
    {
      delete _udp_server;
    }
  }

  void sendCanPacket(uint32_t id, uint8_t* data, uint8_t len)
  {
    uint8_t dlc = data[0];
    uint8_t* can_data = &data[4];

    if (dlc <= 8 && (dlc + 4) <= len)
    {
      platform_can_dyn_xmit_array_mb(CAN0, _rx_mbn, id, dlc, can_data, len);
    }
  }

  void ethernetPacketBuild(uint8_t* buf, uint8_t* input, uint8_t len)
  {    
    buf[0] = len;
    buf[1] = 0;
    buf[2] = _cntr & 0xFF;
    buf[3] = (_cntr >> 8) & 0xFF;
    _cntr++;
    std::memcpy(&buf[4], input, len);
  }

  static void receiveTcpPacket(void* arg, AsyncClient* client, void* data, size_t len);

  void handleTcpPacket(AsyncClient* client, void* data_tcp, size_t len)
  {
    _connection_type = XCP_TCP;
    _tcp_client = client;

    sendCanPacket(_rx_id, (uint8_t*)data_tcp, len);
    Serial.println("отправлен по CAN");
  }

  static void receiveUdpPacket(void *arg, AsyncUDPPacket packet);

  void handleUdpPacket(AsyncUDPPacket packet)
  {
    _connection_type = XCP_UDP;
    _udp_bind_IP = packet.remoteIP();
    _udp_bind_port = packet.remotePort();

    sendCanPacket(_rx_id, packet.data(), packet.length());
  }

  void receiveCanPacket(uint32_t id, uint8_t* input, uint8_t len)
  {
    Serial.printf("ID 0x%03X, DLC %d, input[0] 0x%02X\n", id, len, input[0]);
    if (id == _tx_id)
    {
      ethernetPacketBuild(_buffer, input, len);
      uint8_t ethernetPacketSize = len + 4;
      if (_connection_type == XCP_TCP
          // && _tcp_server
          && _tcp_client 
          && _tcp_client->connected()
          && _tcp_client->space() >= ethernetPacketSize)
      {
        Serial.println("отправлен по TCP");
        _tcp_client->write((const char*)_buffer, ethernetPacketSize);
      }
      else
      {
        Serial.printf("_tcp_server %d\n", _tcp_server);
        Serial.printf("_tcp_client %d\n", _tcp_client);
        Serial.printf("connected %d\n", _tcp_client->connected());
        Serial.printf("space %d\n", _tcp_client->space());
      }
      // else if (_connection_type == XCP_UDP
      //     && _udp_server
      //     && _udp_bind_port != 0)
      // {
      //   Serial.println("отправлен по UDP");
      //   _udp_server->writeTo(_buffer, ethernetPacketSize, _udp_bind_IP, _udp_bind_port);
      // }
    }
  }

  uint32_t rx_mbn() { return _rx_mbn; }
  uint32_t tx_mbn() { return _tx_mbn; }

private:
  AsyncUDP* _udp_server = nullptr;
  AsyncServer* _tcp_server = nullptr;
  AsyncClient* _tcp_client = nullptr;
  uint8_t _buffer[20];
  Xcp_connect_type _connection_type;
  uint32_t _rx_id;
  uint32_t _tx_id;
  uint16_t _cntr = 0;
  uint32_t _rx_mbn;
  uint32_t _tx_mbn;

  IPAddress _udp_bind_IP;
  uint16_t _udp_bind_port;
};


// Статический мост-приемник пакета
void XcpBridge::receiveTcpPacket(void* arg, AsyncClient* client, void* data, size_t len) {
  // Извлекаем наш объект класса из void*
  XcpBridge* instance = static_cast<XcpBridge*>(arg);
    
  if (instance != nullptr) {
      // Вызываем обычный не-статический метод класса для обработки данных
      instance->handleTcpPacket(client, (uint8_t*)data, len);
  }
}

void XcpBridge::receiveUdpPacket(void *arg, AsyncUDPPacket packet) {
  // Serial.print("Получены данные от IP: ");
  // Serial.print(packet.remoteIP());
  // Serial.print(", Порт: ");
  // Serial.println(packet.remotePort());
    // Извлекаем наш объект класса из void*
  XcpBridge* instance = static_cast<XcpBridge*>(arg);
    
  if (instance != nullptr) {
      // Вызываем обычный не-статический метод класса для обработки данных
      instance->handleUdpPacket(packet);
  }
}

#endif // XCP_BRIDGE_H