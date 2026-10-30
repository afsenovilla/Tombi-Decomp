# Roadmap

Working order: take the first open item of the highest section that has one. Inside a section, easiest first.
When an item is done, tick it, note the commit, and move on without waiting. Update the JP table in README
(`tools/build_report.py --version slps01144` + `tools/progress_jp.py`) and push `notes/report_stamp.txt` (decomp.dev)
after each JP batch.

## P1 — quick wins (minutes to an hour)
- [x] Port: guard `playerCheckPerchLanding+0xae` null read (GRABBED_OBJECT == 1) — 71568b9.
- [x] Civ towns: ES title 74 accent renders correctly (headless check, frames 1545-1650).
- [ ] Civ towns: check the green teller's head above the room view.
- [ ] Port: --civ-town with --civ-maps lacking the area segfaults in I0NFO2_topdownBuildMapStrip+0x1d5; fall back to the stock town or refuse with a message.
- [ ] Civ towns: look at the unchecked teller sheets (EN low/300k, ES 300k).
- [ ] Civ towns: test resident 2 (AREA11, dlg 1) and AREA17 dlg 4 in game (only the safety net covers them).
- [ ] JP: re-run the report and refresh README + decomp.dev after every matching batch (standing item).

## P2 — JP retail decompilation, easy first
Order by expected yield per effort: areas whose PAL twin is fully matched and the JP code is near-identical.
- [ ] Auto-match pass: reuse PAL C for every JP function whose masked bytes equal a matched PAL function
      (`config/slps01144/reuse.txt`, `tools/version_share.py`), all programs. Cheapest % gain.
- [ ] Small areas to 100 %: X08 (3 f), X19 (3 f), X05 (22 f), X13 (11 f), X02 (41 f).
- [ ] Cut towns X16 (77 %), X17 (68 %), X18 (73 %), X11 (37 %): finish; X18 also uses the teller lines.
- [ ] Jungle X10 (62 %): finish; read every function with no PAL counterpart (notes/jp_retail.md).
- [ ] Mid areas X01, X03, X04, X06, X09, X14.
- [ ] X00 shared object code (47 %), then the engine SLPS_011.44 (54 %): kana/kanji text system, file loading.
- [ ] Ghidra analysis stages (notes/jp_retail.md): better PAL-name matching, then read what has no counterpart.
      Anything JP-only goes to docs/CUT_CONTENT.md and, if worth it, into the port.

## P3 — restoring cut content in the port (only what P2 or earlier digging proves)
- [ ] Funga chief judgement scene (jungle text exists; needs a trigger, probably event 184).
- [ ] JP titles/descriptions of unused events and items as a text patch.

## Parked (do not start without an explicit go)
- PSP versions; Tombi-Tools zip; building ISOs; FMV-resident sprites; the JP demo (39.6 %).
