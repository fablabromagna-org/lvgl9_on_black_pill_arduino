#ifndef EEZ_LVGL_UI_FONTS_H
#define EEZ_LVGL_UI_FONTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXT_FONT_DESC_T
#define EXT_FONT_DESC_T
typedef struct _ext_font_desc_t {
    const char *name;
    const void *font_ptr;
} ext_font_desc_t;
#endif

extern ext_font_desc_t fonts[];

#if !defined(LV_FONT_MONTSERRAT_20) || (LV_FONT_MONTSERRAT_20 == 0)
#define lv_font_montserrat_20 lv_font_montserrat_14
#endif

#if !defined(LV_FONT_MONTSERRAT_24) || (LV_FONT_MONTSERRAT_24 == 0)
#define lv_font_montserrat_24 lv_font_montserrat_14
#endif

#if !defined(LV_FONT_MONTSERRAT_28) || (LV_FONT_MONTSERRAT_28 == 0)
#define lv_font_montserrat_28 lv_font_montserrat_14
#endif

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_FONTS_H*/
