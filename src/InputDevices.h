#include <Arduino.h>
#include <lvgl.h>
#include <ESP_Knob.h>
#include <Button.h>

extern lv_indev_t* encoder_indev;
extern lv_obj_t *globalLabel;
void encoder_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);
void init_input_devices();
