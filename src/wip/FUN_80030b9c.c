// FUNC 80030b9c 268 MAIN0
// wip: solo falla la posicion de `move s1,s4` (copia del parametro short): el juego la pone tras el jal
// (hueco del beqz), aqui sale en el prologo. Una local `short b = a` cambia la asignacion de registros.
#include "TOBJ.H"
extern TObj *FUN_800184d8(void);
extern unsigned short DAT_800a6066;
extern unsigned char DAT_8009cef7;
extern int DAT_8009c984;
extern void FUN_8004d620(int, int);

void FUN_80030b9c(short a, int x, int y, int z)
{
    TObj *p = FUN_800184d8();
    unsigned short u;
    if (p) {
        p->active = 1;
        p->type = 0x20;
        u = DAT_800a6066;
        p->a.raw = x << 16;
        p->y.raw = y << 16;
        p->b.raw = z << 16;
        p->subtype = a;
        p->b0d = 0;
        p->animFrame = u & 1;
        if (a == 0) {
            p->step = 1;
        } else {
            p->step = a;
            DAT_8009cef7 = 1;
        }
        p->state = 0;
        if (DAT_8009c984 & 0x100) {
            if (a == 1)
                FUN_8004d620(0xf, 2);
            p->b04 = 3;
            p->step = 0;
            p->state = 0;
        }
    }
}
