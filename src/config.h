#pragma once

// --- Hardware Pins ---
#define PIN_BUZZER    3
#define PIN_LED_GREEN 2
#define PIN_LED_RED   5

// SD Card
#define PIN_SD_CS     4

// EM-18 RFID (SoftwareSerial)
#define PIN_RFID_RX   10
#define PIN_RFID_TX   11 // Not connected, but needed for constructor

// Keypad
// COLS: 9, 8, 7, 6
// ROWS: A0, A1, A2, A3

// --- Settings ---
#define LOG_FILENAME "access.csv"
#define ADMIN_ROLE "Admin"
#define PROF_ROLE "Professor"
#define STUDENT_ROLE "Student"