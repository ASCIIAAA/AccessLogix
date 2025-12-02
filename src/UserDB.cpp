#include "UserDB.h"

UserDB::UserDB() {
    userCount = 0;
}

void UserDB::init() {
    // ROLES: "A"=Admin, "P"=Prof, "S"=Student
    // NAMES: Must be max 12 letters!
    
    // 1. Ex-Student
    users[0] = User("570064A276", "Ex-Student", "S", "", false); 

    // 2. Admin 
    users[1] = User("570066B113", "Ananya", "A", "A57845", true);

    // 3. Professor 
    users[2] = User("570066AF3E", "Prof. Sonali", "P", "B6501", true);

    // 4. Student 
    users[3] = User("570067BA4D", "Aditi", "S", "D9877", true);

    // 5. Student 
    users[4] = User("5700657DA1", "Anushka", "S", "D8709", true);
    
    userCount = 5;
}

User* UserDB::findUser(String scannedUid) {
    for(int i = 0; i < userCount; i++) {
        if(scannedUid.equalsIgnoreCase(users[i].getUid())) {
            return &users[i];
        }
    }
    return nullptr; 
}