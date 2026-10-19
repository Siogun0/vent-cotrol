#ifndef INC_CAN_PLATFORM_H_
#define INC_CAN_PLATFORM_H_

#include <stdint.h>
#include <string.h>
#include <esp_attr.h>
#include <esp_twai.h>
#include <esp_twai_onchip.h>

#define CAN_BUS_MAX     1
#define VIRT_MBX_MAX    14

#define CAN0	0

typedef struct
{
	uint32_t arrived;
	uint32_t transmitting;

	uint8_t buffer[8];
	twai_frame_t twai_frame;
} virtual_mailbox_t;

void IRAM_ATTR platform_can_msg_recieve(uint32_t bus_id, const twai_frame_t* twai_message);

void platform_can_init(uint32_t bus_id, uint32_t bus_speed, uint32_t rx_pin, uint32_t tx_pin);

void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);

void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);

void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg);

void platform_can_dyn_xmit_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc, uint64_t msg);

void platform_can_xmit_array_mb(uint32_t bus_id, uint32_t mbn, uint8_t* data, uint8_t len);

void platform_can_dyn_xmit_array_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc, uint8_t* data, uint8_t len);

uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn);

uint32_t platform_can_get_mb_dlc(uint32_t bus_id, uint32_t mbn);

uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn);

uint32_t platform_can_is_message_sent(uint32_t bus_id, uint32_t mbn);


#endif /* INC_CAN_PLATFORM_H_ */
