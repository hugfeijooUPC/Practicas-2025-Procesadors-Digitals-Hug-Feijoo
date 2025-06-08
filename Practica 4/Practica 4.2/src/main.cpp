#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // Incluir la librería NeoPixel

// Definir el pin del NeoPixel
#define NEOPIXEL_PIN 48

// Definir el número de LEDs en la tira (en este caso, solo 1)
#define NUM_LEDS 1

// Declarar el objeto NeoPixel
Adafruit_NeoPixel pixels(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Declarar el semáforo
SemaphoreHandle_t xSemaphore = NULL;

// Declarar las funciones de las tareas
void taskEncenderLED(void *parameter);
void taskApagarLED(void *parameter);

void setup()
{
  Serial.begin(112500);

  // Inicializar el NeoPixel
  pixels.begin();
  pixels.show(); // Inicializar todos los LEDs apagados

  // Crear el semáforo binario
  xSemaphore = xSemaphoreCreateBinary();

  // Verificar si el semáforo se creó correctamente
  if (xSemaphore == NULL) {
    Serial.println("Error al crear el semáforo");
    while (1); // Detener la ejecución si no se pudo crear el semáforo
  }

  // Dar el semáforo inicialmente (el LED está apagado)
  xSemaphoreGive(xSemaphore);

  // Crear la tarea para encender el LED
  xTaskCreate(
    taskEncenderLED,   // Función de la tarea
    "Encender LED",    // Nombre de la tarea
    10000,            // Tamaño de la pila
    NULL,             // Parámetro de la tarea
    1,                // Prioridad de la tarea
    NULL              // Identificador de la tarea
  );

  // Crear la tarea para apagar el LED
  xTaskCreate(
    taskApagarLED,     // Función de la tarea
    "Apagar LED",      // Nombre de la tarea
    10000,            // Tamaño de la pila
    NULL,             // Parámetro de la tarea
    1,                // Prioridad de la tarea
    NULL              // Identificador de la tarea
  );
}

void loop()
{
  // No es necesario hacer nada en el loop
}

// Tarea para encender el LED
void taskEncenderLED(void *parameter)
{
  for (;;) // Bucle infinito
  {
    // Tomar el semáforo (esperar si no está disponible)
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Encender el NeoPixel en color rojo (puedes cambiar el color)
      pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Rojo
      pixels.show(); // Actualizar el NeoPixel
      Serial.println("LED encendido (Rojo)");
      delay(1000); // Esperar 1 segundo
      xSemaphoreGive(xSemaphore); // Liberar el semáforo
    }
    delay(100); // Pequeña pausa para evitar uso excesivo de la CPU
  }
}

// Tarea para apagar el LED
void taskApagarLED(void *parameter)
{
  for (;;) // Bucle infinito
  {
    // Tomar el semáforo (esperar si no está disponible)
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Apagar el NeoPixel
      pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Apagar (color negro)
      pixels.show(); // Actualizar el NeoPixel
      Serial.println("LED apagado");
      delay(1000); // Esperar 1 segundo
      xSemaphoreGive(xSemaphore); // Liberar el semáforo
    }
    delay(100); // Pequeña pausa para evitar uso excesivo de la CPU
  }
}