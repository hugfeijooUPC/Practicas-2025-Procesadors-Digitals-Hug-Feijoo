#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// Definición de los pines para el RC522 en ESP32-S3
#define SS_PIN 10   // Pin SS (SDA) del RC522
#define RST_PIN 9   // Pin de Reset del RC522

// Pines SPI en ESP32-S3
#define SCK_PIN 12
#define MISO_PIN 13
#define MOSI_PIN 11

// Crear objeto MFRC522 para la comunicación con el lector RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(115200);  // Iniciar la comunicación serie
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); // Configurar SPI en ESP32-S3
    mfrc522.PCD_Init();  // Inicializar el lector RFID MFRC522

    Serial.println("Esperando tarjeta RFID...");
}

void loop() {
    // Verificar si hay una nueva tarjeta presente
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return; // No hay tarjeta, salir del loop
    }

    // Intentar leer la tarjeta
    if (!mfrc522.PICC_ReadCardSerial()) {
        return; // Fallo al leer la tarjeta
    }

    // Mostrar el UID de la tarjeta en Serial Monitor
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Detener la lectura de la tarjeta
    mfrc522.PICC_HaltA();
}