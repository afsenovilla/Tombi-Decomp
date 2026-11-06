// FUNC 801380b8 352 X000
// MATCHING 801380b8 352
// VERSION SLPS_01144 x01 80139E94 536
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009E95A, D_800A7BE1, D_8009E4EA, D_800A7C38;
extern unsigned int D_8009E52C;
extern unsigned short D_800A7B8E;
extern unsigned short *PLAYER_H;
extern unsigned char *func_8001798C(void);
extern void func_8013822C(void *);
extern void func_80017F54(void *);

static inline short talkCheck(unsigned char *o)
{
    unsigned char *p = *(unsigned char **)(o + 0x1c);
    short n = 0;
        if (D_8009E95A >= 2)
            return 0;
        {
            if ((D_8009E52C & 0x100) == 0 && D_800A7BE1 != 0 &&
                (unsigned short)(D_800A7B8E + 1290) < 16 &&
                (unsigned short)(PLAYER_H[1] - ((unsigned short *)*(unsigned char **)(p + 0x40))[1] + 48) < 96) {
                if (*(unsigned short *)(o + 0x24) != 0)
                    n = 1;
                else if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C8) {
                    *(unsigned short *)(o + 0x24) = 1;
                    n = 1;
                    D_8009E4EA = 1;
                    D_800A7C38 = 1;
                }
            }
        }
    return n;
}

void func_80139E94(unsigned char *o)
{
    unsigned char *q;
    switch (o[4]) {
    case 0:
        if (D_8009E95A < 2 && (D_8009E52C & 0x100) == 0) {
            q = func_8001798C();
            if (q != 0) {
                q[0] = 1;
                q[2] = 22;
                q[10] = 2;
                *(short *)(q + 0x12) = 4187;
                *(short *)(q + 0x16) = -1286;
                *(short *)(q + 0x2e) = 0;
                q[3] = 0;
                q[12] = 0;
                *(short *)(q + 0x1a) = 1146;
                *(unsigned char **)(o + 0x1c) = q;
            }
            o[4] = 1;
        } else
            o[4] = 3;
        break;
    case 1:
        if (talkCheck(o) != 0)
            func_8013822C(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        func_80017F54(o);
        break;
    }
}
#else
extern unsigned char DAT_a, DAT_b, DAT_c;
extern unsigned char *allocObjectLayer2(void);
extern short charlesTalkCheck(void *);
extern void updateCharlesTalkScene(void *);
extern void FUN_80018980(void *);

void updateCharlesEventObj(unsigned char *o)
{
    unsigned char *q;
    unsigned char **p;
    switch (o[4]) {
    case 0:
        p = (unsigned char **)(o + 0x1c);
        if (DAT_a == 0xff)
            o[4] = 3;
        if (DAT_b < 6) {
            q = allocObjectLayer2();
            if (q != 0) {
                q[0] = 1;
                q[2] = 0x15;
                if (DAT_c == 0) {
                    *(short *)(q + 0x2e) = 1;
                    *(short *)(q + 0x12) = 0x140;
                    *(short *)(q + 0x16) = -0x32;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0x63;
                } else {
                    *(short *)(q + 0x12) = 0x138;
                    *(short *)(q + 0x2e) = 0;
                    *(short *)(q + 0x16) = -0x11c;
                    *(short *)(q + 0x1a) = 0;
                    q[3] = 0;
                }
                *p = q;
            }
            o[4] = 1;
        } else
            o[4] = 3;
        break;
    case 1:
        if (charlesTalkCheck(o) != 0)
            updateCharlesTalkScene(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
#endif
