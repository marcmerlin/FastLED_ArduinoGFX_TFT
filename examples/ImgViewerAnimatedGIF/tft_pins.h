// Seems that SSD1331 might require 80Mhz to work, while others prefer 24Mhz due to poor wiring
int tft_spi_speed = 40 * 1000 * 1000;

    /*  https://pinout.xyz/pinout/spi
    SD1331 Pin	    Arduino	ESP8266		ESP32	ESP32	rPi     rPi
    1 GND                                       VSPI    HSPI	SPI0    SPI1
    2 VCC
    3 SCL/SCK/CLK/D0	13	GPIO14/D5	18	14	BC11/22	BC21/40
    4 SDA/SDI/MOSI/D1	11	GPIO13/D7	23	13	BC10/19	BC20/38
    ---- 2 pins above and MISO are HWSPI, pins below are anything
    ---- RST is not part of SPI, it's an out of band signal to reset a TFT
    ---- This could be wired to the ESP32 EN(reset) pin
    5 RES/RST		9	GPIO15/D8	26	26	BC24				
    ---- Data/Command pin is not part of SPI but used to tell the TFT if incoming SPI
    ---- data is actually a command, or pixel data.
    6 DC/A0/RS (data)	8	GPIO05/D1	25	25	BC23				
    ---- Cable select chooses which SPI device we're talking to, if there is only
    ---- one, it can be tied to ground. Any pin is fine
    7 CS/SS => GND	10	GPIO04/D2	0	2	BC08			

    CS2: 2
    
    ---- MISO is not used to talk to TFTs, but is one of the 3 SPI hardware pins
      MISO		12	GPIO12/D6	19	12	BM11/23	BC19/35	
    */


// https://learn.adafruit.com/adafruit-2-dot-8-color-tft-touchscreen-breakout-v2?view=all
// HWSPI default

#if defined(ESP32)
// this is the TFT reset pin. Some boards may have an auto-reset circuitry on the breakout so this pin might not required but it can be helpful sometimes to reset the TFT if your setup is not always resetting cleanly. Connect to ground to reset the TFT
    #define TFT_RST 26 // Grey
    //#define TFT_RST -1 // Grey, can be wired to ESP32 EN to save a pin
    #define TFT_DC  25 // Purple
    //#define TFT_CS -1 // for display without CS pin
    #define TFT_CS  0 // White can be wired to ground
    #define TFT_CS2 2 // Orange can be wired to ground
    
    #define TFT_MOSI 23 // Blue
    #define TFT_CLK  18 // Green
    #define TFT_MISO 19 // Yellow
    #define TFT_BL 15
#else // ESP8266
    #define TFT_RST  15
    #define TFT_DC   5
    #define TFT_CS   4
    
    // You can use any (4 or) 5 pins
    // hwspi hardcodes those pins, no need to redefine them
    #define TFT_MOSI 13
    #define TFT_CLK  14
#endif
#define TFT_SCK TFT_CLK
