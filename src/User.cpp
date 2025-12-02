#include "User.h"
#include <string.h>

User::User() {
    this->active = false;
    uid[0] = '\0';
    name[0] = '\0';
    role[0] = '\0';
    password[0] = '\0';
}

User::User(const char* uid, const char* name, const char* role, const char* password, bool active) {
    // Copy data safely into fixed buffers
    strncpy(this->uid, uid, sizeof(this->uid));
    strncpy(this->name, name, sizeof(this->name));
    strncpy(this->role, role, sizeof(this->role));
    strncpy(this->password, password, sizeof(this->password));
    this->active = active;
}

const char* User::getUid() { return uid; }
const char* User::getName() { return name; }
const char* User::getRole() { return role; }

// FIX: Changed 'String' to 'const char*' to match the header
bool User::checkPassword(const char* inputPass) {
    // Compare two C-strings (returns 0 if they are equal)
    return strcmp(this->password, inputPass) == 0;
}

bool User::isActive() { return active; }

bool User::isAdmin() {
    // Check if the first letter of the role is 'A' (for Admin)
    return role[0] == 'A'; 
}