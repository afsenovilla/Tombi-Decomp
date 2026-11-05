# psx_tomba names for the Japanese build (SLPS-01144), imported 2026-11-05

Source: https://github.com/hansbonini/psx_tomba, `symbols/slps_011.44/slps_011.44_symbol_addrs.txt` (MIT per its README;
its LICENSE file was missing on that date). Cross-checked against `config/slps01144/symbol_addrs_slps01144.txt`.

- Same name on both sides: 525 functions. Imported: 29 renames of our generic `func_*` names and 54 functions our JP
  symbols lacked (mostly BIOS / libapi stubs, plus main, awardEventProgress, playSFX, loadWssFile, increaseMaxHealth,
  printInfoMessage), each paired with its PAL function by name (`PAL main0 0x...`, so jp_autofix can try the PAL C);
  sizes from the PAL twin when the masked code matches, else by scanning to `jr $ra`. JP report 85.03 -> 85.05 % (2779 of 3102
  functions, +13; the 82 % figures seen while importing were a branch behind the sweep's matches). Dropped: `SsInitHot` (0x80066280) and `_patch_gte` (0x80061488), which we place at
  other addresses; `libgte_stamp` / `libapi_stamp`, which are strings, not functions.
- Overlay symbol files (x00..x19) of psx_tomba hold no names we lack. Data symbols were not imported.
- The USA build has no config here; nothing to import for it yet.

## Both sides named, differently (ours kept: PAL names, consistent with notes/names.md)

| JP address | psx_tomba | ours |
|---|---|---|
| 0x80016610 | `dispatchTasks` | `ThreadTableRun` |
| 0x80016740 | `openTask` | `ThreadCreate` |
| 0x800167C0 | `sleepTask` | `ThreadWaitFrames` |
| 0x80017A1C | `allocObjectLayer3` | `ObjAlloc` |
| 0x80017DB8 | `freeObjectLayer3` | `ObjFree` |
| 0x80020958 | `nextRandom` | `Rand` |
| 0x80020C5C | `applyObjectSpeedXY` | `ObjApplyVelocity` |
| 0x80020D24 | `fixedMulSin` | `MulNegSinScaled` |
| 0x80020D54 | `fixedMulCos` | `MulCos` |
| 0x80020DE4 | `readAnimFrameCount` | `AnimLoadDuration` |
| 0x80020E0C | `advanceAnimFrame` | `AnimJump` |
| 0x80020E38 | `tickAnimation` | `AnimAdvance` |
| 0x80022098 | `setSpawnAreaFlag` | `markSectionVisited` |
| 0x80036044 | `distanceToPlayer` | `getDistanceToPlayer` |
| 0x80038A6C | `uploadSpriteFrame` | `uploadSpriteFrameToVram` |
| 0x80040E40 | `probeCollisionAtDepthA` | `TileCollideAt` |
| 0x8004C7BC | `addDrawModePrim` | `AddDrawMode` |
| 0x80065D80 | `__SN_ENTRY_POINT` | `start` |
| 0x80067CD0 | `_SsSetNrpnVabAttr17` | `sndCmdSetReverbFeedback` |
| 0x80067CF4 | `_SsSetNrpnVabAttr18` | `sndCmdSetReverbDelayA` |
| 0x80067D18 | `_SsSetNrpnVabAttr19` | `sndCmdSetReverbDelayB` |
| 0x800687AC | `SsQuit` | `PadStartCom` |
| 0x80068B84 | `SsStart` | `SpuInitHot` |
