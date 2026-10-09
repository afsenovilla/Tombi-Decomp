// FUNC 8012f988 1476 X001
// MATCHING 8012f988 1476
#include "TOBJ.H"

extern short D_1F80027E;
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern unsigned char D_8009CEAE[];
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern short D_800A45AC[];
extern int D_1F80018C, D_1F800190;
extern int ObjCullRegister(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern short func_8004065C(TObj *, short, short, short);
extern void FUN_80020aec(int);
extern void FUN_8001e76c(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void FUN_800189b8(TObj *);

void func_8012F988(TObj *o)
{
    unsigned char t = o->b04;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 0x20;
        o->box1 = 0x40;
        o->box2 = 0x18;
        o->box3 = 0x38;
        o->b69 = 0;
        o->wb4 = o->b6b;
        o->b6b = 0;
        o->d84 = 0;
        o->d88 = -0x400;
        o->d8c = 0;
        break;
    case 1:
        if (!ObjCullRegister(o)) break;
        switch (o->step) {
        case 0:
            o->y.p.whole += 4;
            if (FUN_80040278(o, o->h->p.whole + 0x20, o->y.p.whole + 0x18)) {
                o->d8c = (-D_1F80027E << 6) & 0xfc0;
                o->step++;
            }
            break;
        case 1:
            if (o->b6a) {
                if (o->b6a & 1) o->h->p.whole--;
                else o->h->p.whole++;
            }
            o->y.p.whole += 2;
            func_8004065C(o, o->h->p.whole + 0x20, o->y.p.whole, 0);
            if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x18)) o->d8c = (-D_1F80027E << 6) & 0xfc0;
            if (o->h->p.whole >= 0x2d0) break;
            FUN_80020aec(*(unsigned short *)&o->wb4);
            o->type = 0xc;
            o->category = 2;
            D_8009C93F[0] = 1;
            D_8009C93E[0] = 1;
            D_8009C942[0] = 1;
            D_8009CEAE[0] = 1;
            D_800A4553[0] = 5;
            D_800A4568[0] = 0;
            D_800A45AC[0] = 0;
            D_1F800190 = o->y.raw;
            D_1F80018C = o->h->raw;
            o->timer = 0x20;
            o->velH = 0;
            o->step++;
            break;
        case 2:
            o->velH += 0x10;
            if (o->velH >= 0x300) o->velH = 0x300;
            o->h->raw -= o->velH << 8;
            o->y.p.whole += 3;
            if (!FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x18)) {
                o->b69 = 0;
                o->velV = 0;
                o->step++;
                break;
            }
            o->d8c = (-D_1F80027E << 6) & 0xfc0;
            break;
        case 3:
            if (o->d8c == 0x200) o->step++;
            o->d8c += 0x20;
            o->h->p.whole--;
            break;
        case 4:
            if (o->timer) {
                o->h->p.whole--;
                if (--o->timer == 0) FUN_8001e76c(0xd, 1, -8, 0xb4);
            }
            o->d8c += 0x10;
            if (o->d8c >= 0x1000) o->d8c = 0x1000;
            o->velV += 0x10;
            if (o->velV > 0x400) o->velV = 0x400;
            o->y.raw += o->velV << 8;
            func_8004065C(o, o->h->p.whole + 0x20, o->y.p.whole, 0);
            if (o->b69 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x18)) {
                FUN_8001e560(5, 0);
                o->step++;
                D_8009CEAE[0] = 2;
            }
            D_1F800190 = o->y.raw;
            D_1F80018C = o->h->raw;
            break;
        case 5:
            if (D_8009CEAE[0] == 3) o->step++;
            o->y.p.whole += 4;
            FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x1e);
            break;
        case 6:
            o->y.p.whole += 4;
            FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x1e);
            if (D_8009CEAE[0] == 4) o->b04++;
            break;
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_800189b8(o);
        break;
    }
}
