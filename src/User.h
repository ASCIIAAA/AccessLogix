#pragma once
#include <Arduino.h>

class User {
private:
    char uid[11];      // 10 chars + null terminator
    char name[13];     // Max 16 chars + null
    char role[2];     // Max 11 chars + null
    char password[8];  // Max 7 chars + null
    bool active;

public:
    User();
    // Constructor uses const char* instead of String
    User(const char* uid, const char* name, const char* role, const char* password, bool active);

    const char* getUid();
    const char* getName();
    const char* getRole();
    bool checkPassword(const char* inputPass); // Input can still be String for convenience
    bool isActive();
    bool isAdmin();
};