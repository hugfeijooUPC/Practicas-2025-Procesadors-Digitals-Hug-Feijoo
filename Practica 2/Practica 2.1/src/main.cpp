#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48         // Pin del NeoPixel
#define BUTTON_PIN 18      // Pin del botón
#define NUMPIXELS 1        // Número de NeoPixels

volatile uint32_t buttonPressCount = 0;  // Contador de pulsaciones
volatile bool buttonPressed = false;      // Estado del botón

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);  // Inicializar el strip NeoPixel

// Función de interrupción del botón
void IRAM_ATTR isr() {
  buttonPressCount++;  // Incrementar el contador por cada pulsación
  buttonPressed = true;  // Marcar que el botón ha sido presionado
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configurar el pin del botón

  // Inicializar el NeoPixel
  strip.begin();
  strip.show(); // Asegúrate de que el LED está apagado al inicio

  // Configurar la interrupción en el botón
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isr, FALLING); // Interrupción en el flanco de bajada
}

void loop() {
  if (buttonPressed) {
    Serial.printf("Button pressed %u times\n", buttonPressCount);
    buttonPressed = false;  // Limpiar la bandera

    // Encender el NeoPixel en blanco (255, 255, 255)
    strip.setPixelColor(0, strip.Color(255, 255, 255)); // Blanco
    strip.show(); // Actualizar el color del NeoPixel

    delay(100);  // Mantener el LED encendido por 500ms

    // Apagar el NeoPixel
    strip.setPixelColor(0, strip.Color(0, 0, 0)); // Apagar el LED
    strip.show(); // Actualizar el color del NeoPixel
  }
}
