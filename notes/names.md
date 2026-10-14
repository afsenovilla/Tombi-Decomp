# MAIN0 function names (review list)

One line per name: `address name (old file) - justification`.

- `80068fa4 PadChkVsync` (was func_80068FA4) - libpad padentry thunk that calls _padChkVsync (returns and clears the vsync flag D_80098214); order and shape match PadChkVsync (khasinski libpad/padentry.c)
- `80068fc4 PadStartCom` (was FUN_80068fc4) - libpad padentry thunk that calls _padStartCom
- `80068fe4 PadStopCom` (was func_80068FE4) - libpad padentry thunk that calls _padStopCom
- `80069004 PadChkMtap` (was func_80069004) - checks multitap flag and port entry byte +0xE8 == 8; same body as PadChkMtap (padentry.c, psx_tomba)
- `80069050 PadGetState` (was FUN_80069050) - maps state byte +0x49 (3,2->1; 6->4) after pending-request test; same switch as PadGetState (padentry.c)
- `8006911c PadInfoMode` (was FUN_8006911c) - switch on info kind 1,2,3,4,100 over the port object (+0xE8/E6/E4/E3/0x4C); same as PadInfoMode (padentry.c)
- `80069214 PadInfoAct` (was func_80069214) - actuator table lookup, field 1..5; already documented in its source as PadInfoAct
- `800692e8 PadInfoComb` (was func_800692E8) - actuator combination table lookup; sits between PadInfoAct and PadSetActAlign in padentry order
- `80069390 PadSetActAlign` (was FUN_80069390) - thunk: _padSetActAlign(lookup(ch), cmd)
- `800693c8 PadSetMainMode` (was func_800693C8) - thunk: _padSetMainMode(lookup(ch), b1, b2)
- `80069410 PadSetAct` (was FUN_80069410) - thunk: _padSetAct(lookup(ch), payload, size)
- `8006957c _padSetVsyncParam` (was FUN_8006957c) - stores two callback addresses and clears two words, as _padSetVsyncParam (libpad padmain.c)
- `800697a0 _padChkVsync` (was func_800697A0) - returns the vsync flag and clears it
- `800697b4 _padStartCom` (was FUN_800697b4) - source says _padStartCom (psx_tomba padmain.c); Dequeue/Enqueue SIO IRQ, enable, clear counters
- `80069880 _padStopCom` (was func_80069880) - EnterCriticalSection, ChangeClearRCnt(3,1), SysDeqIntRP, Exit; matches _padStopCom
- `800698c4 _padInitSioMode` (was func_800698C4) - source says _padInitSioMode with PAL SIO reset tail
- `80069ce8 _padSioRW` (was FUN_80069ce8) - source says older revision of _padSioRW: one SIO0 byte exchange
- `8006a170 _padClrIntSio0` (was func_8006A170) - clears SIO IRQ flag and waits for stat bit 0x80 with timeout; same as psx_tomba _padClrIntSio0
- `8006a254 _padSetAct` (was FUN_8006a254) - stores payload and size into the port object; target of PadSetAct
- `8006a270 _padSendAtLoadInfo` (was FUN_8006a270) - switch on port state +0x46 calling per-command emitters; psx_tomba _padSendAtLoadInfo (same size)
- `8006a2f4 _padRecvAtLoadInfo` (was FUN_8006a2f4) - source says older revision of _padRecvAtLoadInfo
- `8006a564 _padGetActSize` (was FUN_8006a564) - computes size from mode count, actuator count and combination size (padcmd _padGetActSize)
- `8006a59c _padLoadActInfo` (was FUN_8006a59c) - source says _padLoadActInfo (padcmd.c)
- `8006ab10 _padSetActAlign` (was func_8006AB10) - sets state 1 with the 0x4D emitter and align command; target of PadSetActAlign
- `8006ac5c _padSetMainMode` (was FUN_8006ac5c) - source says _padSetMainMode (padcmd.c); target of PadSetMainMode
- `8006b464 PadInitDirect` (was FUN_8006b464) - source says PadInitDirect (padportd.c): installs direct pad object table
- `8006efa8 SetRCnt` (was FUN_8006efa8) - psx_tomba ut_roff.c SetRCnt matched by bytes
- `8006f07c StartRCnt` (was func_8006F07C) - psx_tomba ut_roff.c StartRCnt matched by bytes
- `8006f0e0 ResetRCnt` (was FUN_8006f0e0) - psx_tomba ut_roff.c ResetRCnt matched by bytes
- `8006c544 SsInitHot` (was FUN_8006c544) - psx_tomba ssinit.c SsInitHot matched by bytes (calls SpuInit-like and _SsInit)
- `8007131c SsUtReverbOff` (was FUN_8007131c) - psx_tomba ut_rfb.c matched by bytes
- `8007133c SsUtReverbOn` (was FUN_8007133c) - psx_tomba ut_rfb.c matched by bytes
- `800745b8 SsSetMono` (was func_800745B8) - psx_tomba vs_mono.c matched by bytes
- `800745cc SsSetStereo` (was func_800745CC) - psx_tomba vs_mono.c matched by bytes
- `80074b6c SpuInit` (was FUN_80074b6c) - psx_tomba vs_vh.c SpuInit matched by bytes (thunk calling _SpuInit)
- `8006bf84 setRC2wait` (was FUN_8006bf84) - source says setRC2wait (libpad waitrc2.c): stores root counter 2 value and limit
- `8006bfa4 chkRC2wait` (was FUN_8006bfa4) - compares elapsed root counter 2 ticks against the limit; same as chkRC2wait (waitrc2.c)
- `8005d300 StopCARD` (was func_8005D300) - calls StopCARD2, _patch_card2 and a card restart helper; psx_tomba libcard init.c StopCARD matched by bytes

# Area overlay function names (X001..X019)

Overlay-scoped: a name applies only to the overlay named on its line (the same address in another overlay is another function; the same
name in several overlays means the same role, not the same code). Applied with `tools/apply_ovl_names.py`. Handler = the function a
placed object runs (`tools/objects.py`: handler table[layer][type]); enemy names come from the rendered animation strips (`tools/anim.py export`).

- `x001/x003/x004/x009/x010/x018 updateAreaMechanisms` (x001 8011bd84, x003 8011a874, x004 8011b7c4, x009 8011ab44, x010 80118558, x018 801169d0) - the area's layer 4 type 7 handler (`dispatchAreaInitB` target, objects.py AREA_INIT_B); switch on subtype over linked switches/doors/rotating parts that copy the parent's state (`o->d90`) and start/complete events through `o->wac`
- `x003 8011a6c4 / x004 8011afa8 / x009 8011aa04 / x010 80118418 / x018 801168a0 stepAreaSwitch` - subtype 0 of updateAreaMechanisms: activates when the player is inside a +-0x20/0x30 box with player state byte da0+2 == 2, sinks and returns 1/2 that the caller turns into setEventStarted/Complete
- `x002 80118684 / x004 8012b438 / x006 8011e54c / x013 80116fe0 updateSceneSequence` - the layer 2 "scene sequence" objects of objects.py SEQ_TYPES: walk a table of (op, arg) sequences, show message boxes and start/complete events per op
- `x014 801163bc updateSectionClearController` - section controller (called through the FUN_801168a0 thunk): locks the scroll, runs the room's sub-handler table D_8012568C, then setEventComplete(D_801256A8[section]) (objects.py AREA14_BOSS_EVENT), timer and fade out
- `x004 80126e54 updateBlackCrow` - layer handler whose animation tables render a black crow (labels.json animowner a04)
- `x009 80123e24 updateRedFlower` - handler whose strips show a red flower bud that blooms and laughs (labels.json animowner a09)
- `x010 8012ac48 updateSpearPig` - handler whose strips show the blue pig with a spear (labels.json animowner a10)
- `x010 8012ce80 updateShieldSpearPig` - strips show the same blue spear pig holding a round wooden shield, then throwing the spear
- `x010 8012c8a0 updateParrot` - strips show a red/yellow parrot standing, walking and flying
- `x001 8012b2d4 updatePurplePig` - layer 2 type 0 (11 placements, AREA01/07): strips show a purple pig walking, turning and falling
- `x001 80138d4c updateCeilingDrip` - layer 2 type 34: strips show water gathering on the ceiling, forming a drop, falling and splashing
