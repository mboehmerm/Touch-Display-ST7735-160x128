This modified program uses TFT, Touch, SD and LittleFS

Original : Arduino\libraries\TFT_eSPI\examples\Generic\ESP32_SDcard_jpeg\ESP32_SDcard_jpeg.ino

Usage :

- install library JPEGDecoder
- Copy the 4 files in the data folder to the root directory of an Fat32 SD-card
- first usage : uncomment the line "#define SD_TO_LITLEFS". 01.jpg is now copied from SD-Card to the LittleFS file system
- second usage : comment the line "#define SD_TO_LITLEFS". Three files are now loaded from SD-Card and 01.jpg from LittleFS
- Upload with partition scheme: "Default 4MB with spiffs..." ( ESP32 DevKitC v4 )

Run :

The 4 pictures are shown on the TFT. Press touch to repeat this. The serial montior shows data of the jpg-files and a listing of the spiffs file system.

