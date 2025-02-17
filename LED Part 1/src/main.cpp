#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#define LED_PIN     48   // Cambia al pin donde conectaste el NeoPixel
#define NUM_LEDS    1   // Si solo tienes un LED NeoPixel
#define DELAY_TIME  500 // Tiempo en milisegundos


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
 Serial.begin(115200); 
 strip.begin();
 strip.show();  // Apaga todos los LEDs al inicio
 Serial.println("ESP32-S3 iniciado, NeoPixel parpadeando en blanco...");
}


void loop() {
 // Enciende el LED en blanco (RGB: 255,255,255)
 strip.setPixelColor(0, strip.Color(255,255,255)); 
 strip.show();
 Serial.println("NeoPixel ENCENDIDO");
 delay(DELAY_TIME);


 // Apaga el LED
 strip.setPixelColor(0, strip.Color(0, 0, 0)); 
 strip.show();
 Serial.println("NeoPixel APAGADO");
 delay(DELAY_TIME);
}
