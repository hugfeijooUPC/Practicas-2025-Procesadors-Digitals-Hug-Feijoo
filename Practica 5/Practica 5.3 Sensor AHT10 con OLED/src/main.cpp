#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>

// Definir los pines I2C personalizados
#define SDA_PIN 6  // Nuevo pin SDA
#define SCL_PIN 7  // Nuevo pin SCL

// Definir las dimensiones de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Dirección I2C de la pantalla OLED
#define SCREEN_ADDRESS 0x3C

// Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_RESET -1

// Declarar el objeto display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declarar el objeto sensor AHT10
Adafruit_AHTX0 aht;

void setup() {
  // Iniciar la comunicación serial
  Serial.begin(115200);

  // Configurar los pines I2C personalizados
  Wire.begin(SDA_PIN, SCL_PIN);

  // Iniciar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error al inicializar la pantalla OLED"));
    for (;;); // Detener la ejecución si la pantalla no se inicializa
  }

  // Iniciar el sensor AHT10
  if (!aht.begin(&Wire)) {
    Serial.println(F("Error al inicializar el sensor AHT10"));
    for (;;); // Detener la ejecución si el sensor no se inicializa
  }

  // Mostrar mensaje de inicio en la pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Iniciando..."));
  display.display();
  delay(2000); // Esperar 2 segundos
}

void loop() {
  // Leer los datos del sensor AHT10
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Obtener temperatura y humedad

  // Mostrar los datos en el monitor serial
  Serial.print(F("Temperatura: "));
  Serial.print(temp.temperature);
  Serial.print(F(" °C, Humedad: "));
  Serial.print(humidity.relative_humidity);
  Serial.println(F(" %"));

  // Mostrar los datos en la pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Mostrar temperatura
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(temp.temperature);
  display.print(F(" °C"));

  // Mostrar humedad
  display.setCursor(0, 16);
  display.print(F("Humedad: "));
  display.print(humidity.relative_humidity);
  display.print(F(" %"));

  // Actualizar la pantalla
  display.display();

  // Esperar 2 segundos antes de la siguiente lectura
  delay(2000);
}
