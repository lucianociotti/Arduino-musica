# Marcha Peronista

[← Arduino música](../README.md)

La marcha con las **tres estrofas y el estribillo** sincronizados en el LCD. Usa el mismo esquema de buzzer en D12 que el proyecto de [Roco](https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html).

## Estructura

El MIDI (un piano interpretado) trae una pasada rápida y una lenta. El sketch toca tres pasadas, definidas en la tabla `PASADAS` del `.ino` (dura unos 3 min 43 s):

| Pasada | Melodía | Letra |
|---|---|---|
| 1 | Rápida | Estrofa 1 y estribillo dos veces |
| 2 | La misma rápida | Estrofa 2 y estribillo dos veces |
| 3 | Lenta (final) | Estrofa 3 y estribillo |

La pausa entre pasadas es la del propio MIDI. Mientras dura, la fila de abajo ya muestra el primer trozo de la estrofa siguiente.

## Cómo está hecho

- Del piano se toma la melodía por **velocidad**: se descartan el acompañamiento y los adornos agudos. El ritmo y los silencios se guardan en doceavos de negra.
- La letra va en trozos de hasta 16 caracteres. La fila de arriba es lo que se canta ahora y la de abajo lo que sigue.
- La letra se reparte por sílabas sobre las notas. Algún corte puede correrse una nota, y los versos más largos ("El pueblo entero esta unido", "Es la realidad efectiva") quedan apretados.
- Se puede bajar la velocidad con `#define BPM`.

El script que lo generó está en [herramientas](../herramientas). El MIDI no se incluye.

## Conexiones y uso

Ver el [README principal](../README.md#conexiones). Necesita la biblioteca **LiquidCrystal I2C** y un buzzer pasivo. La dirección I2C es `0x27` (alternativa: `0x3F`).

Compila con avr-gcc. No se probó todavía en la placa. La letra se incluye tal como se pegó; ver la nota de [derechos](../README.md#derechos).
