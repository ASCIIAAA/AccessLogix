#include "User.h"

user::user(){
    active=false;
}

user::user(String uid, String name, String role){
    uid=uid;
    name=name;
    role=role;
    active=true;
}
String user::getUid(){
    return uid;
}

String user::getName(){
    return name;
}

void user::printdets(){
    if(active){
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