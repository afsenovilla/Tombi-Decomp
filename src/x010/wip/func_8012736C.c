// FUNC 8012736c 2060 X010
/* score 306: 22-state cutscene, control flow and shared tails decoded. Left: store/load scheduling in most states
   (game keeps statement order: velH stored before the second global load, state++ placement), the velY test in
   case 4 (blez vs bgtz layout) and the signed/unsigned shifts in case 5. */
#include "TOBJ.H"

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

extern TObj D_800A6038;
extern unsigned short *D_800A605C;
extern unsigned short D_800A6066;
extern short D_800A604A[];
extern short D_800A604E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009D078;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern short D_1F80016A[];
extern short D_1F80016E[];
extern int D_1F800168;
extern int D_1F80016C;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void ObjSetFacingToPlayer(TObj *);
extern void FUN_800eea7c(TObj *, int, int);
extern void FUN_80025f40(int, int, int, int);
extern void func_801271D8(TObj *, int);
extern short TileCollideAt(TObj *, short, short);
extern TObj *FUN_8002dcc8(int, int, void *);
extern void FUN_8005a8a8(int, int, int);
extern void addItemToInventory(int, int, int);

#define SETANIM(k) o->anim = ANIMS(o)[k]; AnimLoadDuration(o);
#define WAITCHILD() \
    AnimAdvance(o); \
    if (((TObj *)o->d90)->b04 == 2) { ((TObj *)o->d90)->b04 = 3; o->state++; }

void func_8012736C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b0f = 2;
        SETANIM(0);
        o->state++;
    case 1:
        if (D_1F80016A[0] >= 0xc81 && D_1F80016E[0] >= -0x27) {
            *(signed char *)&o->b0f = -7;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A6038.active = 5;
            D_800A603C = 5;
            D_800A603D = 100;
            D_800A603E = 0;
            FUN_800eea7c(&D_800A6038, 0x1e, 0);
            D_800A604E = -0x1e;
            o->d34 = -0x1e0000;
            o->timer = 0x3c;
            o->state++;
            playSFX(0xe);
        }
        break;
    case 2:
        if (*D_800A605C == 0x3d) FUN_80025f40(0, 0, 0xff, 4);
        if (--o->timer == -1) {
            o->velH = (D_1F800168 - o->h->raw) >> 15;
            o->state++;
            o->velV = (D_1F80016C - o->y.raw) >> 15;
            o->velY = -0x300;
            o->timer = 0x80;
            *(signed char *)&o->b0f = -9;
            ObjSetFacingToPlayer(o);
            SETANIM(7);
        } else if (o->timer < 0x1e) {
            func_801271D8(o, 1);
        }
        break;
    case 3:
        func_801271D8(o, 1);
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x10;
        o->y.raw += (short)o->velY << 8;
        if (o->velY > 0) goto next8;
        o->timer--;
        break;
    case 4:
        func_801271D8(o, 1);
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (o->velY < 0x300) {
            o->velY += 0x10;
            o->y.raw += (short)o->velY << 8;
        }
        if (o->timer <= 0) {
            o->state++;
            o->a.p.whole = D_1F80016A[0] + 8;
            o->timer = 8;
            o->y.p.whole = D_1F80016E[0];
        } else {
            o->timer--;
        }
        break;
    case 5:
        func_801271D8(o, 1);
        if (--o->timer == -1) {
            o->state++;
            o->velV = (0xff880000 - o->y.raw) >> 15;
            o->velY = -0x500;
            o->timer = 0x80;
            o->animFrame = 1;
            o->velH = (0x0c4e0000 - o->h->raw) >> 15;
            o->state++;
            SETANIM(7);
        }
        break;
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x18;
        o->y.raw += (short)o->velY << 8;
        D_800A604A[0] = o->a.p.whole - 8;
        D_800A604E = o->y.p.whole;
        if (o->velY > 0) {
        next8:
            o->state++;
            SETANIM(8);
        }
        o->timer--;
        break;
    case 7:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x18;
        o->y.raw += (short)o->velY << 8;
        D_800A604A[0] = o->a.p.whole - 8;
        D_800A604E = o->y.p.whole;
        if (o->velY > 0x400) goto next;
        break;
    case 8:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velY << 8;
        {
            short *k = D_800A604A[0];
            *k = o->a.p.whole - 8;
            D_800A604E = o->y.p.whole;
            if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                o->state++;
                SETANIM(9);
                o->timer = 10;
                D_800A6066 = 0;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                *k -= 4;
            }
        }
        break;
    case 9:
        if (--o->timer == -1) {
            short *k = D_800A604A[0];
            *k -= 4;
            o->state++;
            ObjSetFacingToPlayer(o);
            SETANIM(4);
        }
        break;
    case 10:
        if (AnimAdvance(o)) {
            short *k = D_800A604A[0];
            *k -= 2;
            *(signed char *)&o->b0f = -7;
            goto next;
        }
        break;
    case 11:
        SETANIM(5);
        {
            short *k = D_800A604A[0];
            *k -= 2;
        }
        ObjSetFacingToPlayer(o);
        o->state++;
        o->d90 = (int)FUN_8002dcc8(2, 0x13, &o->a);
        break;
    case 12:
    case 14:
    case 18:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            goto next;
        }
        break;
    case 13:
        o->d90 = (int)FUN_8002dcc8(2, 0x14, &o->a);
        o->state++;
        break;
    case 15:
        FUN_8005a8a8(0x40, 0, 0);
        o->timer = 0x168;
        goto next;
    case 16:
    case 20:
        if (--o->timer == -1) goto next;
        break;
    case 17:
        o->d90 = (int)FUN_8002dcc8(2, 0x15, &o->a);
        o->state++;
        break;
    case 19:
        addItemToInventory(0x9f, 1, 1);
        o->timer = 0x50;
    next:
        o->state++;
        break;
    case 21:
        D_8009D078 = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A6038.active = 1;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state = 0;
        o->step++;
        break;
    }
}
