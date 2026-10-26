// FUNC 800f7600 2776 I0NFO2
// MATCHING 800f7600 2776
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2DC;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDCA[];
extern unsigned char D_8009CEE7;
extern unsigned char D_8009CEF6;
extern unsigned char D_8009CF2A;
extern void topdownActorSetAnim(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void addItemToInventory(int, int, int);
extern void increaseMaxHealth(void);
extern void printInfoMessage(int, int);

void func_800F7600(TObj *o)
{
    B12 v;
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2DC;

    switch (o->state) {
    case 0:
        a->step = 2;
        a->state = 0;
        b->step = 2;
        b->state = 0;
        D_800A603D = 2;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->w08 = 0x3C;
        o->state++;
        topdownActorSetAnim(a, 3, 0);
        topdownActorSetAnim(b, 3, 0);
        if (D_8009CDCA[0] == 0xFF) {
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 0x12, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state = 0x13;
        }
        if (D_8009CDCA[0] == 1) {
            if (D_8009CEF6 >= 5) {
                setEventComplete(0x26, 0);
                o->w08 = 0xC8;
                o->state = 0xD;
            } else {
                o->w08 = 4;
                o->state = 0xA;
            }
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            v = *(B12 *)&a->a;
            S16(a, 0x2E) = 3;
            a->d90 = (int)showMessageBoxDirect(2, 4, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 2:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            S16(b, 0x2E) = 3;
            b->d90 = (int)showMessageBoxDirect(2, 5, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 3:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 6, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 4:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            S16(b, 0x2E) = 3;
            b->d90 = (int)showMessageBoxDirect(2, 7, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 5:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 8, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 6:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            S16(b, 0x2E) = 3;
            b->d90 = (int)showMessageBoxDirect(2, 9, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 7:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            setEventStarted(0x26, 0, 0);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 3, 0);
            o->w08 = 0xC8;
            o->state++;
        }
        break;
    case 8:
        if (--o->w08 <= 0) {
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0xA, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 9:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 3, 0);
            addItemToInventory(0x79, 1, 1);
            o->w08 = 0x3C;
            o->state++;
        }
        break;
    case 10:
        if (--o->w08 <= 0) {
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 0xB, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 11:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0xC, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 12:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state = 0x14;
        }
        break;
    case 13:
        if (--o->w08 <= 0) {
            v = *(B12 *)&a->a;
            S16(a, 0x2E) = 3;
            a->d90 = (int)showMessageBoxDirect(2, 0xD, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 14:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            b->d90 = (int)showMessageBoxDirect(2, 0xE, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 15:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0xF, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 16:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 0x10, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            addItemToInventory(7, 1, 1);
            o->state++;
        }
        break;
    case 17:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0x11, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            increaseMaxHealth();
            printInfoMessage(0x15, 3);
            o->state++;
        }
        break;
    case 18:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 0x12, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            D_8009CF2A = 1;
            o->state++;
        }
        break;
    case 19:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 20:
        a->b68 = 0;
        b->b68 = 0;
        a->step = 0;
        a->state = 0;
        b->step = 0;
        b->state = 0;
        D_8009CEE7 = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
