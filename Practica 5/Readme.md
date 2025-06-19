# Práctica 5: Buses de Comunicación I - I2C

## Introducción

El objetivo de esta práctica es comprender el funcionamiento del bus I2C como medio de comunicación entre el ESP32 y periféricos externos. Se explora su arquitectura maestro-esclavo, su funcionamiento síncrono y su simplicidad en el uso de solo dos líneas (SDA y SCL).

En esta práctica se implementan tres ejercicios:

- **Ejercicio 1**: Escaneo del bus I2C.
- **Ejercicio 2**: Control de una pantalla OLED SSD1306.
- **Ejercicio 3**: Lectura de temperatura y humedad usando un sensor AHT10 y visualización en pantalla.

---

## Ejercicio 1: Escáner de dispositivos I2C

### Objetivo

Detectar dispositivos conectados al bus I2C usando pines personalizados (SDA=6, SCL=7) y mostrar sus direcciones en el monitor serie.

### Código utilizado

```cpp
#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 6
#define SCL_PIN 7

void setup() {
    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.begin(115200);
    while (!Serial);
    Serial.println("\nI2C Scanner");
}

void loop() {
    byte error, address;
    int nDevices = 0;
    Serial.println("Scanning...");

    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    delay(2000);
}
```

### Salida esperada

```
Scanning...
I2C device found at address 0x3C
done
```

### Explicación

El programa recorre las 126 direcciones posibles del bus I2C. Por cada dirección, intenta iniciar y cerrar una transmisión:
- Si un dispositivo responde, se considera encontrado.
- Si no, se continúa al siguiente.

Es útil para comprobar conexiones físicas y direcciones de sensores o pantallas.

---

## Ejercicio 2: Pantalla OLED I2C SSD1306

### Objetivo

Mostrar una animación de puntos desplazándose sobre una pantalla OLED I2C de 128x32 píxeles.

### Código utilizado

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 6
#define SCL_PIN 7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
}

void loop() {
  for (int offset = 0; offset <= SCREEN_WIDTH; offset++) {
    display.clearDisplay();
    for (int i = 0; i < 10; i++) {
      int x = (i * 12) + offset;
      if (x < SCREEN_WIDTH) {
        display.drawPixel(x, 16, SSD1306_WHITE);
      }
    }
    display.display();
    delay(50);
  }
}
```

### Funcionamiento

- Inicializa la pantalla OLED por I2C usando los pines personalizados.
- Dentro del bucle `loop()`, se desplazan 10 puntos horizontalmente generando una animación suave.
- Se limpia y actualiza la pantalla en cada ciclo de animación.

---

## Ejercicio 3: Sensor AHT10 + OLED (Lectura y visualización)

### Objetivo

Leer temperatura y humedad del sensor AHT10 y mostrar los valores tanto por pantalla como por el monitor serie.

### Código utilizado

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>

#define SDA_PIN 6
#define SCL_PIN 7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error al inicializar la pantalla OLED"));
    while (1);
  }

  if (!aht.begin(&Wire)) {
    Serial.println(F("Error al inicializar el sensor AHT10"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Iniciando..."));
  display.display();
  delay(2000);
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  Serial.print(F("Temperatura: "));
  Serial.print(temp.temperature);
  Serial.print(F(" °C, Humedad: "));
  Serial.print(humidity.relative_humidity);
  Serial.println(F(" %"));

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(temp.temperature);
  display.print(F(" °C"));
  display.setCursor(0, 16);
  display.print(F("Humedad: "));
  display.print(humidity.relative_humidity);
  display.print(F(" %"));
  display.display();

  delay(2000);
}
```

### Funcionamiento

- Se inicializa la pantalla OLED y el sensor AHT10 en el bus I2C.
- En cada iteración del bucle:
  - Se obtienen los valores de temperatura y humedad.
  - Se muestran por el puerto serie y en pantalla.
- Permite una monitorización básica en tiempo real.

---

## Conclusión

En esta práctica hemos comprendido:

- Cómo escanear dispositivos conectados al bus I2C.
- Cómo trabajar con pantallas OLED mediante la librería `Adafruit_SSD1306`.
- Cómo integrar sensores I2C como el AHT10 para aplicaciones prácticas.
- La simplicidad y potencia del bus I2C para sistemas embebidos.

