// src/User.cpp

#include "User.h"
#include <Arduino.h> // Necessary for String and Serial functions

// Default Constructor Definition
// Initializes the user as inactive
user::user() {
    active = false;
}

// Parameterized Constructor Definition (using Member Initializer List)
// This correctly assigns parameters to class members (this->uid = uid;)
user::user(String uid, String name, String role) 
    : uid(uid), name(name), role(role) 
{
    active = true;
}

// Getter for UID
String user::getUid() {
    return uid;
}

// Getter for Name
String user::getName() {
    return name;
} 

// Utility function to print user details
void user::printdets() {
    if (active) {
        Serial.println("-------------------");
        Serial.print("User ID:  ");
        Serial.println(uid);
        Serial.print("Name:     ");
        Serial.println(name);
        Serial.print("Role:     ");
        Serial.println(role);
        Serial.println("-------------------");
    }
}
