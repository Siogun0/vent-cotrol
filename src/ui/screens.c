#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

static const char *screen_names[] = { "Manual", "Main", "Settings", "Idle" };
static const char *object_names[] = { "manual", "main", "settings", "idle", "status_bar", "status_bar__obj0", "status_bar__time", "status_bar__can", "status_bar__wi_fi", "status_bar__bluetooth", "status_bar__temperature", "status_bar__power_param", "background_image", "input_flow", "valve1", "obj0", "valve2", "obj1", "valve3", "obj2", "valve4", "obj3", "output_flow", "obj4", "obj5", "valve5", "valve6", "fans", "fans_common_speed", "obj6", "fans_common_speed_1", "obj7", "obj8", "obj9", "ssid_text", "obj10", "password_text", "obj11", "conect_wifi", "ip_address", "power_data_hiden", "brightnes_slider", "brightnes_idle_slider", "idle_timeout", "reset_button", "keyboard", "numboard", "modal_reset_overlay", "obj12", "obj13", "obj14", "idle_time", "idle_temperature" };

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

static void event_handler_cb_manual_manual(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_GESTURE) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 1, 0, e);
    }
}

static void event_handler_cb_manual_valve1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 5, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_valve2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 7, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_valve3(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 9, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_valve4(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 11, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_valve5(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 17, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_valve6(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 18, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_fans_common_speed(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 21, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_manual_fans_common_speed_1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 23, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_GESTURE) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 1, 0, e);
    }
}

static void event_handler_cb_settings_settings(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_GESTURE) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 1, 0, e);
    }
}

static void event_handler_cb_settings_ssid_text(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            assignStringProperty(flowState, 2, 3, value, "Failed to assign Text in Textarea widget");
        }
    }
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 2, 0, e);
    }
}

static void event_handler_cb_settings_password_text(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            assignStringProperty(flowState, 4, 3, value, "Failed to assign Text in Textarea widget");
        }
    }
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 4, 0, e);
    }
}

static void event_handler_cb_settings_obj11(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        action_connect_wifi(e);
    }
}

static void event_handler_cb_settings_conect_wifi(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        action_connect_wifi(e);
    }
}

static void event_handler_cb_settings_power_data_hiden(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 8, 0, e);
    }
}

static void event_handler_cb_settings_brightnes_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 11, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_settings_brightnes_idle_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            assignIntegerProperty(flowState, 12, 3, value, "Failed to assign Value in Slider widget");
        }
    }
}

static void event_handler_cb_settings_idle_timeout(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            assignStringProperty(flowState, 14, 3, value, "Failed to assign Text in Textarea widget");
        }
    }
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 14, 0, e);
    }
}

static void event_handler_cb_settings_reset_button(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_LONG_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 15, 0, e);
    }
}

static void event_handler_cb_settings_keyboard(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CANCEL) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 17, 0, e);
    }
    if (event == LV_EVENT_READY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 17, 1, e);
    }
}

static void event_handler_cb_settings_numboard(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CANCEL) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 18, 0, e);
    }
    if (event == LV_EVENT_READY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 18, 1, e);
    }
}

static void event_handler_cb_settings_obj13(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 22, 0, e);
    }
}

static void event_handler_cb_settings_obj14(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        action_reset_request(e);
    }
}

static void event_handler_cb_idle_idle(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 1, 0, e);
    }
}

//
// Screens
//

void create_screen_manual() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.manual = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_manual_manual, LV_EVENT_ALL, flowState);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // BackgroundImage
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.background_image = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_background);
        }
        {
            // StatusBar
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.status_bar = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 40);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_status_bar(obj, getFlowState(flowState, 2), 5);
        }
        {
            // InputFlow
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.input_flow = obj;
            lv_obj_set_pos(obj, 10, 171);
            lv_obj_set_size(obj, 460, 210);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x0088ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_width(obj, 460, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radial_offset(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 118, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_grey(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Приточные клапана");
                }
                {
                    // Valve1
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve1 = obj;
                    lv_obj_set_pos(obj, 119, 45);
                    lv_obj_set_size(obj, 325, 10);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve1, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 15, 40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Гостинная");
                }
                {
                    // Valve2
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve2 = obj;
                    lv_obj_set_pos(obj, 119, 90);
                    lv_obj_set_size(obj, 325, 10);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve2, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 15, 81);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Спальня");
                }
                {
                    // Valve3
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve3 = obj;
                    lv_obj_set_pos(obj, 119, 135);
                    lv_obj_set_size(obj, 325, 10);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve3, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_pos(obj, 15, 123);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Детская");
                }
                {
                    // Valve4
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve4 = obj;
                    lv_obj_set_pos(obj, 118, 180);
                    lv_obj_set_size(obj, 325, 10);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve4, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj3 = obj;
                    lv_obj_set_pos(obj, 15, 163);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Кладовка");
                }
            }
        }
        {
            // OutputFlow
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.output_flow = obj;
            lv_obj_set_pos(obj, 10, 389);
            lv_obj_set_size(obj, 460, 81);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x0088ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_width(obj, 460, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radial_offset(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj4 = obj;
                    lv_obj_set_pos(obj, 15, 42);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Туалет");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 114, 7);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_grey(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Вытяжные клапана");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj5 = obj;
                    lv_obj_set_pos(obj, 251, 42);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Ванная");
                }
                {
                    // Valve5
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve5 = obj;
                    lv_obj_set_pos(obj, 114, 49);
                    lv_obj_set_size(obj, 85, 10);
                    lv_slider_set_range(obj, 0, 1);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve5, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    // Valve6
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.valve6 = obj;
                    lv_obj_set_pos(obj, 359, 49);
                    lv_obj_set_size(obj, 85, 10);
                    lv_slider_set_range(obj, 0, 1);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_valve6, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
            }
        }
        {
            // Fans
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.fans = obj;
            lv_obj_set_pos(obj, 10, 40);
            lv_obj_set_size(obj, 460, 123);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x0088ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_width(obj, 460, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radial_offset(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 114, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_grey(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Вентиляторы");
                }
                {
                    // Fans_Common_Speed
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.fans_common_speed = obj;
                    lv_obj_set_pos(obj, 119, 45);
                    lv_obj_set_size(obj, 325, 10);
                    lv_slider_set_range(obj, 0, 4);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_fans_common_speed, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj6 = obj;
                    lv_obj_set_pos(obj, 15, 40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Приток");
                }
                {
                    // Fans_Common_Speed_1
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.fans_common_speed_1 = obj;
                    lv_obj_set_pos(obj, 119, 90);
                    lv_obj_set_size(obj, 325, 10);
                    lv_slider_set_range(obj, 0, 4);
                    lv_obj_add_event_cb(obj, event_handler_cb_manual_fans_common_speed_1, LV_EVENT_ALL, flowState);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj7 = obj;
                    lv_obj_set_pos(obj, 15, 80);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Вытяжка");
                }
            }
        }
    }
    
    tick_screen_manual();
}

void tick_screen_manual() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    tick_user_widget_status_bar(getFlowState(flowState, 2), 5);
    {
        int32_t new_val = evalIntegerProperty(flowState, 5, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve1;
            lv_slider_set_value(objects.valve1, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 7, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve2);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve2;
            lv_slider_set_value(objects.valve2, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 9, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve3);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve3;
            lv_slider_set_value(objects.valve3, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 11, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve4);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve4;
            lv_slider_set_value(objects.valve4, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 17, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve5);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve5;
            lv_slider_set_value(objects.valve5, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 18, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.valve6);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.valve6;
            lv_slider_set_value(objects.valve6, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 21, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.fans_common_speed);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.fans_common_speed;
            lv_slider_set_value(objects.fans_common_speed, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 23, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.fans_common_speed_1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.fans_common_speed_1;
            lv_slider_set_value(objects.fans_common_speed_1, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, flowState);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.obj8 = obj;
            lv_obj_set_pos(obj, 86, 61);
            lv_obj_set_size(obj, 308, LV_SIZE_CONTENT);
            lv_dropdown_set_options_static(obj, "Ручная настройка\nНикого нет\nТолько спальня\nДве спальни\nТолько гостинная\nГостинная и спальни\nМаксимально");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x858080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_SELECTED | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
}

void create_screen_settings() {
    void *flowState = getFlowState(0, 2);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_settings_settings, LV_EVENT_ALL, flowState);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000063), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj9 = obj;
            lv_obj_set_pos(obj, 15, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "SSID");
        }
        {
            // SSID_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.ssid_text = obj;
            lv_obj_set_pos(obj, 102, 36);
            lv_obj_set_size(obj, 160, 48);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_ssid_text, LV_EVENT_ALL, flowState);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj10 = obj;
            lv_obj_set_pos(obj, 15, 96);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Password");
        }
        {
            // Password_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.password_text = obj;
            lv_obj_set_pos(obj, 102, 84);
            lv_obj_set_size(obj, 160, 48);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_password_text, LV_EVENT_ALL, flowState);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.obj11 = obj;
            lv_obj_set_pos(obj, 323, 77);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj11, LV_EVENT_ALL, flowState);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ConectWifi
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.conect_wifi = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_conect_wifi, LV_EVENT_ALL, flowState);
                    add_style_label_white(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Connect");
                }
            }
        }
        {
            // IP_address
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ip_address = obj;
            lv_obj_set_pos(obj, 133, -180);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "0.0.0.0");
        }
        {
            // power_data_hiden
            lv_obj_t *obj = lv_checkbox_create(parent_obj);
            objects.power_data_hiden = obj;
            lv_obj_set_pos(obj, 16, 440);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_checkbox_set_text_static(obj, "Power data in status bar");
            lv_obj_add_event_cb(obj, event_handler_cb_settings_power_data_hiden, LV_EVENT_ALL, flowState);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 144);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Яркость экрана");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 190);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Яркость часов");
        }
        {
            // brightnes_slider
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.brightnes_slider = obj;
            lv_obj_set_pos(obj, 173, 151);
            lv_obj_set_size(obj, 250, 10);
            lv_slider_set_range(obj, 50, 255);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_brightnes_slider, LV_EVENT_ALL, flowState);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
        }
        {
            // brightnes_idle_slider
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.brightnes_idle_slider = obj;
            lv_obj_set_pos(obj, 174, 193);
            lv_obj_set_size(obj, 250, 10);
            lv_slider_set_range(obj, 50, 255);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_brightnes_idle_slider, LV_EVENT_ALL, flowState);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 241);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Время бездействия, с");
        }
        {
            // idle_timeout
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.idle_timeout = obj;
            lv_obj_set_pos(obj, 211, 229);
            lv_obj_set_size(obj, 150, 48);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_idle_timeout, LV_EVENT_ALL, flowState);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Reset_button
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.reset_button = obj;
            lv_obj_set_pos(obj, 323, 410);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_reset_button, LV_EVENT_ALL, flowState);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_label_white(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Reset");
                }
            }
        }
        {
            // Keyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard = obj;
            lv_obj_set_pos(obj, -2, 318);
            lv_obj_set_size(obj, 482, 162);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_keyboard, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Numboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.numboard = obj;
            lv_obj_set_pos(obj, 0, 318);
            lv_obj_set_size(obj, 480, 162);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, event_handler_cb_settings_numboard, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // modal_reset_overlay
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.modal_reset_overlay = obj;
            lv_obj_set_pos(obj, -1, 0);
            lv_obj_set_size(obj, 481, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj12 = obj;
                    lv_obj_set_pos(obj, 92, 144);
                    lv_obj_set_size(obj, 300, 200);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x05135f), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 95, 24);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_label_yellow(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Перегрузить?");
                        }
                        {
                            lv_obj_t *obj = lv_button_create(parent_obj);
                            objects.obj13 = obj;
                            lv_obj_set_pos(obj, 171, 124);
                            lv_obj_set_size(obj, 100, 50);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj13, LV_EVENT_ALL, flowState);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text_static(obj, "Отмена");
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_button_create(parent_obj);
                            objects.obj14 = obj;
                            lv_obj_set_pos(obj, 27, 124);
                            lv_obj_set_size(obj, 100, 50);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_obj14, LV_EVENT_ALL, flowState);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text_static(obj, "Да");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.keyboard, objects.ssid_text);
    lv_keyboard_set_textarea(objects.numboard, objects.idle_timeout);
    
    tick_screen_settings();
}

void tick_screen_settings() {
    void *flowState = getFlowState(0, 2);
    (void)flowState;
    {
        const char *new_val = evalTextProperty(flowState, 2, 3, "Failed to evaluate Text in Textarea widget");
        const char *cur_val = lv_textarea_get_text(objects.ssid_text);
        uint32_t max_length = lv_textarea_get_max_length(objects.ssid_text);
        if (strncmp(new_val, cur_val, max_length) != 0) {
            tick_value_change_obj = objects.ssid_text;
            lv_textarea_set_text(objects.ssid_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 4, 3, "Failed to evaluate Text in Textarea widget");
        const char *cur_val = lv_textarea_get_text(objects.password_text);
        uint32_t max_length = lv_textarea_get_max_length(objects.password_text);
        if (strncmp(new_val, cur_val, max_length) != 0) {
            tick_value_change_obj = objects.password_text;
            lv_textarea_set_text(objects.password_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 11, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.brightnes_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.brightnes_slider;
            lv_slider_set_value(objects.brightnes_slider, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 12, 3, "Failed to evaluate Value in Slider widget");
        int32_t cur_val = lv_slider_get_value(objects.brightnes_idle_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.brightnes_idle_slider;
            lv_slider_set_value(objects.brightnes_idle_slider, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 14, 3, "Failed to evaluate Text in Textarea widget");
        const char *cur_val = lv_textarea_get_text(objects.idle_timeout);
        uint32_t max_length = lv_textarea_get_max_length(objects.idle_timeout);
        if (strncmp(new_val, cur_val, max_length) != 0) {
            tick_value_change_obj = objects.idle_timeout;
            lv_textarea_set_text(objects.idle_timeout, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_idle() {
    void *flowState = getFlowState(0, 3);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.idle = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_idle_idle, LV_EVENT_ALL, flowState);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // idle_time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.idle_time = obj;
            lv_obj_set_pos(obj, 0, -65);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_black_150, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // idle_temperature
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.idle_temperature = obj;
            lv_obj_set_pos(obj, 0, 121);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_white(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_black_80, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
    }
    
    tick_screen_idle();
}

void tick_screen_idle() {
    void *flowState = getFlowState(0, 3);
    (void)flowState;
    {
        const char *new_val = evalTextProperty(flowState, 0, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.idle_time);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.idle_time;
            lv_label_set_text(objects.idle_time, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 2, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.idle_temperature);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.idle_temperature;
            lv_label_set_text(objects.idle_temperature, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_status_bar(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex) {
    (void)flowState;
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 40);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x080f4a), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Time
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 426, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "23:57");
        }
        {
            // Can
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 380, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_grey(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "CAN");
        }
        {
            // WiFi
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 329, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_grey(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Wi-Fi");
        }
        {
            // Bluetooth
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
            lv_obj_set_pos(obj, 298, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_label_grey(obj);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "BT");
        }
        {
            // Temperature
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
            lv_obj_set_pos(obj, 5, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "-12.5°C 68%");
        }
        {
            // PowerParam
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 6] = obj;
            lv_obj_set_pos(obj, 122, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_condensed_semi_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "12.5V  0.25A  2.0W");
        }
    }
}

void tick_user_widget_status_bar(void *flowState, int startWidgetIndex) {
    (void)flowState;
    (void)startWidgetIndex;
    {
        bool new_val = evalBooleanProperty(flowState, 6, 3, "Failed to evaluate Hidden flag");
        bool cur_val = lv_obj_has_flag(((lv_obj_t **)&objects)[startWidgetIndex + 6], LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 6];
            if (new_val) {
                lv_obj_add_flag(((lv_obj_t **)&objects)[startWidgetIndex + 6], LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_remove_flag(((lv_obj_t **)&objects)[startWidgetIndex + 6], LV_OBJ_FLAG_HIDDEN);
            }
            tick_value_change_obj = NULL;
        }
    }
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_manual,
    tick_screen_main,
    tick_screen_settings,
    tick_screen_idle,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 4) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Styles
//

static const char *style_names[] = { "label_green", "label_red", "label_yellow", "label_grey", "label_white" };

extern void add_style(lv_obj_t *obj, int32_t styleIndex);
extern void remove_style(lv_obj_t *obj, int32_t styleIndex);

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "Roboto-condensed-semiBold_20", &ui_font_roboto_condensed_semi_bold_20 },
    { "Roboto_black_150", &ui_font_roboto_black_150 },
    { "Roboto_black_80", &ui_font_roboto_black_80 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
//
//

void create_screens() {
    // Initialize styles
    eez_flow_init_styles(add_style, remove_style);
    eez_flow_init_style_names(style_names, sizeof(style_names) / sizeof(const char *));

eez_flow_init_fonts(fonts, sizeof(fonts) / sizeof(ext_font_desc_t));

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    // Create screens
    create_screen_manual();
    create_screen_main();
    create_screen_settings();
    create_screen_idle();
}