import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from midiparse import parse, nname

def skyline(path):
    fmt, ppq, tracks = parse(path)
    ev = tracks[0]['ev']
    # pares on/off por nota
    ons = {}
    notes = []  # (start, end, pitch)
    for t, kind, a, c in sorted(ev, key=lambda e: (e[0], 0 if e[1] == 'off' else 1)):
        if kind == 'on': ons.setdefault(a, []).append(t)
        elif kind == 'off' and ons.get(a):
            s = ons[a].pop(0); notes.append((s, t, a))
    notes.sort()
    # cluster por onset: nota mas aguda
    by = {}
    for s, e, p in notes:
        if s not in by or p > by[s][1]: by[s] = (e, p)
    starts = sorted(by)
    line = []
    for k, s in enumerate(starts):
        e, p = by[s]
        nxt = starts[k+1] if k + 1 < len(starts) else e
        line.append((s, min(e, nxt) if nxt > s else e, p, nxt - s))
    return ppq, notes, line, tracks

if __name__ == '__main__':
    path = sys.argv[1]; n = int(sys.argv[2]) if len(sys.argv) > 2 else 60
    ppq, notes, line, tr = skyline(path)
    print('notas totales', len(notes), 'onsets', len(line), 'ppq', ppq)
    # histograma de duraciones (en beats) de las notas de la linea
    from collections import Counter
    cnt = Counter(round((e - s) / ppq, 3) for s, e, p, g in line)
    print('duraciones top:', cnt.most_common(8))
    gaps = Counter(round(g / ppq, 3) for s, e, p, g in line)
    print('separaciones top:', gaps.most_common(8))
    out = []
    for s, e, p, g in line[:n]:
        out.append(f'{s/ppq:.2f}:{nname(p)}x{(e-s)/ppq:.2f}')
    print(' '.join(out))
    # firma de tramos: marcar compas de inicio de cada 8 compases
    print('fin ultimo evento (beats):', max(e for s, e, p, g in line) / ppq)
