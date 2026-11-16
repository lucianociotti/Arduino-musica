# Alma Misionera

[← Arduino música](../README.md)

Adaptación del sketch de **Roco** ([Rocoblog](https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html), lista de canciones con pantalla) para una pantalla **LCD I2C**.

## Qué cambió respecto del original

- `LiquidCrystal` (keypad shield) → `LiquidCrystal_I2C`, con `lcd.init()` y `lcd.backlight()`.
- Se quitó el relé.
- El buzzer sigue en D12.

El resto (melodía, textos y caracteres personalizados) es el del original.

## Conexiones y uso

Ver el [README principal](../README.md#conexiones). Necesita la biblioteca **LiquidCrystal I2C** y un buzzer pasivo. La dirección I2C es `0x27` (alternativa: `0x3F`).

Compila con avr-gcc. No se probó todavía en la placa.

El archivo original no se incluye: bajalo del blog de Roco.
