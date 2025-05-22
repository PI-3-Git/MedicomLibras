#include <Arduino.h>
#include <stdio.h>
#include <time.h>

#include "hora.h"

Hora::Hora(void){
    hourBuffer[0] = '\0'; 
};

void Hora::initTimes(void){
    updateTime();
}

void Hora::updateTime(void){
    Serial.print("Ajuste de relógio por SNTP");
    configTime(TIME_ZONE, 0, NTP_SERVERS);
    time_t now = time(NULL);
    uint32_t timeOut=0;
    int tries = 0;
    while (tries < 10){
        while (now < 1510592825){
            Serial.print(".");
            now = time(NULL);
            timeOut++;
            if (timeOut > ntpTimeOutMs/500){
                Serial.print("Relógio não ajustado. Servidor não alcançado. Tentativa n.");
                Serial.println(tries);
                break;
            }
        }
        delay(1000);
        tries++;
    }
    Serial.println("Relógio ajustado!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.printf("%s", asctime(&timeinfo));
}

char* Hora::getHour(void){
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    sprintf(hourBuffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    return hourBuffer;
}

char* Hora::getDate(void){
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    sprintf(dateBuffer, "%02d/%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1);
    return dateBuffer;
}

char* Hora::getSaldacao(void){
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    int horaAtual = timeinfo.tm_hour;
    if (horaAtual >= 1 && horaAtual <= 12) {
        strcpy(saldacaoBuffer, "Bom dia");
    } else if (horaAtual >= 13 && horaAtual <= 17) {
        strcpy(saldacaoBuffer, "Boa tarde");
    } else {
        strcpy(saldacaoBuffer, "Boa noite");
    }
    return saldacaoBuffer;
}