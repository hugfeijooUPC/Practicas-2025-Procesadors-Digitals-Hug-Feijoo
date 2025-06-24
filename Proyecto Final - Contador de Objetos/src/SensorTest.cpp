#include <Arduino.h>
#include "SensorManager.h"
#include "SensorTest.h"


void setupSensorTest(SensorManager& sensor) {
    Serial.begin(115200);
    delay(1000);
    
    sensor.begin();
    Serial.println("Begin del Sensor completado.");
    sensor.setThreshold(500);
    Serial.println("Threshold del Sensor establecido a 500 mm.");
    sensor.setInterval(1);
    Serial.println("Intervalo del Sensor establecido a 1 ms.");
}

void loopSensorTest(SensorManager& sensor) {
    static int previousCount = 0; // Variable para almacenar el conteo anterior.

    sensor.readSensor();
    sensor.checkForObject();

    int count = sensor.getObjectCount();
    if (count > previousCount) { // Solo muestra el conteo si es mayor que el anterior.
        Serial.print("Conteo Actual: ");
        Serial.println(count);
        previousCount = count; // Actualiza el conteo anterior.
    }

    delay(1);
}
