# Hidden / debug features and cut content

Method: byte scans of MAIN0.EXE, MAIN1..8.EXE and every overlay on the disc for stores to scratchpad flags (`sb/sh/sw x, 0x1B4(reg)` etc.),
the Ghidra decompile of MAIN0 (`game/MAIN0_EXE_decomp.c`) for readers, and `tools/psxemu` (poke the flag, drive the pad) for verification.

## Result in one line
There are **no button-sequence cheats** (no table of pad codes with a counter) in the pad readers of MAIN0 that were reviewed, and **no code
anywhere writes the debug flags**: they stay 0 unless set from outside (GameShark / emulator poke). All of it is dead debug code left in the retail game.

## Debug flags (scratchpad, 0x1F800000 + off). Nothing in MAIN0..MAIN8 or any overlay stores to these offsets
| flag | read by | effect when != 0 | verified |
|---|---|---|---|
| `0x1F8001B4` `debug_mode_enabled` | `introSequenceHandler` (0x80019D88, 2 reads), `displayDebugScreen` (0x8001A0F0), `debugSelectHandler` | NEW GAME opens "AREA SELECT = nn / SECTION SELECT = nn" (gameplay mode 0) instead of the village; UP/DOWN pick the row, LEFT/RIGHT change value (clamped by the section table at 0x80078790), CIRCLE/START loads that area+section. Also re-entered when Tombi dies in a debug-started game. | yes, PNG `debugselect` (title -> poke -> START; reproduced by `capture.base_state`) |
| `0x1F8001AB` | `initGameConfig` (0x8001783C) | NEW GAME gives a **debug loadout**: equipment/inventory bytes at 0x8009D1A4.. = 1B 21 22 15 16 17 20 1D 18 1C 05 06 24 14, `0x8009D2A4` = 0x0E (instead of 0x20 / 2), and ~12 progress flags (0x8009D0A9..0x8009D0C8) set to 1. | yes (poke before START: bytes read back as listed, game starts normally) |
| `0x1F8001B3` | `initGameConfig` | != 0 skips both the normal and the 1AB loadout (leaves the config as the caller set it). | no (static) |
| `0x1F8001BD` | main loop (`main`, 0x80016604) | while R2 is held (`pad & 0x200`) and a display mode flag (0x1D0/0x1D1) is on: the D-pad moves the display environment (`DAT_8009BC80/82` +-4 per frame) = screen position adjuster. | no (static) |
| `0x1F8001BE` / `0x1F8001BF` | main loop (0x80016824 / 0x80016790) | frame control through `0x1F8001F0` / `0x1F8001EE`: with 1BE: L1 (0x400) toggles pause (1EE), L2 (0x100) held steps / freezes (`1F0 ^= 0x4000`, the thread table is not run when 1F0 > 0x4000); with 1BF: L1 flips `0x8000 - 1F0`. Needs 0x1CC == 0, 0x1CE == 1 and 0x1D0/0x1D1 != 0. | partly: poke 1BE = 1 in AREA00, L2 sets `1F0` to 0x4001 (freeze); L1 toggle did not change 1EE in the test (not understood) |
| `0x1F8001BB` | area task step (0x8001A810, `FUN_8001A64C`) | != 0 stops the countdown `0x8009C980` that ends the mode (timer / auto exit of a state). | no |
| `0x1F8001B8` | `FUN_8001CA58` (0x8001CBD8) | != 0 blocks the button-mask check (`0x1F8003CA & pad`) in a menu = disables that input. | no |
| `0x1F8001BA` | 0x8002D89C | read only, purpose unknown. | no |

`fontDebugPrintf`, `SetGraphDebug(0)`, `CdSetDebug` are Psy-Q leftovers (printf into a stub); the debug screen text is drawn by the game's own font.
No other debug menu or unreachable screen found: `displayDebugScreen` / `debugSelectHandler` are the only ones named.

## Title screen
`loopTitleScreen` (0x8004E41C): holding RIGHT-ish bit `0x20` in `0x1F8001F8` (held pad word) draws one extra sprite (`func_800E972C(0x38, 0xA0, 0)`), probably a
cursor/hand; not a secret. Button sequences: none (only UP/DOWN/CROSS/START handling; menu has 3 entries incl. the options).

## Event titles (CFNT999*.WFM, 136 entries 0..135)
The title text of event e is `D_800806B4[area][e]` (RAM 0x80080410, 200 bytes = 200 events, identical for all 20 area slots; the 0x800806B4 array holds 20
pointers to it). Layout record list = area-overlay table `[e]`.
- Referenced entries: 2..134 except **90 "The Black Water"** and **126 "The Rusty Steel Car"** (English; 90 / 126 are never selected by any event id;
  the other 131 distinct entries are used, with a few unused slots 0xFF in the table: events 6, 18, 45, 48, 52, 58 .. = no title, fallback entry 1
  "!!!DUMMYCUBE!!!").
- **Entry 52 "MiniatureMakingTrumpets" IS in the table (event 32)**, so it is not removed from the title table; it is the one without a layout list in the
  aligner (`tools/title_layout.py`), the alignment of lists to dialogues is by length so the "missing lists" are 4 guesses (52, ~90, ~126, 135): 90 and 126
  being unreferenced fits the "no list" for them. 135 ("......................") is past the table end (max 134) = placeholder; 0 ("CLEAR!") is the banner bitmap source only.
  Whether event 32 can ever be awarded was not checked (script op not located; `awardEventProgress` is reached from script op 0x8003AA68 `scriptOpAwardEvent`,
  which is not in a MAIN0 table, so the scripts use the overlay op table). Not resolved: tracing the per-area table at 0x8013A044 (it holds a pointer and
  a byte table, not the pointer array the decompile assumes).

## Not done
- Exhaustive proof that no overlay has a button sequence (checked: MAIN0 pad readers; overlay pad code not audited one by one).
- Unreferenced dialogue / room / item text.
