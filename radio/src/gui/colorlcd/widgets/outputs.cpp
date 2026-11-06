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

#include "widget.h"

#include "edgetx.h"
#include "messaging.h"
#include "static.h"

constexpr int16_t OUTPUT_INVALID_VALUE = INT16_MIN;

#define ETX_STATE_OUTPUT_MONITOR_BG_FILL LV_STATE_USER_1

class InputMonitorChannelValue : public Window
{
 public:
  InputMonitorChannelValue(Window* parent, uint8_t col, uint8_t row,
                           coord_t colWidth, uint8_t channel,
                           LcdFlags textColor, LcdFlags accentColor,
                           LcdFlags barColor,
                           LcdFlags backgroundColor) :
      Window(parent,
             {col * colWidth, row * ROW_HEIGHT,
              colWidth - 1 + (colWidth & 1), (ROW_HEIGHT + 1)}),
      channel(channel),
      textColor(textColor),
      accentColor(accentColor),
      barColor(barColor),
      backgroundColor(backgroundColor)
  {
    setWindowFlag(NO_FOCUS | NO_CLICK);

    refreshMsg.subscribe(Messaging::REFRESH_OUTPUTS_WIDGET, [=](uint32_t param) { refresh(); });
  
    delayLoad();
  }

  void delayedInit() override
  {
    etx_obj_add_style(lvobj, styles->border_thin, LV_PART_MAIN);
    lv_obj_set_style_border_color(lvobj,
                                  makeLvColor(colorToRGB(accentColor)),
                                  LV_PART_MAIN);

    padAll(PAD_ZERO);

    valueLabel = etx_label_create(lvobj, FONT_XS_INDEX);
    etx_obj_add_style(valueLabel, styles->text_align_right, LV_PART_MAIN);
    etx_txt_color_from_flags(valueLabel, textColor);
    lv_obj_set_width(valueLabel, LV_PCT(100));
    lv_label_set_text(valueLabel, "");

    chanLabel = etx_label_create(lvobj, FONT_XS_INDEX);
    etx_obj_add_style(chanLabel, styles->text_align_left, LV_PART_MAIN);
    etx_txt_color_from_flags(chanLabel, textColor);
    lv_label_set_text(chanLabel, "");

    // Draw the white live-output bar over the white labels, then repeat the
    // labels in black inside the clipped bar to create a true inversion.
    bar = lv_obj_create(lvobj);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(bar, 0, ROW_HEIGHT - 1);
    etx_bg_color_from_flags(bar, barColor);
    barValueLabel = etx_label_create(bar, FONT_XS_INDEX);
    etx_obj_add_style(barValueLabel, styles->text_align_right, LV_PART_MAIN);
    etx_txt_color_from_flags(barValueLabel, backgroundColor);
    lv_obj_set_width(barValueLabel, width());
    barChanLabel = etx_label_create(bar, FONT_XS_INDEX);
    etx_obj_add_style(barChanLabel, styles->text_align_left, LV_PART_MAIN);
    etx_txt_color_from_flags(barChanLabel, backgroundColor);
    lv_obj_set_width(barChanLabel, width());

    chanHasName = g_model.limitData[channel].name[0] != 0;
    setChannel();

    divPoints[0] = {(lv_coord_t)(width() / 2 - 1), 0};
    divPoints[1] = {(lv_coord_t)(width() / 2 - 1), ROW_HEIGHT - 1};

    auto divLine = lv_line_create(lvobj);
    lv_line_set_points(divLine, divPoints, 2);
    etx_line_color_from_flags(divLine, accentColor);

    refresh();
  }

  void setChannel()
  {
    char s[16];
    if (chanHasName) {
      formatNumberAsString(s, 16, channel + 1, LEADING0, 2, "", " ");
      strAppend(s + 3, g_model.limitData[channel].name, LEN_CHANNEL_NAME);
    } else {
      getSourceString(s, MIXSRC_FIRST_CH + channel);
    }
    lv_label_set_text(chanLabel, s);
    lv_label_set_text(barChanLabel, s);
  }

  void refresh()
  {
    if (!loaded || deleted()) return;

    int16_t value = channelOutputs[channel];

    if (value != lastValue || lastExtendedLimits != g_model.extendedLimits) {
      lastValue = value;
      lastExtendedLimits = g_model.extendedLimits;

      std::string s;
      if (g_eeGeneral.ppmunit == PPM_US)
        s = formatNumberAsString(PPM_CH_CENTER(channel) + value / 2, 0, 0, "", STR_US);
      else if (g_eeGeneral.ppmunit == PPM_PERCENT_PREC1)
        s = formatNumberAsString(calcRESXto1000(value), PREC1, 0, "", "%");
      else
        s = formatNumberAsString(calcRESXto100(value), 0, 0, "", "%");

      if (s != lastText) {
        lastText = s;
        lv_label_set_text(valueLabel, s.c_str());
        lv_label_set_text(barValueLabel, s.c_str());
      }

      const int lim = (g_model.extendedLimits ? (1024 * LIMIT_EXT_PERCENT / 100) : 1024);
      uint16_t w = width() - PAD_TINY;
      int16_t scaledValue = divRoundClosest(w * limit<int16_t>(-lim, value, lim), lim * 2);

      if (scaledValue != lastScaledValue) {
        lastScaledValue = scaledValue;

        uint16_t fillW = abs(scaledValue);
        uint16_t x = value > 0 ? w / 2 : w / 2 - fillW + 1;

        lv_obj_set_pos(bar, x, 0);
        lv_obj_set_size(bar, fillW, ROW_HEIGHT - 1);
        lv_obj_set_pos(barValueLabel, -x, 0);
        lv_obj_set_pos(barChanLabel, -x, 0);
      }
    }

    bool hasName = g_model.limitData[channel].name[0] != 0;
    if (hasName != chanHasName) {
      chanHasName = hasName;
      setChannel();
    }
  }

  static LAYOUT_VAL_SCALED(ROW_HEIGHT, 16)

 protected:
  uint8_t channel;
  LcdFlags textColor;
  LcdFlags accentColor;
  LcdFlags barColor;
  LcdFlags backgroundColor;
  int16_t lastValue = OUTPUT_INVALID_VALUE;
  int16_t lastScaledValue = OUTPUT_INVALID_VALUE;
  std::string lastText;
  bool lastExtendedLimits = false;
  bool chanHasName = false;
  lv_obj_t* valueLabel = nullptr;
  lv_obj_t* chanLabel = nullptr;
  lv_obj_t* barValueLabel = nullptr;
  lv_obj_t* barChanLabel = nullptr;
  lv_point_t divPoints[2];
  lv_obj_t* bar = nullptr;
  Messaging refreshMsg;
};

class InputMonitorWidget : public Widget
{
 public:
  InputMonitorWidget(const WidgetFactory* factory, Window* parent,
                     const rect_t& rect, int screenNum, int zoneNum) :
      Widget(factory, parent, rect, screenNum, zoneNum)
  {
    padAll(PAD_ZERO);

    etx_obj_add_style(lvobj, styles->bg_opacity_transparent, LV_PART_MAIN);
    etx_obj_add_style(lvobj, styles->bg_opacity_cover,
                      LV_PART_MAIN | ETX_STATE_OUTPUT_MONITOR_BG_FILL);
    lv_obj_set_style_border_side(lvobj, LV_BORDER_SIDE_FULL, LV_PART_MAIN);
    lv_obj_set_style_border_width(lvobj, 1, LV_PART_MAIN);

    titleBar = new Window(this, {0, 0, width(), TITLE_HEIGHT});
    titleBar->setWindowFlag(NO_FOCUS | NO_CLICK);
    lv_obj_set_style_bg_opa(titleBar->getLvObj(), LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_side(titleBar->getLvObj(), LV_BORDER_SIDE_BOTTOM,
                                 LV_PART_MAIN);
    lv_obj_set_style_border_width(titleBar->getLvObj(), 1, LV_PART_MAIN);
    titleLabel = new StaticText(
        titleBar,
        {PAD_SMALL, 0, width() - TITLE_HEIGHT - PAD_SMALL, TITLE_HEIGHT},
        "OUTPUT MONITOR_", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    closeLabel = new StaticText(
        titleBar, {width() - TITLE_HEIGHT, 0, TITLE_HEIGHT, TITLE_HEIGHT}, "X",
        COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | CENTERED);

    content = new Window(
        this, {0, TITLE_HEIGHT, width(), height() - TITLE_HEIGHT});
    content->setWindowFlag(NO_FOCUS | NO_CLICK);

    frame = new Window(this, {0, 0, width(), height()});
    frame->setWindowFlag(NO_FOCUS | NO_CLICK);
    etx_obj_add_style(frame->getLvObj(), styles->bg_opacity_transparent,
                      LV_PART_MAIN);
    lv_obj_set_style_border_side(frame->getLvObj(), LV_BORDER_SIDE_FULL,
                                 LV_PART_MAIN);
    lv_obj_set_style_border_width(frame->getLvObj(), 1, LV_PART_MAIN);

    update();
  }

  void update() override
  {
    auto widgetData = getPersistentData();

    const LcdFlags newBackgroundColor =
        widgetData->options[3].value.unsignedValue;
    const LcdFlags newTextColor =
        widgetData->options[4].value.unsignedValue;
    const LcdFlags newAccentColor =
        widgetData->options[5].value.unsignedValue;
    const LcdFlags newBarColor =
        widgetData->options[6].value.unsignedValue;

    etx_bg_color_from_flags(lvobj, newBackgroundColor);
    etx_bg_color_from_flags(titleBar->getLvObj(), newBackgroundColor);
    if (widgetData->options[2].value.boolValue)
      lv_obj_add_state(lvobj, ETX_STATE_OUTPUT_MONITOR_BG_FILL);
    else
      lv_obj_clear_state(lvobj, ETX_STATE_OUTPUT_MONITOR_BG_FILL);

    etx_txt_color_from_flags(titleLabel->getLvObj(), newTextColor);
    etx_txt_color_from_flags(closeLabel->getLvObj(), newTextColor);
    lv_obj_set_style_border_color(lvobj,
                                  makeLvColor(colorToRGB(newAccentColor)),
                                  LV_PART_MAIN);
    lv_obj_set_style_border_color(titleBar->getLvObj(),
                                  makeLvColor(colorToRGB(newAccentColor)),
                                  LV_PART_MAIN);
    lv_obj_set_style_border_color(frame->getLvObj(),
                                  makeLvColor(colorToRGB(newAccentColor)),
                                  LV_PART_MAIN);

    if (height() <= SHOW_MIN_H || width() <= SHOW_MIN_W)
      return;

    bool changed = false;

    if (newBackgroundColor != backgroundColor) {
      backgroundColor = newBackgroundColor;
      changed = true;
    }
    if (newTextColor != textColor) {
      textColor = newTextColor;
      changed = true;
    }
    if (newAccentColor != accentColor) {
      accentColor = newAccentColor;
      changed = true;
    }
    if (newBarColor != barColor) {
      barColor = newBarColor;
      changed = true;
    }

    // Setup channels
    uint8_t chan = widgetData->options[0].value.unsignedValue;
    if (chan != firstChan) { firstChan = chan; changed = true; }
    chan = widgetData->options[1].value.unsignedValue;
    if (chan != lastChan) { lastChan = chan; changed = true; }

    // Get size
    if (width() != lastWidth) { lastWidth = width(); changed = true; }
    if (height() != lastHeight) { lastHeight = height(); changed = true; }
    uint8_t n =
        (lastHeight - TITLE_HEIGHT) / InputMonitorChannelValue::ROW_HEIGHT;
    if (n != rows) { rows = n; changed = true; }
    n = (lastWidth > COLS_MIN_W) ? 2 : 1;
    if (n != cols) { cols = n; changed = true; }

    if (changed) {
      content->clear();
      coord_t colWidth = lastWidth / cols;
      uint8_t chan = firstChan;
      for (uint8_t c = 0; c < cols && chan <= lastChan; c += 1) {
        for (uint8_t r = 0; r < rows && chan <= lastChan;
             r += 1, chan += 1) {
          new InputMonitorChannelValue(content, c, r, colWidth, chan - 1,
                                       textColor, accentColor, barColor,
                                       backgroundColor);
        }
      }
    }
  }

  void foreground() override
  {
    Messaging::send(Messaging::REFRESH_OUTPUTS_WIDGET);
  }

  static const WidgetOption options[];

 protected:
  coord_t lastWidth = -1;
  coord_t lastHeight = -1;
  uint8_t firstChan = 255;
  uint8_t lastChan = 255;
  uint8_t cols = 0;
  uint8_t rows = 0;
  LcdFlags textColor = 0;
  LcdFlags accentColor = 0;
  LcdFlags barColor = 0;
  LcdFlags backgroundColor = 0;
  Window* titleBar = nullptr;
  StaticText* titleLabel = nullptr;
  StaticText* closeLabel = nullptr;
  Window* content = nullptr;
  Window* frame = nullptr;
  static LAYOUT_VAL_SCALED(TITLE_HEIGHT, 18)
  static LAYOUT_VAL_SCALED(SHOW_MIN_W, 100)
  static LAYOUT_VAL_SCALED(SHOW_MIN_H, 20)
  static LAYOUT_VAL_SCALED(COLS_MIN_W, 300)
};

const WidgetOption InputMonitorWidget::options[] = {
    {STR_FIRST_CHANNEL, WidgetOption::Integer, {1}, {1}, {MAX_OUTPUT_CHANNELS}},
    {STR_LAST_CHANNEL, WidgetOption::Integer, {MAX_OUTPUT_CHANNELS}, {1}, {MAX_OUTPUT_CHANNELS}},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, false},
    {STR_BG_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_TEXT_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_COLOR, WidgetOption::Color, COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Bar fill", WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {nullptr, WidgetOption::Bool}};

// Note: Must be a template class otherwise the linker will discard the 'outputsWidget' object
template <class T>
class InputMonitorWidgetFactory : public WidgetFactory
{
 public:
  InputMonitorWidgetFactory(const char* name, const WidgetOption* options,
                            const char* displayName = nullptr) :
      WidgetFactory(name, options, displayName)
  {
  }

  Widget* createNew(Window* parent, const rect_t& rect,
                 int screenNum, int zoneNum) const override
  {
    return new T(this, parent, rect, screenNum, zoneNum);
  }

  // Fix the options loaded from the model file to account for
  // addition of the 'last channel' option
  const void checkOptions(int screenNum, int zoneNum) const override
  {
    auto widgetData = g_model.getWidgetData(screenNum, zoneNum);
    if (widgetData && widgetData->options.size() >= 2 &&
        widgetData->options.size() < 6 &&
        widgetData->options[1].type == WOV_Bool) {
      WidgetOptionValueTyped v;
      v.type = WOV_Signed;
      v.value.signedValue = MAX_OUTPUT_CHANNELS;
      widgetData->options.insert(widgetData->options.begin() + 1, v);
      storageDirty(EE_MODEL);
    }
  }
};

InputMonitorWidgetFactory<InputMonitorWidget> outputsWidget(
    "Outputs", InputMonitorWidget::options, STR_WIDGET_OUTPUTS);
