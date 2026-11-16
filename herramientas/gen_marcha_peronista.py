import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rawdump import notes_with_vel
from midiparse import nname

midi, out = sys.argv[1], sys.argv[2]
diag = len(sys.argv) > 3
ppq, notes = notes_with_vel(midi)
BPM, Q, VEL_MIN = 120, 12, 10

# ---------------- melodia (igual que gen_marcha3.py) ----------------
mel = [(s / ppq, e / ppq, p) for s, e, p, v in notes if v >= VEL_MIN]
by = {}
for s, e, p in mel:
    k = round(s * Q)
    if k not in by or p > by[k][2]: by[k] = (s, e, p)
starts = sorted(by)
FREQ, IOI, SND, PITCH = [], [], [], []
for i, k in enumerate(starts):
    s, e, p = by[k]
    ln = max(1, round((e - s) * Q))
    ioi = (starts[i + 1] - k) if i + 1 < len(starts) else ln
    FREQ.append(int(round(440 * 2 ** ((p - 69) / 12)))); IOI.append(ioi); SND.append(min(ln, ioi)); PITCH.append(p)
NN = len(FREQ)

# ---------------- letra: trozos <= 16 caracteres (sin acentos: el LCD no los tiene) ----------------
A = [["Los muchachos", "peronistas"], ["Todos unidos", "triunfaremos"], ["Y como siempre", "daremos"],
     ["Un grito de", "corazon"], ["Viva Peron!", "Viva Peron!"]]
B = [["Por ese gran", "argentino"], ["Que se supo", "conquistar"], ["A la gran masa", "del pueblo"],
     ["Combatiendo", "al capital"]]
C = [["Peron, Peron!", "Que grande sos"], ["Mi general!", "Cuanto vales"], ["Peron, Peron,", "gran conductor!"],
     ["Sos el primer", "trabajador!"]]
V3 = [["Por los", "principios", "sociales"], ["Que Peron ha", "establecido"], ["El pueblo entero", "esta unido"],
      ["Y grita de", "corazon"], ["Viva Peron,", "Viva Peron!"]]
B2 = [["Por ese gran", "argentino"], ["Que trabajo", "sin cesar"], ["Para que reine", "en el pueblo"],
      ["El amor y", "la igualdad"]]
V5 = [["Imitemos", "el ejemplo"], ["De este varon", "argentino"], ["Y siguiendo", "su camino"],
      ["Gritemos de", "corazon"], ["Viva Peron,", "Viva Peron!"]]
# la pasada original de D tenia 4 trozos en una sola linea; se separa en dos versos para calzar con las 4 frases de la melodia
D = [["Por esa", "Argentina grande"], ["con que", "San Martin sono"], ["Es la realidad", "efectiva"],
     ["Que debemos", "a Peron"]]

# Notas del MIDI: pasada rapida 0-120 (A, B, C, C), pasada lenta 121-223 (A, B, C).
# La melodia rapida se usa dos veces (estrofa 1 y 2) y la lenta una vez (estrofa 3, final).
# seccion = (nombre, nota_inicio, nota_fin, lineas, cola); cola = notas repetidas al final que sostienen la ultima silaba
PASADAS = [
    [('A', 0, 38, A, 0), ('B', 38, 70, B, 1), ('C', 70, 96, C, 0), ('C', 96, 121, C, 0)],
    [('V3', 0, 38, V3, 0), ('B2', 38, 70, B2, 1), ('C', 70, 96, C, 0), ('C', 96, 121, C, 0)],
    [('V5', 121, 160, V5, 0), ('D', 160, 194, D, 3), ('C', 194, 224, C, 0)],
]

def syl_word(w):
    w = w.lower()
    if w == 'y': return 1
    w = re.sub(r'(?<=[gq])u(?=[eiéí])', '', w)
    weak = 'iuü'
    n = 0
    for g in re.findall(r'[aeiouáéíóúü]+', w):
        n += 1
        for a, b in zip(g, g[1:]):
            dip = (a in weak and (b in 'aeoáéóiuü')) or (b in weak and (a in 'aeoáéóiuü'))
            if not dip: n += 1
    return n
def syl(chunk): return sum(syl_word(re.sub(r"[^\wáéíóúüñ]", '', w.lower())) for w in chunk.split())

# ---------------- alineacion por seccion (programacion dinamica) ----------------
rest = [0] + [max(0, starts[a_] - (starts[a_ - 1] + SND[a_ - 1])) for a_ in range(1, NN)]
MOTIVOS = ([74, 72, 71, 72], [74, 72, 71, 69])   # re do si do / re do si la: abren versos del estribillo
def bonus_for(nm):
    out_ = [0.0]
    for a_ in range(1, NN):
        v = min(rest[a_], 12) / 12 + max(0, min(IOI[a_ - 1], 36) - 12) / 24
        if IOI[a_ - 1] >= 18 and a_ >= 2 and IOI[a_ - 2] >= 18: v += 0.3     # 2da de dos notas largas seguidas
        if nm == 'C' and PITCH[a_:a_ + 4] in MOTIVOS: v += 3.0
        out_.append(v)
    return out_
LAM = 1.5
assert rest[121] >= 60, ('no hay silencio largo antes de la pasada lenta', rest[121])

texts, cues = [], []
def tid(t):
    assert len(t) <= 16, t
    if t not in texts: texts.append(t)
    return texts.index(t)

PASS_TAB = []     # (nota_desde, nota_hasta, cue_desde, cue_hasta)
total_lines = 0
for pi, secs in enumerate(PASADAS):
    pbounds, peff, plines, pls = [], [], [], []
    for name, na, nb0, lines, cola in secs:
        nb = nb0 - cola
        bonus = bonus_for(name)
        Ssec = [sum(syl(c) for c in ch) for ch in lines]
        scale = (nb - na) / sum(Ssec)
        T = [x * (scale if scale < 0.9 else 1.0) for x in Ssec]
        m = len(lines); INF = 1e18
        f = [[INF] * (nb + 1) for _ in range(m + 1)]; pv = [[-1] * (nb + 1) for _ in range(m + 1)]
        f[0][na] = 0
        for j in range(1, m + 1):
            for e in range(na + 2, nb + 1):
                best, bb = INF, -1
                for s0 in range(max(na, e - 14), e - 1):
                    if f[j - 1][s0] >= INF: continue
                    c = f[j - 1][s0] + abs((e - s0) - T[j - 1]) - (LAM * bonus[s0] if j > 1 else 0)
                    if c < best: best, bb = c, s0
                f[j][e], pv[j][e] = best, bb
        cut = [nb]
        for j in range(m, 0, -1): cut.append(pv[j][cut[-1]])
        cut = cut[::-1]
        pbounds += cut[:-1]; pls += Ssec; plines += lines
        peff += cut[1:-1] + [nb]
        if diag: print(f'[P{pi + 1} {name}] notas {na}-{nb0 - 1} cola={cola} N={nb - na} S={sum(Ssec)} escala {scale:.2f}')
    c0 = len(cues)
    pc = []
    for j, ch in enumerate(plines):
        a, b = pbounds[j], peff[j]
        n_notes = b - a
        sy = [syl(c) for c in ch]; tot = sum(sy); acc = 0
        for c, s_ in zip(ch, sy):
            pc.append([a + (acc * n_notes) // tot, tid(c)])
            acc += s_
    for k in range(1, len(pc)):                      # monotono dentro de cada pasada
        if pc[k][0] <= pc[k - 1][0]: pc[k][0] = pc[k - 1][0] + 1
    n0, n1 = secs[0][1], secs[-1][2]
    assert pc[0][0] == n0 and pc[-1][0] < n1, (pi, pc[0], pc[-1], n0, n1)
    cues += pc
    PASS_TAB.append((n0, n1, c0, len(cues)))
    total_lines += len(plines)
    if diag:
        j = 0
        for name, na, nb, lines, cola in secs:
            print(f'--- pasada {pi + 1} / {name}')
            for _ in lines:
                x, y = pbounds[j], peff[j]
                print(f'  l{j + 1:2d} notas {x:3d}-{y - 1:3d} N={y - x:2d} S={pls[j]:2d}  beats {starts[x] / Q:6.1f}-{starts[min(y, NN - 1)] / Q:6.1f}  '
                      + ' '.join(nname(PITCH[k]) for k in range(x, min(y, x + 9))) + ' ... | ' + ' / '.join(plines[j]))
                j += 1
print('lineas', total_lines, '| notas', NN, '| cues', len(cues), '| textos unicos', len(texts), '| pasadas', PASS_TAB)

# duracion total estimada
dur = 0.0
for n0, n1, _, _ in PASS_TAB:
    dur += (starts[n1 - 1] + IOI[n1 - 1] - starts[n0]) / Q * 60 / BPM
print('duracion aprox', round(dur), 's')

# ---------------- C ----------------
def arr(vals, per): return ',\n'.join('  ' + ', '.join(str(v) for v in vals[i:i + per]) for i in range(0, len(vals), per))
ttab = ',\n'.join('  "%s"' % t for t in texts)
ptab = ',\n'.join('  {%d, %d, %d, %d}' % p for p in PASS_TAB)
src = r'''// Marcha Peronista - LCD I2C 16x2 + buzzer pasivo, con letra sincronizada (3 estrofas)
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
#define BPM        @BPM@     // tempo del MIDI; bajalo para que suene mas lento
#define SIN_TEXTO  255

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// ---- Melodia: frecuencia (Hz), tiempo hasta la siguiente nota (IOI) y cuanto suena (SND) ----
// IOI y SND estan en doceavos de negra; lo que IOI tiene de mas que SND es silencio.
const uint16_t FREQ[] PROGMEM = {
@FREQ@
};
const uint8_t IOI[] PROGMEM = {
@IOI@
};
const uint8_t SND[] PROGMEM = {
@SND@
};

// ---- Letra: trozos de hasta 16 caracteres ----
const char TEXTO[][17] PROGMEM = {
@TEXTOS@
};

// ---- En la nota CUE_NOTA[k] aparece el trozo CUE_TEXTO[k] (las notas se repiten entre pasadas) ----
const uint16_t CUE_NOTA[] PROGMEM = {
@CNOTA@
};
const uint8_t CUE_TEXTO[] PROGMEM = {
@CTEXTO@
};

// ---- Pasadas: {primera nota, ultima nota + 1, primer cue, ultimo cue + 1} ----
const uint16_t PASADAS[][4] PROGMEM = {
@PASADAS@
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
'''
src = (src.replace('@BPM@', str(BPM)).replace('@FREQ@', arr(FREQ, 14)).replace('@IOI@', arr(IOI, 24))
          .replace('@SND@', arr(SND, 24)).replace('@TEXTOS@', ttab).replace('@PASADAS@', ptab)
          .replace('@CNOTA@', arr([c[0] for c in cues], 16)).replace('@CTEXTO@', arr([c[1] for c in cues], 24)))
os.makedirs(os.path.dirname(out), exist_ok=True)
open(out, 'w', newline='\n').write(src)
print('escrito', out)
