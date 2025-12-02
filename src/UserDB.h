#pragma once
#include "User.h"
#include "config.h"

class UserDB {
private:
    User tempUser;

public:
    UserDB();
    void init(); 
    User* findUser(const char* scannedUid);
};