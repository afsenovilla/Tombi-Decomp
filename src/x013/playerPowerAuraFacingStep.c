// FUNC 80111590 388 X013
// MATCHING 80111590 388
typedef struct T { int **p; int a, b; } T;
typedef struct O { unsigned char b0, b1, b2, b3, b4, b5, state, b7; char p0[4]; unsigned char bc, bd, be, bf;
  char p1[0x16-0x10]; short y; char p2[0x20-0x18]; short timer; char p3[2]; int *anim; char p4[0x40-0x28];
  short *h; short *d; char p5[0x6b-0x48]; unsigned char b6b; char p6[0xa5-0x6c]; unsigned char ba5; } O;
extern void FUN_8001fe6c(O *);
extern void FUN_8001fec0(O *);
extern T DAT_8011592c[];
extern unsigned char PLAYER_VISIBLE, PLAYER_DRAW_DEPTH[];
extern unsigned short PLAYER_ANIM_FRAME, PLAYER_Y[];
extern short *PLAYER_H;
extern unsigned short *PLAYER_D[];

void playerPowerAuraFacingStep(O *o)
{
    unsigned char b;
    short v;
    unsigned char c;
    switch (o->state) {
    case 0:
        o->anim = DAT_8011592c[o->b3].p[o->bc];
        FUN_8001fe6c(o);
        o->timer = 0xb4;
        o->ba5 = 1;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->timer--;
        o->b1 = PLAYER_VISIBLE;
        if (o->timer >= 0x3d) {
            c = o->b6b;
            if (c < 0x7f)
                b = c + 8;
            else
                b = c;
        } else if (o->timer < 0) {
            o->b6b = 0;
            o->b4 = 3;
            goto L;
        } else {
            b = o->b6b;
            if (b != 0)
                b = b - 2;
            else
                b = 0;
        }
        o->b6b = b;
    L:
        {
            short *h = o->h;
            int w;
            w = PLAYER_H[1];
            if (PLAYER_ANIM_FRAME & 1) h[1] = w + 2;
            else h[1] = w - 2;
        }
        o->y = PLAYER_Y[0] - 8;
        o->d[1] = PLAYER_D[0][1];
        o->bf = PLAYER_DRAW_DEPTH[0] + 1;
    }
}
