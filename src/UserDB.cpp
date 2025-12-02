#include "UserDB.h"
#include <avr/pgmspace.h>

UserDB::UserDB() {
    userCount = 0;
}

void UserDB::init() {
    // We use setFromFlash with PSTR() macro.
    // PSTR("...") stores the string in Flash memory and returns a pointer to it.
    
    // 1. Ex-Student
    users[0].setFromFlash(PSTR("570064A276"), PSTR("Ex-Student"), PSTR("S"), PSTR(""), false);

    // 2. Admin 
    users[1].setFromFlash(PSTR("570066B113"), PSTR("Ananya"), PSTR("A"), PSTR("A57845"), true);

    // 3. Professor 
    users[2].setFromFlash(PSTR("570066AF3E"), PSTR("Prof. Sonali"), PSTR("P"), PSTR("B6501"), true);

    // 4. Student 
    users[3].setFromFlash(PSTR("570067BA4D"), PSTR("Aditi"), PSTR("S"), PSTR("D9877"), true);

    // 5. Student 
    users[4].setFromFlash(PSTR("5700657DA1"), PSTR("Anushka"), PSTR("S"), PSTR("D8709"), true);
    
    userCount = 5;
}

User* UserDB::findUser(const char* scannedUid) {
    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(scannedUid, users[i].getUid()) == 0) {
            return &users[i];
        }
    }
    return nullptr; 
}