/*-------------------------------------------------------------------------
  Arduino library based on Adafruit_Neomatrix but modified to work with SmartMatrix
  by Marc MERLIN <marc_soft@merlins.org>

  You should have received a copy of the GNU Lesser General Public
  License along with NeoMatrix.  If not, see
  <http://www.gnu.org/licenses/>.
  -------------------------------------------------------------------------*/

#include <FastLED_ArduinoGFX_TFT.h>

// This library allows having a framebuffer that is smaller than the size of the TFT. This is needed
// in case the TFT is too large for the amount of RAM to hold everything in RAM. For instance, 
// ILI9341 is 320x240 which in 24bpp (this library requires 24bpp to be FastLED CRGB compatible), 
// takes 224KB. The biggest block of RAM you can get on ESP32, is around 180KB, unless you use PSRAM
// but PSRAM is 10 times slower and affects the refresh rate (slowing it down to 5fps or so).
// One solution around this, is to have a framebuffer that is 320x120 (112KB), render the first half
// of the screen in the FB, push it to the screen, and then render the second half of the screen.
// If you are using straight GFX, it's honestly better to skip this library and the framebuffer altogeher,
// but if you are using FastLED/LEDMatrix code, then you need the FastLED CRGB NeoMatrix compatible layer
FastLED_ArduinoGFX_TFT::FastLED_ArduinoGFX_TFT(CRGB *__fb, const uint16_t fbw, const uint16_t fbh, 
	Arduino_TFT *gfx): 
  Framebuffer_GFX(__fb, fbw, fbh, NULL), _fbw(fbw), _fbh(fbh) {
      _gfx = gfx;
}

void FastLED_ArduinoGFX_TFT::begin() {
    uint16_t ms = _fbw * 2;
    Serial.print("Malloc FastLED_ArduinoGFX_TFT _line for bit depth adjustment. Bytes: ");
    Serial.println(ms);
    while ((_line = (uint16_t *) malloc(ms)) == NULL) Serial.println("malloc failed");
    Framebuffer_GFX::begin();
    show_free_mem("After FastLED_ArduinoGFX_TFT malloc");
    setfpsfreq(3000);
}


// If the framebuffer is smaller than the TFT size, you can set an offset to display
// the framebuffer
void FastLED_ArduinoGFX_TFT::show(uint16_t x, uint16_t y, boolean speedtest) {
    Framebuffer_GFX::showfps();
    _gfx->setAddrWindow(x, y, _fbw, _fbh);
    _gfx->startWrite();
    for (uint16_t tftline = 0; tftline < _fbh; tftline++) {
	for (uint16_t i = 0; i < _fbw; i++) {
	    // If the FB is in PSRAM, reading from PSRAM is slow, allow bypassing
	    // this for simple writePixels benchmarking
	    // Actually CRGBtoint32 is also quite slow and brings speed from 33fps down to 19fps
            // Without the Color24to16, 19fps goes to 24fps
            // Without both convertions, 24fps goes to 29fps (the last 4fps are the time to store in _line[i])
	    // for 320x240 at 80Mhz. One way to make it faster is to use .raw(), shove it directly
	    // into a uint32_t (masking the highest byte that's not used) and deal with endianness
	    if (!speedtest) _line[i] = Color24to16(CRGBtoint32(_fb[tftline*matrixWidth + i]));
	}

	yield();
	_gfx->writePixels(_line, _fbw);
    }
    _gfx->endWrite();
}

// vim:sts=4:sw=4
//
