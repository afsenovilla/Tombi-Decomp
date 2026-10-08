// FUNC 8011dd48 1116 X003
// MATCHING 8011dd48 1116
#include "TOBJ.H"
typedef struct {
    unsigned char p0[8];
    unsigned char b8;
    unsigned char p9[0x17];
    short w20;
} PL;
typedef struct {
    unsigned char active;   /* +0x00 */
    unsigned char visible;   /* +0x01 */
    unsigned char type;   /* +0x02 */
    unsigned char subtype;   /* +0x03 */
    unsigned char b04;   /* +0x04 */
    unsigned char step;   /* +0x05 */
    unsigned char state;   /* +0x06 */
    unsigned char substep;   /* +0x07 */
    short w08;   /* +0x08 */
    unsigned char b0a;   /* +0x0a */
    unsigned char b0b;   /* +0x0b */
    unsigned char b0c;   /* +0x0c */
    unsigned char b0d;   /* +0x0d */
    unsigned char _pad0e[0x1];
    unsigned char b0f;   /* +0x0f */
    Fix16 a;   /* +0x10 */
    Fix16 y;   /* +0x14 */
    Fix16 b;   /* +0x18 */
    unsigned char category;   /* +0x1c */
    unsigned char b1d;   /* +0x1d */
    short w1e;   /* +0x1e */
    short timer;   /* +0x20 */
    short w22;   /* +0x22 */
    void * anim;   /* +0x24 */
    void * movetab;   /* +0x28 */
    unsigned short animTimer;   /* +0x2c */
    unsigned short animFrame;   /* +0x2e */
    int d30;   /* +0x30 */
    int d34;   /* +0x34 */
    int d38;   /* +0x38 */
    int d3c;   /* +0x3c */
    Fix16 * h;   /* +0x40 */
    Fix16 * d;   /* +0x44 */
    short w48;   /* +0x48 */
    short w4a;   /* +0x4a */
    short w4c;   /* +0x4c */
    short w4e;   /* +0x4e */
    short w50;   /* +0x50 */
    short w52;   /* +0x52 */
    unsigned char _pad54[0x2];
    short w56;   /* +0x56 */
    unsigned short w58;   /* +0x58 */
    unsigned char _pad5a[0x2];
    short w5c;   /* +0x5c */
    unsigned char _pad5e[0x2];
    int d60;   /* +0x60 */
    int d64;   /* +0x64 */
    unsigned char b68;   /* +0x68 */
    unsigned char b69;   /* +0x69 */
    unsigned char b6a;   /* +0x6a */
    unsigned char b6b;   /* +0x6b */
    short box0;   /* +0x6c */
    short box1;   /* +0x6e */
    short box2;   /* +0x70 */
    short box3;   /* +0x72 */
    short w74;   /* +0x74 */
    short w76;   /* +0x76 */
    short w78;   /* +0x78 */
    short w7a;   /* +0x7a */
    short velX;   /* +0x7c */
    short velY;   /* +0x7e */
    short velH;   /* +0x80 */
    short velV;   /* +0x82 */
    int d84;   /* +0x84 */
    int d88;   /* +0x88 */
    int d8c;   /* +0x8c */
    int d90;   /* +0x90 */
    int d94;   /* +0x94 */
    short w98;   /* +0x98 */
    short w9a;   /* +0x9a */
    unsigned char b9c;   /* +0x9c */
    unsigned char b9d;   /* +0x9d */
    unsigned char b9e;   /* +0x9e */
    unsigned char b9f;   /* +0x9f */
    unsigned char ba0, ba1, ba2, ba3;
    unsigned char ba4, ba5, ba6, ba7;
    unsigned char ba8, ba9, baa, bab;
    unsigned char bac, bad, bae, baf;
    short wb0;
    short wb2;
    unsigned char pb4[0x42];
    unsigned short wf6;
} TP;

extern PL *D_8009C330;
extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_8009D07C, D_8009D07D, D_8009D002, D_8009CFFD, D_8009CDC1;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void addItemToInventory(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8001e4f0(int);

void func_8011DD48(TP *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        D_8009C330->b8 = o->active;
        o->active = 2;
        o->d8c = 0;
        o->velX = 0xb4;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        o->bad = 0;
        o->b69 = 0;
        o->ba2 = 2;
        o->animFrame &= 1;
        PlayerSetAnimIfChanged((TObj *)o, 0x2b);
        o->wf6 = o->d->p.whole;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
    case 1:
        FUN_8001fec0((TObj *)o);
        o->d->p.whole++;
        o->velX -= 2;
        if (o->velX != 0) break;
        PlayerSetAnimIfChanged((TObj *)o, 0x2c);
        o->visible = 0;
        o->b0f = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        o->velX = 0xb4;
        o->wb2 = 0;
        o->velY = 0;
        D_8009C330->w20 = 0;
        o->ba3 = 2;
        o->timer = 0x3c;
        o->state++;
        o->ba2 = 0;
        switch (o->w7a) {
        case 37:
            if (D_8009CDC1 == 0xff) break;
            addItemToInventory(0x25, 1, 1);
            o->timer = 0x3c;
            o->state = 4;
            break;
        case 7:
            p = &D_8009D07C;
            if (*p) break;
            addItemToInventory(7, 1, 1);
            *p = 1;
            break;
        case 8:
            p = &D_8009D07D;
            if (*p) break;
            addItemToInventory(0x1f, 1, 1);
            *p = 1;
            break;
        case 9:
            p = &D_8009D002;
            if (*p) break;
            addItemToInventory(0x67, 1, 1);
            *p = 1;
            break;
        case 55:
            p = &D_8009CFFD;
            if (*p) break;
            addItemToInventory(0x37, 1, 1);
            *p = 1;
            break;
        default:
            addItemToInventory(7, 1, 1);
            break;
        }
        break;
    case 2:
        if (--o->timer > 0) break;
        o->visible = 1;
        *(signed char *)&o->b0f = -0x14;
        o->state++;
        break;
    case 3:
        FUN_8001fec0((TObj *)o);
        o->d->p.whole--;
        o->velX -= 2;
        if (o->velX > 0) break;
        o->state = 9;
        break;
    case 4:
        if (--o->timer > 0) break;
        FUN_8005a9a4(0x1d, 0);
        o->timer = 200;
        o->state++;
        break;
    case 5:
        if (--o->timer > 0) break;
        FUN_8005a8a8(0x1e, 0, 0);
        o->timer = 200;
        o->state = 2;
        break;
    case 6:
        o->wb2 = 0;
        o->ba2 = 1;
        PlayerSetAnimIfChanged((TObj *)o, 0x42);
        FUN_8001e4f0(0x20);
        o->bab = 1;
        o->timer = 0x78;
        o->state++;
        break;
    case 7:
        FUN_8001fec0((TObj *)o);
        if (--o->timer > 0) break;
        o->timer = 0x3c;
        o->state = 0;
        break;
    case 8:
        break;
    case 9:
        FUN_8001fec0((TObj *)o);
        o->d->p.whole = o->wf6;
        *(signed char *)&o->b0f = -8;
        o->active = D_8009C330->b8;
        o->b9c = 0;
        o->ba2 = 0;
        o->ba3 = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        D_8009C330->w20 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        break;
    }
}
