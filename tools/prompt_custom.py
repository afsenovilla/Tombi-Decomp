"""Custom button icons (--prompts custom --prompt-colors ...): presets and the glyph algorithm of
port2/shims/prompts.c, for the PC launcher's "Customise buttons" dialog (the Android launcher has a Java copy in
LauncherActivity.java).  Positions are a south, b east, x west, y north (as --prompt-map); each has a disc and a
letter colour; sym picks the symbols drawn: xbox (A B X Y), nintendo (B A Y X: Nintendo / AYN Thor), ps (shapes)."""

POS = "abxy"   # south, east, west, north
SYMS = ("xbox", "nintendo", "ps")

# (key, Spanish name, English name, sym, [(disc, letter) for south, east, west, north])
PRESETS = [
    ("thor-black", "Thor negro", "Thor black", "nintendo", [(0x181818, 0xFFFFFF)] * 4),
    ("thor-white", "Thor blanco", "Thor white", "nintendo", [(0xECECE8, 0x828282)] * 4),
    ("thor", "Thor arcoíris", "Thor rainbow", "nintendo",
     [(0xF2C230, 0xFFFFFF), (0xC8281E, 0xFFFFFF), (0x2E8B4E, 0xFFFFFF), (0x1E3FB0, 0xFFFFFF)]),
    ("xbox", "Xbox", "Xbox", "xbox",
     [(0x292929, 0x3CB043), (0x292929, 0xE03C31), (0x292929, 0x2F6FD6), (0x292929, 0xF5C518)]),
    ("nintendo", "Nintendo", "Nintendo", "nintendo", [(0x292929, 0xFFEFDE)] * 4),
    ("playstation", "PlayStation", "PlayStation", "ps",
     [(0x292929, 0x6B94FF), (0x292929, 0xFF5A52), (0x292929, 0xFF8484), (0x292929, 0x00FF00)]),
    ("gamecube", "GameCube", "GameCube", "xbox",
     [(0x1FAE4B, 0xFFFFFF), (0xD8D8D8, 0x404040), (0xE03C31, 0xFFFFFF), (0xD8D8D8, 0x404040)]),
    ("snes-us", "SNES (América)", "SNES (US)", "nintendo",
     [(0x5B3F9E, 0xFFFFFF), (0x5B3F9E, 0xFFFFFF), (0xB8A8E0, 0x3A2A6E), (0xB8A8E0, 0x3A2A6E)]),
    ("snes-pal", "SNES PAL / Super Famicom", "SNES PAL / Super Famicom", "nintendo",
     [(0xF0C020, 0x5A4000), (0xD0302A, 0xFFE0D8), (0x3A9A50, 0xE0FFE0), (0x3050C0, 0xE0E8FF)]),
    ("famicom", "Famicom", "Famicom", "nintendo",
     [(0xB02828, 0xFFFFFF), (0xB02828, 0xFFFFFF), (0xD8C8A0, 0x7A1E1E), (0xD8C8A0, 0x7A1E1E)]),
    ("pastel", "Pastel", "Pastel", "xbox",
     [(0xA8E6CF, 0x3A3A3A), (0xFFB7B2, 0x3A3A3A), (0xC7CEEA, 0x3A3A3A), (0xFFDAC1, 0x3A3A3A)]),
]
DEFAULT = "a=181818:FFFFFF,b=181818:FFFFFF,x=181818:FFFFFF,y=181818:FFFFFF,sym=nintendo"


def to_arg(sym, cols):
    return ",".join("%s=%06X:%06X" % (POS[i], d, l) for i, (d, l) in enumerate(cols)) + ",sym=" + sym


def from_arg(s):
    sym, cols = "nintendo", [(0x181818, 0xFFFFFF)] * 4
    cols = list(cols)
    for part in (s or "").split(","):
        k, _, v = part.partition("=")
        if k == "sym" and v in SYMS:
            sym = v
        elif len(k) == 1 and k in POS:
            d, _, l = v.partition(":")
            try:
                cols[POS.index(k)] = (int(d, 16), int(l or "FFFFFF", 16))
            except ValueError:
                pass
    return sym, cols


# ---- the glyph (prompts.c draw(), PROMPT_CUSTOM): 12 x 16, disc rows 3..14 ----
DISC_ROWS = ["...BCDDCB...", ".EC666666CE.", ".C66666666C.", "B6666666666B", "C6666666666C", "D6666666666D",
             "D6666666666D", "C6666666666C", "B6666666666B", ".C66666666C.", ".EC666666CE.", "...BCDDCB..."]
RIM = {"B": (206, 198, 165), "C": (165, 156, 140), "D": (140, 132, 115), "E": (255, 239, 222)}
FONT6 = {"A": [0x0C, 0x12, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21], "B": [0x3E, 0x21, 0x21, 0x3E, 0x21, 0x21, 0x21, 0x3E],
         "X": [0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x33, 0x21], "Y": [0x21, 0x33, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C]}


def _rgb(c):
    return ((c >> 16) & 255, (c >> 8) & 255, c & 255)


def _q(c):
    return tuple((v & 248) | (v >> 5) for v in c)   # 5 bits per channel, expanded as the port GPU does


def _mix(a, b, t):
    return tuple(x + (y - x) * t // 100 for x, y in zip(a, b))


def glyph(pos, sym, disc, letter):
    """16 rows of 12 pixels: None (transparent) or (r, g, b)"""
    D, L = _rgb(disc), _rgb(letter)
    img = [[None] * 12 for _ in range(16)]
    for y, row in enumerate(DISC_ROWS):
        for x, ch in enumerate(row):
            img[y + 2][x] = None if ch == "." else "F" if ch == "6" else RIM[ch]
    mark = [[False] * 12 for _ in range(16)]

    def put(x, y):
        if 0 <= x < 12 and 0 <= y < 16:
            mark[y][x] = True
    if sym == "ps":
        if pos == 0:
            for i in range(6):
                put(3 + i, 5 + i); put(8 - i, 5 + i)
        elif pos == 1:
            ring = [0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C]
            for y in range(8):
                for x in range(8):
                    if ring[y] >> (7 - x) & 1:
                        put(2 + x, 4 + y)
        elif pos == 2:
            for i in range(6):
                put(3 + i, 5); put(3 + i, 10); put(3, 5 + i); put(8, 5 + i)
        else:
            for y in range(7):
                put(5 - y // 2, 4 + y); put(6 + y // 2, 4 + y)
            for x in range(2, 10):
                put(x, 10)
    else:
        ch = ("ABXY" if sym == "xbox" else "BAYX")[pos]
        for y, bits in enumerate(FONT6[ch]):
            for x in range(6):
                if bits >> (5 - x) & 1:
                    put(3 + x, 4 + y)
    # shadow, soft edge, letters (letter_fx)
    for y in range(14, -1, -1):
        for x in range(10, -1, -1):
            if mark[y][x] and not mark[y + 1][x + 1] and img[y + 1][x + 1] == "F":
                img[y + 1][x + 1] = "S"
    for y in range(16):
        for x in range(12):
            if img[y][x] == "F" and not mark[y][x] and any(
                    0 <= x + dx < 12 and 0 <= y + dy < 16 and mark[y + dy][x + dx]
                    for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1))):
                img[y][x] = "E2"
    col = {"F": _q(D), "S": _q(_mix(D, (0, 0, 0), 30)), "E2": _q(_mix(D, L, 35))}
    L = _q(L)
    out = [[None] * 12 for _ in range(16)]
    for y in range(16):
        for x in range(12):
            v = L if mark[y][x] else img[y][x]
            out[y][x] = col.get(v, v) if isinstance(v, str) else v
    return [[None] * 12] + out[:15]   # 1 px lower (rows 3..14)


PALETTE = [0x000000, 0x181818, 0x404040, 0x828282, 0xC8C8C8, 0xFFFFFF, 0xFFEFDE, 0xC8281E, 0x7A1E1E, 0xF08030,
           0xF5C518, 0xF2C230, 0x8CD040, 0x3CB043, 0x2E8B4E, 0x20A0A0, 0x40C8F0, 0x2F6FD6, 0x1E3FB0, 0x5B3F9E,
           0xB8A8E0, 0xFF8484, 0xE040A0, 0x8B5A2B]
