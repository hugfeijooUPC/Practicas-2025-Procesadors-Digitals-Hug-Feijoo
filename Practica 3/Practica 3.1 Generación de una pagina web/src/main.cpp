#include <WiFi.h>
#include <WebServer.h>

// Identificación del WiFi al que nos queremos conectar (SSID:Service Set IDentifier)
const char* ssid = "Livebox6-BEB1"; 
const char* password = "6VDPndX7fsnt"; 

// Object of WebServer(HTTP port, 80 is defult)
WebServer server(80); 

// Declaración de handle_root
void handle_root();

void setup() {

Serial.begin(115200);
Serial.println("Intentando conectar a ");
Serial.println(ssid);

// Connexión al modem WiFi
WiFi.begin(ssid, password);

// Comprovación de la existencia de la red WiFi
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}

Serial.println("");
Serial.println("Connexión WiFi establecida correctamente");
Serial.print("Redirigiendo a la IP: ");

//Obtener la dirección IP local asignada a ESP32
Serial.println(WiFi.localIP());

//Configurar un servidor web 
//Cuando un cliente accede a la dirección IP del dispositivo sin una ruta adicional, se activa handle_root().
server.on("/", handle_root);
server.begin();
Serial.println("HTTP server started");
delay(100);
}

void loop() {
//atender las solicitudes HTTP entrantes:
server.handleClient();
}

// HTML y CSS
String HTML = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Página Estética</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #4e54c8, #8f94fb);
            color: #fff;
            text-align: center;
        }
        .container {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        h1 {
            font-size: 3rem;
            margin-bottom: 10px;
        }
        p {
            font-size: 1.2rem;
            max-width: 600px;
        }
        .btn {
            display: inline-block;
            margin-top: 20px;
            padding: 10px 20px;
            font-size: 1.2rem;
            color: #fff;
            background: #000000;
            border: none;
            border-radius: 25px;
            cursor: pointer;
            transition: 0.3s;
        }
        .btn:hover {
            background: #e55039;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Bienvenido a mi Página Web</h1>
        <p>Soy Hug Feijoo, aquí puedes ver más de mis proyectos</p>
        <a href="https://www.sayinghello.es/">
        <button class="btn">Explorar</button>
        </a>
    </div>
</body>
</html>
)rawliteral";

// Función para atender peticiones (/)
void handle_root() {
//Enviar una respuesta HTTP desde el servidor web
server.send(200, "text/html", HTML);
}

