#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETTINGS = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *settings;
    lv_obj_t *status_bar;
    lv_obj_t *status_bar__time;
    lv_obj_t *status_bar__can;
    lv_obj_t *status_bar__wi_fi;
    lv_obj_t *status_bar__bluetooth;
    lv_obj_t *status_bar__temperature;
    lv_obj_t *status_bar__humidity;
    lv_obj_t *status_bar__power_param;
    lv_obj_t *background_image;
    lv_obj_t *obj0;
    lv_obj_t *valve1;
    lv_obj_t *obj1;
    lv_obj_t *valve2;
    lv_obj_t *obj2;
    lv_obj_t *valve3;
    lv_obj_t *obj3;
    lv_obj_t *valve4;
    lv_obj_t *obj4;
    lv_obj_t *valve5;
    lv_obj_t *obj5;
    lv_obj_t *valve6;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *ssid_text;
    lv_obj_t *obj8;
    lv_obj_t *password_text;
    lv_obj_t *obj9;
    lv_obj_t *conect_wifi;
    lv_obj_t *obj10;
    lv_obj_t *ip_address;
    lv_obj_t *power_data_hiden;
    lv_obj_t *keyboard;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_settings();
void tick_screen_settings();

void create_user_widget_status_bar(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_status_bar(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/