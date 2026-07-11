/*
 * can_platform.c
 *
 *  Created on: Dec 3, 2021
 *      Author: arh
 */
#include "can_platform.h"


static virtual_mailbox_t virt_mbx[CAN_BUS_MAX][VIRT_MBX_MAX] = {0};

void platform_can_init()
{
 // 1. Конфигурация контактов и режима работы
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);

  // 2. Установка скорости шины (например, 500 kbps)
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();

  // 3. Настройка фильтрации (принимать все сообщения)
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // 4. Установка драйвера
  twai_driver_install(&g_config, &t_config, &f_config);

  // 5. Запуск драйвера
  twai_start();
}

uint32_t platform_can_poll(uint32_t bus_id)
{
	twai_message_t message = {};
	if(twai_receive(&message, pdMS_TO_TICKS(0)) == ESP_OK)
	{
		platform_can_msg_recieve(bus_id, &message);
		return 1;
	}
	return 0;
}

void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc)
{
	uint32_t mask_or_id;

    virt_mbx[bus_id][mbn].id = id;
    virt_mbx[bus_id][mbn].dlc = dlc;
}

void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc)
{
	virt_mbx[bus_id][mbn].id = id;
}

void platform_can_msg_recieve(uint32_t bus_id, const twai_message_t* twai_message)
{
	for(int i = 0; i < VIRT_MBX_MAX; i++)
	{
		if(twai_message->identifier == virt_mbx[bus_id][i].id)
		{
			virt_mbx[bus_id][i].arrived = 1;
			virt_mbx[bus_id][i].data = (uint64_t)twai_message->data[0] << 0 |
					(uint64_t)twai_message->data[1] << 8 |
					(uint64_t)twai_message->data[2] << 16 |
					(uint64_t)twai_message->data[3] << 24 |
					(uint64_t)twai_message->data[4] << 32 |
					(uint64_t)twai_message->data[5] << 40 |
					(uint64_t)twai_message->data[6] << 48 |
					(uint64_t)twai_message->data[7] << 56;
			return;
		}
	}
}

void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg)
{
	platform_can_dyn_xmit_mb(bus_id, mbn, virt_mbx[bus_id][mbn].id, 8, msg);
}

void platform_can_dyn_xmit_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc, uint64_t msg)
{
	twai_message_t twai_message = {};

	// for standart ID
	twai_message.identifier = id;

	twai_message.extd = 0;
	twai_message.rtr = 0;
	twai_message.data_length_code = dlc;

	twai_message.data[0] = (msg >> 0) & 0xFF;
	twai_message.data[1] = (msg >> 8) & 0xFF;
	twai_message.data[2] = (msg >> 16) & 0xFF;
	twai_message.data[3] = (msg >> 24) & 0xFF;
	twai_message.data[4] = (msg >> 32) & 0xFF;
	twai_message.data[5] = (msg >> 40) & 0xFF;
	twai_message.data[6] = (msg >> 48) & 0xFF;
	twai_message.data[7] = (msg >> 56) & 0xFF;

	twai_transmit(&twai_message, pdMS_TO_TICKS(0));
}

uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn)
{
	virt_mbx[bus_id][mbn].arrived = 0;
	return virt_mbx[bus_id][mbn].data;
}

uint32_t platform_can_get_mb_dlc(uint32_t bus_id, uint32_t mbn)
{
	return virt_mbx[bus_id][mbn].dlc;
}

uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn)
{
	return virt_mbx[bus_id][mbn].arrived;
}

uint32_t platform_can_is_message_sent(uint32_t bus_id, uint32_t mbn)
{
	return 1;
}

