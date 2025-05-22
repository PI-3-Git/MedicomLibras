/* Classe otaHandle: 
    Implementa atualização de firmware por WiFi (OTA - Over the Air)*/

#ifndef OTA_H
#define OTA_H

#include <ArduinoOTA.h>
#include "MQTT\mqttSys.h"
#include "Display\display.h"

class Display;  

class otaHandle {
public:
    static void otaInit(Display& display); // Declaração de interface

private:
   
};

#endif // OTA_H