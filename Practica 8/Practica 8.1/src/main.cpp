#include <Arduino.h>

// UART2 asignado a GPIO16 (RXD2) y GPIO17 (TXD2)
#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200); // UART0 = TXD0 (GPIO1), RXD0 (GPIO3) = comunicación USB <-> PC
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // UART2 = TXD2 (GPIO17), RXD2 (GPIO16)

  Serial.println("== PUENTE UART0 <--> UART2 ACTIVADO ==");
  Serial.println("Escribe algo en el terminal. Se enviará a UART2 y se mostrará lo recibido.");
}

void loop() {
  // UART0 (desde el PC) --> UART2
  if (Serial.available()) {
    char c = Serial.read();
    Serial2.write(c);  // TXD2
    Serial.print("[TX UART2] ");
    Serial.println(c);
  }

  // UART2 --> UART0 (hacia el terminal del PC)
  if (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);   // TXD0
    Serial.print("[RX UART2] ");
    Serial.println(c);
  }
}
