#ifndef NATIVE_VARIABLES_H
#define NATIVE_VARIABLES_H

#include <stdint.h>
#include "src/ui/vars.h"

extern int32_t brightness;
extern int32_t brightness_idle;
extern int32_t idle_timeout;
extern bool idle_is_active;

extern int32_t scenario_id_active;

const char *get_var_family_room_aqi_str();
void set_var_family_room_aqi_str(const char *value);
const char *get_var_family_room_temp_str();
void set_var_family_room_temp_str(const char *value);
const char *get_var_family_room_humid_str();
void set_var_family_room_humid_str(const char *value);
const char *get_var_family_room_tvoc_str();
void set_var_family_room_tvoc_str(const char *value);
const char *get_var_family_room_eco2_str();
void set_var_family_room_eco2_str(const char *value);
const char *get_var_master_bedroom_aqi_str();
void set_var_master_bedroom_aqi_str(const char *value);
const char *get_var_master_bedroom_temp_str();
void set_var_master_bedroom_temp_str(const char *value);
const char *get_var_master_bedroom_humid_str();
void set_var_master_bedroom_humid_str(const char *value);
const char *get_var_master_bedroom_tvoc_str();
void set_var_master_bedroom_tvoc_str(const char *value);
const char *get_var_master_bedroom_eco2_str();
void set_var_master_bedroom_eco2_str(const char *value);
const char *get_var_childroom_aqi_str();
void set_var_childroom_aqi_str(const char *value);
const char *get_var_childroom_temp_str();
void set_var_childroom_temp_str(const char *value);
const char *get_var_childroom_humid_str();
void set_var_childroom_humid_str(const char *value);
const char *get_var_childroom_tvoc_str();
void set_var_childroom_tvoc_str(const char *value);
const char *get_var_childroom_eco2_str();
void set_var_childroom_eco2_str(const char *value);
const char *get_var_family_room_flow_str();
void set_var_family_room_flow_str(const char *value);
const char *get_var_master_bedroom_flow_str();
void set_var_master_bedroom_flow_str(const char *value);
const char *get_var_childroom_flow_str();
void set_var_childroom_flow_str(const char *value);
human_presence get_var_family_room_moving();
void set_var_family_room_moving(human_presence value);
human_presence get_var_master_bedroom_moving();
void set_var_master_bedroom_moving(human_presence value);
human_presence get_var_childroom_moving();
void set_var_childroom_moving(human_presence value);
#endif //NATIVE_VARIABLES_H