// FUNC 800f86e8 1172 X009
// MATCHING 800f86e8 1172
typedef struct P { char p0[2]; short h02; char p04[4]; unsigned char b8; char p1[0x20-9]; short h20; char p2[0x2c-0x22]; unsigned short h2c; unsigned short h2e; } P;
typedef struct H { short p0; short whole; } H;
typedef struct O {
    unsigned char active; char p01[4]; unsigned char step; unsigned char state; char p07[0xf-7]; signed char b0f;
    char p10[4]; int y; char p18[0x24-0x18]; void *anim; char p28[0x44-0x28]; H *d;
    char p48[0x69-0x48]; unsigned char b69; char p6a[0x7c-0x6a]; short vx; short vy; char p80[2]; short h82;
    char p84[0x8c-0x84]; int d8c; char p90[0x9c-0x90]; unsigned char b9c; char p9d; unsigned char b9e; char p9f;
    unsigned char ba0; char pa1; unsigned char ba2; char pa3[2]; unsigned char ba5; char pa6[4]; unsigned char baa; char pab;
    unsigned char bac; char pad[3]; short hb0; short hb2; char pb4[2]; short hb6;
    char pb8[0xe0-0xb8]; short he0; char pe2[2]; void *de4; char pe8[0xf6-0xe8]; short hf6;
} O;
extern P *D_8009C330;
extern unsigned char D_801152E8[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F;
extern char D_80010CA0[];
extern void *D_8009D2E8;
extern void AnimJump(O *, int);
extern int AnimAdvance(O *);
extern short ObjTileCollide(O *, int, int);
extern int ObjCheckHeadCollision(O *);
extern void SfxPlay3(int, int);
extern void ObjSetAnimFromTable(O *);

void func_800F86E8(O *o)
{
    switch (o->state) {
    case 0:
        o->ba0 = 0;
        o->active = 4;
        o->b9e = 0;
        o->d8c = 0;
        o->hb6 = 0;
        D_8009C330->h2c = 0x1f;
        if (D_8009C330->h2e != 0x1f) {
            D_8009C330->h2c = 0x1f;
            ObjSetAnimFromTable(o);
            AnimJump(o, 0);
            D_8009C330->h2e = D_8009C330->h2c;
        }
        D_8009C330->h02 = 0;
        o->vy = -0x340;
        o->hb2 = 0;
        o->vx = 0;
        if (o->step == 0x49)
            o->vy = -0x580;
        o->hf6 = o->d->whole;
        o->state++;
    case 1:
        AnimAdvance(o);
        o->y += o->vy << 8;
        if (o->step == 0x49)
            o->vy += 0x40;
        else
            o->vy += 0x20;
        if (o->step != 0x49 && ObjCheckHeadCollision(o) != 0) {
            o->active = 3;
            o->bac = 1;
            o->b9c = 2;
            o->vy = 0;
            o->h82 = 0;
            o->state = 2;
            if (o->step == 0x49) {
                o->ba2 = 0;
                o->b0f = -8;
                if (o->he0 != 0)
                    o->active = 3;
                else
                    o->active = 1;
                o->step = 2;
                o->state = 3;
            }
        }
        if (o->vy > 0) {
            o->active = 3;
            o->bac = 1;
            o->b9c = 2;
            o->state = 2;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->y += o->vy << 8;
        o->vy += 0x20;
        if (o->bac == 2) {
            if (o->he0 == 0)
                o->active = 1;
            else
                o->active = 3;
            if (D_8009C942[0] != 0)
                D_8009C942[0] = 0;
            if (D_8009C93F != 0)
                D_8009C93F = 0;
            D_8009D2E8 = o->de4;
            o->b0f = -8;
            D_8009C330->b8 = 0;
            D_8009C330->h20 = 0;
            o->ba5 = 0;
            D_8009C330->h02 = 0;
            o->b9e = 0;
            o->ba2 = 0;
            o->vx = 0;
            o->vy = 0;
            o->baa = 0;
            o->step = 0xe;
            o->state = 0;
        } else if (o->b69 == 1 || ObjTileCollide(o, 0, 0) != 0) {
            if (o->step != 0x49)
                o->ba2 = 1;
            o->bac = 0;
            SfxPlay3(0x1c, 0x7f);
            o->b0f = -8;
            o->anim = D_80010CA0;
            AnimJump(o, 4);
            o->d8c = D_801152E8[o->hb0];
            o->state++;
        } else if (o->step == 0x49 && o->vy > 0x200) {
            o->baa = 0;
            o->ba2 = 0;
            o->b0f = -8;
            if (o->he0 != 0)
                o->active = 3;
            else
                o->active = 1;
            o->step = 2;
            o->state = 3;
        }
        break;
    case 3:
        if (AnimAdvance(o) != 0) {
            if (D_8009C942[0] != 0)
                D_8009C942[0] = 0;
            if (D_8009C93F != 0)
                D_8009C93F = 0;
            o->b0f = -8;
            if (o->he0 != 0)
                o->active = 3;
            else
                o->active = 1;
            o->bac = 0;
            D_8009C330->h02 = 0;
            o->b9e = 0;
            o->baa = 0;
            o->ba2 = 0;
            o->vx = 0;
            o->vy = 0;
            o->d8c = D_801152E8[o->hb0];
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
