#!/usr/bin/env python3
"""PrePre Vol. 8 / "Pure 8" Tomba demo: decoder for the in-exe text (event titles and dialogue). See docs/PREPRE8.md.
Reads only; prints decoded text to stdout. Do not commit the output.

  python3 -I tools/prepre8_text.py TOMBA.EXE            all blocks
  python3 -I tools/prepre8_text.py TOMBA.EXE --raw      also show offsets and unknown codes as {xx}

Format (found 2026-10-30): text is u16 glyph indices, terminated by 0xFFFE; 0xFFFD/0xFFFC/0xFFFB/0xFFF8 and 0x40xx are
page/wait/speaker controls in dialogue. Each block is preceded by a glyph table of 5-byte records (u, v, w, h, flag),
so the code IS the record index. 10x10 kana cells (25 per row), then 12x12 kanji (21 per row) sorted by on-reading
(gojuon order), then two 18x10 cells ("100", "1000").
  - Event-title table 0x9A1FC (372 entries, font page = the VRAM snapshot SYSTEM/SYS008.000 at x=768 4bpp):
    titles 0x9A940..0x9B92A.  Kana: hiragana 0x3E.., katakana 0x75.., dakuten 0xAC, handakuten 0xAD, kanji 0xAE..0x171.
  - Dialogue tables 0x933CC, 0x94860, 0x96674, 0x9B930, 0x9CDC4 (font at v=0x50 of a VRAM page; 0x9B930 = TIM001).
    Kana: hiragana 0x31.., katakana 0x68.., dakuten 0x2F, handakuten 0x30, kanji from 0x95 (set differs per table;
    only the TIM001 set, table 0x9B930, is labelled here).
"""
import struct, sys

EXE_FILE_TO_VRAM = 0x80010000 - 0x800
HIRA = "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん" + "ぁぃぅぇぉっゃゅょ"
KATA = "アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン" + "ァィゥェォッャュョ"
DAKU = dict(zip("かきくけこさしすせそたちつてとはひふへほウ", "がぎぐげござじずぜぞだぢづでどばびぶべぼヴ"))
DAKU.update(zip("カキクケコサシスセソタチツテトハヒフヘホ", "ガギグゲゴザジズゼゾダヂヅデドバビブベボ"))
HANDAKU = dict(zip("はひふへほハヒフヘホ", "ぱぴぷぺぽパピプペポ"))

# Event-title font (table 0x9A1FC). Symbols row 0/1 are ASCII order from 0x20 with Japanese punctuation after '}'.
EV_SYM = " !\"#$%&'()*+,-./012345678" + "9:;<=>?[¥]^_`{|}…「」、。・、〜ー" + "▼←→↑↓○×△□‼AP"
# 196 kanji 0xAE..0x171 in on-reading order; '〓' = not identified (12x12 glyph unclear).
EV_KANJI = (
    "ⅠⅡⅢ悪案飲〓影〓円王黄億下何家果歌花会解怪海〓割寒間岩願危気紀議救泣魚強橋郷玉金銀空窟形〓穴件見幻"
    "固湖光口行黒〓材作山始姉子師思止死〓歯事字持時実車主取手樹集出助女勝少床晶消笑上城色食森神身進人吹水"
    "数世性生青石赤雪先占戦側賊族村体大達脱誰探団知地中仲挑町長沈通停底鉄天電渡塔島盗当動道銅〓読届内入年"
    "〓〓白箱発犯飛不風物文捕宝方法忘本魔妹万夢霧名明木門〓友溶〓落卵嵐里立旅料良力緑涙路老話〓迷")
# Dialogue font of table 0x9B930 (TIM001), 94 kanji 0x95..0xF2.
DLG_SYM = "?!  ()0123456789…「」、。・、〜ー" + "▼←→↑↓○×△□‼AP" + "ンァィゥェォッャュョ"
DLG_KANJI = ("哀悪一隠泳奥押屋下何可会解回楽気協強教撃見元言向攻考行高込散仕使始子枝事持自手集住出小信新森人水数先"
             "前全相側足速大誰知長転登奴盗道入年飛表敷部物分聞方魔無木目戻貰用来卵利裏里溜力録老話村屋")

EV_TABLE, EV_TEXT, EV_END = 0x9A1FC, 0x9A940, 0x9B92A
DLG_BLOCKS = [0x933CC, 0x94860, 0x96674, 0x9B930, 0x9CDC4]


def ev_map():
    m = {i: c for i, c in enumerate(EV_SYM)}
    m.update({0x3E + i: c for i, c in enumerate(HIRA)})
    m.update({0x75 + i: c for i, c in enumerate(KATA)})
    m[0xAC], m[0xAD] = "゛", "゜"
    m.update({0xAE + i: c for i, c in enumerate(EV_KANJI)})
    m[0x172], m[0x173] = "100", "1000"
    return m


def dlg_map(labelled):
    m = {i: c for i, c in enumerate(DLG_SYM)}
    m.update({0x31 + i: c for i, c in enumerate(HIRA)})
    m.update({0x68 + i: c for i, c in enumerate(KATA)})
    m[0x2F], m[0x30] = "゛", "゜"
    if labelled:
        m.update({0x95 + i: c for i, c in enumerate(DLG_KANJI)})
    return m


def join(chars):
    out = []
    for c in chars:
        if c == "゛" and out and out[-1] in DAKU:
            out[-1] = DAKU[out[-1]]
        elif c == "゜" and out and out[-1] in HANDAKU:
            out[-1] = HANDAKU[out[-1]]
        else:
            out.append(c)
    return "".join(out)


def table_end(d, s):
    o = s
    while d[o + 2] in (10, 12, 18) and d[o + 3] in (10, 12):
        o += 5
    return o


def strings(d, start, end, m):
    """Yield (offset, text) for 0xFFFE-terminated u16 strings; controls shown as <xxxx>."""
    o, cur, st = start, [], start
    while o < end:
        v = struct.unpack_from("<H", d, o)[0]
        o += 2
        if v == 0xFFFE:
            yield st, join(cur)
            cur, st = [], o
        elif v == 0xFFFF or (v == 0 and not cur):
            st = o
        elif v == 0xFFFB:             # speaker/window header: three parameter halfwords follow
            a, b, c = struct.unpack_from("<HHH", d, o)
            o += 6
            cur.append("<FFFB %X %X %X>" % (a, b, c))
        elif v >= 0xFFF0 or (v >> 8) == 0x40:
            cur.append("<%04X>" % v)
            if v in (0xFFFC, 0xFFF8):
                yield st, join(cur)
                cur, st = [], o
        else:
            cur.append(m.get(v, "{%x}" % v))
    if cur:
        yield st, join(cur)


def main():
    d = open(sys.argv[1], "rb").read()
    raw = "--raw" in sys.argv
    print("== event titles (table 0x%X)" % EV_TABLE)
    for o, s in strings(d, EV_TEXT, EV_END, ev_map()):
        print(("%08X  " % (o + EXE_FILE_TO_VRAM) if raw else "") + s)
    for i, t in enumerate(DLG_BLOCKS):
        s0 = (table_end(d, t) + 1) & ~1
        lab = (d[t:t + 1280] == d[0x9B930:0x9B930 + 1280])   # same TIM001 layout as 0x9B930
        e = DLG_BLOCKS[i + 1] if i + 1 < len(DLG_BLOCKS) and DLG_BLOCKS[i + 1] > s0 else s0 + 0x1400
        bad, o = 0, s0
        while o < e and bad < 3:      # text runs until three non-code halfwords
            v = struct.unpack_from("<H", d, o)[0]
            bad = 0 if (v < 0x180 or v >= 0xFFF0 or (v >> 8) == 0x40) else bad + 1
            o += 2
        print("== dialogue (table 0x%X, text 0x%X..0x%X)%s" % (t, s0, o, "" if lab else " kanji unlabelled (font page not in the file tree)"))
        for so, s in strings(d, s0, o - 6, dlg_map(lab)):
            if s.strip():
                print(("%08X  " % (so + EXE_FILE_TO_VRAM) if raw else "") + s)


if __name__ == "__main__":
    main()
