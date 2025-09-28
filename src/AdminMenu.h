#pragma once
#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H

#include "User.h"
#define MAX_USERS 10

class adminMenu{
    user users[MAX_USERS];
    int usercount;
public:
    adminMenu();
    void checkSerial();
    void addUser(String command);
    void listUsers();

};
#endif