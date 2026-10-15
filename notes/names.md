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
