# MAIN0 function names (review list)

One line per name: `address name (old file) - justification`.

User naming workflow (no toolchain needed): `tools/unnamed.py` writes `notes/unnamed.json`, the generic-named functions of MAIN0, X000
and the SYSTEM overlays with their `// probably:` note, callers, table slot and the sprite frames resolved as the "Animaciones" node does.
The graphics editor (tombi-gfx.exe) shows it under "Sin nombre / Unnamed" ("Probablemente" = the ones with a note, to review; "Resto"):
pick one, read the note and frames, type a name and a comment, "Exportar nombres" -> `names_user.json`. On the decomp side
`tools/apply_names.py names_user.json [--dry]` renames files, symbols, csv rows and twins exactly as the `names:` commits do, appends the
rows under "# User names" at the end of this file (confidence `media` in names_*.csv); then `tools/build_full.py` and regenerate
`notes/unnamed.json`.

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
- `800587a8 drawHudLives` (was func_800587A8) - drawHud element at (0x18,0xF0): one or two digit sprites of GAME.playerLives - 1 plus the icon frame
- `8005892c drawHudAP` (was func_8005892C) - drawHud element at (0x12C,0x10): the 8 digits at HUD+0x3C (written from GAME.playerAP 8009C96C by addPlayerAP/initHud), leading zeros skipped, box frame by digit count
- `80058d28 drawHudItemGauge` (was FUN_80058d28) - drawHud loop over 3 gauges: item icon (frame D_8009CD98[i]) and a 0x3D-wide bar filled to D_8009CD9C[i] via addTilePrim
- `80058f98 drawHudWeaponIcon` (was FUN_80058f98) - drawHud element at (0x48,0x10): switch on the equipped weapon D_8009D2B2 (same byte spawnWeaponObject uses) picking the icon frame
- `80057368 drawObjectModelNoRot` (was FUN_80057368) - drawObject render kind 0x10: base matrix + object position only, then the model draw FUN_80023b04
- `80057470 drawObjectModelRotated` (was FUN_80057470) - render kind 0x11: RotMatrix from +0x84/88/8C (axes swapped by 1F8001C8 bit 0), position, MulMatrix0 with the camera, FUN_80023b04
- `80057610 drawObjectModelRotatedVtx` (was FUN_80057610) - render kind 0x12: same matrix as kind 0x11 but draws with FUN_800242b8, which takes an extra vertex block (+0xA8)
- `800577b8 drawObjectModelRotScaled` (was FUN_800577b8) - render kind 0x13: RotMatrixZ/X/Y from +0x84/88/8C plus ScaleMatrix by +0x74/76/78
- `800579a8 drawObjectModelCameraFixed` (was FUN_800579a8) - render kind 0x14: fresh base matrices and a fixed translation (-0xA0,0x80,0x220) instead of the camera, drawn with FUN_80024e58
- `80057ae4 drawObjectModelOwnMatrix` (was FUN_80057ae4) - render kind 0x15: multiplies the camera by the object's own matrix at +0x48
- `80057bf0 drawObjectModelOwnMatrixVtx` (was FUN_80057bf0) - render kind 0x16: own matrix +0x48 like 0x15, drawn with FUN_800242b8 (extra vertex block)
- `80050544 drawPlayer` (was FUN_80050544) - drawGameplayScene: if PLAYER is active, switch on PLAYER.obj.type (800A603A) to the player renderer (FUN_800505f0 or the X000 ones)
- `80050ea4 drawCategory1Objects` (was FUN_80050ea4) - like drawCategory4Objects for the category 1 list (1F800218, ObjListPush_1F800218 in cullObjectToDrawList), by render kind 0/2/0xD/0x11
- `80054f54 drawCategory2Objects` (was FUN_80054f54) - category 2 list (1F80021C): drawObject, render kind 0xD through the player-type renderer
- `80055174 drawCategory3Objects` (was FUN_80055174) - category 3 list (1F800220): per render kind sprite/model renderers, message boxes
- `80057edc drawCategory7Objects` (was FUN_80057edc) - category 7 list (1F80022C): render kind 0..4 (kind 3 by player type, kind 4 area 4/12 only)
- `8001db38 startMovieStream` (was FUN_8001db38) - moviePlayerTask helper: DecDCToutCallback, CdMix, StSetRing(0x20), StSetStream, then CdControl(CdlSetloc) and CdRead2(0x1C0) until both succeed
- `80059d44 drawScreenFade` (was FUN_80059d44) - drawGameplayScene fade (mode D_8009C975): full-screen 0x140x0x100 semi-transparent tile of level a plus its draw modes
- `80057d04 drawAreaModelList` (was FUN_80057d04) - draws every model pointer of the area model list D_800B0D98 (count at +3, filled per area by FUN_8002af10) with drawModelUnlit (area 9 section 6: FUN_80120280)
- `8002216c modelDrawFT3` (was func_8002216C) - model primitive group: rtpt/nclip/avsz3 of each face into POLY_FT3 (tag length 7) linked into the OT
- `8002235c modelDrawFT4` (was func_8002235C) - same for POLY_FT4 (tag length 9, ft4 layout, avsz4)
- `80022630 modelDrawGT3` (was func_80022630) - same for POLY_GT3 (tag length 9, gt3 layout)
- `80022838 modelDrawGT4` (was func_80022838) - same for POLY_GT4 (tag length 0xC)
- `80022abc modelDrawF3Lit` (was func_80022ABC) - POLY_F3 (length 4) with normal-colour lighting (nccs) of the face colour
- `80022c80 modelDrawF4Lit` (was func_80022C80) - POLY_F4 (length 5) with nccs lighting
- `80022ea8 modelDrawG3Lit` (was func_80022EA8) - POLY_G3 (length 6) with ncct lighting of the three vertex colours
- `80023084 modelDrawG4Lit` (was func_80023084) - POLY_G4 (length 8) with nccs/ncct lighting
- `800232e4 modelDrawF3` (was func_800232E4) - POLY_F3 (length 4) without lighting
- `8002348c modelDrawF4` (was func_8002348C) - POLY_F4 (length 5) without lighting
- `80023698 modelDrawG3` (was func_80023698) - POLY_G3 (length 6) without lighting
- `80023860 modelDrawG4` (was func_80023860) - POLY_G4 (length 8) without lighting
- `80023b04 drawModel` (was func_80023B04) - draws a model by running the primitive groups above in order (lit or unlit by the last argument); used by the drawObjectModel* renderers
- `800242b8 drawModelVtx` (was func_800242B8) - drawModel variant taking a separate vertex block (fourth argument, +0xA8), used by drawObjectModelRotatedVtx/OwnMatrixVtx
- `80024a70 drawModelUnlit` (was func_80024A70) - drawModel with the unlit groups only; used by drawAreaModelList
- `80027c74 cameraFollowPlayerX` (was FUN_80027c74) - camera horizontal follow: target = player +-0x10 by facing, speed ramps to 4 px/frame, clamped to the section bounds +0x2C/+0x2E (flags +0x44 bits 1/2)
- `8002795c cameraFollowPlayerY` (was func_8002795C) - camera vertical follow of the player with dead zone and ramp, clamped to +0x32 (flag +0x44 bit 8)
- `80029cb4 updateCameraFollow` (was func_80029CB4) - camera tick: by camera mode D_800A60D6 calls cameraFollowPlayerX/Y, then the scroll/angle steps and Clamp
- `8003f7cc objCollideSideWalls` (was func_8003F7CC) - after every player step: probeWallAt 8 px ahead (three heights from box2) and behind; a hit on the facing side sets +0xA6 = 2/3 (wall contact used by the steps)
- `8004065c probeWallAt` (was func_8004065C) - getCollisionPlaneAt for x and the object's depth into 1F800278, then pushOutOfWallSegments with the side flag
- `800402ec pushOutOfWallSegments` (was func_800402EC) - walks the wall segments of the collision plane; on overlap moves the object's x out by the penetration and stores the segment flags in 1F800282/1F800284
- `8004fed4 getObjectRenderKind` (was func_80045D0C, US name) - drawObject's switch value: the object's render kind +0x0A, remapped for kinds < 4 except in the area/section combinations listed (D_8009BCCA)
- `800597bc drawPauseMenu` (was FUN_800597bc) - drawGameplayScene while the object lists are frozen (1F8001C6 == 1): dark semi-transparent full-screen tile plus the menu text entries chosen by 1F8003CD
- `80058168 drawScreenSpriteObject` (was FUN_80058168) - drawCategory7Objects render kind 1: draws the object's sprite frame parts as SPRTs at its screen x/y (priority +0x0F or +0x0E), with draw-mode wrappers
- `8005840c drawLetterboxOverlay` (was FUN_8005840c) - drawCategory7Objects render kind 2: the sprite list PTR_DAT_80080258[subtype] plus black 16-px bars at the top and bottom of the screen
- `8001c688 gameEndScreenState` (was func_8001D2F0) (uncertain) - gameStateDispatcher state 2 (state 3 is gameOverScreenState): loads system list 0x5D like the game-over screen but runs its other entry (800E8B08) for 0xF0 frames (skippable while > 0x3C), stopBgm at 0x3C, then SoundStopAll and back to titleSequenceTask
- `8001f96c spawnImpactEffect` (was FUN_8001f96c) - allocates a type 0xF object with subtype = arg 0 at the given x/y/z; called by the hit resolvers, weapon wall hits and the hurt/bite player steps (50 call sites)
- `80033040 weapon0ObjTick` (was func_80033040) - layer-1 handler of type 0 (table 8007A10C[0]; weapon objects use the equipped weapon D_8009D2B2 as type): init weapon0Init, then weapon0Swing for steps 0..2, release counter D_800A611B on exit
- `800317c0 weapon0Swing` (was func_800317C0) - weapon type 0 state machine: weapon head positioned from the player anim frame (D_8007A04C offsets), launched at 0x1400/0x1900 by D_8009C330->b0, drives the player anim (AnimJump/AnimAdvance), wall hits spawn spawnImpactEffect
- `80031664 weapon0Init` (was FUN_80031664) - weapon type 0 init: D_8009C338 = &o->wac, CLUTs (0x80,0x1EF/0x1F0), D_8009D600 = 0xF/8/0x5A, position at the player
- `80032fec weapon0SwingByStep` (was func_80032FEC) (no caller found) - switch on step 0..2 calling weapon0Swing (the inner switch of weapon0ObjTick as a separate function)
- `80034f58 weapon5to7ObjTick` (was func_80034F58) - layer-1 handler of types 5, 6, 7 (8007A120..28): weapon5to7Init(weapon - 5), then weapon5to7Swing
- `80033b44 weapon5to7Swing` (was FUN_80033b44) - weapon types 5..7 state machine (same launch as weapon0Swing plus extra states 7/8 that spin the player anim), uses weaponFindNearestTarget
- `8003509c weapon5to7Init` (was FUN_8003509c, US func_800384F0) - sets the weapon object's sprite data from table 1F800340[n] and copies the player position
- `80033188 weapon5to7InitIfNew` (was func_800365DC, US name) (no caller found) - state 0: clears +0x22 and calls weapon5to7Init(D_8009C61A - 5)
- `800370a4 weapon8to9ObjTick` (was func_800370A4) - layer-1 handler of types 8, 9 (8007A12C/30): weapon8to9Init, then weapon8to9Update
- `80035198 weapon8to9Init` (was FUN_80035198) - weapon types 8/9 init (same CLUT/D_8009D600 setup as weapon0Init)
- `80036f30 weapon8to9Update` (was FUN_80036f30) - per step: weapon8to9Swing and the direction +0x8C by anim frame; steps 3/4 weapon8to9FollowTarget
- `8003566c weapon8to9Swing` (was FUN_8003566c) - weapon types 8/9 swing state machine (same helpers as weapon0Swing)
- `80036cb8 weapon8to9FollowTarget` (was FUN_80036cb8) - moves the weapon with its target object +0x90 (8 px lower for category 2)
- `800331c4 weaponFindNearestTarget` (was FUN_800331c4) - scans the object list 1F800260 for the nearest hittable object inside a 0x80 x 0x90 window around the weapon
- `800314a8 weaponSetPlayerAnimA` (was FUN_800314a8) - sets the player anim script (800A605C = PLAYER+0x24) from tables 80010E80.. by step and facing frame
- `800312e8 weaponSetPlayerAnimB` (was FUN_800312e8) - same with a second set of player anim tables
- `800313c8 weaponSetPlayerAnimC` (was FUN_800313c8) - same with a third set (80010EF8..)
- `80031588 weaponCalcBentOffset` (was FUN_80031588) - x/y offset of a weapon link from angle and length (MulCos/MulSin) plus a bend term mirrored around p[2]
- `8005296c drawObjectSprite` (was func_8005296C) - drawObject render kind 0 (also category 3 kind 0): the object's sprite frame as textured quads at its projected position
- `80052db8 drawObjectSpriteScaled` (was FUN_80052db8) - render kind 1 (category 3 kinds 1/9): as kind 0 with the quad size scaled by +0x64 (and by depth)
- `800532fc drawObjectSpriteRotated` (was FUN_800532fc) - render kind 2 (category 1/3 kind 2): sprite quads rotated by the angle +0x8C
- `80053b5c drawObjectSpriteRotScaled` (was func_80053B5C) - render kind 3 (category 3 kinds 3/11): rotated by +0x8C and scaled by +0x64
- `80054c38 drawObjectSpriteQuad` (was func_80054C38) - render kind 4 (category 3 kind 6): one textured quad from the four vertices at +0xB4/+0xBC/+0xC4/+0xCC through the base matrix
- `80054868 drawObjectSprtTinted` (was func_80054868) - render kind 13: the frame parts as SPRT primitives (SetSprt) with colour = brightness +0x6B, culled to the 0x140 x 0x100 screen
- `8004432c collidePlayerWithCat5Object` (was FUN_8004432c) - processObjectCollisions: box test of the player against a category 5 object (1F800228 list); on contact sets player +0x6A (hit) and frees the object (active 4, +4 = 2); type 0xB with +0xB4 == 1 excluded
- `800490ac collideCat2Type7WithCat5Type0B` (was FUN_800490ac) - only for D_8009C960 == 0x10000: category 2 objects of type 7 (subtype != 0) against category 5 objects of type 0xB (FUN_80126f68)
- `80048e68 collideCat2ObjectsPairwise` (was func_80048E68) - every category 2 object (types enabled in D_8007B6A4) against the solid category 2 objects of types 4/6/7/0xE/0x10 (push-out helpers), 0x1C/0x21 area hooks
- `800491f0 collideCat2WithCat4Objects` (was func_800491F0) - category 2 objects (types enabled in D_8007B63C) against category 4 objects (1F800224), per category 4 type and area
- `80046d58 collidePlayerWithCat4Objects` (was func_80046D58) - player (when not holding, b9e 0/5/6) against category 4 objects, per type and area handlers
- `80047484 collidePlayerWithCat2Objects` (was FUN_80047484) - player against category 2 objects (1F80021C), per type and area handlers
- `80045f00 collideWeaponsWithCat2Objects` (was FUN_80045f00) - category 1 objects (weapons, layer-1 pool) against category 2 objects: weaponOnHitGetResult then the resolveHitResult* handlers
- `80045d98 collideWeaponsWithCat5Objects` (was FUN_80045d98) - category 1 objects with +0xA5 set against category 5 objects (weapons 5..7 test every active one, others only subtypes 0/2), FUN_80044b80
- `80046610 collideWeaponsWithCat4Objects` (was FUN_80046610) - category 1 objects against category 4 objects (weaponOnHitGetResult, per type handlers)
- `80049524 collideCat5WithCat4Objects` (was func_80049524) - category 5 objects against category 4 objects, per type and area
- `800425c4 weaponOnHitGetResult` (was func_800425C4) - switch on the weapon object type (0, 1, 4, 5..7, 8, 10): marks it as hit (active 2, recoil +0xA8 = 0x4FF ...) and returns the hit result kind the resolvers switch on
- `80030034 spawnPlayerPowerEffect` (was func_80030034) (uncertain) - spawns 1..6 type 0x4E objects at the player (sub = n, +0x0C = slot) after FUN_80111a50(n); called by playerTryAttack and playerStepSuperJump (n = 2)
- `80051088 drawWeaponObject` (was func_80051088) - category 1 render kind 0: draws the weapon head and its chain quads (weaponBuildChainBoxes from the D_8009C338 weapon state)
- `8005a014 weaponBuildChainBoxes` (was FUN_8005a014) - fills the D_8009C338 weapon state with the chain link boxes for the given dx/dy (MulCos/MulNegSinScaled)
- `8004cb00 spawnObjectFromRecord` (was FUN_8004cb00) - allocates and initialises one object from a 16-byte placement record (layer allocator by b1 & 0x7F, type/subtype/position/flags; see tools/objects.py)
- `800505f0 drawPlayerSpriteDefault` (was FUN_800505f0) - drawPlayer renderer for the default player type: uploads the sprite frame to VRAM (uploadSpriteFrameToVram) and draws it
- `8004bbc0 playerProbeGrab` (was func_8004BBC0) - probe for something to grab/bite ahead of the player: objects (per type/area callbacks) or terrain (b9e 5/6); the result D_1F8003C0 is what the jump/climb steps store in D_8009F0EC
- `80043260 pushObjectOutOfObjectBoxA` (was func_80043260) - box overlap of two objects (depth within 0x2D) and push of the first out along the shallower axis; 10 callers (contact handlers)
- `8004306c playerPushOutOfObjectBox` (was func_8004306C) - same push-out for the player: sets the wall-contact byte +0xA6 (2/3) on a side push, lands on top otherwise
- `800482ec pushObjectOutOfObjectBoxB` (was func_800482EC) - variant of the push-out used by collideCat2ObjectsPairwise for types 0xE/0x10 and area hooks
- `80041ebc probeFloorPlaneAt` (was func_80041EBC) - walks the collision plane list of getCollisionPlaneAt for the column and returns the floor (flags bit 0) height below y
- `80025aa8 copyModelPrimGroups` (was FUN_80025aa8) - copies the 8 primitive groups of a model (count words x per-group sizes 0x20..0x54) into the destination buffer
- `80029078 updateCameraScroll` (was func_80029078) - camera scroll step after the follow logic (ramped speed, look scroll input, section height); called by updateCameraFollow and two X000 section controllers
- `8004c1ec updateSectionSpawnManager` (was func_8004C1EC) - the unlayered section manager object (tools/objects.py): on entry spawnSectionObjects, on section change despawnOtherSectionObjects and the scroll spawners
- `8004c570 spawnSectionObjects` (was FUN_8004c570) - spawns the placement records of slot (section & 7) and the area-wide slot (min(sections,15), records whose b0 == section or ~section and whose persistent flag is clear) with spawnObjectFromRecord
- `8004c848 spawnSectionObjectsWithPrevious` (was FUN_8004c848) - as spawnSectionObjects for records with b0 >= 0, plus the negative-b0 records of the previous section when +0x0E > 0
- `8004c6c8 spawnScrollBackObjects` (was FUN_8004c6c8) - when D_8009C981 == 1 and +0x0E > 0: spawns the previous section's records with b0 < 0 / b0 == -section
- `8004d260 spawnSectionObjectsAlt` (was FUN_8004d260) - same body as spawnSectionObjects, called from func_8005B1A4
- `8004c418 despawnOtherSectionObjects` (was FUN_8004c418) - marks every object of the three pools (200 at 800A6610, 0x2D at 800B1828, 10 at 800A49A8) not in the current section (flag +0x0F bit 7 clear) for removal (active 2, +4 = 3)
- `8003bf80 updateThrownObject` (was func_8003F3D4, US name) (uncertain) - layer-1 type 1 handler (8007A110): 14 subtypes with box/sprite/anim from D_8007A148, spins (+0x8C) and flies by facing; category 1, so weaponOnHitGetResult treats it like a weapon (case 1)
- `80035428 weapon8to9HitWall` (was FUN_80035428) - weapon8to9Swing helper: on a terrain hit marks the weapon hit, impact effect (spawnImpactEffect / FUN_800eaffc by surface) and sfx
- `80028be0 cameraFollowRampStep` (was func_80028BE0) - updateCameraFollow helper: ramps the camera speed +0x24 by 0x80 toward the target (+0x73) and decays it
- `80019244 bootStartupTask` (was func_80019244) (no caller found; task pointer) - boot task: display init, boot logo, debug text, CD seek, analog pad mode, then the next task
- `80049cdc eventTakePlayerControlA` (was func_80049CDC) - scripted event object (jump table at 8004B3C0): when the player is in mode 1 step 0x30 state 2 takes it over (mode 5, step 0x41) and plays its anims
- `8004a124 eventCarryPlayerB` (was FUN_8004a124) - event object (jump table 8004B3D0): when the player is in step 4 state 3 mode 5, puts the player on it (y - 0x14, anim 0x2C, step 0x41) and moves with it
- `8004a4cc eventTakePlayerControlC` (was FUN_8004a4cc) - event object (8004B404): takes the player over like eventTakePlayerControlA and calls 800E8A98/96E8/940C by +0x6A
- `8004a9c8 eventTakePlayerControlD` (was FUN_8004a9c8) - event object (8004B414): same pattern as eventTakePlayerControlC
- `8004b7c4 probeGrabObjectBox` (was func_8004B7C4) - playerProbeGrab helper: tests the grab point (+0xE8/+0xEA) against an object's box (depth within 0x2D), per object type result
- `80027ed8 cameraFollowTargetY` (was FUN_80027ed8) - moves the camera Y toward the target 1F80018C with a 2..4 px dead zone and ramped speed
- `800288c4 cameraScrollInputStep` (was FUN_800288c4) - updateCameraScroll helper: look-scroll input (D_800A60D4) and the decay of 1F8000E6
- `800439f4 pushObjectOutOfObjectBoxC` (was func_800439F4) - another box push-out variant (two area hooks call it)
- `800489d8 pushObjectOutOfObjectFacing` (was func_800489D8) - box push-out where the first object's box is mirrored by its facing (animFrame bit 0)
- `80043464 objectBoxOverlapSide` (was FUN_80043464) - box overlap test returning -1 (no overlap) or the side of the overlap
- `8003ff48 tileCollideColumnAt` (was func_8003FF48) - TileCollideAt helper: walks one column's collision entries (D_8009BD70 count) for the floor/wall under the object
- `800271b8 cameraEventScrollStep` (was func_800271B8) - cameraEventScrollHandler helper: scripted camera pan steps (offsets +0x4C/+0x50 from 1F8000EE/F6)
- `8002a4d0 cameraEventScrollHandler` (was FUN_8002a4d0) - camera controller in the tables at 80079A9C.. and X000 80138424: scripted scroll back to the player with stopBgm; falls back to updateCameraFollow while GVARS.b38
- `8002be80 updateImpactEffect` (was func_8002BE80) - ObjAlloc-pool handler of type 0xF (pool type table 80079BAC[0xF]); the objects spawnImpactEffect creates: anim, sfx, category 3 list, freed at the end
- `8002fe78 updatePlayerEffect4E` (was func_8002FE78) - pool type 0x4E handler (80079CE4): the effects spawned by spawnPlayerPowerEffect and spawnPlayerFootstepEffect
- `8002b8c4 updatePoolType5DObject` (was func_8002B8C4) - pool type 0x5D handler (80079D20)
- `8002c43c updatePoolType12Object` (was func_8002C43C) - pool type 0x12 handler (80079BF4)
- `80044d54 weaponHitCat2Type02` (was FUN_80044d54) - collideWeaponsWithCat2Objects pair handler for category 2 type 2 (resolveHitResultE, awardAPByArea)
- `80044f18 weaponHitCat2Type0E` (was FUN_80044f18) - pair handler for category 2 type 0xE (weaponOnHitGetResult, impact effect, sfx)
- `80045344 weaponHitCat2Type07` (was func_80045344) - pair handler for category 2 type 7
- `800449a0 weaponHitCat4Type00` (was FUN_800449a0) - collideWeaponsWithCat4Objects pair handler for category 4 type 0
- `800447c0 weaponHitCat4Type03` (was FUN_800447c0) - pair handler for category 4 type 3
- `800450e8 weaponHitCat4Type39` (was FUN_800450e8) - pair handler for category 4 type 0x39
- `80044b80 weaponHitCat5Object` (was FUN_80044b80) - collideWeaponsWithCat5Objects pair handler (box test, resolveHitResultD, spawns a pool object)
- `80045684 playerTouchCat2Type0E` (was FUN_80045684) - collidePlayerWithCat2Objects handler for type 0xE (push-out + impact effect)
- `80045800 playerTouchCat2Type59` (was FUN_80045800) - collidePlayerWithCat2Objects handler for types 0x59/0x5A
- `80043d80 playerContactCat4Type0B` (was FUN_80043d80) - collidePlayerWithCat4Objects handler for type 0xB
- `80043ebc playerContactCat4Kind3` (was func_80043EBC) - collidePlayerWithCat4Objects contact kind 3 (inner switch, next to playerPushOutOfObjectBox for kind 0)
- `800544c8 drawObjectSprtWithMode` (was func_800544C8) - category 3 render kind 16 / category 7: frame parts as SPRT primitives wrapped in SetDrawMode (like drawObjectSprtTinted)
- `80047f3c pushCat2ObjectOutOfType04` (was func_80047F3C) - collideCat2ObjectsPairwise handler for solid type 4
- `80048500 pushCat2ObjectOutOfType07` (was func_80048500) - collideCat2ObjectsPairwise handler for solid type 7
- `8002c654 spawnPlayerPoolType12Object` (was FUN_8002c654) - when D_8009CEF8 != 0 spawns a pool type 0x12 object at the player (CLUT 0x80,499); called by playerInitMode and playerStepUseItem
- `8002c578 spawnWeaponPoolType12Object` (was FUN_8002c578) - spawns a pool type 0x12 object at x/y/z with the weapon's facing and D_8009C338 sprite data (weapon0Swing, weapon8to9Swing)
- `800339a4 weapon5to7UpdatePlayerStep` (was FUN_800339a4) - weapon5to7Swing helper: for camera modes 4/7 forces player step 0x18, else switches on the player step
- `80033834 weapon5to7StartSpin` (was FUN_80033834) - weapon5to7Swing helper: player anim (weaponSetPlayerAnimB), launch angle by frame, weaponFindNearestTarget
- `80033638 weapon5to7HitWall` (was FUN_80033638) - weapon5to7Swing helper: terrain hit, impact effect and sfx (like weapon8to9HitWall)
- `8002809c cameraFollowTargetYB` (was FUN_8002809c) - second camera Y follow (dead zone 1, ramp 0x8000) used by two camera controllers
- `80028250 lookScrollStepX` (was FUN_80028250) - updateLookScroll helper: ramps the scroll speed and calls stepScrollApproachX
- `80028754 cameraScrollToPlayerYA` (was FUN_80028754) - cameraScrollInputStep helper: Y offset of the player (camera mode 3 uses 800A606C - 0x14)
- `800285ec cameraScrollToPlayerYB` (was func_800285EC) - same calculation, second variant
- `8002eddc initPoolType32Object` (was FUN_8002eddc) - sets an object up as pool type 0x32 (anim PTR_80012208[+0xAC], sprite data 1F8002D8)
- `8002ef20 playerEffect4EInit` (was FUN_8002ef20) - updatePlayerEffect4E init: sprite/anim from D_80079D50 by subtype and slot +0x0C
- `8002f16c playerEffect4EStepA` (was func_8002F16C) - pool type 0x4E effect variant (0xB4 frames)
- `8002f384 playerEffect4EStepB` (was FUN_8002f384) - pool type 0x4E effect variant (0x3C frames)
- `8002f4e0 playerEffect4EStepC` (was func_8002F4E0) - pool type 0x4E effect variant (slots 5..8 at fixed angles)
- `8002f960 playerEffect4EStepD` (was FUN_8002f960) - pool type 0x4E effect variant (slots 0..2 at angles 0/0x56/0xAB)
- `8002fbc8 playerEffect4EDispatchA` (was func_8002FBC8) - switch on the 0x4E subtype over the step functions above
- `8002fc98 playerEffect4EDispatchB` (was FUN_8002fc98) - second dispatcher over the same step functions (called by updatePlayerEffect4E)
- `8002cd20 spawnPlayerPoolType13Object` (was FUN_8002cd20) - spawns a pool type 0x13 object copying the source position/facing (playerInitMode, playerEventStepDropIn)
- `80025d90 playerDecodePadInput` (was FUN_80025d90) - updatePlayer: decodes the pad / analog bytes into the 4-way direction bits (D_8009D670) and the press/release masks
- `800498f4 dispatchAreaNpcHandler` (was func_800498F4) - layer 2 type 0x32 handler (MAIN0 table slot 80077E64): switch on area/section calling the area overlay's NPC handler (updateBacoTownNpc, updateRaftReturnNpc, ...)
- `8003e960 updateL5Type09Item` (was func_8003E960) - layer 5 type 9 handler (slot 8007B334): item object (initItemObject), falls with TileCollideAt, sets its section flag when taken
- `8003edb0 updateL5Type0AItem` (was func_8003EDB0) - layer 5 type 0xA handler (slot 8007B338): item object variant (active 4, optional launch velocity)
- `8002b0bc updateL8Type01Object` (was func_8002B0BC) - layer 8 type 1 handler (slot 80079B8C): sprite slot, event start, movement l8Type01MoveStep, category 8 list
- `8002eb34 updatePoolType32Object` (was func_8002EB34) - pool type 0x32 handler (slot 80079C74): spawns random type 0x32 children (initPoolType32Object)
- `8002ca88 updatePoolType68Object` (was func_8002CA88) - handler at pool table slot 80079D4C (index 0x68): anim from D_800121F4 by subtype, placed at the player
- `80020828 setPlayerAxisPointers` (was func_80020828) (no caller found) - sets the player horizontal/depth pointers D_800A6078/7C to +0x10/+0x18 or swapped by 1F8001C8 bit 0, same for two other object lists
- `80041480 probeSlopeFloorAt` (was func_80041480, US func_80044694) - slope/floor probe over the collision plane segments of getCollisionPlaneAt
- `8004cfd8 updateSectionFadeController` (was func_8004CFD8) (uncertain) - unlayered handler next to updateSectionSpawnManager (slot 8007BF38): sets the fade mode D_8009C975 = 1 and runs the per-area table D_8007C178
- `800480d4 pushObjectOutOfObjectBoxD` (was func_800480D4) - another box push-out variant (depth within 0x2D)
- `8002e4b4 updateGrabSnout` (was updatePoolType20Object; psx_tomba name) - pool type 0x20 handler (slot 80079C2C)
- `8002b3c8 l8Type01MoveStep` (was FUN_8002b3c8) - updateL8Type01Object movement: timed move toward +0xC4 with Rand wobble
- `80029f1c cameraModeHandler2` (was func_80029F1C) - camera controller tables (7 slots per group at 80079A9C/AB8/AD4, X000 80138424; slot 0 = cameraEventScrollHandler): slot 2, decays the X speed and follows the target Y (cameraFollowTargetY/YB)
- `8002a0fc cameraModeHandler3` (was func_8002A0FC) - camera controller slot 3: cameraFollowPlayerX/Y
- `8002a258 cameraModeHandler5` (was func_8002A258) - camera controller slot 5: target Y follow (cameraFollowTargetY/YB)
- `8002a3d4 cameraModeHandler6` (was FUN_8002a3d4) - camera controller slot 6 of the first group: cameraFollowPlayerX/Y then clamps to the player for player modes < 4 or 7
- `80027810 cameraModeHandler6B` (was func_80027810) - slot 6 of the second group (80079AD0)
- `8002c73c updatePoolType66Object` (was func_8002C73C) - pool handler at table slot 80079D44 (index 0x66): anim D_80014764 placed at the player
- `8002c920 updatePoolType67Object` (was func_8002C920) - pool handler at slot 80079D48 (index 0x67): anim D_800121F4[subtype]
- `8002e6c4 updateEventCube` (was updatePoolType22Object; psx_tomba name) - pool type 0x22 handler (slot 80079C34): render kind 7, category 3 list
- `8004baa8 probeClimbGrabObject` (was FUN_8004baa8) - playerClimbFollowObject helper: probeGrabObjectBox over the list 1F800254; result in 1F8003C0
- `80041d20 probeSlopeAngleAt` (was func_80041D20) (no caller found) - collision plane probe returning the slope angle (angleFromDelta)
- `8002f060 playerEffect4EStepE` (was func_8002F060) (no caller found) - another pool type 0x4E effect step (anim from D_80079D58, follows the player's visibility)
- `8003c550 updateL1Type02Reward` (was func_8003F9A4) - layer 1 type 0x2 handler (MAIN0 table slot 8007A114); psx_tomba reward.c; spawns spawnPoolType05Ring
- `8003bcd0 updateL2Type30Object` (was func_8003F124) - layer 2 type 0x30 handler (MAIN0 table slot 80077E5C)
- `8004985c dispatchAreaEnemyDraw` (was updateL2Type02Object; psx_tomba name) - layer 2 type 0x2 handler (MAIN0 table slot 80077DA4)
- `80030afc updateL4Type1BObject` (was func_80033F50) - layer 4 type 0x1B handler (MAIN0 table slot 80079DE0)
- `80030404 updateL4Type08Object` (was func_80030404) - layer 4 type 0x8 handler (MAIN0 table slot 80079D94)
- `8002b0b4 updateL8Type07Object` (was func_8002B0B4) - layer 8 type 0x7 handler (MAIN0 table slot 80079BA4)
- `80055618 drawObjectMultiPartSprite3D` (was FUN_80055618) - drawCategory3Objects render kind 4: GTE-projected object position, up to 7 FT4 parts from the row table by WC6(o), part frames picked by o->wb4[i]
- `80055cb8 drawObjectSpriteParts` (was func_80055CB8) - render kind 7: screen-space FT4 parts at o->x/y + entry offsets (mirrored by animFrame bit 0), semi-trans from b0d
- `80052190 drawObjectSpritePartsRotated3D` (was func_80052190) - render kind 8: projected FT4 parts rotated by d8c through the sine table D_8007A3F0
- `80056e4c drawObjectSpriteScaled3D` (was func_80056E4C) - render kind 14: one projected FT4 scaled by wb8/100, grey tint wb4
- `80040f78 testCollisionSegmentList` (was func_80040F78) - walks the segment list at 1F800278 and dispatches by kind (1 testCollisionSegment, 2/4/8 below, 0x10 wall); stores the hit word in 1F800282
- `80040d30 testCollisionSegmentStep` (was FUN_80040d30) - segment kind 2 (stepped height h + k*((x-lo)%n)/n); `80040ac0 testCollisionSegmentSlopeA` / `80040bfc testCollisionSegmentSlopeB` (was func_80040AC0 / FUN_80040bfc) - kinds 4 / 8 (rising/falling slopes)
- `80040ea8 testCollisionSegmentWall` (was func_80040EA8) (no caller found) - kind 0x10 test that testCollisionSegmentList inlines
- `80041240 probeSlopeWallSideAt` (was func_80041240) - plane probe over the kind 4/8 segments: 1 = blocked by a kind-4 side, 2 = kind-8 side
- `8004211c probeFloorSegmentBelow` (was func_8004211C) (no caller found) - plane probe over kind 2 (non-wall) segments: 1 when the floor height at x is at or below y
- `800435e0 objectBoxOverlapSideFacing` (was func_800435E0) - objectBoxOverlapSide variant using b's facing (s2e bit 0); -1 none, 0/1 side, 2 top, 3 bottom
- `800437e0 pushObjectOutOfObjectBoxE` (was func_800437E0) - another push-out/land variant (lands a on b's top when within 0xC)
- `80044424 boxesOverlapSideB` (was func_80044424) - box overlap returning -1 / 0,1 (side by sign of dx) / 2,3 (above/below)
- `80045bac landObjectOnObjectBox` (was FUN_80045bac) (no caller found) - lands a on top of b (clears velY, sets b69 on both) or pushes it below; returns 1 / 3
- `8004886c pushObjectOutOfObjectFacingB` (was FUN_8004886c) - facing-aware vertical push-out (called by the X000 area-0/3 collision hooks)
- `8004b57c probeGrabPointInBoxA` / `8004b6a0 probeGrabPointInBoxB` (was FUN_8004b57c / FUN_8004b6a0) - identical grab tests of the point we8/wea against a box, write the hang offset to 1F8003BC (X000 callers)
- `8004b454 probeGrabObjectBoxOffset` (was func_8004B454) - playerProbeGrab case: grab box offset by D_8007B5E4[b0c]; sets b9e = 3 and 1F8003C0 = the object
- `80048c24 captureCat2ByCat4Box` (was func_80048C24) - cat-2 types 0x22/0x2C inside a cat-4 box are removed and the cat-4 object's state advances
- `80048cec dispatchAreaCollisionHookA` / `80048d3c dispatchAreaCollisionHookB` (was func_80048CEC / func_80048D3C) (no caller found) - area 0 / 3 overlay collision callbacks
- `80047a3c collidePlayerWithCat5Objects` (was func_80047A3C) (no caller found) - loops the cat-5 list calling collidePlayerWithCat5Object
- `80044290 playerPushOutOfObjectBoxThunk` (was func_80044290) (no caller found) - just calls playerPushOutOfObjectBox
- `8002c0e0 poolType12DirectionalStep` / `8002c238 poolType12FollowPlayerStep` (was FUN_8002c0e0 / FUN_8002c238) - updatePoolType12Object subtype 0 (placed at one of 8 directions around the d90 parent) / subtype 1 (follows the player, anim by player mode)
- `8002ee50 spawnPoolType32At` (was FUN_8002ee50) - allocates a pool type 0x32 object at x/y/z with size 0x10/0x18/0x20 by kind
- `8002ff20 spawnPlayerEffect4E` (was FUN_8002ff20) - allocates a pool type 0x4E effect at the player position
- `8002af10 callAreaModelLinkHook` (was FUN_8002af10) - calls the per-area PTR_DAT_80079b3c entry with the WMD data (area/section exceptions); see linkAreaModelBlock
- `8003a89c scriptOpSetPlayerStateEx` (was FUN_8003a89c) - the script player-state op that follows scriptOpSetPlayerState (extra 9/8,9 toggles of 8009C93E)
- `8003c338 hopSpinFallStep` (was FUN_8003c338) (no caller found) - hop up (velY -0x200), spin d8c by 0x14 and fall until probeCollisionAtDepthB hits
- `80024e58 modelDrawFT4WithG4Lit` (was FUN_80024e58) (no caller found) - modelDrawFT4 followed by modelDrawG4Lit on the result
- libpad internals without a psx_tomba name (behaviour names, underscore like the library): `8006bac4 _padInitDirSeq` (was FUN_8006bac4) - installs the three direct-sequence callbacks (psx_tomba padseqd.c _padInitDirSeq); `8006be6c _dirFailAuto` (was FUN_8006be6c) - padseqd.c _dirFailAuto with the PAL retry limit 10; `8006bf4c _dirChkSeqDone` (was func_8006BF4C) - third padseqd callback (US func_8006B494): 0 once the sequence step is 0xFF; `8006b720 _padLatchId` (was FUN_8006b720) - copies port +0x37 to +0x38 and clears it; `800695a8 _padChkSioIrq` (was func_800695A8) - both SIO IRQ bits set -> runs the D_800981D4 hook; `80069610 _padVsyncHandler` (was func_80069610) - US func_80069058, polls both ports; `80069ef8 _padSioExchangeByte` (was func_80069EF8) - waits for SIO ready, writes the control byte and waits for the ack with the RC2 timeout; `8006ad48 _padChkMainModeDone` (was func_8006AD48); `8006ae54 _padIfSioRW` (was func_8006AE54) - US padif.c func_8006A40C; `8006b020 _padSendCmdByte` (was func_8006B020)
- `8002ac68 updateCameraController` (was func_8002AC68) - updateInventoryOverlay's camera object tick: init (geometry screen 0x220, area 6 / 9-6 special cases), then the controller D_80079AEC[mode] and computeCameraMatrix, copies the camera position to 1F800174..
- `8002771c cameraClampAroundTarget` (was func_8002771C) (no caller found) - clamps the camera X to +-0x90 of its target and the player Y (0x5C in area 0x2000E) for player modes < 4 or 7
- `80017798 resetCameraScratchDefaults` (was func_80017798) (no caller found) - base matrix to 1F800BC0, default camera offsets -0x220/0xA0/-0x80 and scratch flags
- `80025cc4 menuAnalogAxisRepeat` / `80025c60 menuDigitalAxisRepeat` (was func_80025CC4 / func_80028794) (no caller found) - map an analog axis byte (dead zone 0x50..0xAF) / digital 0 or 0xFF to the menu repeat values of the mode
- `8002eaac spawnPoolType31At` (was func_8002EAAC), `8002ffb0 spawnPlayerEffect4EStep1` (was func_8002FFB0) (no caller found) - pool spawn helpers (type 0x31 timer 0x20; type 0x4E at the player, step 1)
- `800352f4 weapon8to9AttachAnchor` (was func_800352F4) - weapon8to9HitWall: sfx 0x26, anchors the player swing at the hit object (distance wb4 >= 0x28, angle w7a)
- `800337c0 weaponCheckNearestTarget` (was func_80036C14, psx_tomba objlogic.c) - weaponFindNearestTarget result 2 -> 1; `80034f04 weapon5to7SwingStep` (was func_80034F04) (no caller found) - calls weapon5to7Swing for states 0..2
- `8003395c isPlayerStateRestricted` (was func_80036DB0, psx_tomba objlogic.c) (uncertain) - 1 for the player states D_800A539D in a fixed list (5..69)
- `80036ebc setAngleFromDir8` (was func_80036EBC) (no caller found) - d8c from the direction index +0x2E (0/0x20/0xE0/0x40/0xC0) unless step is 6/7
- `800371b0 setUnusedGlobal8007A108` (was func_800371B0) (no caller found) - only writes D_8007A108 (read nowhere)
- `80038b04 scriptLoadHeader` (was func_80038B04) - clears the 64 script objects, checks the "WS" magic 0x530057 of the script block and copies its 64 header words
- `800399c4 scriptOpSpawnObject` (was func_800399C4) - execGameOpcode 0x80: allocates a layer-2 (kinds 2/9/10 -> types 0x18/0x19) or layer-4 (kind 4 -> type 0x1E) object at x/y/z and stores it in SCRIPT_OBJECTS[idx]
- `8003abc0 scriptOpReleaseType19` (was func_8003ABC0) (no caller found) - script object of type 0x19: release its +0x94 link or func_800EDFB8(o, 1)
- `8003af70 scriptOpReadResultBCD4` (was func_8003E3C4, psx_tomba scriptop.c) - stores D_8009BCD4 into the script argument +0x1190
- `8004d20c initSectionSpawnerAlt` (was func_8005B1A4, psx_tomba objpool.c) - first tick: records area/section, then spawnSectionObjectsAlt
- `8002b018 stubNop8002B018`, `8004f344 stubReturnZero8004F344` (was func_8002B018 / func_8004F344) (no caller found) - empty / return 0

# X000 function names (shared overlay code)

Player steps are the cases of the shared dispatcher `func_80100788` (`o->step`). Pad bits (8009D670): 0x10 Up, 0x20 Right, 0x40 Down, 0x80 Left; 1F8003C6 = configured jump button.

- `800f1478 playerStepIdle` (was FUN_800f1478) - player step 0 handler: standing state; Left/Right held switches to step 1, else idle/walk anim selection
- `800f1d44 playerStepWalk` (was func_800F1D44) - player step 1 handler (entered from step 0 when Left/Right is held)
- `800f2b98 playerStepJump` (was FUN_800f2b98) - player step 2 handler: jump sfx (2,4), rise extended while the jump button (1F8003C6) is held 5..14 frames, state 3 = falling
- `801099b8 playerStepClimb` (was FUN_801099b8) - player step 13 handler: climbing anims 10/11/12 (idle/up/down) chosen by pad Up/Down
- `8010d390 playerStepSwim` (was FUN_8010d390) - player step 0x3D handler: swim stroke anims 0x43/0x47 (swim_stroke / swim_stroke_slow)
- `80102a08 playerStep2EByArea` (was FUN_80102a08) - player step 0x2E: only dispatches to the area overlay's handler (area 1 or 10)
- `80106f98 playerStep3CByArea` (was FUN_80106f98) - player step 0x3C: only dispatches to the area overlay's handler (area 3 or 4)
- `800efa80 playerSelectGroundAnim` (was FUN_800efa80) - picks idle/walk/jog/run anim (0..3, 0x21 when crouching) from speed +0xB2 and loads it when it changed
- `800f00ac playerCheckGroundAndFall` (was FUN_800f00ac) - wall-contact step changes (0x10/0x17/0x1B/0x1E/0x21) then gravity when unsupported; past max fall speed switches to step 2 state 3 (fall)
- `800eff1c playerGetWallContactKind` (was FUN_800eff1c) - returns 1..5 from the contact flags +0xBE (and speed +0xB2 past 0x144); bit 0x20 forces step 0x1F
- `8010f328 playerSetJumpRiseVelocity` (was FUN_8010f328) - sets velY from the per-mode jump table D_80115468 by the jump-hold counter (player +0x20 <= 12 or 13)
- `8010e628 playerStartFall` (was func_8010E628) - sets step 2 state 3 (jump step, falling) with the knock-back timer/flags
- `8010beb8 playerTryStartJump` (was func_8010BEB8) - if the jump button (1F8003C6) is pressed: step 2 state 0 and returns 1
- `800ef490 setFacingFromPad` (was FUN_800ef490) - animFrame bit 0 from Right (0x20) / Left (0x80) held; neither keeps facing and sets bit 1
- `800ef4e4 setFacingAndUpDownFromPad` (was FUN_800ef4e4) - as setFacingFromPad plus bit 3 for Up (0x10) and bit 2 for Down (0x40)
- `800eea7c playerSetAnim` (was FUN_800eea7c) - sets player anim id, loads table[id] and starts at the given frame unconditionally
- `800eeae4 playerSetAnimIfChangedAt` (was FUN_800eeae4) - PlayerSetAnimIfChanged with an explicit start frame
- `800ef460 objApplyDepthVelocity` (was func_800EF460) - adds velH to the depth coordinate (*o->d) and velV to Y (16.16)
- `800fc104 updatePlayerHealth` (was func_800FC104) - copies +0x98 health to GAME.playerHealth when it changed; at 0: game-over flags, stopBgm, playJingle(3)
- `800fbcec setLeapVelocityFromPad` (was func_800FBCEC) - sets velX/velY launch (-0x600 up) by facing, stronger/weaker with Left/Right held
- `8010d7f0 setAngleSpeedFromDir8` (was func_8010D7F0) - +0x7A = (+0x76 & 7) * 0x20 angle, +0x74 speed 0x200 unless bit 3 of +0x76
- `8010cfb0 setAngleSpeedFromDir8B` (was func_8010CFB0) - byte-identical behaviour to setAngleSpeedFromDir8 (second copy)
- `8011133c setVelXFromPadLR` (was FUN_8011133c) - velX = -/+0x200 for Left/Right held, else 0 (scaled down in mode 7)
- `801113bc adjustVelYFromPadDown` (was FUN_801113bc) - velY += 0x20 while Down held, else -= 0x20; clamped to 0xC0..0x300
- `800ec84c spawnColorBurst` (was FUN_800ec84c) - spawns 8 objects of type 0x5F around a circle (angle i*0x20) tinted red/blue/green by the argument
- `800ef150 playerResetStep` (was FUN_800ef150) - SfxPlay3(0x1C,0x7F), clears the player action flags and sets step 0 state 0
- `801097ec playerStepPush` (was FUN_801097ec) - player step 0x1C: while Left/Right held moves at 0x200 with anim 0x20 and sfx 0x24 every 16 frames; leaves when contact flag +0xA6 clears (step 1) or the pad is released (step 0)
- `800f262c playerJumpRiseUpdate` (was FUN_800f262c) - jump-hold logic of the jump step state 1: counts frames while the jump button (1F8003C6) is held (max 14, min 5) then calls playerSetJumpRiseVelocity
- `800f0374 playerFallIfUnsupported` (was FUN_800f0374) - tile collision, then gravity (+0x223) when nothing supports the player; returns 1 once the fall speed passes 0x447
- `800f1308 playerAnimateTalkPose` (was FUN_800f1308) - sets anims 0x4B..0x4F by TALK_POSE (1..5) and advances them
- `800f2724 playerUpdateFall` (was FUN_800f2724) - falling state of the jump step: anim 4, air steering (FUN_8010eaf8), velX, gravity step
- `8011116c playerDecelerateSpeed` (was FUN_8011116c) - reduces |speed +0xB2| toward 0 by the per-mode friction table D_80115510 (zero inside +-0x50)
- `800ee948 spawnWeaponObject` (was FUN_800ee948) - if the active weapon objects (+0xE3) are under the limit D_8007A038[weapon], allocates a layer-1 object of type = equipped weapon (D_8009D2B2)
- `800f44b0 playerStepWeaponAttack` (was FUN_800f44b0) - player step 3: 8-way aim from pad (Up+Left/Right...), spawns the equipped weapon object (limit D_8007A038)
- `800f36e0 playerStepAirWeaponAttack` (was FUN_800f36e0) - player step 4: weapon object spawn as in step 3 with the jump rise/fall logic (entered from step 3 when falling)
- `80104698 playerStepBitten` (was func_80104698) - player step 14: entered with wac == 2; state 0 attaches to the bitten object D_8009D2E8 by its type, then per-type holders
- `801066c4 playerStepBittenJump` (was func_801066C4) - player step 15: jump (anim 13, playerSetJumpRiseVelocity) carrying the bitten object D_8009D2E8, per-type follow code
- `800f5078 playerStepSpinOnObject` (was func_800F5078) - player step 5 (entered from playerEnterGrabStep, contact kind 3): spins around the grabbed object D_8009F0EC (types 14/66) by angle +0xB6; Left/Right raise the spin level, jump button releases to step 9
- `800f705c playerStepSwingFromObject` (was func_800F705C) - player step 6 (contact kind 2): pendulum below the object D_8009F0EC (radius 0x18 around angle 0xC0); jump/Up -> step 11, Down drops (playerSwingHandlePad)
- `800f8d2c playerStepFlipOntoObject` (was FUN_800f8d2c) (uncertain) - player step 7 (contact kind 1): decaying spin with anim 7 that ends standing on the object D_8009F0EC, following its rotation; jump/Up -> step 11, Down drops to the fall state
- `800f67bc playerStepSpinRelease` (was func_800F67BC) - player step 9 (entered from step 5 on jump): launch velocity from table D_801153C0 indexed by the spin level +0xB2, then lands into step 2 state 3
- `800f7aac playerStepPerchLeap` (was func_800F7AAC) (uncertain) - player step 11 (from steps 6/7/12 on jump/Up): plays anim 9 until frame 0x61, then leaps (velY -0x300) away with a spin (+0x88) and falls back into the jump step
- `800f98a8 playerStepFlipOntoPerch` (was func_800F98A8) (uncertain) - player step 12: same decaying spin as step 7 around a fixed point (own position), ends standing (anim 7); jump/Up -> step 11, Down -> climb step 13 (anim 10)
- `801075d8 playerStepSlopeSlide` (was FUN_801075d8) (uncertain) - player step 0x10 (playerGetWallContactKind 1: surface flag 0x02 at speed > 0x144): slides with speed clamped +-0x144, pad vibration and particles; bump bounces (anim 0x12); jump leaves to step 2
- `80107fcc playerStepDepthShift` (was func_80107FCC) (uncertain) - player step 0x20: anim 0x2B, waits for GCD.bcda2 == 0, then moves 5 units per frame along the depth axis (sign from 1F8001C8 bit 0) for 0x78/5 frames
- `801073c0 playerSlopeSlideUpdateAnim` (was FUN_801073c0) (uncertain) - step 0x10 helper: while |speed| > 0x144 anim 0x11 with growing roll (+0x6B) and an area sfx every 32 frames, else back to step 1 with the ground anim
- `801102b4 playerSlopeSlideAccelerate` (was func_801102B4) (uncertain) - step 0x10 helper: ramps speed +0xB2 toward the D_80115510 table limits by surface direction (+0xBE bit 0) and facing; table index as playerGetSpeedTableIndex
- `80110eec playerAccelerateDownhill` (was FUN_80110eec) (uncertain) - adds to speed +0xB2 toward +-0x500 in the direction of surface bit +0xBE&1 (bigger steps near 0); used by the slide steps 0x17 and 0x1B
- `8010e918 playerGetSpeedTableIndex` (was func_8010E918) - returns the speed-table row: GV2.bb3 + 4 * (+0xC1), forced to (+0xC1)<<2|3 by GAME.displayExpBar&3 and to 8 by two GCD flags; same index computation as playerSlopeSlideAccelerate
- `800e9f74 awardAPByArea` (was FUN_800e9f74) - switch on the area calling that overlay's popup function with (points,x,y,z,1); the X000 one (spawnApAwardNumber) calls addPlayerAP and spawns the digit object; callers pass 500 on item pickups
- `800ee454 playerTiltTowardSlope` (was func_800EE454) - moves the sprite angle +0x8C toward D_801152E8[slope +0xB0] by 4/2/1 per call (wrapping byte angle)
- `800ee51c playerTiltTowardSlopeSlow` (was func_800EE51C) - moves +0x8C toward D_8011530C[slope +0xB0] by 1 per call
- `800ee4e0 playerSetSlopeTilt` (was FUN_800ee4e0) - +0x8C = D_8011530C[+0xB0] or D_801152E8[+0xB0] by the flag argument
- `800eea3c playerSetAngleFromSlope` (was func_800EEA3C) - +0xB6 = slope +0xB0 * 4 as a byte angle (0 when flat)
- `800ee5c8 playerAdvanceSpinAngle` (was func_800EE5C8) - adds the spin speed (player state +0xE) to the spin angle +0xB6 and sets the half-turn flag (state +8); same as the rot() inline of the spin steps 5/6/7/12
- `800eee40 playerReleaseBitten` (was func_800EEE40) - if the player holds an object (+0xAC >= 2) sets the bitten object D_8009D2E8 (= PLX.p611c) bytes 4/5/6 to 2,2,0; clears +0xAC
- `800eee90 playerReleaseBittenAndClearFlags` (was FUN_800eee90) - playerReleaseBitten plus clearing GVARS.i34, +0xC7 = 1, +0x9D/+0xC6/+0xE3 and the player state byte 0
- `800ef62c setWeaponAimFromPad` (was func_800EF62C) - 8-way aim into animFrame (0/1 sides, 2/3 neutral, 4/5 Up+side, 6/7 Up) from pad Up/Left/Right, keeping facing bit 0
- `800ef588 setWeaponAimFromPadTurn` (was func_800EF588) - as setWeaponAimFromPad but without Up Left/Right only turn (3/2) instead of side aim
- `800ef6d8 setWeaponAimFromPadKeepFacing` (was FUN_800ef6d8) - aim 2/3, 4/5 (Up+forward) or 6/7 (Up) without changing facing
- `800f43d0 offsetByWeaponAim` (was func_800F43D0) - moves the object by the stored aim (player state +7): forward+down for 0..3, back-up for 4/5, up for 6/7; clears +0x69
- `800efc8c playerEnterGrabStep` (was func_800EFC8C) - after a contact probe: releases a bitten object, then by contact kind +0x9E enters step 7 (1), 6 (2), 5 (3, attaches to D_8009F0EC) or others
- `800f2a30 playerLandFromJump` (was func_800F2A30) - jump-step landing: sfx 0x1C, releases a bitten object, friction 0xD0 on velX moved into speed +0xB2, sprite angle from slope table
- `800f4ed0 playerSpinPlaceAndAccelerate` (was FUN_800f4ed0) - step 5: places the player on the circle (radius state +1) around pivot +0x30/+0x34 at the spin angle, sets +0x8C and adds/subtracts the spin acceleration
- `800f6f38 playerSwingPlaceOnCircle` (was FUN_800f6f38) - step 6: places the player on the swing circle around the pivot plus offset +0xB8/+0xBA, sets +0x8C
- `800f9548 playerFlipPlaceAndAccelerate` (was FUN_800f9548) - step 12: as playerSpinPlaceAndAccelerate around the fixed point +0x30/+0x34
- `800f8b7c playerFlipOntoObjectPlace` (was FUN_800f8b7c) - step 7: pivot = D_8009F0EC position + offset, circle placement plus the object's own rotation (+0x8C, box0 radius)
- `800f6e28 playerSwingUpdatePivot` (was FUN_800f6e28) - step 6: pivot +0x30/+0x34 from D_8009F0EC; per object type (0xE offset, 0x14/0x34 area callbacks, 0x1F facing check) returns 1 to let go
- `800f6c34 playerSwingHandlePad` (was FUN_800f6c34) - step 6 pad: jump or Up -> step 11, Down lets go (jump step state 3, nudged 14 units)
- `800f6d80 playerDropFromHold` (was func_800F6D80) - the Down branch of playerSwingHandlePad as a function: clears the hold and spin state, faces +0x88, nudges 14 units back and clears D_8009F0EC+0x69 (the caller sets the step)
- `80102214 updatePlayer` (was FUN_80102214) - player tick: pad/flag prologue then switch on the mode byte +0x04 (0 init FUN_800f0490, 1 normal steps func_80100788, 2 hurt steps, 5 event steps, 6 forced steps); game over (GVARS.b38) forces mode 2 step 2 state 2
- `80101780 playerHurtModeDispatch` (was FUN_80101780) - standalone copy of updatePlayer's mode 2 block: step 0..4 -> the five hurt/death steps below, then the collision pass FUN_8003f7cc
- `80101d74 playerEventModeDispatch` (was func_80101D74) - updatePlayer mode 5: event steps (0x40 hidden during area exits, 0x61..0x65 talk poses with playerAnimateTalkPose, step 1 timer back to mode 1 step 0)
- `80102110 playerForcedModeDispatch` (was FUN_80102110) - copy of updatePlayer mode 6 (entered from scripted NPCs like updateCharlesTalkScene with a preset velX): steps 0,3,4,5,6,7 without pad control
- `800fd358 playerStepHurtKnockback` (was FUN_800fd358) - hurt step 0: velY -0x500, hurt anim 0x39/0x53/0x50, pad vibration, health update (death jingle at 0), blink while flying back, lands into playerHurtLandRecover
- `800fdb7c playerStepHurtStun` (was FUN_800fdb7c) - hurt step 1 (damage type 4 in func_800425C4): holds the hurt pose 0x1E until state 2 is set from outside, then bounces off (velY -0x400, velX +-0x200)
- `800fe13c playerStepHazardRespawn` (was FUN_800fe13c) - hurt step 2: spins up (velY -0x800) and is put back at the saved spot +0xEC/+0xF0 with invulnerability +0xE0 = 0x8C; on game over plays the 180-frame death pose (anim 0x3C) and sets GVARS.b38 = 2
- `800fe75c playerStepDie` (was FUN_800fe75c) - hurt step 3 (entered when health <= 0): game-over flag GVARS.b38 = 1, stopBgm, death jingle 3, waits 0xB4 frames then GVARS.b38 = 2 (except area 10 section 0)
- `800fe98c playerStepHurtSpin` (was FUN_800fe98c) - hurt step 4: horizontal knock (velX +-0x200), anims 0x2E then 0x10 while +0x8C spins, health update, lands into playerHurtLandRecover
- `800fc414 playerHurtLandRecover` (was FUN_800fc414) - shared last state of the hurt steps: substep tumble (anim 0x10/0x57) until landing, then back to mode 1 once GVARS.b38 == 0
- `800fc240 playerHurtCheckFastFall` (was func_800FC240) - while hurt, once velY reaches 0x680 drops back to mode 1 falling (step 2 state 3) or to the swim steps 0x3D/0x3E in the water rooms
- `800ff610 playerHandleHazardContact` (was FUN_800ff610) - handles the hazard byte +0xA1 (1 harmful floor/pit, 2 water): per-area respawn spot (+0xEC/+0xF0), damage from getHazardDamage, switches to hurt step 2 (playerStepHazardRespawn) or to swim step 0x3D
- `800ff404 getHazardDamage` (was func_800FF404) - damage amount for playerHandleHazardContact by GAME byte +0x30 & 3 and a few area/section exceptions
- `800ff0e8 playerUpdateWaterLevel` (was func_800FF0E8) - per area/section water surface: sets +0x56 (water level) and the push GVARS.s44/s46 (-0x40 upward when below the surface); skipped in the swim steps 0x3D/0x3E/0x42..0x48
- `800fe8f8 playerHurtMoveAndBlink` (was func_800FE8F8) - hurt helper: adds GVARS.s44/s46 push and velX/velY to the position, blinks with 1F8001F8 bit 0 unless D_8009C330->b8
- `80100788 playerNormalModeDispatch` (was func_80100788) - updatePlayer mode 1: the switch on o->step calling every normal player step, then collision, playerHandleHazardContact and the tile shape probe
- `800fb420 playerStepRopeSwing` (was FUN_800fb420) - player step 0x32: pendulum around +0x30/+0x34 with radius +0xB4 and angle +0x84 (anim 0x31/0x34); Left/Right at the ends raise the swing level +0xB2
- `800fb0c0 playerRopeAdjustLength` (was FUN_800fb0c0) - rope step helper: Up/Down accelerate the rope length (+0x38, clamped 0x28..0x78) into +0xB4
- `800fb280 playerRopeGetSwingSpeed` (was FUN_800fb280) - rope step helper: angle increment (2/3, 4 when D_8009C330[10]) added to +0x84 each frame
- `800fb370 playerRopeSetAnim` (was FUN_800fb370) - sets the rope anim 0x31 with the frame taken from the swing angle (frame 15 when the rope is shortest)
- `800fadec playerRopeCheckRelease` (was func_800FADEC) - rope step helper: jump button lets go (step 2, 0x49 or 0x37 by headroom probes); landing on ground/object returns to step 0/1
- `800fbdb4 playerStepRopeJumpOff` (was func_800FBDB4) - player step 0x37: launch velocity from the swing angle sector and Left/Right, jump sfx, then jump step 2 state 2
- `800fa01c playerStepPerchIdle` (was func_800FA01C) - player step 0x1D: standing on a perch (anim 7); jump/Up -> playerStepPerchLeap (11), Down -> climb step 13, like the end of steps 7/12
- `800fa91c playerStepGrabbedStruggle` (was func_800FA91C) - player step 0x40: held in pose 0x1E; more than 10 pad presses break free (state 3, jump sfx); damage while held goes to hurt/die
- `8010b444 playerStepForcedJump` (was func_8010B444) - forced-mode step 4: jump without pad control (anim 4, jump sfx, playerSetJumpRiseVelocity for 5 frames), lands into the idle anim
- `8010b050 playerStepForcedLaunch` (was func_8010B050) - forced-mode steps 5/7: same rise as playerStepForcedJump with anim 0x2E and the preset velX (set by updateCharlesTalkScene)
- `80109920 playerStepForcedHold` (was FUN_80109920) - forced-mode step 3: clears motion and holds pose 0x1E
- `800f8130 playerStepTossed` (was FUN_800f8130) - forced-mode step 6: follows the anim offset until frame 0x87, then flies off (velY -0x300) rotating, lands (sfx 0x1C) and waits in event step 1
- `8010c034 playerStepClingToObject` (was FUN_8010c034) - player step 8: kept at offset +0xB8/+0xBA of the object D_8009F0EC (anim 80010D68), turning walks round it; per-type release
- `8010b880 playerStepClingWeaponAttack` (was func_8010B880) - player step 0x18: weapon attack (spawnWeaponObject, aim from pad) while clinging to D_8009F0EC, back to step 8/0x22/0x46 by object type
- `80109038 playerStepLaneJump` (was func_80109038) - player step 18: jump (anim 0x19, velY -0x570) that moves the depth coordinate to the next 90-unit lane
- `80108708 playerStepLaneShiftFall` (was func_80108708) (uncertain) - player step 17: drifts 5 units per frame along depth for 0x5A frames (anim 0x15/0x16) then falls into the next lane
- `80101938 playerEventStepLandIdle` (was func_80101938) - event steps 0/6: falls with anim 4 until landing, then stands (talk pose or idle anim)
- `8010186c playerEventLandReset` (was FUN_8010186c) - landing reset for the event steps: idle anim 80010748, clears speeds/anim cache, state++
- `800ee428 playerSetFallLean` (was FUN_800ee428) - +0x20 = 10, +0x88 = 0x10/0xF0 by facing, +0x84/+0x8C = 0: the lean set before entering the fall state (step 2 state 3)
- `800ee88c playerStickToGround` (was FUN_800ee88c) - when not airborne (+0xAD == 0) pushes y down 3/5 or 8/12 px (by substep and speed) to follow the ground
- `800ee9cc playerMoveByAngleSpeed` (was FUN_800ee9cc) - velocity from angle +0xB6 and speed +0xB2, adds it to the position and applies ObjApplyVelocity
- `800f86e8 playerStepVerticalJump` (was func_800F86E8) (uncertain) - player steps 0x16/0x49: straight-up jump (velY -0x340, -0x580 for 0x49) with anim 0x1F, lands or bites (step 0xE)
- `8010ca18 playerStepClimbObject` (was FUN_8010ca18) - player steps 0x23/0x2F: attached to the object D_8009F0EC (anim 80011148); Up/Down climb (anims 0x26/0x27), Left/Right swaps sides (anim 0x38), jump button lets go
- `80108ad0 playerStepWalkIntoDepth` (was func_80108AD0) - player step 0x31: anim 0x2C while the depth coordinate moves 2 per frame for 0xB4 frames (area/section specific timings), e.g. walking through a passage into the background
- `80109494 playerEventStepLaneJumpBack` (was FUN_80109494) - event step 2: jump (anim 0x19, velY -0x570) moving the depth coordinate back to the lane +0xF6; in area 0 with a pitch-slide sfx and startAreaBgm; lands into mode 1
- `80108168 playerEventStepDepthShift` (was func_80108168) - event step 7: event copy of playerStepDepthShift (anim 0x2B, waits for D_8009CDA2 == 0, depth +5 per frame for 0x5A/5 frames)
- `80108dbc playerEventStepWalkIntoDepth` (was FUN_80108dbc) - event step 9: event copy of playerStepWalkIntoDepth (anim 0x2C, depth +-2 per frame for 0x5A/2 frames), then mode 1
- `8010ac90 playerEventStepRiseFadeOut` (was FUN_8010ac90) - event step 5: releases what is held, rises (velY -= 0x30) for 0x1E frames, starts the fade (D_8009C975 = 3) and after 0x3C frames sets the task mode 7
- `8010ae94 playerEventStepRiseUp` (was FUN_8010ae94) - event step 12: rises with anim 4 until y < -0x20C, completes event 0xA4 and goes to event step 2
- `8010a9f4 playerEventStepDropIn` (was func_8010A9F4) - event step 4: falls in from above (velY 0x1000 slowing to 0x200) until y reaches +0xF2, lands (sfx 0x1C) and returns to mode 1
- `80108338 playerEventStepJumpIn` (was func_80108338) (uncertain) - event step 8: entrance jump (velY -0xA00, velX +-0x100) in areas 2/0x13 section 1 (area 4: in place), anim 4, lands into mode 1; other areas go straight to falling
- `800eec40 playerAirSpinToFall` (was func_800EEC40) - airborne helper: turns +0x8C 4 per frame back toward upright (anims 0xFA..0xFC rock +-0x20 instead) and switches to the fall anim 80010AE8 once upright
- `800ef1ac playerCheckPerchLanding` (was FUN_800ef1ac) (uncertain) - perch/climb/swing helper: on landing (+0x69) goes to step 0, or to the slope step 0x1B with anim 8/0x11 when +0xBE bit 3 is set; landing sfx 0x1C
- `8010e328 playerTryAttack` (was func_8010E328) - on the attack button (1F8003C8): weapon object limit D_8007A038[weapon], then by caller mode 0 ground attack step 3 (or 0x2A/0x2B), 1 air attack step 4, 2 cling attack step 0x18; first tries playerTryTalkToNpc
- `8010e114 playerTryTalkToNpc` (was func_8010E114) (uncertain) - on button 1F8003C4 scans the category 2 draw list for talkable types (0x18, 0x23, 0x32, 0x37, 0x4D, 0x55, 0x58..0x5A) in range and facing; sets their +0x68 to 4/5 and returns it
- `8010eaf8 playerAirAccelerateX` (was FUN_8010eaf8) - airborne horizontal control: accelerate/limit velX by the speed table D_8011545C per direction bits in +0x2E, wall contact +0xA6 blocks, otherwise decelerate
- `8010d888 playerStepSwimUnderwater` (was func_8010D888) - player step 0x3E: 8-direction swimming (anim 0x44, stroke 0x45 on button 1F8003C4 = speed 0x400), jump button -> step 0x44, reaching the surface (+0x56) -> surface swim step 0x3D
- `8010d048 playerSwimDirFromPad` (was FUN_8010d048) - turns the pad bits into the 8-way swim direction code +0x76 (bit 3 = idle after 10 frames), snapping the heading +0xB6 when reversing
- `8010d280 playerSwimMove` (was FUN_8010d280) - clamps the swim speed +0xB2 to 0..0x400, applies the current GVARS.s44/s46 and the heading velocity, then the tile collision probes
- `8010dd8c playerStepSwimSpin` (was FUN_8010dd8c) - player step 0x44: underwater spin (anim 0x1D, +0x8C rotating, velY 0x300 decaying) for 0x78 frames, biting goes to step 0x48, then back to step 0x3E
- `8010df84 playerEventWalk` (was FUN_8010df84) - event step 1/10 helper: walks with the ground anim (playerSelectGroundAnim/playerMoveByAngleSpeed), footstep sfx every 8 frames; on +0xAF >= 2 picks the door anims and returns to step 0
- `8010c7b0 playerClimbFollowObject` (was FUN_8010c7b0) - playerStepClimbObject helper: looks up the climbable object at hand height (FUN_8004baa8) and sticks to it (offset +0xB8/+0xBA); none -> lets go into the fall state; contact 11 -> step 11
- `8010c8d4 playerClimbJumpOff` (was FUN_8010c8d4) - jump button while climbing an object: releases it, steps 0x10 sideways by Left/Right (or turns), then jump step 2
- `8010db0c playerStepWaterJumpOut` (was FUN_8010db0c) - player step 0x43: leaps out of the water (velY -0x780, jump sfx 2/4 and splash sfx 0xE), can grab objects on the way, falls into step 2 state 3
- `8010bd08 playerReleaseClingObject` (was func_8010BD08) - lets go of the clung object D_8009F0EC: restores its active byte (per type, chained objects via +0x94) and moves the player 0x10 away
- `800f0490 playerInitMode` (was FUN_800f0490) - updatePlayer mode 0: loads the weapon TIM (weapons 7/8/9), health from GAME (+0x98/+0x9A), sprite data, per-area init, then the entry step from the spawn data (mode 5 event steps or mode 1)
- `800fe88c playerExitHurtToFall` (was func_800FE88C) (uncertain, no caller found) - invulnerability +0xE0 = 0x8C, lean +0x88 by facing and mode 1 step 2 state 3 (falling)
- `800fa35c playerHurtTumbleGetUp` (was FUN_800fa35c) (uncertain, no caller found) - substep variant of playerHurtLandRecover: tumble (anim 0x10) until landing, anim 0x2F then get-up anim 0x30, back to mode 1 step 0
- `80102ac0 playerBiteHoldType02` (was func_80102AC0) - playerStepBitten holder for bitten objects (D_8009D2E8) of type 2 / 0x21
- `80102f40 playerBiteHoldType03` (was func_80102F40) - playerStepBitten holder for types 3 / 0x12 / 0x2B
- `80103d40 playerBiteHoldType0E` (was FUN_80103d40) - playerStepBitten holder for type 0xE (and 0x29 when its +0x6A == 1)
- `801040d0 playerBiteHoldType04` (was func_801040D0) - playerStepBitten holder for types 4 / 0x29
- `801037d8 playerBiteHoldGeneric` (was func_801037D8) - playerStepBitten holder shared by types 0, 8, 0xA, 0x13, 0x1A, 0x1E, 0x1F, 0x27, 0x28, 0x38..0x42
- `80103290 playerBiteHoldType0B` (was func_80103290) - playerStepBitten holder for types 0xB / 0x1C
- `80122a18 playerBiteHoldType15` (was FUN_80122a18) - playerStepBitten holder for type 0x15
- `80104454 playerBiteHoldType18` (was FUN_80104454) - playerStepBitten holder for type 0x18
- `80104f88 playerBittenJumpGeneric` (was func_80104F88) - playerStepBittenJump state 2 for the generic bitten types (0, 2, 8, 0xA, 0xB, 0x1E, 0x21, 0x28, 0x38..0x42)
- `80105bcc playerBittenJumpType03` (was FUN_80105bcc) - playerStepBittenJump state 2 for types 3, 0x12, 0x1C, 0x1F, 0x2B
- `801064d0 playerBittenJumpType0E` (was FUN_801064d0) - playerStepBittenJump state 2 for type 0xE (and 0x29)
- `80105694 playerBittenJumpType04` (was func_80105694) - playerStepBittenJump state 2 for type 4
- `800f3f3c playerStepAttackJump` (was func_800F3F3C) (uncertain) - player step 10: jump with hold-to-extend (playerSetJumpRiseVelocity, sfx 2/4) and air control; at the apex or on a head hit goes to step 4 state 3 (playerStepAirWeaponAttack), to step 0xE when it bites (wac == 2)
- `80106fec playerStepSkidStop` (was func_80106FEC) (uncertain) - player step 0x13 (entered from the slide steps when contact flags +0xBE bit 1): anim 0x1A (0x2A with the area flag 0x40 and button 3C4), moves on the slope, back to walk/idle by pad and frame, jump allowed
- `80109e50 playerStepRiseToCeiling` (was FUN_80109e50) (uncertain) - player step 0x15: anim 0x1B, then rises 1.5 px/frame (falls with gravity in state 3) until probeFlaggedSegmentA finds flag 2 above, then mode 1 step 0x16
- `80122f14 playerStepSlideDownSurface` (was func_80122F14) - player step 0x17: slides downhill on a surface flagged in +0xBE (anim 0x11, playerAccelerateDownhill), leaves to step 0 when the flag clears, 0x13 on bit 1, falls when unsupported
- `80107c00 playerStepSlideSteer` (was func_80107C00) - player step 0x1B: slide on a +0xBE surface with Left/Right turning (anim 8 or 0x11 by facing vs slope), can grab an object (func_8004BBC0) when it ends, else falls (step 2 state 3)
- `80106b50 playerStepItemReactionA` (was FUN_80106b50) (uncertain) - player step 0x2A, set by playerStepUseItem for item 0x82 when D_8009D2B1 == 1: anim 0x28, sfx 0x20, timed (0x3C frames hidden with +0xAB bit 7, else 0x28), decelerates, back to step 0
- `80106db0 playerStepItemReactionB` (was FUN_80106db0) (uncertain) - player step 0x2B (item 0x82, D_8009D2B1 == 2): anim 0x29/0x36, sfx 0x25, pushed at 0x280 against the facing for 0x1E frames, back to step 0
- `800ffec4 playerStepUseItem` (was FUN_800ffec4) - player step 0x30: switch on the used item id (removeItemFromInventory, spawnColorBurst for items 0x62..0x64, item 0x82 -> steps 0x2A/0x2B, others set events/steps 5/0x3E)
- `8010e66c playerStepSuperJump` (was func_8010E66C) - player step 0x45: launches upward (velY accelerating to -0x800, sfx 2/4, spawnPlayerPowerEffect(2)) with air control, then falls (step 2 state 3) on a head hit or when velY turns positive
- `801274cc enemyProbeGround` (was FUN_801274cc) - AREA00 enemy helper (38 calls): floor probe at y+0x10, sets the slope angle +0x8C / speed +0xB2 from the probe, returns 1 when supported
- `801275e4 enemyCheckWallAhead` (was FUN_801275e4) - AREA00 enemy helper: probeWallAt 0x10 px ahead along the slope direction; 2 for subtype 9 at a fixed spot
- `80127720 enemyPickActionByPlayerDistance` (was FUN_80127720) - returns 0xFF when the player is out of range, else a random entry (Rand & 0xF) of one of three action tables by horizontal distance
- `8010f400 playerUpdateSpeedFromPad` (was FUN_8010f400) - ground speed +0xB2 from the pad with the per-mode acceleration table D_80115510 (0x74-byte rows): accelerate, decelerate, turn; used by idle/walk/jump/push steps
- `800eef0c spawnPlayerFootstepEffect` (was FUN_800eef0c) - by the floor kind D_8009C990 spawns a type 0x4E effect at the player's feet (sub = kind - 1); returns the effect kind
- `800eae0c spawnGroundDebris` (was FUN_800eae0c) (uncertain) - spawns a type 0xC object at x/y/z (areas 0/1/9), sometimes a random drop through FUN_80114968
- `800ec710 spawnWeaponTrailObjects` (was FUN_800ec710) - by weapon type (0, 5..9) spawns 1..4 type 0x5C objects linked to the weapon (+0x90); called from the weapon swings
- `8010fe70 playerSkidDecelerate` (was FUN_8010fe70) - deceleration of +0xB2 by the D_80115510 table plus the per-frame move used by playerStepSkidStop
- `80133474 updateFrog` (was func_80133474) - layer 2 type 0x12 handler in AREA00 (MAIN0 table slot 80077DE4): its animation strips show a green frog (tools/anim.py export, a00_func_80133474)
- `800ec420 updateWeaponTrailObject` (was FUN_800ec420) - pool type 0x5C handler (80079D1C): the objects spawnWeaponTrailObjects attaches to the weapon (+0x90); weaponTrailFollowStep / weaponTrailOrbitStep by subtype
- `800ec378 updateWeaponTrailObjectAlt` (was FUN_800ec378) (no caller found) - same dispatch as updateWeaponTrailObject without the init/free
- `800ebd40 weaponTrailFollowStep` (was FUN_800ebd40) - weapon trail object: copies the weapon position (+0x90) by the player state 800A6066
- `800ebf68 weaponTrailOrbitStep` (was FUN_800ebf68) - weapon trail object: orbits/swings around the weapon by angle (MulCos/MulNegSin, Rand)
- `800ec9c0 updateColorBurstParticle` (was FUN_800ec9c0) - pool type 0x5F handler (80079D28): the particles spawnColorBurst creates (four vertices +-4 at +0xB4..)
- `800ea67c updateGroundDebris` (was FUN_800ea67c) - pool type 0xC handler (80079BDC): subtype 0 keeps spawning subtype-1 type 0xC children at random intervals (the objects spawnGroundDebris creates)
- `800eb394 updatePoolType1BEffect` (was FUN_800eb394) - pool type 0x1B handler (80079C18): anim + sfx, freed when done
- `800eb50c spawnPoolType1BEffect` (was FUN_800eb50c) - spawns a pool type 0x1B object at x/y/z with the per-area sprite/anim table D_80114AE4 (subtype = arg)
- `800eb6ac updateBreakPiece` (was FUN_800eb6ac) - pool type 0x1E handler (80079C24): the pieces spawnBreakPieces creates, moved by applyAnimVelocityX
- `800eb8f0 spawnBreakPieces` (was FUN_800eb8f0) - spawns pool type 0x1E pieces at x/y offsets from D_80114C08, copying sprite/flags of the source object
- `800eb9f0 updatePoolType44Object` (was FUN_800eb9f0) - pool type 0x44 handler (80079CBC)
- `80121a5c spawnPoolType44Object` (was FUN_80121a5c) - allocates a pool type 0x44 object at x/y/z with +0x0C = arg
- `800ea168 updatePoolType04Object` (was FUN_800ea168) - pool type 4 handler (80079BBC)
- `800ea284 updateEggShellPiece` (was updatePoolType05Object; psx_tomba name) - pool type 5 handler (80079BC0): falls until probeCollisionAtDepthB, despawned offscreen
- `800eb0c8 updatePoolType10Object` (was FUN_800eb0c8) - pool type 0x10 handler (80079BEC)
- `800ebb48 updatePoolType5BObject` (was FUN_800ebb48) - pool type 0x5B handler (80079D18)
- `8010a518 playerSpinAimDir8B` (was FUN_8010a518) - playerStepSpinOnObject helper: 8-way direction (D_8009C330+0xB, 8 = none) from the pad, mirrored by facing
- `8010a168 playerSpinAimDir8A` (was func_8010A168) - same 8-way pad direction with another table
- `800f0e7c playerIdleAnimUpdate` (was func_800F0E7C) - playerStepIdle helper: picks the idle/fidget animation (FUN_800f0b5c / FUN_800f0c28) and restarts it when it changed
- `80110760 playerDecelerateSpeedB` (was FUN_80110760) (no caller found) - deceleration of +0xB2 by the D_80115510 table (same pattern as playerSkidDecelerate)
- `80110fdc playerAttackDecelerate` (was func_80110FDC) - weapon attack steps 3/4: decelerates +0xB2 with the attack columns of the D_80115510 table (row by weapon and floor kind)
- `80104a04 playerBittenJumpAirMove` (was func_80104A04) - bitten-jump helper: air motion, re-bite (wac == 2) and grab probe (playerProbeGrab) while carrying the bitten object
- `8011a148 spawnPoolType09Object` (was FUN_8011a148) - spawns a pool type 9 object (sub = arg) at x/y/z when fewer than 3 exist
- `800eaffc spawnWeaponHitDebris` (was FUN_800eaffc) - weapon terrain hits (weapon0Swing, weapon5to7HitWall, weapon8to9HitWall): in area 1 sections 0/1 spawns a pool type 0xC subtype 2 object (ground debris) at the hit
- `80104cd8 playerPlaceBittenObject` (was FUN_80104cd8) - bitten-jump helper: places the bitten object D_8009D2E8 around the player by the angle +0x88 (mirrored by facing) and its height +0x70
- `800ef760 playerGroundAnimTransition` (was func_800EF760) - playerSelectGroundAnim helper: picks/continues the ground anim by kind k (idle anim 80010748 for 0) from the current anim id
- `800f0c28 playerIdlePickFidgetAnim` (was FUN_800f0c28) - playerIdleAnimUpdate helper: idle fidget anim id by +0xA8 / item D_8009D2B1 and area (0xFF = none)
- `800f0b5c playerIdleFidgetAllowed` (was FUN_800f0b5c) - returns 1 when idle fidgets may play (area 1 / 9 special cases by section and flags)
- `80111b40 spawnItemDropsFromTable` (was FUN_80111b40) - spawns the drops of table T_80115A58[+0x6A] at the object position (spawnItemFromEntry, random entry when marked 0xFF)
- `8011468c updateL2Type18Object` (was FUN_8011468c) - shared layer 2 type 0x18 handler (MAIN0 table slot 80077DFC; bite holder playerBiteHoldType18): box/sprite/anim from D_80115DE4 by subtype, steps func_80113F38 / func_801142C0
- `80113484 updateTreasureChest` (was updateL2Type0EObject; psx_tomba name) - shared layer 2 type 0xE handler (slot 80077DD4; bite holder playerBiteHoldType0E)
- `80114890 updateL2Type1FObject` (was FUN_80114890) - shared layer 2 type 0x1F handler (slot 80077E18): thunk to the area's own code
- `8011191c updateL1Type0AObject` (was FUN_8011191c) - layer 1 type 10 handler (slot 8007A134): subtype 0/1 steps, short hit flash via +0x22/+0x6A
- `800f9684 playerPerchFlipLandCheck` (was FUN_800f9684) - playerStepFlipOntoPerch helper: probes the landing (probeCollisionAtDepthC, side tile response) and sets the landing anim
- `80103628 playerBiteHoldAttach` (was FUN_80103628) - playerBiteHoldGeneric helper: snaps the player onto the bitten object D_8009D2E8 (x, y - height +0x70), anim 0xD, per bitten type setup
- `8010cda8 playerSwimDirFromPadB` (was FUN_8010cda8) - playerStepSwim helper: swim direction +0x76 / angle +0xB6 from the pad bits D_8009D670 (when D_8009CE3D != 0xFF)
- `800ec5f4 spawnWeaponTrailSpark` (was FUN_800ec5f4) - weaponTrailOrbitStep helper: spawns a pool type 0x5C subtype 9 object at the trail object plus an offset
- `80111a50 spawnL1Type0AObject` (was FUN_80111a50) - allocates a layer 1 type 10 object (updateL1Type0AObject) at the player with subtype/slot/state from the arguments (spawnPlayerPowerEffect)
- `800f28a0 playerJumpArea3Float` (was FUN_800f28a0) (uncertain) - playerStepJump helper: in area 3 with D_8009C944 set, anim 0x35 and pad-driven velX / velY (setVelXFromPadLR, adjustVelYFromPadDown)
- `80106a18 playerStep48ByArea` (was FUN_80106a18) - player step 0x48: per-area handlers (overlay code at 8011B47C / 8011C758 / 8011D5F4), like playerStep2EByArea
- `800ea544 spawnPoolType05Ring` (was FUN_800ea544) - spawns 6 pool type 5 objects (updateEggShellPiece) at offsets D_80114A80 around x/y/z (layer-1 type 2 reward handler)
- `80113754 objSetAnimFromSubtypeTable` (was FUN_80113754) - sets the anim from PTR_80115DF0[subtype][+0x74] and jumps to frame +0x76 (script ops and execGameOpcode)
- `800ee6d0 playerFloatDirFromPad` (was FUN_800ee6d0) - playerJumpArea3Float helper: direction +0x76 from the pad bits D_8009D670 (Left 0xF0, Down sets 8) and a MulCosDup speed
- `800ef05c spawnPlayerSplashEffect` (was FUN_800ef05c) - when spawnPlayerFootstepEffect spawned nothing: pool type 0x31 subtype 1 object at x/y/z (playerStepWaterJumpOut)
- `800fc02c playerReleaseBittenAirborne` (was FUN_800fc02c) - sets +0x9C = 1 and releases the held object like playerReleaseBitten (D_8009D2E8 = PLX.p611c bytes 4/5/6 = 2,2,0)
- `8010f254 playerSpinReleaseSpeedKind` (was func_8010F254) - playerStepSpinRelease helper: classifies |speed +0xB2| by the spin counter D_8009C330->w20
- `8010e97c playerDecelerateIfFlag` (was func_8010E97C) (no caller found) - when +0xC1 bit 0: decelerates +0xB2 by 0x20 above 0x200, stops near 0
- `8010b784 playerRestoreClingObjectActive` (was func_8010B784) (no caller found) - restores the active byte of the clung object D_8009F0EC by type (the switch of playerReleaseClingObject)
- `80105ae4 playerBittenJumpReleaseFall` (was FUN_80105ae4) (no caller found) - drops the bitten object and enters the fall (jump counter 0xE, playerSetFallLean)
- `800ebc64 initWeaponTrailSprite` (was FUN_800ebc64) (no caller found) - sprite/anim of a weapon trail object from D_80114C20/24 by subtype and slot
- `800ef388 playerResetOnLanding` (was FUN_800ef388) (no caller found) - on landing (+0x69): sfx 0x1C and clears the jump/attack/grab flags
- `800f66e4 playerSetFallAnimLoop` (was FUN_800f66e4) (no caller found) - sets player anim 4 (fall) at frame 1 when not already playing
- `80104dc8 playerBittenCarryAirStep` (was FUN_80104dc8) (no caller found) - air step carrying the bitten object on top (x = player, y + box2), gravity and head collision
- `800ee2a4 updateL4Type3EObject` (was FUN_800ee2a4) - layer 4 type 0x3E handler (MAIN0 table slot 80079E6C): tall invisible box (4..8 x 0xAA..0x154 by +0x0C)
- `800ee18c updateL4Type13Object` (was FUN_800ee18c) - layer 4 type 0x13 handler (slot 80079DC0)
- `80134e5c updateHoodedOldMan` (was func_80134E5C) - AREA00-only object handler: layer 2 type 0x34 slot (MAIN0 table 80077E6C) while AREA00 (X000) is loaded
- `80116d14 updateA00L5Type11` (was FUN_80116d14) - AREA00-only object handler: layer 5 type 0x11 slot (MAIN0 table 8007B354) while AREA00 (X000) is loaded
- `8011bf68 updateA00L4Type02` (was func_8011BF68) - AREA00-only object handler: layer 4 type 0x2 slot (MAIN0 table 80079D7C) while AREA00 (X000) is loaded
- `801288b8 updateA00L2Type2C` (was func_801288B8) - AREA00-only object handler: layer 2 type 0x2C slot (MAIN0 table 80077E4C) while AREA00 (X000) is loaded
- `80132174 updateRedBird` (was FUN_80132174) - AREA00-only object handler: layer 2 type 0x8 slot (MAIN0 table 80077DBC) while AREA00 (X000) is loaded
- `8011b8c0 updateA00L4Type01` (was FUN_8011b8c0) - AREA00-only object handler: layer 4 type 0x1 slot (MAIN0 table 80079D78) while AREA00 (X000) is loaded
- `8012d294 updateA00L2Type2B` (was FUN_8012d294) - AREA00-only object handler: layer 2 type 0x2B slot (MAIN0 table 80077E48) while AREA00 (X000) is loaded
- `8011ec68 updateA00L4Type0A` (was FUN_8011ec68) - AREA00-only object handler: layer 4 type 0xA slot (MAIN0 table 80079D9C) while AREA00 (X000) is loaded
- `8012e8c4 updateA00L2Type06` (was func_8012E8C4) - AREA00-only object handler: layer 2 type 0x6 slot (MAIN0 table 80077DB4) while AREA00 (X000) is loaded
- `8011e838 updateA00L4Type09` (was func_8011E838) - AREA00-only object handler: layer 4 type 0x9 slot (MAIN0 table 80079D98) while AREA00 (X000) is loaded
- `8012e4c4 updateA00TouchSparkTrigger` (was func_8012E4C4) - AREA00-only object handler: layer 2 type 0x5 slot (MAIN0 table 80077DB0) while AREA00 (X000) is loaded
- `8011f288 updateA00L4Type11` (was func_8011F288) - AREA00-only object handler: layer 4 type 0x11 slot (MAIN0 table 80079DB8) while AREA00 (X000) is loaded
- `8011ad98 updateA00BackdropSprites` (was func_8011AD98) - AREA00-only object handler: pool type 0x14 slot (MAIN0 table 80079BFC) while AREA00 (X000) is loaded
- `801236f0 updateA00L5Type0B` (was func_801236F0) - AREA00-only object handler: layer 5 type 0xB slot (MAIN0 table 8007B33C) while AREA00 (X000) is loaded
- `8012e1c0 updateEgg` (was updateA00L2Type04; psx_tomba name) - AREA00-only object handler: layer 2 type 0x4 slot (MAIN0 table 80077DAC) while AREA00 (X000) is loaded
- `801328b4 updateA00L2Type09` (was FUN_801328b4) - AREA00-only object handler: layer 2 type 0x9 slot (MAIN0 table 80077DC0) while AREA00 (X000) is loaded
- `8011a744 updateA00PoolType0B` (was func_8011A744) - AREA00-only object handler: pool type 0xB slot (MAIN0 table 80079BD8) while AREA00 (X000) is loaded
- `80134404 updateA00L2Type14` (was func_80134404) - AREA00-only object handler: layer 2 type 0x14 slot (MAIN0 table 80077DEC) while AREA00 (X000) is loaded
- `801195f4 updateA00PoolType09` (was FUN_801195f4) - AREA00-only object handler: pool type 0x9 slot (MAIN0 table 80079BD0) while AREA00 (X000) is loaded
- `801224c0 updateA00L7Type01` (was func_801224C0) - AREA00-only object handler: layer 7 type 0x1 slot (MAIN0 table 80079FE8) while AREA00 (X000) is loaded
- `80134c20 updateNpcCharles` (was updateA00L2Type15; psx_tomba name) - AREA00-only object handler: layer 2 type 0x15 slot (MAIN0 table 80077DF0) while AREA00 (X000) is loaded
- `800ed024 updateL4Type00Object` (was FUN_800ed024) - layer 4 type 0x0 handler (MAIN0 table slot 80079D74)
- `80126fec updateMushroomCluster` (was FUN_80126fec) - AREA00-only object handler: layer 2 type 0x1 slot (MAIN0 table 80077DA0) while AREA00 (X000) is loaded
- `80118f90 updateA00PoolType03` (was FUN_80118f90) - AREA00-only object handler: pool type 0x3 slot (MAIN0 table 80079BB8) while AREA00 (X000) is loaded
- `80118438 updateA00PoolType01` (was FUN_80118438) - AREA00-only object handler: pool type 0x1 slot (MAIN0 table 80079BB0) while AREA00 (X000) is loaded
- `800edf94 updateL4Type03Object` (was FUN_800edf94) - layer 4 type 0x3 handler (MAIN0 table slot 80079D80)
- `8011d010 updateA00L4Type06` (was FUN_8011d010) - AREA00-only object handler: layer 4 type 0x6 slot (MAIN0 table 80079D8C) while AREA00 (X000) is loaded
- `801202a4 updateA00L4Type18` (was FUN_801202a4) - AREA00-only object handler: layer 4 type 0x18 slot (MAIN0 table 80079DD4) while AREA00 (X000) is loaded
- `80121870 updateA00L4Type26` (was FUN_80121870) - AREA00-only object handler: layer 4 type 0x26 slot (MAIN0 table 80079E0C) while AREA00 (X000) is loaded
- `8012dedc updateChick` (was updateA00L2Type03; psx_tomba name) - AREA00-only object handler: layer 2 type 0x3 slot (MAIN0 table 80077DA8) while AREA00 (X000) is loaded
- `80118bc0 updateA00PoolType02` (was FUN_80118bc0) - AREA00-only object handler: pool type 0x2 slot (MAIN0 table 80079BB4) while AREA00 (X000) is loaded
- `8011b2e8 updateA00PoolType24` (was func_8011B2E8) - AREA00-only object handler: pool type 0x24 slot (MAIN0 table 80079C3C) while AREA00 (X000) is loaded
- `80122724 updateA00L7Type02` (was func_80122724) - AREA00-only object handler: layer 7 type 0x2 slot (MAIN0 table 80079FEC) while AREA00 (X000) is loaded
- `8011f608 updateA00L4Type17` (was FUN_8011f608) - AREA00-only object handler: layer 4 type 0x17 slot (MAIN0 table 80079DD0) while AREA00 (X000) is loaded
- `801191b0 updateA00PoolType27` (was FUN_801191b0) - AREA00-only object handler: pool type 0x27 slot (MAIN0 table 80079C48) while AREA00 (X000) is loaded
- `8012236c updateA00L7Type00` (was FUN_8012236c) - AREA00-only object handler: layer 7 type 0x0 slot (MAIN0 table 80079FE4) while AREA00 (X000) is loaded
- `80136090 updateA00RoamingFlyer` (was FUN_80136090) - AREA00-only object handler: layer 2 type 0x57 slot (MAIN0 table 80077EF8) while AREA00 (X000) is loaded
- `8011ab10 updateA00PoolType11` (was FUN_8011ab10) - AREA00-only object handler: pool type 0x11 slot (MAIN0 table 80079BF0) while AREA00 (X000) is loaded
- `80123a1c updateA00L5Type0C` (was FUN_80123a1c) - AREA00-only object handler: layer 5 type 0xC slot (MAIN0 table 8007B340) while AREA00 (X000) is loaded
- `80119468 updateBubbleDroplet` (was updateA00PoolType08; psx_tomba name) - AREA00-only object handler: pool type 0x8 slot (MAIN0 table 80079BCC) while AREA00 (X000) is loaded
- `8011b6e0 updateA00PoolType59` (was FUN_8011b6e0) - AREA00-only object handler: pool type 0x59 slot (MAIN0 table 80079D10) while AREA00 (X000) is loaded
- `8011935c updateYellowChick` (was FUN_8011935c) - AREA00-only object handler: pool type 0x7 slot (MAIN0 table 80079BC8) while AREA00 (X000) is loaded
- `8011b0d4 updateA00PoolType16` (was FUN_8011b0d4) - AREA00-only object handler: pool type 0x16 slot (MAIN0 table 80079C04) while AREA00 (X000) is loaded
- `8011b1e0 updateA00PoolType17` (was FUN_8011b1e0) - AREA00-only object handler: pool type 0x17 slot (MAIN0 table 80079C08) while AREA00 (X000) is loaded
- `8011a63c updateA00PoolType0A` (was FUN_8011a63c) - AREA00-only object handler: pool type 0xA slot (MAIN0 table 80079BD4) while AREA00 (X000) is loaded
- `8011926c updateA00PoolType06` (was FUN_8011926c) - AREA00-only object handler: pool type 0x6 slot (MAIN0 table 80079BC4) while AREA00 (X000) is loaded
- `8011b7f0 updateA00PoolType5A` (was func_8011B7F0) - AREA00-only object handler: pool type 0x5A slot (MAIN0 table 80079D14) while AREA00 (X000) is loaded
- `8011f1d0 updateA00L4Type0D` (was func_8011F1D0) - AREA00-only object handler: layer 4 type 0xD slot (MAIN0 table 80079DA8) while AREA00 (X000) is loaded
- `80111ae0 updateL2Type07Object` (was func_80111AE0) - layer 2 type 0x7 handler (MAIN0 table slot 80077DB8)
- `800ee25c updateL4Type38Object` (was func_800EE25C) - layer 4 type 0x38 handler (MAIN0 table slot 80079E54)
- `80114a08 updateVillagerNpc` (was FUN_80114a08) - layer 2 type 0x55 handler (MAIN0 table slot 80077EF0)
- Collision pass callbacks named by the pass and the object type of their switch case (like playerContactCat4Type0B / weaponHitCat2Type07):
  collidePlayerWithCat4Objects `80124ebc playerContactCat4Type01` (push-out unless linked +0x94), `80124f3c playerContactCat4Type02` (sets the facing +0x2E from the push side), `80124fa4 playerContactCat4Type04`, `80124d10 playerContactCat4Type09` (sloped boxes by b0c: `80124bbc pushOutOfSlopedBoxSide` / `80124a84 landOnSlopedBoxTop`, player step 0x17 on contact), `80124fd4 playerContactCat4Type0A`, `80124efc playerContactCat4Type17`, `80124f1c playerContactCat4Type18`;
  collidePlayerWithCat2Objects `80125f30 playerContactCat2Type01`, `80126464 playerContactCat2Type08`, `801260f8 playerContactCat2Type12`, `80126198 playerContactCat2Type15`, `80126010 playerContactCat2Type2B`;
  collideWeaponsWithCat4Objects `80125a28 weaponHitCat4Type01`, `80125984 weaponHitCat4Type02`; collideWeaponsWithCat2Objects `80125aac weaponHitCat2Type01`, `80125da4 weaponHitCat2Type06`, `80125c84 weaponHitCat2Type08`, `80125c1c weaponHitCat2Type12`;
  collideCat2/Cat5WithCat4Objects `80126ca4 pushCat2ObjectOutOfCat4Type04` / `80126d04 pushCat5ObjectOutOfCat4Type04` (type 4 in area 0; the same functions are the area-0 hooks of MAIN0 dispatchAreaCollisionHookA/B), `80126b50 pushObjectOutOfCat4Type01`, `80126c60 pushObjectOutOfCat4Type02`, `80126d48 pushCat2ObjectOutOfCat4Type0A`;
  `80126f68 collideCat2Type7Cat5Type0BPair` (collideCat2Type7WithCat5Type0B on overlap: both parents freed); playerProbeGrab `801269d0 playerGrabCat4Type01` (b9e = 1) / `80126940 playerGrabCat4Type02` (b9e = 4)
- `80126770 collideArea0Section2Slope` (was FUN_80126770) - processObjectCollisions in area 0 section 2: hard-coded slope line at depth 0xB4 and wall at x 0xC35
- Koma pig, area 0 (X000 `8012ccf8 updateKomaPig` (was FUN_8012ccf8), called by MAIN0 dispatchAreaEnemyDraw for area 0; same template as the x001/x003/x004 handlers): b04 1 step 0 runs the subtype table D_801390F8 (`8012b5d0 komaPigSubtype00Step` for subtypes 0/1/8, `8012ba94 komaPigSubtype02Step`, `8012c2c8 komaPigSubtype03Step` (3/10), `8012c598 komaPigSubtype04Step`, `8012c8ec komaPigSubtype05Step`, `8012cae4 komaPigSubtype06Step`, `8012d7fc komaPigSubtype07Step`, `8012d918 komaPigSubtype09Step`); step 1 runs the state functions `80129fb4 komaPigState0`, `80129bb0 komaPigState1`, `801294fc komaPigState2`, `80128fac komaPigState3`, (state 4 = updateA00L2Type2C), `80128190 komaPigState5`, `80127eb0 komaPigState6`, `80127820 komaPigState7` (numbered by state: the X000 bodies differ from the x003/x004 idle/walk/rest/leap/jump/fall functions, so the roles are not carried over); b04 2 steps `8012ae1c komaPigHit`, `8012a820 komaPigGrabbed`, `8012a6a4 komaPigDefeated`
- updateTreasureChest per-subtype tables (kind of object unknown): b04 0 D_80115D90 `80112864 l2Type0EInitSubtype0` (subtypes 0, 3..6), `80112898 l2Type0EInitSubtype1`, `8011287c l2Type0EInitSubtype2`; b04 1 D_80115DAC `801128f8 l2Type0EActiveIdle` (0, 5, 6), `80111f58 l2Type0EActive1`, `80112918 l2Type0EActive2`, `801129d0 l2Type0EActive3`, `80112520 l2Type0EActive4`; b04 2 D_80115DC8 `80112c24 l2Type0EHit0` (0, 6), `80112204 l2Type0EHit1`, `80113034 l2Type0EHit2` (2, 5), `801133b8 l2Type0EHit3`, `801125e8 l2Type0EHit4`
- updateL2Type18Object steps: `80113f38 l2Type18Active` (b04 1), `801142c0 l2Type18Hit` (b04 2 step 0)
- Small player/object helpers, mostly with no caller (dead code, named by what they do): `800ee3d4 playerResetMotionState` (clears the D_8009C330 step state and the player velocities/angles), `800ee920 playerShiftDown7C` (y += 0x7C unless +0xAD), `800f29dc playerEnterRecoilA` / `800f369c playerEnterRecoilB` / `800f3eec playerEnterRecoilStep4` (b9c 2, timer 10, state 3, angle 0x10/0xF0 by facing), `800f4e68 playerSetAnimPairA` / `800f4e9c playerSetAnimPairB` (two anim bytes from D_80115220/D_80115228 by wb2 into D_8009C330), `800fb094 playerSetMoveAngleFromW7A`, `800fb414 copyD88ToD8C`, `800fc18c playerClampClimbTop` (top -0x8C or -0x3C0 by GV2.bc3 & 0x40; 2 when GCD.bce3d == 0xFF), `800ff074 getHudExpBarMode` (0..2 from GCD.bcffb / bcdac, 0 in area 0 sections 0-1 or with the exp bar shown), `801049c0 playerResetToStep0`, `80104ea0 playerCheckJumpPressedA` / `80104f2c playerCheckJumpPressedB` (jump/attack button -> substep 2), `80107590 playerEnterStep10Fall`, `80108fe0 playerResetAfterEvent`, `8010e5e4 playerAccelerateUpCapped` (velY -= 0x10 to -0x800), `801128f0 clearObjectB0A`, `801363c4 isPlayerPastX0D8`, `800edc0c stubNop800EDC0C`, `80113cc0 stubNop80113CC0`, `8011379c animAdvanceThunk`
- `800ea220 initPoolType04Sprite` (sprite/anim PTR_8013B110[subtype]), `800eebc4 playerSetIdleAnimByArea`, `800f0450 objectSinkAndProbe` (y += 2 while b69, then func_8003FD78), `800f434c playerLandResumeWalk` (sfx 0x1C, step 1 or 0x1F by bbe & 0x20), `80103774 playerResetSlopeState`, `8010dd44 playerEnterStep3E` (anim 0x44), `801229d8 playerHitAnimWithImpact` (anim 0xD + spawnImpactEffect 2), `80122990 playerSetSpawnTimerArea0Sec3` (+0x20 = 0x8C in section 3 when the 8009CDA5 flag is set), `80112be0 l2Type0ECheckAreaFlag`, `8011465c l2Type18HitIfStep0`, `80121ae0 a00L4Type26ReleaseParts` (sets b04 2 on the six part objects +0xB4..0xC8)
- AREA00-only handler steps, named after their updateA00* handler (structural names; the object kinds are not identified): `80118638 a00PoolType01Init` / `8011899c a00PoolType01Step` (with `8011880c a00PoolType01SubStep`), `80118e70 a00PoolType03Step`, `80119988 a00PoolType09Step` / `80119864 a00PoolType09SubStep`, `8011a2f8 spawnPoolType09Child` (spawnPoolType09Object), `8011a418 a00PoolType0AStep`, `8011ac58 a00BackdropSpritesStep4`, `8011b5dc a00PoolType59Talk` (showMessageBox), `80118d8c spawnMushroomClusterDebris` / `80119584 a00L2Type06SpawnChild` (pool allocations), `8011aa30 spawnTouchSparkTriggerSparks`, updateRedBird steps `801307a0 redBirdStep0`, `801308d8 redBirdStep1`, `801319d0 redBirdStep1B`, `801320a8 redBirdStep1C` (box + spawnItemNotification), `80130a38 redBirdStep2`, `80130bb0 redBirdStep3`, `801317e4 redBirdStep4`; `80134190 a00L2Type14Step`, `80134980 a00L2Type15Step1`, `801346c0 a00L2Type15Step3`, `80135b0c a00RoamingFlyerSteer` (angleBetweenPoints) / `80135db0 a00RoamingFlyerBob`; updateA00L4Type18 `80120164 a00L4Type18Step` -> `8011f79c a00L4Type18Idle` / `8011f9e0 a00L4Type18Active` (also collidePlayerWithCat2Objects case 0x29); updateA00L4Type26 `80121124 a00L4Type26Step`, `8012160c a00L4Type26SpawnParts`, `8012171c a00L4Type26Reward` (spawnItemFromEntry); updateFrog `80133254 frogStep0`, `80132f80 frogHop` (AnimJump + applyFrameVelocityX, wall probe)
- `801257f4 komaPigContact` (was FUN_801257f4) - collidePlayerWithCat2Objects type 2 in area 0, the X000 counterpart of the x001/x003/x004 komaPigContact (x004 twin of this one)
- More collision callbacks (type = the case of the pass; area 0 where the pass switches on the area): `801252b8 playerContactCat4Type06` (with `80124ff4 playerContactCat4Type06Land`), `80126240 playerContactCat2Type06`, `8012555c playerContactCat2Type07` (area 0), `80126698 playerContactCat2Type09`, `80126a64 playerGrabCat4Type06`, `80126d7c pushObjectOutOfCat4Type06` (collideCat2/Cat5WithCat4Objects)
- updateL1Type0AObject: `8011145c l1Type0AInit`, `80111880 l1Type0AStep` (by subtype: `80111590 l1Type0AStepSub0`, `80111714 l1Type0AStepSub1`); l2Type18Active states `80113888 l2Type18State2`, `80113b6c l2Type18State5`, `80113cc8 l2Type18State7`; `80111dc8 l2Type0ESharedStep` (used by l2Type0EActive1 and l2Type0EHit1)
- updateL2Type07Object -> `8012ed60 l2Type07Update`, by subtype `8012f1b4 l2Type07Sub0` (`8012f228 l2Type07Sub0Step`, `8012f36c l2Type07Sub0End`), `8012f380 l2Type07Sub1` (`8012f3f4 / 8012f864 / 8012f9e0 l2Type07Sub1StepA/B/C`), `8012fb3c l2Type07Sub2` (`8012fbb0 l2Type07Sub2Step`, `8012fe84 l2Type07Sub2End`), `8012fe98 l2Type07Sub3`, `801302a0 l2Type07Sub4` (`80130314 l2Type07Sub4Step`)
- Renderers: `80123fd0 drawObjectSpriteRotatedFT4` (drawObject kind 6, also used by drawObjectSpriteRotated/RotScaled: FT4 rotated by d8c), drawCategory3Objects `80123e04 drawObjectProjectedSpriteB` (kind 5: projected FT4 with offsets wb6/wb8/wba), `80124780 drawObjectPolyFT4Strip` (kind 17), `800ecd88 drawObjectModelKind19` (kind 19: model rotated by d84/d88/d8c)
- Area-0 handlers of MAIN0 layer 4 types (structural names): updateL4Type38Object -> `80122118 l4Type38Update` (steps `80121b28 / 80121e50 / 80121fe0 l4Type38StepA/B/C`), updateL4Type1BObject -> `80120d04 l4Type1BArea0Update` (`80120734 l4Type1BInitModel` -> `80120494 l4Type1BDeformModel` (copyModelPrimGroups + subtractScaledVertices), `80120bcc l4Type1BProbe`, `80120890 l4Type1BCarryPlayer` (playerSetAnim)), updateL4Type03Object steps `800edc14 l4Type03Step` / `800eddfc l4Type03StepB`
- `80114968 spawnGroundDebrisPiece` (spawnGroundDebris helper, allocObjectLayer2), `801148d0 spawnDebrisObjectB` (no caller found, same kind of layer-2 allocation)

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
- `x001 8012b2d4 updatePurpleCrawler` - layer 2 type 0 (11 placements, AREA01/07): (was updatePurplePig) old strips read as a purple pig; the draw capture shows a purple many-legged crawler, see "Draw captures"
- `x001 80138d4c updateCeilingDrip` - layer 2 type 34: strips show water gathering on the ceiling, forming a drop, falling and splashing
- `x001 80139914 updateFlowerTowerKeeper` - layer 2 type 0x23 (5 placements, section 5): talks through flowerTowerKeeperTalk; strips show a red horned figure
- `x001 80139770 flowerTowerKeeperTalk` - MFNT010S 2/126-127: "...te he asustado? ... la torre ... para que la Torre de flores no se seque"
- `x001 8013721c updatePuppyBaron` - layer 2 type 0x17: strips show a white puppy; subtype 1 calls addItemToInventory(10) and sets a flag (X002 2/7-7 "el pequeño cachorro que salvaste en el Bosque de enanos ... Barón")
- `x001 8013a2d8 puppyRumorTalk` - MFNT010S 2/31-33: heard a puppy barking near the spores, it is hurt
- `x001 80139c50 lostSeedKidTalk` - MFNT010S 2/10-11: crying, cannot get down, was looking for flower seeds, "take me home?"
- `x001 8013b254 stepRaceEvent` - script-state switch (GCD.bce59/bcede) over the four race functions below
- `x001 8013a8cc raceStartCountdown` - MFNT010S 2/25 "Listo...ya!" (state 1 step 0, first race)
- `x001 8013aab8 raceResult` - MFNT010S 2/26-29: lost/won the race, "the treasure is yours", rematch offer
- `x001 8013af10 raceOfferRematch` - MFNT010S 2/30 "Una revancha? Vale! Vamos abajo." (state 2)
- `x001 8013b0f4 raceStartRematch` - MFNT010S 2/25 countdown again (state 3, after the rematch offer)
- `x002 8011a114 pigGateDiscoveryScene` - MFNT020S 7/3-8: another evil-pig bag and gate found, seven people on seven pedestals, gives Baron
- `x002 8011bda8 pigGateSevenFriendsScene` - MFNT020S 3/3-4, 7/9-10: the gate opens, "we still need one more... Baron!"
- `x002 80117d70 sevenFriendsWaitTalk` - MFNT020S 7/6, 7/11: "I'll wait here, tell your friends to come", gate left open
- `x003 8012d280 updateWindblownMonkey` - layer 2 handler, strips show a monkey with a bag and a striped umbrella; MFNT030S 6/4 "the wind caught me and brought me here"
- `x003 8012cc10 windblownMonkeyRepeatTalk` - MFNT030S 6/5 (repeat of the wind / torn trousers line)
- `x003 8012cdc0 windblownMonkeyAskTrousers` - MFNT030S 6/6: lost the other trousers in the wind, "find them"
- `x003 8012cfe0 windblownMonkeyGiveUmbrella` - MFNT030S 6/7-9: thanks for the trousers, gives the umbrella (explains gliding)
- `x003 8012c00c tunnelDiggerTalk` - MFNT030S 2/0-3: "I'm digging a tunnel, nobody digs like me"
- `x003 8012b66c treasureChestGuardTalk` - MFNT030S 4/0-1: guards the millennium treasure chest until the millennium key holder comes
- `x003 8012fcac lostBossItemNpcTalk` - MFNT030S 8/1-4: lost something important of his boss, asks you to find it (same character as x004 8012c9f0)
- `x003 801307b0 updatePhoenixGossipNpc` - layer 2 type 0x5A handler (2 placements), calls the four phoenix gossip lines below
- `x003 801301c0 / 801304b8 phoenixGossipAskTalk / phoenixGossipAskTalkB` - MFNT030S 2/13 "Quieres ver al Fénix?" (two copies)
- `x003 8013034c / 80130644 phoenixGossipLavaHint / phoenixGossipLavaHintB` - MFNT030S 2/14: fell from the phoenix, lava caves need a hook (two copies)
- `x003 8012962c / 80131fec phoenixSpiritHintTalk / phoenixSpiritHintTalkB` - MFNT030S 2/15: "you just felt the evil spirit of Phoenix Mountain" (two copies)
- `x003 80132d80 flameBlockedNpcTalk` - MFNT030S 7/0-3: cannot go further because of the flames, gives an item found on the way
- `x003 801346c8 hidingSonTalk` - MFNT030S 5/0-2: hide-and-seek son, food basket from his father
- `x006 801176b0 updateMotocrossRaceStarter` - layer 3 type 0x55 (1 placement): MFNT060S 2/0-1, 2/11 race start / retry; strips show a straw-hat farmer
- `x004 8011ae9c updateKeyTablet` - layer 3 type 0x41 (1 placement, AREA04/12 section 19): subtype 0 strip is a stone tablet engraved with a key, the other subtypes golden key pieces (uncertain role)
- `x004 80128114 mermaidNecklaceTalk` - MFNT040S 4/0, 4/3, 4/9: how to get my necklace back from the waterfall; gives the Big fish food
- `x004 80128304 mermaidDivingOfferTalk` - MFNT040S 4/1-3: "that's my necklace!", offers to teach diving
- `x004 8012866c mermaidDivingLesson` - MFNT040S 4/4-9: the diving lesson (press down to dive, direction + special to move)
- `x004 80129be0 paintingSlotHintTalk` - MFNT040S 2/1: strange painting with a slot, door with a big lock
- `x004 8012a31c townHallWorkerTalk` - MFNT040S 2/0: town hall worker who came to repair the lake pump, carries the small room key
- `x004 8012c9f0 lostBossItemNpcTalk` - MFNT040S 3/0-4: "lost it again", hides from his boss (same character as x003 8012fcac)
- `x004 8012fbf4 hidingSonTalk` - MFNT040S 2/14-16: hide-and-seek son, his father's food basket (same as x003 801346c8)
- `x004 8012f75c unbendingCableTalk` - MFNT040S 5/0: "a cable that does not bend? ... I do have one"
- `x009 801283a4 updateJellyfish` - layer 2 type 0x21 (14 placements): strips show a purple one-eyed jellyfish with tentacles
- `x009 8012955c updateBoatman` - layer 2 type 0x59 (1 placement): MFNT090S 3/1 "want a ride in my boat? only one way"
- `x009 80128c44 flowerTearsSeekerTalk` - MFNT090S 2/6-7: came for Flower tears to revive the Charity fountain, needs the tear bottle
- `x009 80129ef8 aspiringActorGuardTalk` - MFNT090S 2/0-4: guard who wants to be an actor and practises faces; the mayor went to the mountain
- `x010 801253ac lostHatManTalk` - MFNT100S 4/0-3: man from the Watchtower whose hat a strange monkey took
- `x010 801268d4 tenThousandYearElderTalk` - MFNT100S 5/0-5: the 10,000-year elder, the 10 numbered beads under water, learn diving from the mermaid
- `x010 80125fb4 tenThousandYearElderBeadsTalk` - MFNT100S 5/6-8: beads returned, evil pig history, key to the underground labyrinth
- `x010 801264b4 powerFishCollectorTalk` - MFNT100S 5/10-14: collector who trades the Power fish for the five golden items
- `x010 8012855c updateSwimTeacher` - layer 2 type 0x4D (4 placements): runs the four swim teacher lines below
- `x010 8012736c swimTeacherIntroTalk` - MFNT100S 2/19-21: "you can't swim...", no more bananas, thirsty; found a hat
- `x010 80127b78 swimTeacherNoBananasTalk` - MFNT100S 2/20, 2/22: "I said no more bananas!"
- `x010 80127e08 swimTeacherJumpInTalk` - MFNT100S 2/24: "I'll teach you to swim, jump in the river"
- `x010 801280fc swimTeacherFloatTalk` - MFNT100S 2/26: "if you relax you can float"
- `x010 8012ba44 nobleTreeBiteTalk` - called by updateParrot; MFNT100S 2/4-7: "why do you bite me?", the noble tree is weak, accordion rocks
- `x010 8012bde4 nobleTreeAccordionRocksHint` - MFNT100S 2/7 (accordion rocks reminder)
- `x010 8012bf24 nobleTreeRestoredTalk` - MFNT100S 2/8-10: all rocks jumped, the tree is healthy, message from grandpa
- `x010 8012c0dc nobleTreeGrandpaMessage` - MFNT100S 2/10 (Tree of knowledge message, repeat)
- `x010 8012c220 nobleTreeGoldenItemsAsk` - MFNT100S 2/38-40: asking the tree where the five golden items are
- `x010 8012c5ac nobleTreeGoldenItemsHint` - MFNT100S 2/40 (golden items locations, repeat)
- `x010 8012d8e4 hidingSonTalk` - MFNT100S 3/0-2: hide-and-seek son again (same as x003/x004)
- `x005 80116690 mansionKeyHintTalk` - MFNT050S 2/51: "did you find the key? it must open a door of the Haunted mansion"
- `x005 801167f0 townGuideTalk` - MFNT050S 2/60, 2/78, 2/88: "this is Central Park and the Town hall", asks about the fortune tellers
- `x005 80116b44 wineKeeperTalk` - MFNT050S 2/101-102: lets Tombi take wine "as long as it's not for you"
- `x006 8011ed24 updatePitchforkPig` - layer 2 type 0x53 (7 placements): strips show a pink pig with a pitchfork walking, thrusting and fainting
- `x011 8011708c goGoCarMechanicTalk` - MFNT110S 2/6-10: the GoGo car needs a fuel bar made with wine
- `x011 80117560 goGoCarFuelTalk` - MFNT110S 2/10 (fuel bar / wine repeat)
- `x011 801177dc goGoCarWineReceivedTalk` - MFNT110S 3/7-8, 3/20: "the famous Baco town wine!", builds the bar and drives off
- `x011 80117c34 bonsugeeResearcherTalk` - MFNT110S 2/0-2: researcher of the Mushroom forest, Flower tears and the yellow Bonsugee
- `x011 80117ef8 bonsugeeResearcherRepeatTalk` - MFNT110S 2/1-2 (repeat)
- `x011 80118140 factoryBossTalk` - MFNT110S 2/12: "I'm in charge of this factory"
- `x011 801185ec factoryBossCoconutBombTalk` - MFNT110S 2/15: the coconut-looking bomb hidden in the tallest jungle tree
- `x011 801187d4 factoryBossNoBombHereTalk` - MFNT110S 2/16: "don't bring that here!"
- `x011 801189f8 factoryBossDontUseBombTalk` - MFNT110S 2/17: "don't use that here!"
- `x011 80118be0 factoryBossDoorAskTalk` - MFNT110S 2/18: "did you manage to blow the door?"
- `x011 80118dc8 powerPlantThanksTalk` - MFNT110S 2/4-5: the power plant works again, elevator near the Watchtower
- `x011 80119034 powerPlantElevatorHintTalk` - MFNT110S 2/5 (elevator hint repeat)
- `x016 80117b40 clockTowerRestoredTalk` - MFNT160S 2/12: the Clock tower is alive again, the gear room is dangerous
- `x016 801180fc juiceSellerTalk` - MFNT160S 2/10-20: juice seller (bananas, Masakari river, clock tower)
- `x016 80118458 juiceSellerSqueezeTalk` - MFNT160S 2/17-19: squeezes the bananas into juice
- `x017 80117224 ironCastleMachineRoomTalk` - MFNT170S 2/44: the Iron castle machine room door does not open
- `x017 8011750c waterfallElderHintTalk` - MFNT170S 2/35, 2/38: the 10,000-year elder above the waterfall, key of the Old pond door
- `x017 80118110 raftReturnTalk` - MFNT170S 2/35, 2/39: take the raft from the noble tree hill back to the Haunted mansion
- `x018 8011a32c updateWitchMizuno` - step switch over the ten Mizuno lines below (init/idle + talk states)
- `x018 801177d0 witchMizunoIntroTalk` - MFNT180S 3/0, 3/14: "I'm Mizuno, the Witch Mizuno", magic candy recipe
- `x018 80117c38 witchMizunoLostCrystalBallsTalk` - MFNT180S 3/2-3: forgot three big crystal balls (Watchtower)
- `x018 80117fac witchMizunoCrystalBallsFoundTalk` - MFNT180S 3/3-4: the crystal balls are back, come back later
- `x018 801182fc witchMizunoBusyTalk` - MFNT180S 3/5 "Hocus, pocus... I'm busy"
- `x018 8011858c witchMizunoMagicMirrorTalk` - MFNT180S 3/6-9: Magic mirror, combines the weapons into the hook-grapple
- `x018 80118c84 witchMizunoKeepMirrorTalk` - MFNT180S 3/10: "only I can use the Magic mirror, keep it"
- `x018 80118f14 witchMizunoSneezeTalk` - MFNT180S 3/11: sneezing (cold)
- `x018 801191dc witchMizunoCuredTalk` - MFNT180S 3/12: cured by the cold medicine, gives a reward
- `x018 801194d4 witchMizunoHuntPigsTalk` - MFNT180S 3/13 "shouldn't you be hunting evil pigs?"
- `x018 8011973c witchMizunoCandyRecipeTalk` - MFNT180S 3/15-22: lists the missing magic candy ingredients
- `x018 80116bdc juiceManJoinFightTalk` - MFNT180S 2/13, 3/23: "want more juice? ... you'll fight the evil pig, I'll come with you"
- `x001 80135164 wrongWayHintTalk` - MFNT010S 2/12 "Eh, vas por el camino equivocado..." (called by 8013649c)
- `x002 80118028 sevenFriendsGoodLuckTalk` - MFNT020S 3/0-2: "I hope you find seven friends soon... good luck Tombi!"
- `x003 8012ab5c fungaDwarfTalk` - MFNT030S 3/0-3 "...¡Funga!" (four lines, item check, starts/completes events; uncertain speaker)
- `x003 80132214 soakedPhoenixGuideTalk` - MFNT030S 2/4-12: soaked, the Phoenix (giant bird) can take you to the jungle, sneezes, "get off the mountain"
- `x004 80129650 scaredGoldVolunteerTalk` - MFNT040S 5/1 "Evil pigs... gold...! I'm scared but I'll go!"
- `x004 8012d4ac updateFreedVillager` - subtype switch over the MFNT040S group 5 villager lines below (+ lostBossItemNpcTalk)
- `x004 8012c004 cableOwnerThanksTalk` - MFNT040S 5/9-10: thanks for breaking the evil pig spell, "a cable? I used to use one"
- `x004 8012c1a8 singingMinerRumorTalk` - MFNT040S 5/11-12: "la la la... I wasn't singing", a miner found something
- `x004 8012c3d0 homeSweetHomeTalk` - MFNT040S 5/7-8: "home sweet home, thanks Tombi", the mermaid is somewhere above
- `x004 8012c574 mermaidSightingTalk` - MFNT040S 5/4-5: long time since we came here, a mermaid was seen leaving the mansion
- `x004 8012c718 ownHouseThiefTalk` - MFNT040S 5/6 "what kind of thief robs his own house..."
- `x004 8012c884 villagerRewardTalk` - MFNT040S 5/13 "here, this is for helping us"
- `x004 8012d198 lostTreasureBossTalk` - MFNT040S 5/2-3: "long time no see... my treasure? I gave it to my right-hand man / lost it"
- `x005 80116e78 stepBacoTownNpcTalk` - subtype switch over the Baco town lines (wineMerchantTalk, townGuideTalk, ..., wineKeeperTalk)
- `x005 801162b4 wineMerchantTalk` - MFNT050S 2/39-41, 2/50: "it's where?!", then "wine again! take all you want"
- `x005 8011698c noticeBoardTownsfolkTalk` - MFNT050S 2/61, 2/67, 2/82, 2/89: town notices board, small key in the haunted mansion
- `x005 80116cf8 safeMushroomHintTalk` - MFNT050S 2/98: healing mushrooms vs laughing/crying/suspicious mushrooms
- `x011 80119498 stepFactoryNpcTalk` - subtype/step switch over the GoGo car, Bonsugee, factory boss and power plant talk functions
- `x011 80118328 factoryBossIronCastleDoorTalk` - MFNT110S 2/13-15: the Iron castle door is old, "make a bomb? just kidding", the hidden bomb
- `x011 8011921c gratefulHelperTalk` - MFNT110S 3/21 "you've done a lot for me, I'm ready to help" (subtype 3; speaker uncertain)
- `x016 801174b8 brokenStoneGearsTalk` - MFNT160S 4/0 "the stone gears are broken..."
- `x017 801179ec pigGateFavorTalk` - MFNT170S 2/48: "the evil pig gate near the underground labyrinth? if it's a favor for you, I'll do it"
- `x017 80118ea0 waterfallElderHintScene` - MFNT170S 2/32-39 scene (bgm/events): we owe you, the 10,000-year elder, Old pond key, raft back
- `x001 8013649c updateForestDwarf` - layer handler whose strips show the pink Funga-speaking dwarf with a red scarf; calls wrongWayHintTalk
- Talk object handlers (pattern: `update<Npc>` = the placed-object handler, `switch (o->b04)` init/active/free; `step<Npc>Talk` = its `switch (o->step)` choosing which talk function runs from story flags):
  x001 8013b984 updateLostSeedKid, 8013bab0 updatePuppyRumorNpc; x002 801182ac stepSevenFriendsTalk, 80118534 updateSevenFriendsNpc;
  x003 80129868 stepPhoenixSpiritHintTalk, 8012b454 updateFungaDwarf, 8012bcb8 stepTreasureChestGuardTalk, 8012bedc updateTreasureChestGuard,
  8012c858 updateTunnelDigger, 801300b0 updateLostBossItemNpc, 801331fc stepSoakedPhoenixGuideTalk, 801332cc updateSoakedPhoenixGuide,
  80134c84 stepHidingSonTalk, 80134e60 updateHidingSon; x004 80129250 stepMermaidTalk, 801298dc stepScaredGoldVolunteerTalk,
  80129a9c updateScaredGoldVolunteer, 80129f10 stepPaintingSlotHintTalk, 8012a124 updatePaintingSlotHintNpc, 8012a670 stepTownHallWorkerTalk,
  8012a8b8 updateTownHallWorker, 8012fb4c updateUnbendingCableNpc, 8013013c stepHidingSonTalk, 80130320 updateHidingSon;
  x005 801171a4 updateBacoTownNpc; x009 801290f4 stepFlowerTearsSeekerTalk, 8012a70c updateAspiringActorGuard; x010 80125c50 updateLostHatMan,
  80126ea0 stepTenThousandYearElderTalk, 801270ac updateTenThousandYearElder, 8012de2c stepHidingSonTalk, 8012e010 updateHidingSon;
  x011 80119940 updateFactoryNpc; x016 801179a4 updateBrokenStoneGearsNpc, 80117d80 stepClockTowerRestoredTalk, 80117f60 updateClockTowerRestoredNpc,
  80118ae8 updateJuiceSeller; x017 80117c84 stepWaterfallHintNpcTalk (machine room / pig gate favor / elder hint lines), 801184fc updateRaftReturnNpc,
  80118338 stepRaftReturnTalk, 8011974c updateWaterfallElderHintSceneObj; x018 801170f4 updateJuiceManJoinFight
- NPC init helpers (pattern `init<Npc>`: called only from case 0 of `update<Npc>`; loads box0..3, w1e, the sprite set and the anim from the NPC's
  per-subtype table, AnimLoadDuration, b04++): x003 8012b2e4 initFungaDwarf, 8012bd5c initTreasureChestGuard, 8012c68c initTunnelDigger,
  80134cf8 initHidingSon, 80129a2c initPhoenixSpiritHintNpc; x004 80129914 initScaredGoldVolunteer, 80129fc8 initPaintingSlotHintNpc,
  8012f9f4 initUnbendingCableNpc, 801301b0 initHidingSon; x005 80117050 initBacoTownNpc; x010 8012dea0 initHidingSon (twin of x004 801301b0),
  80125ae0 initLostHatMan, 80126f48 initTenThousandYearElder; x011 8011977c initFactoryNpc; x016 80117e04 initClockTowerRestoredNpc,
  80118974 initJuiceSeller; x017 80118388 initRaftReturnNpc, 80117e9c initWaterfallHintNpc; x018 80116f7c initJuiceManJoinFight, 8011a1b4 initWitchMizuno
- Talk object handlers (same `switch (o->b04)` init/active/free shape): x003 80129be4 updatePhoenixSpiritHintNpc (runs stepPhoenixSpiritHintTalk),
  x004 80129524 updateMermaid (stepMermaidTalk), x009 801294b4 updateFlowerTearsSeeker (stepFlowerTearsSeekerTalk; twin of x004 80129524),
  x017 80118014 updateWaterfallHintNpc (stepWaterfallHintNpcTalk)
- `x016 80117870 stepBrokenStoneGearsTalk` - step switch of updateBrokenStoneGearsNpc: case 0 waits for the player at the gears (or a used item 0x3e), 1 brokenStoneGearsTalk, 2 brokenStoneGearsRepair
- `x016 8011767c brokenStoneGearsRepair` - locks the player, setEventComplete(0x61) and removeItemFromInventory(0x3e) (the item given for the gears), waits 200 ticks, releases
- `x016 80118898 stepJuiceSellerTalk` - step switch: case 0 talk / used item 3 (bananas, D_8009D12F == 0), 1 juiceSellerTalk, 2 juiceSellerSqueezeTalk
- `x001 8013bc24 updateRaceEventObj` - unlayered object variant 5 (MAIN0 dispatchUnlayeredByVariant table D_8007C1C8[5]): runs the race functions by GCD.bce59/bcede
  (raceStartCountdown/raceResult/raceOfferRematch/raceStartRematch); `8013a4b0 initRaceEventObj` spawns the race partner object, `8013a664 checkRaceEventTrigger` the per-state start condition (player x >= 0x19B, ...)
- `x002 8011c628 updatePigGateScene` - unlayered variant 0x23 (b04 shape: spawn table, step, free); `8011c4ec stepPigGateScene` - its step switch over pigGateDiscoveryScene / pigGateSevenFriendsScene
- `x014 801168a0 callSectionClearController` - one-line thunk to updateSectionClearController
- `x010 8011e990 probeTiltedBarContact` (uncertain role) - contact test of the player (a) against a rotated bar object (b) at angle `ang`: box prefilter, rcos/rsin of the angle times b->box0,
  then either hang contact (a->b9e = 9, a->wb8/wba offsets) or side contact (b9e = 8); sets SPAD.p3c0 = b, b->w7a = mode; `8011ef98 probeTiltedBarObject` - calls it by b->subtype (one or two sides, angle d30, d30+0x400), only when GV2.bc3 & 0x40
- `updateAreaScroll` (x001 80116990, x002 80116358, x003 80117494, x004 80116d3c, x005 80115ee4, x006 801165bc, x008 80115ea8, x009 80116278, x010 801165e0,
  x011 80115fc0, x013 80115fb4, x016 80115f88, x017 8011611c, x018 801162d8, x019 80115ea8) - the area's scroll/section controller: waits for GVARS.b3a, runs the
  scroll mode of `o->subtype` from the MAIN0 tables D_80079A98 / D_80079AD0 / D_80079AB4 (camera follow functions FUN_80029cb4, FUN_8002a4d0 ...), then per
  GAME.selectedSection compares the player position with the section borders (markSectionVisited + selectedSection change, tryStartItemInteraction exits,
  nextArea warps). x016's is the address MAIN0 `dispatchSectionScroll` calls for sections != 0. x009 has two: `801165a0 updateAreaScrollB` (sections 1-5, tables
  A98/AB4) chosen by `80116dd0 dispatchAreaScroll` (section 0 -> updateAreaScroll, 1-5 -> B, 6 -> 80116bd0). `callAreaScroll` (x001 80117490, x003 80117dc4,
  x004 80118700) - one-line thunks to it, entries 0x16/0x18/0x19/0x1c of the D_80079A98 table (the csv "twin" PadStartCom is only the masked jal)
- `updateMightyFishFood` (x004 8011e594, x010 8011da50, x013 80116cdc) - appears when item 0x7E is used (GVARS.b40 / b41 == 0x7E), floats with an rcos sway;
  the same item completes event 0x77 "Mighty Fish Food" (title table, CFNT999)
- Event objects named after the event title they start/complete (`setEventStarted/Complete(id)`, title = CFNT999 entry D_80080410[id]); `update*` = unlayered
  object of MAIN0 dispatchUnlayeredByVariant (table D_8007C1C8, variant = index) unless noted, `step*` its step switch, `*Scene` the locked-player sequence:
  x004 8012e6d0 updateKeyHoles (variant 0x13; b04 0 jumps straight to free), 8012e554 stepKeyHoles (player standing at x 0x370/y -0x1D6 or 0x3C8/-0x2E4),
  8012dfbc largeKeyHoleScene (event 0x37 "A Large Key Hole!"), 8012e204 smallKeyHoleScene (0x33 "A Small Key Hole!"); 8012f10c paintingBigKeyReward (completes
  0x38 "Painting of a Big Key?", addItemToInventory(0x35)); 8012f508 hauntedPigBagEventStart (starts 0xAE "The Haunted Pig Bag");
  x010 8012eba0 underwaterPigBagEventStart (starts 0x7B "The Underwater Pig Bag"), 8012ed44 updateUnderwaterPigBagStart (variant 0x17), 8012ecd8
  stepUnderwaterPigBagStart (unreferenced copy of its step switch); 8012e328 junglePigBagEventStart (starts 0xB7 "The Jungle Pig Bag"), 8012e494
  updateJunglePigBagStart (variant 0x1D), 8012e728 updateJunglePigBagComplete (variant 0x33, completes 0xB7); 801190bc updateRaft (layer 4 type 0x32:
  item 0x5C used -> player step 0x12, on contact completes 0x96 "Let's Ride the Raft!", accelerates right and warps to area 4 spawn 11);
  x002 80119914 updateFlowerGardenEvent (variant 0x24: spawns object 0x5F when AP >= D_8009C98C), 80119788 flowerGardenCompleteScene (completes 0x27
  "Plant a Flower Garden"); x003 80133a2c updateWindDiesEvent (variant 0x28), 80133938 stepWindDiesEvent (same step logic, unreferenced copy), 801337b4
  windDiesEventScene (starts/completes 0xAA "When the Wind Dies..."); 80133df4 updatePhoenixFavoriteEvent (variant 0x2C), 80133cb4 stepPhoenixFavoriteEvent
  (counter GCD.bd0e6 >= 5 -> completes 0xAB "The Phoenix's Favorite"), 80133bdc phoenixFavoriteCompleteScene (unreferenced copy of the completion states);
  x009 8011c5cc updateApBox (called from 800ee25c), 8011c30c stepApBoxEvent (AP <= 149,999 starts event 0x10 "The AP Box", more completes it),
  801276cc checkMonsterHuntComplete (called from the jellyfish step 8012781c; completes 0x25 "Monster Hunt");
  x001 8013946c brokenFountainScene (step switch run by updateFlowerTowerKeeper: fountain animation, completes 0x1B "The Broken Fountain")
- `updateLeafButterfly` (x001 8013839c, x010 80123d18, twins) - leaf butterfly object: collected with addItemToInventory(0x10); `leafButterflyEventStart`
  (x001 80137968, x010 80123330) - its first-contact state: locks the player and starts event 0xB2 "Leaf Butterflies"
- Section NPC sets: unlayered objects (MAIN0 dispatchUnlayeredByVariant, D_8007C1C8[variant]) whose only work is case 0: a spawn function walks a table of
  0x1C-byte records (flag 0x80 -> layer 4 type 0x1A object, else layer 2 type 0x32 talk NPC with subtype/b0c/wb4/wb8/wba/w74..w7a from the record, d90 = the set);
  then idle until freed (code twin of x003 80133668). Named `updateNpcSet<variant>` / `spawnNpcSet<variant>` (variant = global table index, so the same
  suffix never means two different objects): x005 8011762c/8011724c (09), x017 80118b8c/8011890c (12; also clears D_8009C330->b1e near x 0x100..0x190 until
  GCD.bcfd6), x004 8012dc5c/8012da60 (1B), x016 80117408/80117228 (1C), x011 80119ba8/801199f0 (1E), x004 8012df14/8012dcf4 (1F), x003 80133668/80133448 (20),
  x010 8012d5d8/8012d3b8 (21), x016 80117190/80116f88 (22), x004 8012d9c8/8012d788 (25), x003 8013433c/80134118 (29), 80134630/801343d4 (2A),
  80134080/80133e94 (2B), x018 8011765c/80117484 (38), 801173ec/801171ac (39). The same spawn step of already named objects: x003 801318e4
  spawnSoakedPhoenixGuideNpcs, x009 80129c98 spawnAspiringActorGuardNpcs, x010 8012e114 spawnJunglePigBagStartNpcs, 8012e550 spawnJunglePigBagCompleteNpcs
  (twin of x003 80133e94), x002 80119a1c spawnPigGateSceneNpcs, x017 80118c6c spawnWaterfallElderHintSceneNpcs
- `x004 8012f35c updateBigKeyPainting` - variant 0x14: until the five flags GCD.bcf24[0..4] are set, then runs paintingBigKeyReward
- Enemy helpers (step functions of a named enemy handler; b04 1 = active steps, b04 2 = after being hit: step 0 `*Hit` starts with velV -0x400, facing
  1 - w7a (awardAPByArea when the hit points w98 reach 0), the last step `*Defeated` = active 2, b0b 1, b0f 4, velV -0x400, movetab D_80077D0C: the enemy flies off
  the screen): x001 801282d0 purpleCrawlerIdle (step 0: waits 0xC0/0x180 ticks, then turns to the player), 801285e4 purpleCrawlerWalk (step 1: walk anim, sfx 0x5E,
  movetab), 8012ab64 purpleCrawlerHit, 8012b184 purpleCrawlerDefeated; x009 801233f0 redFlowerHit, 80123ce8 redFlowerDefeated; x010 8012aae4 spearPigDefeated
- x001 forest dwarf (updateForestDwarf): 801356c0 initForestDwarf (box, anims per section/subtype), 80135850 stepForestDwarfWander (subtype 0: walk 0x3C ticks, turn),
  80135c90 forestDwarfFlee (b04 2, subtypes 1/2: hops away with velX +-0x120), 80136164 forestDwarfCaught (b04 2, subtypes 3/4: locks the player, plays the caught anim)
- x001 others: 80136b4c stepPuppyBaron (step switch of updatePuppyBaron), 80139b2c checkLostSeedKidTalk (player standing at the kid's ledge pressing the talk
  button), 8013a0d8 spawnPuppyRumorNpcs (case 0 of updatePuppyRumorNpc: spawns the layer 2 type 0x16 objects while GCD.bceab is 1..2), 8013759c initLeafButterfly,
  80137714 leafButterflyFlutter (sine flight, 4 cycles); x009 80128180 initJellyfish (case 0 of updateJellyfish)
- x014 evil pig bosses (AREA14 = the evil pig rooms; one boss per section, layer 2 types 0x3B..0x42 = 0x3B + section, all handled by
  `8011f4e0 updateEvilPigBoss`, which stores itself in GVARS.p48). The section -> boss mapping comes from the event each section completes on clear
  (D_801256A8, see updateSectionClearController / objects.py AREA14_BOSS_EVENT) and its CFNT999 title: 0 "Phoenix Mountain", 1 "Baccus Village",
  2 "Trick Village", 3 "Lava Caves", 4 "The 100 Flower Forest", 5 "The Deep Jungle Pig", 6 "The Haunted Mansion", 7 "The Real Evil Pig"; the intro
  lines (MFNT140S group 2, index D_80125E20[section]) are the "Cerdiablo" speeches (section 7: "Soy el último Cerdiablo, el que creó los otros").
  `8011f218 evilPigBossIntroTalk` (showMessageBoxDirect of that line), `8011d9d0 initEvilPigBoss` (anim/box/point tables D_80125CC4/CE4/D04 by type;
  setRank in section 7), b04 1 steps: `8011dc5c evilPigBossTeleport` (picks one of 16 points of the d94 table that is on screen, flickers out, reappears there),
  `8011e550 evilPigBossAttack` (aims at the player, picks by the fight timer wb4 from D_80125CA8/CB8/CC8 and runs the section attack D_80125CD8[section];
  section 7 goes through `8011e1f0 realEvilPigChooseAttack`, which runs the attack of a chosen other section), `8011db10 evilPigBossWait`; b04 2 steps:
  `8011e76c evilPigBossHit`, `8011eb08 evilPigBossStunned` (anims 11/12 with timers, uncertain), `8011ee50 evilPigBossDefeated` (locks player and scroll,
  anim 15, falls to the floor); helpers `8011d604 evilPigBossCheckWall` (room bounds D_800A457C/E +- 0x90, wall probe), `8011d7b0 evilPigBossCheckLanded`,
  `8011d88c evilPigSetAnimBox` (box + anim number n from the boss tables), `8011d948 evilPigSetBoxByDirection` (box from wb8)
- The attack table D_80125CD8 (indexed by section): 8011f8c0 evilPigAttackPhoenixMountain, 8011fa34 evilPigAttackBaccusVillage, 8011fba0
  evilPigAttackTrickVillage, 8011fe14 evilPigAttackLavaCaves, 8011ff80 evilPigAttackFlowerForest, 80120160 evilPigAttackDeepJungle, 80120330
  evilPigAttackHauntedMansion, 8012049c evilPigAttackRealEvilPig. Their projectile spawns (allocObjectLayer2 + type) and the handlers of those types
  (MAIN0 layer 2 table 0x80077D9C): 80120638 spawnTrickVillagePigShot (type 0x43 -> 801206c0 updateTrickVillagePigShot), 80121170
  spawnBaccusVillagePigShot (0x46 -> 801215b0 updateBaccusVillagePigShot), 8012176c spawnFlowerForestPigShot (0x47, aimed at the player ->
  80122418 updateFlowerForestPigShot, also placed twice in section 4), 801227c0 spawnHauntedMansionPigShot (0x48 -> 80122de8 updateHauntedMansionPigShot)
- x001 Dwarf Forest objects (item names: printInfoMessage table D_8007DE38 + MFNT; event titles: D_80080410 -> CFNT999):
  `80119bf4 updateFlowerTearsThrow` (layer 3 type 0x54: when item 0x0D "Flower Tears" is used, a tear flies from the player to x 0xB5E, sets
  GCD.bd081 = 1 and removes the item; `80119ad0 spawnFlowerTearsSparkle` its trail, uncertain), `80119874 updateFlowerTowerSprout` (type 0x53:
  waits for GCD.bd081 == 2 (brokenFountainScene), plays the sprout growth, fade + thread LAB_8001d6a4 (the tower scene), bd081 = 4, starts event
  0xA4 "The Flower Tower"; `80119750 spawnFlowerTowerSproutSparkle` (uncertain) the type 0x31 sparkles around it), `8011c158 updateFlowerTowerBlocker`
  (layer 4 type 0x0B: box from D_8013C484, freed when bd081 >= 4 or GCD.bce48; uncertain role), `8011ffbc updateFlowerTowerGrowthFlag` (type 0x2D:
  sets bd081 4 -> 5; uncertain);
  `8011d73c updateLookAndSeeViewpoint` (layer 4 type 0x12: pressing up on it fades out and runs the scene thread D_8001D6A4 (b1cd 5 / 8 by
  GCD.bce40), then completes event 0x16 "Look and See?!"), `80123f24 updateLookAndSeeEventStart` (layer 5 type 0x0D: on contact starts event 0x16);
  `8011fb10 updateBucket` (the bucket object of MAIN0 dispatchEffectByVariant; `8011faa8 stepBucket` unreferenced copy of its step switch):
  `8011f2b0 bucketFillWithWater` (held under the drip, GCD.bcead counts the fills, printInfoMessage 0xD/0xE, then removeItemFromInventory(0x0A
  "Bucket") + addItemToInventory(0x12 "Bucket of Water")), `8011f6c4 bucketThrowWater` (flies to x 0x79B, spawns layer 2 type 0x22 (updateCeilingDrip
  subtype 1) and sets GCD.bce55 = 2); `801315dc updateBurningFire` (layer 2 type 0x0F: animates until GCD.bce55 == 2 (the thrown water), then
  becomes the charred subtype 1); `80131ca4 updateRollingBarrel` (layer 2 type 0x11: pushed by hits (velX from the hit angle table D_8013C984),
  rolls down; starts / completes event 0xB4 "Where The Barrel Rolls...", GCD.bceb0 = progress);
  `80118ac0 updateAnimScenerySprite` / `80118c1c updateScenerySprite` (layer 3 types 0x18 / 0x19, 35 / 170 placements: decoration sprites, scale
  d64 and rotation d8c from the two nibbles of box0, the first animated, the second static with an optional palette D_8013C404/428);
  purple crawler steps (uncertain): `80128a2c purpleCrawlerClimb` (step 2: turns d8c +-0x40 and moves 0x300/0x80 per tick along a wall, sfx 0x5E),
  `80129588 purpleCrawlerJump` (step 3: velV -0x400 / -0x500 hops); forest dwarf: `80135b48 forestDwarfCountCatch` (sets the GV2.sc0 bit of b0c,
  GCD.bcdb5 += 1 with printInfoMessage(count - 1, 2), GCD.bcec1..7 flags), `80135a14 forestDwarfAssignSlot` (uncertain: picks a free slot of
  GCD.bcec1[0..6]); `80137afc leafButterflyReleased` (b04 2 step 3 of updateLeafButterfly: after item 0x10 is used, flutters inside the box of
  the parent d90; uncertain)
- Koma pig (layer 2 type 2; MAIN0 `func_8004985C` calls the area's handler: area 0 X000 8012CCF8, area 1 x001 `8012efc4 updateKomaPig`, area 3
  x003 `80124b78 updateKomaPig`, area 4 x004 `80123750 updateKomaPig`). The identity is uncertain (the rendered strips are not readable): the
  handlers draw from SPR_DATA[2] with w1e 1, the pig sprite table of updatePurpleCrawler / updateSpearPig, and type 2 is placed in the first room of
  area 0. Same template as the other enemies: b04 2 step 0 `komaPigHit`, step 1 `komaPigGrabbed` (spawnItemNotification(0, 6), the bite hold),
  step 2 `komaPigDefeated` (velV -0x400, movetab D_80077D0C, flies off): x001 8012df98 / 8012d4ac / 8012ca08 (+ step 3 `8012cb78 komaPigGrabbedB`,
  also notification 0), x003 8012338c / 80122db4 / 80122c44, x004 80122a54 / 8012247c / 8012230c. The b04 1 step 1 states (x003 / x004, same
  order; names from the anims and velocities, uncertain): state 0 `komaPigIdle` (random anim from D_80135B20, timers 0x38/0x60) 801226a0 / 80121d44,
  1-2 `komaPigWalk` 80121fd0 / 801216b4, 3 `komaPigRest` (anims 8, 0xC..0xE, 0x20 with timers) 80121a88 / 8012116c, 4 `komaPigLeap` (sfx 7,
  velV -0x400, movetab D_80077D0C) 80121398 / 80120a7c, 5-6 `komaPigJump` (velV -0x280) 801210a8 / 8012078c, 7 `komaPigFall` (bounces with dust
  spawns until landed) 801209f8 / 801200dc. x003 helpers `801206f0 komaPigCheckLanded` (floor probe FUN_80040278, slope -> d8c, deadly tile
  class >= 0xE -> w98 = 0), `8012080c komaPigCheckWall`; `801243b8 komaPigDropInIntro` (subtype 4 entry of the subtype table D_80135C64: locks the
  player, the camera follows the pig falling in with bounces and dust, then starts event 0x1F "Where did I Come From?")
- `x001 80126a70 burningFireContact` - MAIN0 FUN_80047484 contact case type 0xF (updateBurningFire in area 1): on touching it the first time starts
  event 0xB1 "Something's Cookin'?" (flag GCD.bcecc) and knocks the object into b04 2
- x001 platforms and scenery (identities from the code; the rendered strips are not readable, so object kinds marked uncertain are guesses):
  `8011dd94 updateSeesawPlank` (layer 4 type 0x14, 7 placements: a 0xEC-0xEE wide, 8 high solid box (b0a 0x11) whose angle d8c = d38 >> 8 is driven by
  `8011db5c seesawPlankTilt` from the rider's velX (b69 contact), clamped +-0x38E00 (subtype 0, pivot in the middle) or 0..0x80000 (subtype 1, hinged);
  b0c 1 planks take part in the purple crawler scene GCD.bceae), `8011d4d0 updateHangingVine` (type 0x10: allocates 18 more layer 4 objects and links them
  as a chain of segments 0x10 apart, d90/d94 parent/child), `8011c354 updateBendingBranch` (type 0x0E, uncertain: bends down (d8c -= wb4 >> 8, sfx 0x4B)
  while stood on), `8011f1a8 updateClimbPole` (type 0x19, uncertain: 0x10 x 0x72 box), `8011fd7c updateBreakableBlock` (type 0x21, uncertain: on hit
  spawns debris and an applyItemEffect burst, sets GCD.bceae = 4, frees itself and its partner d94 and sets both area flag bits),
  `8011c574 updateRopeChains` (type 0x0F, uncertain: builds six chains of layer 4 segments from the table D_8013C498 copied to D_800A4470),
  `80120f4c updateBackdropSprite` / `801210e8 updateParallaxBackdropSprite` (layer 7 types 4 / 7: screen-space backdrop sprites positioned from the
  camera SPAD.i174 / s186), `8012f448 updateChainSegmentEnemy` (layer 2 type 0x0B, 22 placements, uncertain kind: each part follows the root of its d90
  chain at a fixed offset rotated by the root's d8c; biting (b04 2, subtype 0 shows spawnItemNotification(0, 6)) frees the chain, then the part flies off
  like a defeated enemy (movetab D_80077D0C));
  `8011a1c0 updateSpringVine` (layer 4 type 5, 29 placements, uncertain kind: subtype 0 = the handle, subtype 2 = the rope quad drawn to the parent d90;
  step functions D_8013C44C[step] for itself and D_8013C460[step] for the rope, run by `8011a528 stepSpringVine`): `8011a5dc springVineIdle` (step 0),
  `8011a85c springVineGrabbed` (step 1 when the player hangs on it (b6a): stretches down and springs back with sfx 0x42/0x43 (section 0) or 0x47/0x48;
  b0c 1 releases the object D_8013C474 (spawnLayer2ObjectUnlessFlagged), b0c 3/4 a forest dwarf); `8013675c spawnForestDwarfAt` (allocObjectLayer2
  type 0x16 = updateForestDwarf at x/y/z, b0c = dwarf number, skipped when its GV2.sc0 bit is set)
- x003 (the windy mountain area: GV2.bc3 & 2 = the wind has died, see updateWindDiesEvent): `80118f3c updateWindStreak` (layer 3 type 0x29, 92 placements,
  uncertain: semi-transparent sprites that cross the screen to the right and respawn at the left edge (`80118df8 windStreakRespawn`, random height and
  depth); freed once the wind has died), `80118cf4 updateWindSwayDecor` (type 0x28, 74 placements, uncertain: animated decoration that switches to its
  calm animation (subtype + 2) after the wind dies), `8012552c updateSwayingHopPlant` (layer 2 type 0x24, 82 placements, uncertain kind:
  `8012526c swayingHopPlantSway` rocks d8c while idle; when the player comes within x +-0x2D and 0xB0 below, `801253b0 swayingHopPlantHop` jumps up
  (velV 0x200, -0x20 per tick) and falls back to its spawn height);
  `8012a890 updateMasterOfTheSkiesEvent` (run by MAIN0 func_800498F4 in area 3 section 3: when GCD.bd004 is set (or the check passes) removes 5 x item
  0x42 "Bunk Flower" and runs `80129cc0 masterOfTheSkiesScene`, which starts / completes event 0x23 "The Master of the Skies"; b04 2 = the movie part:
  cdSeekStream(7), fade, thread); `8011dd48 playerEnterHollowForItem` (uncertain: a playerEventModeDispatch mode: the player walks into the depth
  (z += 1 per tick, invisible), receives the item of the spot w7a (0x25 "Red Evil Pig Bag" + event 0x1D "A Stormy Pig Bag" completed / 0x1E "Phoenix
  Mountain" started, 0x07 "Charity Wings", 0x1F "Large Lunch Box", 0x67 "Molasses", 0x37 "Cheese", once each by GCD flags) and walks back out)
- x004 objects: `801274fc updateCountedCollectible` (layer 2 type 0x29, 219 placements in areas 4/12, kind uncertain: b0c 0 objects are skipped when
  their persistent bit (subtype) is set; collecting one plays sfx 0xF7, bursts (FUN_800ea544), GCD.bcefb += 1 and sets bit subtype of
  SCRIPT_GLOBALS[0x58 + subtype / 8]; subtype 0x16 sparkles and palette-cycles until bcefb reaches 0x16; updateSceneSequence checks bcefb >= 0x17),
  `8011d544 updateStormBackdrop` (layer 7 type 9: backdrop sprite; subtype != 0 also flashes lightning (drawScreenFade 0x80 -> 0, sfx 0x82 every
  0x17C ticks) until area 4 is purified (GAME.purifiedAreas & 8); `8011d804 stormBackdropCycleClut` loads one of 7 palette rows (LoadImage to
  0xD0,0x1E0) on a timer), `8011aa3c updateSmokeEmitter` (layer 3 type 0x2A, uncertain: subtype 0 spawns a subtype 1 copy every 64 ticks;
  `8011a840 smokePuffRise` rises, grows (d64 += 0x20) and changes anim twice before freeing itself), `8011c540 updateHealingSpot` (layer 4 type 0x25:
  standing in the subtype 0 box restores one vitality point every 0x3C ticks, sfx 9), `8011bf10 updateBlockingWall` (type 0x1E, uncertain: 0x82..0xA0
  high box, removed when GCD.bd091 is set (then the camera bound D_800A457E is widened by 0x140); subtype 1 shrinks away when bd091 becomes 1),
  `8011c1b8 updateShuttlePlatform` (type 0x1F: 0x4E wide platform; when stepped on it travels between the two points D_80130CCC with a looping
  sfx 0x83, keyed off on arrival), `8011d2b8 updateRaftArrival` (type 0x44: only at spawn point 11 (where updateRaft of area 10 sends the player):
  carries the player in to x 0xB0, then gives control back (player step 0x12)), `8011c708 updateSceneryModel` (type 0x30, uncertain: model drawn by
  FUN_80018cf0 with fixed rotations per subtype, section dependent visibility), `8012b210 updateCarriedGuide` (layer 2 type 0x33, kind uncertain:
  held at the player's hand like the bucket; `8012aa10 carriedGuideFollowPlayer` keeps the player inside allowed x ranges per depth lane and shows
  MFNT040 dialogue 2/10 "Hey, hey! Not that way!" at the limits), `8011dff8 updateThrownBreakable` (layer 1 type 3, uncertain kind:
  `8011de60 thrownBreakableFly` spins up (velV -0x200 / +0x200) and falls until it hits the floor, then b04 2 bursts it (sfx 0x34) and sets
  bit subtype of GV2.bae)
- Event flag bytes: GCD + 0x8009CDA4 + id is the progress byte of event `id` (0xFF = completed; e.g. x001 Look and See 0x16 = GCD.bcdba, x003 0x1D =
  bcdc1, 0x1F = bcdc3, 0x23 = bcdc7), which ties several objects to their event:
  x009 Death Fruit (event 0x26 "Death Fruit Juice", flag GCD.bcdca; collected count GCD.bcef6 shown with printInfoMessage(0x29 + count, 2), per-fruit flag
  GCD.bcfe5[b0c]): `801246d0 updateDeathFruit` (layer 2 type 0x1A, 5 placements, gone when its flag is set or the event is completed; b04 2 step 2 =
  `8012455c deathFruitCollected`: counts it and flies off spinning), `8011ee50 updateFallingDeathFruit` (layer 1 type 4: same anim D_8012E04C, falls
  spinning 5 px per tick; on the floor awards 500 AP and is counted the same way), `801203cc deathFruitCatchContact` (contact callback: while the event
  runs (bcdca == 1) and the player state wac == 1, the touched fruit goes to b04 2 and is kept in o->de4; uncertain);
  x009 `8011b6bc updateTearJar` (the held object of MAIN0 dispatchEffectByVariant in area 9, like updateBucket in area 1; kind uncertain: the code only
  shows the event): `8011b508 stepTearJar`, `8011b01c tearJarHeld` (follows the player's hand, freed when another item 5/7/0x0E/0x7C/0x97/0x98 is used;
  completes event 0x28 "Tears from a Flower"); `801248d4 updateRedPlusBlueObject` (layer 2 type 0x1C, uncertain kind: subtype 4 completes event 0xBD
  "Red+Blue=?" and sets PLAYER.obj.subtype = 1); `8011bb0c updateBobbingPlatform` (layer 4 type 0x36, uncertain: only when GCD.bcdac (event 0x08 "I Can't
  Swim...") is set; a 0x40 wide platform bobbing on a sine at one of two spots D_8012B26C)
- `playerInitAreaHook` (x001 801212a0, x003 8011dc4c, x004 8011d8c4, x009 8011d334) - the per-area call of MAIN0 playerInitMode (`switch (selectedArea)`:
  1/7, 3, 4/12, 9; area 0 calls X000 801228bc): clears D_8009F0EC, puts the player into a scripted entry state for some spawn points (b04 5 step 9 ...);
  the area 4 one also respawns the carried guide (`x004 8012b388 spawnCarriedGuide`, layer 2 type 0x33 = updateCarriedGuide at the player's hand)
  while event 0x32 "Road to Baccus Lake" is in progress (GCD.bcdd6 == 1) or after it with 0x31 "A Drink for Grownups" (bcdd5) running
- Event trigger checks (set o->step / o->state of their event object; no static caller, so reached through data or a computed call): x003
  `80133708 checkWindDiesEventTrigger` (player on the ground at x 0xC8E..0xCB5, y -0x7BB; event 0xAA progress GCD.bce4e, GV2.bc3 & 2),
  `80133b9c checkPhoenixFavoriteTrigger` (GCD.bd0e6 >= 5 while event 0xAB is open), `8012b5dc preciousChestCheckOpened` (hit (b68) or event 0xB9
  "Precious Treasure Chest?" completed), `8012b63c isPreciousChestEventActive` (GCD.bd0c8 set and 0xB9 not completed); x004 `8012e44c checkKeyHolesTrigger`
  (the two key hole spots of stepKeyHoles; events 0x37 / 0x33 flags GCD.bcddb / bcdd7), `8012f094 checkBigKeyPaintingPanels` (the five GCD.bcf24 flags while
  event 0x38 is open); x010 `8012eb7c checkUnderwaterPigBagTrigger` (event 0x7B not started); x016 `801177b0 checkStoneGearsTrigger` (player at x 0x82..0xA9,
  y -0x3D while event 0x61 is open: item 0x3E "Iron Wheel" used -> step 2 (brokenStoneGearsRepair), else the up button -> step 1);
  x018 `8011a038 selectWitchMizunoTalkStep` (talk step from events 0x84 / 0x74 / 0x78 / 0x72 / 0x67 and GCD.bcfd9 / bcfda); x002
  `8011a0a8 selectSevenFriendsSceneStep` (event 0x9D "Seven Friends": completed -> 3, GCD.bcff7 == 6 -> 2, all seven GV2.bc3 bits -> 1)
- x002: `80119330 updateSevenFriendsMember` (layer 2 type 0x52, uncertain: subtypes 0..6 with start timers 120/180/60/240/30/0/150 until event 0x9D
  starts; `80119178 stepSevenFriendsMember`), `801169c0 updateSevenFriendsBackdropSprite` (layer 3 type 0x4C: camera-fixed backdrop sprite (b0a 7,
  CLUT 0x80,0x1FF) whose subtypes 2..8 pick their frame from the seven GV2.bc3 bits / GCD.bcfdb..bcfe1; uncertain), `80119710 spawnGoldenFlowerIfAffordable`
  (spawns object 0x5F at 0xB0,-0x38 when AP >= D_8009C98C, event 0x27 "Plant a Flower Garden" started and no golden flower yet), `801194ac
  updateLightsOutDarkness` (sets the room fade colour from D_8007C178[area][section]; keeps the screen dimmed (fade amount 0xC0) while event 0xB3 "Where'd
  the Lights Go?" is in progress and fades back in once it is completed)
- `updateBackdropSpriteSet` (x016 801162d0 layer 3 type 0x46, x017 80116d18 type 0x47): camera-fixed backdrop sprites (b0a 7, CLUT 0x80,0x1FF, position
  d30/d34 minus the camera) with per-subtype animations (x017 subtype 0 changes with GCD.bcfd6)
- x004 `801293a0 initMermaid` (case 0 of updateMermaid, only once event 0x7E "The 10 Math Beads" is open (GCD.bce22)), `8012e828 updateMermaidSpawner`
  (unlayered: spawns layer 2 type 0x32 = the MAIN0 func_800498F4 section dispatcher that runs updateMermaid, from D_8013143C, while 0x7E is open;
  `8012e750 spawnMermaidForMathBeads` the same spawn as a one-shot step); x010 `8012b8c0 parrotPumpRocksClutFlash` (updateParrot helper: after event 0x3F
  "The Pump Rocks" is completed, cycles palette rows at 0x120,0x1E0 with MoveImage 12 times; uncertain); x011 `80116d14 updateRaftPickup` (layer 5 type
  0x11: while event 0x96 "Let's Ride the Raft!" is open, touching it gives item 0x5C "Raft"), `801163d4 updateFoodForFuelObject` (event 0x68 "Food for
  Fuel?" (GCD.bce0c), uncertain kind: b0c 1 is hit, the others roll away (velX -0x180) when GCD.bd082 reaches 4)
- Leaf butterfly steps of x010 updateLeafButterfly (twin layout of x001): `80122f48 initLeafButterfly`, `801230d0 leafButterflyFlutter`,
  `801234c8 leafButterflyReleased` (b04 2 steps 1-3), `80123708 leafButterflyRoam` (step 4: random flight patterns D_8012F398 inside the box of the parent
  d90; uncertain); x001 `80137d8c leafButterflyRoam` the same step 4
- x014 evil pig boss effects and shots (ObjAlloc objects are layer 3 (ObjAlloc sets +0x1C = 3), handlers from the MAIN0 layer 3 table 0x80079BAC):
  `80116c10 spawnEvilPigHitStars` (from updateEvilPigBoss / evilPigBossHit / evilPigBossDefeated: layer 3 type 0x15, b0c = the boss step) ->
  `80116d1c updateEvilPigHitStars` (MAIN0 anim D_8001226C from SPR_DATA[1], kept above the boss's head (dy -0x10 / -0x20) while the boss stays in b04 2 at
  that step; uncertain that they are stars), `80116c9c spawnEvilPigAttackFlash` (called by every evilPigAttack*: type 0x60) -> `80116ec0
  updateEvilPigAttackFlash` (semi-transparent anim D_8012A1B0 with the section's palette D_80125B70, placed at the boss offset table D_80125C00 by facing;
  uncertain), `80117c50 spawnEvilPigEffectRing` (evilPigBossTeleport: 24 type 0x61 objects in a ring, given subtype) / `801184cc spawnSectionClearEffect`
  (updateSectionClearController: type 0x61 subtype 5 at the top of the screen) -> `80118760 updateEvilPigEffect` (type 0x61, per subtype step functions
  80117AA8 / 80117D94 / 80118048, subtype 3 emits subtype 4 particles), `80117428 spawnRealEvilPigEffect` (realEvilPigChooseAttack: type 0x4F) ->
  `80117490 updateRealEvilPigEffect` (semi-transparent anim D_8012A014 for 0x78 ticks), `8011d6bc evilPigBossDefeatedCheckWall` (room bounds +-0x90 and
  wall / floor probe, used by evilPigBossDefeated), `8011f850 evilPigPlayAttackSfx` / `8011f888 evilPigPlayAttackSfxB` (FUN_8001e4f0 of the section's
  sound D_80125E30 / D_80125E50; called by the attacks);
  shot spawns (allocObjectLayer2) and their layer 2 handlers: `80122f18 spawnPhoenixMountainPigShot` (evilPigAttackPhoenixMountain, type 0x49 ->
  `80122fa0 updatePhoenixMountainPigShot`, sfx 0xCA / 0xEF in section 7), `80123698 spawnLavaCavesPigShot` (evilPigAttackLavaCaves, type 0x4B -> `8012415c
  updateLavaCavesPigShot`), `8012441c spawnTrickVillagePigShotB` (evilPigAttackTrickVillage, its second shot kind, type 0x4F -> `801244d0
  updateTrickVillagePigShotB`), `80124848 spawnDeepJunglePigShot` (evilPigAttackDeepJungle, type 0x50 -> `801251b4 updateDeepJunglePigShot`)
- x010 `80125040 updateFlyingBug` (layer 2 type 0x27, 30 placements in areas 10; kind uncertain: SPR_DATA[3], 16 x 16 box, moves freely along the angle d38
  (any direction, below the ceiling y -0x398), gone once GV2.bc3 & 0x40): step 0 `80124370 flyingBugWander`, step 1 `80124a00 flyingBugChase` (while the
  player is near, speed velH 0x100..0x200, turning), b04 2 `80124f0c flyingBugDefeated` (velV -0x400 fly-off), `80123ecc flyingBugProbeBlocked` (wall /
  floor / ceiling probes by the direction octant, returns blocked bits)
- NPC anim setters `o->anim = TABLE[o->subtype].anims[n]; AnimJump(o, k)` (one copy after each NPC's code, named after the talk functions that use
  the same table; several copies are unreferenced): x002 `80117cec sevenFriendsSetAnim` (D_8011C778, also skips when n == wd2), x003 `801289c8
  npcSetAnim` (D_80135D84: flame blocked / soaked phoenix / phoenix spirit NPCs), x004 `801280c0 mermaidSetAnim` (D_8013117C), `801295cc
  scaredGoldVolunteerSetAnim` (D_80131188), `80129b64 paintingSlotHintNpcSetAnim` (D_801311B8), `8012a26c townHallWorkerSetAnim` (D_801311D0), x009
  `80128af8 npcSetAnim` (D_8012B2E4: flower tears seeker, aspiring actor guard), x010 `80125dac tenThousandYearElderSetAnim` (D_8012F3C4), x018
  `80116b58 juiceManSetAnim` (D_8011A4B8), `8011774c witchMizunoSetAnim` (D_8011A564)
- x010 updateShieldSpearPig steps: `8012cb50 shieldSpearPigFungaShout` (step 0: showMessageBoxDirect(2, 2) = MFNT100 2/2 "Funga!"), `8012cce8
  shieldSpearPigHongagaaaShout` (step 1: 2/3 "Hongagaaa!")
- Camera-fixed backdrop sprites (b0a 7, CLUT 0x80,0x1FF, drawn at d30/d34 minus the camera): `updateBackdropSpriteSet` x005 80115fe0 (layer 3 type 0x30),
  x013 801166f8 (type 0x58, 15 placements); x018 `801165a8 updateAnimBackdropSprite` (type 0x4D, animated); x018 `801166f0 updateDriftingCloud` (type
  0x5E, uncertain: semi-transparent quad that drifts right 0xC000 per tick and wraps from x 0x1E1 to -0x78)
- x013 camera: `801162d0 dispatchAreaScroll` (section 0 -> updateAreaScroll, other sections follow the player clamped to the camera box),
  `80116234 cameraFollowPlayerClamped` (the same clamp as a separate function)
- x009 jellyfish (updateJellyfish) helpers: b04 2 step 0 `80126890 jellyfishHit`, step 5 `8012781c jellyfishDefeated` (checkMonsterHuntComplete,
  spawnItemDropAtPos), b04 1 step 3 `80125a38 jellyfishSpawnChild` (allocObjectLayer2 type 0x21 = another jellyfish with b0c 1, which copies the parent's
  b04/step/state), step 1 `8012516c jellyfishSwim` (velV 0x100 strokes with sfx 0x97; uncertain); x009 red flower b04 2 step 1 `80123828
  redFlowerGrabbed` (between redFlowerHit and redFlowerDefeated; drives PLAYER.state / wac while held; uncertain)
- x004: `8011ec78 cullObjectPerspective` / `8011ea30 cullObjectPerspectiveWide` (cullObjectToDrawList with the screen box scaled by the object's depth
  relative to the camera (s17e), depth window +0x5A / +0xB4 out of 0x277, then pushes the object on the draw list of its category); black crow
  (updateBlackCrow) steps: b04 1 step 0 `80126d04 blackCrowPerched` (state switch over 80125420 / 80125554 / 801256A8 / 80125818 / 80126464; uncertain),
  step 1 `80126688 blackCrowSwoop` (velV -0x400 hops, flight angle from angleBetweenPoints, sfx 0x13; uncertain), b04 2 step 1 `80126c50 blackCrowGrabbed`
  (spawnItemNotification(2, 6))
- x003 phoenix spirit hint NPC group (stepPhoenixSpiritHintTalk by wba 0/1/2; all uncertain): `80128a1c phoenixSpiritHintEnterA` / `80128c08
  phoenixSpiritHintEnterB` (fall in from the right to x 0x1F4, GCD.bceef counts the arrivals, step 1 when all four are in), `80128df8
  phoenixSpiritHintLeaveA` / `80128fa0 phoenixSpiritHintLeaveB` (run off left at velX -0x300), `80129240 phoenixSpiritHintWalkC` / `80129430
  phoenixSpiritHintLeaveC` (wba 2 walk / leave); `80129148 stepPhoenixSpiritHintGroupA`, `801291c4 ...GroupB`, `801295b0 ...GroupC` (unreferenced copies of
  the per-wba step switches)
- x009 flower tears seeker: `80129344 initFlowerTearsSeeker` (case 0 of updateFlowerTearsSeeker: box / SPR / anim from D_8012B2DC), `80128ecc
  flowerTearsSeekerPace` (walks between x 0x708 and 0x780 until talked to; freed off screen once event 0x28 is completed), `80128b4c
  flowerTearsSeekerWalkOff` (walks left until off screen, unreferenced; uncertain); `8011f128 updateItemPickup` (layer 5 type 0x0E: initItemObject item,
  gone when D_8009D119 or GCD.bcf06 is set; b04 2 runs the MAIN0 item effect D_8007A890[D_8007A7F0[subtype]])
- x011: `8011617c updateBackdropSpriteSet` (layer 3 type 0x45), `80116b58 updateTossedItem` (layer 5 type 0x0F: initItemObject item thrown in an arc
  from 0x3C,-0x50 for 0x3C ticks, collectable once falling), `80116fa8 updateFlyingOctopus` (layer 2 type 0x2A, 1 placement, kind uncertain: 32 x 32
  box, palette 0x7952; `80116e54 flyingOctopusMove` moves along its facing with a sine bob for one d88 cycle, hovers one cycle, then turns)
- Evil pig bag portal (one copy per area overlay, same code; "bag" is a guess from the evil pig bag events, the code only shows the gate): the handler
  `updateEvilPigBagPortal` (x001 80120a2c, x002 801178f4, x003 8011d84c, x004 8011cec0, x009 8011cd84, x010 8011a778, x016 80116a60) exists only while the
  area's gate flag GCD.bd0c9 / bd0cd / bd0cf / bd0cc / bd0ce / bd0ca / bd0cb (x001, x002, x003, x004, x009, x010, x016) is set and the boss bit of GV2.bc3 (2 / 1 / 8 / 4 / 0x20 / 0x10 / 0x40) is
  clear; b0c 0 is the body (box 0x18 x 0x32, palette row loaded to 0x80,0x1FC), b0c 1/2 the two halves that follow it with the body's scale w74 and swing
  open (d88 to +-0x638) once told to (b6a); GCD.bcf0d/bcf0f remember it open. Steps of the body: on screen -> `evilPigBagPortalOpen` (x001 801205f0,
  x002 801174b4, x003 8011d40c, x004 8011ca80, x009 8011c944, x010 8011a338, x016 80116620: locks the player, centres the camera on it, grows w74 0x800 ->
  0x1000, then opens the halves) -> `evilPigBagPortalEnter` (x001 80120780, x002 80117644, x003 8011d59c, x004 8011cc10, x009 8011cad4, x010 8011a4c8,
  x016 801167b0: when `evilPigBagPortalCheckPlayer` (x001 801204dc, x002 801173a0, x003 8011d2f8, x004 8011c96c, x009 8011c830, x010 8011a224, x016
  8011650c: player box overlaps it within +-0x2D in depth and is free to act) says the player walked in, hides the player and warps to area 14 (the evil pig
  rooms) section x001 0 / x002 4 / x003 6 / x004 3 / x009 5 / x010 1 / x016 2, spawn 0)
- Player contact callbacks: MAIN0 `FUN_80047484` walks the layer 2 objects touching the player and calls a callback by the object's type (q[2]); the
  callbacks live in the overlay that has the type, so each is named `<handler>Contact` after the layer 2 handler of that type in the same overlay
  (MAIN0 table 0x80077D9C): x001 `801262f8 purpleCrawlerContact` (type 0), `801240a8 komaPigContact` (type 2, area 1 branch), `8012687c
  chainSegmentEnemyContact` (0x0B), `80126bbc rollingBarrelContact` (0x11), `80124348 puppyBaronContact` (0x17), `801242c0 forestDwarfContact` (0x16),
  `801243d4 ceilingDripContact` (0x22); x003 `8011f8cc komaPigContact` (2, area 3), `8011fc84 swayingHopPlantContact` (0x24), `8011fad4
  windblownMonkeyContact` (0x37); x004 `8011efa4 komaPigContact` (2, area 4), `8011f6c8 blackCrowContact` (0x1E), `8011f9e0 countedCollectibleContact`
  (0x29); x009 `80120c68 redFlowerContact` (0x0A), `80121180 redPlusBlueObjectContact` (0x1C), `80120f34 jellyfishContact` (0x21); x010 `801206f4
  spearPigContact` (0x3A), `8011dc34 flyingBugContact` (0x27); x006 `8011e3b4 pitchforkPigContact` (0x53); x014 `8011d138 evilPigBossContact` (0x3B..0x42),
  `8011d040 trickVillagePigShotContact` (0x43), `8011c80c baccusVillagePigShotContact` (0x46), `8011ca60 flowerForestPigShotContact` (0x47), `8011cc1c
  hauntedMansionPigShotContact` (0x48), `8011cd6c phoenixMountainPigShotContact` (0x49), `8011ce64 lavaCavesPigShotContact` (0x4B), `8011cf2c
  trickVillagePigShotBContact` (0x4F), `8011c8c0 deepJunglePigShotContact` (0x50). (x009 deathFruitCatchContact and x001 burningFireContact are the
  type 0x1A / 0x0F entries.)
- x004 koma pig helpers: `8011fef0 komaPigCheckWall` (csv twin of x003 8012080c), `8011fdd4 komaPigCheckLanded` (called by every x004 komaPig state)
- Layer 4 collision callbacks (MAIN0 dispatchers by the layer 4 type, table 0x80079D74; named `<handler>PlayerCollision` / `ObjectCollision` /
  `EnemyCollision` after the layer 4 handler of that type in the same overlay):
  `func_80046D58` (player vs layer 4): x001 `80125044 springVinePlayerCollision` (5), `80124ba8 seesawPlankPlayerCollision` (0x14), `801251b0
  bendingBranchPlayerCollision` (0x0E), `8012538c ropeChainsPlayerCollision` (0x0F), `80124058 lookAndSeeViewpointPlayerCollision` (0x12); x003
  `8011f310 hitWobbleObjectPlayerCollision` (0x24), `8011f3f8 knockDownObjectPlayerCollision` (0x22); x004 `8011f5b8 healingSpotPlayerCollision` (0x25),
  `8011f4dc shuttlePlatformPlayerCollision` (0x1F), `8011f3cc blockingWallPlayerCollision` (0x1E); x010 `8011f644 solidSceneryPlayerCollision` (0x28);
  `FUN_80046610` (layer 1 objects (category list 1F800218: thrown objects such as updateThrownBreakable) vs layer 4): x001 `80125ae4
  springVineObjectCollision`, `801258e4 flowerTowerBlockerObjectCollision` (0x0B/0x0C), `80125868 seesawPlankObjectCollision`; x003 `8011f418
  knockDownObjectCollision`, `8011f330 hitWobbleObjectCollision`; x004 `8011f568 shuttlePlatformObjectCollision`, `8011f41c blockingWallObjectCollision`
  (hit result 7 on subtype 0 breaks the wall: b04 2, sfx 7); x010 `8011f5b4 solidSceneryObjectCollision`;
  `func_800491F0` (layer 2 enemies vs layer 4): x001 `801280dc ropeChainsEnemyCollision`, `80127fdc seesawPlankEnemyCollision` (csv twin of
  80124ba8), `80128054 breakableBlockEnemyCollision` (0x21); x003 `801203d0 knockDownObjectEnemyCollision`; x004 `8011fa74
  shuttlePlatformEnemyCollision`, `8011fae8 blockingWallEnemyCollision`; x010 `801210b4 solidSceneryEnemyCollision`
- x003 layer 4 objects (3D models, kinds uncertain): `8011afc8 updateKnockDownObject` (type 0x22: `8011add4 knockDownObjectStep` wobbles when touched
  (b69: spin velH 0x400, bounce), pad vibration, then falls away with sfx 0x7C), `8011b270 updateHitWobbleObject` (type 0x24: `8011b0b8
  hitWobbleObjectStep` swings with a decaying oscillation after a hit (b68))
- x010 `80118b40 updateSolidScenery` (layer 4 type 0x28, 44 placements: a culled model with a collision box per subtype (13 sizes); kind uncertain)
- (The `*EnemyCollision` callbacks above are also the ones MAIN0 func_80049524 calls for the category 5 objects (list 1F800228) against layer 4.)
- Rotated-bar collision helpers (one per kind of colliding object; mode 0 = one face, 1/2 = the faces of a two-sided bar, angle `ang`): x001
  `801244a0 collidePlayerWithTiltedBar` (seesawPlankPlayerCollision), `801278c0 collideEnemyWithTiltedBar` (seesawPlankEnemyCollision), `801253ac
  collideObjectWithTiltedBar` (seesawPlankObjectCollision; csv twin x010 8011f05c); x010 `8011e000 collidePlayerWithTiltedBar` (same source as the x001
  one per its header note), `80121534 collideEnemyWithTiltedBar`, `8011f05c collideObjectWithTiltedBar`
- x010 `80119a6c updateTurningPlank` (layer 4 type 0x34, 30 placements: 0xEC wide solid plank like the seesaw; when touched it turns by a quarter
  (velH +-0x1200, d34 = d38 +- 0x40000 within the subtype limits); with GV2.bc3 & 0x40 it runs 801198F8 instead; kind uncertain) with
  `8011e8e4 turningPlankPlayerCollision`, `80121994 turningPlankEnemyCollision`, `8011f518 turningPlankObjectCollision`; `801194e0 updateRotatingWheel`
  (type 0x33: round solid (radius box0) that turns slowly (subtypes 0/1), section 7 sets GAME.purifiedAreas |= 0x40; uncertain) with `80120c88
  rotatingWheelPlayerCollision` (circle test with SquareRoot0 / ratan2, pushes the player out along the radius), `80120fc0 rotatingWheelObjectCollision`,
  `80121a40 rotatingWheelEnemyCollision`; `8011ad54 updateCollapsingBridge` (type 0x3B, 30 placements: linked planks (d90 / d94 neighbours) that, once
  stepped on (b69), shake and drop one after the other (`8011ab70 collapsingBridgePlankFall`: falls 3 px per tick with a random tilt, then tells the
  neighbours); the subtype 0 anchor swings down; gone once GCD.bd078 is set) with `8011dee8 collapsingBridgePlayerCollision`
- x001 `8011c274 updateSolidBlock` (layer 4 type 0x0C: culled model with a 0x10 x 0x60 or 0x30 x 0x50 box) with `80125074 solidBlockPlayerCollision`
  (csv twin of 80125044); `8011e0d4 updateSwingingRope` (type 0x15, 13 placements, uncertain kind: 0x18 wide, per-subtype height; swings like a pendulum
  (wave table D_8013C714) after a hit (b68)) with `801250a4 swingingRopePlayerCollision`, `80125994 swingingRopeObjectCollision`
- x009 `8011accc updateFlowerPlatform` (layer 4 type 0x1C, 12 placements, uncertain kind: a solid model (box per subtype D_8012B1B8, transformed by
  801174C4 with the plane rotation) that carries red flowers: `8011ae94 spawnFlowerPlatformRedFlower` maps subtypes 1/2/5..9 to entries of D_8012B218 and
  `8011af18 spawnRedFlowerOnPlatform` allocates the layer 2 type 0x0A (updateRedFlower) object at the entry's offset with d90 = the platform) with
  `8012047c flowerPlatformPlayerCollision`, `801208c4 flowerPlatformObjectCollision`, `801216a0 flowerPlatformEnemyCollision`
- Area names: the MAIN0 sources use the constants AREA01_DWARFFOREST, AREA02_DWARFVILLAGE, AREA03_PHOENIXMOUNTAIN, AREA04_HAUNTEDMANSION,
  AREA05_BACCUSVILLAGE, AREA06_DIRTMOTOCROSS, AREA08_BACCUSLAKE, AREA09_MUSHROOMVILLAGE, AREA10_DEEPJUNGLE, AREA11_VILLAGEOFCIVILIZATION,
  AREA13_PIGISLAND, AREA14_EVILPIGS, AREA16..19 (clock tower, iron tower, Y crossing, purified village of civilization).
- drawObject render kinds 6..10 call fixed overlay addresses (rotated / scaled FT4 sprite strips): kind 7 `x003 8011ebc0 drawRotatedSpriteKind7`,
  kind 8 `x009 8011f240 drawRotatedSpriteKind8`, kind 9 `x014 8011b830 drawRotatedSpriteKind9`, kind 10 `x014 8011c00c drawRotatedSpriteKind10`
  (kind 6 = X000 80123FD0); in area 6 kinds 0..3 go to x006 `8011cef0 drawArea6SpriteQuad`, `8011d1bc drawArea6SpriteStrip`, `8011d4e0
  drawArea6RotatedSpriteStrip`, `8011dbf4 drawArea6ScaledRotatedSpriteStrip` (from their header notes)
- x006 (Dirt Motocross) race HUD, drawn by MAIN0 drawGameplayScene through `8011bc34 drawRaceHudIfActive` (D_800B1410 timer and flag) ->
  `8011bcac drawRaceHud`: `8011bd40 drawRaceHudBestTime` (label sprites + m'ss"cc of GCD.scfe2 at 0x10,0x10), `8011c4ac drawRaceHudLapTime` (the same
  layout for D_8009CFD0 at 0x10,0x1A; which time is which is a guess), `8011cc18 drawRaceHudGauge` (blinking icon + a yellow bar of width g->wd0 out of
  0x80 over a grey one, g = GVARS.p4c), `8011cd78 drawRaceHudLives` (one or two digits of GAME.playerLives - 1 under the icon); `80118304
  spawnRaceMessage` (ObjAlloc layer 3 type 0x56 objects from the letter table D_8011F3CC[sub]) -> `80117ddc updateRaceMessageSprite` (per-subtype timers
  0x32 / 0x78; uncertain), `8011ba3c updateItemPickup` (layer 5 type 0x10, the same as x009's), `8011ebd0 cullObjectWideWindow` (visible within depth
  +-0x190, x -0xC8..+0x2BC, y window 0x231 around the camera, then pushes the object on its category list)
- x003 (Phoenix Mountain) objects: `80124ffc updateSpringPad` (layer 2 type 0x1D, 22 placements: plays its bounce anim with sfx 0x73 when b69 is set)
  with `8011fd20 springPadContact` (landing on it from above (contact result 3): pad vibration, player b04 1 step 0x33 with velY -0x600 (subtype 0)
  or -0x980) -> `8011e358 playerStepSpringBounce` (the x003 code at the player step 0x33 address of MAIN0 playerNormalModeDispatch: anim 0x39,
  sfx 0x23/0x24, flies up until the head hits or it falls); `8011ce24 updatePlayerLauncher` (layer 4 type 0x2E, kind uncertain: SPR_DATA[6],
  allocates a layer 3 type 0x38 part below itself; b0c & 8 starts it in a scripted state when GV2.bc2) whose `8011c050 playerLauncherThrow` (sfx 0x72)
  puts the player in step 0x34 with velX +-0x400 -> `8011e6b0 playerStepLaunched`; `801202e4 playerLauncherObjectCollision` (FUN_80046610 type 0x2E)
- x003 thrower enemy (layer 2 type 0x38, 13 placements, kind uncertain; SPR_DATA[3], anim table D_8013A698): `8012f524 updateWingedForkPig`,
  `8012e6b0 wingedForkPigThrow` (subtype 1 state 3: spawns layer 2 type 0x39 shots), `8012f9a0 updateWingedForkPigShot` (type 0x39: semi-transparent,
  speed 0x80 (subtype 0) or 0x480), `8011f508 wingedForkPigShotContact` (2 points of damage), `8011f634 wingedForkPigContact`; b04 2 steps:
  `8012eb2c wingedForkPigHit` (subtype 1 step 0), `8012f234 wingedForkPigGrabbed` / `8012ef3c wingedForkPigGrabbedB` (step 1 of subtype 0 / 1,
  spawnItemNotification(1, 6)), `8012f3b0 wingedForkPigDefeated`
- x003 `801281fc updateSpikyLizard` (layer 2 type 0x28, 12 placements; "horned" only from the badly composed strips, uncertain): b04 2 step 0
  `8012748c spikyLizardHit`, step 1 `80127d48 spikyLizardGrabbed` (spawnItemNotification(2, 6), counts the shakes in wb6), step 2 `80128060
  spikyLizardDefeated`; `80120140 spikyLizardContact` (FUN_80047484 type 0x28), `801256f4 spikyLizardCheckWall` (wall / floor probe 0x18 ahead
  along the slope d8c)
- x001 `80134c00 updateSegmentedCrawler` (layer 2 type 0x13, spawned (not placed), kind uncertain: `80134ee8 initSegmentedCrawler` allocates 3 or 5 more
  layer 2 objects (needs that many free, else frees itself and decrements the spawner count GVARS.p48[5]) and links them as body segments (b0c != 0,
  d90/d94) that `80133a24 segmentedCrawlerFollowParent` keeps behind the head at the angle d84; the head moves with `80132b64 segmentedCrawlerMove`;
  freed when GV2.bc3 & 1); b04 2 = `80134944 stepSegmentedCrawlerHurt`: step 0 `80133ba4 segmentedCrawlerHit`, 1 `80134070 segmentedCrawlerGrabbed`,
  2 `801344fc segmentedCrawlerDefeated` (fly-off), 3/4 free the whole chain; `80126564 segmentedCrawlerContact` (FUN_80047484 type 0x13)
- x010 talk helpers: `80125e00 selectTenThousandYearElderTalkStep` (talk step from event 0x7E and the ten math bead flags GCD.bd0e7[0..9], events
  0x40/0x74/0x7F and the five golden item flags), `801271d8 swimTeacherFloatPlayer` (bobs the player on a sine around d34 during the swim lesson,
  sfx 0x0E), `8012c6ec parrotPushBackAndSelectTalk` (pushes the player back (b04 6 step 5, velX +-0xC0), then picks the parrot's talk step from
  events 0x3F / 0x3E / 0x7F; uncertain)
- x010 zipline (uncertain kind, from the code): `80118a78 updateZiplinePulley` (layer 4 type 0x1D, 18 placements: when the player hangs on it (b6a)
  `801186e0 ziplinePulleySlide` accelerates it diagonally with a per-b0c velocity ratio (b0c 8: 0x180/0x400) that matches the slope of the cable quads
  of `80119f58 ziplineCableInit` (cable 4: dx 0x149, dy 0x76); b0c 8 arriving at x >= 0xAC9 sets GVARS.b58 += 1 and locks the scroll, then it resets);
  `8011a0c0 updateZiplineParts` (layer 4 type 0x35, 42 placements, subtype table D_8012F298: 0 `80119dd4 wobbleWhenStoodOn` (toggles the frame while
  b69), 2 `8011a1e0 followParentWithOffset`, 3 ziplineCableInit); `80118e2c updateSinkingLedge` / `80118cd4 sinkingLedgeCheckPlayer` (uncertain:
  sinks 0x30 when the player stands at depth 0x5A in an x window, or when GVARS.b58 == 1; sfx 0xA3); `8012b6e8 updateSpearPigDroppedSpear` (layer 3
  type 0x48, allocated by updateSpearPig when its hit points reach 0: flies up and falls rocking)
- Layer 8 background models with texture-UV animation (`updateUvAnimBgModel`: x004 80119114 (type 3), x010 80117364 (type 8); the u/v of the model's
  FT3/FT4/GT3/GT4 faces flip by +-0x30/0x60 every 10 ticks, frozen while GVARS.b38): x004 `8011942c uvAnimBgModelFlip`, `80119648 uvAnimBgModelStepB`,
  `80119a04 uvAnimBgModelStepC`; x010 `8011752c uvAnimBgModelFlip`
- Backdrop sprites (camera-relative screen sprites, named like x001's): `updateParallaxBackdropSprite` (layer 7, position scaled >> 1..3 by section)
  x006 8011b8a4, x009 8011d17c, x010 8011afd8 (subtype 99 hidden once GAME.purifiedAreas & 0x40), x014 8011b03c; x010 `8011b1f8
  updateParallaxBackdropSpriteB`; `updateBackdropSprite` (layer 3, unscaled) x002 801167bc, x006 80117468, x009 801198dc, `80119bac updateBackdropSpriteB`;
  x010 `80118188 updateScreenDecorSprite` (subtype 2 sways with MulNegSin, uncertain); `updateAnimDecorSprite` (culled looping animated sprite)
  x002 801165dc, x010 801180b4, x014 801175a0; x009 `8011976c updateScenerySprite` (scale/rotation from the box0 nibbles as x001's); x009 `80118838
  updateOneShotEffect` (frees itself when the anim ends); `updateSceneryModel` (culled static model) x006 8011aee8, x010 8011af20; x014 `80119610
  updateStaticSolid`
- x014 evil pig rooms (uncertain kinds): `8011a5fc updateBossRoomPlatform` (layer 4 type 0x3A, one per section: rotation/orbit mode by section, section 2
  orbits GVARS.p54), `8011a9e0 updateBossRoomVortex` (type 0x3C, section 2: stores itself in GVARS.p54, spins and pushes the player through
  GVARS.s44/s46 within 0xA0), `80119344 updateRotatingPlatformCross` (type 0x2A: spawns four subtype 1 children that follow its rotation matrix),
  `8011ac9c updateBossRoomPillar` (type 0x3D, box height per b0c), `8011ad78 / 8011aedc updateRotatedChildPart / B` (follow the parent d90 matrix),
  `80117114 updateRisingEffect` (layer 3 type 0x23: subtypes 1-3 rise for a random time and vanish)
- x009 `8011a4a8 updateWaypointRide` (layer 3 type 0x40, section 6: hides the player (active 5), `80119f5c waypointRideStep` carries it along the 3D
  waypoint path D_8012AC88 turning the camera, then warps to area 9 section 0 spawn 2; vehicle uncertain), `8011a7e0 updateBluePowderSprinkle`
  (layer 3 type 0x42: item 0x75 "Blue Powder" used next to the parent -> after 30 ticks sets the parent to step 8, subtype |= 0x80; uncertain)
- x002 `80116928 sevenFriendsMemberSetAnim` (table D_8011C6DC; callers stepSevenFriendsMember and the pig gate scenes), `8011836c initSevenFriendsNpc`
  (box/w1e/SPR/anim from D_8011C770[subtype]), `80118ff0 sevenFriendsWaitGateOpen` (step helper: waits for GCD.bcfea == 2; unreferenced),
  `80119f40 spawnSparkleNear` (layer 3 type 0x31 subtype 1 at an offset from D_80115FF4; uncertain)
- x013 ending stats screen (run by updateSceneSequence with the values it computes): `801179b4 drawGameStats` (AP, play time, events started (flag != 0),
  events completed (flag 0xFF)), `80117b4c drawStatsLabelGlyph`, `80117c10 drawStatsNumber`, `80117e14 drawStatsPlayTime` (h:mm, glyph 10 = ':'),
  `80118028 drawStatsDigit`; `linkAreaModelBlock` (x013 801166e0, x018 8011657c section 1 only) - entries 13 / 18 of the MAIN0 per-area table
  PTR_DAT_80079b3c that FUN_8002af10 calls with the WMD data: points a->d4 at the block b + b->d4 and sets a->b3 (uncertain)
- x006 motocross bike (MAIN0 func_80033F50 calls `8011a87c updateMotocrossBike` in AREA06_DIRTMOTOCROSS; stores itself in GVARS.p4c, b0c 0 = body
  (allocates the rider type 0x3B -> GVARS.p50), 1..5 = parts following its matrix, wheels turning by velH): `801198f0 motocrossBikeRide` (body steps:
  follows the path D_8011F3F8, jump on the button), `801183f0 motocrossFindPathSegment` / `801184f0 motocrossAdvanceAlongPath` (walk the path points
  by distance, heading/pitch with ratan2), `80118810 motocrossBikeSetSpeed` (speed by GVARS.b58 / input; uncertain), `8011a73c motocrossBikeBob`
  (sine bob + jump/land morphs; uncertain)
- x004 `8011c890 updateHangingRope` (layer 4 type 0x37: strip shows a knotted hanging rope, 0xA0 x 0xC4 box), `8012bf58 updateAnimDecorSprite`
  (layer 2 type 0x36); x003 `8011910c updateAnimDecorSprite` (layer 3 type 0x35); x009 `8011b8e0 updateSwingingPendulum` (layer 4 type 0x2C, uncertain
  kind: subtype 0 swings d8c from a growing phase, subtype 1 hangs 0x4C from its parent at the parent's angle)
- x001 area player steps (the x001 copies of the playerNormalModeDispatch targets, mostly uncertain): `801237e4 playerStepSlopeSlideB` (step 0x1E,
  slides with playerSlopeSlideUpdateAnim while the contact bits +0xBE hold), `80121b58 playerStepContactStop` (step 0x1F: sfx 0x3E + pad vibration,
  slows to a stop; `80121a10 playerContactStopCheckJump` its jump/leave check), `80121790 playerStepRideSlopeContact` (step 0x21: falls along the
  slope angle while +0xBE is set; uncertain), `80122410 playerStepClingRotating` (step 0x22: clings to the rotating D_8009F0EC object;
  `80122214 playerClingPlaceOnRotation` places the player on its angle, `801222c4 playerClingJumpRelease` jump button lets go and releases the chain),
  `80121d6c playerStepRiseToLedge` (step 0x28: moves up until probeFlaggedSegmentA bit 2, then step 0x16), `80122044 playerStepSinkToGround` (step
  0x29: sinks 1 px per frame until a tile), `80122f54 playerStepRideSeesaw` (playerStep2EByArea area 1: stands on the tilted seesaw plank), `80122cbc
  seesawPlankSwingPivot` (playerSwingUpdatePivot type 0x14 callback), `801213d0 playerBittenJumpCrawler` (playerStepBittenJump bitten type 0x13)
- x001 contact callbacks: MAIN0 FUN_80045f00 (weapon/attack object vs layer 2 by the enemy type) `80125f04 purpleCrawlerWeaponHit` (0),
  `80126004 rollingBarrelWeaponHit` (0x11), `80125b88 segmentedCrawlerWeaponHit` (0x13); MAIN0 func_8004BBC0 (player grab probe vs layer 4, sets
  the contact kind +0x9E) `80126fe4 springVineGrabProbe` (5), `80126f64 flowerTowerBlockerGrabProbe` (0xB), `80126e8c bendingBranchGrabProbe` (0xE),
  `8012767c seesawPlankGrabProbe` (0x14; `80127074 seesawPlankProbeFace` tests one face), `80126d2c swingingRopeGrabProbe` (0x15); `8012f988
  updateRollingBoulder` (layer 4 type 0x23 body, via the 8012f968 thunk: rolls down the slope, sets an area flag and turns into type 0xC; uncertain kind)
  with `80126cc4 rollingBoulderPlayerCollision`, `80125fbc rollingBoulderObjectCollision`; `80124410 grabObjectContact` (FUN_80047484 type 0x1F in
  area 1: holds the touching object; uncertain); `80127700 landOnSlopedBox` (stand-on test for a sloped box; uncertain)
- x001 purple crawler steps 4..8: `80129b2c purpleCrawlerFall`, `80129dac purpleCrawlerRun` (random movetab/timer), `8012a290 purpleCrawlerCarryPlayer` (holds the player
  and jumps; uncertain), `8012a5cc purpleCrawlerDrop`, `8012a794 purpleCrawlerCrawlWall` (uncertain); `80128110 purpleCrawlerChooseStep` (next step from the distance
  to home/player); `80123b30 updateFallingSparkleItem` (item that falls leaving type 0x31 sparkles; uncertain), `80123e50 spawnItemEffectFlyingItem`
  (applyItemEffect case 6: layer 5 type 6 subtype 0x13 with velH)
- x001 race rival (layer 2 type 0x0D, the opponent of the race event GCD.bce59; kind uncertain): `801312f8 updateRaceRival` with the steps
  `80130298 raceRivalWaitStart`, `80130420 raceRivalRun`, `801305d0 raceRivalRunCourse`, `80130dc4 raceRivalTurn`, `80130f4c raceRivalFall`,
  `8013109c raceRivalFinish` (sets GCD.bcedf) and, for subtype 0, `8012ff4c raceRivalIdleWander`, `80130114 raceRivalHopAway` (step names uncertain)
- x001 misc: `80131904 updateAnimProp` (layer 2 type 0x10, 2 placements: a culled looping animation; uncertain kind), `80132698
  spawnRollingBarrelDebris` (rollingBarrelWeaponHit: 7 type 0x21 pieces) -> `80132760 updateBarrelDebrisPiece` (uncertain), segmented crawler
  `80132964 segmentedCrawlerCheckWall`, `80132a3c segmentedCrawlerWiggle` (head angle sway), `801334dc segmentedCrawlerWander` (uncertain),
  `80134674 segmentedCrawlerHurtFlyOff` (stepSegmentedCrawlerHurt: the head and body pieces fly off; uncertain), `801319c8 updateParentFollowModel` (layer 8
  model that copies the d90 parent's position/angle; uncertain), `801368d4 spawnCaughtForestDwarf` / `801374c0 spawnHeldPuppyBaron` (playerInitAreaHook:
  type 0x16 / 0x17 objects already in the caught step 3), `801369f8 initPuppyRumorNpc` (while GCD.bceab is 1..2), `8013a1f4
  checkPuppyRumorNpcTalk`, `80136aac playPuppyBarkNearby` (random sfx 0x41 near x 0x8AE..0xAAE, louder higher up), `80139a80 spawnLostSeedKidDwarf`
  (type 0x16 at 0x105B,-0x506 while GCD.bcdb2 < 2; uncertain), `80138550 stepCeilingDripDrop` (updateCeilingDrip subtype 0), `80138c4c
  updateOneShotAnimEffect` (uncertain), `80138e7c brokenFountainPoseA` / `80139174 brokenFountainPoseB` (brokenFountainScene step 6 by b0c;
  uncertain), `8013bda0 updateLeafButterflySwarm` (runs while event 0xB2 Leaf Butterflies (GCD.bce56) is open: `8013b344
  spawnLeafButterflySwarm`, `8013b51c stepLeafButterflySwarm`, `8013b504 isLeafButterflyEventOpen`; uncertain), `8013be48 updateKomaPigSpawner`
  (type 2 objects near x 1000..0x885 while GCD.bcdb5), `8013c028 updateSegmentedCrawlerSpawner` (type 0x13, at most 2)
- x001 more: koma pig b04 1 states of this overlay's layout `8012c8ec komaPigIdle` (state 0), `8012c0ec komaPigMove` (states 1..8), `8012bd6c
  komaPigLeapAtPlayer` (state 9), `8012eae8 komaPigDropIn` (uncertain), `8012bc84 komaPigCheckWall`; overlap tests `8012614c purpleCrawlerOverlapTest`
  (purpleCrawlerContact), `80124c20 swingingRopeOverlapTest` (per-height x offsets D_8013C6A4), `80124e34 ropeChainsOverlapTest`; rope chains
  (updateRopeChains, links in D_800A4470): `8011cec8 ropeChainsComputeLinkPos`, `8011d28c ropeChainsRaiseLower` (sfx 0x4B/0x4C), `8011d264
  getRopeChainLinkAngle` -> `80117bd4 updateRopeChainLinkModel` (layer 8, 3/4 of the link angle; uncertain); `80120e24 updateBackdropSpritePos`
  (updateBackdropSprite: scroll/parallax by b0c); `8011bc6c mechanismRotateWhenPlayerNear` (updateAreaMechanisms subtype 1; uncertain);
  `80118998 spawnBreakableBlockDebris` (32 type 0xE pieces) -> `80118724 updateBlockDebris` (layer 3 type 0xE; `8011886c blockDebrisFly`);
  `801191e4 spawnSpringVineLeafBurst` (type 0x1F) -> `80118da8 updateLeafBurst` (uncertain); `801193e4 updateRisingParticles` / `801196e0
  initRisingParticle` (type 0x3D; uncertain), `8011935c spawnType2DEffect`, `80119268 updateOneShotSpriteEffect`, `80119eec updateAmbientCritter`
  (layer 3 type 0x57, 18 placements; animation by b0c, uncertain kind), `8011a72c springVineSwayStep` (springVineGrabbed: decaying sway),
  `801200ec updateEventHitSwitch` (layer 4 type 0x2F, 14 placements: subtype 0 hit (b68) starts event 0x18 and spawns a type 0x3D effect, subtypes
  1..3 move with GCD.bcdf1; uncertain kind)
- x003 player launcher steps (updatePlayerLauncher b04 1: `8011ca70 stepPlayerLauncher`, subtype & 4 -> `8011cc8c stepPlayerLauncherScripted`;
  step names uncertain): 0 `8011b6f4 playerLauncherWait` (until the player is near), 1 `8011b870 playerLauncherAim` (follows the player; `8011b460
  playerLauncherCheckCatch`), 2 `8011ba34 playerLauncherGrab` (lunges at velV 0xE00; `8011b324 playerLauncherCheckReach`), 3 `8011bcc8
  playerLauncherSwing`, 4 playerLauncherThrow, 6 `8011c3d8 playerLauncherPullPlayer`, scripted step 5/7 `8011c7fc playerLauncherScriptedPull`
  (GV2.bc2 == 2); `8011b588 playerLauncherTurnToward` (turns d38 toward the player / a fixed side / a target angle)
- x003 thrower enemy b04 1 steps (uncertain): `8012d740 wingedForkPigWaitAppear` (random delay while on screen, picks the subtype), subtype 0:
  `8012d914 wingedForkPigRise`, `8012da68 wingedForkPigFlyAcross` (spawns type 0x43); subtype 1: `8012dcac wingedForkPigRiseB`, `8012ddd4
  wingedForkPigWander`, `8012e378 wingedForkPigChase`; `8012d518 wingedForkPigCheckCollide`, `8012d66c wingedForkPigCheckWall`
- x003 spiky lizard b04 1 states (uncertain): `80125834 spikyLizardLand`, `801259ac spikyLizardIdle`, `80125cf8 spikyLizardWalk`, `8012632c
  spikyLizardRun`, `80126b68 spikyLizardTurnToPlayer`; x003 koma pig subtype entries (D_80135C64; uncertain): `80123b20 komaPigStandPose` (anim
  by b0c), `80123c60 komaPigWaitForPlayer`, `801241c0 komaPigWaitForPlayerB`, `80123ea8 komaPigSceneEntry`; `801196f8
  updateTranslucentAnimSprite` (layer 3 type 0x36, ~50 placements: semi-transparent animated sprite, drifting by section through `801191c4
  translucentSpriteDrift` / `801193d0 translucentSpriteDriftB`; uncertain kind)
- x003 callbacks: func_8004BBC0 grab probes `8011f474 knockDownObjectGrabProbe` (0x22), `8011f38c hitWobbleObjectGrabProbe` (0x24); FUN_80045f00
  `8011ff64 spikyLizardWeaponHit` (0x28), `8011f7ec wingedForkPigWeaponHit` (0x38), `8011fa6c komaPigIntroWeaponHit` (FUN_80044d54 for the area 3
  subtype 4 koma pig: removed once GCD.bcdc3); `8011fbb8 areaMechanismsPlayerCollision` (func_80046D58 type 7 in area 3 by subtype), `8011fec8
  areaInitAObjectCollision` (FUN_80046610 type 4 outside area 0; uncertain)
- `updateAreaCameraHeight` (x001 801165c0, x003 80116f74) - MAIN0 FUN_80029078 per-area tail after loadSectionHeight (area 0 X000 80116758):
  camera height offset +0x32 by section and the camera x (piecewise linear ramps)
- x003 more: `8011e1a4 playerStepDizzy` (playerStep3CByArea area 3: anim 0x42, sfx 0x20, 0x78 + 0x3C frames; uncertain), `8011e2d0
  playerFallIfHeadHit`, `8011eae8 playerStepTumble` (step 0x36: anim 0x37 then 0x10 with a turning d8c; uncertain), `8011d07c
  updatePlayerLauncherPart` (the layer 3 type 0x38 part: anim from the launcher's angle), `8012fbc4 updateWingedForkPigPuff` (one-shot anim;
  uncertain), `8012a6d8 initMasterOfTheSkiesEventObj` (box/sprites per subtype from D_80135DB8, anim by GCD.bcdc7), `8012c97c windblownMonkeyIdle`
  (step 0; uncertain), `80131e44 soakedPhoenixGuideIntroScene` (section 0 step 0: locks the player at x 0x1B8), `80132960
  soakedPhoenixGuideSection1Scene` (section 1, GCD.bd00a 1 -> 2), `80130990 updateWindPush` (pushes the player with GVARS.s44 in sections 0/1
  until GAME.purifiedAreas & 2), spawners `80130e3c updatePlayerLauncherSpawner` (layer 4 type 0x2E per section), `8013123c
  updateSpikyLizardSpawner` (type 0x28), `801315b0 updateKomaPigSpawner` (type 2)
- x004 black crow perched states (blackCrowPerched state switch; uncertain): 0 `80125420 blackCrowPerchIdle`, 1 `80125554 blackCrowPerchLook`, 2
  `801256a8 blackCrowPerchCaw` (sfx 0x13), 3 `80125818 blackCrowFlyAround`, 4 `80126464 blackCrowWatchPlayer` (entered when the player is within
  +-0x80 on the same depth); callbacks `8011f844 blackCrowWeaponHit` (FUN_80045f00 type 0x1E), `8011f978 countedCollectibleWeaponHit` (0x29),
  `8011f5d8 shuttlePlatformGrabProbe` (func_8004BBC0 type 0x1F), `8011eec0 areaInitAPlayerCollision` (func_80046D58 type 4)
- x004 big key painting (layer 2 type 0x32 objects; the five pieces GCD.bcf24[0..4]): `8012eca0 spawnBigKeyPaintingPieces` (the painting plus
  the collected pieces), `8012eb78 spawnBigKeyPiece` (entry i of D_8013145C, placed or flying from the player), `8012ee14
  bigKeyPaintingCheckPiece` (uncertain), `8012f274 stepBigKeyPainting` (-> paintingBigKeyReward once all five are set), `8012e950
  spawnHauntedPaintingObj` / `8012ea30 updateHauntedPaintingSpawner` (type 0x32 from D_8013144C while GCD.bcf2d == 0; uncertain), `8012f69c
  updateHauntedPigBagEvent` (runs hauntedPigBagEventStart until GV2.bc3 & 8)
- x004 more: `8011e778 drawArea4Kind4Sprites` (drawCategory7Objects render kind 4 in area 4: semi-transparent SPRT list; uncertain),
  `8011d9a4 playerStepDizzy` (step 0x3B: anim 0x41, sfx 0x25; uncertain), `8011e154 updateArea4ItemObj` (MAIN0 dispatchAreaItemInit area 4;
  uncertain), `8012a740 initTownHallWorker` (box/sprites per subtype from D_801311C8 while GCD.bcf2d == 0), `80129e14
  paintingSlotHintNpcMoveTo` (moves toward velX/velY targets; uncertain)
- x009 jellyfish steps (updateJellyfish; uncertain): b04 1 step 0 `80124e04 jellyfishFloat`, step 2 `80125438 jellyfishSwimB` (sfx 0x97), step 3 of
  a child (b0c) `801260cc jellyfishChildFollow`, step 4 `801261c0 jellyfishSink`, step 5 `80126460 jellyfishGrabPlayer`; b04 2 step 2 `80126ce0
  jellyfishHurtFall`, step 4 `80127290 jellyfishGrabbedB`; `80120ba0 jellyfishWeaponHit` (FUN_80045f00 type 0x21), `80121254 jellyfishGrabProbe`
  (func_8004BBC0 enemy list type 0x21)
- x009 red flower b04 1 states (updateRedFlower; uncertain): 0 `80121a58 redFlowerBud`, 1 `80121d2c redFlowerLand`, 2 `80121e5c redFlowerHop`, 3
  `801220cc redFlowerTurnToPlayer`, 4 `801225bc redFlowerWalk`, 5 `801229ac redFlowerWalkB`, 6 `80122f08 redFlowerChase`, 7 `80123294
  redFlowerFall`; `80121964 redFlowerCheckEdge`, `801224ac redFlowerChooseState`; `80120a2c redFlowerWeaponHit` (FUN_80045f00 type 0x0A)
- x009 flower platform / plane: `801212c4 flowerPlatformGrabProbe` (func_8004BBC0 type 0x1C), `80121934 flowerPlatformPushOut` (func_80048E68
  type 0x1C), `8012030c areaMechanismsPlayerCollision` (func_80046D58 type 7 subtype 9); `8011759c updateRotatingPlane` (layer 8 model whose
  children follow its rotation; `80117d48 spawnRotatingPlaneObjects` per subtype -> `80117ec0 spawnRotatingPlaneChild` (layer 2 type 0x1C from
  D_8012A878), `801178b0 rotatingPlanePlaySfx`; uncertain)
- x010 spear pig b04 1 steps (updateSpearPig; uncertain): subtype 0 `80128d78 spearPigWatch` / `80129f7c spearPigJumpAttack` (velV -0x900),
  subtype 1 `80129048 spearPigInitB` / `801291c4 spearPigWanderB` / `801297d8 spearPigChargeB`, subtype 3 `80128764 spearPigActC`; b04 2 step 0
  `8012a664 spearPigHitFall`; `80120564 spearPigWeaponHit` (FUN_80045f00 type 0x3A), `8011ddc0 flyingBugWeaponHit` (0x27), `8012418c
  flyingBugAccelerate` (flyingBugWander speed ramps by subtype)
- x010 layer 4: `80118fe4 updateTallPillar` (type 0x31, 4 placements: 0x20 x 0x1B0 box, culled; kind uncertain) with `801210e4
  tallPillarPlayerCollision`, `801212d4 tallPillarGrabProbe`, `801213f0 tallPillarObjectCollision`; `8011df8c ziplinePulleyGrabProbe` (func_8004BBC0
  0x1D); `80119374 rotatingWheelSpinStep`, `80119648 turningPlankTurnStep`, `801198f8 turningPlankPurifiedStep` (the GV2.bc3 & 0x40 branch)
- x010 player: `8011ca00 turningPlankSwingPivot` (playerSwingUpdatePivot type 0x34), `8011ccd8 playerStepRideTurningPlank` (playerStep2EByArea area
  10), `8011beac playerStepClingRotating` (step 0x46, like x001's step 0x22; `8011bd5c playerClingJumpRelease`); `8012e9e0 updateWaterCurrent`
  (GVARS.s44/s46 push, level by section / GAME.purifiedAreas & 0x40; uncertain)
- x014 section controller / room code: `801161d4 sectionClearCameraFollow` / `801168c0 sectionClearCameraReturn` (camera sub-handlers of the
  updateSectionClearController table D_8012568C; uncertain roles), `80116a10 animateEvilPigRoomPalettes` (area 14 entry of the MAIN0 per-area script
  op table data_80079B3C: section 6 flashes the CLUT rows at 0x120,0x1E0 by the D_80125978 timings, section 3 cycles 0x130,0x1F0 (lava glow, uncertain)),
  `80116900 loadEvilPigRoomClutRow` (LoadImage of one 16-colour row at 0x130,0x1E0+n), `8011709c spawnRisingEffect` (layer 3 type 0x23 ->
  updateRisingEffect), `801172dc updateHauntedMansionPigShotTrail` (layer 3 type 0x25, spawned by the haunted mansion shot: slides 0x28 ticks; uncertain),
  `80117710 updateBossRoomDecorSprite` (layer 3 type 0x51) / `801177c4 updateBossRoomDecorSpriteB` (type 0x52, anim per subtype) (uncertain kinds)
- x014 evil pig effect (type 0x61, updateEvilPigEffect) per subtype: `80117aa8 evilPigEffectSubtype0Step` (follows the parent d90),
  `80117d94 evilPigEffectSubtype1Step`, `80118048 evilPigEffectSubtype2Step`, `80118390 evilPigEffectSubtype4Step`, `80118544 evilPigEffectSubtype6Step`
  (falls from the clear effect); `80118314 spawnEvilPigEffectEmitter` (subtype 3, emits subtype 4), `801178cc spawnBossCaptureEffectRing` (12 subtype 0
  objects around the capturing platform); MAIN0 drawCategory3Objects render kinds 20 / 21: `801189ec drawEvilPigEffectKind20` (triangles, rtpt),
  `80118ddc drawEvilPigEffectKind21` (quads)
- x014 boss room platform (updateBossRoomPlatform): `80119b58 bossRoomPlatformCaptureBoss` (once the defeated boss GVARS.p48 is in front and in reach
  (`80119a4c bossCaptureBossInReach`) it pulls it in (`80119914 bossCapturePullBoss`), spins bouncing off the screen edges (`801196a4
  bossCaptureBounceDirection`, sfx 7), flies to the room centre, sets the GV2.bc3 bit of the section and the clear event; the platform may be the
  evil pig bag, uncertain), `8011a498 bossRoomPlatformDescend` (sections 1/5/7: moves down to the D_80125C90 height; uncertain);
  updateRotatingPlatformCross steps `801190d0 rotatingPlatformCrossHubStep` (stores itself in GVARS.p54, spins and reverses), `8011920c
  rotatingPlatformCrossArmStep` (arms tilt d84 0 <-> 0x800 when the hub stops)
- x014 player steps: `8011b170 playerStepVortexLaunch` (playerNormalModeDispatch step 0x47: thrown up, pushed by GVARS.s44/s46 until landing;
  uncertain), `8011b47c playerStep48Area14` (playerStep48ByArea area 14 handler)
- x014 collision callbacks (MAIN0 dispatchers): `8011d2dc evilPigBossWeaponHit` (collideWeaponsWithCat2Objects types 0x3B..0x42), `8011cba0
  flowerForestPigShotWeaponHit` (type 0x47); layer 4 types 0x42 / 0x43 (updateRotatedChildPart / B): `8011d370 rotatedChildPartPlayerCollision`,
  `8011c99c rotatedChildPartBPlayerCollision` (grabs the player), `8011d3f8 rotatedChildPartGrabProbe` (playerProbeGrab 0x42), `8011d4fc
  rotatedChildPartEnemyCollision` (collideCat2WithCat4Objects 66/67); `8011d8f8 evilPigPointOnScreen` (unreferenced, uncertain)
- x014 pig shot helpers (uncertain step names): baccus village `801211f0 baccusVillagePigShotFall` (subtype 0: falls from the top of the screen
  spinning), `80120da0 baccusVillagePigShotCheckLanded` (floor probe, debris, spawns the pieces), `80120c54 spawnBaccusVillagePigShotPieces` (3 type
  0x46 objects; inlined copy), `801213f4 baccusVillagePigShotPieceStep`; flower forest `8012194c spawnFlowerForestPigShotPair`, `80121d08
  flowerForestPigShotScatter` / `8012185c spawnFlowerForestPigShotPieces` (6 type 0x47 pieces), `80121f90 flowerForestPigShotSlide`; haunted mansion
  `8012284c hauntedMansionPigShotPickPoint` (random on-screen point of 16), `80122aac hauntedMansionPigShotAttack`; lava caves `80123d64
  lavaCavesPigShotDrop`, `80123aac lavaCavesPigShotFragmentArc`, `80123730 spawnLavaCavesPigShotFragments`; deep jungle `80124ad0
  deepJunglePigShotHop`, `8012491c deepJunglePigShotCheckWall`, `801249d0 deepJunglePigShotPickHop`, `80124f28 deepJunglePigShotSubtype2Step`
- x009 rotating plane (uncertain step roles): `801177f8 rotatingPlaneSway`, steps of rotatingPlanePlaySfx (which is really the lift step switch on
  GV2.bb1 / b6a) `8011792c rotatingPlaneLevelStep`, `80117a9c rotatingPlaneRaisedStep`, `80117bf0 rotatingPlaneLoweredStep`; `801174c4
  followParentRotation` (child position from the parent d90 matrix), `80117464 objSetRotMatrixFromAngles`, `80117fe4 spawnRotatingPlaneChildAt`
  (unreferenced); `80116bd0 scrollFollowRotatingPlane` (dispatchAreaScroll camera hook); `801191b0 updateRotatingPlaneSprite` (layer 3 type 0x27,
  follows the plane; `80118a80 initRotatingPlaneSprite` / `80118bf8 initRotatingPlaneSpriteB` (animFrame & 0x80), `8011934c rotatingPlaneSpriteStep`
  (anim by the linked switch b6a), `80118f7c rotatingPlaneSpriteBStep` (GCD.bd07e, removes an item), `80118e00 rotatingPlaneSpriteSpawnPetals` (two
  layer 2 type 0x2C), `80118970 rotatingPlaneSpriteSetQuad`); `801288b8 updateDriftingPetal` (layer 2 type 0x2C, kind uncertain)
- x009 others: `80116e4c animateArea9Palettes` (area 9 entry of MAIN0 data_80079B3C), `80118440 spawnApAwardNumber` (awardAPByArea: digit sprite of the
  AP amount; same code at 80118440 in x001/x003/x004/x011/x018), `80118910 spawnArea9OneShotEffect` (playerInitMode: layer 3 type 0x26 at a fixed
  point), `80119d2c waypointRideAdvanceSegment` / `80119e4c waypointRidePlace`, `8011a6a0 bluePowderFindRedFlower` (red flower type 0x0A near the
  player), `8011bfd8 apBoxOpenStep` (updateApBox, spawns type 0x44 objects; uncertain), `8011ecc8 deathFruitFindFlowerPlatform`, `80124c70
  redPlusBlueObjectRespawn` (hidden 0x4B0 ticks, then rises back), `80129b70 updatePlayerWalkInTrigger` (unlayered variant 8: `80129acc
  checkPlayerWalkInTrigger`, `80129914 stepPlayerWalkInScene` walks the locked player to x 0x1068 then into the depth; uncertain),
  `801217f8 jellyfishCollideObjects` (collideCat2ObjectsPairwise type 0x21)
- x009 player: `8011d3ec playerBittenJumpType1A` (playerStepBittenJump type 26), playerNormalModeDispatch steps `8011d7e0 playerStep24Area9`
  (with `8011d680 playerStep24PlaceOnCircle`), `8011e688 playerStep25Area9`, `8011de64 playerStep26Area9`, `8011e0d8 playerStep27Area9` (all ride
  D_8009F0EC; roles uncertain); collision-column wrappers `80121434 columnPushOutOfWall`, `801214d0 columnProbeA`, `80121540 columnProbeB`
- x009 section 6 model drawing (drawAreaModelList): `80120280 drawArea9SectionModel` -> `8011f96c drawArea9ModelFT3`, `8011fb4c drawArea9ModelFT4`,
  `8011fdc4 drawArea9ModelGT3`, `8011ffe0 drawArea9ModelGT4`
- x004 (mostly structural names, roles uncertain): `80116934 cameraModeHandler1` (camera mode 1 slot of the MAIN0 per-area camera mode tables,
  areas 4/12; `80116764 cameraModeHandler1Step`), `80116ae0 checkScrollItemDoorA` / `80116c0c checkScrollItemDoorB` (updateAreaScroll: at fixed
  player positions tryStartItemInteraction(o, 1, 7 / 0) and lock the game), `80118bfc area4ScriptOp` (areas 4 and 12 entry of MAIN0 data_80079B3C;
  `80118720 area4ScriptOpSetQuad`, `801188f0 area4ScriptOpPointInside`), `8011a448 spawnApAwardNumber` (awardAPByArea), `8011aba0
  updateL3Type2BObject`, `8011ad24 updateL3Type3CObject`, `80125248 updateDeferredLayer1Type2Spawn` (spawns a layer 1 type 2 object), `80127ec4
  updateHomingCarrierFlyer` (`80127b14 homingCarrierFlyerChase`, `80127d54 homingCarrierFlyerReturn`); updateAreaMechanisms subtypes `8011b218 stepAreaSwitchB` (subtypes
  8/10/0xC: player box + da0+2 like stepAreaSwitch), `8011b138 stepAreaRotatingPart` (17/19/21: copies the parent state, turns d88 by 0x600),
  `8011b438 stepAreaEventDoor` (moves once D_800A4555 reaches the subtype's values); `8011dc0c playerStep3AArea4` (playerNormalModeDispatch step
  0x3A); D_801310EC subtype table of updateKomaPig: `801231d0 komaPigSubtype0Step`, `80123310 komaPigSubtype1Step`, `80123558 komaPigSubtype2Step`,
  entries 3-6 `801242b0 area4Type07StepA`, `80124954 area4Type07StepB`, `80124b4c area4Type07StepC`, `80124e88 area4Type07StepD` (also run by
  `80123b94 updateArea4Type07Object`, the area 4 branch of MAIN0 updateL2Type07Object); `8012747c testGcdFlagBitBcefc`, `801274b0
  setScriptGlobalFlagBit` (SCRIPT_GLOBALS[0x58 + n / 8] |= bit)
- x010 (roles uncertain unless noted): `80116b08 animateArea10Palettes` (area 10 entry of MAIN0 data_80079B3C, LoadImage CLUT rows),
  `80117cbc spawnApAwardNumber`, player steps `8011b330 playerStep3FArea10`, `8011b7d4 playerStep41Area10` (playerNormalModeDispatch 0x3F / 0x41),
  `8011c758 playerStep48Area10` / `8011d5f4 playerStep48Area10B` (playerStep48ByArea handlers); collidePlayerWithCat2Objects callbacks `8011de58
  l2Type1FContact` (type 0x1F outside area 1), `8012095c l2Type07Contact` (type 7, area 10); `80121bac updateArea10Type07Object` (area 10 branch of
  updateL2Type07Object) with helpers `801220f8 area10Type07NearSfx`, `801226a8 area10Type07StepBPart`, `80122878 area10Type07Swing`, `80122bb4
  area10Type07StepDPart`; unlayered objects `8012d1bc updateSpearPigAmbushSpawner` (subtype 0, section 0: `8012cfcc spearPigAmbushSpawnStep` waits
  for GVARS.b38, then allocates 8 layer 2 type 0x3A spear pigs from D_8012F4AC), `8012d200 updateRandomEnemySpawner` (FUN_80114968 at random
  intervals until GCD.bceaf + bd0b4 reach 0x1E), `8012d858 updatePlayerFallExitTrigger` (variant 0x26: `8012d678 stepPlayerFallExitScene` walks the
  player left off a ledge), `8012eee0 spawnL2Type32Object` (variant 0x18, one layer 2 type 0x32 object from D_8012F598) / `8012ee18
  spawnL2Type32ObjectB` (unreferenced)
- x003: `80117de4 animateArea3Palettes` (area 3 entry of data_80079B3C; `80118284 loadArea3ClutRows`), `801188fc spawnApAwardNumber`, handlers named by
  table slot (kinds unknown) `80119838 updateL3Type39Object`, `8011992c updateL3Type3EObject` (22 placements), `801287f8 updateL3Type2FObject`,
  `801251c4 updateL2Type20Object`; updateSpikyLizard subtype table D_80135D6C: `80126d18 spikyLizardSubtype0Step`, `80126e24
  spikyLizardSubtype1Step`, `80126f38 spikyLizardSubtype2Step` (walks with sfx 0x76), `8012724c spikyLizardSubtype3Step`
- x001: spring vine step tables (stepSpringVine; roles uncertain): own D_8013C44C (src/data/x001/D_8013C404.c ptrs_8013C44C) steps 2-4 `8011b164
  springVineStep2`, `8011b624 springVineStep3`, `8011b958 springVineStep4`; rope D_8013C460 steps 1-4 `8011aefc springVineRopeStep1`, `8011b498
  springVineRopeStep2`, `8011b74c springVineRopeStep3`, `8011badc springVineRopeStep4`; `801174b0 selectArea1VisibleModels` (areas 1/7 entry of
  MAIN0 data_80079B3C: picks the model column range lo..hi (80 px columns) around the scroll position per section into the list q->e; so the
  data_80079B3C entries are per-area script ops, the x003/x009/x010/x014 ones mostly upload CLUTs), `80117de0 updateL8Type06Object` (layer 8 table
  slot 6), `8011832c spawnApAwardNumber`, `8011e40c updateL4Type16Object` (layer 4 type 0x16; `8011e524 l4Type16ObjectStep`)
- x006: `801167a0 area6ScriptOp` (area 6 entry of data_80079B3C), `80117148 updateL3Type3BObject`
- Strip-identified renames (tools/anim.py strips of each handler, rendered with the emulated init of tools/objinit.py):
  - X000 `80132174 updateRedBird` (was updateA00L2Type08, layer 2 type 8, 9 placements in area 0): every animation of the handler draws a red bird
    with a yellow belly and beak (perched, flapping, gliding, landing; SPR 0x35); its steps `801307a0..801320a8 redBirdStep0..4 / Step1B / Step1C`
    keep the step numbers (roles not traced).
  - X000 `80126fec updateMushroomCluster` (was updateA00L2Type01, layer 2 type 1, 108 placements): idle anim D_8013B1CC is a clump of purple
    mushrooms, D_8013B1D0 (while the player stands on it, b69 == 1, sfx 0x2E/0x2F) the same clump bouncing; when destroyed (b04 2, sfx 0x31)
    `80118d8c spawnMushroomClusterDebris` (was a00L2Type01SpawnChild) spawns 6 pool objects, the single mushroom stalks of the strip.
  - x003 `801281fc updateSpikyLizard` and its 16 helpers (were updateHornedEnemy / hornedEnemy*, layer 2 type 0x28): the strip is a blue lizard
    with a spiked brown/gold shell on its back that walks on all fours, rears up on its hind legs and curls up; "horned" was a guess from the code.
  - x003 `8012f524 updateWingedForkPig` and its 18 helpers (were updateThrowerEnemy / throwerEnemy*, layer 2 type 0x38): the strip is a pig in a
    dark tunic with small blue bat wings carrying a trident: flying, hovering, throwing the trident (the "shot" object), tumbling when hit.
  - x011 `80116fa8 updateFlyingOctopus` / `80116e54 flyingOctopusMove` (were updatePatrollingFlyer / patrollingFlyerMove): the strip is a pink
    one-eyed octopus that floats with dangling tentacles and lashes them out.
  - fix: x011 updateFactoryNpc called itself in the C (ncheck DIFF); the callee at 80119498 is stepFactoryNpcTalk.

# psx_tomba import (Oct 2026, upstream fb3b367): name conflicts kept as ours

Source: psx_tomba `symbols/sces_013.31/main0_symbol_addrs.txt` (US names at PAL addresses) for MAIN0; for X000 the US
`src/overlays/x00/engine*.c` / `x00*.c` function order, placed by a constant US->PAL delta (0xCA0 / 0xCB0 / 0xCBC) between
neighbours whose addresses are known on both sides. Imported names are listed above with "psx_tomba name". Not imported:
- their map is 8 bytes off at 8002b018 / 8002b0b4 / 8002b8c4 (gives stubInventory1..3, which we already have at 8002b010 /
  8002b0ac / 8002b8bc); 8006eb00 / 8006eed8 also disagree (SsQuit / SsStart there vs our SsStart / SpuInitHot).
- their per-overlay `updatePointsBalloon = 0x80117CAC` in x01..x18 is the area-0 address used by the X000 dispatcher; the
  overlay function at 80117cac (16 bytes) is something else. The real copies are the X000 twins (now updatePointsBalloon).
- MAIN0, ours kept (theirs): ThreadTableRun (dispatchTasks), ThreadCreate (openTask), ThreadWaitFrames (sleepTask),
  InitSubsystems (initObjectPools), resetCameraScratchDefaults (resetCamera), ObjAlloc (allocObjectLayer3), ObjFree
  (freeObjectLayer3), ObjFreeDup (freeObjectLayer4), PoolFree_1F800210 (freeObjectUnlayered), ObjListPush_1F800218/21C/220/224/
  230/228/22C (pushDrawListLayer1/Main/Capped/Layer4/Layer8/Layer5/Layer7), SoundUpdate (updateSound), SoundFindFreeVoice
  (allocSfxVoice), SfxPlay (playSFX), SfxPlay2 (playSFXWithNote), SfxPlay3 (playSFXWithVolume), MusicStopOrFadeIn (stopBgm),
  SoundStopAll (stopAllSound), MusicMute/MusicUnmute (muteBgm/unmuteBgm), SfxStopAll (keyOffSfxAll), SoundInit
  (shutdownSound), SoundShutdown (sndQuit), Rand (nextRandom), ObjApplyVelocity (applyObjectSpeedXY), MulNegSinScaled /
  MulCos / MulNegSin / MulCosDup (fixedMulSin / fixedMulCos / fixedMulSin2 / fixedMulCos2), AnimLoadDuration
  (readAnimFrameCount), AnimJump (advanceAnimFrame), AnimAdvance (tickAnimation), loadCollisionBounds (loadButtonConfig:
  disagree, to check), drawDebugText (fontDebugPrintf), markSectionVisited (setSpawnAreaFlag), updateCameraController
  (updateCameraScroll), getDistanceToPlayer (distanceToPlayer), uploadSpriteFrameToVram (uploadSpriteFrame),
  ObjSpawnType0/1/5/8 (spawnItem / spawnItemDrop / spawnItemFixed / spawnItemChest), ObjCheckHeadCollision
  (clampToCeilingAndProbeSides), ObjTileCollide (probeSidesAndApplyTileResponse), TileCollideAt (probeCollisionAtDepthA),
  dispatchAreaCollisionHookA/B (dispatchAreaEnemyInit / dispatchAreaEnemyUpdate: disagree, to check), isQuadOnScreen
  (isPolyFT4OnScreen), AddDrawMode (addDrawModePrim), start (__SN_ENTRY_POINT).
- X000 player steps, ours kept (theirs): playerInitMode (initPlayer), playerStepWalk (playerWalk), playerStepJump (playerJump),
  playerStepWeaponAttack (playerAttack), playerStepSwingFromObject (playerGrabLedge), playerStepPerchLeap
  (playerClimbOverLedge), playerStepPerchIdle (playerClimb), playerStepClimb (playerCling), playerStepBitten (playerHoldEnemy),
  playerStepBittenJump (playerThrowEnemy), playerStepLaneJump (playerJumpPlane), playerStepHurtKnockback (playerHurt),
  playerStepHurtStun (playerHurtBitten), playerStepHazardRespawn (playerFallInWater), playerNormalModeDispatch
  (updatePlayerActive). Their PlayerState / PlayerHurtType / PlayerSubState enums are in include/tomba/game.h.
- X000 area 0: updateMushroomCluster (theirs updateEvilGrass, 80126fec): to check against the strip.
- Re-check Oct 26 2026 (psx_tomba symbols/sces_013.31 after 2026-10-19): no new function names apply (every real name in their
  main0 / x00..x18 maps is already ours or is one of the MAIN0 conflicts above; 55 MAIN0 conflicts, none in X000 / overlays).
  Data imported into data_names_main0.csv: PAD_PRESSED, BUTTON_CONFIG_INDEX, CAMERA_SCROLL, BALLOON_TAIL_SPRITES, BUTTON_CONFIGS,
  ATAN_TABLE, RAND_SEED. Not imported: their data-typed entries for functions we already name (freeObjectLayer3 = ObjFree,
  pushDrawListCapped, setSpawnAreaFlag = markSectionVisited), GAME_A / GAME_B aliases of GAME, Psy-Q stamps / _gp / _ctype_.
  Header alias D_8009C9D8 (NTSC spelling, resolves to PAD_PRESSED) left in its header.

# Draw captures (tools/objdraw.py, Oct 2026)

tools/objdraw.py spawns a placement record in a running room (debug area select, MAIN0 spawnObjectFromRecord), lets the game update
it and captures the primitives its category draw routine puts in the ordering table (method of TombaEdit formats/level/draw.py: a
draw list holding only the object, a camera on the object). Objects drawn from several parts, children or their own primitives now
come out as the game shows them, where the init-only strips (tools/objinit.py / tools/animobj.py) were unreadable.

- X000 `80134e5c updateHoodedOldMan` (was updateA00L2Type34; layer 2 type 0x34, area 0 room 3): an old man with white hair, bent
  over in a long tan hooded robe (5 frames of a slow sway).
- X000 `80114a08 updateVillagerNpc` and its 16 twins (was updateL2Type55Object; layer 2 type 0x55): area 3 -> updateLostBossItemNpc,
  else x00n updateFreedVillager. Captured in area 12 room 0: subtype 7 a bearded sailor in a red coat and blue shirt, subtype 0 a
  blond villager in a blue tunic - the freed villagers.
- x001 `8012b2d4 updatePurpleCrawler` and its 16 helpers purpleCrawler* (were updatePurplePig / purplePig*; layer 2 type 0, area 1
  room 3): not a pig. The capture is a purple dome-shaped creature with many purple legs and a green top with two frog-like eyes,
  walking and turning; it fits the climb / wall crawl / carry-player steps of the code. (The old strip was the SPR table shared
  with the pigs, read with the wrong frames.)
- Koma pig (layer 2 type 2, the area handlers updateKomaPig): area 1 room 0 subtype 8 captures a pink pig with a red mohawk lying
  down and snoring, so the koma pig handlers are pigs (Koma Pigs are the game's pig enemies); names kept.
- X000 `8011935c updateYellowChick` (was updateA00PoolType07, pool type 7): captured in area 0 room 1 (P/07:0, P/07:1): a round yellow
  fluffy chick with red spots (subtype 0 small, subtype 1 larger with brown legs), anim by subtype, CLUT direction from b0c; static otherwise.
- X000 `80126fec updateMushroomCluster` (theirs updateEvilGrass): area 0 room 3 capture of L2/01 is a clump of purple bulbs on stalks;
  ours kept (a mushroom clump, bounces when stood on, breaks into single stalks).
- Named by behaviour (no picture):
  - X000 `8012e4c4 updateA00TouchSparkTrigger` (was updateA00L2Type05): draws nothing; when the player (or, on alternate frames, one of the 4
    objects of the 800B1478 table) enters its hit box: `8011aa30 spawnTouchSparkTriggerSparks` (was a00L2Type05Init) spawns 3 pool type 0xB
    sparks at the contact point, SfxPlay2 0x39, two pool type 0x16 one-shot effects at fixed spots (by subtype), then a 120-tick cooldown.
  - X000 `80136090 updateA00RoamingFlyer` (was updateA00L2Type57, only while 8009CDAC == -1; nothing drawn in room 5): subtype 0
    `80135b0c a00RoamingFlyerSteer` (was a00L2Type57Aim) turns toward a target point every 120 ticks and flies inside a box, else back to
    0x1EC,-0x87; subtype 1 `80135db0 a00RoamingFlyerBob` (was a00L2Type57Move) bobs on a sine.
  - X000 `8011ad98 updateA00BackdropSprites` (was updateA00PoolType14) / `8011ac58 a00BackdropSpritesStep4`: camera-fixed sprites (b0a 7,
    drawn at d30/d34 minus the camera), CLUT flicker by subtype, as the other areas' updateBackdropSpriteSet.
  - x004 `80127ec4 updateHomingCarrierFlyer` (was updateL2Type31Object; the player clings to type 0x31): bobs until the player is past
    x 0x83, `80127b14 homingCarrierFlyerChase` (was l2Type31StepA) flies at the player and holds it, `80127d54 homingCarrierFlyerReturn`
    (was l2Type31StepB) flies off and returns to its spawn point. Spawner still not found (no placement or record in X004).
  - x004 `80125248 updateDeferredLayer1Type2Spawn` (was updateL2Type1BObject): hit box only; when another object sets step 2 it spawns a
    layer 1 type 2 object with its position / subtype / b0c / b6b and frees itself. Spawner not found.
- Pool types 0x39 / 0x3C in area 0: no area-0 handler; the MAIN0 table slots hold x003 updateL3Type39Object / x004 updateL3Type3CObject.
- Still pending: L2/06 (code: a multi-hit breakable, each hit changes stage/anim/box and spawns pool type 8 debris with sfx 0x33, removed
  after the last stage), pool type 0x11, L4/0A, L4/0D (draws nothing), L4/11, type 07 in area 4.
- Open conflicts: loadCollisionBounds -> loadButtonConfig (renamed: theirs is right, copies T[1F80019E] into 1F8003C4..CA, the
  button masks the player code tests, e.g. talk = 1F8003C4; callers bootStartupTask, o0ptsub0 buttonConfigMenuStep). dispatchAreaCollisionHookA/B:
  ours kept (area 0 calls pushCat2/Cat5ObjectOutOfCat4Type04, area 3 their x003 twins: collision passes, not enemy init/update).
  updateMushroomCluster vs updateEvilGrass: not captured yet.
- Twin sync (Oct 2026): 434 twin rows in notes/functions_x0nn.csv that still held FUN_/func_ names now carry their X000 twin's name
  (names_x000.csv), 30 stale functions_x000.csv rows fixed, 595 src/x0nn files renamed to the function they hold (content too). Twins
  whose X000 name is already used by another function of the same overlay (identical code at two addresses, e.g. x0nn 800f4e9c ->
  X000 800f4e68 playerSetAnimPairA, 8010d7f0 setAngleSpeedFromDir8B) keep their generic name.

# Behaviour names, round 4 (Oct 2026): shared X000 handlers and area-0 objects

Names from code reading (draw captures where noted); the X000 name is applied to every overlay twin row that carried it (a second
twin of the same function at another address of an overlay keeps its generic name, e.g. x004 FUN_8012f660 = scriptActorLeaveSceneIfIdle).
- `8011468c updateScriptActor` (was updateL2Type18Object; layer 2 type 0x18, allocated only by MAIN0 scriptOpSpawnObject kinds 2 / 10: the
  sprite / model actors of the script cutscenes, SCRIPT_OBJECTS[idx]). Steps: `80113f38 scriptActorRun` (was l2Type18Active: step 0 animate,
  1 walk timed, 3 message box showMessageBoxDirectTimed(0xd..), 4 release the player when the box closes), `80113888 scriptActorWalk`
  (State2: velX walk clamped in area 2, TileCollideAt), `80113b6c scriptActorJump` (State5: velX/velY arc, lands on TileCollideAt),
  `80113cc8 scriptActorLaneJump` (State7: the arc also moves the depth to the next 90-unit lane), `801142c0 scriptActorLeaveScene` (was
  l2Type18Hit; b04 2: message box 2/x by GCD.bcdb1, puts the player in mode 6 and walks off until the player b69, printInfoMessage, frees
  itself), `8011465c scriptActorLeaveSceneIfIdle` (HitIfStep0).
- updateTreasureChest helpers (layer 2 type 0xE; subtype = chest kind: 0 static, 1 carried by the parent object d90, 2 dropped onto the
  ground, 3 waits for a flag, 4 falls, 5/6 static variants; b0c & 0x7F = chest colour, opened only with the matching key GCD.bd0aa/bd0c8/
  bd0d0/bd0d1 else printInfoMessage(0, 2) and a shake): `80112864 treasureChestInitStatic`, `80112898 treasureChestInitCarried`,
  `8011287c treasureChestInitDropped`, `801128f8 treasureChestIdle`, `80111f58 treasureChestFollowCarrier` (follows d90, thrown off when
  the carrier passes the camera), `80112918 treasureChestSettleOnGround`, `801129d0 treasureChestWaitForFlag` (active 4 until
  DAT_8009c93b), `80112520 treasureChestFall`, `80111dc8 treasureChestTossMove` (was l2Type0ESharedStep: knocked along the hit angle w78,
  else gravity), `80112c24 treasureChestOpenLocked` (Hit0: key check, spawnItemDropsFromTable, open anim, sfx 0x18/8, falls and
  spawnBreakPieces; subtype 6 completes event 0x19), `80112204 treasureChestOpenCarried` (Hit1: two spawnItemFromEntry items, tossed),
  `80113034 treasureChestOpenLockedStatic` (Hit2), `801133b8 treasureChestOpenAnimOnly` (Hit3), `801125e8 treasureChestOpenFalling`
  (Hit4: two D_80115D28 items), `80112be0 treasureChestCheckAreaFlag`.
- `8011191c updatePlayerPowerAura` (was updateL1Type0AObject; layer 1 type 0xA made by `80111a50 spawnPlayerPowerAura` (was
  spawnL1Type0AObject) from MAIN0 spawnPlayerPowerEffect at the player): a sprite glued to the player (TBL8011[subtype] anim, b0a 13, alpha
  b6b ramps up 8/frame then down for 0xB4 frames, hit flash via w22/b6a); `8011145c playerPowerAuraInit`, `80111880 playerPowerAuraStep`,
  `80111590 playerPowerAuraFacingStep` (subtype 0: x = player x -/+ 2 by facing, y - 8), `80111714 playerPowerAuraHiddenStep` (subtype 1:
  visible = 0, same timing).
- `800ee25c updateApBoxByArea` (was updateL4Type38Object): layer 4 type 0x38 = area 0 `80122118 updateA00ApBox100k` (was l4Type38Update:
  room 5, b0c 0 the box (0x13 model scaled 0xC00), other b0c parts copy its state; `80121b28 a00ApBox100kShowRequiredAp` (StepA: while the
  player holds it, six digit sprites "100000" at 0xA3C/-0x15E), `80121e50 a00ApBox100kCheckAp` (StepB: AP (DAT_8009c96c) < 100000 starts
  event 0x10 "The AP Box", else completes it), `80121fe0 a00ApBox100kOpenAnim` (StepC: lid rotations d84/d8c, then spawnItemFromEntry
  D_80138F04)), area 9 x009 updateApBox.
- `80121870 updateA00ApBox50k` (was updateA00L4Type26; room 2, same event 0x10 with 50,000 AP): `80121124 a00ApBox50kShowRequiredAp`
  (Step: digits "50000" at 0xA78../-0x4D8 for 0x78 frames), `8012160c a00ApBox50kSparkle` (SpawnParts: a type 0x31 sparkle every 8 frames
  on a circle D_80138EF4), `8012171c a00ApBox50kReward` (two D_80138D6C items, blink out), `80121ae0 a00ApBox50kHideDigits` (ReleaseParts).
- `800eb9f0 updateDigitSprite` / `80121a5c spawnDigitSprite` (were updatePoolType44Object / spawnPoolType44Object): pool type 0x44, one
  digit b0c of the MAIN0 digit table TBL_80012150 (the "50000" / "100000" of the AP boxes), freed 0x1E frames after b04 2.
- `800ebb48 updateWeaponCarryLink` (was updatePoolType5BObject): pool 0x5B made by MAIN0 weaponHitCat5Object: d90 = the weapon, d94 = the
  hit category-5 object (active 5); copies the weapon position into it while the weapon is in states 2..6, then releases it (b68 = 1,
  animFrame 2).
- `8011b6e0 updateA00TutorialHint` (was updateA00PoolType59): with DAT_8009d005 == 1 the box MFNT000S 9/0 "you can swing hanging from a tree
  branch", else `8011b5dc a00TutorialHintJumpMessage` (was a00PoolType59Talk): 9/1.. "press X to jump..." by the button configuration
  D_801386A8[D_8009F3DC]; freed when the box closes.
- area 0 layer 7 backdrops: `8012236c updateA00FarBackdropWest` (was updateA00L7Type00: drawn at (d30 - camera x) / 8 while the camera is
  left of x 0x71A), `80122724 updateA00FarBackdropEast` (L7Type02: the same for x >= 0x719, b0c 0 wraps the 12-bit x), `801224c0
  updateA00ColorCyclingBackdrop` (L7Type01: parallax 1/2 with the CLUT 0xC0/0x1E7 cycled +-0x40 every 15 frames).
- `80116d14 updateA00ScrollController` (was updateA00L5Type11): the area-0 camera object (stepScrollReturnY / stepScrollClearX,
  cameraFollowPlayerX, syncObjectTargetY, updateCameraScroll; DAT_8009c966 intro state).
- `8011f1d0 updateA00FlagGatedModel` (was updateA00L4Type0D: room 5, draws nothing itself; with GCD.bcdac == 0xFF takes model 12 of the
  SPAD.i334 table and is pushed to the layer 4 draw list) and `8011b7f0 updateA00FlagGatedSprite` (was updateA00PoolType5A: sprite
  D_8013B168[b0c] while GCD.bcdac == 0xFF, else freed).
- `8011a744 updateA00FallingLeaf` (was updateA00PoolType0B; capture P/0B:0 in room 1: a green leaf): the three leaves that
  `8011aa30 spawnLeafShakeLeaves` (was spawnTouchSparkTriggerSparks) drops when the player touches `8012e4c4 updateA00LeafShakeTrigger`
  (was updateA00TouchSparkTrigger / updateA00L2Type05): D_8013B244[b0c], four subtype velocity sets, rotates d8c, freed off screen.
- `8011a63c updateA00KomaPigSpawner` / `8011a418 a00KomaPigSpawnerStep` (were updateA00PoolType0A / a00PoolType0AStep): subtype 0, when the
  camera is within 0xC0 x / 0x2D z, spawns a layer 2 type 2 subtype 5 Koma pig at its position (depth 0x5A) every 2000 frames, 10 at most
  (w98), the pig's d94 points back at its w9a counter.
- `801191b0 spawnA00PoolType03Pair` (was updateA00PoolType27: not a handler; makes two pool type 3 objects (b0a 5, subtype 0 / 1, b0c =
  a == 0) at x/y/z).
- `80114890 updateLeafButterflyByArea` (was updateL2Type1FObject): the layer 2 type 0x1F thunk jumps to x001 8013839c / x010 80123d18
  updateLeafButterfly (jal targets read from the X001 / X010 binaries); x010 `8011de58 leafButterflyContact` (was l2Type1FContact: the
  player's box overlap puts the butterfly in b04 2 and the player's de4 / bac 2 "caught" state, spawnImpactEffect).
- `800ee18c updateStaticDecorModel` (was updateL4Type13Object): layer 4 type 0x13, no collision (active 2, b0a 0x10 model); in areas 4 / 12
  it is pushed straight to the draw list, elsewhere culled with ObjCullRegister.
- `80118438 updateA00FogCloud` (was updateA00PoolType01; capture P/01:0: a wide undulating cloud, b0a 0x11 quad with four corner vectors
  v[0..3] from D_80138644[subtype]): `80118638 a00FogCloudDrift` (subtype 0: bobs on velH/w7c, corners move in and out for 0x140 frames each
  way), `8011899c a00FogCloudClearStep` (subtype 1, at the camera depth: DAT_8009c966 1 = drift left 1.5 px/frame wrapping at x 0x4D9,
  2 = come back, 3 or DAT_8009cda6 == 0xFF = freed: the "Clear the Fog" scene), `8011880c a00FogCloudWobble` (SubStep: corner x +-1 per
  frame between -0x50..0x50).
- biting plant (layer 2 type 7: three placed parts linked by d94; captures in room 1: subtype 0 a green wavy stalk,
  subtypes 1 / 2 the two white toothed jaw halves on a green base; area 0 GAME.bittingPlantFlowerState): `80111ae0 updateBitingPlantByArea` (was updateL2Type07Object:
  area 0 / 4 / 10 thunk), `8012ed60 updateA00BitingPlant` (was l2Type07Update), x004 `80123b94 updateA04BitingPlant`, x010 `80121bac
  updateA10BitingPlant` (were updateArea4/10Type07Object), x010 `8012095c bitingPlantPlayerContact` (was l2Type07Contact). Steps:
  `8012f1b4 bitingPlantSway` / `8012f228 bitingPlantSwayStep` (velH 0x80 pendulum into d8c) / `8012f36c bitingPlantCopyParentAngle` (step 0),
  `8012f380 bitingPlantLunge` (step 1, when the player is within 33 x / 91 z / above: `8012f3f4 bitingPlantLungeHead` springs up with velV
  -0x800 as a stretched quad (b0a 4, corners wb4..d0), `8012f864 bitingPlantLungeStemA` / `8012f9e0 bitingPlantLungeStemB` follow with
  their d8c angle), `8012fb3c bitingPlantHitWobble` / `8012fbb0 bitingPlantHitWobbleStep` / `8012fe84 bitingPlantHitCopyParent` (step 2 on
  b68: damped sideways wobble), `8012fe98 bitingPlantStep3` (step 3 after b69 == 1, sfx 0x2C), `801302a0 bitingPlantEatBait` /
  `80130314 bitingPlantEatBaitStep` (step 4 after b69 == 2: GCD.bd07b = 1 or frees the thrown bait object D_8009C948, three swallow swings).
- `801195f4 updateA00Tornado` (was updateA00PoolType09; uncertain - the draw of P/09:0 is empty because the anim comes from the spawner):
  `8011a148 spawnA00Tornado` (makes the pair subtype n / n+1 with the models D_8013B1B8 / D_8013B1B4 plus `8011a2f8 spawnA00TornadoOrbiters`,
  four subtype-4 parts orbiting on sines), `80119988 a00TornadoStep` (rises toward the camera target D_1F800168 with pad vibration, sets the
  player mode 4/5 and D_800A6048 to its position (carries the player), sfx 0x36, scale d64 shakes, FUN_80026c50(4,1,1) when done; step 10
  waits for GAME.area00_eventControl & 1), `80119864 a00TornadoOrbitStep` (subtype 4: position = parent + sin/cos of wb4..wb8).
- `80118bc0 updateMushroomClusterDebris` (was updateA00PoolType02; capture P/02:0: a purple bulb flung in an arc; six made by
  spawnMushroomClusterDebris): jumps with velY -0x400 and the movetab by subtype parity, freed when it lands.
- `80118f90 updateA00SporeCloud` (was updateA00PoolType03; capture P/03:0: a purple cloud of puffs that grows): CLUT 0xE0/0x1E0, grows
  wb8/wba every 0x38 frames; `80118e70 a00SporeCloudStunPigs` (subtype 1 b0c 1: a layer 2 type 2 Koma pig in its box is put in state 2);
  `801191b0 spawnA00SporeCloudPair` (was updateA00PoolType27); made by `8011bf68 updateA00SporeBranch` (was updateA00L4Type02: a chain of
  segments, the tip has the hit box; shaken (wb4 1..6 sway tables by the hit facing) it releases a spore pair at the tip every 0x78 frames
  with sfx 0x32).
- `800eb0c8 updateKomaPigDustPuff` (was updatePoolType10Object; capture P/10:0: a white puff that expands and fades): pool 0x10 made by
  komaPigGrabbed (subtype 0, sfx 0x10, drifts 0x5A frames) and the pig landing code of komaPigState6/7 (subtype 1, sfx 0x27); per-area
  sprite tables DAT_80114AB8 / AC0 / AC8.
- Captures with nothing to name: P/0A:0 (updateA00KomaPigSpawner draws one black screen-sized quad), P/09:0 / P/01:1 nothing drawn; 4/03,
  4/00, P/11, P/04 and 2/06 in room 4 hit the 10-minute cap without output.
- updateNpcCharles steps: `80134980 npcCharlesHopToSpot` (was a00L2Type15Step1: when the player comes within 0xA0 it hops (velX 0xC0, velY
  -0x480) and glides to x 0x138 / y -0x11C), `801346c0 npcCharlesHopAway` (Step3: hops left, lands (active 4, anim D_8013AC30), hops again
  away from its facing).
- `801236f0 updateA00BitingPlantBait` (was updateA00L5Type0B; room 1 layer 5 item object initItemObject): on contact it is thrown along its
  facing (velH/velV from D_80138F8C), the camera follows it when GCD.bce4d == 0 (D_8009C948 = this object, GVARS.b3f/b3e/b42), lands with
  a bounce; bitingPlantEatBaitStep frees it when the plant swallows it.
- `8012d294 updateA00PinkPiglet` (was updateA00L2Type2B; capture 2/2B synthetic in room 1: a small pink piglet; made by updateKomaPig state 3
  for the pig with the b6b flag, camera on it): falls from the spawn point (velV), wanders with four random walk / idle / turn states, and
  when its b04 is set to 2 starts event 0xA7, addItemToInventory(0x23) and sets its area flag (uncertain which pig / item).
- `801328b4 updateA00WoodenBranch` (was updateA00L2Type09; captures 2/09:0 / 2/09:1: wooden branch pieces with leaves, D_8013B24C[b0c]
  anims, CLUT 0x90/0x1E6): box 0x20 x 0x40, subtype step table D_801392E0, per-b0c vectors D_801392EC (uncertain role).
- `800eb394 updateFlashEffect` / `800eb50c spawnFlashEffect` (were updatePoolType1BEffect / spawnPoolType1BEffect; capture P/1B:1: a large
  white jagged flash): pool 0x1B with the per-area sprite table D_80114AE4; subtype 1 sfx (0xE, 4), subtype 2 sfx (0xE, 0), subtype 0 is
  remembered in D_800A611C; made by updateFrog (event 3) and the area-0 scene code FUN_801363e4.
- Captures with nothing new: P/44:0:5 draws the digit "5" (updateDigitSprite confirmed), 2/0E:0 a golden treasure chest, P/0A:0 / P/0A:1 one
  screen-sized black quad, L4/0A part 3 (record 00040a02..) a small pink two-lobed shape and part 1 a vertical brown pole: not identified,
  L4/0A stays generic; 4/06:1 one brown wooden plank model (the segments of updateA00L4Type06, role unknown).
- `8011926c updateA00SplashDroplets` (was updateA00PoolType06; capture P/06: blue droplets spraying up, sfx 0x30, one-shot anim
  D_8013B1F0): spawned by updateL4Type00Object subtype 0 at anim frame 4 of its stretch.
- `80134404 updateA00DarkCloakedFigure` / `80134190 a00DarkCloakedFigureIdle` (were updateA00L2Type14 / a00L2Type14Step; capture 2/14
  synthetic in room 1: a dark hooded figure with a scarf standing and gesturing; made by spawnA00DarkCloakedFigure at 0xA38/-0x124 or, with
  DAT_8009c984 bits 8|0x10, subtype 1 at 0xC29/-0x2E3): random idle anims D_8013B270 / D_8013B274 (step by subtype), when GCD.bcda9 ==
  0xFF it leaps off (velX 0x200, velY -0x400) and is freed off screen (identity of the character unknown).
- Captures with nothing: 4/18 and 4/17 (synthetic, draw nothing without their scene), 4/1B (room 4), 7/01 / 7/02 (nothing drawn),
  P/5A (flag-gated), P/10:1 / P/17 / 4/01 / P/16 / P/04 hit the cap.

Names from code reading only (no captures), table slots from pointer scans of MAIN0.EXE / X000.BIN:
- `8003dd28 updateItemObject` (was FUN_8003dd28): the layer 5 handler of item types 0..5 and 8 (MAIN0 table 8007B310..8007B330):
  initItemObject, per-type launch (5/8 velY -0x500), bounce/land/wall logic with D_8007B35C, then D_8007A890[D_8007A7F0[subtype]] and
  freeObjectLayer5; the flag-gated subtypes 0x3F/0x9E/0x90/0x3C/3 skip to free.
- `8002e83c updatePoolType31Object` (was FUN_8002e83c; pool table slot 80079C70): subtype 0 spawns subtype-1 type 0x31 children on a
  random 0x800/0x1000 radius every other frame until timer runs out; children play D_80012204 once.
- `80069464 PadEnableCom` (was FUN_80069464; US psx_tomba libpad padmain.c, as the file comment says).
- `800e9dc0 updateL8Type00Object` (was FUN_800e9dc0; layer 8 slot 80079B88): box width from D_80114A48[b0c], culled with margin for
  b0c 0, freeObjectLayer8 at b04 3. `800e9e98 updatePoolType00ByArea` (was FUN_800e9e98; pool slot 80079BAC): switch on the area
  DAT_8009c960 (0 updatePointsBalloon, 1/3/4/6/7/9/10/12/17 the overlay handlers).
- `801175a4 a00CameraController` (was FUN_801175a4): area 0 entry of the per-area table D_80079AEC that updateCameraController steps 1/2
  call; section 3 runs its own steps (cameraModeHandler6, the two tryStartItemInteraction exits, cameraEventScrollHandler), the other
  sections go to updateA00ScrollController. `801176d8 a00LinkAreaModelBlocks` (was func_801176D8): area 0 entry of PTR_DAT_80079b3c
  (callAreaModelLinkHook): per section picks the WMD column blocks (x / 80, lo..hi) around the camera into o->p[].
- `801228bc playerInitAreaHook` (was FUN_801228bc; playerInitMode `case 0`, same name as the x001/x003/x004/x009 entries): sections 4/5
  set b0f, flag 2 of DAT_8009c984 spawns a frog, the tornado unless bcda6 == 0xFF, DAT_8009d2af == 0 enters b04 5.
- `801340c4 spawnFrog` (was FUN_801340c4): layer 2 type 0x12 (updateFrog) at x/y/z, anim PTR_DAT_8013b184, step 3. `8012e0b8 spawnChick`
  (was FUN_8012e0b8): layer 2 type 3 (updateChick) at x/y/z, b0c 1, anim PTR_8013b26c. `801231f4 playerEventStepSpawnChicks` (was
  FUN_801231f4; playerEventModeDispatch): player anim 0x22, at frame 0xBD four spawnChick around the player and sfx (0,4).
- chick (updateChick) steps: `8012dcfc chickDropToGround` (was FUN_8012dcfc; step 0: launched up (b0c 0: velY -0x400) and falls to the
  tile, then step 1), `8012d9fc chickWander` (was func_8012D9FC; step 1: random idle / walk with chirp sfx 0x3A / hop by D_80139124),
  `8012de8c chickStepDispatch` (was FUN_8012de8c; the step switch over the two, unreferenced).
- dark cloaked figure event (unlayered variant 1, D_8007C1C8[1]): `80138008 updateDarkCloakedFigureEvent` (was FUN_80138008),
  `80136a98 spawnA00DarkCloakedFigure` (was FUN_80136a98; the type 0x14 figure at 0xA38/-0x124, or subtype 1 at 0xC29/-0x2E3 with
  flags 8|0x10), `80136bc0 a00DarkCloakedFigureTalkCheck` (was func_80136BC0; 1 when the player stands at the figure and presses the
  talk button), `80136ce0 a00DarkCloakedFigureScene` (was FUN_80136ce0; the message-box 6 dialogue sequence).
- Charles event (unlayered variant 2, D_8007C1C8[2]): `801380b8 updateCharlesEventObj` (was FUN_801380b8; spawns the type 0x15 npc at
  0x140/-0x32 or 0x138/-0x11C), `80137458 charlesTalkCheck` (was func_80137458; counts the talk / event conditions of section 0 and
  frees the object when none holds), then updateCharlesTalkScene.
- `801306f0 uploadThrownObjectFrame` (was FUN_801306f0; updateThrownObject): unpackSpriteFrame frame a into DAT_800d7e28 and loadTIM
  into the VRAM strip b at (0x1C0 + b*0x20, 0xA0).
- `8011e30c updateAreaMechanisms` (was func_8011E30C; dispatchAreaInitB target, same name as the x001/x003/... entries).
- `80118040 spawnApAwardNumber` (was func_80118040; awardAPByArea area 0, same name as the x004/x009 entries).
- `80123b64 spawnA00L5Type0CObject` (was FUN_80123b64; biting plant): layer 5 type 0xC subtype 8 at the given position record.
- `80030510 updateL4Type1AObject` (was FUN_80030510; MAIN0 layer 4 table slot 80079DDC, next to updateL4Type1BObject at 80079DE0).
- `80138218 updateA00UnlayeredType0D` (was FUN_80138218; entry 13 of the MAIN0 unlayered type table D_8007BF34): subtype 0 advances
  D_8009d005 0 -> 1 -> 2 when DAT_800a60d6 == 3 (and DAT_800a60ea == 4) spawning an unlayered type 0x59 object; subtype 1 (spawned by
  a00IntroEventScroll) runs `8011b4b0 a00UnlayeredType0DShowMessage` (was FUN_8011b4b0): once visible and DAT_a == 0, showMessageBox
  (10, 0, 100, 236) and frees itself when the box closes.
- `80116818 a00IntroEventScroll` (was func_80116818; x000 table 80138420): the section-0 camera sequence of event (2,0): when the
  player reaches x >= 0x49E / y < -0xE7 sets the intro state D_8009C966 = 1, setEventStarted(2,0,1), spawns the type 0xD/1 object;
  with area00_eventControl bit 0 and GVARS.b30 == 3 scrolls to 0x52E with stream 2 (cdSeekStream), fade 3/4, startAreaBgm and
  setEventComplete(2,0).
- `80116758 a00SectionScrollAdjust` (was func_80116758; dispatchAreaItemHandler area 0 after loadSectionHeight): section 1 lowers the
  scroll target w32 by the camera x past 0x948 (to -0x154), section 5 sets -0x54 once GCD.bce3d == 0xFF.
- `80123344 updateA00ItemObj` (was func_80123344; dispatchAreaItemInit area 0, as x004 updateArea4ItemObj): subtype 0 / 1 sprite
  objects (anims D_8013B0E0 / D_80134DC4).
- `80127380 komaPigAnimAdvance` (was func_80127380; komaPigState0): AnimAdvance variant that loads the box from D_80138FD8[frame].
- Left generic with a `// probably:` line in the source (purpose readable but owner unknown, mostly unreferenced player step entry
  code): FUN_80028b84, FUN_8006ae9c, FUN_8006af74, FUN_8006b0ac, func_8006B62C, func_8006B7EC (libpad, no US names), FUN_800eb29c,
  FUN_800ed258, FUN_800ed750 (villager step table D_80114C58[1]/[2]), FUN_800ee560, FUN_800ee7e0, FUN_800f35e0, FUN_800f5014,
  FUN_800f7a38, FUN_800f802c, FUN_800f809c, FUN_800f8660, FUN_80102a5c, FUN_80102eb4, FUN_801031fc, FUN_80103cbc, FUN_80104040,
  FUN_801043f4, FUN_80108f60, FUN_8010afc4, FUN_8010b3b8, FUN_8010bf70, FUN_80113a8c, FUN_801137bc, FUN_801139b4, FUN_8011dd30,
  func_8011DE84, func_8011D204 (chained three-part object, table 80138B80), FUN_8011e1b0 (a mechanism of updateAreaMechanisms),
  FUN_80123648, func_80123C20, func_8011C804 (area-0 item dispatch targets), FUN_80134308, FUN_80137378, FUN_80137f54 / FUN_801361e4 /
  FUN_801363e4 (unlayered variant 0 scene), FUN_80132bb0, FUN_80136a8c, FUN_8013736c, FUN_80137f48 (stubs), func_80105A34,
  func_801072FC, func_80110CAC, func_8012127C, func_80122C10 (player step 0x35), func_8012FF0C (bitingPlantStep3).

# User names (names_user.json, tools/apply_names.py)

- `800e8044 gameOverSetupCamera` (was func_800E8044; G0OVER) - camera object 1F8000C0 at (0,0,-0x220) rot (0xA0,-0x78,0): base matrix minus position plus rotated offset
- `800e8194 gameOverSetupLighting` (was func_800E8194; G0OVER) - colour matrix / far-back colour of the light object 1F800118, light direction from RotMatrixX/Y(0x638,0x800)
- `800e82f8 gameOverLetterFlyIn` (was func_800E82F8; G0OVER) - letter object state 0: velocity towards the target slot D_800E8C94/CA4[type] over 24 frames with a vertical arc; state 1 integrates it
- `800e8404 gameOverLetterStep` (was func_800E8404; G0OVER) - per-letter state machine of the 8 'GAME OVER' model letters: start slot D_800E8CB4/CC4, fly in, pause, drop, hold, then tumble by subtype and fade the light (type 0)
- `800e87a0 gameOverDrawLetters` (was func_800E87A0; G0OVER) - draws the 8 letter models: RotMatrix of d84/88/8c times the camera, lit F3/F4/G3/G4 groups (modelDrawF3Lit..G4Lit)
- `800e897c gameOverScreenTask` (was func_800E897C; G0OVER) - overlay entry of gameOverScreenState: w4e 0 camera/light setup and letter objects from the model table 1F800334, w4e 1 step (hold 0) and draw
- `800e8b08 gameEndScreenTask` (was func_800E8B08; G0OVER) - overlay entry of gameEndScreenState: same as gameOverScreenTask with hold 1 (letters stay until the task timer < 0x3D)
- `800e8078 titleDrawSpriteRecs` (was func_800E8078; O0PTSUB0) - draws a -1 terminated SprRec list as SPRTs (clut by GetClut, draw mode per record) at an x/y offset into OT+1
- `800e8284 optionsMenuStep` (was func_800E8284; O0PTSUB0) - options menu input: w4e row 1 sound mode D_8009D67C, 2 stereo/mono D_8009F0E8, 3 vibration D_8009D610.mode, 4 button config (w4c 3), 5 screen adjust (w4c 2); returns 1 on exit, draws with optionsMenuDraw
- `800e8618 buttonConfigMenuStep` (was func_800E8618; O0PTSUB0) - button config screen: triangle/circle cycles D_8009F3DC 0..5 and loadButtonConfig, cross back to the options menu (w4c 1); draws with buttonConfigMenuDraw
- `800e8700 screenAdjustMenuStep` (was func_800E8700; O0PTSUB0) - screen position screen: d-pad with auto-repeat moves D_8009D4FC/D_8009D4FE (x -7..0x18, y 5..0x14) into the display offsets D_8009E350.., circle restores the saved values
- `800e8a14 bootLogoSetSpriteBase` (was func_800E8A14; O0PTSUB0) - stores D_800EB014 into D_8009D630 before the boot logo (bootLogoState)
- `800e8a2c bootLogoDraw` (was func_800E8A2C; O0PTSUB0) - draws the boot logo sprite list D_800EB13C (bootLogoState w4a 4/5)
- `800e8a5c titleLogoInitBounce` (was func_800E8A5C; O0PTSUB0) - resets the title logo bounce (amplitude 0x1000, phase 0x40) at loopTitleScreen w4a 2
- `800e8a94 titleLogoDraw` (was func_800E8A94; O0PTSUB0) - loopTitleScreen: logo bounce by MulNegSin of the decaying amplitude (unless paused), 8-px top bar, logo sprite lists D_800EB17C (bouncing) and D_800EB1AC
- `800e8b94 optionsMenuDraw` (was func_800E8B94; O0PTSUB0) - cursor tile, option text with the sound/vibration values and the frame
- `800e8be0 buttonConfigMenuDraw` (was func_800E8BE0; O0PTSUB0) - cursor tile, button layout text for D_8009F3DC and the frame
- `800e8c24 screenAdjustMenuDraw` (was func_800E8C24; O0PTSUB0) - screen adjust text (with the pressed direction) and the frame
- `800e8c58 titleDrawSprite` (was func_800E8C58; O0PTSUB0) - one SprDef as a semi-transparent SPRT plus its draw mode into OT+4+pri
- `800e8e44 optionsMenuDrawCursor` (was func_800E8E44; O0PTSUB0) - green tile over the selected row from D_800EAFD4[sel] (D_800EB004 when the pad has no actuator and sel >= 4)
- `800e8fd8 buttonConfigDrawCursor` (was func_800E8FD8; O0PTSUB0) - fixed green tile (0x1C,0x4C 0x40x0x18) over the config number
- `800e90d0 optionsMenuDrawText` (was func_800E90D0; O0PTSUB0) - option frame sprites D_800EB1DC and the labels: title 0, rows 1..5 with the sound mode (6+sound) and vibration (9+vib) values
- `800e9254 buttonConfigDrawText` (was func_800E9254; O0PTSUB0) - the six button layouts: config name 0xF..0x13 and the four action labels 0x19..0x1C at the button positions
- `800e950c screenAdjustDrawText` (was func_800E950C; O0PTSUB0) - screen adjust frame D_800EB3BC, labels, the four direction arrows (highlighted while pressed) and the x/y offsets as signed numbers
- `800e96a4 optionsDrawFrame` (was func_800E96A4; O0PTSUB0) - sprite list D_800EB3EC (tpage 1,0,0x180) into OT+8: the frame shared by the three option screens
- `800e972c titleDrawText` (was func_800E972C; O0PTSUB0) - title text entry i (D_800EB040 strings, title font D_800EA1D8, per-glyph palette D_800EB05C) at x/y
- `800e9798 optionsDrawLabel` (was func_800E9798; O0PTSUB0) - options text entry i (D_800EB070, font D_800EA634) with palette 0
- `800e97f4 optionsDrawLabelPal2` (was func_800E97F4; O0PTSUB0) - options text entry i with palette 2 (clut 0x7C15) and glyph palette list D_800EB124
- `800e985c optionsDrawLabelPal` (was func_800E985C; O0PTSUB0) - options text entry i with the palette given (3 = highlighted arrow in screenAdjustDrawText)
- `800e98b4 screenAdjustDrawNumber` (was func_800E98B4; O0PTSUB0) - signed number: +/- glyph D_800EB11C/120 then up to two digits from D_800EB0F4
- `800e99fc titleDrawString` (was func_800E99FC; O0PTSUB0) - renders a MenuItem string (-1 end, -3 newline, -6 skips 3 bytes, 0 = space) as SPRTs from the 5-byte glyph table, clut by pal mode, draw mode after
- `800e9d08 titleDrawMenuCarousel` (was func_800E9D08; O0PTSUB0) - the rotating title menu: each entry of D_800EA5D0 at angle+4*i through titleDrawCarouselGlyph, the front entry flat; then the left/right arrows by sel
- `800e9f5c titleDrawCarouselGlyph` (was func_800E9F5C; O0PTSUB0) - one title glyph as a POLY_FT4 at y 0xB8 whose x edges come from MulNegSin of the angle (flat when off == -1), hidden on the back half
- `800e87bc subScreenApplyPendingEquip` (was func_800E87BC; D0SPSUB) - when D_8009D006 is set: D_8009D2B3 = D_8009D090 and the player anim table / D_800A60FC from D_8007A024 by the pants kind D_8009C990 (slot 4 when D_8009CF06 and no pants 1..3), then clears the flag
- `800e88d4 subScreenApplyPendingGauges` (was func_800E88D4; D0SPSUB) - when D_8009CFFB is set: copies the saved block D_8009D089..D_8009D090 into the three item gauges D_8009CD9C.., the flags D_8009C984 (|0x40 when D_8009D096 or event 0x9A done) and D_8009D2B3, resets the player anim table
- `800e9ab0 subScreenDrawStatusNotes` (was func_800E9AB0; D0SPSUB) - status strings at the bottom: D_801091A8 when flags D_8009C984 & 0x40, D_801091A4[state] of event 0xD, D_801091BC when event 8 and D_801091C0 when event 0x99 are completed
- `800e9cb4 subScreenDrawDigitsSprite` (was func_800E9CB4; D0SPSUB) - the 8 digit sprites of o->num (leading zeros skipped) at x-0x3C.. plus the object's own frame parts as semi-transparent SPRTs (AP counter style)
- `800e9ea0 subScreenDrawTabs` (was func_800E9EA0; D0SPSUB) - the tab row of the sub-screen by mode (slot tables D_8010924C / D_801092C4 / D_80109294, ids D_80109304[mode]): highlighted cursor entry, lit tab of the current mode, the two sprites D_800FE8D0 at the -2 slot
- `800ea168 subScreenDrawEventIcons` (was func_800EA168; D0SPSUB) - the icons of the 10 events of the page (D_801086F0 order) whose flag D_8009CDA4 is 0xFF (completed), special cluts for the medals / keys
- `800ea370 subScreenDrawEventList` (was func_800EA370; D0SPSUB) - the 10 event names of the page (D_801093C4[event], D_801096D0 '???' while the flag is 0; green when completed) and the green selection tile
- `800ea8f4 subScreenDrawItemGrid` (was func_800EA8F4; D0SPSUB) - the 10 item icons of the page from the item list D_8010A0C8 (count D_8009D2A4) through subScreenDrawItemIcon, with the in-use flag of the equipped pants / weapon / pending items
- `800eac9c subScreenDrawItemNames` (was func_800EAC9C; D0SPSUB) - the item names of the page (D_8010979C[D_801096D4[item]], clut from D_80109AF8, white when selected) and 'x NN' counts D_8009D0A4[item] for the items flagged 0x80
- `800eaf28 subScreenDrawPageNumber` (was func_800EAF28; D0SPSUB) - the page footer D_80109C10: page+1 and the count n as digit sprites plus the fixed icons
- `800eb148 subScreenDrawGaugePercent` (was func_800EB148; D0SPSUB) - gauge i of D_80109C40: its icon then (60 - D_8009CD9C[i]) * 100 / 60 as a percentage (100 draws the extra '1')
- `800eb2f4 subScreenDrawSprite` (was func_800EB2F4; D0SPSUB) - the frame D_80111144 of the sprite bank as SPRTs at D_8011113C (clut override by attr, semi-trans by attr bit 15) into OT+4+pri, draw mode D_80111140
- `800eb4e8 subScreenDrawSpriteFT4` (was func_800EB4E8; D0SPSUB) - the frame as POLY_FT4s (tpage D_8011113C.w, clut override by attr); the last part one OT level nearer
- `800eb7a8 subScreenDrawItemIcon` (was func_800EB7A8; D0SPSUB) - item icon n from the 15-column sheet (clut D_80109C88[n]), the in-use overlay frame when flag, and when sel the green selection tile (dark green when the item's condition script D_800FFAF8[D_800FECE8[n]] evaluated through D_800FFBB0 fails)
- `800ebcb8 subScreenDrawString` (was func_800EBCB8; D0SPSUB) - renders the string D_8011114C with the 5-byte glyph font D_80111150 at D_8011113C (-1 end, -3 newline when multiline, -6 skips 3) as SPRTs with clut D_80111142
- `800ebf34 subScreenDrawPopupMenu` (was func_800EBF34; D0SPSUB) - the popup list D_8010A068 (strings D_80108FCC, D_8010A05C rows, cursor tile at D_8010A058) in a balloon frame, plus the second-level list D_80109168 when D_8010A060 >= 0
- `800ec390 subScreenInitMenu` (was func_800EC390; D0SPSUB) - loads the 0x30x0x1F image D_800F18E0 to VRAM (0x120,0x1E0), sets the sprite banks 1F8003A0/A4 and the menu's sprite tables p48..p64, w10 = 1
- `800ec474 subScreenClampTabCursor` (was func_800EC474; D0SPSUB) - moves a tab cursor < 4 to the first valid tab of the mode (same rule as the inline fixCursor of subScreenMoveTabCursor); returns 1 when changed
- `800ec504 subScreenMoveTabCursor` (was func_800EC504; D0SPSUB) - d-pad navigation of the tab cursor (0..3 top row, 4..7 mode tabs, 8 list) per mode with sfx 8; returns 1 when handled
- `800eca28 subScreenMoveGridCursor` (was func_800ECA28; D0SPSUB) - d-pad over a 10-per-page grid: left/right within the row, up/down by 10 (fixed count), updates m->row
- `800ecbf0 subScreenMoveGridCursorN` (was func_800ECBF0; D0SPSUB) - subScreenMoveGridCursor with the entry count n as argument
- `800ecd94 subScreenMoveEventCursor` (was func_800ECD94; D0SPSUB) - grid cursor over the event list, count from subScreenCountEvents rounded up to 10
- `800ecf88 subScreenMainStep` (was func_800ECF88; D0SPSUB) - sub-screen mode 0: tab cursor, cross/circle picks result 1/2/0/3 (tab 7 leaves with 2), triangle leaves with 1
- `800ede64 subScreenOpenMessage` (was func_800EDE64; D0SPSUB) - allocates the message box D_8010A200 in a free slot of D_800A4648 (geometry 0x1A/0xA/0x14.., kind) with the string = entry (kind*2+0x10 of bank b) + add of the text bank D_1F800398[bank]
- `800ee028 subScreenEventListStep` (was func_800EE028; D0SPSUB) - sub-screen mode 2 (events): tab row, then the event grid with L1/R1 paging over subScreenCountEvents, cross opens the event text through subScreenOpenMessage (sub 3 runs the message until D_8010A204 == 2)
- `800ee37c subScreenMapInitMarker` (was func_800EE37C; D0SPSUB) - the player marker sprite D_8010A2D4[0] at the midpoint of the current room's map link (D_800FE064[area][section] -> D_800FDAB4 -> nodes D_800FD418)
- `800ee580 subScreenMapInitAreaSprites` (was func_800EE580; D0SPSUB) - the map sprites D_8010A550 of the 8 areas whose flag D_8009D2C4[i] is 0 from the spawn lists D_8010A02C[i] (anim D_800FECB8[id])
- `800ee6b4 subScreenMapReadInput` (was func_800EE6B4; D0SPSUB) - L1/R1 held: d-pad scrolls the map view D_801110F8/D_80111100 by 2 (returns -1); else the pressed direction as 4 (up) 0 (down) 2 (left) 6 (right), -1 none
- `800ee7b8 subScreenMapScrollToMarker` (was func_800EE7B8; D0SPSUB) - projects the marker and scrolls the view by 2 px when it leaves the 0x30..0x110 x 0x50..0xB0 window, clamped to the bounds D_800FCED0 of the last discovered area
- `800ee9ac subScreenMapStep` (was func_800EE9AC; D0SPSUB) - sub-screen mode 3 (map): saves the camera 1F8000C0, places the cursor / marker / area sprites / two icons D_80110F50, tab row; tab 7 walks the marker along the map links (D_800FDAB4, D_800FDE50) in 4-frame moves by the pressed direction
- `800efaa4 subScreenUpdate` (was func_800EFAA4; D0SPSUB) - the sub-screen entry: state 0 loads the image and sprite tables into the menu D_800A45D8, state 1 d-pad auto-repeat then the mode step (subScreenMainStep / func_800ED0C4 / subScreenEventListStep / subScreenMapStep); returns its result
- `800efd10 subScreenCountEvents` (was func_800EFD10; D0SPSUB) - index+1 of the last entry of the event list D_801086F0 whose flag D_8009CDA3[id] is set
- `800f0930 subScreenMapDrawNode` (was func_800F0930; D0SPSUB) - an 8x8 textured dot (u 0x70 v 0x90, tpage 0x85) centred at the packed screen position into OT+pri-5
- `800f0a5c subScreenMapDrawRoutes` (was func_800F0A5C; D0SPSUB) - for every area with D_8009D2C4[i] set the entries of D_800FDA70[i]: type 0 a node dot at the projected node, type 1 a white F4 strip (shape D_800FE0B4[c]) between two projected nodes
- `800f0e0c subScreenMapDrawAreaName` (was func_800F0E0C; D0SPSUB) - the area name string at (0x10,0xD8): D_80108B78 in area 14, else D_80108BE4[i], overridden for links 0x26/0x27/0x2E (D_80108BCC) and 0xC (D_80108BE0)
- `800f0f20 subScreenMapDrawRoomName` (was func_800F0F20; D0SPSUB) - the room name string at (0x10,0xE8): D_80108DDC[section] in area 14, else D_80108F8C[i][j] (link 0xC: D_80108F94)
- `800f100c subScreenCondFalse` (was func_800F100C; D0SPSUB) - item condition script op: always 0
- `800f1014 subScreenCondTrue` (was func_800F1014; D0SPSUB) - item condition script op: always 1
- `800f101c subScreenCondPlayerH` (was func_800F101C; D0SPSUB) - condition op: compares PLAYER.h->whole with val by op 0..5 (==, !=, <, <=, >, >=)
- `800f1124 subScreenCondPlayerY` (was func_800F1124; D0SPSUB) - condition op: compares PLAYER.y.whole (D_800A604E) with val
- `800f1228 subScreenCondPlayerD` (was func_800F1228; D0SPSUB) - condition op: compares PLAYER.d->whole with val
- `800f1330 subScreenCondArea` (was func_800F1330; D0SPSUB) - condition op: compares GAME.selectedArea with val
- `800f1434 subScreenCondSection` (was func_800F1434; D0SPSUB) - condition op: compares GAME.selectedSection with val
- `800f1538 subScreenCondUnk6Is1` (was func_800F1538; D0SPSUB) - condition op: GAME.unk6 (D_8009C966) == 1
- `800f1550 subScreenCondFlag100` (was func_800F1550; D0SPSUB) - condition op: bit 0x100 of D_8009C984
- `800f1568 subScreenCondEventFlag` (was func_800F1568; D0SPSUB) - condition op: compares the event flag D_8009CDA4[i] with val
- `800f1630 subScreenCondFlag544` (was func_800F1630; D0SPSUB) - condition op: compares the byte D_8009CEA4[i] (GAME+0x544+i) with val
- `800f16f8 subScreenCondHealthSettled` (was func_800F16F8; D0SPSUB) - condition op: GAME.playerHealth == playerHealthDisplayed
- `800f1718 subScreenCondNearType0A` (was func_800F1718; D0SPSUB) - condition op: an active type 0xA object (subtype bit 0x80 clear) of the list 1F800260 within 0x5B x 0xC1 x 0x81 of the player
- `800f182c subScreenCondNoWeapon` (was func_800F182C; D0SPSUB) - condition op: D_8009D2B1 (GAME+0x951) == 0
- `800f183c subScreenCondRefreshingDrinkSpot` (was func_800F183C; D0SPSUB) - condition op: area 16 section 6, event 0x40 started but not completed, and the player object's w2 within 0x64..0xDC
- `800f18a8 subScreenCondFlag40` (was func_800F18A8; D0SPSUB) - condition op: bit 0x40 of D_8009C984
- `800f18c0 subScreenCondVarD0E6Under5` (was func_800F18C0; D0SPSUB) - condition op: D_8009D0E6 (GAME+0x786) < 5
- `800f18d0 subScreenCondVar6102Set` (was func_800F18D0; D0SPSUB) - condition op: D_800A6102 != 0
- `800e82a4 memCardFileListStep` (was func_800E82A4; D0SPSUB2) - save/load file list of the selected card: sub 1 reads the directory (memCardReadDirectory) and sorts the slots, sub 2 browses (messages 3/5/7), save (x1C): overwrite prompt, erase, build the save data (func_800E999C) and write it (memCardWriteFile), load: read the 0xC00 block, checksum (memCardVerifyChecksum), copy into GAME
- `800e90d4 memCardFormatStep` (was func_800E90D4; D0SPSUB2) - format prompt (message 0xD, yes/no cursor D_800F7148), then format() with 3 retries, message 0xC on success / 8 on failure
- `800e9308 memCardScreenStep` (was func_800E9308; D0SPSUB2) - the memory card screen: sub 0 resets both Card records, sub 1 probes the two slots every 0x1E frames (memCardProbe) and picks one, sub 2 runs memCardFileListStep / memCardFormatStep by the card status, subs 3/4 scroll the list (D_800F70F0/F4/F8)
- `800e9888 memCardScreenUpdate` (was func_800E9888; D0SPSUB2) - the D0SPSUB2 entry: d-pad auto-repeat into 1F8001FC then memCardScreenStep in save mode (w12 4, x1C = 1) or load mode (w12 5)
- `800ea0d8 memCardStoreChecksum` (was func_800EA0D8; D0SPSUB2) - xor of the first n-1 bytes stored in the last byte, returned
- `800ea118 memCardVerifyChecksum` (was func_800EA118; D0SPSUB2) - xor of the first n-1 bytes compared with the last byte
- `800ea160 memCardDrawBackground` (was func_800EA160; D0SPSUB2) - the screen's sprite part list D_800F6AD8 (clut 0x80/0x1FF, tpage 1,0,0x80) into OT+4+pri with draw modes
- `800ea37c memCardScreenDraw` (was func_800EA37C; D0SPSUB2) - draws the memory card screen: background, SAVE/LOAD title, the message of the current state (memCardDrawMessage by m->x1A), the slot select prompt, and the file list with icons, names, play time / AP / lives / location per entry and the scroll arrows
- `800ea948 memCardDrawSlotSelect` (was func_800EA948; D0SPSUB2) - the card slot prompt: text D_800F6B58, slot 1 / slot 2 labels D_800F6C18 / D_800F6C38 when present, the cursor tile at the selected one, in a balloon frame
- `800eaa40 memCardDrawParts` (was func_800EAA40; D0SPSUB2) - a -1 terminated sprite part list as SPRTs (clut by GetClut) at an x/y offset into OT+1
- `800eaba4 memCardDrawCursorTile` (was func_800EABA4; D0SPSUB2) - a 0x30 x 0x10 green tile at x/y into OT+1
- `800eac90 memCardDrawFileIcon` (was func_800EAC90; D0SPSUB2) - a file entry's icon frame: parts D_800F6C58 (big 0) or the empty-slot frame D_800F6CA8 with its label string, dimmed (0x50) unless lit, icon clut 0x140/0x1F0+pal
- `800eaf3c memCardDrawFileCursor` (was func_800EAF3C; D0SPSUB2) - the selection highlight parts D_800F6CE8 (clut 0x130/0x1F2) at x/y
- `800eb0ec memCardDrawModeTitle` (was func_800EB0EC; D0SPSUB2) - the 0x28 x 0x10 SAVE (v 0x78) or LOAD (v 0x88) title sprite at x/y
- `800eb25c memCardDrawMessage` (was func_800EB25C; D0SPSUB2) - message n of D_800F6A80 in a balloon frame D_800F65D4[n] (measured by memCardDrawString), with the yes/no cursor tile for messages 7/12/16 (D_800F7148), and the footer D_800F6AC8 at (0x50,0xD4)
- `800eb61c memCardDrawString` (was func_800EB61C; D0SPSUB2) - renders D_800F70D8.str with the 5-byte glyph font (mode 1: -3 newline, mode 2: also wraps at char 0xA8) and measures the text size into 1F8003D4/D6
- `800ec22c memCardDrawNumber2` (was func_800EC22C; D0SPSUB2) - a two-digit number as digit sprites D_800F6D34 (leading zero skipped unless force) into OT+6
- `800ec504 memCardDrawFileLocation` (was func_800EC504; D0SPSUB2) - the location string D_800F6FF0[tbl][idx] (area / room name of a save) at x/y
- `800ec5a4 memCardDrawScrollArrow` (was func_800EC5A4; D0SPSUB2) - the up (0) / down (1) 16x16 arrow sprite D_800F7040[n] at x/y into OT+2
- `800ec77c memCardProbe` (was func_800EC77C; D0SPSUB2) - _card_info then _card_load on the port, waiting on the card events: 0 ok, 1 unformatted / new, -1 no card, -2 error (c->w146 tracks the state)
- `800ec8fc memCardReadDirectory` (was func_800EC8FC; D0SPSUB2) - firstfile/nextfile over the card: the game's saves (D_800F7048 prefix) fill c->slot / names and their header (read at 0x200 by memCardReadFile) into D_800F74B8, other files mark their blocks; the free blocks get the unused icon numbers
- `800eccc8 memCardReadFile` (was func_800ECCC8; D0SPSUB2) - open / lseek / read of len bytes at ofs with 3 retries; 0 on success
- `800ecd94 memCardWriteFile` (was func_800ECD94; D0SPSUB2) - creates the file (blocks << 16 | 0x200) or opens it for writing and writes len bytes, 3 retries; 0 on success
- `800ece68 memCardWaitHwEvent` (was func_800ECE68; D0SPSUB2) - waits for one of the four low-level card events D_8009F10C..118: 0 IOE, 1 error, 2 timeout, 3 new card
- `800eceec memCardWaitSwEvent` (was func_800ECEEC; D0SPSUB2) - waits for one of the four file-system card events D_8009F0F4..100
- `800ecf70 memCardClearHwEvents` (was func_800ECF70; D0SPSUB2) - TestEvent on the four low-level card events (clears them)
- `800ecfc8 memCardClearSwEvents` (was func_800ECFC8; D0SPSUB2) - TestEvent on the four file-system card events (clears them)
- `800ed020 memCardCloseEvents` (was func_800ED020; D0SPSUB2) - CloseEvent on all eight card events inside a critical section
- `800e8714 subScreenApplyPendingEquip` (was func_800E8714; D0SPSUB3) - same code as D0SPSUB subScreenApplyPendingEquip
- `800e882c subScreenApplyPendingGauges` (was func_800E882C; D0SPSUB3) - same code as D0SPSUB subScreenApplyPendingGauges
- `800e9a08 subScreenDrawStatusNotes` (was func_800E9A08; D0SPSUB3) - same code as D0SPSUB subScreenDrawStatusNotes (strings D_800FFD0C..)
- `800e9c0c subScreenDrawDigitsSprite` (was func_800E9C0C; D0SPSUB3) - same code as D0SPSUB subScreenDrawDigitsSprite
- `800e9df8 subScreenDrawTabs` (was func_800E9DF8; D0SPSUB3) - same code as D0SPSUB subScreenDrawTabs (slot tables D_800FFDB0 / D_800FFE28 / D_800FFDF8, ids D_800FFE68)
- `800ea0c0 subScreenDrawEventIcons` (was func_800EA0C0; D0SPSUB3) - same code as D0SPSUB subScreenDrawEventIcons (event list D_800FF254)
- `800ea2c8 subScreenDrawEventList` (was func_800EA2C8; D0SPSUB3) - same code as D0SPSUB subScreenDrawEventList (names D_800FFF28)
- `800ea84c subScreenDrawItemGrid` (was func_800EA84C; D0SPSUB3) - same code as D0SPSUB subScreenDrawItemGrid (item list D_80100988)
- `800eabf4 subScreenDrawItemNames` (was func_800EABF4; D0SPSUB3) - same code as D0SPSUB subScreenDrawItemNames (names D_80100300, cluts D_8010065C)
- `800eae80 subScreenDrawPageNumber` (was func_800EAE80; D0SPSUB3) - same code as D0SPSUB subScreenDrawPageNumber (footer D_80100774)
- `800eb0a0 subScreenDrawGaugePercent` (was func_800EB0A0; D0SPSUB3) - same code as D0SPSUB subScreenDrawGaugePercent
- `800eb24c subScreenDrawSprite` (was func_800EB24C; D0SPSUB3) - same code as D0SPSUB subScreenDrawSprite (state D_80100B70)
- `800eb440 subScreenDrawSpriteFT4` (was func_800EB440; D0SPSUB3) - same code as D0SPSUB subScreenDrawSpriteFT4
- `800eb700 subScreenDrawItemIcon` (was func_800EB700; D0SPSUB3) - same code as D0SPSUB subScreenDrawItemIcon
- `800ebc10 subScreenDrawString` (was func_800EBC10; D0SPSUB3) - same code as D0SPSUB subScreenDrawString
- `800ebe8c subScreenDrawPopupMenu` (was func_800EBE8C; D0SPSUB3) - same code as D0SPSUB subScreenDrawPopupMenu
- `800ec2e8 subScreenInitMenu` (was func_800EC2E8; D0SPSUB3) - loads the image D_800EEA00 to VRAM (0x120,0x1E0), sprite bank 1F8003A0 and the menu's sprite tables p48..p64 (one bank less than the D0SPSUB one)
- `800ec3bc subScreenClampTabCursor` (was func_800EC3BC; D0SPSUB3) - a tab cursor of 3 is moved to 2 (this item-only sub-screen has three top tabs); returns 1 when changed
- `800ec3f0 subScreenMoveTabCursor` (was func_800EC3F0; D0SPSUB3) - up/down navigation of the tab cursor (0..2 top, 4..6, 8 list) with sfx 8; left/right unused here
- `800ec524 subScreenMoveGridCursor` (was func_800EC524; D0SPSUB3) - same code as D0SPSUB subScreenMoveGridCursor
- `800ec6ec subScreenMoveGridCursorN` (was func_800EC6EC; D0SPSUB3) - same code as D0SPSUB subScreenMoveGridCursorN
- `800ed5fc subScreenOpenMessage` (was func_800ED5FC; D0SPSUB3) - same code as D0SPSUB subScreenOpenMessage (message box D_80100A90)
- `800edd28 subScreenUpdate` (was func_800EDD28; D0SPSUB3) - the D0SPSUB3 entry: state 0 inits the menu D_800A45D8 (inline subScreenInitMenu, D_80100918 = -1), state 1 d-pad auto-repeat then the item page step func_800EC89C; returns its result
- `800edf08 subScreenDrawItemPage` (was func_800EDF08; D0SPSUB3) - draws the item sub-screen: tabs, popup menu when D_80100918 >= 0, item grid and names, page number with the L/R page arrows (icons 0x16/0x17) when there are 2+ pages, and the footer sprites D_801007BC
- `800ee12c subScreenCondFalse` (was func_800EE12C; D0SPSUB3) - same code as D0SPSUB subScreenCondFalse
- `800ee134 subScreenCondTrue` (was func_800EE134; D0SPSUB3) - same code as D0SPSUB subScreenCondTrue
- `800ee13c subScreenCondPlayerH` (was func_800EE13C; D0SPSUB3) - same code as D0SPSUB subScreenCondPlayerH
- `800ee244 subScreenCondPlayerY` (was func_800EE244; D0SPSUB3) - same code as D0SPSUB subScreenCondPlayerY
- `800ee348 subScreenCondPlayerD` (was func_800EE348; D0SPSUB3) - same code as D0SPSUB subScreenCondPlayerD
- `800ee450 subScreenCondArea` (was func_800EE450; D0SPSUB3) - same code as D0SPSUB subScreenCondArea
- `800ee554 subScreenCondSection` (was func_800EE554; D0SPSUB3) - same code as D0SPSUB subScreenCondSection
- `800ee658 subScreenCondUnk6Is1` (was func_800EE658; D0SPSUB3) - same code as D0SPSUB subScreenCondUnk6Is1
- `800ee670 subScreenCondFlag100` (was func_800EE670; D0SPSUB3) - same code as D0SPSUB subScreenCondFlag100
- `800ee688 subScreenCondEventFlag` (was func_800EE688; D0SPSUB3) - same code as D0SPSUB subScreenCondEventFlag
- `800ee750 subScreenCondFlag544` (was func_800EE750; D0SPSUB3) - same code as D0SPSUB subScreenCondFlag544
- `800ee818 subScreenCondHealthSettled` (was func_800EE818; D0SPSUB3) - same code as D0SPSUB subScreenCondHealthSettled
- `800ee838 subScreenCondNearType0A` (was func_800EE838; D0SPSUB3) - same code as D0SPSUB subScreenCondNearType0A
- `800ee94c subScreenCondNoWeapon` (was func_800EE94C; D0SPSUB3) - same code as D0SPSUB subScreenCondNoWeapon
- `800ee95c subScreenCondRefreshingDrinkSpot` (was func_800EE95C; D0SPSUB3) - same code as D0SPSUB subScreenCondRefreshingDrinkSpot
- `800ee9c8 subScreenCondFlag40` (was func_800EE9C8; D0SPSUB3) - same code as D0SPSUB subScreenCondFlag40
- `800ee9e0 subScreenCondVarD0E6Under5` (was func_800EE9E0; D0SPSUB3) - same code as D0SPSUB subScreenCondVarD0E6Under5
- `800ee9f0 subScreenCondVar6102Set` (was func_800EE9F0; D0SPSUB3) - same code as D0SPSUB subScreenCondVar6102Set
- `800e8214 overheadSectionStep` (was func_800E8214; I0NFO) - section object of the top-down AREA02 section 0 (I0NFO is its overlay): step 0 waits D_8009C93A, step 1 the two edge exits by player position (tryStartItemInteraction) then the area handler D_80079AD0[subtype], step 2 cameraEventScrollHandler
- `800e8348 overheadNpcSetScriptAnim` (was func_800E8348; I0NFO) - game opcode target: anim = D_800EE8A4[subtype][0][w74] and AnimJump(w76) on the NPC
- `800e8398 overheadNpcUpdateStep` (was func_800E8398; I0NFO) - NPC steps 0..5: cull register (FUN_80020078 with 0x6C for the NPC b0c 0x23) and AnimAdvance when visible
- `800e8420 overheadNpcHandler` (was func_800E8420; I0NFO) - top-down NPC object: init from D_800EE89C[subtype] (anim by w74, AnimJump w76, clut 0x7F88 for b0c 0x1C/0x22 or 0x23/0x29 once event 0x50 is done), update through overheadNpcUpdateStep, ObjFree at b04 3
- `800e8644 overheadPlayerInit` (was func_800E8644; I0NFO) - player init in the top-down section: pose object D_8009C330 at o+0xF8, equips from D_8007A024, 8-direction anim D_800106B4, spawns the shadow (overheadSpawnPlayerShadow) and the follower when D_8009CEAB == 3, playerState 3 / 4 entries
- `800e88a0 overheadPlayerStatusAnim` (was func_800E88A0; I0NFO) - player anim by the status D_8009D00F: 1/5 random blink anim D_800106F0[d], 2/3 D_8001070C[d], 4 D_80010700[d] (d from the facing direction)
- `800e8a98 overheadPlayerSetDirAnim` (was func_800E8A98; I0NFO) - player anim by mode (0 idle D_800106B4.., 1 walk D_800106C8.., 2 run D_800106DC..) and 8-direction dir
- `800e8d60 overheadPlayerSetMoveAnim` (was func_800E8D60; I0NFO) - player anim from animFrame 0..15 (0..7 moving, 8..15 standing): run anims while the dash button 1F8003C4 is held, walk anims otherwise; records the anim id in the pose object
- `800e9024 overheadPlayerWalkStep` (was func_800E9024; I0NFO) - step 1: moves in the facing direction (speed 0x280 running / 0x180 walking) with overheadCollideMapTile, back to step 0 when standing, talk button sets wac 1
- `800e9188 overheadPlayerItemUseStep` (was func_800E9188; I0NFO) - step 0x30: the item used D_8009C940[1]: 0xF/0x11 removed with an event counter, 9 clears D_8009D2B1, 5/0x7C/0x97/0x98/7 start the drop-exit (b04 4 step 4), 0xE the fall state, others cleared
- `800e947c overheadPlayerDropExitStep` (was func_800E947C; I0NFO) - b04 4 step 4: the player falls (velY -= 0x30) for 0x1E frames with sfx 0x16, then fade D_8009C975 = 3 and after 0x3C frames the task continues with w4c 7
- `800e962c overheadPlayerLandStep` (was func_800E962C; I0NFO) - b04 4 step 5: loads the spawn position and drops the player from above until y reaches F2 (sfx 0x1C), then normal control; with D_8009CFF9 spawns a type 0x54 object instead
- `800e9870 overheadPlayerMainStep` (was func_800E9870; I0NFO) - b04 1: step 0 idle with the d-pad direction (setDir), 1 walk, 2 talking, 0x30 item use, 0x63 wait for the fade
- `800e9cd8 overheadPlayerEventStep` (was func_800E9CD8; I0NFO) - b04 4 (scripted): 0 idle, 1 timed stand, 2 timed walk with map collision, 3 stand, 4 drop exit, 5 land, 6 show
- `800e9f60 overheadPlayerHandler` (was func_800E9F60; I0NFO) - the top-down player handler: swaps the pad buffer, equips by subtype, playerDecodePadInput, b04 0 init / 1 main / 4 event steps, stores the position in SPAD 0x16A..0x172
- `800ea124 overheadDrawRotatedSprite` (was func_800EA124; I0NFO) - draws the object's frame parts as FT4 quads rotated by d8c (D_8007A3F0/5F0 tables), mirrored for directions 3..5, uploads the frame to VRAM when it changed (uploadSpriteFrameToVram), insertPrimWithBias
- `800eab58 overheadDrawObjectSprite` (was func_800EAB58; I0NFO) - draws the frame parts as FT4s by category: 2/9 flipped when animFrame == 2, 3 with the clut w08 (dark when subtype & 1); brightness b6b, culled to the screen, linked at ot[b0f]
- `800eb044 overheadDrawMapLayers` (was func_800EB044; I0NFO) - the 5 map SPRTs D_800EE8D0 (cluts / tpages D_800A4668) scrolled by -x/-y, brightness b6b
- `800eb384 overheadLoadMapTiles` (was func_800EB384; I0NFO) - copies the 0x4000-byte tile map D_800EE8C8 (size D_800A4674/76) from the map data 1F800394, sets the three tpages / cluts D_800A4668..
- `800eb4c8 overheadBuildMapStrip` (was func_800EB4C8; I0NFO) - copies the 32-px tile columns of the visible tile list D_800B1320[idx] (type 0: 2 columns x 0x100 rows, 1: 1 x 0x100, 2..4: 1 x 0x20) from the map image D_800A4680 into d
- `800ebc30 overheadSortDrawOrder` (was func_800EBC30; I0NFO) - collects the NPC types 0x19/0x25/0x26 (list 21C) and 0x2E/0x1D (list 220) plus the player, sorts them by y and assigns b0f = index - 10 (shadows +1)
- `800ebedc overheadBuildVisibleTileList` (was func_800EBEDC; I0NFO) - fills D_800B1320 with 10 rows x 12 tile indices around the scroll d30/d34 >> 5
- `800ec084 overheadRefreshMap` (was func_800EC084; I0NFO) - map object update: position from the scroll 1F800176/17A, visible tiles, the 5 strips uploaded to VRAM (0x300,0 / 0x380,0 / 0x40,0..0x40), brightness by D_800A60A3
- `800ec234 overheadMapObjHandler` (was func_800EC234; I0NFO) - the map object: b04 0 overheadLoadMapTiles and init (tpage by GetGraphType), 1 refresh and ObjListPush_1F80022C, 3 freeObjectLayer7
- `800ec358 overheadResolveNpcContact` (was func_800EC358; I0NFO) - box overlap of a with b: pushes a out on the smaller axis, b69 of both = contact side; with wac 1 and facing the side marks b as the talk target (wac 2, b->b68 = 1, d90 = b)
- `800ec5e8 overheadPlayerNpcCollision` (was func_800EC5E8; I0NFO) - sets D_800A60A4.. from D_800EE8C0[player type], then overheadResolveNpcContact against the active types 0x19/0x25/0x26 of list 21C
- `800ec75c overheadResolveCornerTile` (was func_800EC75C; I0NFO) - tile kind 9: moves x/y to the neighbouring 8-px cell by the quadrant inside the tile and the move flag, results in 1F800070/74
- `800ec88c overheadCollideMapTile` (was func_800EC88C; I0NFO) - tile map cell at ((x+0x80)>>3, (y-0x30)>>3): 0 free, 1..4 diagonal walls, 5..8 straight walls, 9 corner; pushes the object out (x or y by f & 2); returns 1 on a wall
- `800ecb40 overheadCheckTileTrigger` (was func_800ECB40; I0NFO) - b68 = bit 0x80 of the tile at x/y (trigger tiles)
- `800ecbbc overheadActorSetScriptAnim` (was func_800ECBBC; I0NFO) - game opcode target for type 0x19 actors: anim = D_800EE914[subtype][0][w74], AnimJump(w76)
- `800ecc2c overheadActorAdvanceAndCollide` (was func_800ECC2C; I0NFO) - AnimAdvance then overheadCollideMapTile at the actor's position
- `800ecc70 overheadActorWaitStep` (was func_800ECC70; I0NFO) - stands still for timer frames (velocities cleared) while no script runs
- `800ecd14 overheadActorWalkStep` (was func_800ECD14; I0NFO) - walks in the direction animFrame at velX (MulCosDup/MulNegSin) with map collision for timer frames, stops moving while the contact b69 faces the direction
- `800ece34 overheadActorTalkStep` (was func_800ECE34; I0NFO) - opens the message box w98/w9a over the actor for timer frames (showMessageBoxDirectTimed), clears D_8009C93F at the end
- `800ecf18 overheadActorWaitMessageStep` (was func_800ECF18; I0NFO) - waits for the message object d90 to reach b04 2, then releases the script flags D_8009C942/C93F and the talk state
- `800ecfd8 overheadActorWalkStepB` (was func_800ECFD8; I0NFO) - same as overheadActorWalkStep (used by step 5)
- `800ed0f8 overheadActorHide` (was func_800ED0F8; I0NFO) - visible = 0
- `800ed100 overheadActorWalkToStep` (was func_800ED100; I0NFO) - walks at 0x100 in the direction animFrame & 3 until the target d84 / d88 is reached
- `800ed2d0 overheadActorStep` (was func_800ED2D0; I0NFO) - type 0x19 actor steps: 0 idle, 1 wait, 2 walk, 3 talk, 4 wait message, 5 walk B, 6 hidden, 9 advance and collide; cull register or ObjListPush_1F80021C by playerState 3
- `800ed7b0 overheadActorInit` (was func_800ED7B0; I0NFO) - box / tpage / sprite bank / anim (by w74, AnimJump w76) from D_800EE90C[subtype], clut 0x7809, category |= 0x80
- `800ed910 overheadActorInitSimple` (was func_800ED910; I0NFO) - box / tpage / bank and the first anim of D_800EE90C[subtype] (subtypes 1 and 3)
- `800eda48 overheadShadowFollowOwner` (was func_800EDA48; I0NFO) - subtype 1 (shadow): position and visibility copied from the owner d90, b0f 0x10
- `800edad4 overheadFollowerStep` (was func_800EDAD4; I0NFO) - subtype 3: anim D_800EE914[subtype][0][0xD2/0xD3] placed beside the player by its facing direction, drawn in front or behind; released when the player state is 3 or D_8009CEDB == 1
- `800ede4c overheadActorHandler` (was func_800EDE4C; I0NFO) - type 0x19 object: b04 0 init (simple for subtypes 1/3), 1 shadow follow / follower / actor step by subtype, 3 freeObjectLayer2
- `800edfb8 overheadSpawnPlayerShadow` (was func_800EDFB8; I0NFO) - allocates a layer-2 type 0x19 object of subtype sub (box 1/2) at o, linked through d90 / d94
- `800ee07c overheadSpawnFollower` (was func_800EE07C; I0NFO) - allocates a layer-2 type 0x19 subtype 3 object at o
- `800ee0f0 overheadItemSpotCheckStep` (was func_800EE0F0; I0NFO) - on contact (b68): checks the required item by D_800EE994[b0c] (D_8009D0AA / D0C8 / D0D0 / D0D1 counts); when held marks D_8009D064[b0c] = 2 and addItemToInventory(7,1,1), else printInfoMessage(0, 2)
- `800ee26c overheadItemSpotUpdate` (was func_800EE26C; I0NFO) - visible while the player is inside the rect D_800EE93C[b0c] (or by D_800A60A3): step 0 check, 1 collected anim D_800F53D0 and the linked object's visibility, 2 done; brightness rules while the player state is 3
- `800ee4c0 overheadItemSpotHandler` (was func_800EE4C0; I0NFO) - item spot object: b04 0 init (box 0x10/0x20/0xD/0x1D, anim D_800F53C0, step from D_8009D064[b0c]), 1 update, 2 taken anim, 3 freeObjectLayer2
- `800ee5e4 overheadTalkSpotStep` (was func_800EE5E4; I0NFO) - on contact with D_800A60A1 == 2 and the player facing 2: opens showMessageBox(1,1,0xA0,0x8C), playerState 2 and the script flags, waits for the message then releases them
- `800ee79c overheadTalkSpotHandler` (was func_800EE79C; I0NFO) - talk spot object: b04 0 init (tpage 0x15, box 4/8/8/0x20, anim D_8001219C), 1 cull register and overheadTalkSpotStep, 3 freeObjectLayer2
- `800e847c tunnelClampPlayerToGates` (was func_800E847C; I0NFO3) - the one-way slopes of the tunnel network (AREA02 section 3, I0NFO3 is its overlay): while events 0x7D / 0x80 are not completed and 0x90 not started, the player is pushed back at x 0x3B6..0x3E9, 0x30C.., 0x2FC..0x36F, 0x510..0x5AB (docs/PORT.md)
- `800e86ac tunnelSectionStep` (was func_800E86AC; I0NFO3) - section object of AREA02 section 3: step 0 waits D_8009C93A (cameraModeHandler6B), step 1 tunnelClampPlayerToGates, the area handler D_80079AD0[subtype] and the five position exits (exit 2 sets D_8009CFED), step 2 cameraEventScrollHandler
- `800e88e4 tunnelSelectBackdropPieces` (was func_800E88E4; I0NFO3) - fills the pointer list with the backdrop pieces of section 3: entries 0x5C / 0x60 by D_800A60A3, else the 8 strips around the scroll (1F800176 - 0x78) / 80 of the 22
- `800e8a14 tunnelGateStepEvent80` (was func_800E8A14; I0NFO3) - gate subtype 2: once event 0x80 is completed and the player is within 0x40 x 0x60, rotates d8c by 0x40 for 16 frames (the linked half b0c != 0 follows the owner's step)
- `800e8b58 tunnelGateStepOpen` (was func_800E8B58; I0NFO3) - gate subtype 5: opens (d8c rotation for 16 frames) as soon as the player is near; no event gate
- `800e8c60 tunnelGateStepEvent7D` (was func_800E8C60; I0NFO3) - gate subtype 6: as tunnelGateStepEvent80 once event 0x7D is completed
- `800e8da4 tunnelGateHandler` (was func_800E8DA4; I0NFO3) - the tunnel gate object: b04 0 init (d8c = animFrame << 9), 1 cull register and the step by subtype 2 / 5 / 6 while D_800A60A3 == 0, 3 ObjFreeDup
- `800e8ebc tunnelEvent90BlockHandler` (was func_800E8EBC; I0NFO3) - object that exists only while event 0x90 is not started (b04 3 at init otherwise): cull register, ObjFreeDup
- `800e8f70 tunnelStaticObjHandler` (was func_800E8F70; I0NFO3) - static object: active 2, cull register while D_800A60A3 == 0, ObjFreeDup
- `800e900c tunnelPlayerInit` (was func_800E900C; I0NFO3) - player init in the tunnels: pose object D_8009C330 at o+0xF8, equips, facing from D_800EECE8[D_8009C982] (4 when a spawn point is set), 8-direction anim D_800106B4, step 0x63, or the event entry (b04 4 step 5) when playerState is 3
- `800e9214 tunnelPlayerStatusAnim` (was func_800E9214; I0NFO3) - same code as I0NFO overheadPlayerStatusAnim
- `800e940c tunnelPlayerSetDirAnim` (was func_800E940C; I0NFO3) - same code as I0NFO overheadPlayerSetDirAnim
- `800e96d4 tunnelPlayerSetMoveAnim` (was func_800E96D4; I0NFO3) - same code as I0NFO overheadPlayerSetMoveAnim
- `800e9998 tunnelPlayerWalkStep` (was func_800E9998; I0NFO3) - step 1: moves in the facing direction (0x280 running / 0x180 walking) with tunnelCollideMapTile; with event 0x90 started and 0x91 not completed, entering x 0x546..0x5AA / 0x334..0x37A completes event 0x91
- `800e9bac tunnelPlayerItemUseStep` (was func_800E9BAC; I0NFO3) - step 0x30: the item used D_8009C941 (0xF / 0x11 removed, 9 / 0x9A clear flags, 5 / 0x7C / 0x97 / 0x98 / 7 drop exit, 0x95 removed with message 0x27 and event 0x7D completed, 0xE fall state)
- `800e9e68 tunnelPlayerDropExitStep` (was func_800E9E68; I0NFO3) - same as I0NFO overheadPlayerDropExitStep (falls, fade, task w4c 7)
- `800ea010 tunnelPlayerLandStep` (was func_800EA010; I0NFO3) - same as I0NFO overheadPlayerLandStep (drops from above to F2, sfx 0x1C)
- `800ea258 tunnelPlayerFallExitStep` (was func_800EA258; I0NFO3) - (no caller) variant of tunnelPlayerDropExitStep without the music stop / sfx: falls 0x1E frames, fade, task w4c 7
- `800ea3bc tunnelPlayerEventStep` (was func_800EA3BC; I0NFO3) - same as I0NFO overheadPlayerEventStep (b04 4 scripted states 0..7)
- `800ea644 tunnelPlayerMainStep` (was func_800EA644; I0NFO3) - b04 1: step 0 idle with the d-pad direction, 1 walk, 2 talking, 0x30 item use, 0x63 wait for the fade; then tunnelCheckTileTrigger and on a trigger tile b6b = 1 + the index of the rect of D_800EECC8 that contains the player
- `800eab84 tunnelPlayerHandler` (was func_800EAB84; I0NFO3) - same as I0NFO overheadPlayerHandler (pad swap, equips, b04 dispatch, position to SPAD 0x16A..)
- `800ead48 tunnelDrawRotatedSprite` (was func_800EAD48; I0NFO3) - same code as I0NFO overheadDrawRotatedSprite
- `800eb77c tunnelDrawObjectSprite` (was func_800EB77C; I0NFO3) - as I0NFO overheadDrawObjectSprite (FT4 parts by category, brightness b6b) with the clut applied only when b0d is set and a 0xF1 bottom cull
- `800ebc5c tunnelSortDrawOrder` (was func_800EBC5C; I0NFO3) - collects the types 0x44 / 0x45 / 0x4C of list 21C with layer byte 0 (1 -> b0f -0xC, 2 -> -1) plus the player, sorts by y, b0f = index - 10 (linked objects +1)
- `800ebecc tunnelMapObjHandler` (was func_800EBECC; I0NFO3) - map object: b04 0 copies the 0x9FF0-byte tile map from 1F800390 (size D_800A4670/72) to 0x80124000, 3 freeObjectLayer7
- `800ebfd8 tunnelResolveNpcContact` (was func_800EBFD8; I0NFO3) - as I0NFO overheadResolveNpcContact; a type 0x45 actor gets b0f -0xC / -3 when the player is above / below it
- `800ec26c tunnelPlayerNpcCollision` (was func_800EC26C; I0NFO3) - as I0NFO overheadPlayerNpcCollision for the types 0x44 / 0x45 / 0x4C (box table D_800EECC0)
- `800ec3dc tunnelResolveCornerTile` (was func_800EC3DC; I0NFO3) - same code as I0NFO overheadResolveCornerTile
- `800ec50c tunnelCollideMapTile` (was func_800EC50C; I0NFO3) - as I0NFO overheadCollideMapTile on the map at 0x80124000, cell (x >> 3, (y + 0x540) >> 3)
- `800ec7ac tunnelCheckTileTrigger` (was func_800EC7AC; I0NFO3) - b68 = bit 0x80 of the tile at x / y
- `800ec820 tunnelItemSpotCheckStep` (was func_800EC820; I0NFO3) - on contact: the required item by b0c (D_8009D0AA / D0C8 / D0D0 / D0D1); subtype 7 gives message 0x4E, 300000 AP and event 0xA1, others addItemToInventory(D_800EECFC[subtype]); setAreaFlagBit(b6b), step 2
- `800ec9d4 tunnelItemSpotHandler` (was func_800EC9D4; I0NFO3) - item spot object: b04 0 init (box 0x10/0x20/0xD/0x1D, anim D_800F13D0[b0c][0]), 1 cull + check step / taken anim [4], hidden while D_800A60A3, 3 freeObjectLayer2
- `800ecb88 tunnelActorSetScriptAnim` (was func_800ECB88; I0NFO3) - (no caller) anim D_800EED64[subtype][0][f], AnimJump(n), D2 = f
- `800ecbfc tunnelActorIdleStep` (was func_800ECBFC; I0NFO3) - (no caller) anim D_800EED64[subtype][0][animFrame] then AnimAdvance
- `800ecc9c tunnelActorPatrolVStep` (was func_800ECC9C; I0NFO3) - patrol kind 1: moves vertically by +-0x80 (facing bit 2) between y 76..7A, turns at the bounds (anim 8), stops while the contact b69 faces the move
- `800ece94 tunnelActorPatrolHStep` (was func_800ECE94; I0NFO3) - patrol kind 2: horizontal between x 74..78
- `800ed088 tunnelActorPatrolDiagStepA` (was func_800ED088; I0NFO3) - patrol kind 3: both axes, turns at the y bounds
- `800ed2bc tunnelActorPatrolDiagStepB` (was func_800ED2BC; I0NFO3) - patrol kind 4: both axes (x mirrored), turns at the x bounds
- `800ed4ec tunnelActorOpenDialogue` (was func_800ED4EC; I0NFO3) - opens the dialogue (6, n) of the actor by its id wba 0..10 and the event flags 0x7D / 0x91 / 0xA1 / 0x92 and the AP (showMessageBoxDirect), marking the event to complete in wac; returns 1 when a message was opened
- `800ed7a8 tunnelActorTalkStep` (was func_800ED7A8; I0NFO3) - faces the player (anim b69 + 3), tunnelActorOpenDialogue, waits for the message; completes the event wac (setEventStarted) or event 0x92, the first time increaseMaxHealth with message 0x15 (D_8009CFEC) and a follow-up message (5, 3)
- `800edb20 tunnelActorStep` (was func_800EDB20; I0NFO3) - step 0 by the movement kind B8 (0 idle, 1..4 patrols), contact starts the talk step 1; brightness by the zone B6 against D_800A60A3
- `800edce4 tunnelActorInit` (was func_800EDCE4; I0NFO3) - box / tpage / bank / anim (by B4) from D_800EED5C[subtype], brightness 0x7F, category |= 0x80; freed (b04 3) when event 0x90 is started and BA is 4..5
- `800ede78 tunnelActorHandler` (was func_800EDE78; I0NFO3) - the type 0x45 actor: b04 0 init, 1 cull register and tunnelActorStep (b0f -4), 3 freeObjectLayer2
- `800edf28 tunnelTalkSpotStep` (was func_800EDF28; I0NFO3) - as I0NFO overheadTalkSpotStep with the message D_800EED80[subtype] and the zone brightness rule
- `800ee140 tunnelTalkSpotHandler` (was func_800EE140; I0NFO3) - talk spot object: init (tpage 0x15, box 4/8/8/0x20, anim D_8001219C[D_800EED84[subtype]]), cull + step, freeObjectLayer2
- `800ee260 tunnelSpawnActors` (was func_800EE260; I0NFO3) - allocates a type 0x45 actor per row of D_800EEF30 (position, facing, zone, patrol kind and bounds, subtype) into D_8009F2D8[i], skipping BA 4..5 once event 0x90 is started
- `800ee414 tunnelUseItem95Step` (was func_800EE414; I0NFO3) - controller step 1: when item 0x95 is used, consumes it, message (6, 0), then completes event 0x7D and pauses 0xC8 frames
- `800ee5a4 tunnelStartEvent4FStep` (was func_800EE5A4; I0NFO3) - (no caller) completes event 0x4F with a 0xC8-frame script pause (inlined as step 2 of tunnelControllerStep)
- `800ee69c tunnelSlidePlayerStep` (was func_800EE69C; I0NFO3) - controller step 3: moves the player 1 px/frame toward D_800A6126 / D_800A612A (directions D_800A60B4 / B6) with playerState 4, then restores control
- `800ee8cc tunnelCheckSlideZone` (was func_800EE8CC; I0NFO3) - the player inside one of the four zones D_800EEDB0 (y -0x4B0..-0x384, x bands) sets the slide target D_800A6126 / 612A and directions; returns 1
- `800eea0c tunnelControllerPickStep` (was func_800EEA0C; I0NFO3) - (no caller) step 0 of tunnelControllerStep as a function: slide zone -> 3, event 0x4F not completed -> 2, event 0x7D started -> 1
- `800eea70 tunnelControllerStep` (was func_800EEA70; I0NFO3) - section controller: step 0 picks (slide zone / event 0x4F / event 0x7D), 1 tunnelUseItem95Step, 2 completes event 0x4F with a pause, 3 tunnelSlidePlayerStep
- `800eec18 tunnelControllerHandler` (was func_800EEC18; I0NFO3) - section controller object: b04 0 tunnelSpawnActors, 1 tunnelControllerStep, 3 PoolFree_1F800210
- `800e8b28 topdownSectionStepA` (was func_800E8B28; I0NFO2) - section object of a top-down section loaded with I0NFO2 (AREA05 / 0x13:2 rooms): step 0 waits D_8009C93A, step 1 three position exits then the area handler D_80079AD0[subtype], step 2 cameraEventScrollHandler
- `800e8c98 topdownSectionStepB` (was func_800E8C98; I0NFO2) - as topdownSectionStepA with two exits by y (> 0x1E8 / < 0x50)
- `800e8d98 topdownSectionStepC` (was func_800E8D98; I0NFO2) - as topdownSectionStepA with two position exits
- `800e8ed4 topdownSectionStepD` (was func_800E8ED4; I0NFO2) - as topdownSectionStepA with three position exits
- `800e9040 topdownSectionStepE` (was func_800E9040; I0NFO2) - as topdownSectionStepA with two exits by y (> 0x23A / < 0x50) inside x bands
- `800e9174 topdownSectionStepBySection` (was func_800E9174; I0NFO2) - as topdownSectionStepA with the exits chosen by the section D_8009C962 (0: two, 2: one)
- `800e9300 topdownShadowSetAnim` (was func_800E9300; I0NFO2) - (no caller) anim = D_800FEBD8[sub][0][i], AnimJump(n)
- `800e9358 topdownShadowInit` (was func_800E9358; I0NFO2) - (no caller) w1e / bank / anim [32] of D_800FEBD4[subtype], b04++ (inlined in topdownShadowHandler)
- `800e9410 topdownShadowFollowOwner` (was func_800E9410; I0NFO2) - (no caller) as I0NFO overheadShadowFollowOwner (inlined in topdownShadowHandler)
- `800e94ac topdownShadowHandler` (was func_800E94AC; I0NFO2) - type 0x3A object subtype 0 (player shadow): init from D_800FEBD4, follows the owner d90 (position / visibility, b0f 0x10), freeObjectLayer2 at b04 3
- `800e964c topdownSpawnPlayerShadow` (was func_800E964C; I0NFO2) - allocates the type 0x3A shadow object of o (d90 = o, b0f + 1)
- `800e96e8 topdownPlayerSetDirAnim` (was func_800E96E8; I0NFO2) - same code as I0NFO overheadPlayerSetDirAnim
- `800e99b0 topdownPlayerInit` (was func_800E99B0; I0NFO2) - player init in the I0NFO2 top-down sections (as I0NFO3 tunnelPlayerInit), spawns the shadow
- `800e9c7c topdownPlayerStatusAnim` (was func_800E9C7C; I0NFO2) - as I0NFO overheadPlayerStatusAnim
- `800e9e84 topdownPlayerSetMoveAnim` (was func_800E9E84; I0NFO2) - same code as I0NFO overheadPlayerSetMoveAnim
- `800ea148 topdownPlayerSetDirAnimCached` (was func_800EA148; I0NFO2) - (no caller) topdownPlayerSetDirAnim keyed by the pose object w2c / w2e (dir + mode * 8)
- `800ea44c topdownPlayerReadDpadDir` (was func_800EA44C; I0NFO2) - animFrame = 8-direction from the d-pad D_8009D670 (bit 8 set when no direction) while wac < 2
- `800ea538 topdownPlayerWalkStep` (was func_800EA538; I0NFO2) - as I0NFO overheadPlayerWalkStep (topdownCollideMapTile)
- `800ea69c topdownPlayerItemUseStep` (was func_800EA69C; I0NFO2) - step 0x30: the item used D_8009C941 (0xF / 0x11 / 9 / 0x9A / 0x1E-0x1F / 0x23 ... with area / position checks), as I0NFO3 tunnelPlayerItemUseStep
- `800eaaa0 topdownPlayerDropExitStep` (was func_800EAAA0; I0NFO2) - as I0NFO overheadPlayerDropExitStep
- `800eac60 topdownPlayerLandStep` (was func_800EAC60; I0NFO2) - as I0NFO3 tunnelPlayerLandStep
- `800eaea8 topdownPlayerMainStep` (was func_800EAEA8; I0NFO2) - b04 1: step 0 idle with the d-pad direction, 1 walk, 2 talking, item use, wait for the fade (as I0NFO3 tunnelPlayerMainStep)
- `800eb2c8 topdownPlayerEventStep` (was func_800EB2C8; I0NFO2) - as I0NFO3 tunnelPlayerEventStep (b04 4 scripted states)
- `800eb550 topdownPlayerHandler` (was func_800EB550; I0NFO2) - as I0NFO3 tunnelPlayerHandler
- `800eb714 topdownDrawRotatedSprite` (was func_800EB714; I0NFO2) - same code as I0NFO overheadDrawRotatedSprite
- `800ec148 topdownDrawObjectSprite` (was func_800EC148; I0NFO2) - as I0NFO overheadDrawObjectSprite
- `800ec710 topdownDrawMapLayers` (was func_800EC710; I0NFO2) - as I0NFO overheadDrawMapLayers (called from MAIN0 drawCategory7Objects by address)
- `800eca74 topdownLoadMapTiles` (was func_800ECA74; I0NFO2) - as I0NFO overheadLoadMapTiles; the third tpage by area 5 / area 0x13 section 2
- `800ecc18 topdownBuildMapStrip` (was func_800ECC18; I0NFO2) - same code as I0NFO overheadBuildMapStrip
- `800ed380 topdownUploadMapStrip` (was func_800ED380; I0NFO2) - builds strip i (topdownBuildMapStrip) and loads it to VRAM (0x300 / 0x380 columns, the third page by area)
- `800ed4d0 topdownSortDrawOrder` (was func_800ED4D0; I0NFO2) - as I0NFO overheadSortDrawOrder for the types 0x2D / 0x2E / 0x30 (layer byte 1 -> b0f -0xC, 2 -> +0xC) of list 21C and list 220
- `800ed7ec topdownBuildVisibleTileList` (was func_800ED7EC; I0NFO2) - same code as I0NFO overheadBuildVisibleTileList
- `800ed994 topdownRefreshMap` (was func_800ED994; I0NFO2) - (no caller) as I0NFO overheadRefreshMap (inlined in topdownMapObjHandler)
- `800eda90 topdownMapObjHandler` (was func_800EDA90; I0NFO2) - as I0NFO overheadMapObjHandler
- `800edc88 topdownResolveNpcContact` (was func_800EDC88; I0NFO2) - as I0NFO overheadResolveNpcContact
- `800edeec topdownPlayerNpcCollision` (was func_800EDEEC; I0NFO2) - as I0NFO3 tunnelPlayerNpcCollision
- `800ee05c topdownResolveCornerTile` (was func_800EE05C; I0NFO2) - same code as I0NFO overheadResolveCornerTile
- `800ee18c topdownCollideMapTile` (was func_800EE18C; I0NFO2) - as I0NFO overheadCollideMapTile
- `800ee440 topdownCheckTileTrigger` (was func_800EE440; I0NFO2) - b68 = bit 0x80 of the tile at x / y
- `800ee4bc topdownItemSpotCheckStep` (was func_800EE4BC; I0NFO2) - as I0NFO overheadItemSpotCheckStep
- `800ee654 topdownItemSpotUpdate` (was func_800EE654; I0NFO2) - item spot b04 1: step 0 check, 2 taken anim D_801073F4; brightness by the zone wb6 against D_800A60A3
- `800ee748 topdownItemSpotHandler` (was func_800EE748; I0NFO2) - item spot object: b04 0 init (box 0x10/0x20/0xD/0x1D, anim D_801073E4, step D_8009D064[b0c]), 1 update, 2 taken anim, 3 freeObjectLayer2
- `800ee8b0 topdownTalkSpotStep` (was func_800EE8B0; I0NFO2) - same code as I0NFO3 tunnelTalkSpotStep
- `800eeac8 topdownTalkSpotHandler` (was func_800EEAC8; I0NFO2) - as I0NFO3 tunnelTalkSpotHandler
- `800eebe8 topdownActorSetAnim` (was func_800EEBE8; I0NFO2) - anim = D_800FED84[subtype][0][i], AnimJump(n) (used by the scripted scene steps)
- `800eec3c topdownActorSetDirAnim` (was func_800EEC3C; I0NFO2) - anim D_800FED84[subtype][0][animFrame + i] when it differs from D2, AnimJump(n)
- `800eecc4 topdownActorIdleStep` (was func_800EECC4; I0NFO2) - (no caller) as I0NFO3 tunnelActorIdleStep
- `800eed78 topdownActorPatrolVStep` (was func_800EED78; I0NFO2) - as I0NFO3 tunnelActorPatrolVStep
- `800eefa0 topdownActorPatrolHStep` (was func_800EEFA0; I0NFO2) - as I0NFO3 tunnelActorPatrolHStep
- `800ef1c0 topdownActorPatrolDiagStepA` (was func_800EF1C0; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepA
- `800ef420 topdownActorPatrolDiagStepB` (was func_800EF420; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepB
- `800ef68c topdownActorOpenDialogue` (was func_800EF68C; I0NFO2) - opens the actor's dialogue by its id and event flags (as I0NFO3 tunnelActorOpenDialogue); returns 0 when none
- `800efd68 topdownActorTalkStep` (was func_800EFD68; I0NFO2) - faces the player, topdownActorOpenDialogue, waits for the message, completes the event wac (as I0NFO3 tunnelActorTalkStep)
- `800f0270 topdownActorStep` (was func_800F0270; I0NFO2) - step 0 by the movement kind B8 (0 idle, 1..4 patrols), contact starts the talk step 1; brightness by the zone B6
- `800f0448 topdownActorInit` (was func_800F0448; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FED7C[subtype], category |= 0x80
- `800f05b0 topdownActorHandler` (was func_800F05B0; I0NFO2) - actor object: b04 0 init, 1 cull register and topdownActorStep, 3 freeObjectLayer2
- `800f0658 topdownA05SceneryInit` (was func_800F0658; I0NFO2) - (no caller) AREA05 scenery object init from D_800FEDC4[subtype] (anim by wb4, clut 0x7F48 / 0x7FC8 by the zone wb6, b0f -0xC), inlined in topdownA05SceneryHandler
- `800f075c topdownA05SceneryUpdate` (was func_800F075C; I0NFO2) - (no caller) zone brightness (wb6 against D_800A60A3), visible, cull register
- `800f07c0 topdownA05SceneryHandler` (was func_800F07C0; I0NFO2) - AREA05 handler of the per-area object slot MAIN0 80079C20 (dispatchAreaDialogInit): b04 0 init from D_800FEDC4, 1 zone brightness + cull register, 3 freeObjectLayer2
- `800f0984 topdownA16SceneryInit` (was func_800F0984; I0NFO2) - (no caller) AREA16 scenery init from D_800FEDD0[subtype], inlined in topdownA16SceneryHandler
- `800f0a88 topdownA16SceneryApplyState` (was func_800F0A88; I0NFO2) - (no caller) by D_8009CFA8 (2 / 3): anim wb4 0xB / 0xC at x -4 / -3, y 0x193
- `800f0b10 topdownA16SceneryUpdate` (was func_800F0B10; I0NFO2) - objects with wb8 5 follow topdownA16SceneryApplyState, then zone brightness and cull register
- `800f0c0c topdownA16SceneryHandler` (was func_800F0C0C; I0NFO2) - AREA16 handler of the per-area object slot MAIN0 80079C20: init from D_800FEDD0, update, freeObjectLayer2
- `800f0d80 topdownA17SceneryInit` (was func_800F0D80; I0NFO2) - (no caller) AREA17 scenery init from D_800FEE04[subtype], inlined in topdownA17SceneryHandler
- `800f0e84 topdownA17SceneryUpdate` (was func_800F0E84; I0NFO2) - objects with wb8 0xD / 0xE run a state machine over D_8009CF9F[k] / D_8009CFA2[k] (anims D_800FEDF4 / D_800FEE0C), others zone brightness
- `800f126c topdownA17SceneryHandler` (was func_800F126C; I0NFO2) - AREA17 handler of the per-area object slot MAIN0 80079C20: init from D_800FEE04, update, freeObjectLayer2
- `800f13e0 topdownA19SceneryInit` (was func_800F13E0; I0NFO2) - AREA19 scenery init from D_800FEE34[subtype] (b0d 0x80 when b0c is set)
- `800f1500 topdownA19SceneryUpdate` (was func_800F1500; I0NFO2) - (no caller) zone brightness, AnimAdvance, cull register (inlined in topdownA19SceneryHandler)
- `800f1590 topdownA19SceneryHandler` (was func_800F1590; I0NFO2) - AREA19 handler of the per-area object slot MAIN0 80079C20: init, animated update, freeObjectLayer2
- `800f16bc topdownA08ActorInit` (was func_800F16BC; I0NFO2) - AREA08 type 0x30 actor init from D_800FEE58[subtype]: anim by BA (1: anim 7, freed when D_8009CF01 is set; 0: anim 1; else dark)
- `800f18c0 topdownA08ActorHandler` (was func_800F18C0; I0NFO2) - AREA08 type 0x30 handler (updateL2Type30Object): init, cull register + AnimAdvance, freeObjectLayer2
- `800f1968 topdownA08SceneryInit` (was func_800F1968; I0NFO2) - (no caller) AREA08 scenery init from D_800FEE70 / 74 / 78 (anim by b0c & 0x7F, freed at once when b0c & 0x80), inlined in topdownA08SceneryHandler
- `800f1a3c topdownA08SceneryUpdate` (was func_800F1A3C; I0NFO2) - (no caller) visible, cull register
- `800f1a60 topdownA08SceneryHandler` (was func_800F1A60; I0NFO2) - AREA08 handler of the per-area object slot MAIN0 80079C20: init, cull register, freeObjectLayer2
- `800f1c88 topdownA11ActorOpenDialogue` (was func_800F1C88; I0NFO2) - opens the dialogue of the AREA11 actor by its id BA (func_8002DCC8 with event / item flags); returns 1 when a message was opened
- `800f1e48 topdownA11ActorSetDirAnim` (was func_800F1E48; I0NFO2) - (no caller) anim D_800FEEC0[subtype][0][animFrame + i], AnimJump(n)
- `800f1ea4 topdownA11ActorTalkStep` (was func_800F1EA4; I0NFO2) - faces the player, topdownA11ActorOpenDialogue, waits for the message and releases the script flags
- `800f20cc topdownA11ActorIdleStep` (was func_800F20CC; I0NFO2) - (no caller) as I0NFO3 tunnelActorIdleStep (D_800FEEC0)
- `800f2164 topdownA11ActorPatrolVStep` (was func_800F2164; I0NFO2) - as I0NFO3 tunnelActorPatrolVStep
- `800f2350 topdownA11ActorPatrolHStep` (was func_800F2350; I0NFO2) - as I0NFO3 tunnelActorPatrolHStep
- `800f2538 topdownA11ActorPatrolDiagStepA` (was func_800F2538; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepA
- `800f2760 topdownA11ActorPatrolDiagStepB` (was func_800F2760; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepB
- `800f2994 topdownA11ActorStep` (was func_800F2994; I0NFO2) - step 0 by the movement kind B8 (idle / patrols), contact starts topdownA11ActorTalkStep; zone brightness
- `800f2b50 topdownA11ActorInit` (was func_800F2B50; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FEEB8[subtype], category |= 0x80
- `800f2ca8 topdownA11ActorHandler` (was func_800F2CA8; I0NFO2) - AREA11 type 0x30 handler (updateL2Type30Object): init, cull register + topdownA11ActorStep, freeObjectLayer2
- `800f2d50 topdownA16ActorSetDirAnim` (was func_800F2D50; I0NFO2) - anim D_800FEEFC[subtype][0][animFrame + i], AnimJump(n), D2 = the index (used by the scene steps)
- `800f2e84 topdownA16ActorOpenDialogue` (was func_800F2E84; I0NFO2) - opens the dialogue of the AREA16 actor by its id BA (func_8002DCC8 with event / item flags); returns 1 when a message was opened
- `800f3058 topdownA16ActorTalkStep` (was func_800F3058; I0NFO2) - faces the player, topdownA16ActorOpenDialogue, waits for the message and releases the script flags
- `800f328c topdownA16ActorPatrolVStep` (was func_800F328C; I0NFO2) - as I0NFO3 tunnelActorPatrolVStep
- `800f3468 topdownA16ActorPatrolHStep` (was func_800F3468; I0NFO2) - as I0NFO3 tunnelActorPatrolHStep
- `800f3640 topdownA16ActorPatrolDiagStepA` (was func_800F3640; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepA
- `800f3858 topdownA16ActorPatrolDiagStepB` (was func_800F3858; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepB
- `800f3a6c topdownA16ActorStep` (was func_800F3A6C; I0NFO2) - step 0 by the movement kind B8 (idle / patrols), contact starts topdownA16ActorTalkStep; zone brightness
- `800f3c28 topdownA16ActorInit` (was func_800F3C28; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FEEF4[subtype], category |= 0x80
- `800f3d84 topdownA16ActorHandler` (was func_800F3D84; I0NFO2) - AREA16 type 0x30 handler (updateL2Type30Object): init, cull register + topdownA16ActorStep, freeObjectLayer2
- `800f3e2c topdownA17ActorSetDirAnim` (was func_800F3E2C; I0NFO2) - anim D_800FEF6C[subtype][0][animFrame + i], AnimJump(n), D2 = the index (used by the scene steps)
- `800f4110 topdownA17ActorOpenDialogue` (was func_800F4110; I0NFO2) - opens the dialogue of the AREA17 actor by its id BA (func_8002DCC8 with event / item flags); returns 1 when a message was opened
- `800f443c topdownA17ActorIdleStep` (was func_800F443C; I0NFO2) - (no caller) as I0NFO3 tunnelActorIdleStep (D_800FEF6C)
- `800f44dc topdownA17ActorPatrolVStep` (was func_800F44DC; I0NFO2) - as I0NFO3 tunnelActorPatrolVStep
- `800f46d4 topdownA17ActorPatrolHStep` (was func_800F46D4; I0NFO2) - as I0NFO3 tunnelActorPatrolHStep
- `800f48c8 topdownA17ActorPatrolDiagStepA` (was func_800F48C8; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepA
- `800f4afc topdownA17ActorPatrolDiagStepB` (was func_800F4AFC; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepB
- `800f4d3c topdownA17ActorTalkStep` (was func_800F4D3C; I0NFO2) - faces the player, topdownA17ActorOpenDialogue, waits for the message and releases the script flags
- `800f4f70 topdownA17ActorStep` (was func_800F4F70; I0NFO2) - step 0 by the movement kind B8 (idle / patrols), contact starts topdownA17ActorTalkStep; zone brightness
- `800f5134 topdownA17ActorInit` (was func_800F5134; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FEF64[subtype], category |= 0x80
- `800f528c topdownA17ActorHandler` (was func_800F528C; I0NFO2) - AREA17 type 0x30 handler (updateL2Type30Object): init, cull register + topdownA17ActorStep, freeObjectLayer2
- `800f5334 topdownA18ActorInit` (was func_800F5334; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FEFA0[subtype], category |= 0x80
- `800f548c topdownA18ActorHandler` (was func_800F548C; I0NFO2) - AREA18 type 0x30 handler (updateL2Type30Object): init, cull register, pad bit 0x1000 cycles the anim wb4 (0..7, D_800FEFA8), AnimAdvance, freeObjectLayer2
- `800f5590 topdownA19ActorSetDirAnim` (was func_800F5590; I0NFO2) - anim D_800FEFFC[subtype][0][animFrame + i] when it differs from D2, AnimJump(n)
- `800f5618 topdownA19ActorIdleStep` (was func_800F5618; I0NFO2) - (no caller) as I0NFO3 tunnelActorIdleStep (D_800FEFFC)
- `800f56cc topdownA19ActorPatrolVStep` (was func_800F56CC; I0NFO2) - as I0NFO3 tunnelActorPatrolVStep
- `800f58f4 topdownA19ActorPatrolHStep` (was func_800F58F4; I0NFO2) - as I0NFO3 tunnelActorPatrolHStep
- `800f5b14 topdownA19ActorPatrolDiagStepA` (was func_800F5B14; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepA
- `800f5d74 topdownA19ActorPatrolDiagStepB` (was func_800F5D74; I0NFO2) - as I0NFO3 tunnelActorPatrolDiagStepB
- `800f5fe0 topdownA19ActorOpenDialogue` (was func_800F5FE0; I0NFO2) - opens the dialogue of the AREA19 actor by its id BA (func_8002DCC8); returns 1 when a message was opened
- `800f61d4 topdownA19ActorTalkStep` (was func_800F61D4; I0NFO2) - faces the player, topdownA19ActorOpenDialogue, waits for the message, completes the event wac
- `800f67b4 topdownA19ActorStep` (was func_800F67B4; I0NFO2) - step 0 by the movement kind B8 (idle / patrols), contact starts topdownA19ActorTalkStep; zone brightness
- `800f698c topdownA19ActorInit` (was func_800F698C; I0NFO2) - box / tpage / bank / anim (by B4) from D_800FEFF4[subtype], category |= 0x80
- `800f6b4c topdownA19ActorHandler` (was func_800F6B4C; I0NFO2) - AREA19 type 0x30 handler (updateL2Type30Object): init, cull register + topdownA19ActorStep, freeObjectLayer2
- `800f6bf4 topdownA11SceneryInit` (was func_800F6BF4; I0NFO2) - (no caller) AREA11 scenery init from D_800FF054[subtype], inlined in topdownA11SceneryHandler
- `800f6cf8 topdownA11SceneryUpdate` (was func_800F6CF8; I0NFO2) - (no caller) zone brightness, visible, cull register
- `800f6d5c topdownA11SceneryHandler` (was func_800F6D5C; I0NFO2) - AREA11 handler of the per-area object slot MAIN0 80079C20: init from D_800FF054, zone brightness + cull register, freeObjectLayer2
- `800f6f20 spawnNpcSet07` (was func_800F6F20; I0NFO2) - spawn step of unlayered variant 7 (MAIN0 D_8007C1C8[7]): allocates the objects of the record list D_800FFD7C[GAME+0x58D] and D_800FF06C
- `800fa424 stepNpcSet07` (was func_800FA424; I0NFO2) - step switch of variant 7: steps 0..12 run the scene functions (they animate the AREA05 actors with topdownActorSetAnim), step 9 starts sound 0x35
- `800fa5c8 updateNpcSet07` (was func_800FA5C8; I0NFO2) - unlayered variant 7 (MAIN0 dispatchUnlayeredByVariant D_8007C1C8[7]): b04 0 spawnNpcSet07, 1 stepNpcSet07, 3 free
- `800fa670 spawnNpcSet0C` (was func_800FA670; I0NFO2) - spawn step of unlayered variant 0xC: allocates the objects of its record list
- `800fa84c npcSet0CMessageScene` (was func_800FA84C; I0NFO2) - step 1 of variant 0xC: locks the player, opens a message after the w08 delay and waits for it (D_800A60C8)
- `800faf68 stepNpcSet0C` (was func_800FAF68; I0NFO2) - (no caller) step switch of variant 0xC (1: npcSet0CMessageScene), inlined in updateNpcSet0C
- `800fafac updateNpcSet0C` (was func_800FAFAC; I0NFO2) - unlayered variant 0xC (MAIN0 D_8007C1C8[0xC]): b04 0 spawnNpcSet0C, 1 npcSet0CMessageScene at step 1, 3 free
- `800fb054 spawnNpcSet0D` (was func_800FB054; I0NFO2) - spawn step of unlayered variant 0xD: two record lists (D_800FFE24) chosen by event flags, stores D_8009CFA5
- `800fb32c updateNpcSet0D` (was func_800FB32C; I0NFO2) - unlayered variant 0xD (MAIN0 D_8007C1C8[0xD]): b04 0 spawnNpcSet0D, then idle until freed
- `800fb3c4 spawnNpcSet0E` (was func_800FB3C4; I0NFO2) - spawn step of unlayered variant 0xE: D_8009CF60 from the event flags, records by bc
- `800fbbcc npcSet0EPickStep` (was func_800FBBCC; I0NFO2) - (no caller) step 0 of stepNpcSet0E: D_8009CFA8 == 1 -> 2, D_8009CFA7 == 1 -> 1, D_8009CFAA == 1 -> 3
- `800fbf5c stepNpcSet0E` (was func_800FBF5C; I0NFO2) - (no caller) step switch of variant 0xE (pick, scene steps 1..3 with the AREA16 actors), inlined in updateNpcSet0E
- `800fc050 updateNpcSet0E` (was func_800FC050; I0NFO2) - unlayered variant 0xE (MAIN0 D_8007C1C8[0xE]): b04 0 spawnNpcSet0E, 1 the step switch, 3 free
- `800fc1c0 spawnNpcSet0F` (was func_800FC1C0; I0NFO2) - spawn step of unlayered variant 0xF: record lists by event flags (D_80100368), stores D_8009CFA5, clears D_8009CF9F
- `800fda4c stepNpcSet0F` (was func_800FDA4C; I0NFO2) - (no caller) step switch of variant 0xF: steps 0..11 run the scene functions (AREA17 actors), inlined in updateNpcSet0F
- `800fdb34 updateNpcSet0F` (was func_800FDB34; I0NFO2) - unlayered variant 0xF (MAIN0 D_8007C1C8[0xF]): b04 0 spawnNpcSet0F, 1 the step switch, 3 free
- `800fdca4 spawnNpcSet10` (was func_800FDCA4; I0NFO2) - spawn step of unlayered variant 0x10: allocates the objects of its record list
- `800fde6c updateNpcSet10` (was func_800FDE6C; I0NFO2) - unlayered variant 0x10 (MAIN0 D_8007C1C8[0x10]): b04 0 spawnNpcSet10, 1 AnimAdvance at step 0, 3 free
- `800fdf24 spawnNpcSet11` (was func_800FDF24; I0NFO2) - spawn step of unlayered variant 0x11: allocates the objects of its record lists
- `800fe848 stepNpcSet11` (was func_800FE848; I0NFO2) - step switch of variant 0x11: items 0x1E / 0x1F used near the AREA19 actor D_8009F2E8 start its message scene (sfx 0x52)
- `800feb28 updateNpcSet11` (was func_800FEB28; I0NFO2) - unlayered variant 0x11 (MAIN0 D_8007C1C8[0x11]): b04 0 spawnNpcSet11, 1 stepNpcSet11, 3 free
- Data names by usage (Oct 26 2026, data_names_main0.csv rows with `usage:` source, applied with tools/rename_data.py): PLAYER_EXT
  (8009C330, pointer to PLAYER+0xF8 set by topdownPlayerInit), BITTEN_OBJECT (8009D2E8), WEAPON_STATE (8009C338), GRABBED_OBJECT
  (8009F0EC, playerProbeGrab result), GAME (8009C960, the gameConfig the ported sources already used; GAME_A / GAME_B stay as two
  extern views in areaTransitionHandler.c, one load each), AREA_VARIANT (8009F838), MOVIE_DEC_ENV (8009BCA8), COS_TABLE / NEG_SIN_TABLE
  (8007A5F0 / 8007A3F0), MENU_STATE (800A45D8, MenuState in game.h replaces the per-file unk_800A3940 macro), SPAWN_TABLES,
  UI_SPRITE_DEFS, BALLOON_FRAME_UVS, WEAPON_HEAD_OFFSETS, AREA_SECTION_COUNTS, AREA_SCRIPT_CTX, BAIT_OBJECT, INFO_MESSAGE_SLOT.
  Header alias D_8009C9D8 renamed to PAD_PRESSED. Not named: the fields of PLAYER (800A603C.., 800A6078, 800A60D0..) and of GAME
  (8009C962 selectedSection, 8009D2B0.., 8009CDA3 inventoryScreen, 8009CD94/96 nextArea/nextSection) that sources reach by
  their own symbols, and the Psy-Q library globals (libgpu 80090xxx / 8009B1xx, libcd 80096xxx, libetc, libpad).
- Second data batch (Oct 26 2026): WEAPON_SWING_TABLE (8007A070), THROWN_OBJECT_DEFS (8007A148), CD_INTR / CD_COMATTR (the psx_tomba
  names the libcd sources carried as comments), GVARS (8009C930, include/gvars.h; game.h now includes it instead of its
  u_char D_8009BC98[0x2C] alias), and csv rows for the psx_tomba library names the libgpu / libcd sources already use bare
  (GPU_INFO, GPU_DATA, GPU_STATUS, DMA1_*/DMA2_*, DPCR, GPU_QIN/QOUT, RSIN_TABLE, GPU_CTLBUF, GPU_QITEM, VBLANK_MINUS, CD_CBSYNC,
  CD_CBREADY, CD_DEBUG, CD_STATUS/1, CD_POS, CD_MODE, CD_COM, CD_COMSTR) so their D_ aliases get the same name.
  spawnEventTitle.c (tomba24 headers) keeps D_8009BC98[0xF] for GVARS.b3f: the struct view changes the code.
- X000 data batch 1 (Oct 26 2026, notes/data_names_x000.csv, tools/rename_data.py --prog X000): 45 overlay globals named from who reads
  them: PLAYER_ANIM_TABLE, PLAYER_MOVE_PARAMS (9 rows of 0x14 by speed class; ObjGravityStep's D_80115466 and
  playerSetJumpRiseVelocity's D_80115468 are its +0xa / +0xc fields, left as fields), PLAYER_DECEL_PARAMS (9 rows of 0x74),
  SLOPE_TILT_TABLE / _SLOW, PLAYER_FOOT_OFFSET_BY_DIR, PLAYER_PERCH/SWING_ANIM_PAIRS, PLAYER_SPIN_JUMP_ANIMS, PLAYER_SPIN_RELEASE_SPEEDS,
  PLAYER_POWER_AURA_DEFS, TREASURE_CHEST_ANIM_SETS / _A3S9_, ITEM_DROP_ENTRIES/LISTS/VELOCITIES, SCRIPT_ACTOR_DEFS, WEAPON_TRAIL_DEFS,
  VILLAGER_NPC_STEP_TABLE / _WALK_PATHS, BREAK_PIECE_DEFS, FLASH_EFFECT_DEFS_BY_AREA, EGG_SHELL_PIECE_VELOCITIES, KOMA_PIG_ANIMS (58
  pointers; D_8013A178..D_8013A268 are its elements and keep their names), KOMA_PIG_ANIM_BOXES, KOMA_PIG_IDLE/WAIT_ANIM_BY_SUBTYPE,
  KOMA_PIG_RAND_TABLE, RED_BIRD_RAND_STATE_TABLE / _SUBTYPE_PARAMS, ENEMY_DISTANCE_ACTION_TABLES, CHICK_WANDER_RAND_TABLE,
  YELLOW_CHICK_CLUT_Y, the A00_* tables (SCROLL_CONTROLLER_HANDLERS, FOG_CLOUD_QUADS, SPORE_BRANCH_SUBTYPE_OFS, WOODEN_BRANCH_HANDLERS /
  _PARAMS, L2_TYPE06_ANIMS, ITEM_OBJ_PATH_START / _NODES, AP_BOX_SPARKLE_TABLE / _OFFSETS, AP_BOX_50K_ITEM_ENTRY, TUTORIAL_JUMP_MESSAGE_IDS).
  The src/x0NN twins get the same identifier rename (the tool's twin pass); overlay-only sources (e.g. src/x004/playerStepDizzy.c)
  that spell D_801152E8 themselves are not touched: their address is their own overlay's. func_80110CAC / playerDecelerateSpeedB keep a
  byte view PLAYER_DECEL_PARAMS_BYTES next to the row view; komaPigState0 keeps KOMA_PIG_ANIMS_B (second extern, anti cross-jump).
- X000 data batch 2 (Oct 26 2026): 49 more rows, mostly the per-object animation pointer tables of the area-0 .data
  (RED_BIRD_ANIMS, FROG_ANIMS, NPC_CHARLES_ANIMS, PINK_PIGLET_ANIMS, HOODED_OLD_MAN_ANIMS, BITING_PLANT_ANIMS, CHICK_ANIM, EGG_ANIM,
  A00_*_ANIMS for the fog cloud / far backdrops / koma pig spawner / spore cloud / tornado / splash droplets / dark cloaked figure /
  backdrop sprites / L4 type 18 / variant-0 scene parts, KOMA_PIG_DUST_PUFF_*_BY_AREA, MUSHROOM_CLUSTER_* , YELLOW_CHICK_ANIMS,
  EGG_SHELL_PIECE_ANIMS, POOL_TYPE04_ANIMS) named after their one owner, and small per-subtype tables (POOL_TYPE05_RING_DEFS,
  GROUND_DEBRIS_RAND_VEL_H/V, VILLAGER_NPC_SPRING_PARAMS, PLAYER_ROPE_JUMP_OFF_SPEEDS, A00_POOL_TYPE24_CLUT_X/Y,
  A00_SCROLL_CONTROLLER_MODE_BY_SECTION, A00_INIT_OBJECT_ANGLE_OFFSETS, A00_L2_TYPE06_PHASE_LIMITS/WAITS/BOXES, BITING_PLANT_BOXES /
  _BAIT_VEL_BY_FRAME, A00_AP_BOX_100K_ITEM_ENTRY, A00_VARIANT0_SCENE_ITEM_ENTRY, L8_TYPE00_BOX0_BY_VARIANT). Only the base of each
  pointer table is named: the element externs (DAT_8013ad38.., D_8013B180.., D_8013B2D0..) keep their address names, the csv row says which.
- 8006eef8: notes/functions_main0.csv says SpuInit, config/symbol_addrs_main0.txt says SsStart2 (psx_tomba); SpuInit itself is matched by
  bytes at 80074b6c (vs_vh.c), so 8006eef8 needs byte verification against libsnd (SsStart2 / SsStart / SpuInitHot neighbours 8006eb00 /
  8006eed8 disagree with psx_tomba too, see above) before either name is applied to the source. Left as is.
- game.h playerEquips moved from offset 0x950 to 0x952 of gameConfig (two bytes unk950 / unk951 before it): the code reads the
  weapon at 8009D2B2 (drawHudWeaponIcon, spawnWeaponObject) and the pants at 8009D2B3 (func_80110CAC speed class); 8009D2B0 is the
  dark cloaked figure scene flag and 8009D2B1 the bitten-jump kind. No source used GAME.playerEquips, so the build is unchanged.
- MAIN0 data round 2, batch 1 (Oct 27 2026): 39 rows. The PLAYER / GAME fields the sources reach by their own symbol keep a standalone
  symbol with a field-style name (no struct access, codegen unchanged): PLAYER_STATE / PLAYER_SUBSTATE / PLAYER_STEP (800A603C..3E),
  PLAYER_X / PLAYER_Y (800A604A / 4E, the whole parts of obj.a / obj.y), PLAYER_H / PLAYER_D (800A6078 / 7C, obj.h / obj.d), PLAYER_HEALTH
  (800A60D0: script opcode a9 heals it and copies it to +0x9A and GAME.playerHealth), GAME_SELECTED_SECTION (8009C962),
  GAME_AREA00_INTRO_PHASE (8009C966, game.h unk6 -> area00_introPhase), GAME_FADE_SCREEN_CONTROL (8009C975), GAME_AREA00_EVENT_CONTROL
  (8009C984), GAME_PLAYER_MODE (8009D2B0, unk950 -> playerMode: 1 idle/walk, 2 message box, 3 scripted scene) and GAME_EQUIPPED_WEAPON
  (8009D2B2). The tool resolved the US-spelled aliases of the tomba24-era sources to the same PAL addresses (D_800A539C.. = PLAYER +4..,
  D_800A53D8 = PLAYER_H, D_800A5430 = PLAYER_HEALTH, D_8009BCCA = GAME_SELECTED_SECTION, D_8009C618 = GAME_PLAYER_MODE, D_8009BCDD =
  GAME_FADE_SCREEN_CONTROL, D_8009C61A = GAME_EQUIPPED_WEAPON), so those game.h aliases carry the new names too; the sources built with
  the tomba24 headers (`// HEADERS tomba24`: spawnEventTitle, startAreaBgmWithReverb, stubInventory1..3) keep the US spellings.
  Library: ST_RING_ADDR / ST_RING_SIZE / ST_MASK / ST_START_FRAME / ST_END_FRAME / ST_FUNC1 / ST_FUNC2 / ST_MODE (libpress, the
  parameters StSetRing / StSetMask / StSetStream store), CD_READ_ATTR (cdread.c ReadAttr_t), PAD_SIO (padlocal.h padSio *).
  Game data: MOVE_TABLE_0..3 (80077CDC.. the o->movetab rows of increasing hop speed), REWARD_HANDLERS / REWARD_HANDLER_BY_SUBTYPE
  (8007A890 / 8007A7F0), PLAYER_EFFECT_4E_ANIM_TABLE (80079D58), WEAPON_BEND_PARAMS (8009D600 = GV6.s00..s04), PAUSE_MENU_CURSOR
  (8009C10B), DEBUG_SCREEN_CURSOR_ROW (8009B6A8), MODEL_PRIM_BUFFERS (800E3E28, 0x1600 per slot), TILE_COL_COUNT / TOP / HEIGHT / FLAGS
  (8009BD70..7C, tileCollideColumnAt scratch). Not named: the libgpu sys.c globals every libgpu source declares but none uses
  (D_80090Cxx / D_8009B1xx), the libcd D_80095F64 Setloc table, the libetc Intr block, libgs OpenTMD offsets, the remaining libpress
  ring indices, and D_8009C8B0 / D_800B3188 / D_800A3348 / D_8009C658, whose only references are commented-out code.
- MAIN0 data round 2, batch 2 (Oct 27 2026): 35 rows + 2 corrections. The references were re-counted by REAL address (the relocations of
  the full-build objects, as tools/rename_data.py resolves them), which is what matters: many sources spell the US address of a PAL
  global (e.g. updatePauseMenu's D_8009C10B is 8009CDA3 = GAME.inventoryScreen, not 8009C10B; displayDebugScreen's D_8009B6A8 is
  8009C340), so the batch-1 rows for PAUSE_MENU_CURSOR (-> GAME_INVENTORY_SCREEN at 8009CDA3) and DEBUG_SCREEN_CURSOR_ROW (-> 8009C340)
  were corrected. Scratchpad fields after the psx_tomba `struct scratchpad` names, standalone symbols next to the game.h macros of the
  same name (the macros stay for the sources that cast the absolute address, acquireSpriteSlot keeps the D_1F800164 / D_1F8001E0
  aliases game.h declares for it): NEXT_PRIM (1F800164), CURRENT_OT (1F8001E0), FRAME_BUFFER_INDEX, FRAME_COUNT, TICK_COUNT (1F8001F8,
  the per-tick counter gameplayTick increments), JOYPAD_STATE, FREE_OBJECTS / FREE_OBJECT_COUNT (1F800208 / 238, one layer's stack),
  MOVIE_PLAY_STATE / MOVIE_ID / MOVIE_SKIP_REQUEST, plus MODEL_VERTEX_PTR (1F80008C) and COLLISION_SEG_PTR (1F800278). PLAYER fields:
  PLAYER_VISIBLE / PLAYER_TYPE / PLAYER_DRAW_DEPTH (b0f, the OT depth otadd draws relative to) / PLAYER_POS (the a/y/b Fix16 triple) /
  PLAYER_Y_FIX / PLAYER_ANIM / PLAYER_ANIM_FRAME / PLAYER_VEL_Y / PLAYER_HELD_OBJECT (PLX.p611c). GAME fields: GAME_SELECTED_SPAWN_POINT,
  GAME_EQUIPPED_PANTS, GAME_BITTEN_JUMP_KIND (unk951 -> bittenJumpKind), GAME_KEEP_BGM, GAME_PLAYER_HEALTH, GAME_PLAYER_MAX_HEALTH
  (8009C971: increaseMaxHealth raises it with a cap of 8 / 16 and the health is clamped with it, so game.h / gamestate.h
  playerHealthDisplayed -> playerMaxHealth, the 10 sources that spell the field followed), GAME_TOTAL_TIME_PLAYED,
  GAME_FADE_SCREEN_AMOUNT, GAME_PLAYER_AP, GAME_DISABLE_SELECT_MENU. Others: AREA_SOUND_BANK_LOADED (8009D69C), DEBUG_NEXT_PRIM (8009D540),
  PAD_INFO (8009D610). Not named: the camera scratch (1F8000E6 / F0 / 16A / 16E: which is x / y / scroll is not certain), the collision loop
  counter 1F80019E, the matrix / vector scratch (1F800000 / 20 / 60 / 68 / C0), SPR_DATA[3] (1F8002D4), 1F8001C6 / 1F8001C8, the button masks
  1F8003C4.., GVARS bytes reached by their own symbol (8009C93F 55 refs, 8009C942, 8009C93E, 8009C940, 8009C93A: meaning unknown),
  PLAYER b6b / b9e / d90 / +0x9A, GAME +0x30 (US displayExpBar, but read as `& 3` by the speed code), GV6.s74 (8009D674, the pad halfword
  next to PAD_PRESSED), and the GCD bytes 8009CEBD.. / 8009CF06 / 8009D006.
- MAIN0 data round 2, batch 3 (Oct 27 2026): 43 rows, chosen by real references. Display: DISPLAY_BUFFERS (8009E348, the two 0xD10
  DISPENV / DRAWENV records of initWideDisplayEnv / initDisplay) and the bytes 12 sources reach by their own symbol, DISPLAY_BUFFER_0_BG_R /
  G / B (8009E375.. = [0].draw.r0/g0/b0, the DRAWENV clear colour initDisplay / updateSectionFadeController set) and DISPLAY_BUFFER_1_BG_R /
  G / B (8009F085..). Objects: SPAWN_RECORD_OBJECT / SPAWN_RECORD_PREV_OBJECT (8009BD98 / 9C, spawnObjectFromRecord's chain), OBJECT_POOL
  (800A6610, 200 x 0xD4) and OBJECT_POOL_LAYER7 (800A49A8, 10 x 0x6C), FREE_OBJECTS_LAYER7 / FREE_OBJECT_COUNT_LAYER7 (1F800214 / 23C),
  FREE_OBJECT_COUNT_LAYER1 (1F800236) / FREE_OBJECT_COUNT_UNLAYERED (1F80023E), the per-frame category stacks CAT2_OBJECT_LIST / COUNT
  (1F80021C / 246), CAT5_OBJECT_LIST / COUNT (1F800228 / 248), CAT4_OBJECT_COUNT (1F80024C), CAT8_OBJECT_COUNT (1F800242),
  UPDATE_OBJECT_INDEX (1F800198, the loop index updateObjectsLayer8 / updateObjectsUnlayered count and the sprites add to TICK_COUNT as a
  phase), SPAWN_AXIS_SWAP (1F8001C8: bit 0 of the spawn row flags; every allocator swaps the h / d axis pointers when set). Player /
  camera scratch: PLAYER_H_WHOLE / PLAYER_Y_WHOLE / PLAYER_D_WHOLE (1F80016A / 16E / 172, the whole parts updatePlayer copies every tick),
  SCREEN_ORIGIN_X / SCREEN_ORIGIN_Y (1F800176 / 186, the world h / y the culling tests subtract: isOnScreen x - X + 0x40 < 0x1C1, Y - y +
  0x40 < 0x171; whole parts of the ints 1F800174 / 184 updateCameraController sets), PLAYER_MESSAGE_BOX (800A60C8 = PLAYER.obj.d90, the
  showMessageBox result of the scene code). Collision: COLLISION_HIT_FLAGS / COLLISION_HIT_HEIGHT (1F800282 / 27E, the TILE_COL_FLAGS /
  TILE_COL_HEIGHT of the last hit). Input: BUTTON_MASK_A..D (1F8003C4..CA, BUTTON_CONFIGS[].a..d of loadButtonConfig; the default config
  is Square / X / Circle / Triangle), PAD_BUFFERS (8009F7F0, the two 0x22-byte libpad buffers of joypadInit / readPadPort), PAD_INTR
  (8009820C, the libpad I_STAT / I_MASK pointer, like PAD_SIO). HUD: HUD_ITEM_GAUGE_TIMERS (800B1468, u8[4][4]). Misc: AREA_MODEL_DRAW_LIST
  (800B0D98, filled by the area model link hook, drawn by drawAreaModelList, cleared by clearOtBuffer), SPRITE_UNPACK_BUFFER (800D7E28),
  LOADING_SCREEN_STATE (8009F7E4). GVARS fields reached by their own symbol: GVARS_USE_ITEM_PENDING / GVARS_USE_ITEM_ID (8009C940 / 41:
  x014 updateSectionClearController sets id + pending, playerStepUseItem / l4Type1BInitModel consume them; gvars.h b40 / b41 ->
  useItemPending / useItemId) and GVARS_FADE_EFFECT (8009C93C, scriptOpSetFadeEffect's first argument; b3c -> fadeEffect). The
  header-declared US aliases (D_8009D6DD.., D_8009E3ED.., D_8009EB4C, D_8009EB58, D_800A5970, D_8009BCA4, D_1F8003C4..CA, D_1F800246 / 248,
  D_1F80016A, D_1F800198, D_1F8001C8, D_1F800176 / 186, D_1F800214 / 236 / 23C) renamed by hand in game.h. Not named: the matrix / vector
  scratch 1F800000 / 20 / 60 / 68 (GTE pipeline temporaries), the libgpu / libetc / libcd / libpress internals (80091930, 8009813C,
  800970B4, 80096F4C, 8009BF58.., 8009BF70, 800A2264.., 8009F7E0, 80097094: no matched source), 8009D614 / 616 / 618 (inside PAD_INFO:
  +2 / +4 / +6 are the vibration bytes, +8 the stick x), 800B1410 (HUD show state tickHud reads as .on / .cnt), the PLAYER bytes b69 /
  b6b / b9c / b9e / w9a / da0 / wb2 / wb4 and PLX.b60f8 / b60fe / b6104 / b611b (meaning unknown), 1F8001C6 / 1F8001CF / 1F8001DC.
- MAIN0 data round 2, batch 4 (Oct 27 2026): 42 rows. Camera: VIEW_CAMERA (1F8000C0, the camera.h struct; the sources that declare it as
  a MATRIX keep their type) and the halves reached by their own symbol, VIEW_CAMERA_ROT_Y (1F8000E6 = rot[1].p.hi, the angle dampCameraAngle
  / stepScrollAndAngle / cameraEventScrollHandler step by 2), VIEW_CAMERA_X (1F8000EE, initGameConfig 160), VIEW_CAMERA_Y_FIX / VIEW_CAMERA_Y
  (1F8000F0 / F2, clamped to the section bounds by cameraFollowPlayerY, initGameConfig -128). GVARS fields: GVARS_PLAYER_DRIFT_H / Y
  (8009C944 / 46 = s44 / s46 -> driftH / driftY, the velocity << 8 the player steps add every tick; playerUpdateWaterLevel sets 0 / -0x40 in
  water), GVARS_PLAYER_HURT_STATE (8009C938 = b38 -> hurtState: 1 playerStepHurtStun / Spin, 2 playerStepDie), GVARS_PLAYER_LANDED (8009C93A =
  b3a -> playerLanded). Objects: OBJECT_POOL_LAYER8 (800B1828), FREE_OBJECTS_LAYER8 / FREE_OBJECT_COUNT_LAYER8 (1F80020C / 23A), CAT8_OBJECT_LIST
  (1F800230), CAT7_OBJECT_COUNT / CAT3_OBJECT_COUNT (1F800240 / 24A) and the copies the drawCategoryN functions save for the later passes
  (CAT2/5/8_OBJECT_LIST_SAVED 1F800260 / 26C / 274, CAT2/3/5/7/8_OBJECT_COUNT_SAVED 1F800250 / 252 / 256 / 25A / 258). Loading: CD_QUEUE_HEAD
  (1F80029C; game.h keeps its macro of the same name for the absolute cast, so the game.h extern stays D_1F80029C like NEXT_PRIM), WHM_DATA
  (1F800308, the load-kind 0x70 collision blocks getCollisionPlaneAt reads), LOADED_AREA / LOADED_SECTION / LOADED_SPAWN_POINT (8009D2A8..AC,
  the GAME selection copied at load time), LOADED_SOUND_SET (1F8003D3), SCREEN_OFFSET_X / Y (8009D4FC / FE -> DISPENV screen.x / y),
  AREA_SCRIPT_RESULT (8009D4F0). Player / collision: PLAYER_Z (800A6052, whole part of obj.b), GAME_AREA_TRANSITION (8009C974),
  SLOPE_PROBE_ANGLE (1F80027C), GRAB_POINT_OFFSET (1F8003BC), TILE_COL_SURFACE_Y (8009BD68), RAND_SEED (1F800200), HUD_STATE (800B1410, 0x68:
  HUD_ITEM_GAUGE_TIMERS is its +0x58). Library: _spu_AllocBlockNum (80098940, libspu s_m_init.c next to the _spu_AllocLastNum / _spu_memList
  the ported sources already use). The `// HEADERS tomba24` sources (stubInventory1..3) must keep the US spellings: the header rename pass
  touched them once and they stopped compiling (kept as asm, sha1 still OK), so the linked-file count of the build log (1124 of 1152) is
  the check to watch. Not named: the libpress ring state (8009BF70 / 800A2264 / 2268 / 8009F7E0 / 80097094 / 8009C334 / 8009C90C / 800A3F00 /
  800A2524: StCdInterrupt internals, no matched source), the libcd timeouts 8009BF58 / 5C / 60 and 80096F4C, libetc 8009813C / 800970B4,
  libgpu 80091930, the remaining libpad state of padlocal.h, GVARS b3f / b42 / b3e / i34 (scene flags set together, meaning not pinned
  down), 8009D2AE (a per-subtype bitmask updateEgg / updateL1Type02Reward share), 1F8001DC / 1F8001C6 / 1F8001CF / 1F8001A4, 1F800244 / 218.
- MAIN0 data round 2, batch 5 (Oct 27 2026): 15 rows, the last clear ones. The rest of the per-category object stacks of the frame:
  CAT3_OBJECT_LIST / CAT3_OBJECT_LIST_SAVED (1F800220 / 264, initMainDrawList resets them to 800B1320), CAT4_OBJECT_LIST / LIST_SAVED /
  COUNT_SAVED (1F800224 / 268 / 254, probeClimbGrabObject walks the saved copy), CAT7_OBJECT_LIST / LIST_SAVED (1F80022C / 270),
  ONSCREEN_OBJECT_LIST / COUNT (1F800218 / 244: ObjListPush_1F800218 is what cullObjectToDrawList / Margin push to when the object passes
  the screen test; collideWeaponsWithCat2 / Cat4Objects walk it), FREE_OBJECTS_UNLAYERED (1F800210). Camera: CAMERA_TARGET_H_FIX /
  CAMERA_TARGET_Y_FIX (1F80018C / 190, the 16.16 point cameraFollowTargetY / YB follow: updateA00BitingPlantBait sets it to the bait,
  cameraModeHandler5 to the player's PLAYER_H_COPY / PLAYER_Y_COPY, 1F800168 / 16C, the 16.16 words whose whole halves PLAYER_H_WHOLE /
  PLAYER_Y_WHOLE are; a00TornadoStep reads the triple as a vector). SPRITE_PART_COUNTER (1F800078). tools/rename_data.py now skips the
  `// HEADERS tomba24` sources like the demo ones: it had renamed D_1F8000E6 inside stubInventory1..3 (declared by the US headers) and
  they stopped compiling. Clear names with >= 5 real references have run out; what remains at >= 5 is the libpress ring state, the libcd /
  libetc / libgpu / libpad internals without a matched source, the GVARS scene flags b3f / b42 / b3e, GAME +0x30 (US displayExpBar, read
  as `& 3` by the speed code), the PLAYER / PLX bytes listed in batch 3, the matrix / vector scratch 1F800000 / 20 / 60 / 68, SPAD.i74
  (passed as insertPrim's unused third argument), 1F8001C6 / CF / DC, 8009C978 / C981 / D2C3 / D2AE (GAME bitmasks and bytes of unknown
  meaning), SPR_DATA[3] / [4] and the SCRIPT_FLAGS / GCD / PAD_INFO bytes inside named blocks.
- SYSTEM data, batch 1: D0SPSUB (Oct 27 2026): 67 rows in notes/data_names_d0spsub.csv, applied with the new `--prog <SYSTEM overlay>`
  mode of tools/rename_data.py (objects build/full/obj/<prog>__<stem>.o, bytes from game/disc/fs/SYSTEM/<PROG>.BIN; in D0SPSUB
  mode every renamed file with an identical src/d0spsub3 twin gets the twin's own D_ spellings renamed by position and the twin
  addresses appended to notes/data_names_d0spsub3.csv as `twin of D0SPSUB <addr>`, 38 rows). SUBSCREEN_DRAW_PARAMS (8011113C,
  {x, y, tpage, clut, frame, +C, str, font}: the block every subScreenDraw* fills for subScreenDrawSprite / SpriteFT4 / String)
  with its fields SUBSCREEN_DRAW_Y / TPAGE / CLUT / FRAME / STR / FONT, SUBSCREEN_FONT (800FFD98, the 5-byte glyph table stored
  in .font), the object structs SUBSCREEN_MESSAGE_BOX (8010A200, + STATE at +4 that subScreenEventListStep waits on),
  SUBSCREEN_MAP_OBJECTS (8010A2D4, Sprite[3]: marker and the two cursor objects), SUBSCREEN_MAP_AREA_SPRITES (8010A550,
  Sprite[8]), SUBSCREEN_MAP_ICON_SPRITES (80110F50) / ICON_POSITIONS, SUBSCREEN_SAVED_CAMERA (8010A1C8); the popup list state
  SUBSCREEN_POPUP_CURSOR / COUNT / SUB_CURSOR / SUB_COUNT / ENTRIES / SUB_ENTRIES / STRINGS / SUB_STRINGS; the map view
  SUBSCREEN_MAP_VIEW_X / Y, SUBSCREEN_MAP_LINK, SUBSCREEN_PAD_LAST / PAD_REPEAT_TIMER; the tables SUBSCREEN_EVENT_ORDER /
  EVENT_SLOTS / ITEM_SLOTS / ITEM_LIST, ITEM_ICON_CLUTS, ITEM_CONDITION_INDEX / SCRIPTS, SUBSCREEN_COND_OPS, ITEM_NAME_INDEX /
  STRINGS / CLUTS, EVENT_NAME_STRINGS / UNKNOWN_STRING, SUBSCREEN_PAGE_FOOTER_SLOTS, SUBSCREEN_GAUGE_SLOTS, the tab tables
  SUBSCREEN_TAB_SLOTS_MAIN / MAP / MODE7 / SLOT_MINUS2 / TAB_IDS / TAB_FRAMES / TAB_EXTRA_FRAMES, SUBSCREEN_DIGIT_FRAMES, and the
  map graph MAP_NODES / LINKS / ROOM_LINK / AREA_ROUTES / ROUTE_STRIP_SHAPES / AREA_BOUNDS / AREA_VIEW_ORIGIN / ROOM_POSITIONS /
  AREA_SPRITE_ANIMS / ICON_ANIMS / MARKER_ANIM / AREA_SPAWN_LISTS and the name tables MAP_AREA_NAMES / AREA14_AREA_NAMES /
  ROOM_NAMES / AREA14_ROOM_NAMES. func_800EA5E8 and its D0SPSUB3 twin func_800EA540 (different stem) keep their `_a` alias
  externs, renamed by hand. Left: the per-field externs inside SUBSCREEN_MAP_OBJECTS[1..2] and SUBSCREEN_MESSAGE_BOX
  (object fields of unknown meaning), the menu sprite tables p4c / p50 / p60 / p64 (800FE8E4 / 8D8 / EC74 / 974), the map
  view words 801110FC / 104 / 108 / 10C / 110 / 114, 800FDE50 (per-node link neighbours, shape unclear), the image 800F18E0.
