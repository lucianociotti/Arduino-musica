import struct, sys

NOTES = ['C','C#','D','D#','E','F','F#','G','G#','A','A#','B']
def nname(n): return NOTES[n % 12] + str(n // 12 - 1)

def vlq(d, i):
    v = 0
    while True:
        b = d[i]; i += 1
        v = (v << 7) | (b & 0x7F)
        if not b & 0x80: return v, i

def parse(path):
    d = open(path, 'rb').read()
    assert d[:4] == b'MThd', 'no es MIDI'
    fmt, ntr, div = struct.unpack('>HHH', d[8:14])
    i = 14
    tracks = []
    while i < len(d) and len(tracks) < ntr:
        cid = d[i:i+4]; ln = struct.unpack('>I', d[i+4:i+8])[0]
        body = d[i+8:i+8+ln]; i += 8 + ln
        if cid != b'MTrk': continue
        j = 0; t = 0; run = None
        ev = []   # (tick, kind, a, b)
        name = ''
        while j < len(body):
            dt, j = vlq(body, j); t += dt
            s = body[j]
            if s == 0xFF:
                typ = body[j+1]; l, k = vlq(body, j+2); data = body[k:k+l]; j = k + l
                if typ == 0x03 and not name: name = data.decode('latin1', 'replace')
                elif typ == 0x51: ev.append((t, 'tempo', int.from_bytes(data, 'big'), 0))
                elif typ == 0x58: ev.append((t, 'timesig', data[0], 2 ** data[1]))
                elif typ == 0x59: ev.append((t, 'key', data[0] - 256 if data[0] > 127 else data[0], data[1]))
                elif typ in (0x05, 0x01) and typ == 0x05: ev.append((t, 'lyric', data.decode('latin1', 'replace'), 0))
                elif typ == 0x2F: break
                continue
            if s in (0xF0, 0xF7):
                l, k = vlq(body, j+1); j = k + l; continue
            if s & 0x80: run = s; j += 1
            st = run; hi = st & 0xF0; ch = st & 0x0F
            if hi in (0xC0, 0xD0):
                a = body[j]; j += 1
                if hi == 0xC0: ev.append((t, 'prog', ch, a))
            else:
                a, b = body[j], body[j+1]; j += 2
                if hi == 0x90 and b > 0:
                    ev.append((t, 'on', a, ch)); ev.append((t, 'vel', a, b))
                elif hi == 0x80 or (hi == 0x90 and b == 0): ev.append((t, 'off', a, ch))
        tracks.append({'name': name, 'ev': ev})
    return fmt, div, tracks

def info(path):
    fmt, div, tracks = parse(path)
    print('==', path)
    print('formato', fmt, 'ppq', div, 'pistas', len(tracks))
    for k, tr in enumerate(tracks):
        ons = [(t, a, c) for t, kind, a, c in tr['ev'] if kind == 'on']
        tempos = [(t, round(60000000 / a, 1)) for t, kind, a, _ in tr['ev'] if kind == 'tempo']
        ts = [(t, a, b) for t, kind, a, b in tr['ev'] if kind == 'timesig']
        ks = [(t, a, b) for t, kind, a, b in tr['ev'] if kind == 'key']
        pg = sorted({(c, a) for t, kind, c, a in tr['ev'] if kind == 'prog'})
        lyr = [a for t, kind, a, _ in tr['ev'] if kind == 'lyric']
        # polifonia maxima
        act = {}; mx = 0
        for t, kind, a, c in sorted(tr['ev'], key=lambda e: (e[0], 0 if e[1] == 'off' else 1)):
            if kind == 'on': act[(a, c)] = act.get((a, c), 0) + 1
            elif kind == 'off' and (a, c) in act:
                act[(a, c)] -= 1
                if act[(a, c)] == 0: del act[(a, c)]
            mx = max(mx, len(act))
        line = f'  pista {k}: "{tr["name"]}" notas={len(ons)}'
        if ons:
            ns = [a for _, a, _ in ons]
            line += f' rango={nname(min(ns))}..{nname(max(ns))} polifonia_max={mx} canales={sorted({c for _,_,c in ons})} t_fin={max(t for t,_,_ in ons)}'
        print(line)
        if tempos: print('     tempo(bpm):', tempos[:5])
        if ts: print('     compas:', ts[:3])
        if ks: print('     tonalidad(sostenidos/bemoles, menor?):', ks[:3])
        if pg: print('     programa(canal,instr):', pg[:4])
        if lyr: print('     letra_en_midi:', len(lyr), 'eventos; muestra:', lyr[:8])

if __name__ == '__main__':
    for p in sys.argv[1:]:
        info(p)
