#include "esp_twai.h"
#include "can_platform.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

static virtual_mailbox_t DRAM_ATTR virt_mbx[CAN_BUS_MAX][VIRT_MBX_MAX] = {0};
static twai_node_handle_t DRAM_ATTR node_hdl[CAN_BUS_MAX] = {NULL};

TaskHandle_t twai_rx_task[CAN_BUS_MAX] = {NULL};
QueueHandle_t twai_rx_queue[CAN_BUS_MAX] = {NULL};

static bool IRAM_ATTR twai_rx_cb(twai_node_handle_t handle, const twai_rx_done_event_data_t *edata, void *user_ctx)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t recv_buff[8];
    twai_frame_t rx_frame = {
        .buffer = recv_buff,
        .buffer_len = sizeof(recv_buff),
    };
    if (twai_node_receive_from_isr(handle, &rx_frame) == ESP_OK)
		{
			xQueueSendFromISR(twai_rx_queue[(uint32_t)user_ctx], &rx_frame, &xHigherPriorityTaskWoken);
    }
    return xHigherPriorityTaskWoken == pdTRUE;
}

static bool IRAM_ATTR twai_tx_done_cb(twai_node_handle_t handle, const twai_tx_done_event_data_t *edata, void *user_ctx)
{
	uint32_t bus_id = (uint32_t)user_ctx;
	for(int mbn = 0; mbn < VIRT_MBX_MAX; ++mbn)
	{
		if(edata->done_tx_frame == &virt_mbx[bus_id][mbn].twai_frame)
		{
			virt_mbx[bus_id][mbn].transmitting = 0;
			break;
		}
	}
	return false;
}

static bool IRAM_ATTR twai_state_change_cb(twai_node_handle_t handle, const twai_state_change_event_data_t *edata, void *user_ctx)
{
	if (edata->new_sta == TWAI_ERROR_BUS_OFF)
	{
		twai_node_recover(handle);
	}
	return false;
}

void platform_can_rx_task(void* user_ctx)
{
	const uint32_t bus_id = (uint32_t)user_ctx;
	while(1)
	{
	  uint8_t recv_buff[8];
    twai_frame_t rx_frame = {
        .buffer = recv_buff,
        .buffer_len = sizeof(recv_buff),
    };
		if (xQueueReceive(twai_rx_queue[bus_id], &rx_frame, portMAX_DELAY) == pdPASS)
		{
			platform_can_msg_recieve(bus_id, &rx_frame);
		}
	}
}

void platform_can_init(uint32_t bus_id, uint32_t bus_speed, uint32_t rx_pin, uint32_t tx_pin)
{
	twai_onchip_node_config_t node_config = {
    .io_cfg.tx = tx_pin,	//TODO bus_id
    .io_cfg.rx = rx_pin,
    .bit_timing.bitrate = bus_speed,
    .tx_queue_depth = VIRT_MBX_MAX,        // Transmit queue depth set
	};

	ESP_ERROR_CHECK(twai_new_node_onchip(&node_config, &node_hdl[bus_id]));

	twai_event_callbacks_t user_cbs = {
    .on_rx_done = twai_rx_cb,
		.on_tx_done = twai_tx_done_cb,
		.on_state_change = twai_state_change_cb,
		.on_error = NULL
	};
	ESP_ERROR_CHECK(twai_node_register_event_callbacks(node_hdl[bus_id], &user_cbs, (void*)bus_id));

	twai_rx_queue[bus_id] = xQueueCreate( VIRT_MBX_MAX, sizeof(twai_frame_t) );
	ESP_ERROR_CHECK(twai_rx_queue[bus_id] ? ESP_OK : ESP_FAIL);

	// Создаем задачу FreeRTOS
  xTaskCreatePinnedToCore(
    platform_can_rx_task, // Функция, которая будет выполняться
    "СAN_Task",           // Имя задачи (для отладки)
    4096,                 // Размер стека в байтах
    (void*)bus_id,        // Параметры задачи
    1,                    // Приоритет задачи (1 - низкий, вполне достаточно)
    &twai_rx_task[bus_id],// Хэндл задачи
    tskNO_AFFINITY        // На каком ядре запускать (Ядро 0)
  );

	ESP_ERROR_CHECK(twai_node_enable(node_hdl[bus_id]));
}

void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc)
{
  platform_can_init_tx_mb(bus_id, mbn, id, dlc);
}

void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc)
{
	virt_mbx[bus_id][mbn].twai_frame.header.id = id;
	virt_mbx[bus_id][mbn].twai_frame.header.dlc = dlc;
	virt_mbx[bus_id][mbn].twai_frame.header.ide = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.rtr = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.fdf = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.brs = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.esi = 0;

	virt_mbx[bus_id][mbn].twai_frame.buffer = virt_mbx[bus_id][mbn].buffer;
	virt_mbx[bus_id][mbn].twai_frame.buffer_len = dlc;
}

void IRAM_ATTR platform_can_msg_recieve(uint32_t bus_id, const twai_frame_t* twai_frame)
{
	for(int mbn = 0; mbn < VIRT_MBX_MAX; ++mbn)
	{
		if(twai_frame->header.id == virt_mbx[bus_id][mbn].twai_frame.header.id)
		{
			memcpy(virt_mbx[bus_id][mbn].buffer, twai_frame->buffer, twai_frame->buffer_len); //TODO queue
			virt_mbx[bus_id][mbn].arrived = 1;
			return;
		}
	}
}

void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg)
{
	virt_mbx[bus_id][mbn].buffer[0] = (msg >> 0) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[1] = (msg >> 8) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[2] = (msg >> 16) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[3] = (msg >> 24) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[4] = (msg >> 32) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[5] = (msg >> 40) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[6] = (msg >> 48) & 0xFF;
	virt_mbx[bus_id][mbn].buffer[7] = (msg >> 56) & 0xFF;

	// virt_mbx[bus_id][mbn].twai_frame.buffer = virt_mbx[bus_id][mbn].buffer;

	if (twai_node_transmit(node_hdl[bus_id], &virt_mbx[bus_id][mbn].twai_frame, 0) == ESP_OK)
  {
		virt_mbx[bus_id][mbn].transmitting = 1;
	}
}

void platform_can_dyn_xmit_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc, uint64_t msg)
{
	virt_mbx[bus_id][mbn].twai_frame.header.id = id;
  virt_mbx[bus_id][mbn].twai_frame.header.ide = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.dlc = dlc;
	virt_mbx[bus_id][mbn].twai_frame.buffer_len = dlc;

	platform_can_xmit_mb(bus_id, mbn, msg);
}

void platform_can_xmit_array_mb(uint32_t bus_id, uint32_t mbn, uint8_t* data, uint8_t len)
{
	len = len < 8 ? len : 8;
	memcpy(virt_mbx[bus_id][mbn].buffer, data, len);

	if (twai_node_transmit(node_hdl[bus_id], &virt_mbx[bus_id][mbn].twai_frame, 0) == ESP_OK)
  {
		virt_mbx[bus_id][mbn].transmitting = 1;
	}
}

void platform_can_dyn_xmit_array_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc, uint8_t* data, uint8_t len)
{
	virt_mbx[bus_id][mbn].twai_frame.header.id = id;
  virt_mbx[bus_id][mbn].twai_frame.header.ide = 0;
	virt_mbx[bus_id][mbn].twai_frame.header.dlc = dlc;
	virt_mbx[bus_id][mbn].twai_frame.buffer_len = dlc;

	platform_can_xmit_array_mb(bus_id, mbn, data, len);
}

uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn)
{
	virt_mbx[bus_id][mbn].arrived = 0;
	uint8_t* buffer = virt_mbx[bus_id][mbn].buffer;
	uint64_t data = (uint64_t)buffer[0] << 0 |
					(uint64_t)buffer[1] << 8 |
					(uint64_t)buffer[2] << 16 |
					(uint64_t)buffer[3] << 24 |
					(uint64_t)buffer[4] << 32 |
					(uint64_t)buffer[5] << 40 |
					(uint64_t)buffer[6] << 48 |
					(uint64_t)buffer[7] << 56;
	return data;
}

uint32_t platform_can_get_mb_dlc(uint32_t bus_id, uint32_t mbn)
{
	return virt_mbx[bus_id][mbn].twai_frame.header.dlc;
}

uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn)
{
	return virt_mbx[bus_id][mbn].arrived;
}

uint32_t platform_can_is_message_sent(uint32_t bus_id, uint32_t mbn)
{
	return virt_mbx[bus_id][mbn].transmitting ? 0 : 1;
}

