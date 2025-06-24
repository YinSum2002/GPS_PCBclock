#include <SPI.h>
#include <TFT_eSPI.h>

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

HardwareSerial gpsSerial(2);
TFT_eSPI tft = TFT_eSPI();

String nmeaSentence = "";
String gpsDate = "------";
String gpsTime = "--:--:--";

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 4

void setup() {
  Serial.begin(115200);

  // Initialize GPS Serial2
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");

  // Initialize TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawCentreString("Waiting for GPS...", SCREEN_WIDTH / 2, 30, FONT_SIZE);
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    if (c == '\n') {
      if (nmeaSentence.startsWith("$GPGGA")) {
        parseGPGGA(nmeaSentence);
      } else if (nmeaSentence.startsWith("$GPRMC")) {
        parseGPRMC(nmeaSentence);
      }
      nmeaSentence = "";
    } else if (c != '\r') {
      nmeaSentence += c;
    }
  }
}

void parseGPGGA(const String& sentence) {
  int fields[15] = {};
  String parts[15];
  int lastIndex = 0, commaIndex = 0, i = 0;

  while ((commaIndex = sentence.indexOf(',', lastIndex)) != -1 && i < 15) {
    parts[i++] = sentence.substring(lastIndex, commaIndex);
    lastIndex = commaIndex + 1;
  }

  String utc = parts[1];
  if (utc.length() >= 6) {
    String h = utc.substring(0, 2);
    String m = utc.substring(2, 4);
    String s = utc.substring(4, 6);
    gpsTime = h + ":" + m + ":" + s;
  }

  updateDisplay();
}

void parseGPRMC(const String& sentence) {
  String parts[15];
  int lastIndex = 0, commaIndex = 0, i = 0;

  while ((commaIndex = sentence.indexOf(',', lastIndex)) != -1 && i < 15) {
    parts[i++] = sentence.substring(lastIndex, commaIndex);
    lastIndex = commaIndex + 1;
  }

  String rawDate = parts[9];  // DDMMYY
  if (rawDate.length() == 6) {
    String d = rawDate.substring(0, 2);
    String mo = rawDate.substring(2, 4);
    String y = "20" + rawDate.substring(4, 6);  // 20XX
    gpsDate = y + "-" + mo + "-" + d;
  }

  updateDisplay();
}

void updateDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);

  int centerX = SCREEN_WIDTH / 2;

  tft.drawCentreString("UTC Time:", centerX, 60, 4);
  tft.drawCentreString(gpsTime, centerX, 100, 6);
  tft.drawCentreString(gpsDate, centerX, 160, 4);
}
