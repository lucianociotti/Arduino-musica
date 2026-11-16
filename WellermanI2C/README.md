# Wellerman

[← Arduino música](../README.md)

El shanty tradicional neozelandés, con la **letra sincronizada** en el LCD. Usa el mismo esquema de buzzer en D12 que el proyecto de [Roco](https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html).

## Cómo está hecho

- La melodía sale de un archivo MIDI (no incluido): en cada instante se toma la nota más aguda. Son 470 notas a 192 BPM, unos 2 min 30 s.
- La letra va en 79 trozos de hasta 16 caracteres. La fila de arriba es lo que se canta ahora y la de abajo lo que sigue.
- Cada trozo aparece en su nota. El reparto es proporcional a las sílabas, así que algún corte puede correrse una nota.
- La reproducción usa el reloj (`millis`) y `tone()` sin bloqueo: escribir en el LCD no atrasa el ritmo.
- Se puede bajar la velocidad con `#define BPM`.

El script que lo generó está en [herramientas](../herramientas).

## Conexiones y uso

Ver el [README principal](../README.md#conexiones). Necesita la biblioteca **LiquidCrystal I2C** y un buzzer pasivo. La dirección I2C es `0x27` (alternativa: `0x3F`).

Compila con avr-gcc. No se probó todavía en la placa.
