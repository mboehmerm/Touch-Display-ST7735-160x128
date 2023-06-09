# SPI Touch Display ST7735 1.8" 160x128 XPT2046

Aliexpress Display ~5€, tested with a ESP32 DevKitC V4 and Arduino IDE 2.0.4 / [PlatformIO](#pla)

Display works with [TFT_eSPI by Bodmer](#espi), [LovyanGFX](#lov) and [LVGL 8.3.6](#lvgl)

Speed about 120fps ( DMA_Bouncy_Circles.ino )

![Touch_Display_1 8_SD](pictures/Touch_Display_1.8_SD.png)

## Connections for ESP32 DevKitC V4

Share SPI MOSI, MISO and CLK, so you need only 11 or 12 Pin's.

| GPIO | TFT   | Touch | SD-Card |                |
| ---: | :---- | :---- | :------ | :------------- |
| 23   | SDA   | T_DIN | SD_MOSI | MOSI           |
| 19   |       | T_DO  | SD_MISO | MISO           |
| 18   | SCK   | T-CLK | SD_SCK  | CLK            |
|  2   | A0    |       |         | DC             |
| EN   | RESET |       |         | RST            |
| 15   | CS    |       |         | CS  (TFT)      |
|  5   |       |       | SD_CS   | CS  (SD)       |
|  4   |       | T_CS  |         | CS  (Touch)    |
|      |       | T_IRQ |         | IRQ (not used) |
|      | GND   |       |         | GND            |
|      | VCC   |       |         | 3.3V           |
|      | LED   |       |         | 3.3V           |

## Install and configure the library TFT_eSPI <a name="espi"></a>
- Default Windows Arduino folder : C:\Users\<username>\Documents\Arduino
- Edit the file : Arduino\libraries\TFT_eSPI\User_Setup.h
- Test : TFT_graphicstest_PDQ3_Touch.ino (works with TFT_eSPI or LovyanGFX)
- Test : LittleFS_SDcard_Touch_Jpeg_Test.ino (confirms that all three SPI devices work)

![TFT_eSPI](pictures/TFT_graphicstest_PDQ3_Touch.jpg)

Edit Arduino\libraries\TFT_eSPI\User_Setup.h :

```java
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
//#define LOAD_FONT8N
#define LOAD_GFXFF
#define SMOOTH_FONT

//Other options
#define SPI_FREQUENCY       40000000 // 80000000 works with errors
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2000000
```
Speedtest : Arduino\libraries\TFT_eSPI\examples\DMA test\Bouncy_Circles\Bouncy_Circles.ino
## Alternative configuration of the library TFT_eSPI 2.5.0
- Create a new File with your configuration data : Arduino\libraries\Setup401_ST7735_Touch.h 
- Edit the file : Arduino\libraries\TFT_eSPI\User_Setup_Select.h

Edit Arduino\libraries\TFT_eSPI\User_Setup_select.h :
```java
// Only **ONE** line below should be uncommented to define your setup.

//#include <User_Setup.h>              // Default setup is root library folder

// new setup file in folder Arduino/libraries (updates will not overwrite your setups)
#include <../Setup401_ST7735_Touch.h>  // new : Setup file for  ST7735 1.8" with touch, ST7735_GREENTAB2

```

## Install and configure the library LovyanGFX 1.1.5 <a name="lov"></a>
- Create the file : Arduino\libraries\LovyanGFX\src\LGFX_ESP32_ST7735S.hpp
- Test : Lovyan_Touch_Test.ino and TFT_graphicstest_PDQ3_Touch.ino

![LovyanGFX](pictures/Lovyan_Touch_Test_02.jpg)

LGFX_ESP32_ST7735S.hpp :

```c++
#pragma once

#define  LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX :  public lgfx::LGFX_Device
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

      cfg.x_min      =   0;    // numbers doesn't work
      cfg.x_max      = 127;
      cfg.y_min      =   0;
      cfg.y_max      = 159;
    //cfg.pin_int    =  -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};
```
## Install and configure LVGL 8.3.6 <a name="lvgl"></a>
- First configure the Library TFT_eSPI. 
- Install only LVGL 8.3.6 "Note that the examples and demos are now part of the main LVGL library."
- Create and edit the file : "Arduino\libraries\lv_conf.h"
  - copy "Arduino\libraries\lvgl\lv_conf_template.h" to "Arduino\libraries\lv_conf.h"
  - Edit "Arduino\libraries\lv_conf.h"
- Modify the lvgl library :
  - Copy the folder "Arduino\libraries\lvgl\demos" to "Arduino\libraries\lvgl\ **src**\demos"
  - Copy the folder "Arduino\libraries\lvgl\examples" to "Arduino\libraries\lvgl\ **src**\examples"
  - Three files can be modified so it fits better on the small display (not necessary) :
  - Edit "Arduino\libraries\lvgl\ **src**\demos\benchmark\lv_demo_benchmark.c"
  - Edit "Arduino\libraries\lvgl\src\demos\keypad_encoder\lv_demo_keypad_encoder.c"
  - Edit "Arduino\libraries\lvgl\src\demos\widgets\lv_demo_widgets.c"
- Test : LVGL_Arduino_rx1.8.ino
  - different rotation : modify the line "#define SCREEN_ROTATION **0**"
  - try other demo or example : uncomment **ONE** of the lines starting with "lv_demo_" or "lv_example_"

LVGL_Arduino_rx1.8 with rotation 0 (lv_demo_widgets.c modified)
![LVGL](pictures/LVGL_Widget_R0_Profile.JPG)

LVGL_Arduino_rx1.8 with rotation 3 (lv_demo_widgets.c modified)
![LVGL](pictures/LVGL_Widget_R3_Analytics.JPG)

LVGL_Arduino_rx1.8 with rotation 0 (lv_demo_widgets.c modified)
![LVGL](pictures/LVGL_Widget_R0_Analytics.JPG)


LVGL_Arduino_rx with rotation 0 and lv_example_btn_1();
![LVGL example](pictures/LVGL_example.jpg)
- Doku : https://docs.lvgl.io/master/examples.html

Modified lines in Arduino\libraries\lv_conf.h :
```java
#if 1                            /* Set it to "1" to enable content */
#define LV_COLOR_DEPTH 16
#define LV_TICK_CUSTOM 1         // Touch works only with 1
#define LV_USE_LOG 1             // See LVGL-warnings in the serial monitor
#define LV_FONT_MONTSERRAT_8  1  // small fonts for the small display
#define LV_FONT_MONTSERRAT_10 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_DEFAULT &lv_font_montserrat_10
#define LV_USE_DEMO_WIDGETS 1                   // Enable demos
#define LV_DEMO_WIDGETS_SLIDESHOW 1
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 1
#define LV_USE_DEMO_BENCHMARK 1
#define LV_USE_DEMO_MUSIC 1
```

The LVGL 8.3.3 library, i first used and modified can be downloaded from the link below. It contains the first LVGL_Arduino.ino that I got working without major modifications. LVGL_Arduino_Test.ino based on this file.

Download "2.8inch_ESP32-2432S028R.rar" from : http://pan.jczn1688.com/directlink/1/ESP32%20module/2.8inch_ESP32-2432S028R.rar?spm=a2g0o.detail.1000023.1.6e8869f0pW0rBB&file=2.8inch_ESP32-2432S028R.rar

## Use Visual Studio Code and PlatformIO <a name="pla"></a>

![TFT_eSPI](pictures/ScreenShot_PlatformIO.jpg)

More photos and screenshots are in this similar repository : [/mboehmerm/Touch-Display-ili9341-320x240](https://github.com//mboehmerm/Touch-Display-ili9341-320x240)

Test PlatformIO with the TFT_eSPI graphicstest example :
- Default Windows PlatformIO folder : C:\Users\ "username"\Documents\PlatformIO
- Install Visual Studio Code and PlatformIO
  - https://platformio.org/install/ide?install=vscode
- Create a new project in PlatformIO
  - Press PIO Home ![PlatfomIO](pictures/PlatformIO_Bottom_Home.jpg) then click on ![New Project](pictures/PlatformIO_New_Project_Button.jpg) 
  - Name : ST7735_graphicstest_PDQ_Touch
  - Board : Espressif ESP32 Dev Module
  - Framework : Arduino
- Edit [platformio.ini](/PlatformIO/Projects/ST7735_graphicstest_PDQ_Touch/platformio.ini) 
- Edit [src/main.cpp](/PlatformIO/Projects/ST7735_graphicstest_PDQ_Touch/src/main.cpp)
- Before uploading :
  - If necessary set environment to "Default (ST7735_graphicstest_PDQ_Touch)"
  - Connect the ESP32 DevKitC v4
  - Edit in "platformio.ini" monitor_port=COM13 
- Upload (Press 
 ![PlatfomIO](pictures/PlatformIO_Bottom_Arrow.jpg) ) 

Use PlatformIO with the LVGL Arduino Demo :

- Create a new project in PlatformIO
  - Name : ST7735_LVGL
  - Board : Espressif ESP32 Dev Module
  - Framework : Arduino
- Edit [platformio.ini](/PlatformIO/Projects/ST7735_LVGL/platformio.ini )
- Edit [main.c](/PlatformIO/Projects/ST7735_LVGL/src/main.cpp )
- In the folder "PlatformIO\Projects\ST7735_LVGL\ .pio\libdeps\esp32dev\lvgl\"
  - copy the folder "demos" to "src\demos"
  - copy the folder "examples" to "src\examples"
- Modify src/main.cpp (not necessary) : 
  - different rotation : modify the line "#define SCREEN_ROTATION **0**"
  - try other demo or example : uncomment **ONE** of the lines starting with "lv_demo_" or "lv_example_"
  - Three files can be modified so it fits better on the small display (not necessary) :
    - Edit "ST7735_LVGL/.pio/libdeps/esp32dev/lvgl/**src**/demos/benchmark/lv_demo_benchmark.c"
- Before uploading :
  - If necessary set environment to "Default ( ST7735_LVGL )"
  - Connect the ESP32 DevKitC v4
- Upload (Press 
 ![PlatfomIO](pictures/PlatformIO_Bottom_Arrow.jpg) ) 

## Use  PlatformIO with lv_conf.h , User_Setup.h and User_Setup_Select.h
- No build_flags nesessary in platform.ini for TFT_eSPI and lvgl (see below)
- The library folder [/PlatformIO/Projects/ST7735_LVGL/.pio/libdeps/esp32dev](/PlatformIO/Projects/ST7735_LVGL/.pio/libdeps/esp32dev) contains the libraries and the configuration files for TFT_eSPI and lvgl
- "lv_conf.h" should be created here : [/PlatformIO/Projects/ST7735_LVGL/.pio/libdeps/esp32dev/lv_conf.h)](/PlatformIO/Projects/ST7735_LVGL/.pio/libdeps/esp32dev/lv_conf.h)

platform.ini :
```c
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
	lvgl/lvgl@^8.3.4
	bodmer/TFT_eSPI@^2.5.23
monitor_port=COM13
monitor_speed=115200
```


<!-- unvisible -->
