#include <Arduino.h>

// Declarar el identificador de la tarea
TaskHandle_t task1Handle = NULL;

// Declarar la función anotherTask
void anotherTask(void *parameter);

void setup()
{
  Serial.begin(112500);

  // Crear la tarea principal
  xTaskCreate(
    anotherTask,       // Función de la tarea
    "another Task",    // Nombre de la tarea
    10000,             // Tamaño de la pila
    NULL,              // Parámetro de la tarea
    1,                 // Prioridad de la tarea
    &task1Handle       // Identificador de la tarea
  );
}

void loop()
{
  Serial.println("this is ESP32 Task");
  delay(1000);
}

// Definición de la función anotherTask
void anotherTask(void *parameter)
{
  // Bucle infinito
  for (;;)
  {
    Serial.println("this is another Task");
    delay(1000);
  }

  // Eliminar la tarea (esto nunca se ejecutará debido al bucle infinito)
  vTaskDelete(NULL);
}