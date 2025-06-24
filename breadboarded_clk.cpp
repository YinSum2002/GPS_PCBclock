#include <SPI.h>
#include <TFT_eSPI.h>

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

HardwareSerial gpsSerial(2);
TFT_eSPI tft = TFT_eSPI();

String nmeaSentence = "";
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
      parseGPGGA(nmeaSentence);
      nmeaSentence = "";
    } else if (c != '\r') {
      nmeaSentence += c;
    }
  }
}

void parseGPGGA(const String& sentence) {
  if (sentence.startsWith("$GPGGA")) {
    int commaIndex = 0;
    int lastIndex = 0;
    int fieldIndex = 0;
    String fields[15];

    while ((commaIndex = sentence.indexOf(',', lastIndex)) != -1 && fieldIndex < 15) {
      fields[fieldIndex++] = sentence.substring(lastIndex, commaIndex);
      lastIndex = commaIndex + 1;
    }

    String utc = fields[1];
    if (utc.length() >= 6) {
      String hour = utc.substring(0, 2);
      String minute = utc.substring(2, 4);
      String second = utc.substring(4, 6);

      String utcTime = hour + ":" + minute + ":" + second;
      Serial.println(utcTime);

      // Display on TFT
      tft.fillScreen(TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
      //tft.drawCentreString("GPS Clock", SCREEN_WIDTH / 2, 30, FONT_SIZE);
      tft.drawCentreString(utcTime, SCREEN_WIDTH / 2, 80, FONT_SIZE);
    }
  }
}
