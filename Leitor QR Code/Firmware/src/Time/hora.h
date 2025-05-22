/* Classe hora:
Implementa funções de relógio e calendário*/

#ifndef HORA_H
#define HORA_H

#include <arduino.h>

#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))
#define ONE_HOUR_IN_SECS 3600
#define NTP_SERVERS "time4.google.com", "time.google.com"
#define TIME_ZONE -3*ONE_HOUR_IN_SECS
#define HOUR_BUFFER_SIZE 6 // HH:MM\0
#define DATE_BUFFER_SIZE 6 // DD/MM\0
#define SALDACAO_BUFFER_SIZE 10 // Bom dia\0, Boa tarde\0, Boa noite\0

class Hora
{
    public:
        Hora(void);
        void initTimes(void);
        void getTime(void);
        char* getDate(void);
        char* getHour(void);
        char* getSaldacao(void);
        void updateTime(void);  

    private: 
        void setTimes(void);
        void sunUpdate(void);
        

        const long y2k = 946684800;
        uint32_t ntpTimeOutMs = 1000;
        char hourBuffer[HOUR_BUFFER_SIZE];
        char dateBuffer[DATE_BUFFER_SIZE];
        char saldacaoBuffer[SALDACAO_BUFFER_SIZE];
        
};

#endif // HORA_H