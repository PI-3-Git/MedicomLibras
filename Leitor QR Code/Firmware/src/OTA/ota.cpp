#include <Arduino.h>
#include <atomic>
#include "OTA.h"

int progressAtual = 101;

void otaHandle::otaInit(Display& display) { 
    //Início de atualização
    ArduinoOTA.onStart([&display](){
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "firmware";
        else                            // U_SPIFFS
            type = "sistema de arquivos";
        Serial.println("Inciando atualização de " + type);       
        char topic[10];
        char msg[100];
        snprintf(topic, sizeof(topic), "OTA");
        snprintf (msg, sizeof(msg), "Iniciando atuallização de %s.", type.c_str());
        display.displayPrint(msg, 0);
        delay(1000);
    });
    
    //Progresso de atualização
    ArduinoOTA.onProgress([&display](unsigned int progress, unsigned int total){
        Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
        char msg[100];
        snprintf (msg, sizeof(msg), "Progresso da atualização: %u%%\r", (progress / (total / 100)));
        int progr = (progress / (total / 100));
        if (progressAtual != progr){
            display.displayPrint(msg, 0);
            display.displayProgressBar(progr);
            progressAtual = progr;
        }
    });
    
    //Finalização de atualização
    ArduinoOTA.onEnd([&display](){
        Serial.println("\nFim");
        char topic[10];
        char msg[100];
        snprintf(topic, sizeof(topic), "OTA");
        snprintf(msg, sizeof(msg), "Fim da atualização. O sistema irá reiniciar.");
        display.displayPrint(msg, 0);
        delay(5000);
    });
    
    //Tratamento de erro
    ArduinoOTA.onError([&display](ota_error_t error) {
        Serial.printf("Erro[%u]: ", error);
        String errorStr;
        if (error == OTA_AUTH_ERROR) errorStr="Falha de autorização";
        else if (error == OTA_BEGIN_ERROR) errorStr="Falha de inicialização";
        else if (error == OTA_CONNECT_ERROR) errorStr="Falha de conexão";
        else if (error == OTA_RECEIVE_ERROR) errorStr="Falha de recepção";
        else if (error == OTA_END_ERROR) errorStr="Falha de finalização";
        char topic[10];
        char msg[100];
        snprintf(topic, sizeof(topic), "OTA");
        snprintf(msg, sizeof(msg), "Erro de atualização[%u] - %s", error, errorStr.c_str());
        display.displayPrint(msg, 0);
    });
    
    //reinicia OTA
    ArduinoOTA.begin();
}