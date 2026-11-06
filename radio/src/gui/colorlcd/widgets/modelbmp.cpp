/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <memory>

#include "widget.h"

#include "edgetx.h"
#include "static.h"

#define ETX_STATE_BG_FILL LV_STATE_USER_1

class ModelBitmapWidget : public Widget
{
 public:
  ModelBitmapWidget(const WidgetFactory* factory, Window* parent, const rect_t& rect,
                    int screenNum, int zoneNum) :
      Widget(factory, parent, rect, screenNum, zoneNum)
  {
    padAll(PAD_ZERO);
    etx_obj_add_style(lvobj, styles->bg_opacity_transparent, LV_PART_MAIN);
    etx_obj_add_style(lvobj, styles->bg_opacity_cover,
                      LV_PART_MAIN | ETX_STATE_BG_FILL);
    lv_obj_set_style_border_side(lvobj, LV_BORDER_SIDE_FULL, LV_PART_MAIN);
    lv_obj_set_style_border_width(lvobj, 1, LV_PART_MAIN);

    delayLoad();
  }

  void delayedInit() override
  {
    titleBar = new Window(this, {0, 0, width(), TITLE_HEIGHT});
    titleBar->setWindowFlag(NO_FOCUS | NO_CLICK);
    lv_obj_set_style_bg_opa(titleBar->getLvObj(), LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_side(titleBar->getLvObj(), LV_BORDER_SIDE_BOTTOM,
                                 LV_PART_MAIN);
    lv_obj_set_style_border_width(titleBar->getLvObj(), 1, LV_PART_MAIN);
    titleLabel = new StaticText(
        titleBar,
        {PAD_SMALL, 0, width() - TITLE_HEIGHT - PAD_SMALL, TITLE_HEIGHT},
        "DRONE RENDER_", COLOR_THEME_SECONDARY1_INDEX, FONT(XS));
    closeLabel = new StaticText(
        titleBar, {width() - TITLE_HEIGHT, 0, TITLE_HEIGHT, TITLE_HEIGHT}, "X",
        COLOR_THEME_SECONDARY1_INDEX, FONT(XS) | CENTERED);

    label = new StaticText(this, rect_t{}, "");
    label->hide();

    status = new StaticText(this, rect_t{},
                            "We'll show you what hackers can do",
                            COLOR_THEME_SECONDARY1_INDEX, FONT(XXS));
    status->hide();

    image = new StaticBitmap(this, {0, 0, width(), height()});
    image->hide();

    foreground();
  }

  void foreground() override
  {
    if (!loaded || deleted()) return;

    if (getHash() != deps_hash) {
      update();
    }

    char s[LEN_MODEL_NAME + 1];
    strAppend(s, g_model.header.name, LEN_MODEL_NAME);
    label->setText(std::string("CONNECTED: [ ") + s + " ]");
  }

  void update() override
  {
    if (!loaded || deleted()) return;

    auto widgetData = getPersistentData();

    isLarge = rect.h >= LARGE_H && rect.w >= LARGE_W;

    // get font size from options[1]
    etx_font(label->getLvObj(),
             (FontIndex)widgetData->options[1].value.unsignedValue);

    LcdFlags textColor;

    // set font colour from options[0], if use theme color option off
    if (widgetData->options[4].value.boolValue) {
      textColor = COLOR2FLAGS(COLOR_THEME_SECONDARY1_INDEX);
      etx_txt_color(label->getLvObj(), COLOR_THEME_SECONDARY1_INDEX,
                    LV_PART_MAIN);
      etx_txt_color(status->getLvObj(), COLOR_THEME_SECONDARY1_INDEX,
                    LV_PART_MAIN);
    } else {
      textColor = widgetData->options[0].value.unsignedValue;
      etx_txt_color_from_flags(label->getLvObj(), textColor);
      etx_txt_color_from_flags(status->getLvObj(), textColor);
    }

    etx_txt_color_from_flags(titleLabel->getLvObj(), textColor);
    etx_txt_color_from_flags(closeLabel->getLvObj(), textColor);
    lv_obj_set_style_border_color(lvobj,
                                  makeLvColor(colorToRGB(textColor)),
                                  LV_PART_MAIN);
    lv_obj_set_style_border_color(titleBar->getLvObj(),
                                  makeLvColor(colorToRGB(textColor)),
                                  LV_PART_MAIN);

    // Set label position
    if (isLarge)
      lv_obj_set_pos(label->getLvObj(), LARGE_LBL_X,
                     TITLE_HEIGHT + LARGE_LBL_Y);
    else
      lv_obj_set_pos(label->getLvObj(), LARGE_LBL_X, TITLE_HEIGHT);
    lv_obj_set_size(label->getLvObj(), width() - LARGE_LBL_X * 2,
                    EdgeTxStyles::STD_FONT_HEIGHT);

    lv_obj_set_pos(status->getLvObj(), LARGE_LBL_X,
                   TITLE_HEIGHT + LARGE_LBL_Y +
                       EdgeTxStyles::STD_FONT_HEIGHT);
    lv_obj_set_size(status->getLvObj(), width() - LARGE_LBL_X * 2,
                    EdgeTxStyles::STD_FONT_HEIGHT);
    // get fill color from options[3]
    etx_bg_color_from_flags(lvobj, widgetData->options[3].value.unsignedValue);
    etx_bg_color_from_flags(titleBar->getLvObj(),
                            widgetData->options[3].value.unsignedValue);

    // Set background opacity from options[2]
    if (widgetData->options[2].value.boolValue)
      lv_obj_add_state(lvobj, ETX_STATE_BG_FILL);
    else
      lv_obj_clear_state(lvobj, ETX_STATE_BG_FILL);

    coord_t w = width();
    coord_t h = height() - (isLarge ? LARGE_IMG_H : 0);
    bool sizeChg = (w != image->width()) || (h != image->height());

    if (sizeChg)
      image->setRect({0, isLarge ? LARGE_IMG_H : 0, w, h});

    if (!image->hasImage() || deps_hash != getHash() || sizeChg) {
      if (g_model.header.bitmap[0]) {
        char filename[LEN_BITMAP_NAME + 1];
        strAppend(filename, g_model.header.bitmap, LEN_BITMAP_NAME);
        std::string fullpath =
            std::string(BITMAPS_PATH PATH_SEPARATOR) + filename;

        image->setSource(fullpath.c_str());
      } else {
        image->clearSource();
      }
      deps_hash = getHash();
    }

    image->show(image->hasImage());

    label->show(isLarge || !image->hasImage());
    status->show(isLarge);
  }

  static const WidgetOption options[];

 protected:
  bool isLarge = false;
  uint32_t deps_hash = 0;
  StaticText* label = nullptr;
  StaticText* status = nullptr;
  StaticBitmap* image = nullptr;
  Window* titleBar = nullptr;
  StaticText* titleLabel = nullptr;
  StaticText* closeLabel = nullptr;

  uint32_t getHash() { return hash(g_model.header.bitmap, LEN_BITMAP_NAME); }

  static LAYOUT_VAL_SCALED(LARGE_W, 120)
  static LAYOUT_VAL_SCALED(LARGE_H, 96)
  static LAYOUT_VAL_SCALED(LARGE_LBL_X, 5)
  static LAYOUT_VAL_SCALED(LARGE_LBL_Y, 5)
  static LAYOUT_VAL_SCALED(TITLE_HEIGHT, 18)
  static LAYOUT_VAL_SCALED(LARGE_IMG_H, 56)
};

const WidgetOption ModelBitmapWidget::options[] = {
    {STR_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_SECONDARY1_INDEX)},
    {STR_SIZE, WidgetOption::TextSize, FONT_STD_INDEX},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, false},
    {STR_BG_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_SECONDARY3_INDEX)},
    {STR_USE_THEME_COLOR, WidgetOption::Bool, true},
    {nullptr, WidgetOption::Bool}};

BaseWidgetFactory<ModelBitmapWidget> modelBitmapWidget(
    "ModelBmp", ModelBitmapWidget::options, STR_WIDGET_MODELBMP);
