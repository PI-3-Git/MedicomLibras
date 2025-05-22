#ifndef IOT_CONFIGS_H
#define IOT_CONFIGS_H
#include <Arduino.h>
#include <Esp.h>
#include <WiFi.h>
#include <esp_wifi.h>

// MQTT

#define MQTT_SERVER "9f069cb597904de3bd31199f7ea60f27.s1.eu.hivemq.cloud"
#define MQTT_USERNAME "Reserv"
#define MQTT_PASSWORD "Ccava1ca"

/*Formação do hostname - o hostname é composto pela concatenação de "MedicomL" e os últimos 2 octetos do endereço MAC da placa ESP32. */

// Obtém o endereço MAC da Wi-Fi e gera o HOSTNAME
static inline const char* get_mqtt_hostname() {
    static char hostname_buffer[32];
    uint8_t mac_addr_wifi[6];
    const char* hostname_base = "MedicomL";
    const char* hostname_suffix_format = "%02X%02X";

    if (esp_wifi_get_mac(WIFI_IF_STA, mac_addr_wifi) == ESP_OK) {
        snprintf(hostname_buffer, sizeof(hostname_buffer), "%s%s", hostname_base, hostname_suffix_format, mac_addr_wifi[4], mac_addr_wifi[5]);
    } else {
        snprintf(hostname_buffer, sizeof(hostname_buffer), "%sXXYY", hostname_base);
    }
    return hostname_buffer;
}

//Obtém o endereço MAC da placa
static uint8_t mac_addr_wifi[6];

#define HOSTNAME get_mqtt_hostname()

#endif // IOT_CONFIGS_H