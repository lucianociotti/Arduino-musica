# The Trooper (Iron Maiden)

[← Arduino música](../README.md)

Adaptación del sketch de **Roco** ([Rocoblog](https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html), lista de buzzer pasivo) para una pantalla **LCD I2C**.

## Qué cambió respecto del original

- El original no usaba pantalla: acá se agrega una **portada** en el LCD ("THE TROOPER / Iron Maiden").
- El relé se reemplazó por el **LED integrado** (D13), que sigue marcando el ritmo donde antes se activaba el relé. Se cambia con `PIN_LUZ` en el sketch.
- El buzzer sigue en D12.

## Conexiones y uso

Ver el [README principal](../README.md#conexiones). Necesita la biblioteca **LiquidCrystal I2C** y un buzzer pasivo. La dirección I2C es `0x27` (alternativa: `0x3F`).

Compila con avr-gcc. No se probó todavía en la placa.

El archivo original no se incluye: bajalo del blog de Roco.
