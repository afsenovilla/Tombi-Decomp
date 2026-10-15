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
- `8002e4b4 updatePoolType20Object` (was func_8002E4B4) - pool type 0x20 handler (slot 80079C2C)
- `8002b3c8 l8Type01MoveStep` (was FUN_8002b3c8) - updateL8Type01Object movement: timed move toward +0xC4 with Rand wobble
- `80029f1c cameraModeHandler2` (was func_80029F1C) - camera controller tables (7 slots per group at 80079A9C/AB8/AD4, X000 80138424; slot 0 = cameraEventScrollHandler): slot 2, decays the X speed and follows the target Y (cameraFollowTargetY/YB)
- `8002a0fc cameraModeHandler3` (was func_8002A0FC) - camera controller slot 3: cameraFollowPlayerX/Y
- `8002a258 cameraModeHandler5` (was func_8002A258) - camera controller slot 5: target Y follow (cameraFollowTargetY/YB)
- `8002a3d4 cameraModeHandler6` (was FUN_8002a3d4) - camera controller slot 6 of the first group: cameraFollowPlayerX/Y then clamps to the player for player modes < 4 or 7
- `80027810 cameraModeHandler6B` (was func_80027810) - slot 6 of the second group (80079AD0)
- `8002c73c updatePoolType66Object` (was func_8002C73C) - pool handler at table slot 80079D44 (index 0x66): anim D_80014764 placed at the player
- `8002c920 updatePoolType67Object` (was func_8002C920) - pool handler at slot 80079D48 (index 0x67): anim D_800121F4[subtype]
- `8002e6c4 updatePoolType22Object` (was func_8002E6C4) - pool type 0x22 handler (slot 80079C34): render kind 7, category 3 list
- `8004baa8 probeClimbGrabObject` (was FUN_8004baa8) - playerClimbFollowObject helper: probeGrabObjectBox over the list 1F800254; result in 1F8003C0
- `80041d20 probeSlopeAngleAt` (was func_80041D20) (no caller found) - collision plane probe returning the slope angle (angleFromDelta)
- `8002f060 playerEffect4EStepE` (was func_8002F060) (no caller found) - another pool type 0x4E effect step (anim from D_80079D58, follows the player's visibility)
- `8003c550 updateL1Type02Reward` (was func_8003F9A4) - layer 1 type 0x2 handler (MAIN0 table slot 8007A114); psx_tomba reward.c; spawns spawnPoolType05Ring
- `8003bcd0 updateL2Type30Object` (was func_8003F124) - layer 2 type 0x30 handler (MAIN0 table slot 80077E5C)
- `8004985c updateL2Type02Object` (was func_8004985C) - layer 2 type 0x2 handler (MAIN0 table slot 80077DA4)
- `80030afc updateL4Type1BObject` (was func_80033F50) - layer 4 type 0x1B handler (MAIN0 table slot 80079DE0)
- `80030404 updateL4Type08Object` (was func_80030404) - layer 4 type 0x8 handler (MAIN0 table slot 80079D94)
- `8002b0b4 updateL8Type07Object` (was func_8002B0B4) - layer 8 type 0x7 handler (MAIN0 table slot 80079BA4)

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
- `800e9f74 awardAPByArea` (was FUN_800e9f74) - switch on the area calling that overlay's popup function with (points,x,y,z,1); the X000 one (func_80118040) calls addPlayerAP and spawns the digit object; callers pass 500 on item pickups
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
- `80102110 playerForcedModeDispatch` (was FUN_80102110) - copy of updatePlayer mode 6 (entered from scripted NPCs like FUN_801377b8 with a preset velX): steps 0,3,4,5,6,7 without pad control
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
- `8010b050 playerStepForcedLaunch` (was func_8010B050) - forced-mode steps 5/7: same rise as playerStepForcedJump with anim 0x2E and the preset velX (set by FUN_801377b8)
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
- `800ea284 updatePoolType05Object` (was func_800EA284) - pool type 5 handler (80079BC0): falls until probeCollisionAtDepthB, despawned offscreen
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
- `80113484 updateL2Type0EObject` (was FUN_80113484) - shared layer 2 type 0xE handler (slot 80077DD4; bite holder playerBiteHoldType0E)
- `80114890 updateL2Type1FObject` (was FUN_80114890) - shared layer 2 type 0x1F handler (slot 80077E18): thunk to the area's own code
- `8011191c updateL1Type0AObject` (was FUN_8011191c) - layer 1 type 10 handler (slot 8007A134): subtype 0/1 steps, short hit flash via +0x22/+0x6A
- `800f9684 playerPerchFlipLandCheck` (was FUN_800f9684) - playerStepFlipOntoPerch helper: probes the landing (probeCollisionAtDepthC, side tile response) and sets the landing anim
- `80103628 playerBiteHoldAttach` (was FUN_80103628) - playerBiteHoldGeneric helper: snaps the player onto the bitten object D_8009D2E8 (x, y - height +0x70), anim 0xD, per bitten type setup
- `8010cda8 playerSwimDirFromPadB` (was FUN_8010cda8) - playerStepSwim helper: swim direction +0x76 / angle +0xB6 from the pad bits D_8009D670 (when D_8009CE3D != 0xFF)
- `800ec5f4 spawnWeaponTrailSpark` (was FUN_800ec5f4) - weaponTrailOrbitStep helper: spawns a pool type 0x5C subtype 9 object at the trail object plus an offset
- `80111a50 spawnL1Type0AObject` (was FUN_80111a50) - allocates a layer 1 type 10 object (updateL1Type0AObject) at the player with subtype/slot/state from the arguments (spawnPlayerPowerEffect)
- `800f28a0 playerJumpArea3Float` (was FUN_800f28a0) (uncertain) - playerStepJump helper: in area 3 with D_8009C944 set, anim 0x35 and pad-driven velX / velY (setVelXFromPadLR, adjustVelYFromPadDown)
- `80106a18 playerStep48ByArea` (was FUN_80106a18) - player step 0x48: per-area handlers (overlay code at 8011B47C / 8011C758 / 8011D5F4), like playerStep2EByArea
- `800ea544 spawnPoolType05Ring` (was FUN_800ea544) - spawns 6 pool type 5 objects (updatePoolType05Object) at offsets D_80114A80 around x/y/z (layer-1 type 2 reward handler)
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
- `80134e5c updateA00L2Type34` (was func_80134E5C) - AREA00-only object handler: layer 2 type 0x34 slot (MAIN0 table 80077E6C) while AREA00 (X000) is loaded
- `80116d14 updateA00L5Type11` (was FUN_80116d14) - AREA00-only object handler: layer 5 type 0x11 slot (MAIN0 table 8007B354) while AREA00 (X000) is loaded
- `8011bf68 updateA00L4Type02` (was func_8011BF68) - AREA00-only object handler: layer 4 type 0x2 slot (MAIN0 table 80079D7C) while AREA00 (X000) is loaded
- `801288b8 updateA00L2Type2C` (was func_801288B8) - AREA00-only object handler: layer 2 type 0x2C slot (MAIN0 table 80077E4C) while AREA00 (X000) is loaded
- `80132174 updateA00L2Type08` (was FUN_80132174) - AREA00-only object handler: layer 2 type 0x8 slot (MAIN0 table 80077DBC) while AREA00 (X000) is loaded
- `8011b8c0 updateA00L4Type01` (was FUN_8011b8c0) - AREA00-only object handler: layer 4 type 0x1 slot (MAIN0 table 80079D78) while AREA00 (X000) is loaded
- `8012d294 updateA00L2Type2B` (was FUN_8012d294) - AREA00-only object handler: layer 2 type 0x2B slot (MAIN0 table 80077E48) while AREA00 (X000) is loaded
- `8011ec68 updateA00L4Type0A` (was FUN_8011ec68) - AREA00-only object handler: layer 4 type 0xA slot (MAIN0 table 80079D9C) while AREA00 (X000) is loaded
- `8012e8c4 updateA00L2Type06` (was func_8012E8C4) - AREA00-only object handler: layer 2 type 0x6 slot (MAIN0 table 80077DB4) while AREA00 (X000) is loaded
- `8011e838 updateA00L4Type09` (was func_8011E838) - AREA00-only object handler: layer 4 type 0x9 slot (MAIN0 table 80079D98) while AREA00 (X000) is loaded
- `8012e4c4 updateA00L2Type05` (was func_8012E4C4) - AREA00-only object handler: layer 2 type 0x5 slot (MAIN0 table 80077DB0) while AREA00 (X000) is loaded
- `8011f288 updateA00L4Type11` (was func_8011F288) - AREA00-only object handler: layer 4 type 0x11 slot (MAIN0 table 80079DB8) while AREA00 (X000) is loaded
- `8011ad98 updateA00PoolType14` (was func_8011AD98) - AREA00-only object handler: pool type 0x14 slot (MAIN0 table 80079BFC) while AREA00 (X000) is loaded
- `801236f0 updateA00L5Type0B` (was func_801236F0) - AREA00-only object handler: layer 5 type 0xB slot (MAIN0 table 8007B33C) while AREA00 (X000) is loaded
- `8012e1c0 updateA00L2Type04` (was FUN_8012e1c0) - AREA00-only object handler: layer 2 type 0x4 slot (MAIN0 table 80077DAC) while AREA00 (X000) is loaded
- `801328b4 updateA00L2Type09` (was FUN_801328b4) - AREA00-only object handler: layer 2 type 0x9 slot (MAIN0 table 80077DC0) while AREA00 (X000) is loaded
- `8011a744 updateA00PoolType0B` (was func_8011A744) - AREA00-only object handler: pool type 0xB slot (MAIN0 table 80079BD8) while AREA00 (X000) is loaded
- `80134404 updateA00L2Type14` (was func_80134404) - AREA00-only object handler: layer 2 type 0x14 slot (MAIN0 table 80077DEC) while AREA00 (X000) is loaded
- `801195f4 updateA00PoolType09` (was FUN_801195f4) - AREA00-only object handler: pool type 0x9 slot (MAIN0 table 80079BD0) while AREA00 (X000) is loaded
- `801224c0 updateA00L7Type01` (was func_801224C0) - AREA00-only object handler: layer 7 type 0x1 slot (MAIN0 table 80079FE8) while AREA00 (X000) is loaded
- `80134c20 updateA00L2Type15` (was FUN_80134c20) - AREA00-only object handler: layer 2 type 0x15 slot (MAIN0 table 80077DF0) while AREA00 (X000) is loaded
- `800ed024 updateL4Type00Object` (was FUN_800ed024) - layer 4 type 0x0 handler (MAIN0 table slot 80079D74)
- `80126fec updateA00L2Type01` (was FUN_80126fec) - AREA00-only object handler: layer 2 type 0x1 slot (MAIN0 table 80077DA0) while AREA00 (X000) is loaded
- `80118f90 updateA00PoolType03` (was FUN_80118f90) - AREA00-only object handler: pool type 0x3 slot (MAIN0 table 80079BB8) while AREA00 (X000) is loaded
- `80118438 updateA00PoolType01` (was FUN_80118438) - AREA00-only object handler: pool type 0x1 slot (MAIN0 table 80079BB0) while AREA00 (X000) is loaded
- `800edf94 updateL4Type03Object` (was FUN_800edf94) - layer 4 type 0x3 handler (MAIN0 table slot 80079D80)
- `8011d010 updateA00L4Type06` (was FUN_8011d010) - AREA00-only object handler: layer 4 type 0x6 slot (MAIN0 table 80079D8C) while AREA00 (X000) is loaded
- `801202a4 updateA00L4Type18` (was FUN_801202a4) - AREA00-only object handler: layer 4 type 0x18 slot (MAIN0 table 80079DD4) while AREA00 (X000) is loaded
- `80121870 updateA00L4Type26` (was FUN_80121870) - AREA00-only object handler: layer 4 type 0x26 slot (MAIN0 table 80079E0C) while AREA00 (X000) is loaded
- `8012dedc updateA00L2Type03` (was func_8012DEDC) - AREA00-only object handler: layer 2 type 0x3 slot (MAIN0 table 80077DA8) while AREA00 (X000) is loaded
- `80118bc0 updateA00PoolType02` (was FUN_80118bc0) - AREA00-only object handler: pool type 0x2 slot (MAIN0 table 80079BB4) while AREA00 (X000) is loaded
- `8011b2e8 updateA00PoolType24` (was func_8011B2E8) - AREA00-only object handler: pool type 0x24 slot (MAIN0 table 80079C3C) while AREA00 (X000) is loaded
- `80122724 updateA00L7Type02` (was func_80122724) - AREA00-only object handler: layer 7 type 0x2 slot (MAIN0 table 80079FEC) while AREA00 (X000) is loaded
- `8011f608 updateA00L4Type17` (was FUN_8011f608) - AREA00-only object handler: layer 4 type 0x17 slot (MAIN0 table 80079DD0) while AREA00 (X000) is loaded
- `801191b0 updateA00PoolType27` (was FUN_801191b0) - AREA00-only object handler: pool type 0x27 slot (MAIN0 table 80079C48) while AREA00 (X000) is loaded
- `8012236c updateA00L7Type00` (was FUN_8012236c) - AREA00-only object handler: layer 7 type 0x0 slot (MAIN0 table 80079FE4) while AREA00 (X000) is loaded
- `80136090 updateA00L2Type57` (was FUN_80136090) - AREA00-only object handler: layer 2 type 0x57 slot (MAIN0 table 80077EF8) while AREA00 (X000) is loaded
- `8011ab10 updateA00PoolType11` (was FUN_8011ab10) - AREA00-only object handler: pool type 0x11 slot (MAIN0 table 80079BF0) while AREA00 (X000) is loaded
- `80123a1c updateA00L5Type0C` (was FUN_80123a1c) - AREA00-only object handler: layer 5 type 0xC slot (MAIN0 table 8007B340) while AREA00 (X000) is loaded
- `80119468 updateA00PoolType08` (was FUN_80119468) - AREA00-only object handler: pool type 0x8 slot (MAIN0 table 80079BCC) while AREA00 (X000) is loaded
- `8011b6e0 updateA00PoolType59` (was FUN_8011b6e0) - AREA00-only object handler: pool type 0x59 slot (MAIN0 table 80079D10) while AREA00 (X000) is loaded
- `8011935c updateA00PoolType07` (was FUN_8011935c) - AREA00-only object handler: pool type 0x7 slot (MAIN0 table 80079BC8) while AREA00 (X000) is loaded
- `8011b0d4 updateA00PoolType16` (was FUN_8011b0d4) - AREA00-only object handler: pool type 0x16 slot (MAIN0 table 80079C04) while AREA00 (X000) is loaded
- `8011b1e0 updateA00PoolType17` (was FUN_8011b1e0) - AREA00-only object handler: pool type 0x17 slot (MAIN0 table 80079C08) while AREA00 (X000) is loaded
- `8011a63c updateA00PoolType0A` (was FUN_8011a63c) - AREA00-only object handler: pool type 0xA slot (MAIN0 table 80079BD4) while AREA00 (X000) is loaded
- `8011926c updateA00PoolType06` (was FUN_8011926c) - AREA00-only object handler: pool type 0x6 slot (MAIN0 table 80079BC4) while AREA00 (X000) is loaded
- `8011b7f0 updateA00PoolType5A` (was func_8011B7F0) - AREA00-only object handler: pool type 0x5A slot (MAIN0 table 80079D14) while AREA00 (X000) is loaded
- `8011f1d0 updateA00L4Type0D` (was func_8011F1D0) - AREA00-only object handler: layer 4 type 0xD slot (MAIN0 table 80079DA8) while AREA00 (X000) is loaded
- `80111ae0 updateL2Type07Object` (was func_80111AE0) - layer 2 type 0x7 handler (MAIN0 table slot 80077DB8)
- `800ee25c updateL4Type38Object` (was func_800EE25C) - layer 4 type 0x38 handler (MAIN0 table slot 80079E54)
- `80114a08 updateL2Type55Object` (was FUN_80114a08) - layer 2 type 0x55 handler (MAIN0 table slot 80077EF0)

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
  the screen): x001 801282d0 purplePigIdle (step 0: waits 0xC0/0x180 ticks, then turns to the player), 801285e4 purplePigWalk (step 1: walk anim, sfx 0x5E,
  movetab), 8012ab64 purplePigHit, 8012b184 purplePigDefeated; x009 801233f0 redFlowerHit, 80123ce8 redFlowerDefeated; x010 8012aae4 spearPigDefeated
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
  purple pig steps (uncertain): `80128a2c purplePigClimb` (step 2: turns d8c +-0x40 and moves 0x300/0x80 per tick along a wall, sfx 0x5E),
  `80129588 purplePigJump` (step 3: velV -0x400 / -0x500 hops); forest dwarf: `80135b48 forestDwarfCountCatch` (sets the GV2.sc0 bit of b0c,
  GCD.bcdb5 += 1 with printInfoMessage(count - 1, 2), GCD.bcec1..7 flags), `80135a14 forestDwarfAssignSlot` (uncertain: picks a free slot of
  GCD.bcec1[0..6]); `80137afc leafButterflyReleased` (b04 2 step 3 of updateLeafButterfly: after item 0x10 is used, flutters inside the box of
  the parent d90; uncertain)
- Koma pig (layer 2 type 2; MAIN0 `func_8004985C` calls the area's handler: area 0 X000 8012CCF8, area 1 x001 `8012efc4 updateKomaPig`, area 3
  x003 `80124b78 updateKomaPig`, area 4 x004 `80123750 updateKomaPig`). The identity is uncertain (the rendered strips are not readable): the
  handlers draw from SPR_DATA[2] with w1e 1, the pig sprite table of updatePurplePig / updateSpearPig, and type 2 is placed in the first room of
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
  b0c 1 planks take part in the purple pig scene GCD.bceae), `8011d4d0 updateHangingVine` (type 0x10: allocates 18 more layer 4 objects and links them
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
  from 0x3C,-0x50 for 0x3C ticks, collectable once falling), `80116fa8 updatePatrollingFlyer` (layer 2 type 0x2A, 1 placement, kind uncertain: 32 x 32
  box, palette 0x7952; `80116e54 patrollingFlyerMove` moves along its facing with a sine bob for one d88 cycle, hovers one cycle, then turns)
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
  (MAIN0 table 0x80077D9C): x001 `801262f8 purplePigContact` (type 0), `801240a8 komaPigContact` (type 2, area 1 branch), `8012687c
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
- x003 thrower enemy (layer 2 type 0x38, 13 placements, kind uncertain; SPR_DATA[3], anim table D_8013A698): `8012f524 updateThrowerEnemy`,
  `8012e6b0 throwerEnemyThrow` (subtype 1 state 3: spawns layer 2 type 0x39 shots), `8012f9a0 updateThrowerEnemyShot` (type 0x39: semi-transparent,
  speed 0x80 (subtype 0) or 0x480), `8011f508 throwerEnemyShotContact` (2 points of damage), `8011f634 throwerEnemyContact`; b04 2 steps:
  `8012eb2c throwerEnemyHit` (subtype 1 step 0), `8012f234 throwerEnemyGrabbed` / `8012ef3c throwerEnemyGrabbedB` (step 1 of subtype 0 / 1,
  spawnItemNotification(1, 6)), `8012f3b0 throwerEnemyDefeated`
- x003 `801281fc updateHornedEnemy` (layer 2 type 0x28, 12 placements; "horned" only from the badly composed strips, uncertain): b04 2 step 0
  `8012748c hornedEnemyHit`, step 1 `80127d48 hornedEnemyGrabbed` (spawnItemNotification(2, 6), counts the shakes in wb6), step 2 `80128060
  hornedEnemyDefeated`; `80120140 hornedEnemyContact` (FUN_80047484 type 0x28), `801256f4 hornedEnemyCheckWall` (wall / floor probe 0x18 ahead
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
- x001 contact callbacks: MAIN0 FUN_80045f00 (weapon/attack object vs layer 2 by the enemy type) `80125f04 purplePigWeaponHit` (0),
  `80126004 rollingBarrelWeaponHit` (0x11), `80125b88 segmentedCrawlerWeaponHit` (0x13); MAIN0 func_8004BBC0 (player grab probe vs layer 4, sets
  the contact kind +0x9E) `80126fe4 springVineGrabProbe` (5), `80126f64 flowerTowerBlockerGrabProbe` (0xB), `80126e8c bendingBranchGrabProbe` (0xE),
  `8012767c seesawPlankGrabProbe` (0x14; `80127074 seesawPlankProbeFace` tests one face), `80126d2c swingingRopeGrabProbe` (0x15); `8012f988
  updateRollingBoulder` (layer 4 type 0x23 body, via the 8012f968 thunk: rolls down the slope, sets an area flag and turns into type 0xC; uncertain kind)
  with `80126cc4 rollingBoulderPlayerCollision`, `80125fbc rollingBoulderObjectCollision`; `80124410 grabObjectContact` (FUN_80047484 type 0x1F in
  area 1: holds the touching object; uncertain); `80127700 landOnSlopedBox` (stand-on test for a sloped box; uncertain)
- x001 purple pig steps 4..8: `80129b2c purplePigFall`, `80129dac purplePigRun` (random movetab/timer), `8012a290 purplePigCarryPlayer` (holds the player
  and jumps; uncertain), `8012a5cc purplePigDrop`, `8012a794 purplePigCrawlWall` (uncertain); `80128110 purplePigChooseStep` (next step from the distance
  to home/player); `80123b30 updateFallingSparkleItem` (item that falls leaving type 0x31 sparkles; uncertain), `80123e50 spawnItemEffectFlyingItem`
  (applyItemEffect case 6: layer 5 type 6 subtype 0x13 with velH)
