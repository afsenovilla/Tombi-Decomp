// VERSION SLPS_01144 x09 8012908C 1956
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { char c[12]; } B12;

extern TObj *D_800A0E68;
extern Fix16 *PLAYER_H;
extern unsigned char D_8009E9FB, D_8009E9FC, D_8009EE59, D_8009E4E7, D_8009E4EA, D_8009E4E6;
extern unsigned char D_800A7B78[], D_800A7B7C[], D_800A7B7D, D_800A7B7E;
extern short D_800A7C2A;
extern short D_800A7BA6;
extern int showMessageBoxDirect(int, int, B12 *);
extern void func_80127C80_slps01144(TObj *, int, int);
extern void playSFX(int);
extern void playSFXWithNote(int, int);
extern void setEventStarted(int, int, int);
extern void func_8001C988(int, int);
extern void playerSetAnim(void *, int, int);

void func_8012908C_slps01144(TObj *o)
{
    TObj *p = D_800A0E68;
    B12 v;

    switch (o->state) {
    case 0:
        if (D_8009E9FC == 255) o->b04 = 3;
        o->state++;
        break;
    case 1:
        if (p->b68 == 0) break;
        D_8009E4E7 = 1;
        D_8009E4EA = 1;
        D_800A7B7C[0] = 5;
        D_800A7B7D = 100;
        D_800A7B7E = 0;
        o->state++;
        if (D_8009E9FB == 1 && D_8009EE59 == 1) {
            playSFX(32);
            playerSetAnim(D_800A7B7C - 4, 40, 0);
            o->w08 = 120;
            o->state = 4;
        }
        if (D_8009E9FB == 255) {
            o->state = 6;
            if (D_8009E9FC == 1 && D_8009EE59 == 2) {
                playSFXWithNote(37, 40);
                playerSetAnim(D_800A7B78, 41, 0);
                o->w08 = 120;
                o->state = 8;
            }
        }
        break;
    case 2:
        v = *(B12 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 0, &v);
        func_80127C80_slps01144(p, 32, 0);
        playerSetAnim(D_800A7B78, 0, 0);
        o->state++;
        break;
    case 3:
        if (((TObj *)p->d90)->b04 != 2) break;
        ((TObj *)p->d90)->b04 = 3;
        o->w08 = 1;
        asm("");
        if (D_8009E9FB == 0) {
            o->w08 = 200;
            setEventStarted(175, 0, 0);
        }
        o->state = 20;
        if (D_8009EE59 == 1) {
            playSFX(32);
            playerSetAnim(D_800A7B78, 40, 0);
            o->w08 = 200;
            o->state = 18;
        }
        func_80127C80_slps01144(p, 20, 0);
        break;
    case 4:
        if (--o->w08 > 0) break;
        playerSetAnim(D_800A7B78, 0, 0);
        v = *(B12 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 1, &v);
        func_80127C80_slps01144(p, 32, 0);
        o->state++;
        break;
    case 5:
        if (((TObj *)p->d90)->b04 != 2) break;
        ((TObj *)p->d90)->b04 = 3;
        func_8001C988(175, 0);
        o->w08 = 200;
        o->state = 20;
        func_80127C80_slps01144(p, 20, 0);
        break;
    case 6:
        v = *(B12 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 2, &v);
        func_80127C80_slps01144(p, 32, 0);
        o->state++;
        break;
    case 7:
        if (((TObj *)p->d90)->b04 != 2) break;
        ((TObj *)p->d90)->b04 = 3;
        o->w08 = 1;
        asm("");
        if (D_8009E9FC == 0) {
            o->w08 = 200;
            setEventStarted(176, 0, 0);
        }
        o->state = 20;
        if (D_8009EE59 == 2) {
            playSFXWithNote(37, 40);
            playerSetAnim(D_800A7B78, 41, 0);
            o->w08 = 200;
            o->state = 19;
        }
        func_80127C80_slps01144(p, 20, 0);
        break;
    case 8:
        if (--o->w08 > 0) break;
        playerSetAnim(D_800A7B78, 0, 0);
        v = *(B12 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 3, &v);
        o->state++;
        func_80127C80_slps01144(p, 32, 0);
        break;
    case 9:
        if (((TObj *)p->d90)->b04 != 2) break;
        ((TObj *)p->d90)->b04 = 3;
        func_8001C988(176, 0);
        o->w08 = 120;
        o->state++;
        func_80127C80_slps01144(p, 20, 0);
        break;
    case 10:
        if (--o->w08 > 0) break;
        p->active = 2;
        p->step = 1;
        p->state = 0;
        o->w08 = 80;
        o->state++;
        break;
    case 11:
        if (--o->w08 > 0) break;
        v = *(B12 *)&p->a;
        p->d90 = showMessageBoxDirect(2, 4, &v);
        func_80127C80_slps01144(p, 32, (p->animFrame = 0, p->step = 0, p->state = 0));
        o->state++;
        break;
    case 12:
        if (((TObj *)p->d90)->b04 != 2) break;
        ((TObj *)p->d90)->b04 = 3;
        o->w08 = 200;
        p->step = 1;
        p->state = 0;
        o->state++;
        break;
    case 13:
        if (--o->w08 > 0) break;
        o->w08 = 1;
        o->state = 20;
        break;
    case 18:
        if (--o->w08 > 0) break;
        o->state = 4;
        break;
    case 19:
        if (--o->w08 > 0) break;
        o->state = 8;
        break;
    case 20:
        if (--o->w08 > 0) break;
        D_8009E4E7 = 0;
        D_8009E4EA = 0;
        D_8009E4E6 = 0;
        D_800A7C2A = 0;
        D_800A7B7C[0] = 1;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        p->b68 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    case 21:
        D_800A7BA6 = p->h->p.whole < PLAYER_H->p.whole;
        if (p->visible) break;
        D_800A7B7C[0] = 1;
        D_8009E4E7 = 0;
        D_8009E4EA = 0;
        D_8009E4E6 = 0;
        D_800A7C2A = 0;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        p->b04 = 3;
        o->b04 = 3;
        break;
    }
}
#endif
