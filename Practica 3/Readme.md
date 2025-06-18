
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
```

Primero se incluyen las librerías necesarias:
- `WiFi.h` permite conectar el ESP32 a redes WiFi.
- `WebServer.h` nos permite crear un servidor web básico que responderá a peticiones HTTP.

```cpp
const char* ssid = "Livebox6-BEB1";
const char* password = "6VDPndX7fsnt";
```
Aquí definimos las credenciales de la red WiFi a la que queremos que se conecte el ESP32. Estas se usarán más adelante para iniciar la conexión.

```cpp
WebServer server(80); // Puerto HTTP
void handle_root();
```

Creamos una instancia del servidor web que escuchará en el puerto 80 (el estándar para HTTP). También declaramos una función llamada `handle_root()` que se encargará de responder cuando el usuario acceda a la página principal.

```cpp
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
```

En la función `setup()`:
- Iniciamos la comunicación por puerto serie a 115200 baudios para poder ver mensajes de depuración.
- Llamamos a `WiFi.begin()` pasando el `ssid` y `password` para iniciar la conexión a la red.
- Con un bucle `while`, esperamos a que el estado del WiFi sea `WL_CONNECTED`, indicando que ya estamos conectados.
- Una vez conectado, imprimimos la dirección IP asignada al ESP32.
- Configuramos el servidor para que cuando alguien acceda a la ruta `/`, se llame a `handle_root()`.
- Finalmente, iniciamos el servidor con `server.begin()`.

```cpp
void loop() {
  server.handleClient();
}
```

En el `loop()`, simplemente se llama a `server.handleClient()`, que se encarga de gestionar las peticiones entrantes al servidor. Esta función debe llamarse constantemente para que el servidor pueda atender a los clientes.

```cpp
void handle_root() {
  server.send(200, "text/html", HTML);
}
```

Esta función es la encargada de responder a las peticiones a la raíz del servidor (`/`). Envia una respuesta HTTP con código 200 (OK) y contenido del tipo "text/html", que es el HTML de la página web que hemos definido.

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


## Conclusión

Con esta práctica se logra:

- Comprender el uso básico de WiFi en modo estación con ESP32.
- Generar y personalizar páginas web con HTML y CSS embebidos.
- Establecer comunicación serie bidireccional por Bluetooth.
- Aplicar el ESP32 como interfaz de red y comunicación inalámbrica.
