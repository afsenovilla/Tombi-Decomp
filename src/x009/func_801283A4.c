// FUNC 801283a4 1300 X009
// MATCHING 801283a4 1300
/* D_800A6047 is the b0f field of the player TObj D_800A6038 (struct access keeps it after the anim store);
   the case-1 tails goto the shared `clr: o->b9d = 0` after case 2's step switch. */
#include "TOBJ.H"

extern unsigned char D_8009C942, D_8009C964;
extern TObj D_800A6038;
extern void *D_8012EE9C[];
extern int ObjCullRegister(TObj *);
extern int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void playSFX(int);
extern void FUN_80018790(TObj *);
extern void func_80128180(TObj *);
extern void func_80124E04(TObj *);
extern void func_8012516C(TObj *);
extern void func_80125438(TObj *);
extern void func_80125A38(TObj *);
extern void func_801260CC(TObj *);
extern void func_801261C0(TObj *);
extern void func_80126460(TObj *);
extern void func_80126890(TObj *);
extern void func_80126CE0(TObj *);
extern void func_80127290(TObj *);
extern void func_8012781C(TObj *);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))
#define ANIMP(o) (*(unsigned short **)((char *)(o) + 0xa8))

void func_801283A4(TObj *o)
{
    TObj *p;
    unsigned char s;

    switch (o->b04) {
    case 0:
        func_80128180(o);
        break;
    case 1:
        if (D_8009C942 && o->b6a != 2) {
            ObjCullRegister(o);
            break;
        }
        if (D_8009C964 == 0x20) break;
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                if (o->b0c) goto set3;
                ObjCullRegister(o);
                func_80124E04(o);
                break;
            case 1:
                if (o->b0c) goto set3;
                ObjCullRegister(o);
                func_8012516C(o);
                break;
            case 2:
                if (o->b0c) goto set3;
                ObjCullRegister(o);
                func_80125438(o);
                break;
            case 3:
                if (o->b0c == 0) {
                    ObjCullRegister(o);
                    func_80125A38(o);
                } else {
                    ObjCullRegister(o);
                    func_801260CC(o);
                }
                break;
            case 4:
                if (o->b0c) goto set3;
                ObjCullRegister(o);
                func_801261C0(o);
                break;
            case 5:
                if (o->b0c) goto set3;
                ObjCullRegister(o);
                func_80126460(o);
                break;
            set3:
                o->b04 = 3;
                break;
            }
        } else {
            if (o->b0c) goto link;
            ObjCullRegister(o);
            if (o->b9f == 0) {
                WD2(o) = o->h->p.whole;
                o->b9f = 0xf;
            } else {
                { int r = FUN_8001f9e0() & 3; int w = WD2(o); w -= 2; o->h->p.whole = w + r; }
                if (--o->b9f == 0) {
                    o->b9e = 0;
                    o->h->p.whole = WD2(o);
                }
            }
        }
        if (o->b0c) goto link;
        if (ANIMP(o)[2] == 0x102 && o->step != 2) {
            o->b04 = 1;
            o->step = 2;
            o->state = 0;
        }
        if (o->ba7 == 1) {
            o->b04 = 2;
            o->step = 4;
            o->state = 0;
            o->active = 3;
        }
        goto clr;
    link:
        if (o->b04 != 3) o->b04 = ((TObj *)o->d90)->b04;
        o->step = ((TObj *)o->d90)->step;
        o->state = ((TObj *)o->d90)->state;
        goto clr;
    case 2:
        o->b6a = 0;
        if (D_8009C964 == 0x20) break;
        if (o->ba7 == 1 && (unsigned char)(o->step - 4) >= 2) {
            o->step = 4;
            o->state = 0;
        }
        switch (o->step) {
        case 0:
            if (o->b0c) goto set3b;
            ObjCullRegister(o);
            func_80126890(o);
            break;
        case 1:
            if (o->b0c) goto set3b;
            ObjCullRegister(o);
            s = o->state;
            switch (s) {
            case 0:
                o->state = s + 1;
                o->wac = 0;
                o->anim = D_8012EE9C[0];
                o->b0f = D_800A6038.b0f + 1;
                o->b0a = 0;
                o->d8c = 0;
                FUN_8001fe6c(o);
                o->active = 2;
                playSFX(0x9c);
            case 1:
                AnimAdvance(o);
                break;
            }
            break;
        case 2:
            if (o->b0c) goto set3b;
            ObjCullRegister(o);
            func_80126CE0(o);
            break;
        case 4:
            if (o->b0c) goto set3b;
            ObjCullRegister(o);
            func_80127290(o);
            break;
        case 5:
            if (o->b0c) goto set3b;
            ObjCullRegister(o);
            func_8012781C(o);
            break;
        set3b:
            o->b04 = 3;
            break;
        }
    clr:
        o->b9d = 0;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
