#pragma once
#ifndef USER_H
#define USER_H

#include <Arduino.h>

class user{
    String uid;
    String name;
    String role;
    bool active;
public:
    user(){}
    user(String uid, String name, String role);
    String getUid();
    String getName();
    void printdets();
    bool isAllowed(String currentTime);
};

#endif