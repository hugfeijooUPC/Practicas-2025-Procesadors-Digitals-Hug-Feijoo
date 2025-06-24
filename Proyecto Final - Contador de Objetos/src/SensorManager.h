#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Arduino.h> // Incluye la biblioteca principal de Arduino.
#include <VL53L1X.h> // Incluye la biblioteca para el sensor VL53L1X.

class SensorManager {
private:
    VL53L1X sensor; // Objeto para interactuar con el sensor VL53L1X.
    int distanceThreshold; // Umbral de distancia para detectar objetos (en milímetros).
    int measurementInterval; // Intervalo de tiempo entre mediciones (en milisegundos).
    int objectCount; // Contador de objetos detectados.
    unsigned long lastMeasurementTime; // Marca de tiempo de la última medición.

public:
    // Constructor: Inicializa los valores predeterminados de la clase.
    SensorManager();

    // Inicializa el sensor y lo configura para comenzar a medir.
    void begin();

    // Establece un nuevo umbral de distancia para la detección de objetos.
    void setThreshold(int newThreshold);

    // Establece un nuevo intervalo de tiempo entre mediciones.
    void setInterval(int newInterval);

    // Lee la distancia del sensor y actualiza los datos si ha pasado el intervalo.
    void readSensor();

    // Verifica si un objeto está dentro del umbral de distancia y actualiza el contador.
    void checkForObject();

    // Devuelve el número total de objetos detectados.
    int getObjectCount();

    // Reinicia el contador de objetos detectados a cero.
    void resetCount();

    // Reinicia la clase a sus valores predeterminados
    void reset(); 
};

#endif // SENSORMANAGER_H


