#include "LCDController.h"

#include "board_config.h"
#include "ILI9341.h"

namespace {

constexpr size_t kBufferRows = 10;

lv_display_t *g_display = nullptr;
bool g_flush_enabled = true;

void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    if (!g_flush_enabled) {
        lv_display_flush_ready(disp);
        return;
    }

    const uint16_t width = static_cast<uint16_t>(area->x2 - area->x1 + 1);
    const uint16_t height = static_cast<uint16_t>(area->y2 - area->y1 + 1);

    ILI9341_SetWindow(area->x1, area->y1, area->x2, area->y2);
    ILI9341_DrawBitmapDMA(width, height, px_map, disp);
}

}  // namespace

void lv_port_disp_init(const ILI9341_Config_t *config) {
    const uint16_t hor_res = Board_GetDisplayHorRes();
    const uint16_t ver_res = Board_GetDisplayVerRes();

    ILI9341_Init(config);
    ILI9341_SetRotation(Board_GetDisplayRotation());

    g_display = lv_display_create(hor_res, ver_res);
    lv_display_set_flush_cb(g_display, disp_flush);

    LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_a[BOARD_TFT_HOR_RES * kBufferRows * 2];
    LV_ATTRIBUTE_MEM_ALIGN static uint8_t buf_b[BOARD_TFT_HOR_RES * kBufferRows * 2];
    lv_display_set_buffers(g_display, buf_a, buf_b, sizeof(buf_a), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

void disp_enable_update(void) {
    g_flush_enabled = true;
}

void disp_disable_update(void) {
    g_flush_enabled = false;
}
