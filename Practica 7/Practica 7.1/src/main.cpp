#include "AudioGeneratorAAC.h"         // Biblioteca para decodificar AAC
#include "AudioOutputI2S.h"           // Salida de audio I2S
#include "AudioFileSourcePROGMEM.h"   // Fuente de audio desde memoria flash
#include "sampleaac.h"                // Archivo AAC en formato binario

// Objetos para el pipeline de audio
AudioFileSourcePROGMEM *in;           // Fuente de audio (datos en flash)
AudioGeneratorAAC *aac;               // Decodificador AAC
AudioOutputI2S *out;                  // Salida I2S

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Iniciando reproducción AAC...");

    // Verificación inicial del archivo AAC
    Serial.print("Primeros bytes: ");
    for (int i = 0; i < 10; i++) {
        Serial.printf("%02X ", sampleaac[i]);
    }
    Serial.println();

    // Configuración del pipeline de audio
    in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
    out = new AudioOutputI2S();
    out->SetGain(0.8);                // Ajuste de volumen (0.0 a 1.0)
    out->SetPinout(5, 4, 6);          // Configura pines I2S (BCLK, LRC, DOUT)
    
    aac = new AudioGeneratorAAC();

    // Inicia la reproducción
    if (aac->begin(in, out)) {
        Serial.println("Decodificador AAC iniciado correctamente");
    } else {
        Serial.println("Error al iniciar el decodificador AAC");
    }
}

void loop() {
    if (aac->isRunning()) {
        if (!aac->loop()) {
            // Cuando termina la reproducción
            Serial.println("Reproducción completada");
            aac->stop();
            
            // Opcional: Reiniciar automáticamente
            // aac->begin(in, out);
        }
    } else {
        delay(1000);  // Pequeña pausa si no se está reproduciendo
    }
}