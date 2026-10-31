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
- [x] JP Psy-Q library (2026-10): the JP exe links the SAME Psy-Q revision as PAL for the C-compiled routines; they were unmatched only because splat cut them into fragments (`jr ra` tails, `*_OBJ_xxx` pieces). Fix = symbol size = PAL size and drop the fragments in `config/slps01144/symbol_addrs_slps01144.txt` (40+1 functions: str*/mem*/sprintf/prnt/putchar/puts, rsin/rcos/ratan2/csqrt, getintr/setIntr/trapIntr/dma_execute/cb_read, _Ss*/_spu_*/vmNoiseOn): 59.96 -> 61.20 %. build_report now assembles R4000 opcodes in handwritten targets (x14 801230FC) and drawObjectSpriteQuad JP block includes camera.h.
      Left (~7.8 KB, ~5 % of the gap): libgte asm (MulMatrix0, ApplyMatrix*, RotMatrix*, ScaleMatrix, SquareRoot0, ...), setjmp/longjmp, start/__main/_patch_*, DecDCTvlc: asm-only in PAL too (no C anywhere); _SsVmSetVol (+VM_VOL_OBJ_*) and _SsContDataEntry: no matching C exists in any public repo checked (psx_tomba, dcb_decomp, sotn-decomp, psy-q-decomp all INCLUDE_ASM them; eternal-silver has C for another libsnd revision). src/wip/_SsVmSetVol.c and src/wip/_SsContDataEntry.c are non-matching ports (score 143 words off for the latter). libgte asm, setjmp/longjmp and DecDCTvlc are hand-written asm even in eternal-silver (INCLUDE_ASM/asm_only), so no C to port; no repo convention counts INCLUDE_ASM as matched. Remaining gap is game code, not library.
- [ ] Near-twins (2026-10, 61.20 -> 61.61 %, +19 functions: exe 6, x00 2, x01 4, x03 3, x06 1, x10 1, x14 2): JP functions whose PAL twin is matched C. Method: opcode-skeleton similarity JP asm vs all matched PAL functions (difflib, regs/imm masked), compile the PAL C with VERSION_SLPS_01144, align instruction by instruction and read the diffs. Recurring JP differences: pad button words are byte-swapped (`& 0x20` -> `& 0x2000`, `0xc00` -> `0xc`); the PAL-only rumble check `SPAD.b1a4` / `startPadVibration(0,0,0xff,4)` and `GV2.bb0 = 0` do not exist in JP; screen-clip constants (0xf1/0x141 instead of 0x100/0x140); object struct fields -0x10 (0xe4 -> 0xd4); SPAD fields are named by their JP address (`SPAD.s19e` = `D_1F80019E`); every global/callee is a `D_xxxxxxxx`/`func_xxxxxxxx` JP symbol (use `#define PAL_NAME JP_NAME`, never for struct+offset: `GAME`/`GVARS`/`GCD`/`GV6` fields need the plain `D_` symbol). New JP-only twins live in `src/[x0nn/]func_<addr>_slps01144.c`, with the symbol renamed `_slps01144` in `config/slps01144/symbol_addrs_<prog>.txt`.
      Stalled: x01/x03 80056874 probeGrabObjectBox (switch `beqz` gets an R_MIPS_PC16 in the target only), 80057B74 spawnObjectFromRecord (retail uses relocated `lui/addiu` for scratch 0x334/0x338, not absolute), x03 80119CEC ok but 800EF8CC playerGroundAnimTransition and 80019DBC displayDebugScreen (jump tables + GVARS fields), 801153B8/80116748/80115C64 updateAreaScroll (constants + GVARS), 80029E88 updateCameraScroll, 8011DCC8, 80104C64, 80056C34 playerProbeGrab (struct -0x10 + SPAD names, 64 diff lines, not attempted beyond 1 try).
      Left (~330 candidates with 1-5 instruction differences, mostly PAL-only features): same recipe, needs per-function reading; GVARS/GCD/GAME struct fields -> D_ symbols table would unlock most (learn it from the diffs).
      PAL->JP globals (2026-10, tools/jp_globals.py [--neartwin] --write): masked-skeleton alignment of paired functions (2279 pairs + 91 near-twin pairs) votes PAL name -> JP address; 147 PAL-named globals/aliases written into config/slps01144/symbol_addrs_*.txt (`// BEGIN jp_globals` blocks). Blocks are shifted: GAME/GVARS/GCD/GV2 +0x1BA8, GV6 +0x1B90, PLAYER +0x1B40 (fields inside PLAYER shift a further -0x10 in places). Raw `D_` names used by JP-only C stay pinned as exact symbols (46) and 29 exact bases stay raw, so existing matches are kept. Overlay-resident data (>= 0x800C0000) and callee names are NOT written (`--funcs`): both re-split the overlays and inflate the code total (+11 KB). Total stays 61.61 %.
      Near-twin pass (tools/jp_neartwin.py --install): finds unmatched JP functions by skeleton similarity (>= 0.85) to matched PAL C, compiles the PAL C for JP in variants (raw, demo branch, pad bits, no vibration, section index doubled). Result: 2 of 125 candidates match (testSectionFlagBit, setSectionFlagBit: 61.63 %). The rest need per-function edits, mostly AREA_SECTION_COUNTS-like unmapped D_ tables (vote more pairs) and struct -0x10 shifts.
- [x] JP splat fragments in game code (2026-10): scripted scan (asm units that do not end in `jr ra`/`j`/`b`): every JP symbol with a PAL counterpart already has the PAL size (969+ eq, 0 smaller outside libgte asm), so no PAL-twin gains left. 30 truncated game functions merged (head size = whole body, bogus `*_OBJ_xxx`/`func_*` tails removed: exe 7, x00 21, x01 1, x14 1): matching unchanged (61.20 %), none has a PAL twin (jpdemo_reuse finds no new game pair), so they stay JP-only, C needed. Chains ending in jump-table data (x01 8013A610, x03 80134060, x10 8012D504, x14 8012248C...) are NOT fragments: merging them breaks matched functions and swells the code total.
- [ ] X00 shared object code (47 %), then the engine SLPS_011.44 (54 %): kana/kanji text system, file loading.
- [x] Ghidra analysis stages 1-2 (notes/jp_retail.md, 2e5e0da, 70fbba1): names ported; no-counterpart lists are an older build of the area handlers, not relocated code.
- [x] JP X11 0x80116348: older build of goGoCarMechanicTalk (Food for Fuel?, 0x68), not JP-only (5d8e580). JP text: "Bofutto Car" runs on alcohol; PAL: GoGo car, wine.
- [ ] Read the remaining large X10 no-counterpart functions (list in notes/jp_retail.md).
      Anything JP-only goes to docs/CUT_CONTENT.md and, if worth it, into the port.

## Port tools
- [x] In-game debug menu `--debug-menu` (noclip, fly, cheats, items, events, warp incl. cut rooms, overlay, speed; docs/PORT.md "Debug menu") — launcher checkbox, Android args.txt.
- [ ] Debug menu: frame advance / pause, a drawn collision view (segments), item names in the item line.

## P3 — restoring cut content in the port (only what P2 or earlier digging proves)
- [ ] Funga chief judgement scene (jungle text exists; needs a trigger, probably event 184).
- [ ] JP titles/descriptions of unused events and items as a text patch.

## Android (M7; docs/PORT.md §4 "Status")

- [x] port2 builds for armeabi-v7a (ARM null-page emulation, coroutines, thunks, loader below 16 MB) — low
- [x] Gradle/NDK project + `android-apk.yml` (debug APK artifact) — medium
- [ ] First run on a handheld (Retroid / AYN Odin): logcat + `files/tombi.log`, fix crashes (VFP loads from null, Thumb faults, layout reservation) — medium
- [x] Native launcher (Selva design: play / options / disc+data with SAF folder import; ES+EN) — medium
- [ ] GLES path of `shims/gpu_gl.c` on Android (soft renderer is the default now) — medium
- [ ] In-app options screen writing `files/args.txt` (language, 60 fps, widescreen, renderer) — low
- [ ] SAF import: pick a .bin/.cue/.chd, verify MAIN0 SHA-1, run extract_data logic natively into `files/data` — medium
- [ ] Touch overlay for phones (handhelds don't need it) — medium
- [ ] Release signing + versioned APK in release.yml — low
- [ ] arm64-v8a: phase 2 pointer model (32-bit offsets in a 4 GB arena, docs/PORT.md §2.1). Needed for Play Store and for newer SoCs that drop 32-bit app support (Snapdragon 8 Gen 3 and later, e.g. some recent handhelds) — high

## Parked (do not start without an explicit go)
- PSP versions; Tombi-Tools zip; building ISOs; FMV-resident sprites; the JP demo (39.6 %).
