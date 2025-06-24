#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include <Arduino.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <functional>

#include "SensorManager.h"
#include "DisplayManager.h"

class WebServerManager {
public:
    WebServerManager(SensorManager& sensorMgr, DisplayManager& displayMgr);
    void begin();
    void loop();
    void handleGraphData();
    void handleWaitingPage();

private:

    // Para controlar Modo 2 de forma no estática
    bool mode2Started;
    unsigned long mode2StartTime;
    int mode2LastCount;

    // Referencias a otros managers
    SensorManager& sensorManager;
    DisplayManager& displayManager;

    // Servidores
    WebServer server;
    WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket en puerto 81

    // Estado de modo
    char selectedMode = 0;
    bool modeSelected = false;

    // Datos de gráfica
    unsigned long graphStartTime;
    int graphData[60];
    bool graphRunning;

    // Métodos de inicialización
    void setupAP();
    void setupServer();

    // Manejo de peticiones HTTP
    void handleRoot();
    void handleNotFound();
    void handleModeStatus();

    // Manejo de WebSocket
    void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

    // Visualización de gráfica
    void updateGraphData();
    void generateGraphHTML(String& html);
};

#endif // WEBSERVERMANAGER_H

