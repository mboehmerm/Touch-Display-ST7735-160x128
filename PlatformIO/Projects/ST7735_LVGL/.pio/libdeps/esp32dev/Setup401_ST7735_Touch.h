/* ----------------------------------------------------------------------------------------------------
// Edit Arduino\libraries\TFT_eSPI\User_Setup_select.h :

// Only **ONE** line below should be uncommented to define your setup.
//#include <User_Setup.h>                                 // Default setup is root library folder

// new setup file in folder Arduino/libraries/TFT_eSPI/User_Setups
//#include <User_Setups/Setup401_ST7735_Touch.h>          // new : Setup file for  ST7735 1.8" with touch, ST7735_GREENTAB2

// new setup file in folder Arduino/libraries (updates will not overwrite your setups)
#include <../Setup401_ST7735_Touch.h>                     // new : Setup file for  ST7735 1.8" with touch, ST7735_GREENTAB2

---------------------------------------------------------------------------------------------------- */ 

// See SetupX_Template.h for all options available
#define USER_SETUP_ID 401

// Driver
#define ST7735_DRIVER

#define TFT_WIDTH  128
#define TFT_HEIGHT 160

#define ST7735_GREENTAB2

// For ST7735, ST7789 and ILI9341 ONLY, define the colour order IF the blue and red are swapped on your display
// Try ONE option at a time to find the correct colour order for your display

//  #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
//  #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Pins
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN

#define TOUCH_CS 4     // Chip select pin (T_CS) of touch screen

// Fonts
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// Other options
#define SPI_FREQUENCY  40000000    // works fine, 80000000 with some errors

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

#define SPI_TOUCH_FREQUENCY  2000000 // otherwise problems on one corner with calibration

// #define SUPPORT_TRANSACTIONS
