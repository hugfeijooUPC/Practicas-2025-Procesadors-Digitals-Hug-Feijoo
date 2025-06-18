# Práctica 4: Sistemas Operativos en Tiempo Real

## Introducción

El objetivo de esta práctica es comprender el funcionamiento de un **sistema operativo en tiempo real (RTOS)** aplicando la funcionalidad de **FreeRTOS** en el microcontrolador ESP32.

En particular, se busca implementar tareas concurrentes que se ejecutan de forma paralela, demostrando cómo el planificador reparte el tiempo de la CPU entre ellas. Se trabaja con dos ejercicios prácticos:

- **Ejercicio 1**: Impresión concurrente por el puerto serie desde dos tareas.
- **Ejercicio 2**: Sincronización entre dos tareas con un semáforo para controlar un LED.

---

## Ejercicio 1: Tareas concurrentes con salida por puerto serie

### Objetivo

Crear dos tareas:
- Una tarea principal en `loop()`.
- Una tarea secundaria (`anotherTask`) creada manualmente con `xTaskCreate`.

Ambas deben imprimir mensajes por el puerto serie simultáneamente.

### Código utilizado

```cpp
#include <Arduino.h>

TaskHandle_t task1Handle = NULL;

void anotherTask(void *parameter);

void setup() {
  Serial.begin(112500);
  xTaskCreate(
    anotherTask,
    "another Task",
    10000,
    NULL,
    1,
    &task1Handle
  );
}

void loop() {
  Serial.println("this is ESP32 Task");
  delay(1000);
}

void anotherTask(void *parameter) {
  for (;;) {
    Serial.println("this is another Task");
    delay(1000);
  }
  vTaskDelete(NULL);
}
```

### Funcionamiento

- Se crea una **tarea adicional** (`anotherTask`) al inicio del `setup()`, con prioridad 1.
- El `loop()` sigue ejecutándose normalmente como la **tarea por defecto de Arduino**.
- Ambas tareas se ejecutan indefinidamente con un `delay(1000)`, alternando su salida por puerto serie.

### Salida esperada

```
this is ESP32 Task
this is another Task
this is ESP32 Task
this is another Task
...
```

Ambos mensajes aparecen intercalados, indicando que las dos tareas están compartiendo el uso de la CPU correctamente bajo el sistema multitarea de FreeRTOS.

### Explicación

- `xTaskCreate()` genera una nueva tarea paralela que se ejecuta junto con la tarea principal.
- El sistema operativo planifica ambas tareas de forma equitativa porque tienen la misma prioridad.
- Gracias a `delay()`, cada tarea cede el uso de CPU durante 1000ms, permitiendo al planificador cambiar de tarea y dar la sensación de ejecución concurrente.

---

## Ejercicio 2: Sincronización de tareas con semáforo (LED)

### Objetivo

Crear dos tareas:
- Una que enciende un LED.
- Otra que lo apaga.

Ambas deben estar sincronizadas mediante un **semáforo binario** para no interferir entre sí.

### Código utilizado

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel pixels(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
SemaphoreHandle_t xSemaphore = NULL;

void taskEncenderLED(void *parameter);
void taskApagarLED(void *parameter);

void setup() {
  Serial.begin(112500);
  pixels.begin();
  pixels.show();

  xSemaphore = xSemaphoreCreateBinary();
  if (xSemaphore == NULL) {
    Serial.println("Error al crear el semáforo");
    while (1);
  }

  xSemaphoreGive(xSemaphore);

  xTaskCreate(taskEncenderLED, "Encender LED", 10000, NULL, 1, NULL);
  xTaskCreate(taskApagarLED,  "Apagar LED",   10000, NULL, 1, NULL);
}

void loop() {
  // No se usa
}
```

```cpp
void taskEncenderLED(void *parameter) {
  for (;;) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Blanco
      pixels.show();
      Serial.println("LED encendido (Blanco)");
      delay(1000);
      xSemaphoreGive(xSemaphore);
    }
    delay(100);
  }
}

void taskApagarLED(void *parameter) {
  for (;;) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Apagar
      pixels.show();
      Serial.println("LED apagado");
      delay(1000);
      xSemaphoreGive(xSemaphore);
    }
    delay(100);
  }
}
```

### Funcionamiento

- Se crea un **semáforo binario** compartido entre las dos tareas.
- Ambas tareas se turnan para acceder al LED, garantizando que no se encienda y apague simultáneamente.
- Cada una toma el semáforo, realiza su tarea (encender o apagar el LED), y luego lo libera.

### Salida esperada

```
LED encendido (Blanco)
LED apagado
LED encendido (Blanco)
LED apagado
...
```

El LED se enciende y apaga en ciclos de 1 segundo, manteniendo una **sincronización precisa** gracias al uso del semáforo.

---

## Conclusión

Con esta práctica se ha logrado:

- Aplicar los conceptos de **multitarea con FreeRTOS** en ESP32.
- Crear tareas paralelas que comparten el tiempo de CPU.
- Sincronizar correctamente tareas usando **semáforos binarios**.
- Demostrar la capacidad del planificador de FreeRTOS para ejecutar múltiples tareas sin conflictos.

Esta práctica sienta las bases para diseñar sistemas embebidos más complejos y eficientes en aplicaciones donde múltiples procesos deben coexistir sin interferencias.

---

## Recomendaciones opcionales

- Incluir **foto del montaje físico del LED** (si se usó uno externo).
- Explorar la fijación de tareas a un núcleo específico usando `xTaskCreatePinnedToCore()`.
