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
    SCREEN_ID_TOP_LAYER_SCREEN = 2,
    SCREEN_ID_SETTINGS = 3,
    _SCREEN_ID_LAST = 3
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *top_layer_screen;
    lv_obj_t *settings;
    lv_obj_t *obj0;
    lv_obj_t *obj0__time;
    lv_obj_t *obj0__can;
    lv_obj_t *obj0__wi_fi;
    lv_obj_t *obj0__bluetooth;
    lv_obj_t *obj0__temperature;
    lv_obj_t *obj0__degree;
    lv_obj_t *obj0__humidity;
    lv_obj_t *obj1;
    lv_obj_t *obj1__time;
    lv_obj_t *obj1__can;
    lv_obj_t *obj1__wi_fi;
    lv_obj_t *obj1__bluetooth;
    lv_obj_t *obj1__temperature;
    lv_obj_t *obj1__degree;
    lv_obj_t *obj1__humidity;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *ssid_text;
    lv_obj_t *obj4;
    lv_obj_t *password_text;
    lv_obj_t *conect_wifi;
    lv_obj_t *obj5;
    lv_obj_t *keyboard;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_top_layer_screen();
void tick_screen_top_layer_screen();

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