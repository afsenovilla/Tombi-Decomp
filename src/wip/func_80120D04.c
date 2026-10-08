// FUNC 80120d04 1056 X000
// wip score 228: structure right (gotos into the step-2 switch); gcc CSEs &D_1F800000 into s0 and puts p in s2, game rematerializes lui/addiu per use and keeps p in s0; case 1 tail gets cross-jumped
#include "TOBJ.H"

extern unsigned short D_8009C962[];
extern char D_1F800000[];
void FUN_80021f5c(void *m);
void RotMatrixX(int r, void *m);
void RotMatrixY(int r, void *m);
void RotMatrix(short *v, void *m);
void MulMatrix0(void *a, void *b, void *c);
void ApplyRotMatrix(short *v, void *out);
void ObjCullRegister(TObj *o);
void ObjFreeDup(TObj *o);
void FUN_80120734(TObj *o);
void func_80120890(TObj *o);
void FUN_80120bcc(TObj *o);

void func_80120D04(TObj *o)
{
    short sv[3];
    TObj *p;
    TObj *e;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if (D_8009C962[0] == 0)
            return;
        o->active = 2;
        *(int *)&o->w5c = o->a.p.whole;
        o->d60 = o->y.p.whole;
        o->d64 = o->b.p.whole;
        switch (o->b0c) {
        case 0:
            for (e = (TObj *)o->d94; e != 0; e = (TObj *)e->d94) {
                e->d90 = (int)o;
                e->d30 = e->a.raw - o->a.raw;
                e->d34 = e->y.raw - o->y.raw;
                e->d38 = e->b.raw - o->b.raw;
            }
            o->b0a = 0x15;
            o->da0 = 0;
            goto c0;
        case 1:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            o->ba4 = 0;
            FUN_80021f5c(D_1F800000);
            sv[0] = ((short *)&o->d30)[1];
            sv[1] = ((short *)&o->d34)[1];
            sv[2] = ((short *)&o->d38)[1];
            goto mul;
        case 2:
        case 3:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            FUN_80021f5c(D_1F800000);
            RotMatrixX(p->velH, D_1F800000);
            RotMatrixY((unsigned short)p->wb4, D_1F800000);
            goto vec;
        case 4:
        case 5:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            goto rx;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (D_8009C962[0] == 0)
            return;
        switch (o->step) {
        case 0:
            FUN_80120734(o);
            break;
        case 1:
            func_80120890(o);
            break;
        case 2:
            o->b0a = 0x15;
            switch (o->b0c) {
            case 0:
                FUN_80120bcc(o);
            c0:
                FUN_80021f5c(&o->w48);
                sv[0] = o->d84;
                sv[1] = o->d88;
                sv[2] = o->d8c;
                RotMatrix(sv, &o->w48);
                break;
            case 1:
                p = (TObj *)o->d90;
                FUN_80021f5c(D_1F800000);
                sv[0] = ((short *)&o->d30)[1];
                sv[1] = ((short *)&o->d34)[1];
                sv[2] = ((short *)&o->d38)[1];
                goto mul;
            case 2:
            case 3:
                p = (TObj *)o->d90;
                FUN_80021f5c(D_1F800000);
                RotMatrixX(p->velH, D_1F800000);
                RotMatrixY((unsigned short)p->wb4, D_1F800000);
                goto vec;
            case 4:
            case 5:
                p = (TObj *)o->d90;
            rx:
                FUN_80021f5c(D_1F800000);
                RotMatrixX(p->velH, D_1F800000);
            vec:
                sv[0] = ((short *)&o->d30)[1];
                sv[1] = ((short *)&o->d34)[1];
                sv[2] = ((short *)&o->d38)[1];
            mul:
                MulMatrix0(&p->w48, D_1F800000, &o->w48);
                ApplyRotMatrix(sv, &o->w5c);
                *(int *)&o->w5c += p->a.p.whole;
                o->d60 += p->y.p.whole;
                o->d64 += p->b.p.whole;
                o->a.p.whole = *(int *)&o->w5c;
                o->y.p.whole = o->d60;
                o->b.p.whole = o->d64;
                break;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
