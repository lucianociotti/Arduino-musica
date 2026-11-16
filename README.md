# Arduino música

Canciones en **Arduino UNO** con pantalla **LCD 16x2 I2C** y **buzzer pasivo**. Algunas muestran la letra sincronizada con la melodía: la fila de arriba es lo que se canta ahora y la de abajo lo que sigue.

## Créditos

La idea de hacer música con un Arduino y un buzzer es de **Francisco Roco** (Rocoblog):
[Música con Arduino uno y un buzzer](https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html) (21 de abril de 2022).

Alma Misionera y The Trooper parten de las transcripciones de su blog; acá están adaptadas a la pantalla I2C. Los archivos originales **no** se incluyen: bajalos desde el post. Wellerman y Marcha Peronista se convirtieron desde archivos MIDI, usando el mismo esquema de conexión (buzzer en D12 con `tone()`).

## Canciones

Cada canción tiene su propia carpeta con su README, así que tiene su propio link:

| Canción | Carpeta | Qué tiene |
|---|---|---|
| Alma Misionera | [AlmaMisioneraI2C](AlmaMisioneraI2C) | Adaptación de la transcripción de Roco, con texto en pantalla |
| The Trooper (Iron Maiden) | [TheTrooperI2C](TheTrooperI2C) | Adaptación de la transcripción de Roco, con un LED que acompaña |
| Wellerman | [WellermanI2C](WellermanI2C) | Convertida desde MIDI, con letra sincronizada |
| Marcha Peronista | [MarchaPeronistaI2C](MarchaPeronistaI2C) | Convertida desde MIDI, con las 3 estrofas sincronizadas |

## Hardware

- Arduino UNO R3
- LCD 1602 con mochila I2C (PCF8574)
- Buzzer **pasivo** (si suena como un pitido fijo, es activo y no sirve para `tone()`)

## Conexiones

| Pieza | Pin del Arduino |
|---|---|
| LCD I2C GND | GND |
| LCD I2C VCC | 5V |
| LCD I2C SDA | A4 |
| LCD I2C SCL | A5 |
| Buzzer (+) | D12 |
| Buzzer (−) | GND |

## Cómo usarlo

1. Instalá la biblioteca **LiquidCrystal I2C** (Frank de Brabander) desde *Programa → Incluir biblioteca → Administrar bibliotecas*.
2. Abrí el `.ino` de la carpeta de la canción. El IDE exige que la carpeta se llame igual que el `.ino`, así que no las renombres.
3. Elegí la placa *Arduino Uno* y subilo.

Si la pantalla queda en blanco, probá primero el potenciómetro de contraste de la mochila. Si sigue en blanco, cambiá `LCD_ADDR` de `0x27` a `0x3F`.

## Estado

- Los cuatro sketches compilan con avr-gcc 7.3 (núcleo AVR 1.8.8, placa UNO).
- **Todavía no se probaron en la placa.** Si alguno falla o suena mal, abrí un issue.
- El LCD no tiene acentos ni ñ, así que las letras van sin ellos.

## Cómo se convirtieron los MIDI

La carpeta [herramientas](herramientas) tiene los scripts en Python (sin dependencias) con los que se generaron `WellermanI2C` y `MarchaPeronistaI2C`: leen el MIDI, se quedan con la melodía, la dejan en tablas `PROGMEM` y reparten la letra por sílabas sobre las notas. Están ajustados a cada MIDI en particular. Los MIDI no se incluyen.

## Derechos

Las melodías y letras pertenecen a sus autores. Este repositorio es un proyecto personal y educativo. Si sos titular de derechos y querés que algo se quite, abrí un issue.
