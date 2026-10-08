// FUNC 80117494 2352 X003
// MATCHING 80117494 2352
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x16 - 8];
    short yw;
    char p18[0x1a - 0x18];
    short bw;
    char p1c[0xa0 - 0x1c];
    unsigned char ba0, ba1, ba2;
    char pa3[0xa8 - 0xa3];
    unsigned char ba8;
} PL;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} O;
typedef void (*Fn)(O *);

extern PL D_800A6038;
extern Fn D_80079A98[];
extern unsigned char D_8009CDA2[];
extern unsigned char D_8009C93A, D_8009C93F, D_8009C975;
extern unsigned char D_8009CEF0, D_8009CDC8, D_8009CDC3, D_8009CDC5, D_8009CDC7;
extern unsigned short D_8009C982;
extern unsigned short D_8009C962;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern char D_8001D6A4[];
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028b40(O *);
extern void FUN_80027c74(O *);
extern void FUN_800277f8(O *);
extern void func_80029078(O *);
extern int FUN_8002715c(int);
extern int FUN_800270a0(O *, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8001eacc(void);
extern void stopBgm(int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, char *);
extern void func_80029CB4(O *);

#define XU (*(unsigned short *)&D_1F80016A)
#define ZU (*(unsigned short *)&D_1F80016E)

void func_80117494(O *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDA2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) D_8009CDA2[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        FUN_800277f8(o);
        func_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        switch (D_8009C962) {
        case 0:
        case 4: {
            unsigned short *p = &D_8009C982;
            switch (*p) {
            case 0:
                if (D_1F80016A < 0x14 && D_1F80016E >= -0x59) {
                    if (D_8009CEF0 != 0) FUN_800270a0(o, 1, 0);
                } else if (D_1F80016A < 0xa7 && D_1F800172 < -0x5a) {
                    FUN_800270a0(o, 1, 1);
                } else if (D_1F80016A >= 0xbc1 && D_1F80016E >= -0x489) {
                    FUN_800270a0(o, 1, 2);
                }
                if (D_8009CDC8 != 0xff) break;
                if ((unsigned)(XU - 0x778) >= 0x18) break;
                if ((unsigned short)(ZU + 0x56a) >= 0x10) break;
                D_800A6038.ba0 = 1;
                D_800A6038.ba8 = 10;
                if (D_1F800172 > 0) FUN_8002715c(0x784);
                break;
            case 4:
                if (D_8009CDC3 != 0xff) FUN_8005a9a4(0x1f, 0);
                *p = 0;
                break;
            default:
                D_8009C982 = 0;
                break;
            }
            break;
        }
        case 1:
        case 5: {
            unsigned char *c;
            unsigned short *p = &D_8009C982;
            if (*p != 0) goto clr;
            if (D_800A6038.ba2 != 0) {
                if ((unsigned)(XU - 0xcac) < 0x60) FUN_8002715c(0xcdc);
                else if ((unsigned)(XU - 0x12dd) < 0x60) FUN_8002715c(0x130d);
                else if ((unsigned)(XU - 0x155b) < 0x60) FUN_8002715c(0x158b);
            }
            if (D_800A6038.ba2 == 2 && (unsigned)(XU - 0x107a) < 0x40 && (unsigned short)(ZU + 0x78e) < 0x20)
                FUN_8002715c(0x109a);
            if (D_8009CDC5 == 0xff && (unsigned)(XU - 0xc72) < 0x14 && (unsigned short)(ZU + 0x63e) < 0x10) {
                D_800A6038.ba0 = 1;
                D_800A6038.ba8 = 0xc;
                if (D_1F800172 > 0) FUN_8002715c(0xc7c);
            }
            if ((unsigned)(XU - 0x10a1) < 0x10 && (unsigned short)(ZU + 0x5b4) < 0x10) {
                D_800A6038.ba0 = 1;
                if (D_1F800172 > 0) {
                    FUN_8002715c(0x10a9);
                    if (D_1F800172 >= 0x51) {
                        o->step = 6;
                        D_800A6038.active = 5;
                        D_800A6038.b04 = 5;
                        D_800A6038.visible = 0;
                        D_800A6038.step = 0x40;
                        D_8009C93F = 1;
                        break;
                    }
                }
            }
            c = &D_8009CDC7;
            if (*c == 2) {
                *c = 3;
                FUN_8001eacc();
                stopBgm(1);
                o->step = 4;
                D_8009C975 = 3;
                D_8009C93F = 1;
                break;
            }
            if (D_1F800172 == 0 && D_1F80016A < 0xb95 && D_1F80016E >= -0x485) {
                FUN_800270a0(o, 1, 0);
                break;
            }
            if (D_1F80016E < -0x986) {
                if ((unsigned)(XU - 0x1536) >= 0x35) break;
                if (D_1F800172 <= 0) break;
                FUN_8002715c(0x1550);
                if (D_1F800172 < 0x2e) break;
                FUN_800270a0(o, 1, 1);
                break;
            }
            if (D_1F80016E < -0x937) break;
            if ((unsigned)(XU - 0x153e) >= 0x1d) break;
            if (D_1F800172 < -0x59) break;
            FUN_8002715c(0x154d);
            if (D_1F800172 < -0x2c) break;
            FUN_800270a0(o, 1, 2);
            break;
        clr:
            *p = 0;
            break;
        }
        case 2: {
            unsigned short *p = &D_8009C982;
            if (*p == 0) {
                if (D_1F80016A < 0x22) {
                    if ((unsigned short)(ZU + 0x16d) < 0x20) FUN_800270a0(o, 1, 0);
                    break;
                }
                if (D_1F80016A >= 0xb00 && D_1F80016E >= -0x4c5) {
                    FUN_800270a0(o, 1, 1);
                    break;
                }
                if (D_1F80016E >= -0x730) break;
                if ((unsigned)(XU - 0x5b3) >= 0xc0) break;
                FUN_800270a0(o, 1, 2);
            } else {
                *p = 0;
            }
            break;
        }
        case 3: {
            unsigned short *p = &D_8009C982;
            switch (*p) {
            case 0:
                if (D_1F80016A < 0x20 && (unsigned short)(ZU + 0x268) < 0x20) FUN_800270a0(o, 1, 0);
                break;
            case 1:
                *p = 0;
                break;
            }
            break;
        }
        }
        break;
    case 3:
        D_80079A98[o->subtype](o);
        break;
    case 4:
        if (D_8009C975 != 1) break;
        o->step++;
        D_1F8001CC = 1;
        D_1F8001CD = 6;
        D_1F8001C6 = 2;
        SoundStopAll();
        ThreadCreate(1, D_8001D6A4);
        break;
    case 5:
        if (D_1F8001CC != 0) break;
        o->subtype = 1;
        o->step = 3;
        D_8009C982 = 3;
        D_8009CDA2[0] = 1;
        break;
    case 6:
        D_800A6038.yw -= 2;
        D_800A6038.bw -= 2;
        if (D_800A6038.bw < 0) D_800A6038.bw = 0;
        if (D_800A6038.yw < -0x78e) {
            D_800A6038.yw = -0x7ee;
            D_800A6038.visible = 1;
            D_800A6038.active = 2;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->step++;
        }
        func_80029CB4(o);
        break;
    case 7:
        if (D_800A6038.yw >= -0x787) {
            D_800A6038.active = 1;
            D_800A6038.bw = 0;
            D_8009C93F = 0;
            o->step = 2;
        }
        func_80029CB4(o);
        break;
    }
}
