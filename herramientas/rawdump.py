import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from midiparse import parse, nname

def notes_with_vel(path):
    fmt, ppq, tracks = parse(path)
    ev = sorted(tracks[0]['ev'], key=lambda e: (e[0], {'off': 0, 'on': 1, 'vel': 2}.get(e[1], 3)))
    ons = {}; out = []; lastvel = {}
    for t, kind, a, b in ev:
        if kind == 'on': ons.setdefault(a, []).append([t, None])
        elif kind == 'vel': ons[a][-1][1] = b
        elif kind == 'off' and ons.get(a):
            s, v = ons[a].pop(0); out.append((s, t, a, v))
    out.sort()
    return ppq, out

if __name__ == '__main__':
    path = sys.argv[1]; b0 = float(sys.argv[2]); b1 = float(sys.argv[3])
    ppq, notes = notes_with_vel(path)
    for s, e, p, v in notes:
        if b0 <= s / ppq < b1:
            print(f'{s/ppq:7.3f} -> {e/ppq:7.3f}  len={(e-s)/ppq:5.2f}  {nname(p):4s} vel={v}')
