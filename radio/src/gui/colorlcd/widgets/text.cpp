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

#include "edgetx.h"
#include "theme_manager.h"
#include "widget.h"

#define TEXT_WIDGET_DEFAULT_LABEL "My Label"

class TextWidget : public Widget
{
 public:
  TextWidget(const WidgetFactory* factory, Window* parent, const rect_t& rect,
             int screenNum, int zoneNum) :
      Widget(factory, parent, rect, screenNum, zoneNum)
  {
    delayLoad();
  }

  void delayedInit() override
  {
    lv_style_init(&style);
    lv_style_set_width(&style, lv_pct(100));
    lv_style_set_height(&style, lv_pct(100));

    shadow = etx_label_create(lvobj);
    lv_obj_add_style(shadow, &style, LV_PART_MAIN);
    lv_obj_set_style_text_color(shadow, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_pos(shadow, 1, 1);

    label = etx_label_create(lvobj);
    lv_obj_add_style(label, &style, LV_PART_MAIN);

    shadow2 = etx_label_create(lvobj);
    lv_obj_add_style(shadow2, &style, LV_PART_MAIN);
    lv_obj_set_style_text_color(shadow2, lv_color_black(), LV_PART_MAIN);

    label2 = etx_label_create(lvobj);
    lv_obj_add_style(label2, &style, LV_PART_MAIN);

    update();
  }

  static const WidgetOption options[];

 protected:
  lv_style_t style;
  lv_obj_t* shadow;
  lv_obj_t* label;
  lv_obj_t* shadow2;
  lv_obj_t* label2;

  void update() override
  {
    if (!loaded || deleted()) return;

    auto widgetData = getPersistentData();

    // Set text value from options
    lv_label_set_text(shadow, widgetData->options[0].value.stringValue.c_str());
    lv_label_set_text(label, widgetData->options[0].value.stringValue.c_str());
    lv_label_set_text(shadow2, widgetData->options[1].value.stringValue.c_str());
    lv_label_set_text(label2, widgetData->options[1].value.stringValue.c_str());

    // get font color from options[1]
    etx_txt_color_from_flags(label, widgetData->options[2].value.unsignedValue);
    etx_txt_color_from_flags(label2, widgetData->options[2].value.unsignedValue);
    // get font size from options[2]
    lv_style_set_text_font(
        &style, getFont(widgetData->options[3].value.unsignedValue << 8));
    // get alignment from options[4]
    LcdFlags alignment = widgetData->options[5].value.unsignedValue;
    lv_style_set_text_align(&style,
                            (alignment == ALIGN_RIGHT)    ? LV_TEXT_ALIGN_RIGHT
                            : (alignment == ALIGN_CENTER) ? LV_TEXT_ALIGN_CENTER
                                                          : LV_TEXT_ALIGN_LEFT);

    if (screenNum < 0) {
      lv_style_set_text_font(&style, getFont(FONT(XS)));
      lv_obj_set_pos(shadow, 1, 3);
      lv_obj_set_pos(label, 0, 2);
      lv_obj_set_pos(shadow2, 1, HeaderDateTime::HDR_DATE_LINE2 + 3);
      lv_obj_set_pos(label2, 0, HeaderDateTime::HDR_DATE_LINE2 + 2);
    } else {
      lv_obj_add_flag(shadow2, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(label2, LV_OBJ_FLAG_HIDDEN);
      lv_obj_set_pos(shadow2, 1, EdgeTxStyles::STD_FONT_HEIGHT + 1);
      lv_obj_set_pos(label2, 0, EdgeTxStyles::STD_FONT_HEIGHT);
    }

    // Show or hide shadow
    if (widgetData->options[4].value.boolValue) {
      lv_obj_clear_flag(shadow, LV_OBJ_FLAG_HIDDEN);
      if (screenNum < 0) lv_obj_clear_flag(shadow2, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(shadow, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(shadow2, LV_OBJ_FLAG_HIDDEN);
    }
  }
};

const WidgetOption TextWidget::options[] = {
    {STR_TEXT, WidgetOption::String,
     WIDGET_OPTION_VALUE_STRING(TEXT_WIDGET_DEFAULT_LABEL)},
    {"Text 2", WidgetOption::String, WIDGET_OPTION_VALUE_STRING("")},
    {STR_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_SECONDARY1_INDEX)},
    {STR_SIZE, WidgetOption::TextSize, 0},
    {STR_SHADOW, WidgetOption::Bool, false},
    {STR_ALIGNMENT, WidgetOption::Align, ALIGN_LEFT},
    {nullptr, WidgetOption::Bool}};

BaseWidgetFactory<TextWidget> textWidget("Text", TextWidget::options,
                                         STR_WIDGET_TEXT);
