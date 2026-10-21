// FUNC 800efd68 1288 I0NFO2
// MATCHING 800efd68 1288
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
typedef struct { int **anims; int p4; int p8; } AnimSet;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern TObj D_800A6038;
extern unsigned char D_800A60E4;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942;
extern unsigned char D_8009CDA4[];
extern unsigned char D_8009CDD0[];
extern unsigned char D_8009CFDE[];
extern unsigned char D_8009CFF7;
extern AnimSet D_800FED84[];
extern short topdownActorOpenDialogue(TObj *o);
extern void AnimJump(TObj *o, int n);
extern void AnimAdvance(TObj *o);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void spawnItemPickupObject(int);
extern void printInfoMessage(int, int);

void topdownActorTalkStep(TObj *o)
{
    B12 v;
    short k;

    switch (U8(o, 0x6)) {
    case 0:
        D_800A603C = 1;
        D_800A603D = 2;
        D_800A603E = 0;
        D_800A60E4 = 3;
        D_8009C93F[0] = 1;
        U16(&D_800A6038, 0x2E) |= 8;
        S16(o, 0xB0) = U16(o, 0x2E);
        k = U8(o, 0x69) + 7;
        U16(o, 0x2E) = U8(o, 0x69) - 1;
        if (k != U16(o, 0xD2)) {
            PTR(o, 0x24) = (void *)D_800FED84[U8(o, 0x3)].anims[k];
            AnimJump(o, 0);
            U16(o, 0xD2) = k;
        }
        U8(o, 0x68) = 2;
        U8(o, 0x6)++;
        if (topdownActorOpenDialogue(o) == 0) {
            D_8009C942 = 0;
            D_8009C93F[0] = 0;
            D_800A60E4 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            U8(o, 0x4) = 1;
            U8(o, 0x5) = 0;
            U8(o, 0x6) = 0;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            D_8009C942 = 0;
            D_8009C93F[0] = 0;
            D_800A60E4 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            U8(o, 0x68) = 0;
            U8(o, 0x6A) = 0;
            U8(o, 0x5) = 0;
            U8(o, 0x6) = 0;
            U16(o, 0x2E) = S16(o, 0xB0);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            if (D_8009CDA4[S16(o, 0xAC)] == 0) {
                setEventStarted(S16(o, 0xAC), 0, 0);
                S16(o, 0x20) = 0xC8;
            } else {
                S16(o, 0x20) = 2;
            }
            U8(o, 0x6)++;
            if ((unsigned short)S16(o, 0xBA) == 0xE) {
                U8(o, 0x6) = 4;
            }
        }
        break;
    case 3:
        if (--S16(o, 0x20) <= 0) {
            D_8009C942 = 0;
            D_800A60E4 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F[0] = 0;
            U8(o, 0x68) = 0;
            U8(o, 0x6A) = 0;
            U8(o, 0x5) = 0;
            U8(o, 0x6) = 0;
            U16(o, 0x2E) = S16(o, 0xB0);
        }
        break;
    case 4:
        v = *(B12 *)&o->a;
        *(short *)&v.c[6] += 0x50;
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x61, &v);
        spawnItemPickupObject(0);
        U8(o, 0x6)++;
        break;
    case 6:
        v = *(B12 *)&o->a;
        if (D_8009CDD0[0] < 2) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x22, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x23, &v);
        }
        U8(o, 0x6)++;
        break;
    case 7:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            S16(o, 0x20) = 2;
            U8(o, 0x6) = 3;
            switch (D_8009CDD0[0]) {
            case 0:
                setEventStarted(0x2C, 1, 1);
                S16(o, 0x20) = 0xC8;
                break;
            case 1:
                D_8009CDD0[0] = 2;
                break;
            case 2:
                setEventComplete(0x2C, 1);
                S16(o, 0x20) = 0xC8;
                break;
            }
        }
        break;
    case 8:
        v = *(B12 *)&o->a;
        *(short *)&v.c[6] += 0x50;
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x63, &v);
        if (D_8009CFDE[0] == 0) {
            D_8009CFDE[0] = 1;
            D_8009CFF7++;
        }
        printInfoMessage(D_8009CFF7 + 0x32, 2);
        U8(o, 0x6)++;
        break;
    case 5:
    case 9:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            S16(o, 0x20) = 2;
            U8(o, 0x6) = 3;
        }
        break;
    }
}
