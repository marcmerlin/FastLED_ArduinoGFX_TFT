/*--------------------------------------------------------------------
  Arduino library based on Adafruit_Neomatrix but modified to work with SmartMatrix
  by Marc MERLIN <marc_soft@merlins.org>

  Original notice and license from Adafruit_Neomatrix:
  NeoMatrix is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoMatrix is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoMatrix.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef _FastLED_ArduinoGFX_TFT_H_
#define _FastLED_ArduinoGFX_TFT_H_
#include "Framebuffer_GFX.h"
#include <Arduino_GFX_Library.h>
#include "FastLED.h"

class FastLED_ArduinoGFX_TFT : public Framebuffer_GFX {
  public:
    FastLED_ArduinoGFX_TFT(CRGB *, uint16_t, uint16_t, Arduino_TFT *gfx);
    void show(uint16_t x=0, uint16_t y=0, boolean speedtest=0);
    void begin();

  protected:
    Arduino_TFT *_gfx;

  private:
    const uint16_t _fbw, _fbh;
    // temporary storage for 24bit to 16bit convertion
    uint16_t *_line;
};

#endif // _FastLED_ArduinoGFX_TFT_H_
// vim:sts=4:sw=4
