// FUNC 80104698 808 X003
// MATCHING 80104698 808
typedef struct P { short x; short y; } P;
typedef struct S {
    unsigned char b00, b01, type, b03, b04, step, state, substep; char p0[0xf - 8];
    unsigned char b0f; char p1[2]; short x; char p2[2]; unsigned short y; char p3[2]; short z; char p4[0x20 - 0x1c];
    short timer; char p5[0x2e - 0x22]; unsigned short frame; char p6[0x40 - 0x30];
    P *h; char p7[0x69 - 0x44]; unsigned char b69, b6a; char p8[0x70 - 0x6b]; unsigned short w70; char p9[0x7e - 0x72];
    short velY; char pa[0x8c - 0x80]; int d8c; char pb[0x9d - 0x90]; unsigned char b9d; char pc[0xb0 - 0x9e];
    short wb0, wb2, wb4, wb6, wb8, wba; char pd[0xc6 - 0xbc]; unsigned char bc6, bc7; char pe[0xe3 - 0xc8]; unsigned char be3;
} S;
extern S *D_8009C330;
extern S *D_8009D2E8;
extern void ObjCheckHeadCollision(S *);
extern void FUN_8001f96c(int, int, int, int);
extern void PlayerSetAnimIfChanged(S *, int);
extern void FUN_80025f40(int, int, int, int);
extern void func_80102AC0(S *), func_80102F40(S *), func_801040D0(S *), func_801037D8(S *), func_80103290(S *);
extern void FUN_80103d40(S *), FUN_80122a18(S *), FUN_80104454(S *);
void func_80104698(S *o)
{
    if (o->state == 0) {
        D_8009C330->frame = 0xff;
        D_8009C330->timer = 0;
        o->bc7 = 1;
        o->b9d = 0;
        o->bc6 = 0;
        o->be3 = 0;
        o->timer = 0;
        o->wb2 = 0;
        o->wb6 = 0;
        o->wb0 = 0;
        o->d8c = 0;
        o->frame &= 1;
        D_8009C330->b00 = 0;
        switch (D_8009D2E8->type) {
        case 0x1f:
            o->d8c = 0;
            D_8009D2E8->d8c = 0;
            o->y = D_8009D2E8->y - D_8009D2E8->w70;
            ObjCheckHeadCollision(o);
            D_8009D2E8->y = o->y + D_8009D2E8->w70;
            break;
        case 0x3: case 0x12: case 0x2b:
            o->wba = 0;
            o->d8c = 0;
            D_8009D2E8->d8c = 0;
            o->b69 = 0;
            D_8009D2E8->b0f = o->b0f + 1;
            FUN_8001f96c(2, o->x, (short)o->y, o->z);
            break;
        case 0x4: case 0x29:
            o->d8c = 0;
            D_8009D2E8->b0f = o->b0f + 1;
            D_8009D2E8->frame = 1;
            o->h->y = D_8009D2E8->h->y;
            o->y = D_8009D2E8->y - D_8009D2E8->w70;
            break;
        case 0x8: case 0x1e:
            o->d8c = 0;
            D_8009D2E8->state = 2;
        case 0x0: case 0xa:
            o->y = D_8009D2E8->y - D_8009D2E8->w70;
            ObjCheckHeadCollision(o);
            D_8009D2E8->y = o->y + D_8009D2E8->w70;
        case 0x28:
            D_8009D2E8->b0f = o->b0f + 1;
            o->velY = 0x100;
            break;
        case 0xe:
            PlayerSetAnimIfChanged(o, 0xd);
            break;
        case 0x42:
            break;
        }
        PlayerSetAnimIfChanged(o, 0xd);
        o->substep = 0;
        FUN_80025f40(0, 0, 0xff, 4);
        o->state++;
    } else {
        switch (D_8009D2E8->type) {
        case 0x2: case 0x21:
            func_80102AC0(o);
            break;
        case 0x3: case 0x12: case 0x2b:
            func_80102F40(o);
            break;
        case 0x29:
            if (D_8009D2E8->b6a == 1) {
                FUN_80103d40(o);
                break;
            }
        case 0x4:
            func_801040D0(o);
            break;
        case 0x0: case 0x8: case 0xa: case 0x13: case 0x1a: case 0x1e: case 0x1f: case 0x27: case 0x28:
        case 0x38: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: case 0x40: case 0x41: case 0x42:
            func_801037D8(o);
            break;
        case 0xb: case 0x1c:
            func_80103290(o);
            break;
        case 0xe:
            FUN_80103d40(o);
            break;
        case 0x15:
            FUN_80122a18(o);
            break;
        case 0x18:
            FUN_80104454(o);
            break;
        }
    }
}
