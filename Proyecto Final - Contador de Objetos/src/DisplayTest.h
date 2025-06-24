#ifndef DISPLAYTEST_H
#define DISPLAYTEST_H

#include "DisplayManager.h"   // Incluye la definición de la clase DisplayManager
#include "SensorManager.h"    // Incluye la definición de la clase SensorManager

// Inicializa el entorno de pruebas para el display y el sensor.
// Muestra el menú de pruebas por Serial y prepara los dispositivos.
void setupDisplayTest(DisplayManager& display, SensorManager& sensor);

// Bucle principal de pruebas para el display y el sensor.
// Ejecuta el modo de prueba seleccionado por el usuario.
void loopDisplayTest(DisplayManager& display, SensorManager& sensor);

#endif // DISPLAYTEST_H