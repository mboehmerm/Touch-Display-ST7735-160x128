# Touch Display ST7735 1.8" 160x128

Aliexpress Display

![Touch_Display_1 8_SD](https://user-images.githubusercontent.com/106549358/229093910-0fbed4b9-aa86-4fae-a7b2-e4387fc9263f.png)

# Connections for ESP32 DevKitC V4
|             | GPIO | TFT   | Touch | SD-Card |
| :---------- | ---: | :---- | :---- | :------ |
| MOSI        | 23   | SDA   | T_DIN | SD_MOSI |
| MISO        | 19   |       | T_DO  | SD_MISO |
| CLK         | 18   | SCK   | T-CLK | SD_SCK  |
| DC          |  2   | SDA   |       |         |
| RST         | EN   | RESET |       |         |
| CS  (TFT)   | 15   | CS    |       |         |
| CS  (SD)    |  5   |       |       | SD_CS   |
| CS  (Touch) |  4   |       | T_CS  |         |
| GND         |      | GND   |       |         |
| 3.3V        |      | VCC   |       |         |
| 3.3V        |      | LED   |       |         |

# Configure the Library TFT_eSPI
Edit the file : C:\users\<username>\Documents\Arduino\TFT_eSPI\User_Setup.h

```
#define USER_SETUP_INFO "User_Setup"

// Driver
#define ST7735_DRIVER 
#define TFT_WIDTH  128
#define TFT_HEIGHT 160
#define ST7735_GREENTAB2

// Pins
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1
#define TOUCH_CS  4

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFFfree fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT

//Other options
#define SPI_FREQUENCY       40000000 
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
```
# Configure the Library LovyanGFX
Create the new file : Arduino\LovyanGFX\User_Setup.h\src\LGFX_ESP32_ST7735S.hpp

```
#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7735S  _panel_instance;
  lgfx::Bus_SPI        _bus_instance;
  lgfx::Touch_XPT2046  _touch_instance;
public:
  LGFX(void)
  { // Configure bus control settings.

      auto cfg = _bus_instance.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode    = 0;
      cfg.freq_write  = 40000000;
      cfg.freq_read   = 16000000;
      cfg.spi_3wire   = true;
      cfg.use_lock    = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = 18;            // SPI SCLK
      cfg.pin_mosi = 23;            // SPI MOSI
      cfg.pin_miso = 19;            // SPI MISO    (-1 = disable)
      cfg.pin_dc   =  2;            // SPI D/C A0  (-1 = disable)
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    { // Set display panel controls.

      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    15;  // CS    (-1 = disable)
      cfg.pin_rst          =    -1;  // RST   (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSY  (-1 = disable)

      cfg.panel_width      =   128;
      cfg.panel_height     =   160;
      cfg.offset_x         =     2;
      cfg.offset_y         =     1;
      cfg.offset_rotation  =     2;
      cfg.dummy_read_pixel =     8;
      cfg.dummy_read_bits  =     1;
      cfg.readable         =  true;
      cfg.invert           = false;
      cfg.rgb_order        =  true;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       =  true;

      _panel_instance.config(cfg);
    }
    { // Configure settings for touch screen control.  (delete if not necessary)

      auto cfg = _touch_instance.config();

      cfg.x_min      =   0;    // doesn't work
      cfg.x_max      = 127;
      cfg.y_min      =   0;
      cfg.y_max      = 159;
      //cfg.pin_int    = -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};
```