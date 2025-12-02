#pragma once
#include "User.h"
#include "config.h"

#define MAX_USERS 5

class UserDB {
private:
    User users[MAX_USERS];
    int userCount;

public:
    UserDB();
    void init(); 
    User* findUser(String scannedUid);
};