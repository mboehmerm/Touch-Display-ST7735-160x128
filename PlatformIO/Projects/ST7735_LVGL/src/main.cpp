// This the original LVGL_demo.ino 8.3.6 with all modifications i needed to compile without errors and 
// get the touch working correctly with Arduino IDE 2.0.4 , TFT_eSPI and ili9341.

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <TFT_eSPI.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 as the examples and demos are now part of the main LVGL library. */

#include <demos/lv_demos.h>                                 //!! lvgl/src/demos
#include <examples/lv_examples.h>                           //!! lvgl/src/examples

// PlatformIO doesn't find .c files in subdirectories ? Arduino IDE does. 
#include <examples/widgets/btn/lv_example_btn_1.c>
#include <examples/widgets/msgbox/lv_example_msgbox_1.c>
#include <examples/widgets/menu/lv_example_menu_1.c>
#include <examples/widgets/tabview/lv_example_tabview_1.c>
#include <examples/get_started/lv_example_get_started_1.c>
// ...

#define SCREEN_ROTATION 3                                   // set the screen rotation

/*Change to your screen resolution*/
#if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
  static const uint16_t screenWidth  = 160;                 // rotation 1 or 3
  static const uint16_t screenHeight = 128;
#else  
  static const uint16_t screenWidth  = 128;                 // rotation 0 or 2
  static const uint16_t screenHeight = 160;
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 4 ];    // screen buffer size
//static lv_color_t buf[ screenWidth * 10 ];                // smaller if compile error

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

#if LV_USE_LOG != 0
// ------------------------------------------------------------------------------------------ //
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif
// ------------------------------------------------------------------------------------------ //
// Display flushing
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )  //!!

{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}
// ------------------------------------------------------------------------------------------ //
// Read the touchpad
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )  //!!modified
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        #if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
          data->point.x = touchY;
          data->point.y = touchX;
        #else
          data->point.x = touchX;
          data->point.y = touchY;
        #endif

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}
// ------------------------------------------------------------------------------------------ //
void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();
    tft.setRotation( SCREEN_ROTATION );

    /*Set the touchscreen calibration data, the actual data for your display can be acquired using
        Generic -> Touch_calibrate example from the TFT_eSPI library */

    #if SCREEN_ROTATION == 0
      uint16_t calData[5] = { 230, 3580, 330, 3600, 4 };  //!!modified
    #elif SCREEN_ROTATION == 1
      uint16_t calData[5] = { 230, 3580, 330, 3600, 6 };
    #elif SCREEN_ROTATION == 2
      uint16_t calData[5] = { 230, 3580, 330, 3600, 2 };
    #elif SCREEN_ROTATION == 3
      uint16_t calData[5] = { 230, 3580, 330, 3600, 0 };
    #endif  
    
    tft.setTouch( calData );

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;  //!!modified
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;  //!!modified
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

#if 0
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
    // *** uncomment only **ONE** of these lines ( examples or demos ) ***
    
    // PlatformIO doesn't find the .c files in subdirectories. Arduino IDE does. 
    // see above : #include <examples/...
    
    // lv_example_btn_1();            
    // lv_example_msgbox_1();
    // lv_example_menu_1();
    // lv_example_tabview_1();
    // lv_example_get_started_1();
    // ...

    lv_demo_widgets();               // OK ( OK = enabled in lv_conf.h or platform.ini)
    // lv_demo_benchmark();          // OK
    // lv_demo_keypad_encoder();     // OK works, but I haven't an encoder
    // lv_demo_music();              // TOO BIG ? (disabled in platform.ini)
    // lv_demo_printer();            // MISSING
    // lv_demo_stress();             // OK
#endif
    Serial.println( "Setup done" );
}
// ------------------------------------------------------------------------------------------ //
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
// ------------------------------------------------------------------------------------------ //
