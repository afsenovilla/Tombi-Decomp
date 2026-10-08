# Game object structure (deduced from MAIN0 + X000; all to be confirmed)

Functions receive an `obj` pointer and access it by offset. Fields seen:

| Off | Size | Use |
|---|---|---|
| +0x00 | u8 | object active (0 = free) |
| +0x01 | u8 | visible (set by ObjCullRegister) |
| +0x02 | u8 | type |
| +0x03 | u8 | subtype |
| +0x06 | u8 | substate (switch of the state machines) |
| +0x10 | s32 | axis A coordinate, 16.16 (integer part at +0x12) |
| +0x14 | s32 | Y, 16.16 (integer part at +0x16) |
| +0x18 | s32 | axis B coordinate, 16.16 (integer part at +0x1A) |
| +0x1C | u8 | category (&0x7F: 1..8 = visible list) |
| +0x24 | ptr | current animation script (8-byte entries; +6 = duration with flags 0x4000/0x8000/0xC000) |
| +0x28 | ptr | per-frame movement table |
| +0x2C | u16 | animation timer |
| +0x2E | u16 | frame index / orientation |
| +0x40 | ptr | pointer to the "horizontal" coordinate (points to +0x10 or +0x18) |
| +0x44 | ptr | pointer to the "depth" coordinate (points to the other one) |
| +0x6C..+0x72 | 4 x s16 | collision box (offsets) |
| +0x7E | s16 | Y velocity |
| +0x80 | s16 | horizontal velocity |
| +0x82 | s16 | velocity (Y/depth) |

## Swappable horizontal axis
`ObjAlloc` makes +0x40/+0x44 point to +0x10/+0x18 **or the other way round** depending on bit 0 of the
scratchpad variable `1F8001C8`. That is, the (2.5D) game can swap the X and Z axes
of all objects with a single bit. That is why 1F8001C8 is a camera/level orientation and
not a frame counter as I initially assumed.

## Global player pointer (8009C330) and 8009C338
Several functions in X000 and in the 3 large MAIN0 state machines
(800317C0, 8003566C, 80033B44) read `_DAT_8009C330` and `_DAT_8009C338`. In
`PlayerSetAnimIfChanged` (800EEB5C) they use +0x2C/+0x2E of that object as "current / previous
animation id", **different** from the use of +0x2C/+0x2E in TObj (timer/frame). It is likely
a different structure (player state) and not a TObj.
