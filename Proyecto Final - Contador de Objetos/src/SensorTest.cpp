#include <Arduino.h>
#include "SensorManager.h"
#include "SensorTest.h"

// Inicializa el sensor y configura los parámetros de prueba.
void setupSensorTest(SensorManager& sensor) {
    Serial.begin(115200);           // Inicia la comunicación Serial.
    delay(1000);                    // Espera para asegurar la inicialización.
    
    sensor.begin();                 // Inicializa el sensor.
    Serial.println("Begin del Sensor completado.");
    sensor.setThreshold(500);       // Establece el umbral de detección a 500 mm.
    Serial.println("Threshold del Sensor establecido a 500 mm.");
    sensor.setInterval(1);          // Establece el intervalo de muestreo a 1 ms.
    Serial.println("Intervalo del Sensor establecido a 1 ms.");
}

// Bucle de prueba para el sensor: lee y muestra el conteo de objetos detectados.
void loopSensorTest(SensorManager& sensor) {
    static int previousCount = 0; // Variable para almacenar el conteo anterior.

    sensor.readSensor();          // Lee la distancia del sensor.
    sensor.checkForObject();      // Verifica si se detectó un nuevo objeto.

    int count = sensor.getObjectCount(); // Obtiene el conteo actual de objetos.
    if (count > previousCount) { // Solo muestra el conteo si es mayor que el anterior.
        Serial.print("Conteo Actual: ");
        Serial.println(count);
        previousCount = count;   // Actualiza el conteo anterior.
    }

    delay(1); // Pequeño delay para evitar saturar el microcontrolador.
}
