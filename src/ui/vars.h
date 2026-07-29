#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

typedef enum {
    status_color_GREY = 0,
    status_color_GREEN = 1,
    status_color_YELLOW = 2,
    status_color_RED = 3,
    status_color_BLUE = 4,
    status_color_WHITE = 5,
    status_color_CLEAR = 6
} status_color;

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_POWER_IS_VISIBLE = 0
};

// Native global variables

extern status_color get_var_can_status();
extern void set_var_can_status(status_color value);
extern status_color get_var_wifi_status();
extern void set_var_wifi_status(status_color value);
extern status_color get_var_bt_status();
extern void set_var_bt_status(status_color value);
extern const char *get_var_ip_address();
extern void set_var_ip_address(const char *value);
extern const char *get_var_temperature_str();
extern void set_var_temperature_str(const char *value);
extern const char *get_var_power_str();
extern void set_var_power_str(const char *value);
extern const char *get_var_time_str();
extern void set_var_time_str(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/