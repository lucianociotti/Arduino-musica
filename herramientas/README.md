# Herramientas: de MIDI a sketch

Scripts en Python 3, sin dependencias, con los que se generaron los sketches de [Wellerman](../WellermanI2C) y [Marcha Peronista](../MarchaPeronistaI2C).

| Archivo | Para qué sirve |
|---|---|
| `midiparse.py` | Lector mínimo de archivos MIDI |
| `skyline.py` | Se queda con la nota más aguda de cada instante (MIDI cuantizados) |
| `rawdump.py` | Lista las notas con su velocidad; sirve para inspeccionar un MIDI (`python rawdump.py archivo.mid 0 16` muestra los tiempos 0 a 16) |
| `gen_wellerman.py` | Genera `WellermanI2C.ino` |
| `gen_marcha_peronista.py` | Genera `MarchaPeronistaI2C.ino` |

## Uso

Desde la carpeta principal del repositorio, con tu propio archivo MIDI:

```bash
python herramientas/gen_wellerman.py "Wellerman/wellerman.mid.mid" WellermanI2C/WellermanI2C.ino
python herramientas/gen_marcha_peronista.py "Marcha Peronista.mid" MarchaPeronistaI2C/MarchaPeronistaI2C.ino
```

Con un tercer argumento cualquiera (por ejemplo `diag`) muestran cómo se repartió la letra sobre las notas.

Los MIDI no se incluyen. Los scripts están ajustados a esos dos archivos: los rangos de notas, las secciones y la letra están escritos adentro. Para otra canción hay que copiar uno y adaptarlo.

La letra se divide en trozos de hasta 16 caracteres, sin acentos ni ñ. Después de generar, compilá en el IDE para verificar.
