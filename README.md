# 🕒 GPS-Powered Digital Clock

A minimalist digital clock driven by GPS time data, designed to run on a custom PCB with an ESP32 dev board.

## 📋 Overview

This project began as a personal exploration into precision timekeeping using GPS satellites, eventually evolving into a custom hardware implementation. The goal is to keep things minimal both in hardware and firmware while delivering a reliable, standalone clock synced to real satellite time.

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

The code is complete and functional, and the PCB is currently being fabricated.

## 📫 Questions?

Feel free to reach out if you have questions or ideas! You can contact me via [LinkedIn](https://www.linkedin.com/in/justin-chan-1a4160181/) or at chanj1421@gmail.com.
