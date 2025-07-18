#include <sys/time.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void printSystemTime();

void printSystemTime() {
  timeval tv;
  gettimeofday(&tv, NULL);
  time_t now = tv.tv_sec;
  struct tm* ptm = gmtime(&now);
  std::cout << "System Time: " << std::put_time(ptm, "%H:%M:%S") << "."
            << std::setw(6) << std::setfill('0') << tv.tv_usec << std::endl;
}

int main() {
  std::ifstream gps("/dev/serial0");
  if (!gps.is_open()) {
    std::cerr << "Failed to open /dev/serial0" << std::endl;
    return 1;
  }

  std::string line;
  int lastUTC = -1;  // Stores the last valid UTC time as seconds since midnight

  while (true) {
    if (!std::getline(gps, line)) {
      usleep(100000);  // wait 100ms if nothing was read
      continue;
    }

    // Filter only GNRMC lines
    if (line.find("$GNRMC") == 0) {
      std::stringstream ss(line);
      std::string token;
      std::string fields[12];  // GNRMC has up to 12 fields
      int i = 0;

      while (std::getline(ss, token, ',') && i < 12) {
        fields[i++] = token;
      }

      if (fields[1].length() >= 6 &&
          fields[2] == "A") {  // Field 1 = time, Field 2 = data valid
        std::string utc_time = fields[1];
        int hour = std::stoi(utc_time.substr(0, 2));
        std::string hours = utc_time.substr(0, 2);
        int minute = std::stoi(utc_time.substr(2, 2));
        std::string minutes = utc_time.substr(2, 2);
        int second = std::stoi(utc_time.substr(4, 2));
        std::string seconds = utc_time.substr(4, 2);
        int currentUTC = hour * 3600 + minute * 60 + second;

        if (currentUTC > lastUTC) {
          std::cout << "UTC Time: " << std::setfill('0') << std::setw(2) << hour
                    << ":" << std::setw(2) << minute << ":" << std::setw(2)
                    << second << std::endl;

          printSystemTime();
          lastUTC = currentUTC;
        } else {
          std::cout << "Waiting for valid fix..." << std::endl;
        }
      }

      usleep(5000);  // Delay to prevent spamming (5ms)
    }

    gps.close();
    return 0;
  }
}
