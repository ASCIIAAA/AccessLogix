#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "Logger.h"
// #include "RFIDReader.h" 
// #include "UserDB.h"
// #include "AdminMenu.h"

class SystemController {
public:
    
    SystemController();

    void setup();

    void loop();

private:
    
    Logger logger;
    // RFIDReader rfidReader;
    // UserDB userDatabase;
    // AdminMenu adminMenu;
};

#endif 