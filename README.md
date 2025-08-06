# 🕒 GPS-Powered Digital Clock - Sponsored by PCBWay!

A minimalist digital clock driven by GPS time data, designed to run on a custom PCB with an ESP32 dev board.

## 📋 Overview

This project began as a personal exploration into precision timekeeping using GPS satellites, eventually evolving into a custom hardware implementation. The goal is to keep things minimal both in hardware and firmware while delivering a reliable, standalone clock synced to real satellite time.

## 🤝 Sponsor

A huge thanks to PCBWay for their generosity and proactiveness in suggesting their sponsorship for this project! The boards arrived at my doorstep in perfect condition, and I had the best possible customer experience with their representative, Liam!
The quality of the boards were perfect, as there was never a single issue with the intended functionality. The service of PCBWay was even better, as the status of my order was clearly communicated, and I was even allowed a redo on my order when I discovered a design mistake!
I cannot express my graditude enough to the people at PCBWay who helped make this sponsorship possible. I hope to work with PCBWay more in the future, and I highly recommend any other PCB hobbyists to do the same!

## 🔌 Hardware Setup

- **ESP32 Dev Board** – Sole microcontroller.
- **NEO-6M GPS Module** – Provides UTC time via serial connection.
- Designed for integration on a custom **PCB**.

## 🧠 Code Structure

There are two files in this repo:

- `gps_pi_prototype.cpp` – Legacy code for an early Raspberry Pi prototype (not used anymore).
- `gps_clock_esp32.cpp` – The current and active firmware that will be deployed to the ESP32.

The firmware is intentionally compact and focused, handling only essential GPS parsing and display logic.

## 🚧 Project Status

The code is complete and functional, and the PCBs have just been delivered by my sponsor, PCBWay! I am now in the soldering stage of this project.

## 📫 Questions?

Feel free to reach out if you have questions or ideas! You can contact me via [LinkedIn](https://www.linkedin.com/in/justin-chan-1a4160181/) or at chanj1421@gmail.com.

## 📸 Images
![IMG_8969](https://github.com/user-attachments/assets/bfacf8ce-9568-4afc-a674-69a1bd1d8be2)
![IMG_8970](https://github.com/user-attachments/assets/ea3885c2-58fe-490e-9c2b-c4953aabe929)
![IMG_8972](https://github.com/user-attachments/assets/e8466150-ac7e-43ad-bebd-3ed9f74f7c42)
![IMG_8974](https://github.com/user-attachments/assets/fe9b844b-bfca-49d2-a0f0-4b3ed1ed093d)
