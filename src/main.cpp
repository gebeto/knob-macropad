#include <Arduino.h>
#include <lvgl.h>
#include "LCD.h"
#include "InputDevices.h"

void lvglTask(void *pvParameters)
{
  while (true)
  {
    lv_timer_handler();           // Handle LVGL tasks
    vTaskDelay(pdMS_TO_TICKS(5)); // delay 5ms
  }
}

void setup()
{
  Serial.begin(115200);

  lv_init();
  lcd_init();
  init_input_devices();

  lv_disp_t *disp = lv_disp_get_default();
  lv_obj_t *screen = lv_obj_create(NULL);

  lv_obj_t *spinner = lv_spinner_create(screen, 1000, 60);
  lv_obj_set_size(spinner, 240, 240);
  lv_obj_center(spinner);

  globalLabel = lv_label_create(screen);
  lv_obj_set_width(globalLabel, LV_SIZE_CONTENT);
  lv_obj_set_height(globalLabel, LV_SIZE_CONTENT);
  lv_obj_set_x(globalLabel, 0);
  lv_obj_set_y(globalLabel, 0);
  lv_obj_set_align(globalLabel, LV_ALIGN_CENTER);
  lv_label_set_text(globalLabel, "Do something\nto start");
  lv_obj_set_style_text_align(globalLabel, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(globalLabel, &lv_font_montserrat_20, LV_STATE_DEFAULT);

  lv_disp_load_scr(screen);

  xTaskCreatePinnedToCore(lvglTask, "lvglTask", 8192, NULL, 2, NULL, 1);
}

void loop() {}