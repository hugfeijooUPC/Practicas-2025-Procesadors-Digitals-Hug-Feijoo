// WebServerManager.cpp
extern "C" {
  #include "esp_wifi.h"
  #include "esp_event.h"
  #include "esp_system.h"
  #include "esp_mac.h"
}

#include "WebServerManager.h"
#include <WiFi.h>

// Constructor: inicializa referencias y variables internas
WebServerManager::WebServerManager(SensorManager& sensorMgr, DisplayManager& displayMgr)
    : server(80),
      sensorManager(sensorMgr),
      displayManager(displayMgr),
      graphStartTime(0),
      graphRunning(false),
      mode2Started(false),     // ← inicializamos la bandera de Modo 2
      mode2StartTime(0),       // ← reinicializamos el temporizador
      mode2LastCount(0)        // ← reinicializamos el último conteo
{
    memset(graphData, 0, sizeof(graphData)); // Inicializa el arreglo de datos de la gráfica
}

// Inicializa el punto de acceso WiFi, el servidor HTTP y el WebSocket
void WebServerManager::begin() {
    setupAP();        // Configura el punto de acceso WiFi
    setupServer();    // Configura las rutas del servidor HTTP
    server.begin();   // Inicia el servidor HTTP
    webSocket.begin();// Inicia el WebSocket
    webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        handleWebSocketMessage(num, type, payload, length);
    });
    graphStartTime = millis();
    graphRunning = true;
}

// Maneja los mensajes recibidos por WebSocket (cambio de modo, reset, etc.)
void WebServerManager::handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    Serial.print("[WS] Tipo: ");
    Serial.print((int)type);
    Serial.print(" Longitud: ");
    Serial.println(length);
    
    if (type == WStype_TEXT && length > 0) {
        char command = payload[0];

        // Cambia el modo según el comando recibido
        if (command == '1' || command == '2') {
            selectedMode = command;
            modeSelected = true;
            Serial.print("Modo recibido por WebSocket: ");
            Serial.println(command);
        }
        // Reinicia el sistema y vuelve al menú si recibe 'm' o 'M'
        else if (command == 'm' || command == 'M') {
            selectedMode = 0;
            modeSelected = false;
            sensorManager.resetCount();
            displayManager.drawMenu();

            // ✅ Reiniciar los datos de la gráfica
            memset(graphData, 0, sizeof(graphData));
            memset(displayManager.graphData, 0, sizeof(displayManager.graphData));
            // ← reiniciar bandera de Modo 2
            mode2Started = false;
            Serial.println("Modo cancelado. Volviendo al menú.");
        }
    }
}

// Bucle principal del servidor web: gestiona clientes, modos y actualizaciones
void WebServerManager::loop() {
    // 1. Verifica conexión WiFi de cliente y muestra en display si es nuevo
    static bool clientAlreadyShown = false;
    int numClients = WiFi.softAPgetStationNum();

    if (numClients > 0 && !clientAlreadyShown) {
        #ifdef ESP32
        wifi_sta_list_t staList;
        tcpip_adapter_sta_list_t adapterList;

        if (esp_wifi_ap_get_sta_list(&staList) == ESP_OK &&
            tcpip_adapter_get_sta_list(&staList, &adapterList) == ESP_OK &&
            adapterList.num > 0) {

            IPAddress clientIP = IPAddress(adapterList.sta[0].ip.addr);
            displayManager.showClientConnected(WiFi.softAPIP().toString());
            Serial.print("Cliente conectado desde IP: ");
            Serial.println(clientIP);
        } else {
            displayManager.showClientConnected("IP desconocida");
            Serial.println("Cliente conectado (IP no detectada)");
        }
        #endif

        clientAlreadyShown = true;
    }

    // 2. Lógica de red y sensor: si hay modo seleccionado, lee sensor
    if (modeSelected) {
        sensorManager.readSensor();
        sensorManager.checkForObject();
    }
    webSocket.loop();
    server.handleClient();

    static unsigned long lastDisplayUpdate = 0;

    // 3. Sin modo seleccionado, salir
    if (!modeSelected) return;

    // 4. MODO 1: Conteo en tiempo real
    if (selectedMode == '1') {
        int count = sensorManager.getObjectCount();
        displayManager.setCounter(count);

        // Actualiza el display y envía el conteo por WebSocket cada 250 ms
        if (millis() - lastDisplayUpdate >= 250) {
            displayManager.screenCounter();
            lastDisplayUpdate = millis();

            // ←← NUEVO: enviamos el conteo al cliente
            String msg = String("{\"total\":") + count + "}";
            webSocket.broadcastTXT(msg);
        }
    }

    // 5. MODO 2: Gráfica en tiempo real (60s)
    else if (selectedMode == '2') {
        // Si no hemos arrancado aún, inicializamos
        if (!mode2Started) {
            displayManager.startRealTimeGraph();
            mode2StartTime   = millis();
            graphStartTime   = mode2StartTime;              // para updateGraphData()
            mode2LastCount   = sensorManager.getObjectCount();
            mode2Started     = true;
            graphRunning     = true;
        }

        // Mientras no superemos 60s desde el inicio
        if (millis() - mode2StartTime < 60000) {
            displayManager.updateRealTimeGraph();

            int count = sensorManager.getObjectCount();
            int delta = count - mode2LastCount;
            if (delta > 0) {
                for (int i = 0; i < delta; i++) {
                    displayManager.incrementCounter();
                }
                mode2LastCount = count;
            }

            displayManager.setCounter(count);
            updateGraphData(); // Actualiza y envía los datos de la gráfica
        } else {
            // Al acabar los 60s
            Serial.println("Modo 2 finalizado.");
            modeSelected   = false;
            selectedMode   = 0;
            mode2Started   = false;  // ← dejamos listo para la siguiente vez
            graphRunning   = false;

            displayManager.drawFinalGraph();
            delay(5000);
            displayManager.drawMenu();
            sensorManager.resetCount();
            webSocket.broadcastTXT("redirect:/");
        }
    }
}

// Configura el punto de acceso WiFi y muestra la info en el display
void WebServerManager::setupAP() {
    const char* ssid = "ESP32_AP";
    const char* password = "12345678";

    WiFi.softAP(ssid, password);
    IPAddress ip = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(ip);

    displayManager.showWiFiInfo(ip, ssid, password);
}

// Configura las rutas del servidor HTTP
void WebServerManager::setupServer() {
    server.on("/", HTTP_GET, [this]() {
        handleRoot();
    });

    server.on("/mode", HTTP_GET, std::bind(&WebServerManager::handleModeStatus, this));
    server.on("/graphdata", HTTP_GET, std::bind(&WebServerManager::handleGraphData, this));  // ✅ NUEVA RUTA
    server.on("/wait", HTTP_GET, std::bind(&WebServerManager::handleWaitingPage, this));

    server.onNotFound([this]() {
        handleNotFound();
    });
}

// Actualiza los datos de la gráfica y los envía por WebSocket
void WebServerManager::updateGraphData() {
    if (!graphRunning) return;

    unsigned long elapsed = (millis() - graphStartTime) / 1000;
    if (elapsed >= 60) {
        graphRunning = false;
        return;
    }

    int currentCount = sensorManager.getObjectCount();
    graphData[elapsed] = currentCount;
    displayManager.graphData[elapsed] = currentCount;

    // Sólo enviamos el dato nuevo por WebSocket
    String msg = String("{\"time\":") + elapsed +
                 ",\"value\":" + currentCount +
                 ",\"total\":" + currentCount + "}";
    webSocket.broadcastTXT(msg);
}

// Genera el HTML de la página principal con la gráfica y los controles
void WebServerManager::generateGraphHTML(String& html) {
    html += R"rawliteral(
    <!DOCTYPE html>
    <html lang="es">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Control de Conteo y Gráfica</title>
      <style>
        body {
          margin: 0; padding: 0;
          font-family: sans-serif;
          background: linear-gradient(180deg, #ffffff, #f0f0f0);
          color: #333;
          min-height: 100vh;
          display: flex;
          flex-direction: column;
          align-items: center;
        }
        h2 {
          font-size: 1.8rem;
          margin: 1rem 0;
        }
        .buttons {
          display: flex;
          gap: 10px;
          margin-bottom: 1rem;
        }
        button {
          padding: 10px 20px;
          border: none;
          border-radius: 20px;
          background: #000;
          color: #fff;
          font-weight: bold;
          cursor: pointer;
          transition: transform 0.2s, background 0.3s;
        }
        button:hover { background: #333; transform: translateY(-2px); }
        button.active { background: crimson; }
        .bar-container {
          display: flex;
          align-items: flex-end;
          gap: 1px;               /* igual spacing del display */
          height: 250px;
          background: #fff;
          border-left: 2px solid #333;
          border-bottom: 2px solid #333;
          overflow-x: auto;
          padding: 10px;
        }
        .bar {
          width: 3px;             /* igual barWidth del display */
          background: crimson;
          border-radius: 3px 3px 0 0;
          transition: height 0.3s ease-out;
        }
        .counter {
          font-size: 2rem;
          text-align: center;
          margin: 0.5rem 0;
        }
        #ws-status {
          position: fixed; top: 10px; right: 10px;
          width: 10px; height: 10px; border-radius: 50%;
          background: red;
        }
        #ws-status.connected { background: green; }
      </style>
    </head>
    <body>
      <div id="ws-status"></div>

      <h2>Contador de Objetos</h2>
      <div class="buttons">
        <button id="btnCount" onclick="sendMode('1')">Modo 1 – Conteo</button>
        <button id="btnGraph" onclick="sendMode('2')">Modo 2 – Gráfica</button>
        <button onclick="sendMenu()">Reset</button>
      </div>

      <!-- Gráfica vacía; solo aparece en Modo 2 -->
      <div class="bar-container" id="barContainer" style="display:none"></div>
      <script>
        // Pre-crea 60 div.bar con height=0
        window._preCreateBars = () => {
          const c = document.getElementById('barContainer');
          for (let i = 0; i < 60; i++) {
            const b = document.createElement('div');
            b.className = 'bar';
            b.style.height = '0px';
            c.appendChild(b);
          }
        };
      </script>

      <!-- Contador siempre visible -->
      <p class="counter" id="totalCounter">Total: )rawliteral";
    // Valor inicial del contador
    html += String(sensorManager.getObjectCount());
    html += R"rawliteral(</p>

      <script>
        let socket;
        const btnCount = document.getElementById('btnCount');
        const btnGraph = document.getElementById('btnGraph');
        const wsStatus = document.getElementById('ws-status');

        function connectWebSocket() {
          socket = new WebSocket("ws://" + location.hostname + ":81");
          socket.onopen  = () => wsStatus.classList.add('connected');
          socket.onclose = () => {
            wsStatus.classList.remove('connected');
            setTimeout(connectWebSocket, 1000);
          };
          socket.onerror = err => console.error("[WS] Error:", err);

          socket.onmessage = event => {
            if (event.data.startsWith("redirect:")) {
              window.location.href = event.data.split("redirect:")[1];
              return;
            }
            try {
              const data = JSON.parse(event.data);
              if ('time' in data && 'value' in data) {
                // Modo gráfica: actualiza barra fija por índice
                const bars = document.getElementById('barContainer').children;
                const idx  = data.time;
                if (bars[idx]) {
                  bars[idx].style.height = (data.value * 1) + 'px';
                }
                document.getElementById('totalCounter')
                        .textContent = "Total: " + data.total;
              } else if ('total' in data) {
                // Modo conteo: solo actualiza contador
                document.getElementById('totalCounter')
                        .textContent = "Total: " + data.total;
              }
            } catch(e) {
              console.error("WS JSON inválido:", e);
            }
          };
        }

        function sendMode(modo) {
          if (socket && socket.readyState === WebSocket.OPEN) {
            socket.send(modo);
          }
          btnCount.classList.toggle('active', modo === '1');
          btnGraph.classList.toggle('active', modo === '2');
          const bc = document.getElementById('barContainer');
          if (modo === '2') {
            // Si no hay barras (p.e. tras Reset) las pre-creamos
            if (bc.children.length === 0) window._preCreateBars();
            bc.style.display = 'flex';
          } else {
            bc.style.display = 'none';
          }
        }

        function sendMenu() {
          sendMode('m');
          const bc = document.getElementById('barContainer');
          bc.style.display = 'none';
          bc.innerHTML = '';
          // regenerar barras para la próxima vez
          window._preCreateBars && window._preCreateBars();
          document.getElementById('totalCounter')
                  .textContent = "Total: 0";
          btnCount.classList.remove('active');
          btnGraph.classList.remove('active');
        }

        window.addEventListener('load', () => {
          window._preCreateBars && window._preCreateBars();
          connectWebSocket();
        });
      </script>
    </body>
    </html>
)rawliteral";
}

// Muestra una página de espera mientras se recopilan los datos de la gráfica
void WebServerManager::handleWaitingPage() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Recopilando datos...</title>
        <meta http-equiv="refresh" content="61;url=/" />
        <style>
            body { font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; padding-top: 100px; }
            h1 { color: #2c3e50; }
        </style>
    </head>
    <body>
        <h1>Recopilando datos de producción...</h1>
        <p>Por favor espera 60 segundos</p>
        <p>La gráfica se mostrará automáticamente</p>
    </body>
    </html>
    )rawliteral";

    server.send(200, "text/html", html);
}

// Maneja la ruta raíz: redirige a /wait si la gráfica aún no está lista
void WebServerManager::handleRoot() {
    unsigned long elapsed = (millis() - graphStartTime);

    // Si aún no se han recopilado los 60s de datos Y la gráfica no está activa
    if (elapsed < 60000 && !graphRunning) {
        server.sendHeader("Location", "/wait", true);
        server.send(302, "text/plain", "");
        return;
    }

    // Si ya terminó la gráfica o está en curso y puede mostrarse
    String html;
    generateGraphHTML(html);
    server.send(200, "text/html; charset=utf-8", html);
}

// Maneja rutas no encontradas (404)
void WebServerManager::handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: " + server.uri() + "\n";
    message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
    message += "Arguments: " + String(server.args()) + "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

// Devuelve el estado actual del modo (menu, conteo, grafica, desconocido)
void WebServerManager::handleModeStatus() {
    String mode;
    if (!modeSelected) {
        mode = "menu";
    } else if (selectedMode == '1') {
        mode = "conteo";
    } else if (selectedMode == '2') {
        mode = "grafica";
    } else {
        mode = "desconocido";
    }

    server.send(200, "text/plain", mode);
}

// Devuelve los datos de la gráfica en formato JSON
void WebServerManager::handleGraphData() {
    String json = "{\"values\":[";
    for (int i = 0; i < 60; i++) {
        if (i > 0) json += ",";
        json += String(graphData[i]);
    }
    json += "],\"total\":";
    json += sensorManager.getObjectCount();
    json += "}";
    server.send(200, "application/json", json);
}


