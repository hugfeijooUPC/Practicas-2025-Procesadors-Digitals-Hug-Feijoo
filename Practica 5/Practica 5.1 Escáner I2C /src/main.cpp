// Este programa escanea el bus I2C usando pines personalizados para SDA y SCL.
// Imprime en el monitor serie las direcciones de los dispositivos encontrados.

#include <Arduino.h>
#include <Wire.h>

// Definición de los pines SDA y SCL personalizados
#define SDA_PIN 6  // Nuevo pin SDA
#define SCL_PIN 7  // Nuevo pin SCL

// Configuración inicial del microcontrolador y del bus I2C
void setup()
{
    // Inicializa el bus I2C en los pines definidos
    Wire.begin(SDA_PIN, SCL_PIN);

    // Inicializa la comunicación serie a 115200 baudios
    Serial.begin(115200);

    // Espera a que el monitor serie esté listo (útil en placas como Leonardo)
    while (!Serial);

    // Mensaje inicial para el usuario
    Serial.println("\nI2C Scanner");
}

// Bucle principal: escanea todas las direcciones posibles del bus I2C
void loop()
{
    // Variables para almacenar errores, dirección y número de dispositivos encontrados
    byte error, address;
    int nDevices;

    // Mensaje de inicio de escaneo
    Serial.println("Scanning...");

    // Inicializa el contador de dispositivos encontrados
    nDevices = 0;

    // Recorre todas las direcciones posibles del bus I2C (1 a 126)
    for(address = 1; address < 127; address++ )
    {
        // Inicia la transmisión I2C a la dirección actual
        Wire.beginTransmission(address);

        // Finaliza la transmisión y obtiene el código de error
        error = Wire.endTransmission();

        // Si no hay error, se ha encontrado un dispositivo en esa dirección
        if (error == 0)
        {
            // Imprime la dirección del dispositivo encontrado
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
            nDevices++;
        }
        // Si el error es 4, se produjo un error desconocido
        else if (error==4)
        {
            // Imprime mensaje de error desconocido para esa dirección
            Serial.print("Unknown error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        }
    }

    // Si no se encontraron dispositivos, lo indica
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    // Si se encontraron, indica que terminó el escaneo
    else
        Serial.println("done\n");

    // Espera 2 segundos antes de volver a escanear
    delay(2000);
}