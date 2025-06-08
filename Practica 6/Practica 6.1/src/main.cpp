#include <SPI.h>
#include <SD.h>

#define SS_PIN 10   
#define RST_PIN 9   
#define SCK_PIN 12 
#define MISO_PIN 13 
#define MOSI_PIN 11 

File myFile;

void setup() {
  Serial.begin(115200);
  Serial.print("Iniciando SD ...");

  if (!SD.begin(SS_PIN)) {
    Serial.println("No se pudo inicializar");
    return;  // Sale de setup si la inicialización falla
  }

  Serial.println("Inicialización exitosa");

  myFile = SD.open("archivo.txt"); // Abrimos el archivo
  if (myFile) {
    Serial.println("archivo.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();  // Cerramos el archivo después de leerlo
  } else {
    Serial.println("Error al abrir el archivo");
  }
}

void loop() {
  // El loop queda vacío si no hay más tareas
}
