// VERSION SLPM_80165 slpm80165 8001C4F4 288
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of setEventStarted (src/setEventStarted.c): message, sound 0x29 and the pickup object (allocated
// inline: active 1, kind 0x0D) only when the event's table byte is set, then always the AP award of its entry.
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char DAT_8009cda5; /* SCRIPT_FLAGS[1] */
extern int GAME;
extern unsigned char D_8006B284[];
extern unsigned char D_8006B364[];
extern short *D_8006B354[];
extern void func_8001C6E0(int, int, int, int);
extern void func_8001CE14(int, int);
extern void func_8001E3D8(int);
extern void func_800266D0(int);
extern unsigned char *ObjAlloc(void);

int func_8001C4F4(int p, int b, int c)
{
    if (SCRIPT_FLAGS[p] == 0) {
        if (p == 1) {
            if (GAME == 0) DAT_8009cda5++;
        } else {
            SCRIPT_FLAGS[p]++;
        }
        if (D_8006B284[p] != 0) {
            unsigned char *o;
            func_8001C6E0(p, 0, 0x3c, c);
            func_8001CE14(p, 0);
            func_8001E3D8(0x29);
            o = ObjAlloc();
            if (o != 0) {
                o[0] = 1;
                o[2] = 0xd;
                o[0xc] = 0;
            }
        }
        func_800266D0(D_8006B354[D_8006B364[p]][0]);
    }
    return SCRIPT_FLAGS[p];
}
#endif
