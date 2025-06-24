#include <Arduino.h>
#include "SensorManager.h"
#include "SensorTest.h"
#include "DisplayManager.h"
#include "DisplayTest.h"
#include "WebServerManager.h"

// Instancias de los componentes
SensorManager sensorManager;
DisplayManager displayManager;
WebServerManager serverManager(sensorManager, displayManager); 
int counter = 0;

enum MenuState { MAIN_MENU, TEST_MENU, RUN_PROGRAM };
MenuState menuState = MAIN_MENU;

void printMainMenu() {
    Serial.println("=== MENÚ PRINCIPAL ===");
    Serial.println("1 - Ejecutar Programa Principal");
    Serial.println("2 - Iniciar Tests");
}

void printTestMenu() {
    Serial.println("=== MENÚ DE TESTS ===");
    Serial.println("1 - Test del Sensor");
    Serial.println("2 - Test del Display");
    Serial.println("3 - Test del WiFi");
    Serial.println("4 - Test de la Web");
    Serial.println("5 - Test de Botones");
    Serial.println("0 - Volver al Menú Principal");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    printMainMenu();
}

void loop() {
    switch (menuState) {
        case MAIN_MENU:
        case TEST_MENU:
            if (Serial.available()) {
                char option = Serial.read();

                if (menuState == MAIN_MENU) {
                    if (option == '1') {
                        Serial.println("Ejecutando Programa Principal...");
                        menuState = RUN_PROGRAM;
                    } else if (option == '2') {
                        printTestMenu();
                        menuState = TEST_MENU;
                    } else {
                        Serial.println("Opción no válida. Selecciona una opción válida.");
                        printMainMenu();
                    }
                } else if (menuState == TEST_MENU) {
                    switch (option) {
                        case '1':
                            Serial.println("Seleccion: Test del Sensor");
                            setupSensorTest(sensorManager);
                            Serial.println("Setup del Sensor completado.");
                            while (true) { 
                                loopSensorTest(sensorManager);
                            }
                            break;
                        case '2':
                            Serial.println("Seleccion: Test del Display");
                            setupDisplayTest(displayManager, sensorManager);
                            while (true) { 
                                loopDisplayTest(displayManager, sensorManager);
                            }
                            break;
                        
                        case '3':
                            Serial.println("Iniciando Test del WiFi...");
                        {
                            serverManager.begin();
                            Serial.print("Conéctate al AP: ");
                            Serial.println(WiFi.softAPIP());  // No uses webServer.getIPAddress()
                            while (true) {
                                serverManager.loop();
                                delay(10);
                            }
                        }
                            break;
                        case '0':
                            printMainMenu();
                            menuState = MAIN_MENU;
                            break;
                        default:
                            Serial.println("Opción no válida. Selecciona una opción válida.");
                            printTestMenu();
                            break;
                    }
                }
            }
            break;

        case RUN_PROGRAM:
            static bool started = false;
            if (!started) {
                Serial.println("Aquí va el código principal del programa...");
                sensorManager.begin();
                sensorManager.setThreshold(500);
                sensorManager.setInterval(1);
                displayManager.begin();
                serverManager.begin();
                started = true;
            }
            serverManager.loop();
            // Aquí podrías poner una condición para volver al menú principal, por ejemplo:
            if (Serial.available() && Serial.read() == '0') {
                 printMainMenu();
                 menuState = MAIN_MENU;
                 started = false;
            }
            
            break;
    }

    delay(200);
}