#include <Arduino.h>
#include "DisplayManager.h"
#include "DisplayTest.h"
#include "SensorManager.h"
#include "SensorTest.h"


bool modeSelected = false;
char selectedMode = 0;

void setupDisplayTest(DisplayManager& display, SensorManager& sensor) {
    Serial.begin(115200);
    delay(1000);
    setupSensorTest(sensor); // Inicializa el sensor antes de iniciar el display
    delay(1000);
    Serial.println("Iniciando Test del Display...");
    Serial.println("1 - Conteo en tiempo real");
    Serial.println("2 - Generar gráfica (60s)");
    
    display.begin(); //Muestra el Menu
}

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
        sensor.readSensor();
        sensor.checkForObject();
        int count = sensor.getObjectCount();
        display.setCounter(count);

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
        display.startRealTimeGraph();
        unsigned long start = millis();
        static int lastRecordedCount = 0;
        lastRecordedCount = sensor.getObjectCount();

        while (millis() - start < 60000) {
            display.updateRealTimeGraph();
            
            sensor.readSensor();
            sensor.checkForObject();
            int count = sensor.getObjectCount();
            int delta = count - lastRecordedCount;
            
            if (delta > 0) {
                for (int i = 0; i < delta; i++) {
                    display.incrementCounter();
                }
                lastRecordedCount = count;
            }

            display.setCounter(count);  // sigue actualizando la cifra en pantalla

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

    else if (selectedMode == '3') {
        display.startRealTimeGraph();  // limpia la pantalla
    
        // Simular 60 segundos con valores
        for (int i = 0; i < 60; i++) {
            display.graphData[i] = (i % 15) + 3; // ejemplo entre 3–17
        }
    
        display.graphStartTime = millis() - 60000;
        display.graphRunning = true;
    
        // Mostrar la gráfica completa en bucle
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
    

    // MODO inválido
    else {
        Serial.println("Modo no válido. Introduce 1 o 2:");
        modeSelected = false;
        selectedMode = 0;
    }
}