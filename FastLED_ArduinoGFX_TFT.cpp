/*-------------------------------------------------------------------------
  Arduino library based on Adafruit_Neomatrix but modified to work with SmartMatrix
  by Marc MERLIN <marc_soft@merlins.org>

  You should have received a copy of the GNU Lesser General Public
  License along with NeoMatrix.  If not, see
  <http://www.gnu.org/licenses/>.
  -------------------------------------------------------------------------*/

#include <FastLED_ArduinoGFX_TFT.h>

FastLED_ArduinoGFX_TFT::FastLED_ArduinoGFX_TFT(CRGB *__fb, const uint16_t tftw, const uint16_t tfth, 
	Arduino_TFT *gfx): 
  Framebuffer_GFX(__fb, tftw, tfth, NULL), _tftw(tftw), _tfth(tfth) {
      _gfx = gfx;
}

void FastLED_ArduinoGFX_TFT::begin() {
    uint16_t ms = _tftw * 2;
    Serial.print("Malloc FastLED_ArduinoGFX_TFT _line for bit depth adjustment. Bytes: ");
    Serial.println(ms);
    while ((_line = (uint16_t *) malloc(ms)) == NULL) Serial.println("malloc failed");
    Framebuffer_GFX::begin();
    show_free_mem("After FastLED_ArduinoGFX_TFT malloc");
    setfpsfreq(3000);
}


void FastLED_ArduinoGFX_TFT::show(boolean speedtest) {
    Framebuffer_GFX::showfps();
    _gfx->setAddrWindow(0, 0, _tftw, _tfth);
    for (uint16_t tftline = 0; tftline < _tfth; tftline++) {
	for (uint16_t i = 0; i < _tftw; i++) {
	    // If the FB is in PSRAM, reading from PSRAM is slow, allow bypassing
	    // this for simple writePixels benchmarking
	    if (!speedtest) _line[i] = Color24to16(CRGBtoint32(_fb[tftline*matrixWidth + i]));
	}

	yield();
	_gfx->startWrite();
	_gfx->writePixels(_line, _tftw);
	_gfx->endWrite();
    }
}

// vim:sts=4:sw=4
//
