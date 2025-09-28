#include "SystemController.h"

SystemController::SystemController() {

}

void SystemController::setup() {
    
    logger.setup();

    
    logger.logInfo("SystemController setup complete.");
}

void SystemController::loop() {
    // String scannedUID = rfidReader.scanForCard();
    // if (scannedUID != "") {
    //   // Handle card scan...
    // }
}