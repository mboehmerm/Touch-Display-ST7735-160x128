#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

// copy this file to Arduino\libraries\LovyanGFX\src\

// Setting example when using LovyanGFX with original settings on ESP32

/*
Duplicate this file, give it a new name, and change the settings to match your environment.
It can be used by including the created file from the user program.
You can put the duplicated file in the lgfx_user folder of the library and use it,
In that case, please note that it may be deleted when updating the library.
If you want to operate safely, make a backup or put it in the user project folder.
*/


/// Create a class that does your own settings, derived from LGFX_Device.
class LGFX : public lgfx::LGFX_Device
{
/*
  You can change the class name from "LGFX" to something else.
  When using with AUTODETECT, "LGFX" is used, so change it to something other than LGFX.
  Also, when using multiple panels at the same time, give each a different name.
  * When changing the class name, it is necessary to change the name of the constructor to the same name as well.
  You can decide how to name it freely, but assuming that the number of settings increases,
  For example, when setting ILI9341 for SPI connection with ESP32 DevKit-C,

   LGFX_DevKitC_SPI_ILI9341

  By using a name such as , and matching the file name and class name, it will be difficult to get lost when using it.
*/

// Prepare an instance that matches the type of panel to be connected.

//lgfx::Panel_GC9A01      _panel_instance;
//lgfx::Panel_GDEW0154M09 _panel_instance;
//lgfx::Panel_HX8357B     _panel_instance;
//lgfx::Panel_HX8357D     _panel_instance;
//lgfx::Panel_ILI9163     _panel_instance;
//lgfx::Panel_ILI9341     _panel_instance;
//lgfx::Panel_ILI9342     _panel_instance;
//lgfx::Panel_ILI9481     _panel_instance;
//lgfx::Panel_ILI9486     _panel_instance;
//lgfx::Panel_ILI9488     _panel_instance;
//lgfx::Panel_IT8951      _panel_instance;
//lgfx::Panel_RA8875      _panel_instance;
//lgfx::Panel_SH110x      _panel_instance; // SH1106, SH1107
//lgfx::Panel_SSD1306     _panel_instance;
//lgfx::Panel_SSD1327     _panel_instance;
//lgfx::Panel_SSD1331     _panel_instance;
//lgfx::Panel_SSD1351     _panel_instance; // SSD1351, SSD1357
//lgfx::Panel_SSD1963     _panel_instance;
//lgfx::Panel_ST7735      _panel_instance;
  lgfx::Panel_ST7735S     _panel_instance;
//lgfx::Panel_ST7789      _panel_instance;
//lgfx::Panel_ST7796      _panel_instance;

// Prepare an instance that matches the type of bus that connects the panels.
  lgfx::Bus_SPI       _bus_instance;   // SPI bus instance 
//lgfx::Bus_I2C       _bus_instance;   // I2C bus instance (ESP32 only)
//lgfx::Bus_Parallel8 _bus_instance;   // 8-bit parallel bus instance (ESP32 only)

// Prepare an instance if backlight control is possible.  (remove if not needed)
//lgfx::Light_PWM     _light_instance;

// Prepare an instance that matches the type of touch screen.  (remove if not needed)
//lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
//lgfx::Touch_GSL1680E_800x480 _touch_instance; // GSL_1680E, 1688E, 2681B, 2682B
//lgfx::Touch_GSL1680F_800x480 _touch_instance;
//lgfx::Touch_GSL1680F_480x272 _touch_instance;
//lgfx::Touch_GSLx680_320x320  _touch_instance;
//lgfx::Touch_GT911            _touch_instance;
//lgfx::Touch_STMPE610         _touch_instance;
//lgfx::Touch_TT21xxx          _touch_instance; // TT21100
  lgfx::Touch_XPT2046          _touch_instance;

public:

  // Create a constructor and set various settings here.
  // If you change the class name, specify the same name for the constructor.
  LGFX(void)
  {
    { // Configure bus control settings.

      auto cfg = _bus_instance.config();    // Get a structure for bus settings.

      // SPI bus settings
      cfg.spi_host = VSPI_HOST;          // Select SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // * Due to the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated,
      // so if an error occurs, use SPI2_HOST and SPI3_HOST instead.
      cfg.spi_mode    = 0;               // Set SPI communication mode (0 ~ 3)
      cfg.freq_write  = 40000000;        // SPI clock when sending (up to 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read   = 16000000;        // SPI clock when receiving
      cfg.spi_3wire   = true;            // Set true if receiving on the MOSI 
      cfg.use_lock    = true;            // Set true to use transaction lock
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting)
      // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel.  
      // Specifying 1ch and 2ch is deprecated.
      cfg.pin_sclk = 18;            // SPI SCLK
      cfg.pin_mosi = 23;            // SPI MOSI
      cfg.pin_miso = 19;            // SPI MISO    (-1 = disable)
      cfg.pin_dc   =  2;            // SPI D/C A0  (-1 = disable)
      // When using the same SPI bus as the SD card, be sure to set MISO without omitting it.
//
// Setting up the I2C bus
      //cfg.i2c_port    = 0;          // Select I2C port to use (0 or 1)
      //cfg.freq_write  = 400000;     // Clock Speed when sending
      //cfg.freq_read   = 400000;     // Clock Speed when receiving
      //cfg.pin_sda     = 21;         // SDA Pin number
      //cfg.pin_scl     = 22;         // SCL Pin nu,ber
      //cfg.i2c_addr    = 0x3C;       // I2C device address
//
// Set 8-bit parallel bus
      //cfg.i2s_port = I2S_NUM_0;     // Select I2S port to use (I2S_NUM_0 or I2S_NUM_1) (Use ESP32 I2S LCD mode)
      //cfg.freq_write = 20000000;    // transmit clock (maximum 20MHz, rounded to 80MHz divided by an integer)
      //cfg.pin_wr =  4;              // WR
      //cfg.pin_rd =  2;              // RD
      //cfg.pin_rs = 15;              // RS(D/C)
      //cfg.pin_d0 = 12;              // D0
      //cfg.pin_d1 = 13;              // D1
      //cfg.pin_d2 = 26;              // D2
      //cfg.pin_d3 = 25;              // D3
      //cfg.pin_d4 = 17;              // D4
      //cfg.pin_d5 = 16;              // D5
      //cfg.pin_d6 = 27;              // D6
      //cfg.pin_d7 = 14;              // D7
//
      _bus_instance.config(cfg);               // Reflect the setting value to the bus.
      _panel_instance.setBus(&_bus_instance);  // Sets the bus to the panel.

    }
    { // Set display panel controls.

      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

      cfg.pin_cs           =    15;  // CS    (-1 = disable)
      cfg.pin_rst          =    -1;  // RST   (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSY  (-1 = disable)

      // * The following setting values ​​are general initial values ​​for each panel, so please comment out any unknown items and try them.

      cfg.panel_width      =   128;  // actual visible width
      cfg.panel_height     =   160;  // actual visible height
      cfg.offset_x         =     2;  // Panel offset amount in X direction
      cfg.offset_y         =     1;  // Panel offset amount in Y direction
      cfg.offset_rotation  =     2;  // Rotation direction value offset 0~7 (4~7 is upside down)
      cfg.dummy_read_pixel =     8;  // Number of dummy read bits before pixel read
      cfg.dummy_read_bits  =     1;  // Number of dummy read bits before non-pixel data read
      cfg.readable         =  true;  // Data can be read set to true
      cfg.invert           = false;  // if panel light and dark are inverted set to true
      cfg.rgb_order        =  true;  // if panel red and blue are reversed set to true
      cfg.dlen_16bit       = false;  // Set to true for panels that transmit data length in 16-bit units with 16-bit parallel or SPI
      cfg.bus_shared       =  true;  // If the bus is shared with the SD card, set to true (bus control with drawJpgFile etc.)

      // Please set below only with drivers that can change the number of pixels such as ST7735 and ILI9163.
      // Please set the following only when the display shifts with a driver with a variable number of pixels such as ST7735 or ILI9163.
      //cfg.memory_width     =   128;  // Maximum width supported by the driver IC
      //cfg.memory_height    =   160;  // Maximum height supported by the driver IC

      _panel_instance.config(cfg);
    }

//*
    { // Set backlight control.  (delete if not necessary)

      //auto cfg   = _light_instance.config(); // Get the structure for backlight configuration.

      //cfg.pin_bl = 32;              // pin number to which the backlight is connected 
      //cfg.invert = false;           // true to invert the brightness of the backlight
      //cfg.freq   = 44100;           // Backlight PWM frequency
      //cfg.pwm_channel = 7;          // PWM channel number to use

      //_light_instance.config(cfg);
      //_panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }
//
    { // Configure settings for touch screen control.  (delete if not necessary)

      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;       // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max      = 127;     // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min      = 0;       // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max      = 159;     // Maximum Y value (raw value) obtained from touchscreen
    //cfg.pin_int    = -1;      // Pin number to which INT is connected (-1 = not connected)
      cfg.bus_shared = true;    // Set true when using a common bus with the screen
      cfg.offset_rotation = 0;  // Adjustment when the orientation does not match Set with a value from 0 to 7

// For SPI connection
      cfg.spi_host = VSPI_HOST; // Select SPI to use (HSPI_HOST or VSPI_HOST)
      cfg.freq     = 2500000;   // SPI Clock frequency 1000000 -> 2500000
      cfg.pin_sclk = 18;        // SCLK
      cfg.pin_mosi = 23;        // MOSI
      cfg.pin_miso = 19;        // MISO
      cfg.pin_cs   =  4;        // CS

// For I2C connection
      //cfg.i2c_port = 1;         // Select I2C to use (0 or 1)
      //cfg.i2c_addr = 0x38;      // I2C device address number
      //cfg.pin_sda  = 23;        // SDA Pin number 
      //cfg.pin_sc l = 32;        // SCL
      //cfg.freq     = 400000;    // set I2C clock fruequency

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }
//
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};

