/*
 * Separate DedSec system-status and flight-log widgets.
 * License GPLv2.
 */

#include "widget.h"

#include <math.h>

#include "edgetx.h"
#include "hal/adc_driver.h"
#include "hal/usb_driver.h"
#include "static.h"
#include "telemetry/telemetry.h"

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

static int32_t sourceDisplayValue(mixsrc_t source)
{
  if (source == MIXSRC_NONE) return 0;
  int32_t value = getValue(source);
  if (source >= MIXSRC_FIRST_TELEM && source <= MIXSRC_LAST_TELEM) {
    const uint8_t sensor = (source - MIXSRC_FIRST_TELEM) / 3;
    const auto& telemetrySensor = g_model.telemetrySensors[sensor];
    value = convertTelemetryValue(value, telemetrySensor.unit,
                                  telemetrySensor.prec, UNIT_DEGREE, 0);
  }
  return value;
}

static bool sourceIsLive(mixsrc_t source)
{
  if (source == MIXSRC_NONE) return false;
  if (source >= MIXSRC_FIRST_TELEM && source <= MIXSRC_LAST_TELEM)
    return TELEMETRY_STREAMING() && sourceIsFresh(source);
  return true;
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

    // Body graphics are created after the title bar.  Bring the opaque title
    // bar back to the foreground so animated lines cannot draw through it.
    lv_obj_move_foreground(titleBar->getLvObj());

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
                         "FLIGHT TRACKING_")
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
             "SESSION: %02u:%02u\nLINK: %s\nREC: %s", minutes, seconds,
             linked ? "[ ONLINE ]" : "  OFFLINE  ",
             recording ? "[ ACTIVE ]" : "  IDLE  ");
    bodyLabel->setText(text);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (flightIcon) {
      lv_obj_set_style_img_recolor(
          flightIcon->getLvObj(), makeLvColor(colorToRGB(textColor)),
          LV_PART_MAIN);
      lv_obj_set_style_img_recolor_opa(flightIcon->getLvObj(), LV_OPA_COVER,
                                       LV_PART_MAIN);
    }
  }

 protected:
  void buildBody() override
  {
    bodyLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT + PAD_TINY, width() - 2 * PAD_SMALL,
               height() - TITLE_HEIGHT - PAD_TINY},
        "SESSION: 00:00\nLINK:   OFFLINE\nREC:  IDLE",
        COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    flightIcon =
        new StaticIcon(this, width() - 51, height() - 56,
                       ICON_FLIGHT_TRACKING, COLOR_THEME_PRIMARY2_INDEX);
  }

  StaticIcon* flightIcon = nullptr;
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
             "%s\n  LAT %s\n  LON %s\n  SAT %s\n  DST %s\n  HDG %s",
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
        "  SCANNING\n  LAT --\n  LON --\n  SAT --\n  DST --\n  HDG --",
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

class LastLocationWidget : public DedsecWindowWidget
{
 public:
  LastLocationWidget(const WidgetFactory* factory, Window* parent,
                     const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "LAST LOCATION_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !bodyLabel) return;

    auto widgetData = getPersistentData();
    const mixsrc_t latitude = widgetData->options[4].value.unsignedValue;
    const mixsrc_t longitude = widgetData->options[5].value.unsignedValue;
    const bool fix = TELEMETRY_STREAMING() && sourceIsFresh(latitude) &&
                     sourceIsFresh(longitude);

    if (fix) {
      lastLatitude = sourceValueText(latitude);
      lastLongitude = sourceValueText(longitude);
      lastFixTime = get_tmr10ms();
      hasFix = true;
    }

    uint32_t age = hasFix ? (get_tmr10ms() - lastFixTime) / 100 : 0;
    const uint32_t minutes = age / 60;
    const uint32_t seconds = age % 60;
    char text[160];
    snprintf(text, sizeof(text), "LAT: %s\nLON: %s\nAGE: %02lu:%02lu",
             hasFix ? lastLatitude.c_str() : "--",
             hasFix ? lastLongitude.c_str() : "--",
             (unsigned long)minutes, (unsigned long)seconds);
    bodyLabel->setText(text);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (trackingIcon) {
      lv_obj_set_style_img_recolor(
          trackingIcon->getLvObj(), makeLvColor(colorToRGB(textColor)),
          LV_PART_MAIN);
      lv_obj_set_style_img_recolor_opa(trackingIcon->getLvObj(), LV_OPA_COVER,
                                       LV_PART_MAIN);
    }
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    bodyLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT + PAD_TINY, width() - 2 * PAD_SMALL,
               height() - TITLE_HEIGHT - PAD_TINY},
        "LAT: --\nLON: --\nAGE: 00:00", COLOR_THEME_PRIMARY2_INDEX,
        FONT(XS));
    trackingIcon =
        new StaticIcon(this, width() - 51, height() - 52, ICON_TRACKING,
                       COLOR_THEME_PRIMARY2_INDEX);
  }

  bool hasFix = false;
  tmr10ms_t lastFixTime = 0;
  std::string lastLatitude;
  std::string lastLongitude;
  StaticIcon* trackingIcon = nullptr;
};

const WidgetOption LastLocationWidget::options[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Latitude", WidgetOption::Source, MIXSRC_NONE},
    {"Longitude", WidgetOption::Source, MIXSRC_NONE},
    {nullptr, WidgetOption::Bool}};

class AttitudeWidget : public DedsecWindowWidget
{
 public:
  AttitudeWidget(const WidgetFactory* factory, Window* parent,
                 const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "DIGITAL ATTITUDE_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !horizon) return;

    // Brief alternating laugh, followed by a long rest. setIcon() only
    // updates the canvas when the state changes, so this remains inexpensive.
    const uint32_t laughPhase = get_tmr10ms() % 200;
    const bool swapSkulls = laughPhase < 15 ||
                            (laughPhase >= 30 && laughPhase < 45);
    if (leftSkull && rightSkull) {
      leftSkull->setIcon(swapSkulls ? ICON_SKULL_CLOSE : ICON_SKULL_OPEN);
      rightSkull->setIcon(swapSkulls ? ICON_SKULL_OPEN : ICON_SKULL_CLOSE);
    }

    auto widgetData = getPersistentData();
    const mixsrc_t rollSource = widgetData->options[4].value.unsignedValue;
    const mixsrc_t pitchSource = widgetData->options[5].value.unsignedValue;
    const bool live = sourceIsLive(rollSource) && sourceIsLive(pitchSource);
    const bool rollTelemetry = rollSource >= MIXSRC_FIRST_TELEM &&
                               rollSource <= MIXSRC_LAST_TELEM;
    const bool pitchTelemetry = pitchSource >= MIXSRC_FIRST_TELEM &&
                                pitchSource <= MIXSRC_LAST_TELEM;
    const int32_t rollValue = rollTelemetry
                                  ? sourceDisplayValue(rollSource)
                                  : calcRESXto100(getValue(rollSource)) * 60 / 100;
    const int32_t pitchValue = pitchTelemetry
                                   ? sourceDisplayValue(pitchSource)
                                   : calcRESXto100(getValue(pitchSource)) * 45 / 100;
    const int32_t roll = live ? limit<int32_t>(-60, rollValue, 60) : 0;
    const int32_t pitch = live ? limit<int32_t>(-45, pitchValue, 45) : 0;

    const float angle = roll * 3.14159265f / 180.0f;
    const float cs = cosf(angle);
    const float sn = sinf(angle);
    const coord_t cx = width() / 2;
    const coord_t cy = TITLE_HEIGHT + (height() - TITLE_HEIGHT) / 2 + pitch / 3;

    setHorizonLine(horizonPoints, cx, cy, HORIZON_HALF_W, cs, sn);
    lv_line_set_points(horizon, horizonPoints, 2);
    for (uint8_t i = 0; i < 4; ++i) {
      const int8_t pitchStep = (i < 2) ? -(2 - i) : (i - 1);
      const coord_t offset = pitchStep * LADDER_GAP;
      const coord_t lx = cx + (coord_t)(-sn * offset);
      const coord_t ly = cy + (coord_t)(cs * offset);
      setHorizonLine(ladderPoints[i], lx, ly,
                     i == 1 || i == 2 ? LADDER_HALF_W : LADDER_HALF_W - 12,
                     cs, sn);
      lv_line_set_points(ladder[i], ladderPoints[i], 2);
    }

    char readout[16];
    snprintf(readout, sizeof(readout), "R%+ld", (long)roll);
    rollLabel->setText(readout);
    statusLabel->setText(live ? "[ LIVE ]" : "STANDBY");
    snprintf(readout, sizeof(readout), "P%+ld", (long)pitch);
    pitchLabel->setText(readout);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (!loaded || deleted()) return;
    if (horizon) etx_line_color_from_flags(horizon, accentColor);
    for (uint8_t i = 0; i < 4; ++i)
      if (ladder[i]) etx_line_color_from_flags(ladder[i], accentColor);
    for (uint8_t i = 0; i < 3; ++i)
      if (reticle[i]) etx_line_color_from_flags(reticle[i], textColor);
    if (statusLabel)
      etx_txt_color_from_flags(statusLabel->getLvObj(), textColor);
    if (rollLabel)
      etx_txt_color_from_flags(rollLabel->getLvObj(), textColor);
    if (pitchLabel)
      etx_txt_color_from_flags(pitchLabel->getLvObj(), textColor);
    for (auto skull : {leftSkull, rightSkull}) {
      if (skull) {
        lv_obj_set_style_img_recolor(
            skull->getLvObj(), makeLvColor(colorToRGB(textColor)),
            LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(skull->getLvObj(), LV_OPA_COVER,
                                         LV_PART_MAIN);
      }
    }
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    horizon = lv_line_create(lvobj);
    lv_obj_set_style_line_width(horizon, 2, LV_PART_MAIN);
    for (uint8_t i = 0; i < 4; ++i) {
      ladder[i] = lv_line_create(lvobj);
      lv_obj_set_style_line_width(ladder[i], 1, LV_PART_MAIN);
    }

    const coord_t cx = width() / 2;
    const coord_t cy = TITLE_HEIGHT + (height() - TITLE_HEIGHT) / 2;
    reticlePoints[0][0] = {(lv_coord_t)(cx - RETICLE_W), (lv_coord_t)cy};
    reticlePoints[0][1] = {(lv_coord_t)(cx - 4), (lv_coord_t)cy};
    reticlePoints[1][0] = {(lv_coord_t)(cx + 4), (lv_coord_t)cy};
    reticlePoints[1][1] = {(lv_coord_t)(cx + RETICLE_W), (lv_coord_t)cy};
    reticlePoints[2][0] = {(lv_coord_t)cx, (lv_coord_t)(cy - 3)};
    reticlePoints[2][1] = {(lv_coord_t)cx, (lv_coord_t)(cy + 5)};
    for (uint8_t i = 0; i < 3; ++i) {
      reticle[i] = lv_line_create(lvobj);
      lv_obj_set_style_line_width(reticle[i], 2, LV_PART_MAIN);
      lv_line_set_points(reticle[i], reticlePoints[i], 2);
    }

    const coord_t readoutWidth = (width() - 2 * PAD_SMALL) / 3;
    const coord_t readoutY = TITLE_HEIGHT + PAD_TINY;
    rollLabel = new StaticText(
        this, {PAD_SMALL, readoutY, readoutWidth,
               EdgeTxStyles::STD_FONT_HEIGHT},
        "R+0", COLOR_THEME_PRIMARY2_INDEX, FONT(XS));
    statusLabel = new StaticText(
        this, {PAD_SMALL + readoutWidth, readoutY, readoutWidth,
               EdgeTxStyles::STD_FONT_HEIGHT},
        "STANDBY", COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | CENTERED);
    pitchLabel = new StaticText(
        this, {PAD_SMALL + readoutWidth * 2, readoutY, readoutWidth,
               EdgeTxStyles::STD_FONT_HEIGHT},
        "P+0", COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | RIGHT);

    const coord_t skullY =
        TITLE_HEIGHT + (height() - TITLE_HEIGHT - SKULL_H) / 2;
    leftSkull = new StaticIcon(this, SKULL_X_INSET, skullY, ICON_SKULL_OPEN,
                               COLOR_THEME_PRIMARY2_INDEX);
    rightSkull = new StaticIcon(this, width() - SKULL_X_INSET - SKULL_W,
                                skullY, ICON_SKULL_CLOSE,
                                COLOR_THEME_PRIMARY2_INDEX);
  }

  static void setHorizonLine(lv_point_t* points, coord_t cx, coord_t cy,
                             coord_t halfWidth, float cs, float sn)
  {
    points[0] = {(lv_coord_t)(cx - cs * halfWidth),
                 (lv_coord_t)(cy - sn * halfWidth)};
    points[1] = {(lv_coord_t)(cx + cs * halfWidth),
                 (lv_coord_t)(cy + sn * halfWidth)};
  }

  lv_obj_t* horizon = nullptr;
  lv_obj_t* ladder[4] = {nullptr};
  lv_obj_t* reticle[3] = {nullptr};
  StaticText* statusLabel = nullptr;
  StaticText* rollLabel = nullptr;
  StaticText* pitchLabel = nullptr;
  StaticIcon* leftSkull = nullptr;
  StaticIcon* rightSkull = nullptr;
  lv_point_t horizonPoints[2];
  lv_point_t ladderPoints[4][2];
  lv_point_t reticlePoints[3][2];

  static LAYOUT_VAL_SCALED(HORIZON_HALF_W, 92)
  static LAYOUT_VAL_SCALED(LADDER_HALF_W, 35)
  static LAYOUT_VAL_SCALED(LADDER_GAP, 12)
  static LAYOUT_VAL_SCALED(RETICLE_W, 27)
  static LAYOUT_VAL_SCALED(SKULL_X_INSET, 20)
  static LAYOUT_VAL_SCALED(SKULL_W, 40)
  static LAYOUT_VAL_SCALED(SKULL_H, 45)
};

const WidgetOption AttitudeWidget::options[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Roll source", WidgetOption::Source, MIXSRC_NONE},
    {"Pitch source", WidgetOption::Source, MIXSRC_NONE},
    {nullptr, WidgetOption::Bool}};

class LinkAnalyzerWidget : public DedsecWindowWidget
{
 public:
  LinkAnalyzerWidget(const WidgetFactory* factory, Window* parent,
                     const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "LINK ANALYZER_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !statusLabel) return;
    auto widgetData = getPersistentData();
    const mixsrc_t lqSource = widgetData->options[4].value.unsignedValue;
    const mixsrc_t rssiSource = widgetData->options[5].value.unsignedValue;
    const mixsrc_t snrSource = widgetData->options[6].value.unsignedValue;
    const mixsrc_t powerSource = widgetData->options[7].value.unsignedValue;
    const bool linked = TELEMETRY_STREAMING();
    const int32_t quality = sourceIsFresh(lqSource)
                                ? limit<int32_t>(0, sourceDisplayValue(lqSource), 100)
                                : (linked ? limit<int32_t>(0, TELEMETRY_RSSI(), 100) : 0);

    statusLabel->setText(linked ? "[ LINKED ]" : "  LINK LOST  ");
    metricLabels[0]->setText((std::string("LQ\n") + sourceValueText(lqSource)).c_str());
    metricLabels[1]->setText((std::string("RSSI\n") + sourceValueText(rssiSource)).c_str());
    metricLabels[2]->setText((std::string("SNR\n") + sourceValueText(snrSource)).c_str());
    metricLabels[3]->setText((std::string("RF PWR\n") + sourceValueText(powerSource)).c_str());

    const uint8_t activeBars = (quality + 9) / 10;
    for (uint8_t i = 0; i < 10; ++i)
      lv_obj_set_style_bg_opa(bars[i], i < activeBars ? LV_OPA_COVER : LV_OPA_20,
                              LV_PART_MAIN);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (!loaded || deleted()) return;
    if (statusLabel)
      etx_txt_color_from_flags(statusLabel->getLvObj(), textColor);
    for (uint8_t i = 0; i < 4; ++i)
      if (metricLabels[i])
        etx_txt_color_from_flags(metricLabels[i]->getLvObj(), textColor);
    for (uint8_t i = 0; i < 10; ++i)
      if (bars[i]) etx_bg_color_from_flags(bars[i], accentColor);
  }

  static const WidgetOption options[];

 protected:
  void buildBody() override
  {
    statusLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT + PAD_TINY, width() - 2 * PAD_SMALL,
               EdgeTxStyles::STD_FONT_HEIGHT},
        "  LINK LOST  ", COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | CENTERED);

    const coord_t metricY = TITLE_HEIGHT + EdgeTxStyles::STD_FONT_HEIGHT;
    const coord_t metricW = width() / 4;
    for (uint8_t i = 0; i < 4; ++i) {
      metricLabels[i] = new StaticText(
          this, {i * metricW, metricY,
                 i == 3 ? width() - i * metricW : metricW,
                 EdgeTxStyles::STD_FONT_HEIGHT * 2},
          i == 0 ? "LQ\n--" : (i == 1 ? "RSSI\n--" :
                                (i == 2 ? "SNR\n--" : "RF PWR\n--")),
          COLOR_THEME_PRIMARY2_INDEX, FONT(XS) | CENTERED);
    }

    const coord_t gap = PAD_TINY;
    const coord_t barW = (width() - PAD_SMALL * 2 - gap * 9) / 10;
    const coord_t barBottom = height() - PAD_SMALL;
    for (uint8_t i = 0; i < 10; ++i) {
      const coord_t barH = 3 + i;
      bars[i] = lv_obj_create(lvobj);
      lv_obj_set_pos(bars[i], PAD_SMALL + i * (barW + gap), barBottom - barH);
      lv_obj_set_size(bars[i], barW, barH);
      lv_obj_set_style_border_width(bars[i], 0, LV_PART_MAIN);
      lv_obj_set_style_bg_opa(bars[i], LV_OPA_20, LV_PART_MAIN);
      lv_obj_clear_flag(bars[i], LV_OBJ_FLAG_CLICKABLE);
    }
  }

  StaticText* statusLabel = nullptr;
  StaticText* metricLabels[4] = {nullptr};
  lv_obj_t* bars[10] = {nullptr};
};

const WidgetOption LinkAnalyzerWidget::options[] = {
    {STR_TEXT_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {STR_BG_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY1_INDEX)},
    {STR_FILL_BACKGROUND, WidgetOption::Bool, true},
    {STR_COLOR, WidgetOption::Color,
     COLOR2FLAGS(COLOR_THEME_PRIMARY2_INDEX)},
    {"Link quality", WidgetOption::Source, MIXSRC_NONE},
    {"RSSI source", WidgetOption::Source, MIXSRC_NONE},
    {"SNR source", WidgetOption::Source, MIXSRC_NONE},
    {"RF power", WidgetOption::Source, MIXSRC_NONE},
    {nullptr, WidgetOption::Bool}};

class GimbalControlWidget : public DedsecWindowWidget
{
 public:
  GimbalControlWidget(const WidgetFactory* factory, Window* parent,
                      const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "LIVE GIMBAL CONTROL_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !markers[0] || !markers[1]) return;

    // Preserve the GIF timing: five distinct frames, each held for 200 ms,
    // looping continuously with no duplicated frame or added pause.
    const uint8_t frameIndex = (get_tmr10ms() / 20) % 5;
    static const EdgeTxIcon pilotFrames[] = {
        ICON_DRONE_PILOT_1, ICON_DRONE_PILOT_2, ICON_DRONE_PILOT_3,
        ICON_DRONE_PILOT_4, ICON_DRONE_PILOT_5};
    const EdgeTxIcon pilotFrame = pilotFrames[frameIndex];
    if (pilotBackground) pilotBackground->setIcon(pilotFrame);

    setMarker(0, calibratedAnalogs[ADC_MAIN_LH],
              calibratedAnalogs[ADC_MAIN_LV]);
    setMarker(1, calibratedAnalogs[ADC_MAIN_RH],
              calibratedAnalogs[ADC_MAIN_RV]);
  }

  void update() override
  {
    DedsecWindowWidget::update();
    if (!loaded || deleted()) return;

    const lv_color_t accent = makeLvColor(colorToRGB(accentColor));
    if (pilotBackground) {
      lv_obj_set_style_img_recolor(
          pilotBackground->getLvObj(), makeLvColor(colorToRGB(textColor)),
          LV_PART_MAIN);
      lv_obj_set_style_img_recolor_opa(pilotBackground->getLvObj(),
                                       LV_OPA_COVER, LV_PART_MAIN);
    }
    for (uint8_t i = 0; i < 2; ++i) {
      if (boxes[i])
        lv_obj_set_style_border_color(boxes[i], accent, LV_PART_MAIN);
      if (markers[i]) etx_bg_color_from_flags(markers[i], accentColor);
      for (uint8_t line = 0; line < 2; ++line)
        if (crossLines[i][line])
          etx_line_color_from_flags(crossLines[i][line], accentColor);
    }
  }

 protected:
  void buildBody() override
  {
    // The supplied artwork is sized to the full inner widget area.  Keep it
    // behind the title, frame, boxes and live markers.
    pilotBackground = new StaticIcon(this, 1, 1, ICON_DRONE_PILOT_1,
                                     COLOR_THEME_PRIMARY2_INDEX);
    lv_obj_move_background(pilotBackground->getLvObj());

    // Reserve the upper half for a future image. The live controls occupy
    // only the lower half of the widget.
    const coord_t lowerTop =
        TITLE_HEIGHT + (height() - TITLE_HEIGHT) / 2;
    const coord_t availableH = height() - lowerTop - PAD_SMALL * 2;
    const coord_t availableW = (width() - PAD_SMALL * 3) / 2;
    gimbalSize = min(availableH, availableW);
    if (gimbalSize < 16) gimbalSize = 16;

    const coord_t y = height() - PAD_SMALL - gimbalSize - 5;
    const coord_t leftX = width() / 4 - gimbalSize / 2;
    const coord_t rightX = width() * 3 / 4 - gimbalSize / 2;
    buildGimbal(0, leftX, y);
    buildGimbal(1, rightX, y);
  }

  void buildGimbal(uint8_t index, coord_t x, coord_t y)
  {
    boxes[index] = lv_obj_create(lvobj);
    lv_obj_set_pos(boxes[index], x, y);
    lv_obj_set_size(boxes[index], gimbalSize, gimbalSize);
    lv_obj_set_style_bg_opa(boxes[index], LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(boxes[index], 1, LV_PART_MAIN);
    lv_obj_clear_flag(boxes[index], LV_OBJ_FLAG_CLICKABLE);

    const lv_coord_t center = gimbalSize / 2;
    crossPoints[index][0][0] = {2, center};
    crossPoints[index][0][1] = {(lv_coord_t)(gimbalSize - 3), center};
    crossPoints[index][1][0] = {center, 2};
    crossPoints[index][1][1] = {center, (lv_coord_t)(gimbalSize - 3)};
    for (uint8_t line = 0; line < 2; ++line) {
      crossLines[index][line] = lv_line_create(boxes[index]);
      lv_obj_set_style_line_width(crossLines[index][line], 1, LV_PART_MAIN);
      lv_line_set_points(crossLines[index][line],
                         crossPoints[index][line], 2);
    }

    markers[index] = lv_obj_create(boxes[index]);
    lv_obj_set_size(markers[index], MARKER_SIZE, MARKER_SIZE);
    lv_obj_set_style_border_width(markers[index], 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(markers[index], LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(markers[index], LV_OBJ_FLAG_CLICKABLE);
  }

  void setMarker(uint8_t index, int16_t horizontal, int16_t vertical)
  {
    const coord_t range = (gimbalSize - MARKER_SIZE - 4) / 2;
    const coord_t center = (gimbalSize - MARKER_SIZE) / 2;
    const coord_t x = center + horizontal * range / RESX;
    const coord_t y = center - vertical * range / RESX;
    lv_obj_set_pos(markers[index], x, y);
  }

  coord_t gimbalSize = 0;
  StaticIcon* pilotBackground = nullptr;
  lv_obj_t* boxes[2] = {nullptr};
  lv_obj_t* crossLines[2][2] = {{nullptr}};
  lv_obj_t* markers[2] = {nullptr};
  lv_point_t crossPoints[2][2][2];

  static LAYOUT_VAL_SCALED(MARKER_SIZE, 5)
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
DedsecWidgetFactory<GpsLocatorWidget> gpsLocatorWidget(
    "GpsLoc", GpsLocatorWidget::options, "GPS Locator");
DedsecWidgetFactory<LastLocationWidget> lastLocationWidget(
    "LastLoc", LastLocationWidget::options, "Last Location");
DedsecWidgetFactory<AttitudeWidget> attitudeWidget(
    "Attitude", AttitudeWidget::options, "Attitude");
DedsecWidgetFactory<LinkAnalyzerWidget> linkAnalyzerWidget(
    "LinkAnal", LinkAnalyzerWidget::options, "Link Analyzer");
DedsecWidgetFactory<GimbalControlWidget> gimbalControlWidget(
    "GimbalCtl", DedsecWindowWidget::commonOptions, "Gimbal Control");
