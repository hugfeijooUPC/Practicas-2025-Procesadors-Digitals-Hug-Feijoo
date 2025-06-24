#include "WebServerTest.h"
#include "WebServerManager.h"

void setupWebServerTest(WebServerManager& server, int* counterPtr) {
    // En esta versión no usamos el counterPtr
    server.begin(); // Llamamos a begin() sin parámetros
}

void loopWebServerTest(WebServerManager& server) {
    server.loop();
}