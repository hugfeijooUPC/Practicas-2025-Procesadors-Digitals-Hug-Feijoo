#include <Arduino.h>

// ## Configuración inicial
void setup() {
  // Iniciar la comunicación por el puerto serie a 115200 baudios
  Serial.begin(115200);

  // Esperar 1 segundo para que el puerto serie se estabilice
  delay(1000);
}

// ## Ciclo principal
void loop() {
  // Leer el valor de la temperatura interna (en grados Celsius)
  float temperature = temperatureRead();

  // Enviar el valor de la temperatura por el puerto serie
  Serial.print("Temperatura interna: ");
  Serial.print(temperature);  // Imprime el valor de la temperatura
  Serial.println(" °C");  // Imprime la unidad de temperatura (°C)

  // Esperar 1 segundo antes de leer la temperatura nuevamente
  delay(1000);  // Pausa de 1000 ms (1 segundo)
}

