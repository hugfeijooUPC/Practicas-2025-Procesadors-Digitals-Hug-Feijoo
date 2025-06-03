#include <Arduino.h>
// Incluye la biblioteca NeoPixel
#include <Adafruit_NeoPixel.h> 

// Pin donde está conectado el NeoPixel
#define LED_PIN 48             

// Número de NeoPixels
#define NUM_PIXELS 1           

// Crea un objeto NeoPixel en el PIN 48
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 // Inicialización del objeto NeoPixel
 pixels.begin();
 // Inicialización del puerto serie
 Serial.begin(115200); 
}

void loop() {
 // Asignación del color Blanco en codigo RGB para el Led NeoPixel
 pixels.setPixelColor(0, pixels.Color(255, 255, 255));
 // Actualización física del NeoPixel para que muestre el color asignado
 pixels.show(); 
 // Envía "ON" al puerto serie
 Serial.println("ON"); 
 // Espera 1 segundo
 delay(1000); 
 // Asignación del color Negro en codigo RGB simulando el Apagado
 pixels.setPixelColor(0, pixels.Color(0, 0, 0));
 // Actualización física del NeoPixel para que muestre el color asignado
 pixels.show();
 // Envía "OFF" al puerto serie
 Serial.println("OFF"); 
  // Espera 1 segundo
 delay(1000); 
}


 