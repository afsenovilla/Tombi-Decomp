// VERSION SLPS_01144 x00 800EB714 384
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
/* Two retail functions in one splat entry: a small object updater, then the GAME dispatcher. */
typedef struct {
    char p0[4];
    unsigned char b04;
    char p1[5];
    char b0a;
    char p2;
    unsigned char b0c;
    char p3[0x21];
    unsigned short s2e;
    char p4[0x3c];
    short s6c;
    char p5[0x18];
    int i88;
} O;

extern unsigned short func_80113DA4[];
extern unsigned short GAME;
extern void func_80020FF0(O *, int);
extern void func_80017E60(void);
extern void func_80116E08_slps01144(void);
extern void func_80116F70(void);
extern void func_80117764(void);
extern void func_80118EE4(void);
extern void func_80115C70(void);
extern void func_80115198(void);
extern void func_8011735C(void);
extern void func_801165C0(void);
extern void func_801151F8(void);

void func_800EB714(O *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->s6c = func_80113DA4[o->b0c];
        if (o->s2e) {
            o->i88 = -1024;
            o->b0a = 17;
        }
        break;
    case 1:
        func_80020FF0(o, o->s6c);
        break;
    case 2:
        break;
    case 3:
        func_80017E60();
        break;
    }
}

void func_800EB7C8(void)
{
    /* per area (GAME.selectedArea); the retail jump table at the overlay start has the default for areas 2, 5, 8
       and 11 (the first link of the JP programs found the cases numbered 0..8) */
    switch (GAME) {
    case 0: func_80116E08_slps01144(); break;
    case 1: func_80116F70(); break;
    case 3: func_80117764(); break;
    case 4: func_80118EE4(); break;
    case 6: func_80115C70(); break;
    case 7: func_80115198(); break;
    case 9: func_8011735C(); break;
    case 10: func_801165C0(); break;
    case 12: func_801151F8(); break;
    }
}
#endif
