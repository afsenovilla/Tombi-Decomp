#ifndef TOMBA_ANIM_H
#define TOMBA_ANIM_H
/* Animation scripts, as read by AnimAdvance / AnimAdvanceWithBox / AnimJump (MAIN0) and set by the object code
 * (o->anim = script; see notes/formats.md "Animations").
 *
 * A script is a stream of 8-byte entries
 *     u16 frame;      frame of the object's sprite (WPP / SPR) table
 *     u16 box;        hit box index (AnimAdvanceWithBox copies D_80138FD8[box * 4])
 *     u16 arg;        per-entry argument (speed profile of the jump scripts, knock back, ...), mostly 0
 *     u16 durFlags;   ticks the frame is shown (& 0x3FFF) | what happens when they are over (ANIMF_*)
 * After an entry with ANIMF_JUMP or ANIMF_JUMP_END the next 4 bytes are not an entry but the address of the entry to
 * continue at (a loop, or the start of another script); the stream itself goes on after them.
 *
 * Because of those 4-byte pointers the stream is not an array of 8-byte structs: a script is written as an array of
 * AnimWord (u32) with the macros below, one entry per line, so that it compiles to the retail bytes. */
typedef unsigned int AnimWord;

#define ANIMF_NEXT     0x0000  /* go on with the next entry */
#define ANIMF_JUMP     0x4000  /* continue at the address that follows */
#define ANIMF_END      0x8000  /* stay on this frame; the advance function returns 1 every durFlags ticks */
#define ANIMF_JUMP_END 0xC000  /* continue at the address that follows and return 1 */

#define ANIM_W(lo, hi) ((AnimWord)(unsigned short)(lo) | ((AnimWord)(unsigned short)(hi) << 16))

/* frame shown `dur` ticks, then the next entry */
#define ANIM(frame, box, arg, dur)        ANIM_W(frame, box), ANIM_W(arg, (dur) | ANIMF_NEXT)
/* frame shown `dur` ticks, then the script stops on it */
#define ANIM_STOP(frame, box, arg, dur)   ANIM_W(frame, box), ANIM_W(arg, (dur) | ANIMF_END)
/* frame shown `dur` ticks, then continue at `target` (an AnimWord address) */
#define ANIM_JUMP(frame, box, arg, dur, target) \
    ANIM_W(frame, box), ANIM_W(arg, (dur) | ANIMF_JUMP), (AnimWord)(target)
/* same, and the advance function reports the end of the script */
#define ANIM_JUMP_STOP(frame, box, arg, dur, target) \
    ANIM_W(frame, box), ANIM_W(arg, (dur) | ANIMF_JUMP_END), (AnimWord)(target)

#endif
