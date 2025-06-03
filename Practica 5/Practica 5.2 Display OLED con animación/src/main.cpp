#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define el pin SDA para la comunicación I2C
#define SDA_PIN 6

// Define el pin SCL para la comunicación I2C
#define SCL_PIN 7

// Define el ancho de la pantalla OLED en píxeles
#define SCREEN_WIDTH 128

// Define el alto de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 32

// Define la dirección I2C de la pantalla OLED
#define SCREEN_ADDRESS 0x3C

// Define el pin de reset de la pantalla OLED (-1 si no se usa)
#define OLED_RESET -1

// Crea el objeto display para controlar la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Función personalizada para escribir texto en la pantalla OLED
void escribirTexto(const char* texto, uint8_t tamanoTexto, uint16_t color, int16_t x, int16_t y) {
  display.clearDisplay();               // Limpia la pantalla antes de escribir
  display.setTextSize(tamanoTexto);     // Establece el tamaño del texto
  display.setTextColor(color);          // Establece el color del texto
  display.setCursor(x, y);              // Establece la posición del cursor
  display.print(texto);                 // Escribe el texto en el buffer
  display.display();                    // Actualiza la pantalla para mostrar el texto
}

// Función de inicialización que se ejecuta una vez al arrancar
void setup() {
  // Inicia la comunicación serie a 115200 baudios
  Serial.begin(115200);

  // Inicializa la comunicación I2C con los pines definidos
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inicializa la pantalla OLED y verifica si la inicialización fue exitosa
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed")); // Mensaje de error si falla
    for(;;); // Detiene la ejecución si la pantalla no se inicializa
  }
}

// Función principal que se ejecuta en bucle
void loop() {
  // Bucle para animar puntos que se mueven de izquierda a derecha
  for (int offset = 0; offset <= SCREEN_WIDTH; offset++) {
    display.clearDisplay(); // Limpia la pantalla en cada iteración

    // Dibuja una fila de 10 puntos, desplazados horizontalmente por 'offset'
    for (int i = 0; i < 10; i++) {
      int x = (i * 12) + offset; // Calcula la posición horizontal de cada punto
      if (x < SCREEN_WIDTH) {    // Verifica que el punto esté dentro de la pantalla
        display.drawPixel(x, 16, SSD1306_WHITE); // Dibuja el punto en la posición calculada
      }
    }

    display.display(); // Actualiza la pantalla para mostrar los puntos
    delay(50);         // Espera 50 ms para controlar la velocidad de la animación
  }
}