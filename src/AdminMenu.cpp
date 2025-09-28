#include "AdminMenu.h"

adminMenu::adminMenu() {
    usercount = 0;
}

void adminMenu::checkSerial() {
    if (Serial.available() >0) {
        String command = Serial.readStringUntil('\n');
        command.trim(); 

        if(command.startsWith("ADDUSER")) {
            addUser(command);
        } 
        else if (command.equalsIgnoreCase("LISTUSERS")) {
            listUsers();
        } 
        else {
            Serial.println("Unknown command.");
        }
    }
}

void adminMenu::addUser(String command) {
    int firstComma=command.indexOf(',');
    int secondComma=command.indexOf(',', firstComma + 1);
    int thirdComma=command.indexOf(',', secondComma + 1);

    if (firstComma > 0 && secondComma > 0 && thirdComma > 0) {
        String uid=command.substring(firstComma + 1, secondComma);
        String name=command.substring(secondComma + 1, thirdComma);
        String role=command.substring(thirdComma + 1);

        if (usercount <MAX_USERS) {
            users[usercount]=user(uid, name, role);
            usercount++;
            Serial.print("SUCCESS: Added user ");
            Serial.println(name);
        } 
        else {
            Serial.println("ERROR: User database is full.");
        }
    } else {
        Serial.println("ERROR: Invalid ADDUSER format. Use: ADDUSER,UID,FullName,Role");
    }
}

void adminMenu::listUsers() {
    Serial.println("--- Listing All Users ---");
    if (usercount==0) {
        Serial.println("No users in the database.");
    }
    for (int i=0;i<usercount;i++) {
        users[i].printdets();
    }
}