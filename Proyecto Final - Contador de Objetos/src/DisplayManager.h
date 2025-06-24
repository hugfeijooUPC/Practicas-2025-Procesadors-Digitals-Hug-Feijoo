#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h> // Incluye la biblioteca principal de Arduino.
#include <Adafruit_GFX.h> // Incluye la biblioteca de gráficos de Adafruit.
#include <Adafruit_ST7789.h> // Incluye la biblioteca para el display ST7789.

class DisplayManager {
private:
    Adafruit_ST7789 tft;         // Objeto para controlar el display TFT.
    uint8_t currentScreen;       // Pantalla actual que se está mostrando.
    int counter;                 // Contador de objetos.
    uint8_t colorIndex;          // Índice para cambiar colores en gráficos.
    uint16_t colors[5];          // Arreglo de colores para gráficos.

    // Dibuja una pantalla con gráficos de líneas y círculos de colores.
    void screenGraphics();

    // Muestra textos dinámicos que cambian en cada llamada.
    void screenDynamicText();

public:
    unsigned long graphStartTime; // Marca de tiempo de inicio de la gráfica.
    int graphData[60];            // Datos de conteo por segundo para la gráfica.
    bool graphRunning;            // Indica si la gráfica está activa.

    // Constructor: Inicializa los valores predeterminados de la clase.
    DisplayManager();

    // Inicializa el display y lo configura para comenzar a medir.
    void begin();

    // Cambia de pantalla según el valor de currentScreen.
    void updateScreen();

    // Dibuja el menú principal en la pantalla.
    void drawMenu();

    // Muestra la pantalla de conteo de objetos en tiempo real.
    void screenCounter();

    // Inicia la gráfica de barras en tiempo real (60 segundos).
    void startRealTimeGraph();

    // Actualiza la gráfica de barras en tiempo real cada segundo.
    void updateRealTimeGraph();

    // Incrementa el contador y actualiza la gráfica si está activa.
    void incrementCounter();

    // Establece el valor del contador manualmente.
    void setCounter(int value);
    
    // Reinicia todos los valores y vuelve a mostrar el menú.
    void reset();
    
    // Muestra la información de la red WiFi en pantalla.
    void showWiFiInfo(IPAddress ip, const char* ssid, const char* password);
    
    // Muestra un mensaje cuando un cliente se conecta por WiFi.
    void showClientConnected(String ip = "");

    // Dibuja la gráfica final de barras con los datos recolectados.
    void drawFinalGraph();
};

#endif // DISPLAYMANAGER_H