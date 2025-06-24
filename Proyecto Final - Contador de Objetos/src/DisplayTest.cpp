#include <Arduino.h>
#include "DisplayManager.h"
#include "DisplayTest.h"
#include "SensorManager.h"
#include "SensorTest.h"

// Variables globales para el modo de prueba
bool modeSelected = false;   // Indica si ya se seleccionó un modo
char selectedMode = 0;       // Almacena el modo seleccionado por el usuario

// Inicializa el display y el sensor, y muestra el menú de pruebas por Serial
void setupDisplayTest(DisplayManager& display, SensorManager& sensor) {
    Serial.begin(115200);
    delay(1000);
    setupSensorTest(sensor); // Inicializa el sensor antes de iniciar el display
    delay(1000);
    Serial.println("Iniciando Test del Display...");
    Serial.println("1 - Conteo en tiempo real");
    Serial.println("2 - Generar gráfica (60s)");
    
    display.begin(); // Muestra el menú en el display
}

// Bucle principal de pruebas para el display y el sensor
void loopDisplayTest(DisplayManager& display, SensorManager& sensor) {
    // Si aún no se eligió un modo, espera entrada del usuario por Serial
    if (!modeSelected) {
        if (Serial.available()) {
            selectedMode = Serial.read();
            modeSelected = true;
            Serial.print("Modo seleccionado: ");
            Serial.println(selectedMode);
        }
        return;
    }

    // MODO 1: Conteo en tiempo real
    static unsigned long lastDisplayUpdate = 0;

    if (selectedMode == '1') {
        // Lee el sensor y actualiza el contador
        sensor.readSensor();
        sensor.checkForObject();
        int count = sensor.getObjectCount();
        display.setCounter(count);

        // Actualiza la pantalla cada 250 ms
        if (millis() - lastDisplayUpdate >= 250) {
            display.screenCounter();
            Serial.print("Conteo actual: ");
            Serial.println(count);
            lastDisplayUpdate = millis();
        }

        delay(1); // Pequeño delay para evitar saturar CPU
    }

    // MODO 2: Gráfica en tiempo real durante 60 segundos
    else if (selectedMode == '2') {
        display.startRealTimeGraph(); // Limpia y prepara la gráfica
        unsigned long start = millis();
        static int lastRecordedCount = 0;
        lastRecordedCount = sensor.getObjectCount();

        // Bucle de 60 segundos para graficar el conteo
        while (millis() - start < 60000) {
            display.updateRealTimeGraph();
            
            sensor.readSensor();
            sensor.checkForObject();
            int count = sensor.getObjectCount();
            int delta = count - lastRecordedCount;
            
            // Si hay nuevos objetos, incrementa el contador en la gráfica
            if (delta > 0) {
                for (int i = 0; i < delta; i++) {
                    display.incrementCounter();
                }
                lastRecordedCount = count;
            }

            display.setCounter(count);  // Actualiza el número en pantalla

            // Permitir cancelar con 'm'
            if (Serial.available()) {
                char c = Serial.read();
                if (c == 'm' || c == 'M') {
                    Serial.println("Salida anticipada. Volviendo al menú...");
                    break;
                }
            }
            delay(50);
        }

        Serial.println("Gráfica finalizada. Volviendo al menú...");
        modeSelected = false;
        selectedMode = 0;
        display.drawMenu();
    }

    // MODO 3: Simulación de gráfica con datos de ejemplo
    else if (selectedMode == '3') {
        display.startRealTimeGraph();  // Limpia la pantalla
    
        // Simular 60 segundos con valores de ejemplo
        for (int i = 0; i < 60; i++) {
            display.graphData[i] = (i % 15) + 3; // ejemplo entre 3–17
        }
    
        display.graphStartTime = millis() - 60000;
        display.graphRunning = true;
    
        // Mostrar la gráfica completa en bucle hasta que el usuario cancele
        while (true) {
            display.updateRealTimeGraph();
    
            if (Serial.available()) {
                char c = Serial.read();
                if (c == 'm' || c == 'M') {
                    break;
                }
            }
    
            delay(100);
        }
    
        Serial.println("Gráfica simulada generada.");
        modeSelected = false;
        selectedMode = 0;
        display.drawMenu();
    }
    
    // MODO inválido: cualquier otro valor
    else {
        Serial.println("Modo no válido. Introduce 1 o 2:");
        modeSelected = false;
        selectedMode = 0;
    }
}