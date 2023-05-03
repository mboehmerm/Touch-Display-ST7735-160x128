#define USER_SETUP_ID 411

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
// Pins
#define TFT_CS    10  // 10 or 34 (FSPI CS0) 
#define TFT_MOSI  11  // 11 or 35 (FSPI D)
#define TFT_SCLK  12  // 12 or 36 (FSPI CLK)
#define TFT_MISO  13  // 13 or 37 (FSPI Q)
#define TFT_DC     7  // Data Command control pin
#define TFT_RST   -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN

#define TOUCH_CS   6  // Chip select pin (T_CS) of touch screen

// Fonts
#define LOAD_GLCD     // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2    // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4    // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6    // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7    // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8    // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF    // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// FSPI / VSPI port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
//#define USE_HSPI_PORT

// Other options
#define SPI_FREQUENCY       40000000  //  32.4 fps. works fine. 
//#define SPI_FREQUENCY     80000000  // ? 63.0 fps. works but problems examples Bouncy_Circles, boing_ball (DMA)

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY   6000000  // ? 20000000 unstable

#define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS
