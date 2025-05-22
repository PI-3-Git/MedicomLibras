#include <Arduino.h>
#include <string>
#include <atomic>

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LittleFS.h>
#include <EEPROM.h>

#include "mqttSys.h"

const char* root_ca =
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";

//variáveis globais
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

//Construção das instâncias
mqtt* mqtt::mqttInstance = nullptr;

WiFiClientSecure espclient;
PubSubClient* client =nullptr;

//Construtor da classe
mqtt::mqtt(void){
    client = new PubSubClient(espclient); // Aloca memória para o objeto PubSubClient
    mqttInstance = this; // Define a instância para o callback estático
}

//Getter para o cliente
PubSubClient* mqtt::getClient() { return client; }


/*Conexão MQTT*/
//Inicialização MQTT
void mqtt::mqttInit(void){
    espclient.setCACert(root_ca);
    const char* initialMsg = buildMessage(HOSTNAME, "isOnLine");
    client->setServer(mqttServer, mqttPort);   
    client->setCallback(mqtt::mqttCallbackStatic);// Callback através da ponte estática
    
    while (!client->connected()) { 
        // Tentativa de conexão
        if (client->connect(mqttClientId.c_str(), mqttUsername, mqttPassword, buildTopic(lastWillTopic), 0, true, "Caiu...")) {
        // Uma vez conectado publica uma mensagem…
        snprintf (msg, MSG_BUFFER_SIZE, initialMsg, value);
        client->publish(buildTopic(isConnectedTopic), initialMsg);
        // … e resubescreve
        client->subscribe(buildTopic(allTopics));// Subescrição em todos os tópicos 
        } 
    delay(500);
    } 
}

//Reconexão MQTT
void mqtt::mqttReconnect(void) {
    static unsigned long lastReconnectAttempt = 0;
    const char* initialMsg = buildMessage(HOSTNAME, "isOnLine");
    unsigned long now = millis(); 
    if (now - lastReconnectAttempt < 5000) {    // Menor frequência de reconexão
      return;
    }  
    lastReconnectAttempt = now;  
    if (client->connect(mqttClientId.c_str(), mqttUsername, mqttPassword, buildTopic(lastWillTopic), 0, true, "caiu")) {
      client->publish(buildTopic(isConnectedTopic), initialMsg);
      client->subscribe(buildTopic(allTopics));
    }
}

//Loop MQTT
void mqtt::mqttLoop(void){
    if (!client->connected()) { mqttReconnect(); }
    client->loop();
}

/*Callback MQTT*/
//Bridge estático de callback
void mqtt::mqttCallbackStatic(char* topic, byte* payload, unsigned int length) {
    if (mqttInstance != nullptr) { // Verifica se a instância é válida
        mqttInstance->mqttCallback(topic, payload, length); // Chama o método da instância
    }
}

//Verificação de tópico de callback
bool mqtt::mqttCallbackCompareTopic(const char* topic, const char* expectedTopic) {
    const char* builtTopic = buildTopic(expectedTopic);
    bool result = false;

    if (builtTopic != nullptr && topic != nullptr) { // Verifica ponteiros nulos
        size_t builtTopicLen = strlen(builtTopic);

        if (strlen(topic) >= builtTopicLen) {
            result = strncmp(topic, builtTopic, builtTopicLen) == 0;
        }
    }

    delete[] builtTopic; // Libera a memória

    return result;
}

//Tratamento de callback
void mqtt::mqttCallback(char* topic, byte* payload, unsigned int length){
    //EM ADAPTAÇÃO
//     if (mqttCallbackCompareTopic(topic, pumpTopic)) { // Verifica se o tópico é o da bomba
//         char message[length + 1]; // Cria um buffer para a mensagem, +1 para o caractere nulo
//         for (unsigned int i = 0; i < length; i++) {
//             message[i] = (char)payload[i];
//         }
//         message[length] = '\0'; // Adiciona o caractere nulo ao final da string
//         if (strcmp(message, "PUMP_ON") == 0) {
//             digitalWrite(SysIni::PUMP, HIGH);
//             client->publish(buildTopic(pumpTopic), "cmd ligar bomba recebido");
//             // Serial.println("Bomba ligada!");
            
//             if(digitalRead(SysIni::PUMP)){
//                 client->publish(buildTopic(pumpTopic), buildMessage("Bomba ligada com sucesso!"));
//                 client->publish(buildTopic(pumpTopic), "ligada");
//                 PUMP_MANUAL=true;
//             }
//             else{
//                 client->publish(buildTopic(pumpTopic), "Erro ao ligar bomba!");
//                 PUMP_MANUAL=false;
//             }
            
//         } else if(strcmp(message, "PUMP_OFF") == 0) {
//             digitalWrite(SysIni::PUMP, LOW);
//             client->publish(buildTopic(pumpTopic), "cmd desligar bomba recebido");
//             Serial.println("Bomba desligada!");
//             if(!digitalRead(SysIni::PUMP)){
//                 client->publish(buildTopic(pumpTopic), buildMessage("Bomba desligada com sucesso!"));
//                 client->publish(buildTopic(pumpTopic), "desligada");
//                 PUMP_MANUAL=false;
//             }
//             else{
//                 client->publish(buildTopic(pumpTopic), "Erro ao desligar bomba!");
//                 PUMP_MANUAL=false;
//             }
//         }
//     }
//     else if (mqttCallbackCompareTopic(topic, report)){
//         char message[length + 1]; // Cria um buffer para a mensagem, +1 para o caractere nulo
//         for (unsigned int i = 0; i < length; i++) {
//             message[i] = (char)payload[i];
//         }
//         message[length] = '\0'; // Adiciona o caractere nulo ao final da string
//         if (strcmp(message, "report") == 0) {
//             mqttSendReport();
//         }   
//     }
//     else  if (mqttCallbackCompareTopic(topic, flowReadEnableTopic)) {
//         // Handle flowReadEnable topic
//         char message[length + 1];
//         for (unsigned int i = 0; i < length; i++) {
//             message[i] = (char)payload[i];
//         }
//         message[length] = '\0'; // Null-terminate

//         if (strcmp(message, "true") == 0) {
//             flowMeasurementEnable = true; // Set the atomic variable directly
//             Serial.println("Flow measurement enabled via MQTT");
//             mqttSendMessage(buildTopic(flowReadEnableTopic),"Flow measurement enabled via MQTT");
//         } else if (strcmp(message, "false") == 0) {
//             flowMeasurementEnable = false; // Set the atomic variable directly
//             Serial.println("Flow measurement disabled via MQTT");
//             mqttSendMessage(buildTopic(flowReadEnableTopic),"Flow measurement disabled via MQTT");

//         } else {
//           Serial.println("Invalid message for flowReadEnable topic");
//           mqttSendMessage(buildTopic(flowReadEnableTopic),"Invalid message for flowReadEnable topic");
//         }
//     }
//     else if (mqttCallbackCompareTopic(topic, restart)){
//         ESP.restart();
//     }
}

/*Senders*/

//Envio de mensagem
void mqtt::mqttSendMessage(const char* topic, const char* message){
    
}

/*Funções auxiliares*/
//Concatenador de tópico e subtópicos
char* mqtt::buildTopic(const char* subtopic) {
    if (rootTopic == nullptr || subtopic == nullptr) {
        return nullptr;                         // manipulação de entrada nula
    }
    size_t len = strlen(rootTopic) + strlen(subtopic) + 2; // +1 para '/' e +1 para '\0'
    char* topic = new char[len];                // usa uma nova char[]
    if (topic == nullptr) {                     // Verifica falha de alocação de memória
        Serial.println("Erro: falha de alocação de memória");
        return nullptr;
    }
    strcpy(topic, rootTopic);
    strcat(topic, "/"); // Separador
    strcat(topic, subtopic);
    return topic;
}

//Concatenador de mensagem e horário time_t
char* mqtt::buildTimetMessage(const time_t timet) {
    static char msg[9];
    struct tm *tmp = gmtime(&timet);
    strftime(msg, sizeof(msg), "%H:%M:%S", tmp);
    return msg;
}

//Concatenador para duas strings em uma mensagem
const char* mqtt::buildMessage(const char* text1, const char* text2) {
    static char msg[64];
    snprintf(msg, sizeof(msg), text1, text2);
    return msg;
}