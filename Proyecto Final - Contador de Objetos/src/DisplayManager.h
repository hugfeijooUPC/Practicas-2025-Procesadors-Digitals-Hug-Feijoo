#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h> // Incluye la biblioteca principal de Arduino.
#include <Adafruit_GFX.h> // Incluye la biblioteca de gráficos de Adafruit.
#include <Adafruit_ST7789.h> // Incluye la biblioteca para el  display ST7789.

class DisplayManager {
private:
Adafruit_ST7789 tft;
uint8_t currentScreen;
int counter;
uint8_t colorIndex;
uint16_t colors[5];



void screenGraphics();
void screenDynamicText();
    

public:

    unsigned long graphStartTime;
    int graphData[60]; // Conteo por segundo
    bool graphRunning;
    
    // Constructor: Inicializa los valores predeterminados de la clase.
    DisplayManager();

    // Inicializa el display y lo configura para comenzar a medir.
    void begin();

    // Dibuja el menú principal en la pantalla.
    void updateScreen();

    //
    void drawMenu();

    //
    void screenCounter();

    //
    void startRealTimeGraph();

    //
    void updateRealTimeGraph();

    // Cambia a la pantalla de contador.
    void incrementCounter();

    //
    void setCounter(int value);
    
    //
    void reset();
    
    //
    void showWiFiInfo(IPAddress ip, const char* ssid, const char* password);
    
    //
    void showClientConnected(String ip = "");

    void drawFinalGraph();


};

#endif // DISPLAYMANAGER_H