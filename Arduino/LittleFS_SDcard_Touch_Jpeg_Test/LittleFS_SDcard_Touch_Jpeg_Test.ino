// This sketch if for an ESP32, it draws Jpeg images pulled from an SD Card onto the TFT.

// As well as the TFT_eSPI library you will need the JPEG Decoder library.
// A copy can be downloaded here, it is based on the library by Makoto Kurauchi.
// https://github.com/Bodmer/JPEGDecoder

// Images on SD Card must be put in the root folder (top level) to be found
// Use the SD library examples to verify your SD Card interface works!

// The example images used to test this sketch can be found in the library
// JPEGDecoder/extras folder 
//----------------------------------------------------------------------------------------------------

// This modified program uses TFT, Touch, SD and LittleFS
// Original: Arduino\libraries\TFT_eSPI\examples\Generic\ESP32_SDcard_jpeg\ESP32_SDcard_jpeg.ino

// Four jpg-files are loaded from SD (and spiffs) ande shown on the TFT. Press touch to repeat.
// Serial montior shows data of the jpg-files and listing of the files on spiffs

// Usage :
// - install library JPEGDecoder
// - format a SD-Card with FAT32 and copy the 4 files in the data folder
// - uncomment the line #define SD_TO_LITLEFS, to copy 01.jpg from SD-Card to LittleFS (first usage)
// - second usage: comment the line #define SD_TO_LITLEFS, 01.jpg is now loaded from LittleFS
// - Upload with Partition scheme: "Default 4MB with spiffs..." ( ESP32 DevKitC v4 )

//----------------------------------------------------------------------------------------------------

#include <SPI.h>

#include <FS.h>
#include <SD.h>

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// JPEG decoder library
#include <JPEGDecoder.h>

#include <LittleFS.h>
#define FORMAT_LITTLEFS_IF_FAILED true

//####################################################################################################
// Setup
//####################################################################################################
void setup() {
  Serial.begin(115200);

  // Set all chip selects high to avoid bus contention during initialisation of each peripheral
  //digitalWrite(TOUCH_CS, HIGH); // 4  Touch controller chip select (if used)
  //digitalWrite(TFT_CS  , HIGH); // 15 TFT screen chip select
  //digitalWrite(SS      , HIGH); // 5  SD card chips select, must use GPIO 5 (ESP32 SS)

  digitalWrite( 4, HIGH); // Touch controller chip select (if used)
  digitalWrite(15, HIGH); // TFT screen chip select
  digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)

  tft.begin();

//  if (!SD.begin(SS)) {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  Serial.println("initialisation SD done.");

  // Dateisystem einbinden (ggf. beim ersten Mal formatieren)
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LittleFS Mount failed");
    return;
  }

  //#define SD_TO_LITLEFS
  #ifdef SD_TO_LITLEFS
    Serial.println("\n----- copy SD to LittleFS --------");

    File sourceFile = SD.open("/01.jpg");
    File destFile = LittleFS.open("/01.jpg", FILE_WRITE);

    static uint8_t buf[512];
    while( sourceFile.read( buf, 512) ) 
      destFile.write( buf, 512 );

    destFile.close();
    sourceFile.close();

    Serial.println("\n----- copy done ------------");
  #endif
    
  Serial.println("\nInformation spiffs file system:");
  Serial.printf("- Bytes total:   %ld\n", LittleFS.totalBytes());
  Serial.printf("- Bytes genutzt: %ld\n\n", LittleFS.usedBytes());

  listDir(LittleFS, "/", 1);

  Serial.println("\n----- listdir done ---------");

}

//####################################################################################################
// Main loop
//####################################################################################################
void loop() {
  uint16_t x = 0, y = 0; // To store the touch coordinates  
  bool pressed;

  tft.setRotation(0);  // portrait
  tft.fillScreen(random(0xFFFF));
  drawSdJpeg("/Baboon40.jpg", 0, 0);     // This draws a jpeg pulled off the SD Card
  delay(2000);

  tft.setRotation(0);  // portrait
  tft.fillScreen(random(0xFFFF));
  drawSdJpeg("/lena20k.jpg", 0, 0);     // This draws a jpeg pulled off the SD Card
  delay(2000);

  tft.setRotation(0);  // portrait
  tft.fillScreen(random(0xFFFF));
  drawLittleFsJpeg("/01.jpg", 0, 0);     // This draws a jpeg pulled off the LittleFS

  delay(1000);
  
  tft.setCursor(40, 145);
  tft.setTextColor(TFT_BLUE);
  tft.setTextSize(1);
  tft.println(F("Touch me!"));

  //while(1); // Wait here
  do {
    pressed = tft.getTouch(&x, &y);
  } while (!pressed);  
}
//####################################################################################################
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Directory-Listing: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {    // Verzeichnis nicht vorhanden
    Serial.println("- Fehler beim Öffnen des Verzeichnisses");
    return;
  }
  if (!root.isDirectory()) {  // kein Verzeichnis, sondern Datei
    Serial.println(" - kein Verzeichnis");
    return;
  }

  File file = root.openNextFile();
  // in einer Schleife durch die Liste aller vorhandenen
  // Einträge gehen
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      // ist der Eintrag ein Verz., dann dessen Inhalt rekursiv
      // anzeigen, wenn maximale Tiefe noch nicht erreicht ist
      if (levels) {
        listDir(fs, file.name(), levels-1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tGröße: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
//####################################################################################################
// Draw a JPEG on the TFT pulled from SD Card
//####################################################################################################
// xpos, ypos is top left corner of plotted image
void drawSdJpeg(const char *filename, int xpos, int ypos) {

  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Use one of the following methods to initialise the decoder:
  bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
  //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}

//####################################################################################################
// Draw a JPEG on the TFT pulled from SD Card
//####################################################################################################
// xpos, ypos is top left corner of plotted image
void drawLittleFsJpeg(const char *filename, int xpos, int ypos) {

  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = LittleFS.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Use one of the following methods to initialise the decoder:
  bool decoded = JpegDec.decodeFsFile(jpegFile);  // Pass the LittleFS file handle to the decoder,
  //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)

  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}

//####################################################################################################
// Draw a JPEG on the TFT, images will be cropped on the right/bottom sides if they do not fit
//####################################################################################################
// This function assumes xpos,ypos is a valid screen coordinate. For convenience images that do not
// fit totally on the screen are cropped to the nearest MCU size and may leave right/bottom borders.
void jpegRender(int xpos, int ypos) {

  //jpegInfo(); // Print information from the JPEG file (could comment this line out)

  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);
  
  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // Fetch data from the file, decode and display
  while (JpegDec.read()) {    // While there is more data in the file
    pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  tft.setSwapBytes(swapBytes);

  showTime(millis() - drawTime); // These lines are for sketch testing only
}

//####################################################################################################
// Print image information to the serial port (optional)
//####################################################################################################
// JpegDec.decodeFile(...) or JpegDec.decodeArray(...) must be called before this info is available!
void jpegInfo() {

  // Print information extracted from the JPEG file
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print("Width      :");
  Serial.println(JpegDec.width);
  Serial.print("Height     :");
  Serial.println(JpegDec.height);
  Serial.print("Components :");
  Serial.println(JpegDec.comps);
  Serial.print("MCU / row  :");
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print("MCU / col  :");
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print("Scan type  :");
  Serial.println(JpegDec.scanType);
  Serial.print("MCU width  :");
  Serial.println(JpegDec.MCUWidth);
  Serial.print("MCU height :");
  Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}

//####################################################################################################
// Show the execution time (optional)
//####################################################################################################
// WARNING: for UNO/AVR legacy reasons printing text to the screen with the Mega might not work for
// sketch sizes greater than ~70KBytes because 16 bit address pointers are used in some libraries.

// The Due will work fine with the HX8357_Due library.

void showTime(uint32_t msTime) {
  //tft.setCursor(0, 0);
  //tft.setTextFont(1);
  //tft.setTextSize(2);
  //tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.print(F(" JPEG drawn in "));
  //tft.print(msTime);
  //tft.println(F(" ms "));
  Serial.print(F(" JPEG drawn in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}

