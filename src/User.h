// src/User.h

#pragma once
#ifndef USER_H
#define USER_H

#include <Arduino.h>

class user{
private:
    String uid;
    String name;
    String role;
    bool active;

public:
    // FIX: DECLARATION ONLY. Remove the '{}' to avoid redefinition error.
    user(); 
    
    // DECLARATION for parameterized constructor
    user(String uid, String name, String role);
    
    // Method Declarations
    String getUid();
    String getName();
    void printdets();
    bool isAllowed(String currentTime);
};
 
#endif // USER_H