// FUNC 80116934 428 X004
// MATCHING 80116934 428
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

extern unsigned char D_8009C938, D_8009C93F[], D_8009C942[];
extern unsigned char D_8009CDA2;
extern unsigned short D_8009C962[], D_8009C982[];
extern short D_1F8001C8;
extern void func_80029CB4(O *);
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028a80(O *);
extern int func_80116764(O *);
extern void FUN_80020828(void);

void func_80116934(O *o)
{
    if (D_8009C938) {
        func_80029CB4(o);
        return;
    }
    switch (o->state) {
    case 0:
        if (FUN_80028420(o) & FUN_80028a80(o)) {
            o->substep = 0;
            o->state++;
        }
        break;
    case 1:
        o->state++;
        D_8009CDA2 = 1;
        break;
    case 2:
        if (func_80116764(o)) o->state++;
        break;
    case 3:
        D_8009CDA2 = 0;
        if (o->b3f == 0) D_1F8001C8 = (D_1F8001C8 + 1) & 1;
        else D_1F8001C8 = (D_1F8001C8 - 1) & 1;
        FUN_80020828();
        o->d7c = D_1F8001C8 * 0x5a00;
        o->anim = 0;
        o->subtype = 0;
        o->state = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C962[0] = o->p68->b6;
        D_8009C982[0] = o->p68->b7;
        break;
    }
}
