# Práctica 8: Buses de Comunicación IV - UART

## Introducción

La comunicación UART (Universal Asynchronous Receiver-Transmitter) es una forma de transmisión **asíncrona**, ampliamente utilizada para comunicar microcontroladores con periféricos como módulos GPS, GSM o simplemente entre microcontroladores.

En esta práctica se trabaja principalmente con **UART2** del ESP32, la cual se configura manualmente para hacer un puente de comunicación con el terminal serie (UART0). Esta práctica es fundamental para entender la base de toda comunicación serial realizada en los microcontroladores.

---

## Ejercicio 1: Puente UART0 ↔ UART2

### Objetivo

Desarrollar un puente de comunicación entre UART0 (USB/PC) y UART2 (GPIO16 y GPIO17) sin hardware adicional.

### Código utilizado (`8.1.cpp`)

```cpp
#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("== PUENTE UART0 <--> UART2 ACTIVADO ==");
  Serial.println("Escribe algo en el terminal. Se enviará a UART2 y se mostrará lo recibido.");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial2.write(c);
    Serial.print("[TX UART2] ");
    Serial.println(c);
  }

  if (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
    Serial.print("[RX UART2] ");
    Serial.println(c);
  }
}
```

### Salida esperada en consola

```
== PUENTE UART0 <--> UART2 ACTIVADO ==
Escribe algo en el terminal. Se enviará a UART2 y se mostrará lo recibido.
[TX UART2] H
[RX UART2] H
[TX UART2] o
[RX UART2] o
```

> Si conectamos físicamente TXD2 a RXD2 (GPIO17 a GPIO16), cualquier carácter enviado se devuelve, funcionando como un bucle.

---

## Explicación del funcionamiento

- `Serial.begin()` inicializa UART0 a 115200 baudios (comunicación con el PC).
- `Serial2.begin()` configura UART2 usando los pines GPIO16 (RX) y GPIO17 (TX).
- En el `loop()`, cualquier byte recibido por el terminal se reenvía a UART2.
- Si UART2 recibe un byte (por ejemplo, mediante loopback o un módulo GPS conectado), lo devuelve al terminal por UART0.
- Se imprimen etiquetas para distinguir TX y RX en la consola del monitor serie.

---

## Conclusión

Este ejercicio permite:

- Comprender la configuración básica de los puertos UART en el ESP32.
- Aplicar redirección de datos entre terminal y periféricos externos.
- Preparar la base para trabajar con módulos como GPS o GSM en UART2.

---

## Recomendaciones

- Para pruebas de bucle, conectar físicamente GPIO17 (TXD2) a GPIO16 (RXD2).
- Para pruebas reales, conectar un módulo GPS o sensor serial al UART2.
- Asegurar que la velocidad, bit de parada y paridad coincidan entre emisor y receptor.

---

## Próximos pasos

Puedes extender esta práctica con:

- Un **módulo GPS** (opcional: ejercicio 2 del enunciado).
- Un **módem GSM** para conectividad a internet (opcional: ejercicio 3).

