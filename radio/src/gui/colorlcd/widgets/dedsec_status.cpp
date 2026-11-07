/*
 * Separate DedSec system-status and flight-log widgets.
 * License GPLv2.
 */

#include "widget.h"

#include "edgetx.h"
#include "hal/usb_driver.h"
#include "static.h"

#define ETX_STATE_DEDSEC_BG_FILL LV_STATE_USER_1

std::string getSensorCustomValue(uint8_t sensor, int32_t value,
                                 LcdFlags flags);

static bool sourceIsFresh(mixsrc_t source)
{
  if (source < MIXSRC_FIRST_TELEM || source > MIXSRC_LAST_TELEM) return false;
  TelemetryItem& item = telemetryItems[(source - MIXSRC_FIRST_TELEM) / 3];
  return item.isAvailable() && !item.isOld();
}

static std::string sourceValueText(mixsrc_t source)
{
  if (source == MIXSRC_NONE) return "--";
  const int32_t value = getValue(source);
  if (source >= MIXSRC_FIRST_TELEM && source <= MIXSRC_LAST_TELEM) {
    return getSensorCustomValue((source - MIXSRC_FIRST_TELEM) / 3, value, 0);
  }
  return getSourceCustomValueString(source, value, 0);
}

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
    if (!loaded || deleted() || !armedLabel || !telemetryLabel ||
        !connectedLabel || !buzzerLabel || !gpsLabel)
      return;

    auto widgetData = getPersistentData();
    const mixsrc_t armSource =
        widgetData->options[4].value.unsignedValue;
    const mixsrc_t gpsSource =
        widgetData->options[5].value.unsignedValue;
    const mixsrc_t buzzerSource =
        widgetData->options[6].value.unsignedValue;
    const bool telemetry = TELEMETRY_STREAMING();
    const bool armed = armSource > MIXSRC_NONE && getValue(armSource) > 0;
    const bool gps = telemetry && sourceIsFresh(gpsSource);
    const bool buzzer = telemetry && sourceIsFresh(buzzerSource);

    armedLabel->setText(armed ? "[ ARMED ]" : "  ARMED  ");
    telemetryLabel->setText(telemetry ? "[ TELEMETRY ]" : "  TELEMETRY  ");
    connectedLabel->setText(telemetry ? "[ CONNECTED ]" : "  CONNECTED  ");
    buzzerLabel->setText(buzzer ? "[ BUZZER ]" : "  BUZZER  ");
    gpsLabel->setText(gps ? "[ GPS ]" : "  GPS  ");
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (!loaded || deleted()) return;
    if (armedLabel) etx_txt_color_from_flags(armedLabel->getLvObj(), textColor);
    if (telemetryLabel)
      etx_txt_color_from_flags(telemetryLabel->getLvObj(), textColor);
    if (dronePrefix)
      etx_txt_color_from_flags(dronePrefix->getLvObj(), textColor);
    if (connectedLabel)
      etx_txt_color_from_flags(connectedLabel->getLvObj(), textColor);
    if (buzzerLabel)
      etx_txt_color_from_flags(buzzerLabel->getLvObj(), textColor);
    if (gpsLabel) etx_txt_color_from_flags(gpsLabel->getLvObj(), textColor);
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    const coord_t contentY = TITLE_HEIGHT + PAD_TINY;
    const coord_t statusX = width() * 2 / 5;
    const coord_t statusW = width() - statusX - PAD_SMALL;

    armedLabel = new StaticText(
        this, {statusX, contentY, statusW, STATUS_ROW_HEIGHT},
        "  ARMED  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    telemetryLabel = new StaticText(
        this, {statusX, contentY + STATUS_ROW_HEIGHT, statusW,
               STATUS_ROW_HEIGHT},
        "  TELEMETRY  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    dronePrefix = new StaticText(
        this, {PAD_SMALL, contentY + STATUS_ROW_HEIGHT * 2,
               statusX - PAD_SMALL, STATUS_ROW_HEIGHT},
        "DRONE ---", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    connectedLabel = new StaticText(
        this, {statusX, contentY + STATUS_ROW_HEIGHT * 2, statusW,
               STATUS_ROW_HEIGHT},
        "  CONNECTED  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    buzzerLabel = new StaticText(
        this, {statusX, contentY + STATUS_ROW_HEIGHT * 3, statusW,
               STATUS_ROW_HEIGHT},
        "  BUZZER  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    gpsLabel = new StaticText(
        this, {statusX, contentY + STATUS_ROW_HEIGHT * 4, statusW,
               STATUS_ROW_HEIGHT},
        "  GPS  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
  }

  StaticText* dronePrefix = nullptr;
  StaticText* armedLabel = nullptr;
  StaticText* telemetryLabel = nullptr;
  StaticText* connectedLabel = nullptr;
  StaticText* buzzerLabel = nullptr;
  StaticText* gpsLabel = nullptr;

  static LAYOUT_VAL_SCALED(STATUS_ROW_HEIGHT, 14)
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
    {"GPS source", WidgetOption::Source, MIXSRC_NONE},
    {"Buzzer source", WidgetOption::Source, MIXSRC_NONE},
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

class GpsLocatorWidget : public DedsecWindowWidget
{
 public:
  GpsLocatorWidget(const WidgetFactory* factory, Window* parent,
                   const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "GPS LOCATOR_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !bodyLabel || !marker) return;

    auto widgetData = getPersistentData();
    const mixsrc_t latitude = widgetData->options[4].value.unsignedValue;
    const mixsrc_t longitude = widgetData->options[5].value.unsignedValue;
    const mixsrc_t satellites = widgetData->options[6].value.unsignedValue;
    const mixsrc_t distance = widgetData->options[7].value.unsignedValue;
    const mixsrc_t heading = widgetData->options[8].value.unsignedValue;
    const bool fix = TELEMETRY_STREAMING() && sourceIsFresh(latitude) &&
                     sourceIsFresh(longitude);

    const std::string latText = sourceValueText(latitude);
    const std::string lonText = sourceValueText(longitude);
    const std::string satText = sourceValueText(satellites);
    const std::string distText = sourceValueText(distance);
    const std::string headingText = sourceValueText(heading);
    char text[160];
    snprintf(text, sizeof(text),
             "%s\nLAT %s\nLON %s\nSAT %s\nDST %s\nHDG %s",
             fix ? "[ GPS FIX ]" : "  SCANNING  ", latText.c_str(),
             lonText.c_str(), satText.c_str(), distText.c_str(),
             headingText.c_str());
    bodyLabel->setText(text);

    const uint8_t phase = (get_tmr10ms() / 25) & 7;
    static const int8_t markerX[8] = {0, 6, 9, 6, 0, -6, -9, -6};
    static const int8_t markerY[8] = {-9, -6, 0, 6, 9, 6, 0, -6};
    lv_obj_set_pos(marker, radarSize / 2 + markerX[phase] - 1,
                   radarSize / 2 + markerY[phase] - 1);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (!loaded || deleted()) return;
    if (radar) {
      lv_obj_set_style_border_color(
          radar, makeLvColor(colorToRGB(accentColor)), LV_PART_MAIN);
    }
    if (horizontalLine) etx_line_color_from_flags(horizontalLine, accentColor);
    if (verticalLine) etx_line_color_from_flags(verticalLine, accentColor);
    if (marker) etx_bg_color_from_flags(marker, accentColor);
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    const coord_t availableH = height() - TITLE_HEIGHT - PAD_SMALL * 2;
    const coord_t availableW = width() * 2 / 5;
    radarSize = availableW < availableH ? availableW : availableH;
    if (radarSize < 24) radarSize = 24;

    radar = lv_obj_create(lvobj);
    lv_obj_set_pos(radar, PAD_SMALL, TITLE_HEIGHT + PAD_SMALL);
    lv_obj_set_size(radar, radarSize, radarSize);
    lv_obj_set_style_bg_opa(radar, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(radar, 1, LV_PART_MAIN);
    lv_obj_clear_flag(radar, LV_OBJ_FLAG_CLICKABLE);

    horizontalPoints[0] = {2, (lv_coord_t)(radarSize / 2)};
    horizontalPoints[1] = {(lv_coord_t)(radarSize - 3),
                           (lv_coord_t)(radarSize / 2)};
    horizontalLine = lv_line_create(radar);
    lv_line_set_points(horizontalLine, horizontalPoints, 2);

    verticalPoints[0] = {(lv_coord_t)(radarSize / 2), 2};
    verticalPoints[1] = {(lv_coord_t)(radarSize / 2),
                         (lv_coord_t)(radarSize - 3)};
    verticalLine = lv_line_create(radar);
    lv_line_set_points(verticalLine, verticalPoints, 2);

    marker = lv_obj_create(radar);
    lv_obj_set_size(marker, 3, 3);
    lv_obj_set_style_bg_opa(marker, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(marker, 0, LV_PART_MAIN);
    lv_obj_clear_flag(marker, LV_OBJ_FLAG_CLICKABLE);

    const coord_t dataX = PAD_SMALL * 2 + radarSize;
    bodyLabel = new StaticText(
        this, {dataX, TITLE_HEIGHT + PAD_TINY, width() - dataX - PAD_SMALL,
               height() - TITLE_HEIGHT - PAD_TINY},
        "  SCANNING\nLAT --\nLON --\nSAT --\nDST --\nHDG --",
        COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
  }

  coord_t radarSize = 0;
  lv_obj_t* radar = nullptr;
  lv_obj_t* horizontalLine = nullptr;
  lv_obj_t* verticalLine = nullptr;
  lv_obj_t* marker = nullptr;
  lv_point_t horizontalPoints[2];
  lv_point_t verticalPoints[2];
};

const WidgetOption GpsLocatorWidget::options[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Latitude", WidgetOption::Source, MIXSRC_NONE},
    {"Longitude", WidgetOption::Source, MIXSRC_NONE},
    {"Satellites", WidgetOption::Source, MIXSRC_NONE},
    {"Distance", WidgetOption::Source, MIXSRC_NONE},
    {"Heading", WidgetOption::Source, MIXSRC_NONE},
    {nullptr, WidgetOption::Bool}};

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
DedsecWidgetFactory<GpsLocatorWidget> gpsLocatorWidget(
    "GpsLoc", GpsLocatorWidget::options, "GPS Locator");
