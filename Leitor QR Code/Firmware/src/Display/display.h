/*Classe Display
Implementa rotinas para display 0,96", 128*64 px, I2C*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <time.h>

//Pinos I2C
#define SDA 14
#define SCL 15

class Display
{   
    public:
        Display(void); //Construtor da classe
        //Funções públicas
        void displayInit(void);
        void statusBar(int wifiConn, bool qrCodeRead, char* dia, char* hora);
        void displayPrint(char* text, int line);
        void displayLogo(void);
        void displayProgressBar(unsigned int progress);
    private:
        /*Ícones*/
        // Wifi 100%
        

};

#endif //DISPLAY_H