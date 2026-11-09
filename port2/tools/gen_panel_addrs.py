#!/usr/bin/env python3
"""Writes port2/runtime/panel_addrs.h: the retail addresses the second-screen panel reads (port2/runtime/panel*.c/.inc),
for each game version the port builds (docs/PORT_VERSIONS_PLAN.md stage U3, "Second-screen panel").

Each address is a symbol of the version's full link plus an offset (the field of a struct: GAME + 0x13, PLAYER + 0xA6;
the table after a named one: SUBSCREEN_COND_OPS + 0xB4), taken from the PROVIDE tables of build/full/<prog>.full.ld
(PAL) and build/scus94236/full/<prog>.full.ld (USA, tools/build_full.py --version usa).  The PAL column must give the
address the panel was written against (checked here: a PAL value that moved stops the script); the USA column is the
same name in the USA link.  With --check, the tables that hold the same bytes on both discs (map graph, menu order,
item tables, the sprite table of the NOW LOADING screen) are compared in the two files, so an anchor that is not the
same table on USA is caught.  Only addresses: no game data.

  python3 port2/tools/gen_panel_addrs.py [--check PAL_DATA_DIR USA_FS_DIR]
"""
import argparse, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
OUT = os.path.join(ROOT, "port2", "runtime", "panel_addrs.h")

# program: (PAL full link, USA full link, PAL file in data/, USA file in the disc's fs, load address PAL, USA)
PROGS = {"main0": ("main0", "scus94236", "EXE/MAIN0.EXE", "SCUS_942.36", 0x80010000 - 0x800, 0x80010000 - 0x800),
         "d0spsub": ("d0spsub", "dspsub", "SYSTEM/D0SPSUB.BIN", "SYSTEM/DSPSUB.BIN", 0x800E8028, 0x800E7388),
         "d0spsub3": ("d0spsub3", "dspsub3", "SYSTEM/D0SPSUB3.BIN", "SYSTEM/DSPSUB3.BIN", 0x800E8028, 0x800E7388)}

# (macro, program, symbol, offset, PAL address, bytes equal on both discs (0: not checked), what it is)
ADDRS = [
    # GAME (include/gamestate.h) and the globals around it
    ("PV_GAME_AREA", "main0", "GAME", 0x00, 0x8009C960, 0, "u16 area"),
    ("PV_GAME_SECTION", "main0", "GAME", 0x02, 0x8009C962, 0, "u16 section"),
    ("PV_GAME_TIME", "main0", "GAME", 0x08, 0x8009C968, 0, "u32 totalTimePlayed"),
    ("PV_GAME_AP", "main0", "GAME", 0x0C, 0x8009C96C, 0, "u32 AP"),
    ("PV_GAME_HEALTH", "main0", "GAME", 0x10, 0x8009C970, 0, "u8 vitality"),
    ("PV_GAME_MAXHEALTH", "main0", "GAME", 0x11, 0x8009C971, 0, "u8 max vitality"),
    ("PV_GAME_LEVEL", "main0", "GAME", 0x13, 0x8009C973, 0, "u8 level"),
    ("PV_GAME_FADE", "main0", "GAME", 0x15, 0x8009C975, 0, "u8 fade control"),
    ("PV_GAME_LIVES", "main0", "GAME", 0x20, 0x8009C980, 0, "u8 lives"),
    ("PV_GAME_SUIT", "main0", "GAME", 0x30, 0x8009C990, 0, "u8 suit"),
    ("PV_GCD_WARP_AREA", "main0", "GCD", 0x94, 0x8009CD94, 0, "u16 warp area"),
    ("PV_GCD_WARP_SECTION", "main0", "GCD", 0x96, 0x8009CD96, 0, "u16 warp section"),
    ("PV_GCD_EXPLEV", "main0", "GCD", 0x98, 0x8009CD98, 0, "u8[3] exp levels"),
    ("PV_GCD_EXPBAR", "main0", "GCD", 0x9B, 0x8009CD9B, 0, "u8[3] exp bars"),
    ("PV_GCD_CDA0", "main0", "GCD", 0xA0, 0x8009CDA0, 0, "u16 (warp)"),
    ("PV_GCD_NO_MENU", "main0", "GCD", 0xA2, 0x8009CDA2, 0, "u8 select menu disabled"),
    ("PV_SCRIPT_FLAGS", "main0", "SCRIPT_FLAGS", 0, 0x8009CDA4, 0, "u8[256] event flags"),
    ("PV_GAME_CEF7", "main0", "GAME", 0x597, 0x8009CEF7, 0, "u8 item 10 in use"),
    ("PV_GAME_CEF8", "main0", "GAME", 0x598, 0x8009CEF8, 0, "u8 item 20 in use"),
    ("PV_GAME_CFF8", "main0", "GAME", 0x698, 0x8009CFF8, 0, "u8 item 12 in use"),
    ("PV_GAME_CFFB", "main0", "GAME", 0x69B, 0x8009CFFB, 0, "u8 item 132 in use"),
    ("PV_GAME_D006", "main0", "GAME", 0x6A6, 0x8009D006, 0, "u8 item 96 in use"),
    ("PV_GAME_PANTS_OK", "main0", "GAME", 0x730, 0x8009D090, 0, "u8 0xFF: pants usable"),
    ("PV_GAME_QTY", "main0", "GAME", 0x744, 0x8009D0A4, 0, "u8[item] quantities"),
    ("PV_GAME_D0E6", "main0", "GAME", 0x786, 0x8009D0E6, 0, "u8 (message 2/3 of the item menu)"),
    ("PV_GAME_INV_ITEM", "main0", "GAME", 0x844, 0x8009D1A4, 0, "u8[256] inventory"),
    ("PV_GAME_INV_COUNT", "main0", "GV2", 0x04, 0x8009D2A4, 0, "u16 inventory count"),
    ("PV_GAME_INV_MODE", "main0", "GV2", 0x06, 0x8009D2A6, 0, "u16 inventory sort mode"),
    ("PV_GAME_PLAYER_MODE", "main0", "GV2", 0x10, 0x8009D2B0, 0, "u8 player mode"),
    ("PV_GAME_WEAPON", "main0", "GV2", 0x12, 0x8009D2B2, 0, "u8 equipped weapon"),
    ("PV_GAME_PANTS", "main0", "GV2", 0x13, 0x8009D2B3, 0, "u8 equipped pants"),
    ("PV_GAME_MAPREG", "main0", "GV2", 0x24, 0x8009D2C4, 0, "u16[12] revealed map regions"),
    ("PV_GVARS_LANDED", "main0", "GVARS", 0x0A, 0x8009C93A, 0, "u8 player landed"),
    ("PV_GVARS_C93F", "main0", "GVARS", 0x0F, 0x8009C93F, 0, "u8 fade effect"),
    ("PV_GVARS_USE_ITEM", "main0", "GVARS", 0x10, 0x8009C940, 0, "u8 use item pending"),
    ("PV_GVARS_USE_ITEM_ID", "main0", "GVARS", 0x11, 0x8009C941, 0, "u8 use item id"),
    # PLAYER (the player object) and the object before it
    ("PV_PLAYER", "main0", "PLAYER", 0x00, 0x800A6038, 0, "u8 PLAYER +0"),
    ("PV_PLAYER_STATE", "main0", "PLAYER", 0x04, 0x800A603C, 0, "u8 state"),
    ("PV_PLAYER_SUBSTATE", "main0", "PLAYER", 0x05, 0x800A603D, 0, "u8 substate"),
    ("PV_PLAYER_STEP", "main0", "PLAYER", 0x06, 0x800A603E, 0, "u8 step"),
    ("PV_PLAYER_X", "main0", "PLAYER", 0x12, 0x800A604A, 0, "s16 x"),
    ("PV_PLAYER_Y", "main0", "PLAYER", 0x16, 0x800A604E, 0, "s16 y"),
    ("PV_PLAYER_FACING", "main0", "PLAYER", 0xA6, 0x800A60DE, 0, "u8 side he faces"),
    ("PV_OBJ_5462", "main0", "PLAYER", -0xBD6, 0x800A5462, 0, "u8 (updatePauseMenu's check)"),
    # MAIN0 data
    ("PV_UI_SPRITES", "main0", "BUTTON_CONFIGS", 0x30, 0x800787F0, 36, "{u16 u v w h; s16 clut} NOW LOADING sprites"),
    ("PV_NEG_SIN_TABLE", "main0", "NEG_SIN_TABLE", 0, 0x8007A3F0, 512, "s16[]"),
    ("PV_HUD_STATE", "main0", "HUD_STATE", 0, 0x800B1410, 0, "the HUD object"),
    ("PV_HUD_FRAME_8", "main0", "A2150", 0x60, 0x800121B0, 0, "frame pointer, weapon 8"),
    ("PV_HUD_FRAME_9", "main0", "A2150", 0x64, 0x800121B4, 0, "frame pointer, weapon 9"),
    ("PV_HUD_FRAME_0", "main0", "A2150", 0x68, 0x800121B8, 0, "frame pointer, weapon 0"),
    ("PV_HUD_FRAME_5", "main0", "A2150", 0x7C, 0x800121CC, 0, "frame pointer, weapon 5"),
    ("PV_HUD_FRAME_6", "main0", "A2150", 0x80, 0x800121D0, 0, "frame pointer, weapon 6"),
    ("PV_HUD_FRAME_7", "main0", "A2150", 0x84, 0x800121D4, 0, "frame pointer, weapon 7"),
    # the sub-screen overlay (D0SPSUB / DSPSUB): map graph, names, icons
    ("PV_MAP_NODES", "d0spsub", "MAP_NODES", 0, 0x800FD418, 203 * 8, "{s16 x y z pad}[203]"),
    ("PV_MAP_AREA_ROUTES", "d0spsub", "MAP_AREA_ROUTES", 0, 0x800FDA70, 0, "route list pointers [12]"),
    ("PV_MAP_LINKS", "d0spsub", "MAP_LINKS", 0, 0x800FDAB4, 154 * 6, "u8[154][6]"),
    ("PV_MAP_ROOM_LINK", "d0spsub", "MAP_ROOM_LINK", 0, 0x800FE064, 0, "s16 *[20]"),
    ("PV_EVENT_ICONS", "d0spsub", "SUBSCREEN_DIGIT_FRAMES", 0x2C, 0x800FE974, 0, "frame record pointers [192]"),
    ("PV_SUBSCREEN_CLUT_IMAGE", "d0spsub", "SUBSCREEN_CLUT_IMAGE", 0, 0x800F18E0, 0, "48 x 31 palettes for (288, 480)"),
    ("PV_EVENT_ORDER", "d0spsub", "SUBSCREEN_EVENT_ORDER", 0, 0x801086F0, 256, "u8[256] events of the menu"),
    ("PV_A14_AREA_NAMES", "d0spsub", "MAP_AREA14_AREA_NAMES", 0, 0x80108B78, 0, "string pointers [24]"),
    ("PV_AREA_NAMES", "d0spsub", "MAP_AREA_NAMES", 0, 0x80108BE4, 0, "string pointers [12]"),
    ("PV_A14_ROOM_NAMES", "d0spsub", "MAP_AREA14_ROOM_NAMES", 0, 0x80108DDC, 0, "string pointers [24]"),
    ("PV_ROOM_NAMES", "d0spsub", "MAP_ROOM_NAMES", 0, 0x80108F8C, 0, "string pointer table pointers [12]"),
    ("PV_EVENT_NAMES", "d0spsub", "EVENT_NAME_STRINGS", 0, 0x801093C4, 0, "string pointers [195]"),
    ("PV_ITEM_NAME_INDEX", "d0spsub", "ITEM_NAME_INDEX", 0, 0x801096D4, 200, "u8[item]"),
    ("PV_ITEM_NAMES", "d0spsub", "ITEM_NAME_STRINGS", 0, 0x8010979C, 0, "string pointers"),
    ("PV_ITEM_ICON_CLUTS", "d0spsub", "ITEM_ICON_CLUTS", 0, 0x80109C88, 200, "u8[item] CLUT row"),
    # the item sub-screen overlay (D0SPSUB3 / DSPSUB3)
    ("PV_S3_CONDIDX", "d0spsub3", "ITEM_CONDITION_INDEX", 0, 0x800F584C, 200, "u8[item] condition script"),
    ("PV_S3_CONDS", "d0spsub3", "ITEM_CONDITION_SCRIPTS", 0, 0x800F665C, 0, "Cond *[]"),
    ("PV_S3_FNS", "d0spsub3", "SUBSCREEN_COND_OPS", 0, 0x800F6714, 0, "the 18 condition ops"),
    ("PV_S3_DEST", "d0spsub3", "SUBSCREEN_COND_OPS", 0x8C, 0x800F67A0, 0, "u8 *[9] {area, section}"),
    ("PV_S3_PAIRS", "d0spsub3", "SUBSCREEN_COND_OPS", 0xB4, 0x800F67C8, 0x11E, "{category, item} x 0x8F"),
    ("PV_S3_ORDER", "d0spsub3", "SUBSCREEN_COND_OPS", 0x1D4, 0x800F68E8, 20, "u8[4][5] category order"),
    ("PV_S3_NCLUT", "d0spsub3", "ITEM_NAME_CLUTS", 0, 0x8010065C, 200, "u8[item] name clut"),
    ("PV_S3_MSG0", "d0spsub3", "ITEM_ICON_CLUTS", 0x100, 0x801008EC, 16, "s16 items of message 0"),
    ("PV_S3_MSG1", "d0spsub3", "ITEM_ICON_CLUTS", 0x110, 0x801008FC, 24, "s16 items of message 1"),
    ("PV_S3_MSG3", "d0spsub3", "ITEM_ICON_CLUTS", 0x128, 0x80100914, 8, "s16 items of message 3"),
    ("PV_S3_END", "d0spsub3", "SUBSCREEN_DRAW_FONT", 0x04, 0x80100B88, 0, "end of the tables"),
]


def provides(path):
    out = {}
    for m in re.finditer(r"PROVIDE\((\w+) = 0x([0-9A-Fa-f]+)\);", open(path).read()):
        out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", nargs=2, metavar=("PAL_DATA", "USA_FS"))
    ap.add_argument("--pal-full", default=os.path.join(ROOT, "build", "full"))
    ap.add_argument("--usa-full", default=os.path.join(ROOT, "build", "scus94236", "full"))
    a = ap.parse_args()
    syms = {}
    for p, (pl, ul, *_r) in PROGS.items():
        syms[("pal", p)] = provides(os.path.join(a.pal_full, pl + ".full.ld"))
        syms[("usa", p)] = provides(os.path.join(a.usa_full, ul + ".full.ld"))
    rows, bad = [], 0
    for macro, prog, sym, off, pal, same, what in ADDRS:
        sp, su = syms[("pal", prog)].get(sym), syms[("usa", prog)].get(sym)
        if sp is None or su is None:
            sys.exit("gen_panel_addrs: %s has no address in the %s link of %s" % (sym, "PAL" if sp is None else "USA", prog))
        if sp + off != pal:
            print("PAL %s: %s%+#x = 0x%08X, expected 0x%08X" % (macro, sym, off, sp + off, pal)); bad += 1
        rows.append((macro, pal, su + off, "%s %s%s" % (prog.upper(), sym, " + 0x%X" % off if off > 0 else " - 0x%X" % -off if off else ""), what))
    if bad: sys.exit("gen_panel_addrs: %d PAL addresses moved" % bad)
    if a.check:
        files = {}
        for p, (_pl, _ul, pf, uf, pb, ub) in PROGS.items():
            files[p] = (open(os.path.join(a.check[0], pf), "rb").read(), open(os.path.join(a.check[1], uf), "rb").read(), pb, ub)
        for (macro, prog, sym, off, pal, same, what), (_, _, usa, _, _) in zip(ADDRS, rows):
            if not same: continue
            dp, du, pb, ub = files[prog]
            ok = dp[pal - pb:pal - pb + same] == du[usa - ub:usa - ub + same]
            print("%-24s %s" % (macro, "same bytes" if ok else "DIFFERENT"))
            bad += not ok
        if bad: sys.exit("gen_panel_addrs: %d tables differ" % bad)
    out = ["/* generated by port2/tools/gen_panel_addrs.py from the full links' PROVIDE tables (build/full, build/scus94236/full):",
           "   the retail addresses the second-screen panel reads, per game version (symbol + offset).  Addresses only. */",
           "#ifndef PANEL_ADDRS_H", "#define PANEL_ADDRS_H", "#if PORT_VERSION_USA"]
    w = max(len(r[0]) for r in rows)
    for macro, pal, usa, src, what in rows:
        out.append("#define %-*s 0x%08Xu   /* %s: %s */" % (w, macro, usa, src, what))
    out.append("#else   /* PAL SCES-01331 */")
    for macro, pal, usa, src, what in rows:
        out.append("#define %-*s 0x%08Xu" % (w, macro, pal))
    out += ["#endif", "#endif"]
    open(OUT, "w").write("\n".join(out) + "\n")
    print("%s: %d addresses" % (os.path.relpath(OUT, ROOT), len(rows)))


if __name__ == "__main__":
    main()
