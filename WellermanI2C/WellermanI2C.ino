// Wellerman (sea shanty) - LCD I2C 16x2 + buzzer pasivo
// Melodia convertida de wellerman.mid.mid (linea mas aguda del piano), letra en el LCD.
// Libreria: "LiquidCrystal I2C" de Frank de Brabander (Administrar bibliotecas)
//
// Conexiones (Arduino UNO):
//   LCD I2C : GND -> GND | VCC -> 5V | SDA -> A4 | SCL -> A5
//   Buzzer  : (+) -> D12 | (-) -> GND   (buzzer PASIVO)
//
// Pantalla: fila de arriba = lo que se canta ahora, fila de abajo = lo que sigue.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR   0x27   // si la pantalla queda en blanco, probar 0x3F
#define PIN_BUZZER 12
#define BPM        192     // tempo del MIDI; bajalo para que suene mas lento
#define SIN_TEXTO  255

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// ---- Melodia: frecuencia en Hz y duracion en unidades de media negra ----
const uint16_t FREQ[] PROGMEM = {
  233, 262, 262, 262, 262, 311, 392, 392, 392, 392, 415, 349, 349, 349,
  415, 415, 523, 523, 392, 392, 392, 262, 262, 262, 311, 392, 392, 392,
  392, 392, 349, 311, 311, 294, 262, 523, 523, 523, 415, 466, 466, 392,
  392, 392, 415, 349, 349, 349, 415, 523, 392, 392, 523, 523, 415, 415,
  466, 466, 392, 392, 392, 392, 349, 466, 294, 392, 392, 262, 262, 262,
  311, 392, 392, 392, 392, 415, 349, 349, 415, 523, 392, 392, 392, 392,
  262, 262, 311, 392, 392, 392, 392, 392, 349, 466, 294, 392, 523, 523,
  523, 415, 466, 466, 392, 392, 392, 415, 349, 349, 349, 415, 523, 392,
  392, 523, 523, 415, 415, 466, 466, 392, 392, 392, 392, 349, 466, 294,
  392, 523, 523, 415, 466, 392, 392, 392, 415, 349, 349, 349, 415, 523,
  392, 392, 523, 523, 415, 466, 392, 392, 392, 392, 349, 466, 294, 392,
  233, 262, 392, 262, 262, 311, 392, 392, 392, 392, 392, 415, 349, 349,
  415, 523, 392, 392, 392, 392, 392, 262, 262, 262, 311, 392, 392, 392,
  392, 392, 349, 466, 294, 392, 523, 523, 523, 415, 466, 466, 392, 392,
  392, 415, 349, 349, 349, 415, 523, 392, 392, 523, 523, 415, 415, 466,
  466, 392, 392, 392, 392, 349, 466, 294, 392, 233, 392, 262, 262, 311,
  392, 392, 392, 392, 415, 349, 349, 415, 523, 392, 392, 262, 262, 392,
  262, 262, 311, 311, 392, 392, 392, 392, 392, 349, 466, 294, 392, 523,
  523, 523, 415, 466, 466, 392, 392, 392, 415, 349, 349, 349, 415, 523,
  392, 392, 523, 523, 415, 415, 466, 466, 392, 392, 392, 392, 349, 466,
  294, 392, 523, 523, 415, 466, 392, 392, 392, 415, 349, 349, 349, 415,
  523, 392, 392, 523, 523, 415, 466, 392, 392, 392, 392, 349, 466, 294,
  392, 233, 392, 262, 262, 311, 392, 392, 392, 392, 415, 349, 349, 415,
  523, 392, 392, 392, 392, 262, 262, 311, 311, 392, 392, 392, 392, 392,
  349, 466, 294, 392, 523, 523, 523, 415, 466, 466, 392, 392, 392, 415,
  349, 349, 349, 415, 523, 392, 392, 523, 523, 415, 415, 466, 466, 392,
  392, 392, 392, 349, 466, 294, 392, 233, 392, 262, 262, 262, 311, 392,
  392, 392, 392, 415, 349, 349, 415, 415, 523, 392, 392, 392, 392, 262,
  262, 262, 311, 392, 392, 392, 392, 392, 392, 415, 349, 349, 349, 349,
  415, 523, 392, 392, 523, 523, 415, 466, 466, 392, 392, 392, 415, 349,
  349, 349, 415, 523, 392, 392, 523, 523, 415, 415, 466, 466, 392, 392,
  392, 392, 349, 466, 294, 392, 523, 523, 587, 622, 622, 466, 392, 392,
  415, 349, 349, 349, 415, 523, 392, 392, 523, 523, 415, 415, 466, 466,
  392, 392, 392, 392, 349, 466, 294, 392
};
const uint8_t DUR[] PROGMEM = {
  2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 2, 3, 1, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 4, 4, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1,
  2, 2, 2, 4, 4, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 8, 1, 1, 2, 2, 2, 2,
  2, 3, 1, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
  4, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1, 2, 2, 2, 4, 4, 2, 1, 1, 1, 1, 2,
  2, 2, 2, 2, 2, 2, 8, 4, 3, 1, 2, 2, 3, 1, 2, 2, 2, 1, 1, 2, 2, 4, 4, 3,
  1, 2, 2, 3, 1, 2, 2, 2, 2, 6, 1, 1, 2, 1, 3, 2, 2, 1, 2, 2, 1, 2, 2, 2,
  2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 3, 1,
  1, 1, 2, 3, 1, 2, 2, 1, 1, 2, 2, 2, 4, 4, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2,
  2, 2, 6, 2, 2, 1, 3, 2, 2, 2, 3, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 3,
  1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1,
  2, 2, 2, 4, 4, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 8, 4, 3, 1, 2, 2, 3,
  1, 2, 2, 2, 1, 1, 2, 2, 4, 4, 3, 1, 2, 2, 3, 1, 2, 2, 2, 2, 6, 2, 2, 1,
  3, 2, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2,
  2, 2, 2, 4, 4, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1, 2, 2, 2, 4, 4, 2, 1,
  1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 6, 2, 1, 1, 1, 4, 1, 2, 2, 3, 1, 2, 2, 2,
  1, 1, 2, 2, 3, 1, 1, 1, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
  2, 4, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1, 2, 2, 2, 4, 4, 2, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 8, 4, 3, 1, 1, 1, 2, 3, 1, 2, 2, 1, 1, 2, 2, 2, 4,
  4, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 12
};

// ---- Letra: trozos de hasta 16 caracteres ----
const char TEXTO[][17] PROGMEM = {
  "There once was a",
  "ship that put",
  "to sea",
  "And the name of",
  "that ship was",
  "the Billy of Tea",
  "The winds blew",
  "hard, her bow",
  "dipped down",
  "Blow, my bully",
  "boys, blow",
  "Soon may the",
  "Wellerman come",
  "To bring us",
  "sugar and tea",
  "and rum",
  "One day, when",
  "the tonguin'",
  "is done",
  "We'll take",
  "our leave",
  "and go",
  "She had not",
  "been two weeks",
  "from shore",
  "When down on",
  "her a right",
  "whale bore",
  "The captain",
  "called all hands",
  "and swore",
  "He'd take that",
  "whale in tow",
  "Before the boat",
  "had hit the",
  "water",
  "The whale's tail",
  "came up and",
  "caught her",
  "All hands to",
  "the side,",
  "harpooned and",
  "fought her",
  "When she dived",
  "down below",
  "No line was",
  "cut, no whale",
  "was freed",
  "The captain's",
  "mind was not",
  "on greed",
  "But he belonged",
  "to the",
  "whaleman's creed",
  "She took the",
  "ship in tow",
  "For forty days,",
  "or even more",
  "The line went",
  "slack, then",
  "tight once more",
  "All boats were",
  "lost, there were",
  "only four",
  "But still that",
  "whale did go",
  "As far as",
  "I've heard,",
  "the fight's",
  "still on",
  "The line's not",
  "cut and the",
  "whale's not gone",
  "The Wellerman",
  "makes his",
  "regular call",
  "To encourage",
  "the captain,",
  "crew and all"
};

// ---- En la nota CUE_NOTA[k] aparece el trozo CUE_TEXTO[k] ----
const uint16_t CUE_NOTA[] PROGMEM = {
  0, 4, 7, 9, 13, 16, 21, 24, 27, 29, 33, 35, 38, 43, 46, 50,
  52, 55, 58, 60, 62, 64, 66, 69, 72, 74, 77, 80, 82, 85, 88, 90,
  93, 96, 99, 104, 107, 111, 113, 116, 119, 121, 123, 125, 127, 129, 133, 136,
  140, 142, 144, 146, 148, 150, 152, 154, 158, 161, 164, 167, 170, 173, 176, 178,
  181, 183, 185, 188, 191, 196, 199, 203, 205, 208, 211, 213, 215, 217, 219, 222,
  225, 227, 230, 233, 236, 240, 242, 245, 248, 251, 254, 259, 262, 266, 268, 271,
  274, 276, 278, 280, 282, 284, 288, 291, 295, 297, 299, 301, 303, 305, 307, 309,
  313, 317, 320, 322, 325, 328, 331, 334, 337, 340, 343, 348, 351, 355, 357, 360,
  363, 365, 367, 369, 371, 374, 376, 378, 380, 383, 386, 389, 393, 395, 400, 404,
  407, 410, 413, 417, 420, 424, 426, 429, 432, 434, 436, 438, 440, 443, 447, 450,
  454, 456, 459, 462, 464, 466, 468
};
const uint8_t CUE_TEXTO[] PROGMEM = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26, 27, 28, 29, 30, 31, 32, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 11, 12, 13, 14, 15, 16, 17,
  18, 19, 20, 21, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 56, 57, 58, 59, 60, 61, 62, 63, 64,
  65, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
  78, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21
};

const uint16_t NNOTAS = sizeof(FREQ) / sizeof(FREQ[0]);
const uint16_t NCUES  = sizeof(CUE_NOTA) / sizeof(CUE_NOTA[0]);

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

void portada() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("   WELLERMAN    "));
  lcd.setCursor(0, 1);
  lcd.print(F("   Sea shanty   "));
  delay(2000);
  lcd.clear();
}

void tocar() {
  uint16_t cue = 0;
  unsigned long t = millis();                 // instante en que debe empezar la nota
  for (uint16_t i = 0; i < NNOTAS; i++) {
    uint16_t f = pgm_read_word(&FREQ[i]);
    unsigned long d = (unsigned long)pgm_read_byte(&DUR[i]) * 30000UL / BPM;
    while ((long)(millis() - t) < 0) { }      // espera exacta (no acumula atrasos)
    if (f) tone(PIN_BUZZER, f, d - d / 8);    // suena 7/8, 1/8 de silencio entre notas
    if (cue < NCUES && pgm_read_word(&CUE_NOTA[cue]) == i) {
      mostrarCue(cue);                        // se escribe mientras la nota suena
      cue++;
    }
    t += d;
  }
  while ((long)(millis() - t) < 0) { }
  noTone(PIN_BUZZER);
}

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  portada();
  tocar();
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("      FIN       "));
  delay(3000);
}
