/* Bibliotecas ESP */
#include <Arduino.h>
#include <AsyncDelay.h>

/* Bibliotecas C99 */
#include <cstdlib>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <atomic>
#include <time.h>

/* Classes proprietárias */
#include "iot_configs.h"
#include "wifiManagement/wifiManagement.h"
#include "OTA/ota.h"
#include "MQTT/mqttSys.h"
#include "Time/hora.h"
#include "Display/display.h"
#include "Camera/camera.h"

/*Construtores das classes*/
  wifiManager wM(HOSTNAME);
  otaHandle OTA_handle;
  mqtt Mqtt;
  Hora hora;
  Display meuDisplay;
  Camera camera(meuDisplay);

/*LEDs*/
  #define WiFiLed 12          //Led indicador de estado Wifi
  #define LendoLed 13         //Led indicador de leitura em andamento
  #define LeituraOkLed 2      //Led indicador de leitura correta
  #define LeituraErroLed 10   //Led indicador de erro de leitura
  int leds[]={WiFiLed, LendoLed, LeituraOkLed, LeituraErroLed};
  int numLeds = sizeof(leds) / sizeof(leds[0]); //tamanho do array leds

  #define INT_LED_PIN 33 //LED interno da placa, somente para monitoramento
  #define FLASH_LIGHT_PIN 4 //Flash para câmera

  bool INT_LED;
  bool FLASH_LIGHT;
  bool displayLogoFlag;

/*Temporizadores e delays*/
  AsyncDelay wifiLedBlinkDelay; //intervalo para acendimmento do led indicador de estado WiFi
  AsyncDelay relogioDelay; //intervalo de refresh do relogio
  AsyncDelay displayLogoInterval; //iintervalo para exibição do logo

/* Inicialização da máquina */
void setup() {
  Serial.begin(115200);

  pinMode(INT_LED_PIN, OUTPUT);
  pinMode(FLASH_LIGHT_PIN, OUTPUT);
  
  digitalWrite(INT_LED_PIN, LOW);
  digitalWrite(FLASH_LIGHT_PIN, HIGH);  //acende o flash para indicação de setup em andamento
  
  INT_LED = true;
  FLASH_LIGHT=false;
  displayLogoFlag=false;
  //inicialização de delays
  relogioDelay.start(1000, AsyncDelay::MILLIS);
  wifiLedBlinkDelay.start(1000, AsyncDelay::MILLIS); 
  displayLogoInterval.start(3000, AsyncDelay::MILLIS); 
  
  /*Inicializações*/
    meuDisplay.displayInit(); //inicalização de display
    wM.wifiInit();          //inicialização de wifi
    hora.initTimes();       //inicialização de relógio
    Mqtt.mqttInit();        //inicialização mqtt
    otaHandle::otaInit(meuDisplay); //inicialização de OTA
    // camera.initCamera();
    Serial.end(); //terminada a inicialização, a porta serial não é mais necessária

    
    //inicializa os pinos de led
    
    for (int i = 0; i < numLeds; i++) {
      pinMode(leds[i], OUTPUT);
    }
    
  digitalWrite(FLASH_LIGHT_PIN, FLASH_LIGHT); //apaga a luz de flash - setup terminado
  
  meuDisplay.statusBar(wM.getWifiSignal(), true, hora.getDate(), hora.getHour());

  
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(1000);
    digitalWrite(leds[i], LOW);
  }
}

/* Função loop, executada a cada ciclo de máquina */
void loop() {
  
  ArduinoOTA.handle();
  
  if (wifiLedBlinkDelay.isExpired()){
    digitalWrite(WiFiLed, INT_LED);
    digitalWrite(INT_LED_PIN, !INT_LED);
    INT_LED = !INT_LED;
    wifiLedBlinkDelay.repeat();
  }

  if (displayLogoInterval.isExpired()){
    displayLogoFlag = !displayLogoFlag;
    displayLogoInterval.repeat();
  }
  
  if (displayLogoFlag){
    char mensagem[60]; 
    strcpy(mensagem, hora.getSaldacao());
    strcat(mensagem, ", por favor apresente seu QRCode para leitura");
    meuDisplay.displayPrint(mensagem, 0);    
  }
  else{meuDisplay.displayLogo();}
  meuDisplay.statusBar(wM.getWifiSignal(), true, hora.getDate(), hora.getHour());


}