/* Classe wifiManager
A classe implementa conectividade WiFi bem como webServer para configuração
*/

#ifndef WIFI_MANAGEMENT_H
#define WIFI_MANAGEMENT_H

#include <Arduino.h>
#include <LittleFS.h>
#include <espasyncbutton.hpp>

#define TRIGGER_PIN GPIO_NUM_0 //botão de configuração de WiFi

class wifiManager
{
    public:
        wifiManager(const char* hostname);
        void wifiInit(void);
        void wifiLoop(void);
        int getWifiSignal(void);

    private:
        String readFile(fs::FS &fs, const char * path);
        void writeFile(fs::FS &fs, const char * path, const char * message);
        void saveParamCallback();
        String getParam(String name);
        void buttonInit(void); //inicializa botão de configuração WiFi
        const char* _hostname;
        int data = 4;
        int timeout = 120; //limite de conexão em segundos
        bool wm_nonblocking = false; // modo de não bloqueio desabilitado
};

#endif // WIFI_MANAGEMENT_H