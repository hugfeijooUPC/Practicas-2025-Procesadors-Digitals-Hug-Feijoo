# Informe de Funcionamiento

## Descripción General

Este proyecto controla un LED NeoPixel conectado a la placa, permitiendo ajustar la frecuencia de parpadeo mediante dos pulsadores:

- **Pulsador UP (pin 18):** Aumenta la frecuencia de parpadeo (reduce el intervalo).
- **Pulsador DOWN (pin 19):** Disminuye la frecuencia de parpadeo (aumenta el intervalo).

El sistema utiliza un temporizador hardware para leer el estado de los pulsadores y aplicar un filtrado de rebotes, asegurando que los cambios de frecuencia sean precisos y estables.

## Funcionamiento

1. **Inicialización:**
   - El LED NeoPixel se configura y se apaga al inicio.
   - Los pulsadores se configuran como entradas con resistencia pull-up interna.
   - El temporizador hardware se inicia para leer los pulsadores cada 10 ms.

2. **Parpadeo del LED:**
   - El LED parpadea en color blanco con un intervalo que puede variar entre 50 ms y 10,000 ms (10 segundos).
   - El intervalo de parpadeo se ajusta en tiempo real según las pulsaciones de los botones.

3. **Ajuste de Frecuencia:**
   - Al pulsar el botón UP, el intervalo de parpadeo se reduce a la mitad (hasta un mínimo de 50 ms).
   - Al pulsar el botón DOWN, el intervalo de parpadeo se duplica (hasta un máximo de 10,000 ms).

4. **Comunicación Serie:**
   - Cada vez que se modifica la frecuencia de parpadeo, se imprime por el puerto serie el nuevo intervalo en milisegundos.

## Salidas por Impresión Serie

Durante el funcionamiento, la salida por el monitor serie será similar a la siguiente:

```
Nuevo intervalo de parpadeo: 250 ms
Nuevo intervalo de parpadeo: 125 ms
Nuevo intervalo de parpadeo: 250 ms
Nuevo intervalo de parpadeo: 500 ms
Nuevo intervalo de parpadeo: 1000 ms
```

Cada línea indica el nuevo intervalo de parpadeo tras pulsar uno de los botones. Así, el usuario puede comprobar en tiempo real los cambios de frecuencia realizados.
