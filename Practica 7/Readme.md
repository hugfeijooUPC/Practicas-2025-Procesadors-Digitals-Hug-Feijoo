# Práctica 7: Buses de Comunicación III - I2S

## Introducción

El objetivo de esta práctica es comprender el protocolo **I2S (Inter-IC Sound)**, utilizado para la transmisión de audio digital entre microcontroladores y convertidores DAC/amplificadores como el **MAX98357A**. Se utilizan dos enfoques:

1. Reproducir audio desde memoria interna (formato AAC).
2. Reproducir audio desde tarjeta SD (formato WAV).

El protocolo I2S permite una alta calidad de sonido al transmitir datos de forma síncrona usando tres líneas: **BCLK (SCK)**, **LRCK (WS)** y **DOUT (SD)**.

---

## Ejercicio 1: Reproducción desde Memoria Interna

### Objetivo

Reproducir un archivo de audio almacenado en la **memoria flash interna** del ESP32 utilizando el protocolo I2S y el decodificador MAX98357A.

### Código utilizado (`7.1.cpp`)

```cpp
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Iniciando reproducción AAC...");

    Serial.print("Primeros bytes: ");
    for (int i = 0; i < 10; i++) {
        Serial.printf("%02X ", sampleaac[i]);
    }
    Serial.println();

    in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
    out = new AudioOutputI2S();
    out->SetGain(0.8);
    out->SetPinout(5, 4, 6);  // BCLK, LRC, DOUT
    aac = new AudioGeneratorAAC();

    if (aac->begin(in, out)) {
        Serial.println("Decodificador AAC iniciado correctamente");
    } else {
        Serial.println("Error al iniciar el decodificador AAC");
    }
}

void loop() {
    if (aac->isRunning()) {
        if (!aac->loop()) {
            Serial.println("Reproducción completada");
            aac->stop();
        }
    } else {
        delay(1000);
    }
}
```

### Salida esperada en monitor serie

```
Iniciando reproducción AAC...
Primeros bytes: 21 43 65 ...
Decodificador AAC iniciado correctamente
Reproducción completada
```

### Explicación

- Se decodifica un archivo de audio AAC almacenado como array binario (`sampleaac`) en la memoria PROGMEM.
- Se reproduce usando el DAC/amplificador MAX98357 vía I2S.
- Se definen los pines I2S: BCLK (5), LRC (4), DOUT (6).
- El archivo se procesa con la librería `ESP8266Audio`.

---

## Ejercicio 2: Reproducción desde tarjeta SD (archivo WAV)

### Objetivo

Reproducir un archivo `.wav` desde una **tarjeta microSD** utilizando I2S y MAX98357A.

### Código utilizado (`7.2.cpp`)

```cpp
#include "Audio.h"
#include "SD.h"
#include "FS.h"

#define SPI_MISO 15
#define SPI_MOSI 16
#define SPI_SCK 17
#define SD_CS 18

#define I2S_DOUT 4
#define I2S_BCLK 5
#define I2S_LRC 6

Audio audio;

void setup() {
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    Serial.begin(115200);
    SD.begin(SD_CS);

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(10);
    audio.connecttoFS(SD, "Ensoniq-ZR-76-01-Dope-77.wav");
}

void loop() {
    audio.loop();
}
```

### Salida esperada en monitor serie

```
info: SD card initialized
info: Opening WAV file...
streaminfo: 44100Hz Stereo 16bit
...
eof_mp3: End of file
```

### Explicación

- Se inicializa la tarjeta SD y se monta el sistema de archivos.
- Se usa la librería `ESP32-audioI2S` para reproducir directamente el archivo `.wav`.
- Se definen pines para SPI y pines I2S (BCLK, LRC, DOUT).
- Se reproduce el audio con buena calidad (44.1 kHz, 16 bits estéreo).

---

## Conclusión

Con esta práctica se logra:

- Comprender la utilidad del bus I2S para audio digital.
- Configurar y usar un DAC amplificador (MAX98357A).
- Reproducir audio desde diferentes fuentes (memoria y SD).
- Aplicar protocolos síncronos para transmisión precisa de datos multimedia.

---

## Recomendaciones

- Incluir **foto del montaje físico** con altavoz, MAX98357 y ESP32.
- Asegurar el uso correcto de pines I2S (y evitar conflictos con otros periféricos).
- Verificar frecuencias de muestreo compatibles con el MAX98357 (8kHz–96kHz).
- Optimizar el código para reconexión automática o reproducción en bucle.

