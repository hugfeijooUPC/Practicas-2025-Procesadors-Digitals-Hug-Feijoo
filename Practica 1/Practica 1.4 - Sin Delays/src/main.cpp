#include <Arduino.h>

// Pin de salida configurable
#define LED_PIN 2  

// Punteros a los registros GPIO
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;
uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;

// Variables para el control del tiempo
unsigned long previousMillis = 0;
const long interval = 1000; // Intervalo de 1 segundo
bool ledState = false;

void setup() {
  Serial.begin(115200);
  *gpio_enable |= (1 << LED_PIN);  // Configurar el pin como salida
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState; // Alternar el estado del LED
    
    if (ledState) {
      *gpio_out |= (1 << LED_PIN);
      Serial.println("ON");
    } else {
      *gpio_out &= ~(1 << LED_PIN);
      Serial.println("OFF");
    }
  }
}
 