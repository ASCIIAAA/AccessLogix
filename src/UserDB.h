#pragma once
#include "User.h"
#include "config.h"

// No longer storing an array in RAM!
class UserDB {
private:
    User tempUser; // We only hold ONE user in RAM at a time

public:
    UserDB();
    void init(); 
    User* findUser(const char* scannedUid);
};