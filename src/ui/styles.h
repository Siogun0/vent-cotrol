#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: label_green
lv_style_t *get_style_label_green_MAIN_DEFAULT();
void add_style_label_green(lv_obj_t *obj);
void remove_style_label_green(lv_obj_t *obj);

// Style: label_red
lv_style_t *get_style_label_red_MAIN_DEFAULT();
void add_style_label_red(lv_obj_t *obj);
void remove_style_label_red(lv_obj_t *obj);

// Style: label_yellow
lv_style_t *get_style_label_yellow_MAIN_DEFAULT();
void add_style_label_yellow(lv_obj_t *obj);
void remove_style_label_yellow(lv_obj_t *obj);

// Style: label_grey
lv_style_t *get_style_label_grey_MAIN_DEFAULT();
void add_style_label_grey(lv_obj_t *obj);
void remove_style_label_grey(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/