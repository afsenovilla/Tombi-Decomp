# Roadmap

Working order: take the first open item of the highest section that has one. Inside a section, easiest first.
When an item is done, tick it, note the commit, and move on without waiting. Update the JP table in README
(`tools/build_report.py --version slps01144` + `tools/progress_jp.py`) and push `notes/report_stamp.txt` (decomp.dev)
after each JP batch.

## P1 — quick wins (minutes to an hour)
- [x] Port: guard `playerCheckPerchLanding+0xae` null read (GRABBED_OBJECT == 1) — 71568b9.
- [x] Civ towns: ES title 74 accent renders correctly (headless check, frames 1545-1650).
- [x] Civ towns: green teller's hat pokes ~8 px above the H3 room view (he stands on its top edge); cosmetic, left as is.
- [x] Port: --civ-town with --civ-maps lacking the area segfaulted in I0NFO2_topdownBuildMapStrip+0x1d5; fall back to the stock town or refuse with a message.
- [ ] Civ towns: look at the unchecked teller sheets (EN low/300k, ES 300k).
- [ ] Civ towns: test resident 2 (AREA11, dlg 1) and AREA17 dlg 4 in game (only the safety net covers them).
- [ ] JP: re-run the report and refresh README + decomp.dev after every matching batch (standing item).

## P2 — JP retail decompilation, easy first
- [x] JP tooling (2026-10): `tools/ncheck.py --version slps01144 FILE.c` (= `tools/jpcheck.py [--norm]`) compiles one JP function and diffs it against the splat target; `tools/jp_data_units.py --write` finds mis-split data and writes `config/slps01144/data_units.txt` (45 units / 129 KB of jump tables, id words, trailing tables left out of the code totals: 55.59 -> 59.87 %); 60 tiny functions scripted (nops, return-const, `p[4]=3`, global getters/setters): 59.91 %. What is left that is tiny is splat fragments of larger functions.
Order by expected yield per effort: areas whose PAL twin is fully matched and the JP code is near-identical.
- [x] Auto-match pass (2026-10, tools/jpdemo_reuse.py fixed for data.o, 2049 -> 2085 pairs, JP stays 55.26 %: new pairs already covered by JP C): reuse PAL C for every JP function whose masked bytes equal a matched PAL function
      (`config/slps01144/reuse.txt`, `tools/version_share.py`), all programs. Cheapest % gain.
- [ ] Small areas to 100 %: X08 (3 f), X19 (3 f), X05 (22 f), X13 (11 f), X02 (41 f).
      Partial: X05 16/22, X13 7/11, X02 27/41 (stubs, updateMightyFishFood); what is left in X08/X19/X05 is data mis-split as functions, X13 80115FD8 done, 801162DC (2196 B) and X02 big handlers open.
- [ ] Cut towns X16, X17, X18, X11 (37 %): X16/X17/X18 small JP-only functions done (stubs, `p[4]=3` setters, x16 80116128, x18 80116098).
      Open: x16 80115A80 (93 %), x17 801170F4 (90 %), x18 8011903C (95 %): register/scheduling stalls. x18 80118854 (2024 B switch) untouched.
      X11 (26/33 now): 801170E0..80117D08 (5 talk twins), 80116F28, 80117644, 801167A8, 80116D24, 80115FC0 and stubs done (talk template: PLAYER fields + volatile wbc read + `volatile short D_800A7C2A`).
      Open X11: 80117AFC (block order of the shared showMessage call, goto/msg var stalls), 80116348 (1120 B, PAL goGoCarMechanicTalk differs a lot), 801156A0, 801169A8, 80117260, 801188AC; data mis-split: 80115178 (jump tables).
      Data mis-split as functions: x16 80118004, x17 80115178/80118314, x18 80115178/801194AC. X18 also uses the teller lines.
- [ ] Jungle X10 (62 %; 10 stubs/setters done 143/178, 80119D9C tried twice: reg alloc a2/a3 + hoist, open; 80115178 is jump tables): finish; read every function with no PAL counterpart (notes/jp_retail.md).
- [ ] Mid areas X01, X03, X04, X06, X09, X14.
- [ ] X00 shared object code (47 %), then the engine SLPS_011.44 (54 %): kana/kanji text system, file loading.
- [x] Ghidra analysis stages 1-2 (notes/jp_retail.md, 2e5e0da, 70fbba1): names ported; no-counterpart lists are an older build of the area handlers, not relocated code.
- [x] JP X11 0x80116348: older build of goGoCarMechanicTalk (Food for Fuel?, 0x68), not JP-only (5d8e580). JP text: "Bofutto Car" runs on alcohol; PAL: GoGo car, wine.
- [ ] Read the remaining large X10 no-counterpart functions (list in notes/jp_retail.md).
      Anything JP-only goes to docs/CUT_CONTENT.md and, if worth it, into the port.

## P3 — restoring cut content in the port (only what P2 or earlier digging proves)
- [ ] Funga chief judgement scene (jungle text exists; needs a trigger, probably event 184).
- [ ] JP titles/descriptions of unused events and items as a text patch.

## Parked (do not start without an explicit go)
- PSP versions; Tombi-Tools zip; building ISOs; FMV-resident sprites; the JP demo (39.6 %).
