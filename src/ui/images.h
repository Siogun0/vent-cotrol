#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_background;
extern const lv_img_dsc_t img_presence;
extern const lv_img_dsc_t img_moving;
extern const lv_img_dsc_t img_input_flow;
extern const lv_img_dsc_t img_drop;
extern const lv_img_dsc_t img_termometer;
extern const lv_img_dsc_t img_leaf;
extern const lv_img_dsc_t img_menu;
extern const lv_img_dsc_t img_detection;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[9];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/