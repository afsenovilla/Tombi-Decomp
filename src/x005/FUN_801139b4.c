// FUNC 801139b4 216 X005
// MATCHING 801139b4 216
typedef struct V { char c[12]; } V;
typedef struct O {
    char p0[5]; unsigned char f5; unsigned char state;
    char p1[0x10 - 7]; V v;
    char p2[0x20 - 0x1c]; unsigned short timer;
    char p3[0x6a - 0x22]; char f6a;
    char p4[0x90 - 0x6b]; int d90;
} O;
extern void FUN_8001fec0(O *);
extern int FUN_8002dcf0(int, int, V *, int);

void FUN_801139b4(O *o)
{
    V v;
    switch (o->state) {
    case 0:
        v = o->v;
        *(unsigned short *)(v.c + 6) = *(unsigned short *)(v.c + 6) - 0x40;
        o->d90 = FUN_8002dcf0(13, 2, &v, (short)o->timer);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        o->timer--;
        if ((short)o->timer == 0) {
            o->f6a = 0;
            o->f5 = 0;
            o->state = 0;
        }
        break;
    }
}
