#include "LCD.h"
#include "Preferences.h"

#define GFX_BL 8

Preferences preferences;
// lv_disp_rot_t orientation = LV_DISP_ROT_270; // LV_DISP_ROT_NONE, LV_DISP_ROT_270
// Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 10 /* CS */, 1 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
// Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, LV_DISP_ROT_270 /* rotation */, true /* IPS */);
Arduino_DataBus *bus;
Arduino_GFX *gfx;

static uint32_t screenWidth = 240;
static uint32_t screenHeight = 240;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}

void LCD_change_rotation()
{
    if (gfx->getRotation() == LV_DISP_ROT_NONE)
    {
        gfx->setRotation(LV_DISP_ROT_270);
    }
    else
    {
        gfx->setRotation(LV_DISP_ROT_NONE);
    }
    preferences.begin("settings", false);
    preferences.putUInt("orientation", gfx->getRotation());
    preferences.end();
}

void lcd_init()
{
    preferences.begin("settings", false);
    bus = new Arduino_ESP32SPI(4 /* DC */, 10 /* CS */, 1 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
    gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, preferences.getUInt("orientation", LV_DISP_ROT_270) /* rotation */, true /* IPS */);
    preferences.end();

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, LOW);
    gfx->begin(80000000);

    disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}
