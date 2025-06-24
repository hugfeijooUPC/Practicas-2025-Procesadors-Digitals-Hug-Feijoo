#include "SensorManager.h"
#include <Wire.h>

// Constructor: Inicializa los valores predeterminados para el umbral de distancia, 
// el intervalo de medición, el contador de objetos y el tiempo de la última medición.
SensorManager::SensorManager() : 
    distanceThreshold(100), // Milímetros
    measurementInterval(10), // Milisegundos
    objectCount(0), 
    lastMeasurementTime(0) {}

// Configura e inicializa el sensor VL53L1X.
// Si la inicialización falla, entra en un bucle infinito.
void SensorManager::begin() {
    Wire.begin(5, 6); // Inicializa el bus I2C en los pines 5 (SDA) y 6 (SCL)
    sensor.setTimeout(500); // Establece el tiempo máximo de espera para el sensor

    if (!sensor.init()) {
        Serial.println("Failed to initialize VL53L1X sensor");
        while (1); // Si falla, se queda en un bucle infinito
    }

    sensor.setDistanceMode(VL53L1X::Long); // Modo de distancia larga
    sensor.setMeasurementTimingBudget(20000); // Presupuesto de tiempo de medición
    sensor.startContinuous(measurementInterval); // Inicia medición continua
}

// Establece un nuevo umbral de distancia para detectar objetos.
void SensorManager::setThreshold(int newThreshold) {
    distanceThreshold = newThreshold;
}

// Establece un nuevo intervalo de medición y actualiza el presupuesto de tiempo del sensor.
void SensorManager::setInterval(int newInterval) {
    measurementInterval = newInterval;
    sensor.setMeasurementTimingBudget(measurementInterval); // Ajusta el presupuesto de tiempo al nuevo intervalo.
}

// Lee la distancia del sensor si ha pasado el intervalo de medición.
// También verifica si ocurrió un tiempo de espera durante la lectura.
void SensorManager::readSensor() {
    if (millis() - lastMeasurementTime >= measurementInterval) { // Verifica si ha pasado el intervalo.
        lastMeasurementTime = millis(); // Actualiza el tiempo de la última medición.
        int distance = sensor.readRangeContinuousMillimeters(); // Lee la distancia en milímetros.
        if (sensor.timeoutOccurred()) { // Verifica si ocurrió un tiempo de espera.
            Serial.println("Sensor timeout!");
        }
    }
}

// Verifica si un objeto está dentro del umbral de distancia.
// Si se detecta un objeto, incrementa el contador de objetos solo si es una nueva detección.
void SensorManager::checkForObject() {
    static bool objectDetected = false; // Variable para rastrear si un objeto ya fue detectado.
    int distance = sensor.readRangeContinuousMillimeters(); // Lee la distancia actual del sensor.

    if (distance < distanceThreshold && distance != 0) { // Verifica si la distancia está dentro del umbral.
        if (!objectDetected) { // Solo incrementa si no se ha detectado un objeto previamente.
            objectCount++; // Incrementa el contador de objetos detectados.
            Serial.print("Object detected. Count: ");
            Serial.println(objectCount); // Imprime el número de objetos detectados.
            objectDetected = true; // Marca que un objeto ha sido detectado.
        }
    } else {
        objectDetected = false; // Reinicia el estado si la distancia está fuera del umbral.
    }
}

// Devuelve el número total de objetos detectados.
int SensorManager::getObjectCount() {
    return objectCount;
}

// Reinicia el contador de objetos detectados a cero.
void SensorManager::resetCount() {
    objectCount = 0;
}

// Reinicia el estado del sensor y el contador.
void SensorManager::reset() {
    resetCount();                // Reinicia el contador de objetos
    lastMeasurementTime = 0;     // Reinicia el tiempo de la última medición
    sensor.stopContinuous();     // Detiene la medición continua (si aplica)
    begin();                     // Reinicia el hardware del sensor
}



