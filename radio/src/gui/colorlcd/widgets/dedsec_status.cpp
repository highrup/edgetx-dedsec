/*
 * Separate DedSec system-status and flight-log widgets.
 * License GPLv2.
 */

#include "widget.h"

#include <math.h>

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

static int32_t sourceDisplayValue(mixsrc_t source)
{
  if (source == MIXSRC_NONE) return 0;
  int32_t value = getValue(source);
  if (source >= MIXSRC_FIRST_TELEM && source <= MIXSRC_LAST_TELEM) {
    const uint8_t sensor = (source - MIXSRC_FIRST_TELEM) / 3;
    const int32_t divisor = g_model.telemetrySensors[sensor].getPrecDivisor();
    if (divisor > 1) value /= divisor;
  }
  return value;
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

class AttitudeWidget : public DedsecWindowWidget
{
 public:
  AttitudeWidget(const WidgetFactory* factory, Window* parent,
                 const rect_t& rect, int screenNum, int zoneNum) :
      DedsecWindowWidget(factory, parent, rect, screenNum, zoneNum,
                         "ATTITUDE_")
  {
  }

  void foreground() override
  {
    if (!loaded || deleted() || !horizon) return;

    auto widgetData = getPersistentData();
    const mixsrc_t rollSource = widgetData->options[4].value.unsignedValue;
    const mixsrc_t pitchSource = widgetData->options[5].value.unsignedValue;
    const bool live = TELEMETRY_STREAMING() && sourceIsFresh(rollSource) &&
                      sourceIsFresh(pitchSource);
    const int32_t roll = live ? limit<int32_t>(-60, sourceDisplayValue(rollSource), 60) : 0;
    const int32_t pitch = live ? limit<int32_t>(-45, sourceDisplayValue(pitchSource), 45) : 0;

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
                     i == 1 || i == 2 ? LADDER_HALF_W : LADDER_HALF_W - 5,
                     cs, sn);
      lv_line_set_points(ladder[i], ladderPoints[i], 2);
    }

    char readout[64];
    snprintf(readout, sizeof(readout), "%s   R%+ld  P%+ld",
             live ? "[ LIVE ]" : "  STANDBY  ", (long)roll, (long)pitch);
    statusLabel->setText(readout);
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

    statusLabel = new StaticText(
        this, {PAD_SMALL, TITLE_HEIGHT + PAD_TINY, width() - 2 * PAD_SMALL,
               EdgeTxStyles::STD_FONT_HEIGHT},
        "  STANDBY     R+0  P+0", COLOR_THEME_PRIMARY2_INDEX,
        FONT(XS) | CENTERED);
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
  lv_point_t horizonPoints[2];
  lv_point_t ladderPoints[4][2];
  lv_point_t reticlePoints[3][2];

  static LAYOUT_VAL_SCALED(HORIZON_HALF_W, 92)
  static LAYOUT_VAL_SCALED(LADDER_HALF_W, 35)
  static LAYOUT_VAL_SCALED(LADDER_GAP, 12)
  static LAYOUT_VAL_SCALED(RETICLE_W, 27)
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
DedsecWidgetFactory<AttitudeWidget> attitudeWidget(
    "Attitude", AttitudeWidget::options, "Attitude");
DedsecWidgetFactory<LinkAnalyzerWidget> linkAnalyzerWidget(
    "LinkAnal", LinkAnalyzerWidget::options, "Link Analyzer");
