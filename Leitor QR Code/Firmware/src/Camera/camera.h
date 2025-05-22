/*Classe Camera
Implementa as rotinas de camera do ESP32-CAM*/

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <ESP32QRCodeReader.h>

#include "Display/display.h"

// class Display

class Camera
{
    public:
        Camera(Display & DisplayRef);
        void initCamera(void);


    private:
        Display& displayInt;
        static void onQrCodeTask(void *pvParameters);


};



#endif //CAMERA_H