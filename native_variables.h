#ifndef NATIVE_VARIABLES_H
#define NATIVE_VARIABLES_H

#include <stdint.h>
#include "src/ui/vars.h"

extern int32_t brightness;
extern int32_t brightness_idle;
extern int32_t idle_timeout;
extern bool idle_is_active;

extern int32_t scenario_id_active;

extern volatile float temperatureSelf;
extern volatile float humiditySelf;
extern char temperature_str[20];
extern char power_str[20];
extern char time_str[10];


// Native global variables
status_color get_var_can_status();
void set_var_can_status(status_color value);
status_color get_var_wifi_status();
void set_var_wifi_status(status_color value);
status_color get_var_bt_status();
void set_var_bt_status(status_color value);
const char *get_var_ip_address();
void set_var_ip_address(const char *value);
const char *get_var_time_str();
void set_var_time_str(const char *value);
const char *get_var_power_str();
void set_var_power_str(const char *value);
const char *get_var_temperature_str();
void set_var_temperature_str(const char *value);
int32_t get_var_supply_fan_speed();
void set_var_supply_fan_speed(int32_t value);
int32_t get_var_exhaust_fan_speed();
void set_var_exhaust_fan_speed(int32_t value);
int32_t get_var_family_room_valve();
void set_var_family_room_valve(int32_t value);
int32_t get_var_master_badroom_valve();
void set_var_master_badroom_valve(int32_t value);
int32_t get_var_children_room_valve();
void set_var_children_room_valve(int32_t value);
int32_t get_var_pantry_room_valve();
void set_var_pantry_room_valve(int32_t value);
int32_t get_var_toilet_exhaust_valve();
void set_var_toilet_exhaust_valve(int32_t value);
int32_t get_var_bathroom_exhaust_valve();
void set_var_bathroom_exhaust_valve(int32_t value);

#endif //NATIVE_VARIABLES_H