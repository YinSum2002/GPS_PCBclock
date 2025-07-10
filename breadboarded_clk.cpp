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

// Timezone arrays
float timezoneOffsets[] = {
  -12.0, -11.0, -10.0, -9.5, -9.0, -8.0, -7.0, -6.0, -5.0,
  -4.0, -3.5, -3.0, -2.0, -1.0, 0.0,
  1.0, 2.0, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 5.75,
  6.0, 6.5, 7.0, 8.0, 8.75, 9.0, 9.5, 10.0, 10.5,
  11.0, 12.0, 12.75, 13.0, 14.0
};

String timezoneNames[] = {
  "UTC-12", "UTC-11", "UTC-10", "UTC-9:30", "UTC-9", "UTC-8", "UTC-7", "UTC-6", "UTC-5",
  "UTC-4", "UTC-3:30", "UTC-3", "UTC-2", "UTC-1", "UTC+0",
  "UTC+1", "UTC+2", "UTC+3", "UTC+3:30", "UTC+4", "UTC+4:30", "UTC+5", "UTC+5:30", "UTC+5:45",
  "UTC+6", "UTC+6:30", "UTC+7", "UTC+8", "UTC+8:45", "UTC+9", "UTC+9:30", "UTC+10", "UTC+10:30",
  "UTC+11", "UTC+12", "UTC+12:45", "UTC+13", "UTC+14"
};

const int numTimezones = sizeof(timezoneOffsets) / sizeof(timezoneOffsets[0]);
int timezoneIndex = 14;  // Start at UTC+0

String timezoneLabel = timezoneNames[timezoneIndex];

// Pushbutton
bool toggleState = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 4
#define BUTTON_PIN 4

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

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // assumes active LOW button
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.print("H");
    timezoneIndex = (timezoneIndex + 1) % numTimezones;
    timezoneLabel = timezoneNames[timezoneIndex];
    Serial.print("Timezone changed to: ");
    Serial.println(timezoneLabel);
  }

  lastButtonState = buttonState;

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
    int hour = utc.substring(0, 2).toInt();
    int minute = utc.substring(2, 4).toInt();
    int second = utc.substring(4, 6).toInt();

    // Apply timezone offset
    float offset = timezoneOffsets[timezoneIndex];
    int offsetHours = int(offset);
    int offsetMinutes = int((abs(offset) - abs(offsetHours)) * 60.0 + 0.5);

    if (offset < 0) offsetMinutes = -offsetMinutes;

    minute += offsetMinutes;
    hour += offsetHours + (minute / 60);
    minute = (minute + 60) % 60;
    hour = (hour + 24) % 24;

    gpsTime = (hour < 10 ? "0" : "") + String(hour) + ":" +
              (minute < 10 ? "0" : "") + String(minute) + ":" +
              (second < 10 ? "0" : "") + String(second);
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
  tft.drawCentreString(timezoneLabel, centerX, 200, 2);
}
