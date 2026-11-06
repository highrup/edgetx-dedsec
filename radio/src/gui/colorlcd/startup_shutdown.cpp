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

#include "startup_shutdown.h"

#include "edgetx.h"
#include "hal/abnormal_reboot.h"
#include "inactivity_timer.h"
#include "mainwindow.h"
#include "os/sleep.h"
#include "stamp.h"
#include "theme_manager.h"

constexpr const char* strip_leading_hyphen(const char* str) {
    return (str[0] == '-') ? str + 1 : str;
}

#if defined(VERSION_TAG)
const std::string ver_str = "" VERSION_TAG;
const std::string nam_str = "" CODENAME;
#else
const std::string ver_str = "" VERSION_PREFIX VERSION;
const std::string nam_str = strip_leading_hyphen("" VERSION_SUFFIX);
const std::string git_str = "(" GIT_STR ")";
#endif

const uint8_t __bmp_splash_logo[] __FLASH = {
#include "bmp_logo_edgetx_splash.lbm"
};

#include "dedsec_splash_data.inc"

static Window* splashScreen = nullptr;
static lv_obj_t* dedsecSplashCanvas = nullptr;
static uint8_t* dedsecSplashPacked = nullptr;
static uint8_t* dedsecSplashAlpha = nullptr;
static uint16_t dedsecSplashFrame = 0;

static uint16_t readSplashUint16(const uint8_t*& data)
{
  uint16_t value = data[0] | (data[1] << 8);
  data += 2;
  return value;
}

static void renderDedsecSplashFrame(uint16_t frame)
{
  const uint8_t* data =
      dedsecSplashFrameData + dedsecSplashFrameOffsets[frame];
  uint32_t position = 0;

  while (true) {
    uint16_t skip = readSplashUint16(data);
    if (skip == 0xffff) break;

    uint16_t count = readSplashUint16(data);
    position += skip;
    memcpy(dedsecSplashPacked + position, data, count);
    data += count;
    position += count;
  }

  for (uint32_t pixel = 0;
       pixel < DEDSEC_SPLASH_WIDTH * DEDSEC_SPLASH_HEIGHT; pixel += 1) {
    dedsecSplashAlpha[pixel] =
        (dedsecSplashPacked[pixel >> 3] & (0x80 >> (pixel & 7)))
            ? 0xff
            : 0x00;
  }
  lv_obj_invalidate(dedsecSplashCanvas);
}

static bool createDedsecSplash()
{
  dedsecSplashPacked = (uint8_t*)malloc(DEDSEC_SPLASH_PACKED_BYTES);
  dedsecSplashAlpha =
      (uint8_t*)malloc(DEDSEC_SPLASH_WIDTH * DEDSEC_SPLASH_HEIGHT);
  if (!dedsecSplashPacked || !dedsecSplashAlpha) {
    free(dedsecSplashPacked);
    free(dedsecSplashAlpha);
    dedsecSplashPacked = nullptr;
    dedsecSplashAlpha = nullptr;
    return false;
  }

  memset(dedsecSplashPacked, 0, DEDSEC_SPLASH_PACKED_BYTES);
  memset(dedsecSplashAlpha, 0,
         DEDSEC_SPLASH_WIDTH * DEDSEC_SPLASH_HEIGHT);

  dedsecSplashCanvas = lv_canvas_create(splashScreen->getLvObj());
  lv_canvas_set_buffer(dedsecSplashCanvas, dedsecSplashAlpha,
                       DEDSEC_SPLASH_WIDTH, DEDSEC_SPLASH_HEIGHT,
                       LV_IMG_CF_ALPHA_8BIT);
  etx_img_color(dedsecSplashCanvas, COLOR_WHITE_INDEX);
  lv_obj_set_pos(dedsecSplashCanvas, 0, 0);

  dedsecSplashFrame = 0;
  renderDedsecSplashFrame(0);
  return true;
}

static void updateDedsecSplash(tmr10ms_t elapsed)
{
  if (!dedsecSplashCanvas) return;

  uint16_t target = elapsed / (DEDSEC_SPLASH_FRAME_MS / 10);
  if (target >= DEDSEC_SPLASH_FRAME_COUNT)
    target = DEDSEC_SPLASH_FRAME_COUNT - 1;

  while (dedsecSplashFrame < target) {
    dedsecSplashFrame += 1;
    renderDedsecSplashFrame(dedsecSplashFrame);
  }
}

static void freeDedsecSplash()
{
  dedsecSplashCanvas = nullptr;
  free(dedsecSplashPacked);
  free(dedsecSplashAlpha);
  dedsecSplashPacked = nullptr;
  dedsecSplashAlpha = nullptr;
}

void drawSplash()
{
  if (!sdMounted()) sdInit();

  splashScreen = new Window(MainWindow::instance(), {0, 0, LCD_W, LCD_H});
  lv_obj_set_parent(splashScreen->getLvObj(), lv_layer_top());

  etx_solid_bg(splashScreen->getLvObj(), COLOR_BLACK_INDEX);

  if (!createDedsecSplash()) {
    auto bg = new StaticImage(splashScreen, {0, 0, LCD_W, LCD_H},
                              BITMAPS_PATH "/" SPLASH_FILE);
    bg->show(bg->hasImage());

    if (bg->hasImage()) {
      MainWindow::instance()->run();
      return;
    }

    LZ4Bitmap* logo = (LZ4Bitmap*)__bmp_splash_logo;
    coord_t x = (LANDSCAPE ? LCD_W / 3 : LCD_W / 2) - logo->width / 2;
    coord_t y = (LANDSCAPE ? LCD_H / 2 : LCD_H * 2 / 5) - logo->height / 2;
    new StaticLZ4Image(splashScreen, x, y, logo);

    coord_t w = LAYOUT_SCALE(200);
    x = (LANDSCAPE ? LCD_W * 4 / 5 : LCD_W / 2) - w / 2;
    y = LCD_H - EdgeTxStyles::STD_FONT_HEIGHT * 4;
    new StaticText(splashScreen, {x, y, w, EdgeTxStyles::STD_FONT_HEIGHT}, ver_str.c_str(), COLOR_GREY_INDEX, CENTERED);
    new StaticText(splashScreen, {x, y + EdgeTxStyles::STD_FONT_HEIGHT, w, EdgeTxStyles::STD_FONT_HEIGHT},
                   nam_str.c_str(), COLOR_GREY_INDEX, CENTERED);
#if !defined(VERSION_TAG)
    new StaticText(splashScreen, {x, y + EdgeTxStyles::STD_FONT_HEIGHT * 2, w, EdgeTxStyles::STD_FONT_HEIGHT},
                   git_str.c_str(), COLOR_GREY_INDEX, CENTERED);
#endif
  }

  // Refresh to show splash screen
  MainWindow::instance()->run();
}

static tmr10ms_t splashStartTime = 0;
static bool splashRunning = false;

void startSplash()
{
  if (!UNEXPECTED_SHUTDOWN()) {
    splashStartTime = get_tmr10ms();
    splashRunning = true;
    drawSplash();
  }
}

void cancelSplash()
{
  if (splashScreen) {
    splashScreen->closeWindow();
    freeDedsecSplash();
    splashScreen = nullptr;
    splashStartTime = 0;
  }
}

void waitSplash()
{
  // Handle color splash screen
  if (splashRunning) {
    inactivityCheckInputs();

#if defined(PWR_BUTTON_DUAL)
    // Wait for dual power buttons to be released
    while (pwrPressed()) {
      WDG_RESET();
      sleep_ms(10);
    }
    // Kill SYS and/or MDL events
    killAllEvents();
    MainWindow::instance()->run();
#endif

    // The splash window is created before the rest of radio initialization.
    // Start the animation clock here so those startup tasks do not consume and
    // visually skip the opening frames.
    splashStartTime = get_tmr10ms();
    tmr10ms_t animationDuration =
        DEDSEC_SPLASH_FRAME_COUNT * DEDSEC_SPLASH_FRAME_MS / 10;
    tmr10ms_t splashDuration =
        animationDuration > SPLASH_TIMEOUT ? animationDuration : SPLASH_TIMEOUT;
    tmr10ms_t splashEndTime = splashStartTime + splashDuration;

    MainWindow::instance()->blockUntilClose(true, [=]() {
      tmr10ms_t now = get_tmr10ms();
      updateDedsecSplash(now - splashStartTime);
      if (splashEndTime < now)
        return true;
      auto evt = getEvent();
      if (evt || inactivityCheckInputs()) {
        if (evt) killEvents(evt);
        return true;
      }
      return false;
    });

    // Reset timer so special/global functions set to !1x don't get triggered
    START_SILENCE_PERIOD();
  }

  cancelSplash();
}

static LAYOUT_VAL_SCALED(SHUTDOWN_CIRCLE_RADIUS, 75)

const int8_t bmp_shutdown_xo[] = {0, 0, -SHUTDOWN_CIRCLE_RADIUS,
                                  -SHUTDOWN_CIRCLE_RADIUS};
const int8_t bmp_shutdown_yo[] = {-SHUTDOWN_CIRCLE_RADIUS, 0, 0,
                                  -SHUTDOWN_CIRCLE_RADIUS};

static Window* shutdownWindow = nullptr;
static StaticIcon* shutdownAnim[4] = {nullptr};
static BitmapBuffer* shutdownSplashImg = nullptr;

void drawSleepBitmap()
{
  cancelShutdownAnimation();

  shutdownWindow = new Window(MainWindow::instance(), {0, 0, LCD_W, LCD_H});
  shutdownWindow->setWindowFlag(OPAQUE);
  etx_solid_bg(shutdownWindow->getLvObj(), COLOR_THEME_PRIMARY1_INDEX);

  (new StaticIcon(shutdownWindow, 0, 0, ICON_SHUTDOWN, COLOR_THEME_PRIMARY2_INDEX))
      ->center(LCD_W, LCD_H);

  // Force screen refresh
  lv_refr_now(nullptr);
}

void cancelShutdownAnimation()
{
  if (shutdownWindow) {
    // Detach before the parent deletes the canvas
    if (shutdownSplashImg) shutdownSplashImg->removeCanvas();
    shutdownWindow->closeWindow();
    shutdownWindow = nullptr;
    for (int i = 0; i < 4; i += 1) shutdownAnim[i] = nullptr;
  }
}

void drawShutdownAnimation(uint32_t duration, uint32_t totalDuration,
                           const char* message)
{
  if (totalDuration == 0) return;

  if (shutdownWindow == nullptr) {
    shutdownWindow =
        new Window(MainWindow::instance(), {0, 0, LCD_W, LCD_H});
    shutdownWindow->setWindowFlag(OPAQUE);
    etx_solid_bg(shutdownWindow->getLvObj(), COLOR_THEME_PRIMARY1_INDEX);

    if (sdMounted() && !shutdownSplashImg)
      shutdownSplashImg = BitmapBuffer::loadBitmap(
          BITMAPS_PATH "/" SHUTDOWN_SPLASH_FILE, BMP_RGB565);
    if (shutdownSplashImg)
      shutdownSplashImg->addCanvas(shutdownWindow);

    (new StaticIcon(shutdownWindow, 0, 0, ICON_SHUTDOWN, COLOR_THEME_PRIMARY2_INDEX))
        ->center(LCD_W, LCD_H);

    for (int i = 0; i < 4; i += 1) {
      shutdownAnim[i] = new StaticIcon(
          shutdownWindow, LCD_W / 2 + bmp_shutdown_xo[i],
          LCD_H / 2 + bmp_shutdown_yo[i],
          (EdgeTxIcon)(ICON_SHUTDOWN_CIRCLE0 + i), COLOR_THEME_PRIMARY2_INDEX);
    }
  }

  int quarter = 4 - (duration * 5) / totalDuration;
  if (quarter < 0) quarter = 0;
  for (int i = 3; i >= quarter; i -= 1) shutdownAnim[i]->hide();

  MainWindow::instance()->run();
}

Window* drawFatalErrorScreen(const char* message)
{
  auto w = new Window(MainWindow::instance(), {0, 0, LCD_W, LCD_H});
  w->setWindowFlag(OPAQUE);
  etx_solid_bg(w->getLvObj(), COLOR_BLACK_INDEX);

  new StaticText(w, rect_t{0, LCD_H / 2 - EdgeTxStyles::STD_FONT_HEIGHT, LCD_W, EdgeTxStyles::STD_FONT_HEIGHT * 2},
                 message, COLOR_WHITE_INDEX, FONT(XL) | CENTERED);

  return w;
}

void runFatalErrorScreen(const char* message)
{
  drawFatalErrorScreen(message);

  MainWindow::instance()->blockUntilClose(true, []() {
    return false;
  }, true);
}
