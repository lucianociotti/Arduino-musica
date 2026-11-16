import sys, os, math, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from skyline import skyline
from midiparse import nname

midi, out = sys.argv[1], sys.argv[2]
diag = len(sys.argv) > 3
ppq, notes, line, tr = skyline(midi)
BPM = 192

N = []
for s, e, p, g in line:
    N.append(dict(b=s / ppq, p=p, gap=g / ppq, ln=(e - s) / ppq))
for n in N:
    assert abs(n['b'] * 2 - round(n['b'] * 2)) < 1e-6, n
    assert abs(n['gap'] * 2 - round(n['gap'] * 2)) < 1e-6, n
rests = [(i, n['gap'] - n['ln']) for i, n in enumerate(N) if n['gap'] - n['ln'] >= 0.5 - 1e-6]
print('notas', len(N), '| silencios >= media negra:', rests)

# ---------------- letra (trozos <= 16 caracteres) ----------------
V = [
 [["There once was a", "ship that put", "to sea"],
  ["And the name of", "that ship was", "the Billy of Tea"],
  ["The winds blew", "hard, her bow", "dipped down"],
  ["Blow, my bully", "boys, blow"]],
 [["She had not", "been two weeks", "from shore"],
  ["When down on", "her a right", "whale bore"],
  ["The captain", "called all hands", "and swore"],
  ["He'd take that", "whale in tow"]],
 [["Before the boat", "had hit the", "water"],
  ["The whale's tail", "came up and", "caught her"],
  ["All hands to", "the side,", "harpooned and", "fought her"],
  ["When she dived", "down below"]],
 [["No line was", "cut, no whale", "was freed"],
  ["The captain's", "mind was not", "on greed"],
  ["But he belonged", "to the", "whaleman's creed"],
  ["She took the", "ship in tow"]],
 [["For forty days,", "or even more"],
  ["The line went", "slack, then", "tight once more"],
  ["All boats were", "lost, there were", "only four"],
  ["But still that", "whale did go"]],
 [["As far as", "I've heard,", "the fight's", "still on"],
  ["The line's not", "cut and the", "whale's not gone"],
  ["The Wellerman", "makes his", "regular call"],
  ["To encourage", "the captain,", "crew and all"]],
]
C = [["Soon may the", "Wellerman come"],
     ["To bring us", "sugar and tea", "and rum"],
     ["One day, when", "the tonguin'", "is done"],
     ["We'll take", "our leave", "and go"]]

SYL = {"billy": 2, "bully": 2, "wellerman": 3, "sugar": 2, "tonguin'": 2, "captain": 2, "captain's": 2,
       "water": 2, "harpooned": 2, "before": 2, "belonged": 2, "whaleman's": 2, "forty": 2, "even": 2,
       "only": 2, "below": 2, "regular": 3, "encourage": 3}
def syl(chunk):
    return sum(SYL.get(w.lower().strip(',.!?;:'), 1) for w in chunk.split())

# ---------------- secciones: (tipo, indice, beat 1a nota, beat alineado a compas) ----------------
SEC = [("V", 0, 0, 1), ("C", 0, 31, 33), ("V", 1, 65, 65), ("C", 0, 95, 97), ("C", 0, 129, 129),
       ("V", 2, 160, 161), ("C", 0, 191, 193), ("V", 3, 224, 225), ("C", 0, 255, 257), ("C", 0, 289, 289),
       ("V", 4, 320, 321), ("C", 0, 351, 353), ("V", 5, 384, 385), ("C", 0, 417, 417), ("C", 0, 449, 449)]

def idx_at(beat):
    for i, n in enumerate(N):
        if abs(n['b'] - beat) < 1e-6: return i
    raise SystemExit(f'no hay nota en el beat {beat}')
def first_ge(beat):
    for i, n in enumerate(N):
        if n['b'] >= beat - 1e-6: return i
    return len(N)
def line_start(T):
    cand = [i for i, n in enumerate(N) if T - 2.0 - 1e-6 <= n['b'] <= T - 0.25 + 1e-6 and n['gap'] >= 1.5 - 1e-6]
    if cand: return cand[-1] + 1
    return first_ge(T - 0.75)

secstart = [idx_at(s[2]) for s in SEC]
texts = []          # tabla de trozos unicos
def tid(t):
    assert len(t) <= 16, t
    if t not in texts: texts.append(t)
    return texts.index(t)

cues = []           # (nota, id)
report = []
for si, (kind, vi, b0, bb) in enumerate(SEC):
    lines = V[vi] if kind == 'V' else C
    starts = [secstart[si]] + [line_start(bb + 8 * k) for k in range(1, 4)]
    end = secstart[si + 1] if si + 1 < len(SEC) else len(N)
    # ajuste fino de cada limite de linea (+-1 nota) segun el conteo de silabas:
    # la anacrusa puede caer en la linea anterior o en la siguiente
    Sl = [sum(syl(c) for c in ch) for ch in lines]
    def cost(k, s):
        nxt = starts[k + 1] if k < 3 else end
        return abs((s - starts[k - 1]) - Sl[k - 1]) + abs((nxt - s) - Sl[k])
    for k in (1, 2, 3):
        best, bc = starts[k], cost(k, starts[k])
        for cand in (starts[k] - 1, starts[k] + 1):
            nxt = starts[k + 1] if k < 3 else end
            if starts[k - 1] + 3 <= cand <= nxt - 3 and cost(k, cand) < bc:
                best, bc = cand, cost(k, cand)
        starts[k] = best
    bounds = starts + [end]
    for k, chunks in enumerate(lines):
        a, b = bounds[k], bounds[k + 1]
        n_notes = b - a
        sy = [syl(c) for c in chunks]
        S = sum(sy)
        acc = 0
        info = []
        for c, s_ in zip(chunks, sy):
            ni = a + (acc * n_notes) // S
            cues.append([ni, tid(c)])
            info.append((c, ni))
            acc += s_
        report.append((si, kind, vi, k, a, b, n_notes, S, info))
# monotonia estricta
for j in range(1, len(cues)):
    if cues[j][0] <= cues[j - 1][0]:
        cues[j][0] = cues[j - 1][0] + 1
assert cues[-1][0] < len(N)

if diag:
    for si, kind, vi, k, a, b, nn, S, info in report:
        tag = f'{kind}{vi + 1}' if kind == 'V' else 'C'
        sec_t = SEC[si][2]
        print(f'[{si:2d} {tag} l{k + 1}] notas {a}-{b - 1} (N={nn}, S={S}, dif={nn - S:+d}) beat {N[a]["b"]:.1f}-{N[b - 1]["b"]:.1f} ' +
              ' | '.join(f'{c} @{nname(N[ni]["p"])}{N[ni]["b"]:.1f}' for c, ni in info))
    print('cues', len(cues), 'textos unicos', len(texts))

# ---------------- C ----------------
def freq(p): return int(round(440 * 2 ** ((p - 69) / 12)))
FREQ = [freq(n['p']) for n in N]
DUR = [int(round(n['gap'] * 2)) for n in N]
assert max(DUR) < 256

def arr(vals, per=14):
    rows = []
    for i in range(0, len(vals), per):
        rows.append('  ' + ', '.join(str(v) for v in vals[i:i + per]))
    return ',\n'.join(rows)

ttab = ',\n'.join('  "%s"' % t for t in texts)
dur_total = sum(DUR) / 2 * 60 / BPM

src = r'''// Wellerman (sea shanty) - LCD I2C 16x2 + buzzer pasivo
// Melodia convertida de wellerman.mid.mid (linea mas aguda del piano), letra en el LCD.
// Idea del buzzer con tone() en D12: Roco (Rocoblog)
// https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html
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
#define BPM        @BPM@     // tempo del MIDI; bajalo para que suene mas lento
#define SIN_TEXTO  255

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// ---- Melodia: frecuencia en Hz y duracion en unidades de media negra ----
const uint16_t FREQ[] PROGMEM = {
@FREQ@
};
const uint8_t DUR[] PROGMEM = {
@DUR@
};

// ---- Letra: trozos de hasta 16 caracteres ----
const char TEXTO[][17] PROGMEM = {
@TEXTOS@
};

// ---- En la nota CUE_NOTA[k] aparece el trozo CUE_TEXTO[k] ----
const uint16_t CUE_NOTA[] PROGMEM = {
@CNOTA@
};
const uint8_t CUE_TEXTO[] PROGMEM = {
@CTEXTO@
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
'''
src = (src.replace('@BPM@', str(BPM)).replace('@FREQ@', arr(FREQ)).replace('@DUR@', arr(DUR, 24))
          .replace('@TEXTOS@', ttab).replace('@CNOTA@', arr([c[0] for c in cues], 16))
          .replace('@CTEXTO@', arr([c[1] for c in cues], 24)))
os.makedirs(os.path.dirname(out), exist_ok=True)
open(out, 'w', newline='\n').write(src)
print('escrito', out, '| duracion aprox', round(dur_total), 's | flash datos ~', len(N) * 3 + len(texts) * 17 + len(cues) * 3, 'bytes')
