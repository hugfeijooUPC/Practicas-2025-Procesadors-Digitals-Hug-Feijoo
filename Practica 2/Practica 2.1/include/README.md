# Informe de Funcionamiento

## Descripción General

Este proyecto utiliza un botón (pulsador) conectado a la placa para controlar un LED NeoPixel. Cada vez que el usuario pulsa el botón, el sistema realiza las siguientes acciones:

- Incrementa un contador de pulsaciones.
- Enciende el NeoPixel en color blanco durante un breve periodo.
- Imprime por el puerto serie el número total de pulsaciones detectadas.

## Funcionamiento

1. **Inicialización:**
   - El pin del botón se configura como entrada con resistencia pull-up interna.
   - El NeoPixel se inicializa y se apaga al inicio.
   - Se configura una interrupción externa en el pin del botón para detectar pulsaciones (flanco de bajada).
   - Se inicia la comunicación por el puerto serie a 115200 baudios.

2. **Detección de Pulsaciones:**
   - Cuando el usuario pulsa el botón, se activa la interrupción.
   - El contador de pulsaciones se incrementa y se marca que el botón ha sido presionado.

3. **Respuesta a la Pulsación:**
   - En el bucle principal (`loop`), si se detecta que el botón fue presionado:
     - Se imprime por el puerto serie el número total de pulsaciones.
     - El NeoPixel se enciende en color blanco durante 100 ms.
     - Luego, el NeoPixel se apaga.

## Salidas por Impresión Serie

Cada vez que se pulsa el botón, la salida por el monitor serie será similar a:

```
Button pressed 1 times
Button pressed 2 times
Button pressed 3 times
...

Cada línea indica el número total de veces que se ha pulsado el botón desde que se inició el programa.
