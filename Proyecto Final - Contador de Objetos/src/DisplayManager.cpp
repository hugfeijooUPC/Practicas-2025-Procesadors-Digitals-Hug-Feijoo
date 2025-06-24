#include "DisplayManager.h"
#include "SensorManager.h"

// Constructor: Inicializa los valores predeterminados
DisplayManager::DisplayManager() : 
    tft(10, 9, 8),
    currentScreen(0),
    counter(0),
    colorIndex(0),
    colors{ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_YELLOW, ST77XX_CYAN} {}

// Configura e inicializa el display ST7789.
void DisplayManager::begin() {
    tft.init(170, 320);
    tft.setRotation(1);
    drawMenu();
}

void DisplayManager::drawMenu() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Menu:");
    tft.setCursor(10, 60);
    tft.println("1. Conteo en tiempo real");
    tft.setCursor(10, 100);
    tft.println("2. Generar grafica Producción 60s");
}

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

void DisplayManager::screenGraphics() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(colors[colorIndex]);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Graphics Screen");

    for (int i = 0; i < 5; i++) {
        tft.drawLine(0, i * 50, 170, i * 50, colors[i]);
        tft.drawCircle(85, 200, i * 20, colors[i]);
    }

    colorIndex = (colorIndex + 1) % 5;
}

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

    messageIndex = (messageIndex + 1) % 5;
}

void DisplayManager::startRealTimeGraph() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Bar Graph");

    // Limpia datos previos
    for (int i = 0; i < 60; i++) {
        graphData[i] = 0;
    }

    Serial.println("startRealTimeGraph llamado, graphData reiniciado.");

    graphStartTime = millis();
    graphRunning = true;
}

void DisplayManager::updateRealTimeGraph() {
    if (!graphRunning) return;

    unsigned long elapsed = (millis() - graphStartTime) / 1000;

    if (elapsed >= 60) {
        graphRunning = false;
        return;
    }

    // Copia el valor anterior si el actual está en 0 y no es el primer segundo
    if (elapsed > 0 && graphData[elapsed] == 0) {
        graphData[elapsed] = graphData[elapsed - 1];
    }
    // Asegura que nunca decrezca
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

    // Texto informativo
    tft.setCursor(10, 180);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.printf("Segundo: %lu", elapsed);
}


void DisplayManager::updateScreen() {
    switch (currentScreen) {
        case 0: drawMenu(); break;
        case 1: screenCounter(); break;
        case 2: screenGraphics(); break;
        case 3: screenDynamicText(); break;
    }
    currentScreen = (currentScreen + 1) % 4;
}

void DisplayManager::incrementCounter() {
    counter++;
    if (graphRunning) {
        unsigned long elapsed = (millis() - graphStartTime) / 1000;
        if (elapsed < 60) {
            if (elapsed > 0 && graphData[elapsed] < graphData[elapsed - 1]) {
                graphData[elapsed] = graphData[elapsed - 1];
            }
            graphData[elapsed]++;
        }
    }
}

void DisplayManager::setCounter(int value) {
    counter = value;
}

void DisplayManager::reset() {
    setCounter(0);
    colorIndex = 0;
    graphRunning = false;
    for (int i = 0; i < 60; i++) graphData[i] = 0;
    begin();  // ← Reconfigura display
}

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
    tft.print("PASS: ");
    tft.println(password);

    tft.setCursor(10, 130);
    tft.print("IP: ");
    tft.println(ip);
}

void DisplayManager::showClientConnected(String ip) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(2);
    tft.setCursor(10, 60);
    tft.println("¡Cliente conectado!");
    if (ip.length() > 0) {
        tft.setCursor(10, 100);
        tft.println(ip);
    }
}

void DisplayManager::drawFinalGraph() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Grafica Final");

    int barWidth = 3;
    int spacing = 1;
    int baseY = 160;

    for (int i = 0; i < 60; i++) {
        int value = graphData[i];
        int barHeight = value;
        if (barHeight > 140) barHeight = 140;

        int x = 20 + i * (barWidth + spacing);
        int y = baseY - barHeight;
        tft.fillRect(x, y, barWidth, barHeight, colors[0]);
    }

    tft.setCursor(10, 180);
    tft.setTextSize(2);
    tft.printf("Total: %d", counter);
}

