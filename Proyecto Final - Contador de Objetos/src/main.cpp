#include <Arduino.h>
#include "SensorManager.h"
#include "SensorTest.h"
#include "DisplayManager.h"
#include "DisplayTest.h"
#include "WebServerManager.h"

// Instancias de los componentes principales del sistema
SensorManager sensorManager;
DisplayManager displayManager;
WebServerManager serverManager(sensorManager, displayManager); 
int counter = 0; // Contador global (no siempre usado directamente)

// Estados posibles del menú principal
enum MenuState { MAIN_MENU, TEST_MENU, RUN_PROGRAM };
MenuState menuState = MAIN_MENU;

// Imprime el menú principal por Serial
void printMainMenu() {
    Serial.println("=== MENÚ PRINCIPAL ===");
    Serial.println("1 - Ejecutar Programa Principal");
    Serial.println("2 - Iniciar Tests");
}

// Imprime el menú de pruebas por Serial
void printTestMenu() {
    Serial.println("=== MENÚ DE TESTS ===");
    Serial.println("1 - Test del Sensor");
    Serial.println("2 - Test del Display");
    Serial.println("0 - Volver al Menú Principal");
}

// Configuración inicial del sistema
void setup() {
    Serial.begin(115200);
    delay(1000);
    printMainMenu(); // Muestra el menú principal al iniciar
}

// Bucle principal del programa
void loop() {
    switch (menuState) {
        // Manejo de los menús principal y de pruebas
        case MAIN_MENU:
        case TEST_MENU:
            if (Serial.available()) {
                char option = Serial.read();

                // Lógica para el menú principal
                if (menuState == MAIN_MENU) {
                    if (option == '1') {
                        Serial.println("Ejecutando Programa Principal...");
                        menuState = RUN_PROGRAM; // Cambia al modo de ejecución principal
                    } else if (option == '2') {
                        printTestMenu();
                        menuState = TEST_MENU; // Cambia al menú de pruebas
                    } else {
                        Serial.println("Opción no válida. Selecciona una opción válida.");
                        printMainMenu();
                    }
                } 
                // Lógica para el menú de pruebas
                else if (menuState == TEST_MENU) {
                    switch (option) {
                        case '1':
                            Serial.println("Seleccion: Test del Sensor");
                            setupSensorTest(sensorManager); // Inicializa el test del sensor
                            Serial.println("Setup del Sensor completado.");
                            // Bucle infinito para el test del sensor
                            while (true) { 
                                loopSensorTest(sensorManager);
                            }
                            break;
                        case '2':
                            Serial.println("Seleccion: Test del Display");
                            setupDisplayTest(displayManager, sensorManager); // Inicializa el test del display
                            // Bucle infinito para el test del display
                            while (true) { 
                                loopDisplayTest(displayManager, sensorManager);
                            }
                            break;
                        case '0':
                            printMainMenu();
                            menuState = MAIN_MENU; // Vuelve al menú principal
                            break;
                        default:
                            Serial.println("Opción no válida. Selecciona una opción válida.");
                            printTestMenu();
                            break;
                    }
                }
            }
            break;

        // Ejecución del programa principal
        case RUN_PROGRAM:
            static bool started = false;
            if (!started) {
                Serial.println("Aquí va el código principal del programa...");
                sensorManager.begin();           // Inicializa el sensor
                sensorManager.setThreshold(500); // Configura el umbral del sensor
                sensorManager.setInterval(1);    // Configura el intervalo de muestreo
                displayManager.begin();          // Inicializa el display
                serverManager.begin();           // Inicializa el servidor web
                started = true;
            }
            serverManager.loop(); // Ejecuta el bucle principal del servidor web

            // Permite volver al menú principal presionando '0' por Serial
            if (Serial.available() && Serial.read() == '0') {
                 printMainMenu();
                 menuState = MAIN_MENU;
                 started = false;
            }
            break;
    }

    delay(200); // Pequeño delay para evitar saturar el microcontrolador
}