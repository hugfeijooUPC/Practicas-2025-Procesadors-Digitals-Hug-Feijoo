#include "Audio.h" // Incluye la biblioteca para manejar audio.
#include "SD.h"    // Incluye la biblioteca para manejar la tarjeta SD.
#include "FS.h"    // Incluye la biblioteca para manejar el sistema de archivos.

// Digital I/O used for I
#define SPI_MISO 15 // Define el pin MISO para la comunicación SPI.
#define SPI_MOSI 16 // Define el pin MOSI para la comunicación SPI.
#define SPI_SCK 17  // Define el pin SCK para la comunicación SPI.
#define SD_CS 18    // Define el pin CS (Chip Select) para la tarjeta SD.

// Define los pines para la comunicación I2S (protocolo de audio digital).
#define I2S_DOUT 4 // Define el pin de salida de datos I2S.
#define I2S_BCLK 5 // Define el pin de reloj de bits I2S.
#define I2S_LRC 6  // Define el pin de reloj de canal I2S.

Audio audio; // Crea una instancia de la clase `Audio` para manejar la reproducción de audio.

void setup() {
    pinMode(SD_CS, OUTPUT);          // Configura el pin CS de la tarjeta SD como salida.
    digitalWrite(SD_CS, HIGH);       // Establece el pin CS en alto para desactivar la tarjeta SD.
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI); // Inicializa la comunicación SPI con los pines definidos.
    Serial.begin(115200);            // Inicializa la comunicación serie a 115200 baudios.
    SD.begin(SD_CS);                 // Inicializa la tarjeta SD utilizando el pin CS.
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // Configura los pines para la comunicación I2S.
    audio.setVolume(10);             // Establece el volumen del audio (rango: 0 a 21).
    audio.connecttoFS(SD, "Ensoniq-ZR-76-01-Dope-77.wav"); // Conecta el archivo de audio en la tarjeta SD.
}

void loop() {
    audio.loop(); // Llama al método `loop` de la clase `Audio` para mantener la reproducción.
}

// Optional callback functions
void audio_info(const char *info) {
    Serial.print("info: "); // Imprime "info: " en el monitor serie.
    Serial.println(info);   // Imprime información adicional en el monitor serie.
}

void audio_id3data(const char *info) { // ID3 metadata
    Serial.print("id3data: "); // Imprime "id3data: " en el monitor serie.
    Serial.println(info);      // Imprime los datos ID3 (metadatos del archivo MP3).
}

void audio_eof_mp3(const char *info) { // End of file
    Serial.print("eof_mp3: "); // Imprime "eof_mp3: " en el monitor serie.
    Serial.println(info);      // Indica que se alcanzó el final del archivo MP3.
}

void audio_showstation(const char *info) {
    Serial.print("station: "); // Imprime "station: " en el monitor serie.
    Serial.println(info);      // Imprime información sobre la estación (si aplica).
}

void audio_showstreaminfo(const char *info) {
    Serial.print("streaminfo: "); // Imprime "streaminfo: " en el monitor serie.
    Serial.println(info);         // Imprime información sobre el flujo de audio.
}

void audio_showstreamtitle(const char *info) {
    Serial.print("streamtitle: "); // Imprime "streamtitle: " en el monitor serie.
    Serial.println(info);          // Imprime el título del flujo de audio.
}

void audio_bitrate(const char *info) {
    Serial.print("bitrate: "); // Imprime "bitrate: " en el monitor serie.
    Serial.println(info);      // Imprime la tasa de bits del archivo de audio.
}

void audio_commercial(const char *info) { // Duration in seconds
    Serial.print("commercial: "); // Imprime "commercial: " en el monitor serie.
    Serial.println(info);         // Imprime información sobre comerciales (si aplica).
}

void audio_icyurl(const char *info) { // Homepage
    Serial.print("icyurl: "); // Imprime "icyurl: " en el monitor serie.
    Serial.println(info);     // Imprime la URL ICY (si aplica).
}

void audio_lasthost(const char *info) { // Stream URL played
    Serial.print("lasthost: "); // Imprime "lasthost: " en el monitor serie.
    Serial.println(info);       // Imprime la última URL de flujo reproducida.
}

void audio_eof_speech(const char *info) {
    Serial.print("eof_speech: "); // Imprime "eof_speech: " en el monitor serie.
    Serial.println(info);         // Indica que se alcanzó el final de un archivo de discurso.
}