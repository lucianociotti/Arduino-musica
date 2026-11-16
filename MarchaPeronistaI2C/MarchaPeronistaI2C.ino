// Marcha Peronista - LCD I2C 16x2 + buzzer pasivo, con letra sincronizada (3 estrofas)
// Melodia convertida de "Marcha Peronista.mid" (notas fuertes del piano; se descartan
// acompanamiento y adornos). Ritmo y silencios tal como estan en el MIDI.
// Idea del buzzer con tone() en D12: Roco (Rocoblog)
// https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html
// Libreria: "LiquidCrystal I2C" de Frank de Brabander (Administrar bibliotecas)
//
// Conexiones (Arduino UNO):
//   LCD I2C : GND -> GND | VCC -> 5V | SDA -> A4 | SCL -> A5
//   Buzzer  : (+) -> D12 | (-) -> GND   (buzzer PASIVO)
//
// Estructura (el MIDI trae una pasada rapida y una lenta):
//   pasada 1 = rapida, estrofa 1 + estribillo (dos veces)
//   pasada 2 = la misma melodia rapida, estrofa 2 + estribillo (dos veces)
//   pasada 3 = lenta (final), estrofa 3 + estribillo
// Pantalla: fila de arriba = lo que se canta ahora, fila de abajo = lo que sigue.
// (Sin acentos ni enie: el LCD no los tiene.)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR   0x27   // si la pantalla queda en blanco, probar 0x3F
#define PIN_BUZZER 12
#define BPM        120     // tempo del MIDI; bajalo para que suene mas lento
#define SIN_TEXTO  255

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// ---- Melodia: frecuencia (Hz), tiempo hasta la siguiente nota (IOI) y cuanto suena (SND) ----
// IOI y SND estan en doceavos de negra; lo que IOI tiene de mas que SND es silencio.
const uint16_t FREQ[] PROGMEM = {
  659, 523, 440, 659, 523, 440, 659, 523, 659, 523, 440, 659, 659, 587,
  523, 587, 494, 587, 494, 415, 587, 494, 415, 587, 494, 698, 698, 698,
  659, 587, 659, 698, 659, 587, 659, 698, 659, 587, 659, 523, 440, 659,
  523, 440, 659, 523, 659, 659, 659, 784, 698, 659, 698, 587, 659, 698,
  698, 659, 587, 659, 659, 523, 587, 659, 587, 523, 494, 440, 659, 659,
  587, 523, 494, 523, 659, 659, 587, 523, 494, 523, 440, 440, 698, 587,
  587, 659, 523, 587, 659, 587, 523, 494, 440, 659, 659, 659, 587, 523,
  494, 523, 659, 659, 587, 523, 494, 523, 440, 440, 698, 587, 587, 659,
  523, 587, 659, 587, 523, 494, 440, 330, 440, 659, 523, 440, 659, 523,
  440, 659, 523, 659, 523, 440, 659, 659, 587, 523, 587, 494, 587, 494,
  415, 587, 494, 415, 587, 494, 698, 698, 698, 698, 659, 587, 659, 698,
  659, 587, 659, 698, 659, 587, 659, 523, 440, 659, 523, 440, 659, 523,
  659, 659, 659, 784, 698, 659, 698, 587, 659, 698, 698, 659, 587, 659,
  659, 523, 587, 659, 587, 523, 494, 440, 659, 659, 659, 659, 587, 523,
  494, 523, 659, 659, 659, 659, 587, 523, 494, 523, 440, 440, 440, 698,
  587, 587, 587, 659, 523, 523, 587, 659, 587, 523, 494, 440, 330, 440
};
const uint8_t IOI[] PROGMEM = {
  12, 8, 4, 12, 8, 4, 24, 24, 12, 8, 4, 8, 4, 8, 4, 24, 24, 13, 7, 4, 12, 8, 5, 24,
  24, 12, 8, 16, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 12, 8, 4, 12, 8, 4, 24, 24, 12, 8,
  4, 12, 8, 5, 35, 8, 5, 11, 12, 13, 12, 11, 25, 7, 5, 12, 12, 12, 12, 8, 12, 36, 4, 8,
  4, 32, 12, 36, 4, 8, 4, 32, 12, 4, 32, 12, 4, 32, 12, 4, 12, 12, 12, 12, 9, 4, 8, 36,
  4, 8, 4, 32, 12, 36, 4, 8, 4, 32, 12, 4, 32, 12, 4, 32, 12, 4, 12, 12, 13, 12, 11, 15,
  96, 24, 16, 8, 24, 16, 8, 48, 48, 25, 15, 9, 15, 9, 15, 9, 48, 48, 24, 16, 8, 24, 16, 8,
  48, 48, 24, 16, 8, 24, 16, 8, 16, 9, 15, 8, 16, 8, 17, 8, 24, 16, 8, 24, 16, 8, 48, 48,
  24, 16, 8, 24, 16, 8, 72, 16, 8, 24, 24, 24, 24, 24, 49, 16, 7, 25, 24, 24, 24, 16, 8, 16,
  8, 64, 8, 16, 8, 64, 8, 16, 8, 64, 8, 17, 7, 65, 8, 16, 8, 64, 8, 16, 8, 64, 8, 16,
  8, 24, 24, 24, 24, 24, 24, 7
};
const uint8_t SND[] PROGMEM = {
  11, 8, 4, 11, 6, 4, 21, 10, 11, 7, 4, 7, 4, 7, 4, 10, 8, 10, 7, 4, 10, 8, 5, 7,
  11, 11, 7, 16, 8, 4, 8, 4, 8, 4, 8, 4, 8, 4, 12, 7, 4, 11, 7, 4, 21, 7, 11, 7,
  4, 11, 8, 4, 22, 6, 4, 11, 12, 12, 9, 11, 23, 7, 5, 11, 7, 7, 11, 8, 11, 26, 4, 7,
  4, 11, 11, 27, 4, 7, 4, 11, 10, 4, 19, 10, 4, 16, 11, 4, 12, 10, 7, 11, 8, 4, 7, 24,
  4, 7, 4, 10, 11, 28, 4, 7, 4, 11, 11, 4, 21, 10, 4, 22, 11, 4, 11, 8, 7, 8, 6, 14,
  8, 22, 9, 8, 20, 13, 8, 20, 11, 16, 8, 8, 15, 8, 8, 8, 7, 13, 7, 14, 8, 12, 14, 8,
  9, 8, 21, 15, 8, 22, 15, 7, 15, 9, 14, 7, 15, 8, 15, 7, 20, 13, 8, 22, 12, 8, 22, 14,
  21, 15, 8, 17, 15, 8, 21, 10, 8, 21, 17, 15, 11, 20, 22, 9, 7, 22, 7, 9, 15, 11, 8, 15,
  8, 19, 8, 11, 8, 14, 8, 15, 8, 21, 8, 8, 7, 12, 7, 9, 7, 20, 7, 9, 8, 18, 7, 9,
  7, 21, 10, 7, 16, 8, 23, 7
};

// ---- Letra: trozos de hasta 16 caracteres ----
const char TEXTO[][17] PROGMEM = {
  "Los muchachos",
  "peronistas",
  "Todos unidos",
  "triunfaremos",
  "Y como siempre",
  "daremos",
  "Un grito de",
  "corazon",
  "Viva Peron!",
  "Por ese gran",
  "argentino",
  "Que se supo",
  "conquistar",
  "A la gran masa",
  "del pueblo",
  "Combatiendo",
  "al capital",
  "Peron, Peron!",
  "Que grande sos",
  "Mi general!",
  "Cuanto vales",
  "Peron, Peron,",
  "gran conductor!",
  "Sos el primer",
  "trabajador!",
  "Por los",
  "principios",
  "sociales",
  "Que Peron ha",
  "establecido",
  "El pueblo entero",
  "esta unido",
  "Y grita de",
  "Viva Peron,",
  "Que trabajo",
  "sin cesar",
  "Para que reine",
  "en el pueblo",
  "El amor y",
  "la igualdad",
  "Imitemos",
  "el ejemplo",
  "De este varon",
  "Y siguiendo",
  "su camino",
  "Gritemos de",
  "Por esa",
  "Argentina grande",
  "con que",
  "San Martin sono",
  "Es la realidad",
  "efectiva",
  "Que debemos",
  "a Peron"
};

// ---- En la nota CUE_NOTA[k] aparece el trozo CUE_TEXTO[k] (las notas se repiten entre pasadas) ----
const uint16_t CUE_NOTA[] PROGMEM = {
  0, 4, 8, 13, 17, 22, 25, 29, 32, 35, 38, 42, 46, 50, 53, 58,
  61, 65, 70, 73, 76, 79, 83, 86, 89, 92, 96, 99, 102, 105, 109, 112,
  115, 118, 0, 2, 5, 8, 11, 16, 20, 25, 28, 31, 34, 38, 42, 46,
  50, 53, 57, 61, 65, 70, 73, 76, 79, 83, 86, 89, 92, 96, 99, 102,
  105, 109, 112, 115, 118, 121, 125, 129, 134, 138, 142, 146, 150, 153, 156, 160,
  162, 168, 170, 175, 179, 183, 187, 194, 198, 202, 206, 210, 214, 218, 221
};
const uint8_t CUE_TEXTO[] PROGMEM = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
  23, 24, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 7, 33, 8, 9, 10, 34,
  35, 36, 37, 38, 39, 17, 18, 19, 20, 21, 22, 23, 24, 17, 18, 19, 20, 21, 22, 23, 24, 40, 41, 42,
  10, 43, 44, 45, 7, 33, 8, 46, 47, 48, 49, 50, 51, 52, 53, 17, 18, 19, 20, 21, 22, 23, 24
};

// ---- Pasadas: {primera nota, ultima nota + 1, primer cue, ultimo cue + 1} ----
const uint16_t PASADAS[][4] PROGMEM = {
  {0, 121, 0, 34},
  {0, 121, 34, 69},
  {121, 224, 69, 95}
};

const uint16_t NCUES    = sizeof(CUE_NOTA) / sizeof(CUE_NOTA[0]);
const uint8_t  NPASADAS = sizeof(PASADAS) / sizeof(PASADAS[0]);

void escribirFila(uint8_t fila, uint8_t id) {
  char buf[17];
  uint8_t n = 0;
  if (id != SIN_TEXTO) {
    strcpy_P(buf, TEXTO[id]);
    n = strlen(buf);
  }
  while (n < 16) buf[n++] = ' ';
  buf[16] = 0;
  lcd.setCursor(0, fila);
  lcd.print(buf);
}

void mostrarCue(uint16_t k) {
  escribirFila(0, pgm_read_byte(&CUE_TEXTO[k]));
  escribirFila(1, (k + 1 < NCUES) ? pgm_read_byte(&CUE_TEXTO[k + 1]) : SIN_TEXTO);
}

// Toca las notas [desde, hasta) mostrando los cues [cue, cueFin)
void tocar(uint16_t desde, uint16_t hasta, uint16_t cue, uint16_t cueFin) {
  unsigned long t = millis();                 // instante en que debe empezar la nota
  for (uint16_t i = desde; i < hasta; i++) {
    uint16_t f = pgm_read_word(&FREQ[i]);
    unsigned long ioi = (unsigned long)pgm_read_byte(&IOI[i]) * 5000UL / BPM;
    unsigned long snd = (unsigned long)pgm_read_byte(&SND[i]) * 5000UL / BPM;
    while ((long)(millis() - t) < 0) { }      // espera exacta (no acumula atrasos)
    tone(PIN_BUZZER, f, snd - snd / 8);       // 1/8 de silencio para separar notas iguales
    if (cue < cueFin && pgm_read_word(&CUE_NOTA[cue]) == i) {
      mostrarCue(cue);                        // se escribe mientras la nota suena
      cue++;
    }
    t += ioi;
  }
  while ((long)(millis() - t) < 0) { }        // la ultima nota incluye la pausa hasta la pasada siguiente
  noTone(PIN_BUZZER);
}

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("MARCHA PERONISTA"));
  delay(1500);
  for (uint8_t p = 0; p < NPASADAS; p++) {
    tocar(pgm_read_word(&PASADAS[p][0]), pgm_read_word(&PASADAS[p][1]),
          pgm_read_word(&PASADAS[p][2]), pgm_read_word(&PASADAS[p][3]));
  }
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("      FIN       "));
  delay(3000);
}
