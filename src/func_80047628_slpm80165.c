// VERSION SLPM_80165 slpm80165 80047628 152
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: draws one sprite frame (picked by the byte at D_8009A830) and a second one 4 lower through
// func_800480A8, then a draw-mode packet (AddDrawMode 0x15, 1).
typedef struct O { char p[0x24]; unsigned short **frames; char p2[0x30 - 0x28]; unsigned short *frame; } O;
extern unsigned char D_8009A830;
extern void func_800480A8(O *, int, int, int);
extern void func_80047FFC(int, int);

void func_80047628(O *o, short x, int y)
{
    func_800480A8(o, x, (short)y, *o->frames[D_8009A830 - 1]);
    func_800480A8(o, x, (short)(y - 4), *o->frame);
    func_80047FFC(0x15, 1);
}
#endif
