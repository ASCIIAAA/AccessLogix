#pragma once

// --- Hardware Pins ---
#define PIN_BUZZER    3
#define PIN_LED_GREEN 3
#define PIN_LED_RED   5

// --- PIN SWAP FIX ---
// SD Card CS *MUST* be on Pin 10 for Arduino Uno SPI to stay stable
#define PIN_SD_CS     10 

// EM-18 RFID (Moved to Pin 4 to avoid conflict)
#define PIN_RFID_RX   4
#define PIN_RFID_TX   2 // Dummy pin (shared with LED, we don't use TX anyway)

// Keypad
// COLS: 9, 8, 7, 6
// ROWS: A0, A1, A2, A3

// --- Settings ---
#define LOG_FILENAME "access.csv"
#define ADMIN_ROLE "Admin"
#define PROF_ROLE "Professor"
#define STUDENT_ROLE "Student"