/*******************************************************************************
 * Animated GIF Image Viewer
 * This is a simple Animated GIF image viewer exsample
 * Image Source: https://www.pexels.com/video/earth-rotating-video-856356/
 * cropped: x: 598 y: 178 width: 720 height: 720 resized: 240x240
 * optimized with ezgif.com
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload Animated GIF file
 *   SPIFFS (ESP32):
 *     upload SPIFFS data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/me-no-dev/arduino-esp32fs-plugin
 *   LittleFS (ESP8266):
 *     upload LittleFS data with ESP8266 LittleFS Data Upload:
 *     ESP8266: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
 *   SD:
 *     Most Arduino system built-in support SD file system.
 *     Wio Terminal require extra dependant Libraries:
 *     - Seeed_Arduino_FS: https://github.com/Seeed-Studio/Seeed_Arduino_FS.git
 *     - Seeed_Arduino_SFUD: https://github.com/Seeed-Studio/Seeed_Arduino_SFUD.git
 ******************************************************************************/

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#define NO_TFT_SPI_PIN_DEFAULTS
#include <Arduino_GFX_Library.h>

#ifdef SINGLEDEVICE
#include "arduinogfx.h"
#else
#include "tft_pins.h"
Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
Arduino_DataBus *bus2 = new Arduino_HWSPI(TFT_DC, TFT_CS2);

// SSD1331 OLED 96x64
// do not add 4th IPS argument, even FALSE
// Do not reset first device when setting 2nd device
Arduino_SSD1331 *gfx1 = new Arduino_SSD1331(bus, TFT_RST, 2 /* rotation */);
// Do not reset 2nd device, it would unsetup the first
Arduino_ILI9341 *gfx2 = new Arduino_ILI9341(bus2, -1, 3 /* rotation */);
#endif

// Choose one screen or the other, here.
#ifndef SCREEN2
Arduino_GFX *gfx = gfx1;
#define GIF_FILENAME "/kiss2.gif"
#else
Arduino_GFX *gfx = gfx2;
#define GIF_FILENAME "/invaders_anim.gif"
#endif

#include "FS.h"
#include <LITTLEFS.h>

#include "GifClass.h"
static GifClass gifClass;

void setup()
{
  Serial.begin(115200);
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  // Init Display
  gfx->begin(tft_spi_speed);
  gfx->fillScreen(BLACK);

  gfx->setCursor(10, 10);
  gfx->setTextColor(RED);
  gfx->println("Hello World!");

  if (!LITTLEFS.begin())
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
  }
  else
  {
    File dir = LITTLEFS.open("/");
    while (File file = dir.openNextFile()) {
	Serial.print("FS File: ");
	Serial.print(file.name());
	Serial.print(" Size: ");
	Serial.println(file.size());
	close(file);
    }

    Serial.printf("File: %s\n", GIF_FILENAME);

    File gifFile = LITTLEFS.open(GIF_FILENAME, "r");
    if (!gifFile || gifFile.isDirectory())
    {
      Serial.println(F("ERROR: open gifFile Failed!"));
      gfx->println(F("ERROR: open gifFile Failed!"));
    }
    else
    {
      // read GIF file header
      gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
      if (!gif)
      {
        Serial.println(F("gd_open_gif() failed!"));
      }
      else
      {
        uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
        if (!buf)
        {
          Serial.println(F("buf malloc failed!"));
        }
        else
        {
          int16_t x = (gfx->width() - gif->width) / 2;
          int16_t y = (gfx->height() - gif->height) / 2;

          Serial.println(F("GIF video start"));
          uint32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
          int32_t res;
          uint32_t duration = 0, remain = 0;
          while (1)
          {
            t_fstart = millis();
            t_delay = gif->gce.delay * 10;
            res = gifClass.gd_get_frame(gif, buf);
            if (res < 0)
            {
              Serial.println(F("ERROR: gd_get_frame() failed!"));
              break;
            }
            else if (res == 0)
            {
              Serial.print(F("rewind, duration: "));
              Serial.print(duration);
              Serial.print(F(", remain: "));
              Serial.print(remain);
              Serial.print(F(" ("));
              Serial.print(100.0 * remain / duration);
              Serial.println(F("%)"));
              duration = 0;
              remain = 0;
              gifClass.gd_rewind(gif);
              continue;
            }

            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);

            t_real_delay = t_delay - (millis() - t_fstart);
            duration += t_delay;
            remain += t_real_delay;
            delay_until = millis() + t_real_delay;
            do
            {
              delay(1);
            } while (millis() < delay_until);
          }
          Serial.println(F("GIF video end"));
          Serial.print(F("duration: "));
          Serial.print(duration);
          Serial.print(F(", remain: "));
          Serial.print(remain);
          Serial.print(F(" ("));
          Serial.print(100.0 * remain / duration);
          Serial.println(F("%)"));

          gifClass.gd_close_gif(gif);
          free(buf);
        }
      }
    }
  }
}

void loop()
{
}
