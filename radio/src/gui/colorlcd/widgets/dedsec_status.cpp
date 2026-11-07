/*
 * Separate DedSec system-status and flight-log widgets.
 * License GPLv2.
 */

#include "widget.h"

#include "edgetx.h"
#include "hal/usb_driver.h"
#include "static.h"

#define ETX_STATE_DEDSEC_BG_FILL LV_STATE_USER_1

class DedsecWindowWidget : public Widget
{
 public:
  DedsecWindowWidget(const WidgetFactory* factory, Window* parent,
                     const rect_t& rect, int screenNum, int zoneNum,
                     const char* title) :
      Widget(factory, parent, rect, screenNum, zoneNum), title(title)
  {
    padAll(PAD_ZERO);
    etx_obj_add_style(lvobj, styles->bg_opacity_transparent, LV_PART_MAIN);
    etx_obj_add_style(lvobj, styles->bg_opacity_cover,
                      LV_PART_MAIN | ETX_STATE_DEDSEC_BG_FILL);
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
        title, COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    closeLabel = new StaticText(
        titleBar, {width() - TITLE_HEIGHT, 0, TITLE_HEIGHT, TITLE_HEIGHT}, "X",
        COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | CENTERED);

    buildBody();

    frame = new Window(this, {0, 0, width() - 1, height() - 1});
    frame->setWindowFlag(NO_FOCUS | NO_CLICK);
    etx_obj_add_style(frame->getLvObj(), styles->bg_opacity_transparent,
                      LV_PART_MAIN);
    lv_obj_set_style_border_side(frame->getLvObj(), LV_BORDER_SIDE_FULL,
                                 LV_PART_MAIN);
    lv_obj_set_style_border_width(frame->getLvObj(), 1, LV_PART_MAIN);

    update();
    foreground();
  }

  void update() override
  {
    if (!loaded || deleted()) return;

    auto widgetData = getPersistentData();
    textColor = widgetData->options[0].value.unsignedValue;
    backgroundColor = widgetData->options[1].value.unsignedValue;
    accentColor = widgetData->options[3].value.unsignedValue;

    etx_bg_color_from_flags(lvobj, backgroundColor);
    etx_bg_color_from_flags(titleBar->getLvObj(), backgroundColor);
    etx_txt_color_from_flags(titleLabel->getLvObj(), textColor);
    etx_txt_color_from_flags(closeLabel->getLvObj(), textColor);
    if (bodyLabel) etx_txt_color_from_flags(bodyLabel->getLvObj(), textColor);

    const lv_color_t accent = makeLvColor(colorToRGB(accentColor));
    lv_obj_set_style_border_color(lvobj, accent, LV_PART_MAIN);
    lv_obj_set_style_border_color(titleBar->getLvObj(), accent, LV_PART_MAIN);
    lv_obj_set_style_border_color(frame->getLvObj(), accent, LV_PART_MAIN);

    if (widgetData->options[2].value.boolValue)
      lv_obj_add_state(lvobj, ETX_STATE_DEDSEC_BG_FILL);
    else
      lv_obj_clear_state(lvobj, ETX_STATE_DEDSEC_BG_FILL);
  }

  static const WidgetOption commonOptions[];

 protected:
  virtual void buildBody() = 0;

  const char* title;
  LcdFlags textColor = 0;
  LcdFlags backgroundColor = 0;
  LcdFlags accentColor = 0;
  Window* titleBar = nullptr;
  StaticText* titleLabel = nullptr;
  StaticText* closeLabel = nullptr;
  StaticText* bodyLabel = nullptr;
  Window* frame = nullptr;

  static LAYOUT_VAL_SCALED(TITLE_HEIGHT, 18)
};

const WidgetOption DedsecWindowWidget::commonOptions[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {nullptr, WidgetOption::Bool}};

class SystemStatusWidget : public DedsecWindowWidget
{
 public:
  SystemStatusWidget(const WidgetFactory* factory, Window* parent,
                     const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "SYSTEM STATUS_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !bodyLabel) return;

    auto widgetData = getPersistentData();
    const mixsrc_t armSource =
        widgetData->options[4].value.unsignedValue;
    const bool telemetry = TELEMETRY_STREAMING();
    const bool armed = armSource > MIXSRC_NONE && getValue(armSource) > 0;

    const char* teleText = telemetry ? "[ TELE ]" : "  TELE  ";
    const char* armedText = armed ? "[ ARMED ]" : "  ARMED  ";
    const char* linkedText = telemetry ? "[ LINKED ]" : "  LINKED  ";
    char text[40];
    snprintf(text, sizeof(text), "%s %s %s", teleText, armedText,
             linkedText);
    bodyLabel->setText(text);
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    bodyLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT, width() - 2 * PAD_SMALL,
               height() - TITLE_HEIGHT},
        "  TELE     ARMED     LINKED  ", COLOR_THEME_PRIMARY2_INDEX,
        FONT(XS) | CENTERED);
  }
};

const WidgetOption SystemStatusWidget::options[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Arm source", WidgetOption::Source, MIXSRC_FIRST_SWITCH},
    {nullptr, WidgetOption::Bool}};

class FlightLogWidget : public DedsecWindowWidget
{
 public:
  FlightLogWidget(const WidgetFactory* factory, Window* parent,
                  const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "FLIGHT LOG_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !bodyLabel) return;

    const bool recording =
        !usbPlugged() && isFunctionActive(FUNCTION_LOGS);
    const bool linked = TELEMETRY_STREAMING();
    const uint16_t minutes = sessionTimer / 60;
    const uint16_t seconds = sessionTimer % 60;
    char text[80];
    snprintf(text, sizeof(text),
             "SESSION: %02u:%02u\nREC: %s\nLINK: %s", minutes, seconds,
             recording ? "[ ACTIVE ]" : "  IDLE  ",
             linked ? "[ ONLINE ]" : "  OFFLINE  ");
    bodyLabel->setText(text);
  }

 protected:
  void buildBody() override
  {
    bodyLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT + PAD_TINY, width() - 2 * PAD_SMALL,
               height() - TITLE_HEIGHT - PAD_TINY},
        "SESSION: 00:00\nREC:   IDLE\nLINK:   OFFLINE",
        COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
  }
};

template <class T>
class DedsecWidgetFactory : public WidgetFactory
{
 public:
  DedsecWidgetFactory(const char* name, const WidgetOption* options,
                      const char* displayName) :
      WidgetFactory(name, options, displayName)
  {
  }

  Widget* createNew(Window* parent, const rect_t& rect, int screenNum,
                    int zoneNum) const override
  {
    return new T(this, parent, rect, screenNum, zoneNum);
  }
};

DedsecWidgetFactory<SystemStatusWidget> systemStatusWidget(
    "SysStatus", SystemStatusWidget::options, "System Status");
DedsecWidgetFactory<FlightLogWidget> flightLogWidget(
    "FlightLog", DedsecWindowWidget::commonOptions, "Flight Log");
