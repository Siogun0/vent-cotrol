#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: label_green
//

void init_style_label_green_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x24e305));
};

lv_style_t *get_style_label_green_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_green_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_green(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_label_green_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_green(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_label_green_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: label_red
//

void init_style_label_red_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xff0000));
};

lv_style_t *get_style_label_red_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_red_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_red(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_label_red_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_red(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_label_red_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: label_yellow
//

void init_style_label_yellow_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xfbff00));
};

lv_style_t *get_style_label_yellow_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_yellow_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_yellow(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_label_yellow_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_yellow(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_label_yellow_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: label_grey
//

void init_style_label_grey_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xa2a2a2));
};

lv_style_t *get_style_label_grey_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_grey_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_grey(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_label_grey_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_grey(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_label_grey_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_label_green,
        add_style_label_red,
        add_style_label_yellow,
        add_style_label_grey,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_label_green,
        remove_style_label_red,
        remove_style_label_yellow,
        remove_style_label_grey,
    };
    remove_style_funcs[styleIndex](obj);
}