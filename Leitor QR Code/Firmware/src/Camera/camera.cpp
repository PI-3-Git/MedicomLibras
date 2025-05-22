#include <Arduino.h>


#include "camera.h"

ESP32QRCodeReader qrReader(CAMERA_MODEL_AI_THINKER);

void Camera::onQrCodeTask(void *pvParameters){
    Camera* cameraInstance = static_cast<Camera*>(pvParameters);
    struct QRCodeData qrCodeData;
    if (qrReader.receiveQrCode(&qrCodeData, 100)){
        cameraInstance->displayInt.displayPrint((const char*)"QR Code identificado. Iniciando leitura.", 0);
        if (qrCodeData.valid){
            char msg[qrCodeData.payloadLen+11];
            strcpy(msg, "Conteúdo: ");
            strcat(msg, (const char *) qrCodeData.payload);
            cameraInstance->displayInt.displayPrint(msg, 0);
            delay(5000);
        }
    }

}

Camera::Camera(Display& DisplayRef) : displayInt(DisplayRef) {};

void Camera::initCamera(void){
    qrReader.setup();
    qrReader.beginOnCore(1);
    xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, this, 4, NULL);
}


