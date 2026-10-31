#include "native_variables.h"

const char *get_var_family_room_aqi_str() {
    return "3";
}

void set_var_family_room_aqi_str(const char *value) {}

const char *get_var_family_room_temp_str() {
    return "20.0°C";
}

void set_var_family_room_temp_str(const char *value) {}

const char *get_var_family_room_humid_str() {
    return "68%";
}

void set_var_family_room_humid_str(const char *value) {}

const char *get_var_family_room_tvoc_str() {
    return "12345";
}

void set_var_family_room_tvoc_str(const char *value) {
    // strncpy(family_room_tvoc_str, value, sizeof(family_room_tvoc_str) / sizeof(char));
    // family_room_tvoc_str[sizeof(family_room_tvoc_str) / sizeof(char) - 1] = 0;
}

const char *get_var_family_room_eco2_str() {
    return "450";
}

void set_var_family_room_eco2_str(const char *value) {
    // strncpy(family_room_eco2_str, value, sizeof(family_room_eco2_str) / sizeof(char));
    // family_room_eco2_str[sizeof(family_room_eco2_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_aqi_str() {
    return "1";
}

void set_var_master_bedroom_aqi_str(const char *value) {
    // strncpy(master_bedroom_aqi_str, value, sizeof(master_bedroom_aqi_str) / sizeof(char));
    // master_bedroom_aqi_str[sizeof(master_bedroom_aqi_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_temp_str() {
    return "21.2°C";
}

void set_var_master_bedroom_temp_str(const char *value) {
    // strncpy(master_bedroom_temp_str, value, sizeof(master_bedroom_temp_str) / sizeof(char));
    // master_bedroom_temp_str[sizeof(master_bedroom_temp_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_humid_str() {
    return "51%";
}

void set_var_master_bedroom_humid_str(const char *value) {
    // strncpy(master_bedroom_humid_str, value, sizeof(master_bedroom_humid_str) / sizeof(char));
    // master_bedroom_humid_str[sizeof(master_bedroom_humid_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_tvoc_str() {
    return "666";
}

void set_var_master_bedroom_tvoc_str(const char *value) {
    // strncpy(master_bedroom_tvoc_str, value, sizeof(master_bedroom_tvoc_str) / sizeof(char));
    // master_bedroom_tvoc_str[sizeof(master_bedroom_tvoc_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_eco2_str() {
    return "555";
}

void set_var_master_bedroom_eco2_str(const char *value) {
    // strncpy(master_bedroom_eco2_str, value, sizeof(master_bedroom_eco2_str) / sizeof(char));
    // master_bedroom_eco2_str[sizeof(master_bedroom_eco2_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_aqi_str() {
    return "2";
}

void set_var_childroom_aqi_str(const char *value) {
    // strncpy(childroom_aqi_str, value, sizeof(childroom_aqi_str) / sizeof(char));
    // childroom_aqi_str[sizeof(childroom_aqi_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_temp_str() {
    return "-30.0°C";
}

void set_var_childroom_temp_str(const char *value) {
    // strncpy(childroom_temp_str, value, sizeof(childroom_temp_str) / sizeof(char));
    // childroom_temp_str[sizeof(childroom_temp_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_humid_str() {
    return "100%";
}

void set_var_childroom_humid_str(const char *value) {
    // strncpy(childroom_humid_str, value, sizeof(childroom_humid_str) / sizeof(char));
    // childroom_humid_str[sizeof(childroom_humid_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_tvoc_str() {
    return "65500";
}

void set_var_childroom_tvoc_str(const char *value) {
    // strncpy(childroom_tvoc_str, value, sizeof(childroom_tvoc_str) / sizeof(char));
    // childroom_tvoc_str[sizeof(childroom_tvoc_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_eco2_str() {
    return "10";
}

void set_var_childroom_eco2_str(const char *value) {
    // strncpy(childroom_eco2_str, value, sizeof(childroom_eco2_str) / sizeof(char));
    // childroom_eco2_str[sizeof(childroom_eco2_str) / sizeof(char) - 1] = 0;
}

const char *get_var_family_room_flow_str() {
    return "25 м³/ч";
}

void set_var_family_room_flow_str(const char *value) {
    // strncpy(family_room_flow_str, value, sizeof(family_room_flow_str) / sizeof(char));
    // family_room_flow_str[sizeof(family_room_flow_str) / sizeof(char) - 1] = 0;
}

const char *get_var_master_bedroom_flow_str() {
    return "5 м³/ч";
}

void set_var_master_bedroom_flow_str(const char *value) {
    // strncpy(master_bedroom_flow_str, value, sizeof(master_bedroom_flow_str) / sizeof(char));
    // master_bedroom_flow_str[sizeof(master_bedroom_flow_str) / sizeof(char) - 1] = 0;
}

const char *get_var_childroom_flow_str() {
    return "15 м³/ч";
}

void set_var_childroom_flow_str(const char *value) {
    // strncpy(childroom_flow_str, value, sizeof(childroom_flow_str) / sizeof(char));
    // childroom_flow_str[sizeof(childroom_flow_str) / sizeof(char) - 1] = 0;
}

human_presence get_var_family_room_moving() {
    return human_presence_MOVING;
}

void set_var_family_room_moving(human_presence value) {
    // family_room_moving = value;
}

human_presence get_var_master_bedroom_moving() {
    return human_presence_NONE;
}

void set_var_master_bedroom_moving(human_presence value) {
    // master_bedroom_moving = value;
}

human_presence get_var_childroom_moving() {
    return human_presence_PRESENCE;
}

void set_var_childroom_moving(human_presence value) {
    // childroom_moving = value;
}
