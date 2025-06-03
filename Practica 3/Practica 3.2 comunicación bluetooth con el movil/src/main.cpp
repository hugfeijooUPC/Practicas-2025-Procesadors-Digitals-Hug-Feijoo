#include "BluetoothSerial.h"        // Librería para comunicación Bluetooth Serial
#include "esp_bt_main.h"            // Librerías de soporte Bluetooth de ESP32
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"

// Verifica que el Bluetooth esté habilitado en la configuración
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable
it
#endif

BluetoothSerial SerialBT;           // Crea un objeto para comunicación Bluetooth Serial

void setup() {
    Serial.begin(115200);           // Inicializa la comunicación serie por USB a 115200 baudios
    SerialBT.begin("ESP32test");    // Inicializa Bluetooth con el nombre "ESP32test"
    Serial.println("The device started, now you can pair it with bluetooth!"); // Mensaje de inicio
}

void loop() {
    // Si hay datos disponibles en el puerto serie USB, los envía por Bluetooth
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }
    // Si hay datos recibidos por Bluetooth, los envía al puerto serie USB
    if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
    delay(20);                      // Espera 20 ms para evitar saturar el microcontrolador
}