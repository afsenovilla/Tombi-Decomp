// FUNC 800e89f8 4280 D0SPSUB
// wip: score 18 (was 22). Fixed: the P334 + [9] addu is base-first with `int off = ((int *)P334)[9]; char *p = P334;
// o->pA0 = p + off;` (the second volatile read into a pointer variable makes it a REG for expand_binop).
// Left: (1) case 0x84 tail = 8 insns (game: lhu w08-value in v1, lhu D_8007A024+14 in v0, sh wc4, li 1, sb GCD, sh w08;
// ours allocates a1/v1/v0 and stores GCD before both sh); the game's RTL order is loads first and the w08 store
// LAST, but every temp/order tried (`a = D_8007A024[GAME.b30].a` before wc4/GCD, int or ushort, `Pal *t`) scores
// 54+ because the `sw D_8009C984` moves; `|=, w08, GCD=1, wc4` (also with a temp before GCD) stays at 18.
// (2) base+index addu operand order at the 3 `Pal *t = D_8007A024; ... t += D_8009D2B0[3]` sites: the game has
// `addu v1,v1(base),v0(idx)`. Cause found in cse.c fold_rtx: for a commutative op whose first operand is a register
// with a KNOWN CONSTANT value (t == D_8007A024, same cse block) and second not, cse swaps them ("place any constant
// second"); so the game's base register must be unknown to cse (set in another block / rematerialized). Tried:
// every (char *)t + (idx << 2) / &t[idx] / t[idx].b / int temps form (18 or 48), a function-scope
// `Pal *pal = D_8007A024` (912: it gets a register). Not tried: a form where the base is loaded in the skipped
// block between the two || tests, or any non-constant base expression.
// do{}while(0) around switch(o->wait) fixes reg alloc; in case 0x84 `D_8009C984_a[0] = D_8009C984 | 0x40` (in-struct
// store) keeps the sw before the D_8007A024 loads.
typedef struct { unsigned short a, b; } Pal;
typedef struct {
    unsigned char b0, p1, b2, b3;
    char p4[6];
    unsigned char bA, pB, bC, pD, pE, bF;
    char p10[0xC];
    unsigned char b1C;
    char p1D[0x11];
    short w2E;
    char p30[0x3B];
    unsigned char b6B;
    char p6C[0x34];
    char *pA0;
    unsigned char bA4;
} Obj;
extern Pal D_8007A024[];
typedef struct {
    char p000[0x1F7];
    unsigned char bcef7;   /* 0x1f7 */
    unsigned char bcef8;   /* 0x1f8 */
    char p1f9[0x2F8 - 0x1F9];
    unsigned char bcff8;   /* 0x2f8 */
    char p2f9[2];
    unsigned char bcffb;   /* 0x2fb */
    char p2fc[0x306 - 0x2FC];
    unsigned char bd006;   /* 0x306 */
} GcdPart;
extern GcdPart GCD;
typedef struct { char p00[0x10]; unsigned char b10, b11; char p12[0x1E]; unsigned char b30; } GamePart;
extern GamePart GAME;
typedef struct {
    unsigned char b00, p01, p02, b03, b04, b05, b06, p07;
    unsigned short w08;
    char p0a[0x98 - 0xA];
    unsigned short w98, w9a;
    char p9c[0xC4 - 0x9C];
    unsigned short wc4;
    char pc6[3];
    unsigned char bc9;
    char pca[2];
    unsigned char bcc;
    char pcd[6];
    unsigned char bd3;
} Pl;
extern Pl PLAYER;
extern unsigned char D_800A6038;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned short D_800A60D2;
extern unsigned char D_800A6101;
extern unsigned char D_800A610B;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D2B0[];
extern unsigned char D_8009D090;
extern unsigned char D_8009C940, D_8009C941;
extern unsigned char D_8009D089, D_8009D08A, D_8009D08B, D_8009D08C, D_8009D08D, D_8009D08E, D_8009D08F;
extern unsigned char D_8009CD98, D_8009CD99, D_8009CD9A, D_8009CD9C, D_8009CD9D, D_8009CD9E, D_8009CD9F;
extern int D_8009C984, D_8009C984_a[];
extern unsigned char D_800B146C, D_800B146D, D_800B146E, D_800B1470, D_800B1471, D_800B1472;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned char D_8009CDBA;
extern unsigned char D_8009CF13;
extern unsigned short D_8009C960;
extern int D_8009C96C, D_8009C98C;
extern unsigned char D_8009CEFA, D_8009C93F, D_8009CF07, D_8009CF08, D_8009CF09;
extern char *D_1F800334;
#define P334 (*(char * volatile *)&D_1F800334)
extern void playSFX(int id);
extern void loadCompressedTIM(int n);
extern void removeItemFromInventory(int id, int n);
extern void setEventComplete(int a, int b);
extern Obj *allocObjectLayer4(void);
extern void subScreenApplyPendingEquip(void);
extern void subScreenApplyPendingGauges(void);

int func_800E89F8(int item)
{
    Obj *o;
    switch (item) {
    case 0xA:
        if (GCD.bcef7) GCD.bcef7 = 0;
        else GCD.bcef7 = 1;
        playSFX(0x19);
        if (item != 0x94) {
            D_800A603C = 1;
            D_800A603D = 0x30;
            D_800A603E = 0;
        }
        D_8009C940 = 1;
        D_8009C941 = item;
        return 2;
    case 0xC:
        if (GCD.bcff8) GCD.bcff8 = 0;
        else GCD.bcff8 = 1;
        playSFX(0x19);
        if (item != 0x94) {
            D_800A603C = 1;
            D_800A603D = 0x30;
            D_800A603E = 0;
        }
        D_8009C940 = 1;
        D_8009C941 = item;
        return 2;
    case 0x14:
        playSFX(0x19);
        if (item != 0x94) {
            D_800A603C = 1;
            D_800A603D = 0x30;
            D_800A603E = 0;
        }
        D_8009C940 = 1;
        D_8009C941 = item;
        if (GCD.bcef8) {
            GCD.bcef8 = 0;
            D_800A6101 = 0;
            return 3;
        }
        GCD.bcef8 = 1;
        D_800A6101 = 1;
        return 1;
    case 0x82:
        switch (D_8009D2B0[1]) {
        case 0:
            D_8009D2B0[1] = 1;
            break;
        case 1:
            D_8009D2B0[1] = 2;
            break;
        case 2:
            D_8009D2B0[1] = 1;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x92:
        switch (D_8009D2B0[1]) {
        case 0:
            D_8009D2B0[1] = 0;
            break;
        case 1:
            D_8009D2B0[1] = 0;
            break;
        case 2:
            D_8009D2B0[1] = 0;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x18:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        D_8009D2B0[3] = 2;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x1C:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        D_8009D2B0[3] = 3;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x1D:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        D_8009D2B0[3] = 1;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x20:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        D_8009D2B0[3] = 0;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x60:
        if (GCD.bd006 == 0) {
            if (D_8009D090 == 0xFF) D_8009D090 = D_8009D2B0[3];
            subScreenApplyPendingGauges();
            PLAYER.b03 = 2;
            GCD.bd006 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
            playSFX(0x19);
            return 1;
        }
        subScreenApplyPendingEquip();
        D_8009D090 = 0xFF;
        playSFX(0x19);
        return 4;
    case 0x84:
        if (GCD.bcffb == 0) {
            if (D_8009D090 == 0xFF) D_8009D090 = D_8009D2B0[3];
            D_8009D089 = D_8009CD9C;
            D_8009D08A = D_8009CD98;
            D_8009D08B = D_8009CD9D;
            D_8009D08C = D_8009CD99;
            D_8009D08D = D_8009CD9E;
            D_8009D08E = D_8009CD9A;
            D_8009D08F = D_8009C984;
            subScreenApplyPendingEquip();
            D_8009CD98 = 9;
            D_8009CD99 = 9;
            D_8009CD9A = 9;
            D_8009CD9C = 0x3C;
            D_8009D2B0[3] = 3;
            D_8009CD9D = 0x3C;
            D_8009CD9E = 0x3C;
            D_8009C984_a[0] = D_8009C984 | 0x40;
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            GCD.bcffb = 1;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
            playSFX(0x19);
            return 1;
        }
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        GCD.bcffb = 0;
        playSFX(0x19);
        return 5;
    case 0x62:
        if (GAME.b30 == 1) {
            GAME.b30 = 0;
            if (D_8009CF06 || GCD.bd006) {
                PLAYER.w08 = D_8007A024[4].a;
                PLAYER.wc4 = D_8007A024[4].b;
            } else {
                {
                    Pal *t = D_8007A024;
                    PLAYER.w08 = t->a;
                    t += D_8009D2B0[3];
                    PLAYER.wc4 = t->b;
                }
            }
            PLAYER.bcc = 0;
            D_800B146C = 0;
            return 2;
        }
        GAME.b30 = 1;
        if (D_8009CF06 || GCD.bd006) {
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[1].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
        }
        D_8009CD9F = 0x3C;
        D_800B1470 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A610B = item;
        PLAYER.bcc |= 1;
        playSFX(0x19);
        return 1;
    case 0x63:
        if (GAME.b30 == 2) {
            GAME.b30 = 0;
            if (D_8009CF06 || GCD.bd006) {
                PLAYER.w08 = D_8007A024[4].a;
                PLAYER.wc4 = D_8007A024[4].b;
            } else {
                {
                    Pal *t = D_8007A024;
                    PLAYER.w08 = t->a;
                    t += D_8009D2B0[3];
                    PLAYER.wc4 = t->b;
                }
            }
            PLAYER.bcc = 0;
            D_800B146E = 0;
            return 2;
        }
        GAME.b30 = 2;
        if (D_8009CF06 || GCD.bd006) {
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[2].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
        }
        D_8009CD9F = 0x3C;
        PLAYER.bcc = 1;
        D_800B1472 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A610B = item;
        playSFX(0x19);
        return 1;
    case 0x64:
        if (GAME.b30 == 3) {
            GAME.b30 = 0;
            if (D_8009CF06 || GCD.bd006) {
                PLAYER.w08 = D_8007A024[4].a;
                PLAYER.wc4 = D_8007A024[4].b;
            } else {
                {
                    Pal *t = D_8007A024;
                    PLAYER.w08 = t->a;
                    t += D_8009D2B0[3];
                    PLAYER.wc4 = t->b;
                }
            }
            PLAYER.bcc = 0;
            D_800B146D = 0;
            return 2;
        }
        GAME.b30 = 3;
        if (D_8009CF06 || GCD.bd006) {
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[3].a;
            PLAYER.wc4 = D_8007A024[D_8009D2B0[3]].b;
        }
        D_8009CD9F = 0x3C;
        PLAYER.bcc = 1;
        D_800B1471 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A610B = item;
        playSFX(0x19);
        return 1;
    case 0x1B:
        D_8009D2B0[2] = 0;
        loadCompressedTIM(0);
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x21:
        D_8009D2B0[2] = 8;
        loadCompressedTIM(1);
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x22:
        D_8009D2B0[2] = 9;
        loadCompressedTIM(2);
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x15:
        D_8009D2B0[2] = 5;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x16:
        D_8009D2B0[2] = 6;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x17:
        D_8009D2B0[2] = 7;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        playSFX(0x19);
        return 1;
    case 0x5:
        D_8009CD96 = 3;
        D_8009CD94 = 0;
        D_8009CDA0 = 0;
        D_8009D2B0[0] = 3;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A6038 = 3;
        D_8009C940 = 1;
        D_8009C941 = item;
        return 1;
    case 0x7C:
        D_8009CD94 = 4;
        D_8009CD96 = 0xF;
        D_8009CDA0 = 0;
        D_8009D2B0[0] = 3;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A6038 = 3;
        D_8009C940 = 1;
        D_8009C941 = item;
        return 1;
    case 0x97:
        D_8009CD94 = 0xA;
        D_8009CD96 = 8;
        D_8009CDA0 = 0;
        D_8009D2B0[0] = 3;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A6038 = 3;
        D_8009C940 = 1;
        D_8009C941 = item;
        return 1;
    case 0x98:
        D_8009CD94 = 2;
        D_8009CD96 = 4;
        D_8009CDA0 = 0;
        D_8009D2B0[0] = 3;
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
        D_800A6038 = 3;
        D_8009C940 = 1;
        D_8009C941 = item;
        return 1;
    case 0xB:
        o = allocObjectLayer4();
        if (o) {
            { int off = ((int *)P334)[9]; char *p = P334; o->pA0 = p + off; }
            o->b1C = 4;
            o->b2 = 0x12;
            o->b3 = 1;
            o->bA4 = 0;
            o->b6B = 0;
            o->b0 = 2;
            o->bC = 0;
            o->w2E = 0;
            o->bA = 0x11;
            o->bF = 0;
            removeItemFromInventory(0xB, 1);
            D_8009CDBA = 2;
            return 1;
        }
        return 1;
    case 0x6F:
        PLAYER.w98 += 4;
        if (GAME.b11 < (short)PLAYER.w98) PLAYER.w98 = GAME.b11;
        D_800A60D2 = PLAYER.w98;
        GAME.b10 = PLAYER.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x9A:
        D_8009CF13 = 0;
        PLAYER.w98 = GAME.b11;
        D_800A60D2 = PLAYER.w98;
        GAME.b10 = PLAYER.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x1E:
        if (D_8009C960 == 0x13) break;
    case 0x70:
        PLAYER.w98 += 8;
        if (GAME.b11 < (short)PLAYER.w98) PLAYER.w98 = GAME.b11;
        D_800A60D2 = PLAYER.w98;
        GAME.b10 = PLAYER.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x1F:
        if (D_8009C960 == 0x13) break;
    case 0x71:
        PLAYER.w98 = GAME.b11;
        D_800A60D2 = PLAYER.w98;
        GAME.b10 = PLAYER.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x3F:
        D_8009C98C = D_8009C96C + 100000;
        setEventComplete(0xF, 1);
        removeItemFromInventory(0x3F, 1);
        D_8009CEFA = 1;
        return 1;
    default:
        if (item == 3 && D_8009C960 == 0) D_8009C93F = 1;
        switch (item) {
        case 0x9C:
            D_8009CF07 = 1;
            removeItemFromInventory(0x9C, 1);
            break;
        case 0x9D:
            D_8009CF08 = 1;
            removeItemFromInventory(0x9D, 1);
            break;
        case 0x9E:
            D_8009CF09 = 1;
            removeItemFromInventory(0x9E, 1);
            break;
        case 0x3C:
            if (D_8009C960 == 0x11) removeItemFromInventory(0x3C, 1);
            break;
        case 0x75:
            removeItemFromInventory(0x75, 1);
            break;
        }
        break;
    }
    if (item != 0x94) {
        D_800A603C = 1;
        D_800A603D = 0x30;
        D_800A603E = 0;
    }
    D_8009C940 = 1;
    D_8009C941 = item;
    return 1;
}
