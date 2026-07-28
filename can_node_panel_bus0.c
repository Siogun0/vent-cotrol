/* Do not edit */
/* This file is generated automatically by dbc_conv */

#include "can_node_panel_bus0.h"


static uint32_t __rx_id_shift__ = 0;
static uint32_t __tx_id_shift__ = 0;
static uint32_t __mb_shift__ = 0;
static uint32_t multiplexor __attribute__ ((unused));

static volatile uint32_t msg_cntr_fan_status;
static volatile uint32_t msg_cntr_valve_status;
static volatile uint32_t msg_cntr_power_status;
static volatile uint32_t msg_cntr_ctrl_fan;
static volatile uint32_t msg_cntr_ctrl_valve;

//====== Auxiliary functions ======
static void can_node_memset(uint8_t *dst, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
    {
        dst[i] = 0;
    }
}

static inline uint32_t ltb32(uint32_t val)
{
    return (val & 0xFF000000) >> 24 | (val & 0x00FF0000) >> 8 | (val & 0x0000FF00) << 8 | (val & 0x000000FF) << 24;
}
static inline uint64_t ltb64(uint64_t val)
{
    return ((uint64_t)ltb32(val & 0xFFFFFFFFUL) << 32) | (uint64_t)ltb32(val >> 32);
}

static inline uint64_t ClearBits(uint64_t src, int offset, int size)
{
    return src & (~(((1ULL << size) - 1ULL) << offset));
}
static inline uint64_t SetBits(int offset, int size, uint64_t value)
{
    return (value & ((1ULL << size) - 1ULL)) << offset;
}
static inline uint64_t SetBitsSigned(int offset, int size, int64_t value)
{
    uint64_t v = value >= 0 ? (uint64_t)value : 1ULL + (((uint64_t)(-value)) ^ (UINT64_MAX >> (64 - size)));
    return SetBits(offset, size, v);
}
static inline uint64_t GetBits(uint64_t src, int offset, int size)
{
    return (uint64_t)((src >> offset) & ((1ULL << size) - 1ULL));
}
static inline int64_t GetBitsSigned(uint64_t src, int offset, int size)
{
    uint64_t v = GetBits(src, offset, size);
    return v & (1ULL << (size - 1)) ? -(int64_t)(1ULL + (v ^ (UINT64_MAX >> (64 - size)))) : (int64_t)v;
}

//====== Init function ======

uint8_t can_node_panel_bus0_init(uint32_t mb_shift, uint32_t rx_id_shift, uint32_t tx_id_shift, volatile t_can_node_panel_bus0_output *out, volatile t_can_node_panel_bus0_input *in)
{
    __mb_shift__    = mb_shift;
    __rx_id_shift__ = rx_id_shift;
    __tx_id_shift__ = tx_id_shift;

    can_node_memset((uint8_t *)out, sizeof(t_can_node_panel_bus0_output));
    can_node_memset((uint8_t *)in, sizeof(t_can_node_panel_bus0_input));
    
    platform_can_init_tx_mb(0, MBN_TX_CTRL_FAN + __mb_shift__, 0x40 + __tx_id_shift__, 8);
    platform_can_init_tx_mb(0, MBN_TX_CTRL_VALVE + __mb_shift__, 0x41 + __tx_id_shift__, 8);
    
    platform_can_init_rx_mb(0, MBN_RX_FAN_STATUS + __mb_shift__, 0x100 + __rx_id_shift__, 8);
    platform_can_init_rx_mb(0, MBN_RX_VALVE_STATUS + __mb_shift__, 0x110 + __rx_id_shift__, 8);
    platform_can_init_rx_mb(0, MBN_RX_POWER_STATUS + __mb_shift__, 0x200 + __rx_id_shift__, 8);
    
    msg_cntr_fan_status = 2 * ALIVE_THRESHOLD;
    msg_cntr_valve_status = 2 * ALIVE_THRESHOLD;
    msg_cntr_power_status = 2 * ALIVE_THRESHOLD;
    
    msg_cntr_ctrl_fan = 0;
    msg_cntr_ctrl_valve = 0;
    return(MBN_NEXT_FREE_BUS_0 );    
}

//====== Receive functions ======

void can_node_panel_bus0_rx(volatile t_can_node_panel_bus0_input *inp)
{
    uint64_t msg;
    
    //FAN_STATUS
    if(platform_can_is_message_arrived(0, MBN_RX_FAN_STATUS + __mb_shift__))
    {
        msg_cntr_fan_status = 0;
        
        msg = platform_can_get_mb_data(0, MBN_RX_FAN_STATUS + __mb_shift__);
        
        inp->FAN_STATUS.FAN_1_ACT = ((uint8_t)GetBits(msg, 0, 8));
        inp->FAN_STATUS.FAN_2_ACT = ((uint8_t)GetBits(msg, 8, 8));
        inp->FAN_STATUS.FAN_3_ACT = ((uint8_t)GetBits(msg, 16, 8));
        inp->FAN_STATUS.FAN_4_ACT = ((uint8_t)GetBits(msg, 24, 8));
        
        can_node_fan_status_cb(0x100 + __rx_id_shift__, msg, 8);
    }
    inp->alive.fan_status = msg_cntr_fan_status >= ALIVE_THRESHOLD * MSG_CYCLE_FAN_STATUS ? 0 : 1;
    
    //VALVE_STATUS
    if(platform_can_is_message_arrived(0, MBN_RX_VALVE_STATUS + __mb_shift__))
    {
        msg_cntr_valve_status = 0;
        
        msg = platform_can_get_mb_data(0, MBN_RX_VALVE_STATUS + __mb_shift__);
        
        inp->VALVE_STATUS.VALVE_1 = ((uint8_t)GetBits(msg, 0, 4));
        inp->VALVE_STATUS.VALVE_2 = ((uint8_t)GetBits(msg, 4, 4));
        inp->VALVE_STATUS.VALVE_3 = ((uint8_t)GetBits(msg, 8, 4));
        inp->VALVE_STATUS.VALVE_4 = ((uint8_t)GetBits(msg, 12, 4));
        inp->VALVE_STATUS.CPU_TEMP = ((int16_t)GetBits(msg, 40, 8) * 1 + -40);
        inp->VALVE_STATUS.V_3V3 = ((float)GetBits(msg, 48, 8) * 0.020000f + 0.000000f);
        
        can_node_valve_status_cb(0x110 + __rx_id_shift__, msg, 8);
    }
    inp->alive.valve_status = msg_cntr_valve_status >= ALIVE_THRESHOLD * MSG_CYCLE_VALVE_STATUS ? 0 : 1;
    
    //POWER_STATUS
    if(platform_can_is_message_arrived(0, MBN_RX_POWER_STATUS + __mb_shift__))
    {
        msg_cntr_power_status = 0;
        
        msg = platform_can_get_mb_data(0, MBN_RX_POWER_STATUS + __mb_shift__);
        
        inp->POWER_STATUS.V_12V = ((float)GetBits(msg, 0, 16) * 0.004000f + 0.000000f);
        inp->POWER_STATUS.I_12V = ((float)GetBitsSigned(msg, 16, 16) * 0.000100f + 0.000000f);
        inp->POWER_STATUS.P_12V = ((float)GetBitsSigned(msg, 32, 16) * 0.001000f + 0.000000f);
        
        can_node_power_status_cb(0x200 + __rx_id_shift__, msg, 8);
    }
    inp->alive.power_status = msg_cntr_power_status >= ALIVE_THRESHOLD * MSG_CYCLE_POWER_STATUS ? 0 : 1;
    
}

//====== Transmit functions ======

void can_node_panel_bus0_tx(volatile t_can_node_panel_bus0_output *out)
{
    uint64_t msg;
    
    //CTRL_FAN
    if((msg_cntr_ctrl_fan >= MSG_CYCLE_CTRL_FAN) || (out->tx_now.ctrl_fan))
    {
        msg_cntr_ctrl_fan -= out->tx_now.ctrl_fan ? msg_cntr_ctrl_fan : MSG_CYCLE_CTRL_FAN;
        out->tx_now.ctrl_fan = 0;
        
        msg = 0;
        
        can_node_ctrl_fan_cb(0x40 + __tx_id_shift__, msg, 8);
        
        //Sinals
        msg |= SetBits(0, 8, (out->CTRL_FAN.FAN_1_REQ));
        msg |= SetBits(8, 8, (out->CTRL_FAN.FAN_2_REQ));
        msg |= SetBits(16, 8, (out->CTRL_FAN.FAN_3_REQ));
        msg |= SetBits(24, 8, (out->CTRL_FAN.FAN_4_REQ));
        
        platform_can_xmit_mb(0, MBN_TX_CTRL_FAN + __mb_shift__, msg);
    }
    //CTRL_VALVE
    if((msg_cntr_ctrl_valve >= MSG_CYCLE_CTRL_VALVE) || (out->tx_now.ctrl_valve))
    {
        msg_cntr_ctrl_valve -= out->tx_now.ctrl_valve ? msg_cntr_ctrl_valve : MSG_CYCLE_CTRL_VALVE;
        out->tx_now.ctrl_valve = 0;
        
        msg = 0;
        
        can_node_ctrl_valve_cb(0x41 + __tx_id_shift__, msg, 8);
        
        //Sinals
        msg |= SetBits(0, 8, (out->CTRL_VALVE.VALVE_1_REQ));
        msg |= SetBits(8, 8, (out->CTRL_VALVE.VALVE_2_REQ));
        msg |= SetBits(16, 8, (out->CTRL_VALVE.VALVE_3_REQ));
        msg |= SetBits(24, 8, (out->CTRL_VALVE.VALVE_4_REQ));
        msg |= SetBits(32, 8, (out->CTRL_VALVE.VALVE_5_REQ));
        msg |= SetBits(40, 8, (out->CTRL_VALVE.VALVE_6_REQ));
        
        platform_can_xmit_mb(0, MBN_TX_CTRL_VALVE + __mb_shift__, msg);
    }
}

//====== Timers functions ======

void can_node_panel_bus0_update_timers(uint32_t time_delta_us)
{
    msg_cntr_fan_status += msg_cntr_fan_status >= ALIVE_THRESHOLD * MSG_CYCLE_FAN_STATUS ? 0 : time_delta_us;
    msg_cntr_valve_status += msg_cntr_valve_status >= ALIVE_THRESHOLD * MSG_CYCLE_VALVE_STATUS ? 0 : time_delta_us;
    msg_cntr_power_status += msg_cntr_power_status >= ALIVE_THRESHOLD * MSG_CYCLE_POWER_STATUS ? 0 : time_delta_us;
    msg_cntr_ctrl_fan += time_delta_us;
    msg_cntr_ctrl_valve += time_delta_us;
}

//====== Weak Planform driver functions declaration ======
__attribute__((weak)) void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc) {}
__attribute__((weak)) void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc) {}
__attribute__((weak)) void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg) {}
__attribute__((weak)) uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn) {return 0;}
__attribute__((weak)) uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn) {return 0;}


//====== Weak Callback functions ======
__attribute__((weak)) void can_node_ctrl_fan_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}
__attribute__((weak)) void can_node_ctrl_valve_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}

__attribute__((weak)) void can_node_ctrl_fan_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}
__attribute__((weak)) void can_node_ctrl_valve_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}

__attribute__((weak)) void can_node_fan_status_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}
__attribute__((weak)) void can_node_valve_status_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}
__attribute__((weak)) void can_node_power_status_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}


