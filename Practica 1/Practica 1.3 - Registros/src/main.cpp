#include <Arduino.h>

// Pin donde está conectado el NeoPixel
#define LED_PIN 48

// Punteros a los registros GPIO
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;
uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;

void setup() {
  // Inicializar la comunicación serial
  Serial.begin(115200);
  // Configurar el pin 48 como salida directamente en el registro
  *gpio_enable |= (1 << LED_PIN);

  // Configurar el pin como salida usando el registro GPIO_ENABLE_REG
  uint32_t *gpio_enable = (uint32_t *)GPIO_ENABLE_REG;
  // Configurar el pin como salida
  *gpio_enable |= (1 << LED_PIN);  
}

void loop() {
  // Puntero al registro de salida GPIO
  uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;

  // Activar el pin (encender el LED)
  *gpio_out |= (1 << LED_PIN);
  // Envía "ON" al puerto serie
  Serial.println("ON"); 
  // Espera 1 segundo
  delay(1000);  

  // Desactivar el pin (apagar el LED)
  *gpio_out &= ~(1 << LED_PIN);
  // Envía "OFF" al puerto serie
  Serial.println("OFF");
  // Espera 1 segundo
  delay(1000); 
}
 
 