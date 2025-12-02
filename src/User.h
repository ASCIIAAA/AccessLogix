#pragma once
#include <Arduino.h>

class User {
private:
    char uid[11];      
    char name[13];     
    char role[2];     
    char password[8];  
    bool active;

public:
    User();
    User(const char* uid, const char* name, const char* role, const char* password, bool active);
    void setFromFlash(const char* uid_P, const char* name_P, const char* role_P, const char* pass_P, bool active);

    const char* getUid();
    const char* getName();
    const char* getRole();
    
    bool checkPassword(const char* inputPass);
    bool checkDuress(const char* inputPass); 

    bool isActive();
    bool isAdmin();
};