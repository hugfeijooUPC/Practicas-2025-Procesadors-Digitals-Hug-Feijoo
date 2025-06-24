#include "DisplayManager.h"
#include "SensorManager.h"

// Constructor: Inicializa los valores predeterminados y el display
DisplayManager::DisplayManager() : 
    tft(10, 9, 8),
    currentScreen(0),
    counter(0),
    colorIndex(0),
    colors{ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_YELLOW, ST77XX_CYAN} {}

// Inicializa el display y muestra el menú principal
void DisplayManager::begin() {
    tft.init(170, 320);
    tft.setRotation(1);
    drawMenu();
}

// Dibuja el menú principal en pantalla
void DisplayManager::drawMenu() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Menu Principal:");
    tft.setCursor(10, 60);
    tft.println("1. Conteo en tiempo real");
    tft.setCursor(10, 100);
    tft.println("2. Generar grafica de 60s");
}

// Muestra la pantalla de conteo de objetos en tiempo real
void DisplayManager::screenCounter() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Numero de Objetos:");
    tft.setTextSize(4);
    tft.setCursor(10, 100);
    tft.printf("Count: %d", counter);
}

// Muestra una pantalla gráfica con líneas y círculos de colores
void DisplayManager::screenGraphics() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(colors[colorIndex]);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Graphics Screen");

    // Dibuja líneas y círculos de diferentes colores
    for (int i = 0; i < 5; i++) {
        tft.drawLine(0, i * 50, 170, i * 50, colors[i]);
        tft.drawCircle(85, 200, i * 20, colors[i]);
    }

    // Cambia el color para la próxima vez
    colorIndex = (colorIndex + 1) % 5;
}

// Muestra textos dinámicos que cambian en cada llamada
void DisplayManager::screenDynamicText() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Dynamic Text Screen");

    const char* messages[] = {"Hello!", "Adafruit ST7789", "ESP32-S3", "PlatformIO", "Arduino"};
    static uint8_t messageIndex = 0;

    tft.setCursor(10, 100);
    tft.setTextSize(3);
    tft.println(messages[messageIndex]);

    // Cambia el mensaje para la próxima vez
    messageIndex = (messageIndex + 1) % 5;
}

// Inicia la gráfica de barras en tiempo real (60 segundos)
void DisplayManager::startRealTimeGraph() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Bar Graph");

    // Limpia los datos previos de la gráfica
    for (int i = 0; i < 60; i++) {
        graphData[i] = 0;
    }

    Serial.println("startRealTimeGraph llamado, graphData reiniciado.");

    graphStartTime = millis();
    graphRunning = true;
}

// Actualiza la gráfica de barras en tiempo real cada segundo
void DisplayManager::updateRealTimeGraph() {
    if (!graphRunning) return;

    unsigned long elapsed = (millis() - graphStartTime) / 1000;

    // Detiene la gráfica después de 60 segundos
    if (elapsed >= 60) {
        graphRunning = false;
        return;
    }

    // Mantiene el valor anterior si el actual es 0 (excepto el primer segundo)
    if (elapsed > 0 && graphData[elapsed] == 0) {
        graphData[elapsed] = graphData[elapsed - 1];
    }
    // Evita que el valor decrezca
    if (elapsed > 0 && graphData[elapsed] < graphData[elapsed - 1]) {
        graphData[elapsed] = graphData[elapsed - 1];
    }

    int barWidth = 3;
    int spacing = 1;
    int baseY = 160;

    int value = graphData[elapsed];
    int barHeight = value * 1;
    if (barHeight > 140) barHeight = 140; // altura máxima

    Serial.print(" | Valor: ");
    Serial.print(value);
    Serial.print(" | Altura barra: ");
    Serial.println(barHeight);

    int x = 20 + elapsed * (barWidth + spacing);
    int y = baseY - barHeight;

    // Dibuja la barra correspondiente
    tft.fillRect(x, y, barWidth, barHeight, colors[0]);

    // Muestra el segundo actual en pantalla
    tft.setCursor(10, 180);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.printf("Segundo: %lu", elapsed);
}

// Cambia de pantalla según el valor de currentScreen
void DisplayManager::updateScreen() {
    switch (currentScreen) {
        case 0: drawMenu(); break;
        case 1: screenCounter(); break;
        case 2: screenGraphics(); break;
        case 3: screenDynamicText(); break;
    }
    // Avanza a la siguiente pantalla
    currentScreen = (currentScreen + 1) % 4;
}

// Incrementa el contador y actualiza la gráfica si está activa
void DisplayManager::incrementCounter() {
    counter++;
    if (graphRunning) {
        unsigned long elapsed = (millis() - graphStartTime) / 1000;
        if (elapsed < 60) {
            // Mantiene el valor anterior si es menor
            if (elapsed > 0 && graphData[elapsed] < graphData[elapsed - 1]) {
                graphData[elapsed] = graphData[elapsed - 1];
            }
            graphData[elapsed]++;
        }
    }
}

// Establece el valor del contador manualmente
void DisplayManager::setCounter(int value) {
    counter = value;
}

// Reinicia todos los valores y vuelve a mostrar el menú
void DisplayManager::reset() {
    setCounter(0);
    colorIndex = 0;
    graphRunning = false;
    for (int i = 0; i < 60; i++) graphData[i] = 0;
    begin();  // ← Reconfigura display
}

// Muestra la información de la red WiFi en pantalla
void DisplayManager::showWiFiInfo(IPAddress ip, const char* ssid, const char* password) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("WiFi Iniciado");

    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.print("SSID: ");
    tft.println(ssid);

    tft.setCursor(10, 90);
    tft.print("PASSWORD: ");
    tft.println(password);

    tft.setCursor(10, 130);
    tft.print("IP: ");
    tft.println(ip);
}

// Muestra un mensaje cuando un cliente se conecta por WiFi
void DisplayManager::showClientConnected(String ip) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(2);
    tft.setCursor(10, 60);
    tft.println("¡Cliente conectado!");
    if (ip.length() > 0) {
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(10, 100);
        tft.println("Conectarse a la IP:");
        tft.setCursor(10, 120);
        tft.println(ip);
    }
}

// Dibuja la gráfica final de barras con los datos recolectados
void DisplayManager::drawFinalGraph() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Grafica Final");

    int barWidth = 3;
    int spacing = 1;
    int baseY = 160;

    // Dibuja las barras de la gráfica final
    for (int i = 0; i < 60; i++) {
        int value = graphData[i];
        int barHeight = value;
        if (barHeight > 140) barHeight = 140;

        int x = 20 + i * (barWidth + spacing);
        int y = baseY - barHeight;
        tft.fillRect(x, y, barWidth, barHeight, colors[0]);
    }

    // Muestra el total de objetos contados
    tft.setCursor(10, 180);
    tft.setTextSize(2);
    tft.printf("Total: %d", counter);
}

