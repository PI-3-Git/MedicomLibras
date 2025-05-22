/* Classe mqttSys
Implementa conectividade, envio e recebimento de mensagens MQTT no broker HiveMQ*/

#ifndef MQTTSYS_H
#define MQTTSYS_H

#include <Arduino.h>
#include <Esp.h>
#include <PubSubClient.h>
#include "iot_configs.h"


//Definições

#define MQTT_PACKET_SIZE 1024
#define MSG_BUFFER_SIZE (500)


class mqtt
{
    public:
        mqtt(void);                                     //Construtor da classe
        //Funções públicas
        void mqttInit(void);                            //Conexão ao broker
        //Getters
        PubSubClient* getClient();

        //Setters

        //Senders
        void mqttSendQRCodeData(int data);              //envia mensagem de dados do usuário
        void mqttSendMessage(const char* topic, const char* message);
        //Loop
        void mqttLoop(void);

    private:
        //Funções privadas à classe
        void mqttReconnect(void);                       //reconexão ao broker
        static void mqttCallbackStatic(char* topic, byte* payload, unsigned int length);//bridge estático de callback
        void mqttCallback(char* topic, byte* payload, unsigned int length); //instância de callback
        PubSubClient * client;                          //cliente MQTT
        static mqtt* mqttInstance;                      //ponteiro estático para a instância de callback

        //Funções auxiliares
        bool mqttCallbackCompareTopic(const char* topic, const char* expectedTopic); //compara o tópico recebido com o tópico esperado em um callback
        const char* buildMessage(const char* text1, const char* text2); //concatenador para duas strings em uma mensagem
        char* buildTopic(const char* subtopic);         //concatenador do tópico para envio
        char* buildTimetMessage(const time_t time);     //concatenador de mensagem e horário

        //Tópicos MQTT
        const char* rootTopic = HOSTNAME;
        const char* allTopics = "#";                    //Raiz
        const char* allTopicsData = "data/#";           //Diretório do subtópico data
        const char* allTopicsMsg = "msg/#";             //Diretório de mensagens ao usuário
        const char* allTopicsStatus = "status/#";       //Todos os tópicos de estado da máquina
        const char* isConnectedTopic = "isConnected";   //Máquina conectada ("Hello world")
        const char* userIdTopic = "userId";             //Subtópico de ID do usuário
        const char* msgTextTopic = "msgText";           //Subtópico de mensagem de texto
        const char* lastWillTopic = "will";             //Tópico de testamento

        //Configurações MQTT
        const int mqttPort = 8883;
        const char* mqttUsername = MQTT_USERNAME;
        const char* mqttPassword = MQTT_PASSWORD;
        const char* mqttServer = MQTT_SERVER;
        const String mqttClientId = String(HOSTNAME);
};

#endif  // MQTTSYS_H