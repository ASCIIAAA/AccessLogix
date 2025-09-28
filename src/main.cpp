#include <Arduino.h>
#include "SystemController.h"

SystemController systemController;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("AccessLogix System Initializing...");
  systemController.setup();
  Serial.println("System Ready");
}

void loop() {
  systemController.loop();
}

