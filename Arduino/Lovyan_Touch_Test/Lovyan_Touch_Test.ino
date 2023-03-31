
#include <LovyanGFX.hpp>

//  Modified version of : Arduino\libraries\LovyanGFX\examples\Lovyan_user_setting.ino
//  Also modified is    : Arduino\libraries\LovyanGFX\src\lgfx\v1\LGFXBase.cpp ( LGFX_Device::calibrate_touch )
//                        Serial monitor shows now the calibration data, if you call display.setTouchCalibrate(calData);

//  Create a class that does your own settings, derived from LGFX_Device. 
class LGFX : public lgfx::LGFX_Device
{ 
  lgfx::Panel_ST7735S      _panel_instance;
  lgfx::Bus_SPI            _bus_instance;
  lgfx::Touch_XPT2046      _touch_instance;

public:

  LGFX(void) 
  { 
    {  //  Configure bus control settings. 
      auto cfg = _bus_instance.config();  //  Get the structure for the bus configuration. 

      //  SPI bus configuration 
      cfg.spi_host    = VSPI_HOST;        //  Select the SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST 
      //  * Due to the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated, so if an error occurs, use SPI2_HOST and SPI3_HOST instead. 
      cfg.spi_mode    = 0;                //  Set SPI communication mode (0 ~ 3)
      cfg.freq_write  = 60000000;         //  SPI clock frequency when sending (max 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read   = 16000000;         //  SPI clock frequency when receiving 
      cfg.spi_3wire   = true;             //  Set true if receiving on the MOSI 
      cfg.use_lock    = true;             //  set true to use transaction 
      cfg.dma_channel = SPI_DMA_CH_AUTO;  //  Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting) 
      //  * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel.   Specifying 1ch and 2ch is deprecated. 
      cfg.pin_sclk    = 18;               //  SPI SCLK
      cfg.pin_mosi    = 23;               //  SPI MOSI
      cfg.pin_miso    = 19;               //  SPI MISO      (-1 = disable) 
      cfg.pin_dc      =  2;               //  SPI D/C = A0  (-1 = disable) 
      //  When using the same SPI bus as the SD card, be sure to set MISO without omitting it. 
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // Set display panel controls.
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    15;  // CS    (-1 = disable)
      cfg.pin_rst          =    -1;  // RST   (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSY  (-1 = disable)

      // * The following setting values ​​are general initial values ​​for each panel, so please try commenting out any unknown items. 

      cfg.panel_width      =   128;  // actual displayable width
      cfg.panel_height     =   160;  // actual displayable  height
      cfg.offset_x         =     2;  // Panel offset in X 
      cfg.offset_y         =     1;  // Panel offset in Y
      cfg.offset_rotation  =     2;  // Rotation value offset 0~7 (4~7 are upside down 
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

    { // Configure settings for touch screen control.  (delete if not necessary)

      auto cfg = _touch_instance.config();

      // numbers for x_min etc. doesn't work
      cfg.x_min           =  330;  // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max           = 3500;  // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min           =  230;  // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max           = 3580;  // Maximum Y value (raw value) obtained from touchscreen
      cfg.pin_int         =   -1;  // Pin number to which INT is connected (-1 = not connected)
      cfg.bus_shared      = true;  // Set true when using a common bus with the screen
      cfg.offset_rotation =    0;  // Adjustment when the orientation does not match Set with a value from 0 to 7
                                   // 6 passt, aber doppelte Linien  
      // For SPI connection
      cfg.spi_host = VSPI_HOST;   // Select SPI to use (HSPI_HOST or VSPI_HOST)
      cfg.freq     = 2500000;     // SPI Clock frequency 1000000 -> 2500000
      cfg.pin_sclk = 18;          // SCLK
      cfg.pin_mosi = 23;          // MOSI
      cfg.pin_miso = 19;          // MISO
      cfg.pin_cs   =  4;          // CS

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};

//  Create an instance of the prepared class. 
LGFX display;

void setup(void)
{
  Serial.begin(115200);
  // Enabled after SPI bus and panel initialization.
  display.init();

  display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);
  
  display.setRotation(0);
  display.drawString("Top left", 10, 10);

  // Calibrate if touch is enabled.  (Optional)
  if (display.touch())
  {
    if (display.width() < display.height()) display.setRotation(display.getRotation() ^ 1);
    // Draw the guidance text on the screen.
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("touch the arrow.", display.width()>>1, display.height() >> 1);
    display.setTextDatum(textdatum_t::top_left);

    // If using touch, calibrate. Touch the tips of the arrows displayed in the four corners of the screen in order.
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (display.isEPD()) std::swap(fg, bg);
    display.calibrateTouch(nullptr, fg, bg, std::max(display.width(), display.height()) >> 3);
      
      // modified in LovyanGFX\src\lgfx\v1\LGFXBase.ccp  LGFX_Device::calibrate_touch
      //...
      //panel()->setCalibrate(orig);
      //setRotation(rot);
      //Serial.print("Calibration data : ");
      //for (int j=0; j < 8;  j++) {
      //  Serial.print(orig[j]); 
      //  Serial.print(", ");
      //}
      //Serial.println("done");
      //...

    //Calibration data for offset_rotation of the panel = 2 and offset_rotation of the touch = 0

    //Calibration data : 3839, 361, 3714, 3809, 521, 325, 464, 3832, done
    //Calibration data : 3910, 297, 3583, 3870, 470, 322, 609, 3845, done
    //Calibration data : 3961, 293, 3967, 3818, 452, 315, 725, 3843, done
    //Calibration data : 3967, 275, 3583, 3868, 458, 297, 797, 3789, done
    //Calibration data : 3781, 293, 3966, 3804, 449, 262, 780, 3805, done
    //Calibration data : 3775, 296, 3583, 3879, 419, 319, 688, 3850, done
  }

  uint16_t calData[8] = {3960, 290, 3960, 3870, 420, 300, 440, 3850};
  display.setTouchCalibrate(calData);

  display.fillScreen(TFT_BLACK);
}

uint32_t count = ~0;
void loop(void)
{
  display.startWrite();
  display.setRotation(++count & 7);
  //display.setColorDepth((count & 8) ? 16 : 24);
  //display.setColorDepth( 16 );

  display.setTextColor(TFT_WHITE);
  display.drawNumber(display.getRotation(), 16, 2);

  display.setTextColor(0xFF0000U);
  display.drawString("R", 30, 16);
  display.setTextColor(0x00FF00U);
  display.drawString("G", 40, 16);
  display.setTextColor(0x0000FFU);
  display.drawString("B", 50, 16);

  display.drawRect(30,30,display.width()-60,display.height()-60, 0x00FF00U);
  display.drawFastHLine(0, 0, 10);

  display.endWrite();

  int32_t x, y;
  if (display.getTouch(&x, &y)) {
    display.fillRect(x, y, 1, 1, 0xFFFFFFU);
  }
}