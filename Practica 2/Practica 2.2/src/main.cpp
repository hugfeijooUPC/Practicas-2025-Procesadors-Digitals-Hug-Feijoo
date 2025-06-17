#include <Arduino.h>                   // Incluye la librería principal de Arduino
#include <Adafruit_NeoPixel.h>         // Incluye la librería para controlar NeoPixels

// Configuración del NeoPixel
#define LED_PIN 48                     // Pin donde está conectado el NeoPixel
#define NUMPIXELS 1                    // Número de LEDs en la tira (en este caso, 1 LED)
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800); // Crea el objeto para controlar el NeoPixel

// Configuración de los pulsadores
#define BUTTON_UP_PIN 18               // Pin para aumentar la frecuencia
#define BUTTON_DOWN_PIN 19             // Pin para disminuir la frecuencia

// Variables para el control de la frecuencia de parpadeo
volatile uint32_t blinkInterval = 500; // Intervalo inicial de parpadeo (500 ms)
volatile bool updateBlink = false;     // Bandera para indicar que se debe actualizar el parpadeo

// Variables para el filtrado de rebotes
volatile bool buttonUpPressed = false;     // Estado del botón de subir frecuencia
volatile bool buttonDownPressed = false;   // Estado del botón de bajar frecuencia
uint32_t lastDebounceTime = 0;             // Último tiempo de rebote registrado
const uint32_t debounceDelay = 50;         // Tiempo de filtrado de rebotes (50 ms)

// Configuración del timer
hw_timer_t *timer = NULL;                  // Puntero al temporizador hardware
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // Mutex para acceso seguro en interrupciones

// Función de servicio de interrupción (ISR) para el timer
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);       // Entra en sección crítica (protege variables compartidas)

  // Leer el pulsador de aumentar frecuencia con filtrado de rebotes
  if (digitalRead(BUTTON_UP_PIN) == LOW) {
    if (!buttonUpPressed && (millis() - lastDebounceTime) > debounceDelay) {
      buttonUpPressed = true;
      lastDebounceTime = millis();
      if (blinkInterval > 50) {            // Limita la frecuencia mínima
        blinkInterval = blinkInterval/2;   // Aumenta la frecuencia (reduce el intervalo)
        updateBlink = true;                // Marca que hay que actualizar el parpadeo
      }
    }
  } else {
    buttonUpPressed = false;
  }

  // Leer el pulsador de disminuir frecuencia con filtrado de rebotes
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    if (!buttonDownPressed && (millis() - lastDebounceTime) > debounceDelay) {
      buttonDownPressed = true;
      lastDebounceTime = millis();
      if (blinkInterval < 10000) {         // Limita la frecuencia máxima
        blinkInterval = blinkInterval *2;  // Disminuye la frecuencia (aumenta el intervalo)
        updateBlink = true;                // Marca que hay que actualizar el parpadeo
      }
    }
  } else {
    buttonDownPressed = false;
  }

  portEXIT_CRITICAL_ISR(&timerMux);        // Sale de la sección crítica
}

void setup() {
  Serial.begin(115200);                    // Inicializa el puerto serie a 115200 baudios

  // Configura los pines de los pulsadores como entradas con pull-up
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  // Inicializa el NeoPixel
  strip.begin();
  strip.show();                            // Apaga todos los LEDs al inicio

  // Configura el timer para leer los pulsadores periódicamente
  timer = timerBegin(0, 80, true);         // Temporizador 0, preescalador de 80, cuenta ascendente
  timerAttachInterrupt(timer, &onTimer, true); // Asocia la ISR al temporizador
  timerAlarmWrite(timer, 10000, true);     // Configura la alarma para 10,000 microsegundos (10 ms)
  timerAlarmEnable(timer);                 // Habilita la alarma del temporizador
}

void loop() {
  static uint32_t lastBlinkTime = 0;       // Guarda el último tiempo de parpadeo
  static bool ledState = false;            // Estado actual del LED

  // Controla el parpadeo del NeoPixel según el intervalo actual
  if (millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    ledState = !ledState;                  // Cambia el estado del LED

    if (ledState) {
      strip.setPixelColor(0, strip.Color(255, 255, 255)); // Enciende el LED en blanco
    } else {
      strip.setPixelColor(0, strip.Color(0, 0, 0));       // Apaga el LED
    }
    strip.show();                          // Actualiza el estado del LED
  }

  // Si se ha cambiado la frecuencia, muestra el nuevo intervalo por el puerto serie
  if (updateBlink) {
    updateBlink = false;
    Serial.printf("Nuevo intervalo de parpadeo: %u ms\n", blinkInterval);
  }
}
