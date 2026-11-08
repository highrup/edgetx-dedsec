/*
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
#include <algorithm>
#include "rgbleds.h"
#include "boards/generic_stm32/rgb_leds.h"
#include "definitions.h"
#include "dataconstants.h"

#if defined(FUNCTION_SWITCHES)
#if defined(FUNCTION_SWITCHES_RGB_LEDS)
static bool hasLedOverride[NUM_FUNCTIONS_SWITCHES] = { false };
static RGBLedColor ledOverride[NUM_FUNCTIONS_SWITCHES];
static int cfsEditOverride = -1;
static uint32_t cfsEditColor;

#if defined(PCBV12)
static uint32_t applyFrontLedMode(uint32_t color)
{
  if (g_eeGeneral.frontLedMode == FRONT_LED_MODE_NORMAL)
    return color;

  if (g_eeGeneral.frontLedMode == FRONT_LED_MODE_OFF)
    return 0;

  // Preserve the configured hue while reducing its brightest channel to the
  // lowest reliably visible level.
  constexpr uint8_t dimLevel = 10;
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;
  uint8_t peak = std::max(red, std::max(green, blue));
  if (peak == 0) return 0;
  red = ((uint16_t)red * dimLevel + peak / 2) / peak;
  green = ((uint16_t)green * dimLevel + peak / 2) / peak;
  blue = ((uint16_t)blue * dimLevel + peak / 2) / peak;
  return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}
#endif

void setFSEditOverride(int index, uint32_t color)
{
  cfsEditOverride = index;
  cfsEditColor = color;
}

void setFSLedOverride(uint8_t index, bool state, uint8_t r, uint8_t g, uint8_t b)
{
  hasLedOverride[index] = state;
  ledOverride[index].r = r;
  ledOverride[index].g = g;
  ledOverride[index].b = b;
}

void setFSLedOFF(uint8_t index) {
  if (g_model.getSwitchType(index) != SWITCH_NONE) {
    uint32_t color;
    if (cfsEditOverride >= 0) {
      uint8_t cfsIdx = switchGetCustomSwitchIdx(cfsEditOverride);
      color = cfsEditColor;
      fsLedRGB(cfsIdx,
#if defined(PCBV12)
               applyFrontLedMode(color)
#else
               color
#endif
      );
    } else {
      uint8_t cfsIdx = switchGetCustomSwitchIdx(index);
      if (hasLedOverride[cfsIdx] && g_model.getSwitchOffColorLuaOverride(index))
        color = ledOverride[cfsIdx].getColor();
      else
        color = g_model.getSwitchOffColor(index).getColor();
      fsLedRGB(cfsIdx,
#if defined(PCBV12)
               applyFrontLedMode(color)
#else
               color
#endif
      );
    }
  }
}

void setFSLedON(uint8_t index) {
  if (g_model.getSwitchType(index) != SWITCH_NONE) {
    uint32_t color;
    if (cfsEditOverride >= 0) {
      uint8_t cfsIdx = switchGetCustomSwitchIdx(cfsEditOverride);
      color = cfsEditColor;
      fsLedRGB(cfsIdx,
#if defined(PCBV12)
               applyFrontLedMode(color)
#else
               color
#endif
      );
    } else {
      uint8_t cfsIdx = switchGetCustomSwitchIdx(index);
      if (hasLedOverride[cfsIdx] && g_model.getSwitchOnColorLuaOverride(index))
        color = ledOverride[cfsIdx].getColor();
      else
        color = g_model.getSwitchOnColor(index).getColor();
      fsLedRGB(cfsIdx,
#if defined(PCBV12)
               applyFrontLedMode(color)
#else
               color
#endif
      );
    }
  }
}

bool getFSLedState(uint8_t index) {
  uint8_t cfsIdx = switchGetCustomSwitchIdx(index);
  return fsGetLedRGB(cfsIdx) == g_model.getSwitchOnColor(index).getColor();
}

uint32_t getFSLedRGBColor(uint8_t index)
{
  return fsGetLedRGB(index);
}
#else
void setFSLedOFF(uint8_t index) {
  index = switchGetCustomSwitchIdx(index);
  fsLedOff(index);
}

void setFSLedON(uint8_t index) {
  index = switchGetCustomSwitchIdx(index);
  fsLedOn(index);
}

bool getFSLedState(uint8_t index) {
  index = switchGetCustomSwitchIdx(index);
  return fsLedState(index);
}
#endif
#endif  // FUNCTION_SWITCHES

void applyFrontLedModeNow()
{
#if defined(PCBV12) && defined(FUNCTION_SWITCHES_RGB_LEDS)
  evalFunctionSwitches();
#endif
}

void turnOffRGBLeds()
{
  rgbLedClearAll();
}
