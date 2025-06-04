#include "InputDevices.h"
#include <Arduino.h>

#define ROTARY_ENCODER_A_PIN 6
#define ROTARY_ENCODER_B_PIN 7
#define ROTARY_ENCODER_BUTTON_PIN GPIO_NUM_9

lv_obj_t *globalLabel;

static int32_t prev_encoder_value = 0;
static int32_t encoder_value = 0;
static int32_t encoder_diff = 0;
bool buttonPressed = false;
bool buttonClicked = false;

ESP_Knob *knob;

void onKnobLeftEventCallback(int count, void *usr_data)
{
    prev_encoder_value = encoder_value;
    encoder_value = count;
    encoder_diff = encoder_value - prev_encoder_value;
    if (buttonPressed) {
        lv_label_set_text(globalLabel, "Pressed\nTurned\nLeft");
    } else {
        lv_label_set_text(globalLabel, "Turned\nLeft");
    }
}

void onKnobRightEventCallback(int count, void *usr_data)
{
    prev_encoder_value = encoder_value;
    encoder_value = count;
    encoder_diff = encoder_value - prev_encoder_value;
    if (buttonPressed) {
        lv_label_set_text(globalLabel, "Pressed\nTurned\nRight");
    } else {
        lv_label_set_text(globalLabel, "Turned\nRight");
    }
}

static void onButtonPressedCb(void *button_handle, void *usr_data)
{
    buttonPressed = true;
    lv_label_set_text(globalLabel, "Button\nPressed");
}

static void onButtonReleasedCb(void *button_handle, void *usr_data)
{
    buttonPressed = false;
    lv_label_set_text(globalLabel, "Button\nReleased");
}

lv_indev_t *encoder_indev;

void encoder_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    data->enc_diff = encoder_diff;
    encoder_diff = 0;
    data->state = buttonPressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

void init_input_devices()
{
    knob = new ESP_Knob(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
    knob->begin();
    knob->attachLeftEventCallback(onKnobLeftEventCallback);
    knob->attachRightEventCallback(onKnobRightEventCallback);

    Button *btn = new Button(ROTARY_ENCODER_BUTTON_PIN, false);
    btn->attachPressDownEventCb(&onButtonPressedCb, NULL);
    btn->attachPressUpEventCb(&onButtonReleasedCb, NULL);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.long_press_time = 500;
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read_cb;
    encoder_indev = lv_indev_drv_register(&indev_drv);
}