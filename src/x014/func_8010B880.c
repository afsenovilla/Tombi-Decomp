// FUNC 8010b880 1160 X014
// MATCHING 8010b880 1160
typedef struct { char p0[2]; short s2; } H;
typedef struct PO {
    unsigned char b0; char p1; unsigned char b2; char p3[3]; unsigned char b6;
    char p7[0x16 - 7]; short s16;
    char p18[0x28 - 0x18]; unsigned short s28, s2a; char p2c[2]; unsigned short s2e;
    int d30; char p34[0x40 - 0x34]; H *h40;
    char p44[0x94 - 0x44]; struct PO *d94;
} PO;
typedef struct {
    char p0[5]; unsigned char step, state;
    char p7[0x16 - 7]; short s16;
    char p18[0x24 - 0x18]; void *anim;
    char p28[0x2e - 0x28]; unsigned short s2e;
    char p30[0x40 - 0x30]; H *h40;
    char p44[0x8c - 0x44]; int d8c;
    char p90[0x9d - 0x90]; unsigned char b9d;
    char p9e[0xb8 - 0x9e]; short sb8, sba;
    char pbc[0xc6 - 0xbc]; unsigned char bc6; char pc7; unsigned char bc8;
} TO;
extern PO *D_8009F0EC;
extern PO *D_8009C330;
extern char D_80010F10[], D_80010F30[], D_80010F50[], D_80010D68[];
extern void FUN_800ef6d8(void);
extern void AnimLoadDuration(TO *);
extern void FUN_800ee948(TO *, int);
extern void AnimJump(TO *, int);

static __inline__ void attach(TO *o)
{
    if (D_8009F0EC->b2 == 0x15 || D_8009F0EC->b2 == 0x31)
        o->h40->s2 = o->sb8 + (D_8009F0EC->h40->s2 + D_8009F0EC->d30);
    else
        o->h40->s2 = D_8009F0EC->h40->s2 + o->sb8;
    if (D_8009F0EC->b2 == 0x24)
        o->s16 = D_8009F0EC->s16 + o->sba + 0x10;
    else
        o->s16 = D_8009F0EC->s16 + o->sba;
}

void func_8010B880(TO *o)
{
    PO *p;
    int x, s;

    if (o->b9d == 0) FUN_800ef6d8();
    switch (o->state) {
    case 0:
        o->b9d = 1;
        o->bc8 = 0;
        o->d8c = 0;
        *(unsigned char *)D_8009C330 = 2;
        D_8009C330->s2e = 0xffff;
        D_8009C330->s28 = 0xffff;
        D_8009C330->s2a = 0xffff;
        D_8009C330->b6 = D_8009F0EC->b0;
        switch (o->s2e) {
        case 0: case 1: case 2: case 3:
            o->anim = D_80010F10;
            break;
        case 4: case 5:
            o->anim = D_80010F30;
            break;
        case 6: case 7:
            o->anim = D_80010F50;
            break;
        }
        AnimLoadDuration(o);
        FUN_800ee948(o, 1);
        attach(o);
        switch (D_8009F0EC->b2) {
        case 0xb: case 0x15:
            if (D_8009F0EC->d94 != 0) {
                p = D_8009F0EC->d94;
                if (p->d94 == 0) {
                    *(unsigned char *)p = 2;
                } else {
                loop0:
                    *(unsigned char *)p = 2;
                    p = p->d94;
                    if (p->d94 != 0) goto loop0;
                    *(unsigned char *)p = 2;
                }
            }
            break;
        case 0x1d:
            *(unsigned char *)D_8009F0EC = 3;
            break;
        case 0x31:
            *(unsigned char *)D_8009F0EC = 5;
            break;
        }
        o->state = 1;
        break;
    case 1:
        attach(o);
        if (*(unsigned char *)D_8009C330 != 0) return;
        if (o->bc6 != 0) return;
        switch (D_8009F0EC->b2) {
        case 0xb:
            p = D_8009F0EC;
            if (p->d94 == 0)
                *(unsigned char *)p = 1;
            else
                *(unsigned char *)p = 3;
        case 0x15:
            p = D_8009F0EC;
            if (p->d94 != 0) {
                *(unsigned char *)p = 3;
                p = D_8009F0EC->d94;
                if (p->d94 == 0) {
                    *(unsigned char *)p = 1;
                } else {
                loop1:
                    *(unsigned char *)p = 3;
                    p = p->d94;
                    if (p->d94 != 0)
                        goto loop1;
                    *(unsigned char *)p = 1;
                }
            } else {
                *(unsigned char *)p = 1;
            }
            break;
        case 0x1d: case 0x31:
            *(unsigned char *)D_8009F0EC = 1;
            break;
        case 0xc: case 0xd: case 0xe:
        default:
            *(unsigned char *)D_8009F0EC = 3;
            break;
        }
        o->anim = D_80010D68;
        AnimJump(o, 0);
        o->b9d = 0;
        switch (D_8009F0EC->b2) {
        case 0x15:
            s = 0x22;
            break;
        case 0x31:
            s = 0x46;
            break;
        default:
            s = 8;
            break;
        }
        o->step = s;
        o->state = 0;
        break;
    }
}
