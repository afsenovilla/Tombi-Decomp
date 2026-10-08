// FUNC 8010186c 204 X019
// MATCHING 8010186c 204
// FLAGS -O2 -G0 -fno-schedule-insns
typedef struct P { char p0[8]; unsigned char b8; char p1[0x20-9]; short h20; char p2[0x28-0x22]; unsigned short h28; unsigned short h2a; char p3[2]; unsigned short h2e; } P;
typedef struct O { char p0[6]; unsigned char state; char p1[0x24-7]; void *anim; char p2[0x7c-0x28]; short vx; short vy;
  char p3[0x8c-0x80]; int d8c; char p4[0x9c-0x90]; unsigned char b9c; char p5[0xa4-0x9d]; unsigned char ba4; unsigned char ba5;
  char p6[0xac-0xa6]; unsigned char bac; char p7[0xb0-0xad]; short hb0; short hb2; } O;
extern P *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern unsigned char DAT_8009d2af;
extern unsigned char DAT_8009c93a;
extern char LAB_80010748[];
extern void FUN_8001fe94(O *, int);

void FUN_8010186c(O *o)
{
    P *p;
    unsigned char *q;
    int i;
    DAT_8009c330->b8 = 0;
    i = o->hb0;
    o->ba4 = 0;
    o->ba5 = 0;
    o->b9c = 0;
    o->bac = 0;
    p = DAT_8009c330;
    o->hb2 = 0;
    o->vx = 0;
    o->vy = 0;
    o->d8c = DAT_801152e8[i];
    p->h20 = 0;
    o->anim = LAB_80010748;
    FUN_8001fe94(o, 0);
    p = DAT_8009c330;
    p->h2e = 0xffff;
    p->h28 = 0xffff;
    p->h2a = 0xffff;
    q = &DAT_8009d2af;
    if (*q == 0) {
        *q = 1;
        DAT_8009c93a = 1;
    }
    o->state++;
}
