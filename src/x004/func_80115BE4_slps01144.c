// VERSION SLPS_01144 x04 80115BE4 440
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { char p[6]; unsigned char b6, b7; } E8;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x24 - 8];
    void *anim;
    char p28[0x3f - 0x28];
    unsigned char b3f;
    char p40[0x68 - 0x40];
    E8 *p68;
    char p6c[0x7c - 0x6c];
    int d7c;
} O;

extern unsigned char D_8009E4E0, D_8009E94A;
typedef struct { unsigned char b; } SB;
typedef struct { unsigned short w; } SW;
extern SB D_8009E4E7, D_8009E4EA;
extern SW D_8009E50A, D_8009E52A;
extern short SPAWN_AXIS_SWAP;
extern void func_8002AAC4(O *);
extern unsigned int func_80029230(O *);
extern unsigned int func_80029890(O *);
extern int func_80115A14_slps01144(O *);
extern void func_800217A0(void);

void func_80115BE4_slps01144(O *o)
{
    if (D_8009E4E0) {
        func_8002AAC4(o);
        return;
    }
    switch (o->state) {
    case 0:
        if (func_80029230(o) & func_80029890(o)) {
            o->substep = 0;
            o->state++;
        }
        break;
    case 1:
        o->state++;
        D_8009E94A = 1;
        break;
    case 2:
        if (func_80115A14_slps01144(o)) {
            o->state++;
            D_8009E94A = 0;
        }
        break;
    case 3:
        if (o->b3f == 0) SPAWN_AXIS_SWAP = (SPAWN_AXIS_SWAP + 1) & 1;
        else SPAWN_AXIS_SWAP = (SPAWN_AXIS_SWAP - 1) & 1;
        func_800217A0();
        o->d7c = SPAWN_AXIS_SWAP * 0x5a00;
        o->anim = 0;
        o->subtype = 0;
        o->state = 0;
        D_8009E4E7.b = 0;
        D_8009E4EA.b = 0;
        D_8009E50A.w = o->p68->b6;
        D_8009E52A.w = o->p68->b7;
        break;
    }
}
#endif
