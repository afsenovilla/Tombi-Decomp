// VERSION SLPM_80165 slpm80165 8001C614 184
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of setEventComplete (src/setEventComplete.c): the message and sound only when the event's table
// byte is set (sound command 0x2B), then always the AP award of the event's entry.
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char D_8006B284[];
extern unsigned char D_8006B364[];
extern short *D_8006B354[];
extern void func_8001C6E0(int, int, int, int);
extern void func_8001CE14(int, int);
extern void func_8001E3D8(int);
extern void func_800266D0(int);

int func_8001C614(int i, int arg)
{
    if (SCRIPT_FLAGS[i] != 0xff) {
        unsigned char t = D_8006B284[i];
        SCRIPT_FLAGS[i] = 0xff;
        if (t != 0) {
            func_8001C6E0(i, 1, 1, arg);
            func_8001CE14(i, 1);
            func_8001E3D8(0x2b);
        }
        func_800266D0(D_8006B354[D_8006B364[i]][1]);
    }
    return SCRIPT_FLAGS[i];
}
#endif
