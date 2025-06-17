
# Práctica 3: WiFi y Bluetooth con ESP32

## Introducción

El objetivo de esta práctica es comprender el funcionamiento de **WiFi** y **Bluetooth** con el microcontrolador ESP32. Se realizan dos partes:

- Generación de una página web desde el ESP32 usando WiFi en modo estación.
- Comunicación serie con un móvil utilizando Bluetooth clásico.

---

## Parte A: Servidor Web con ESP32 (WiFi STA)

### Objetivo

Crear un servidor web accesible desde cualquier dispositivo conectado a la misma red WiFi, que muestre una página HTML personalizada.

### Código utilizado

El código conecta el ESP32 a una red WiFi (modo estación), inicia un servidor web en el puerto 80 y responde con una página HTML:

```cpp
#include <WiFi.h>
#include <WebServer.h>

// SSID y contraseña de la red WiFi
const char* ssid = "Livebox6-BEB1";
const char* password = "6VDPndX7fsnt";

WebServer server(80); // Puerto HTTP

void handle_root();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado con éxito");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

String HTML = R"rawliteral(
// Se omite por brevedad. Ver el archivo completo en el repositorio.
)rawliteral";

void handle_root() {
  server.send(200, "text/html", HTML);
}
```

### HTML incrustado

Este HTML está embebido dentro del código y presenta una página con diseño moderno. Incluye botones y un enlace:

```html
<h1>Bienvenido a mi Página Web</h1>
<p>Soy Hug Feijoo, aquí puedes ver más de mis proyectos</p>
<a href="https://www.sayinghello.es/"><button>Explorar</button></a>
```

### Funcionamiento y salida esperada

- Al subir el código, el monitor serie muestra:
  ```
  WiFi conectado con éxito
  Redirigiendo a la IP: 192.168.X.X
  HTTP server started
  ```

- Desde un navegador conectado a la misma red, accediendo a la IP se visualiza la página web servida desde el ESP32.

---

## Parte B: Comunicación Bluetooth Serial

### Objetivo

Permitir la comunicación bidireccional entre el ESP32 y un teléfono móvil mediante Bluetooth clásico, enviando y recibiendo datos como si fuera un puerto serie.

### Código utilizado

```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
```

### Funcionamiento

- El ESP32 inicia con nombre Bluetooth `ESP32test`.
- Se puede emparejar desde el móvil y usar una app como **Serial Bluetooth Terminal** para enviar/recibir datos.
- Cualquier carácter enviado desde el terminal del PC aparece en el móvil, y viceversa.

### Salida esperada

```
The device started, now you can pair it with bluetooth!
```

---

## Video demostrativo


---

## Ejercicios propuestos para mejora de nota

1. Implementar el servidor WiFi en modo **AP** (Access Point).
2. Probar la funcionalidad **BLE** con el siguiente tutorial:  
   [https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/)

---

## Conclusión

Con esta práctica se logra:

- Comprender el uso básico de WiFi en modo estación con ESP32.
- Generar y personalizar páginas web con HTML y CSS embebidos.
- Establecer comunicación serie bidireccional por Bluetooth.
- Aplicar el ESP32 como interfaz de red y comunicación inalámbrica.
