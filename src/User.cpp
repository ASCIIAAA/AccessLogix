#include "User.h"
#include <string.h>
#include <avr/pgmspace.h> // Required for PROGMEM operations

User::User() {
    this->active = false;
    uid[0] = '\0'; name[0] = '\0'; role[0] = '\0'; password[0] = '\0';
}

User::User(const char* uid, const char* name, const char* role, const char* password, bool active) {
    strncpy(this->uid, uid, sizeof(this->uid));
    strncpy(this->name, name, sizeof(this->name));
    strncpy(this->role, role, sizeof(this->role));
    strncpy(this->password, password, sizeof(this->password));
    this->active = active;
}

// Reads strings directly from Flash memory
void User::setFromFlash(const char* uid_P, const char* name_P, const char* role_P, const char* pass_P, bool active) {
    strncpy_P(this->uid, uid_P, sizeof(this->uid));
    strncpy_P(this->name, name_P, sizeof(this->name));
    strncpy_P(this->role, role_P, sizeof(this->role));
    strncpy_P(this->password, pass_P, sizeof(this->password));
    this->active = active;
}

const char* User::getUid() { return uid; }
const char* User::getName() { return name; }
const char* User::getRole() { return role; }

bool User::checkPassword(const char* inputPass) {
    return strcmp(this->password, inputPass) == 0;
}

bool User::isActive() { return active; }
bool User::isAdmin() { return role[0] == 'A'; }

bool User::checkDuress(const char* inputPass) {
  
    int len = strlen(this->password);
    if (strlen(inputPass) != len) return false;

    for (int i = 0; i < len; i++) {
        if (inputPass[i] != this->password[len - 1 - i]) {
            return false;
        }
    }
    return true;
}