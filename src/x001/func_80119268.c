// FUNC 80119268 244 X001
// MATCHING 80119268 244
typedef struct P { char p[0x24]; int w24; char q[0x3c - 0x28]; int w3c; char r[0x64 - 0x40]; int w64; } P;
extern int D_1F8002D4;
extern int D_8013E684[];
extern int ObjCullRegister(char *);
extern int AnimAdvance(char *);
extern void AnimLoadDuration(char *);
extern void ObjFree(char *);

void func_80119268(char *o)
{
    unsigned char s = o[4];
    switch (s) {
    case 0:
        o[4] = s + 1;
        *(short *)(o + 0x1e) = 8;
        o[0xa] = 1;
        *(int *)(o + 0x64) = 0x1400;
        *(signed char *)&o[0xf] = -0x14;
        o[0xd] = 0x80;
        ((P *)o)->w3c = D_1F8002D4;
        ((P *)o)->w24 = D_8013E684[(unsigned char)o[3]];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o) != 0)
            o[4] = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
