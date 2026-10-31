#include "images.h"

const ext_img_desc_t images[9] = {
    { "Background", &img_background },
    { "presence", &img_presence },
    { "moving", &img_moving },
    { "input_flow", &img_input_flow },
    { "drop", &img_drop },
    { "termometer", &img_termometer },
    { "leaf", &img_leaf },
    { "menu", &img_menu },
    { "detection", &img_detection },
};