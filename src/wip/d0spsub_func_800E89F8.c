// FUNC 800e89f8 4280 D0SPSUB
// wip: score 6 (was 18). Fixed: the case 0x84 tail. The game's sched1 order there (lhu w08val; lhu wc4val; sh wc4;
// li 1; sb GCD; sh w08, the wc4 load ADJACENT to its store) is a scheduler stall: at that cycle nothing else is
// ready because every earlier store of the block is a predecessor of the `D_8007A024[GAME.b30].a` load (in-struct,
// varying address) -> the 0x3c/9 stores to D_8009CD98..9E must be IN-STRUCT (struct Cd98 D_8009CD98_s fields, not
// scalars: a scalar fixed store never conflicts with an in-struct varying load, so it stays ready and fills the
// gap). With that, the source order `a = w08val; wc4 = ...; GCD = 1; w08 = a` (block-scoped unsigned short a) gives
// the game's allocation v1/v0/v0. Volatile loads (D_8007A024_v) also pin the two loads' order but break the store
// order (scores 16-18); register-reuse temps and a `unsigned char *g = &GCD.bcffb` base change nothing.
// Left: (2) base+index addu operand order at the 3 `Pal *t = D_8007A024; ... t += GAME_PLAYER_MODE[3]` sites: the game has
// `addu v1,v1(base),v0(idx)`. Cause found in cse.c fold_rtx: for a commutative op whose first operand is a register
// with a KNOWN CONSTANT value (t == D_8007A024, same cse path) and second not, cse swaps them ("place any constant
// second"); so the game's base register must be unknown to cse. cse's path for the else block is [lbu bd006; beqz]
// -> [else block] (the label is preceded by a barrier so the taken branch is followed), i.e. a set anywhere in those
// two blocks is known. Tried: every (char *)t + (idx << 2) / &t[idx] / t[idx].b / int temps form (18 or 48), a
// function-scope `Pal *pal` set once at the top or once per case before the tests (743-912: the lui/addiu is
// emitted at the set and the pseudo gets a register; local-alloc's move-to-use needs exactly one use, the game has
// two: `lhu a0,0(v1)` and the addu), a volatile pointer (477).
// Second pass (same score): the shape `la v1; lbu v0; lhu a0,0(v1); sll; addu v1,v1,v0` exists nowhere else in the
// game except the D0SPSUB3 twin, and the plain array form at these sites gives `lhu a0,sym; lhu v0,sym+2(v0)` (51),
// so the base is a user pointer the game's cse did NOT know. Facts established (see notes/matching_guide.md):
// `p + i` inside a deref expands MULT-first (idx first, independent of cse); `t += i` / `(char *)t + (i << 2)`
// expand base-first and keep it iff the base pseudo is unknown to cse at the addu (function-scope pointer set
// once at the top: base-first, but then the `la` sits in the prologue and the pseudo gets a1/s-reg). Excluded:
// local-alloc move-to-use (needs REG_N_REFS == 2, the lhu is a second use), reload rematerialisation (the
// pseudo always gets a hard reg here, and `(mem P)` would become `(mem sym)`), cse-folded jumps / do-while /
// for-break / labels around the block (all 6), `-fforce-addr` (breaks the matched helpers), inline helpers with the
// base as parameter (36-51; integrate.c copies the constant into an in-block pseudo), DImode/union carriers (640),
// idx-first sums, matchcheck (CC1PSX agrees: 6). A matched function with a single-block base-first addu is
// x014 hauntedMansionPigShotPickPoint (`p = A; if (c) p = B; p += i`: the join hides the sets from cse).
// do{}while(0) around switch(o->wait) fixes reg alloc; in case 0x84 `D_8009C984_a[0] = GAME_AREA00_EVENT_CONTROL | 0x40` (in-struct
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
extern volatile Pal D_8007A024_v[];
typedef struct { unsigned char b98, b99, b9a, b9b, b9c, b9d, b9e, b9f; } Cd98;
extern Cd98 D_8009CD98_s;
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
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned short D_800A60D2;
extern unsigned char D_800A6101;
extern unsigned char D_800A610B;
extern unsigned char D_8009CF06;
extern unsigned char GAME_PLAYER_MODE[];
extern unsigned char D_8009D090;
extern unsigned char GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern unsigned char D_8009D089, D_8009D08A, D_8009D08B, D_8009D08C, D_8009D08D, D_8009D08E, D_8009D08F;
extern unsigned char D_8009CD98, D_8009CD99, D_8009CD9A, D_8009CD9C, D_8009CD9D, D_8009CD9E, D_8009CD9F;
extern int GAME_AREA00_EVENT_CONTROL, D_8009C984_a[];
extern unsigned char D_800B146C, D_800B146D, D_800B146E, D_800B1470, D_800B1471, D_800B1472;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned char D_8009CDBA;
extern unsigned char D_8009CF13;
extern unsigned short D_8009C960;
extern int GAME_PLAYER_AP, D_8009C98C;
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
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0x30;
            PLAYER_STEP = 0;
        }
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        return 2;
    case 0xC:
        if (GCD.bcff8) GCD.bcff8 = 0;
        else GCD.bcff8 = 1;
        playSFX(0x19);
        if (item != 0x94) {
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0x30;
            PLAYER_STEP = 0;
        }
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        return 2;
    case 0x14:
        playSFX(0x19);
        if (item != 0x94) {
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0x30;
            PLAYER_STEP = 0;
        }
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        if (GCD.bcef8) {
            GCD.bcef8 = 0;
            D_800A6101 = 0;
            return 3;
        }
        GCD.bcef8 = 1;
        D_800A6101 = 1;
        return 1;
    case 0x82:
        switch (GAME_PLAYER_MODE[1]) {
        case 0:
            GAME_PLAYER_MODE[1] = 1;
            break;
        case 1:
            GAME_PLAYER_MODE[1] = 2;
            break;
        case 2:
            GAME_PLAYER_MODE[1] = 1;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x92:
        switch (GAME_PLAYER_MODE[1]) {
        case 0:
            GAME_PLAYER_MODE[1] = 0;
            break;
        case 1:
            GAME_PLAYER_MODE[1] = 0;
            break;
        case 2:
            GAME_PLAYER_MODE[1] = 0;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x18:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        GAME_PLAYER_MODE[3] = 2;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x1C:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        GAME_PLAYER_MODE[3] = 3;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x1D:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        GAME_PLAYER_MODE[3] = 1;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x20:
        subScreenApplyPendingEquip();
        subScreenApplyPendingGauges();
        D_8009D090 = 0xFF;
        GAME_PLAYER_MODE[3] = 0;
        if (D_8009CF06) {
            PLAYER.b03 = 1;
            PLAYER.w08 = D_8007A024[4].a;
            PLAYER.wc4 = D_8007A024[4].b;
        } else {
            PLAYER.w08 = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
            PLAYER.b03 = 0;
        }
        playSFX(0x19);
        return 1;
    case 0x60:
        if (GCD.bd006 == 0) {
            if (D_8009D090 == 0xFF) D_8009D090 = GAME_PLAYER_MODE[3];
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
            if (D_8009D090 == 0xFF) D_8009D090 = GAME_PLAYER_MODE[3];
            D_8009D089 = D_8009CD9C;
            D_8009D08A = D_8009CD98;
            D_8009D08B = D_8009CD9D;
            D_8009D08C = D_8009CD99;
            D_8009D08D = D_8009CD9E;
            D_8009D08E = D_8009CD9A;
            D_8009D08F = GAME_AREA00_EVENT_CONTROL;
            subScreenApplyPendingEquip();
            D_8009CD98_s.b98 = 9;
            D_8009CD98_s.b99 = 9;
            D_8009CD98_s.b9a = 9;
            D_8009CD98_s.b9c = 0x3C;
            GAME_PLAYER_MODE[3] = 3;
            D_8009CD98_s.b9d = 0x3C;
            D_8009CD98_s.b9e = 0x3C;
            D_8009C984_a[0] = GAME_AREA00_EVENT_CONTROL | 0x40;
            { unsigned short a = D_8007A024[GAME.b30].a;
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
            GCD.bcffb = 1;
            PLAYER.w08 = a; }
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
                    t += GAME_PLAYER_MODE[3];
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
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
        }
        D_8009CD9F = 0x3C;
        D_800B1470 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
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
                    t += GAME_PLAYER_MODE[3];
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
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
        }
        D_8009CD9F = 0x3C;
        PLAYER.bcc = 1;
        D_800B1472 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
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
                    t += GAME_PLAYER_MODE[3];
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
            PLAYER.wc4 = D_8007A024[GAME_PLAYER_MODE[3]].b;
        }
        D_8009CD9F = 0x3C;
        PLAYER.bcc = 1;
        D_800B1471 = 6;
        D_800B146C = 0;
        D_800B146D = 0;
        D_800B146E = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        D_800A610B = item;
        playSFX(0x19);
        return 1;
    case 0x1B:
        GAME_PLAYER_MODE[2] = 0;
        loadCompressedTIM(0);
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x21:
        GAME_PLAYER_MODE[2] = 8;
        loadCompressedTIM(1);
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x22:
        GAME_PLAYER_MODE[2] = 9;
        loadCompressedTIM(2);
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x15:
        GAME_PLAYER_MODE[2] = 5;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x16:
        GAME_PLAYER_MODE[2] = 6;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x17:
        GAME_PLAYER_MODE[2] = 7;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        playSFX(0x19);
        return 1;
    case 0x5:
        D_8009CD96 = 3;
        D_8009CD94 = 0;
        D_8009CDA0 = 0;
        GAME_PLAYER_MODE[0] = 3;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        D_800A6038 = 3;
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        return 1;
    case 0x7C:
        D_8009CD94 = 4;
        D_8009CD96 = 0xF;
        D_8009CDA0 = 0;
        GAME_PLAYER_MODE[0] = 3;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        D_800A6038 = 3;
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        return 1;
    case 0x97:
        D_8009CD94 = 0xA;
        D_8009CD96 = 8;
        D_8009CDA0 = 0;
        GAME_PLAYER_MODE[0] = 3;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        D_800A6038 = 3;
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
        return 1;
    case 0x98:
        D_8009CD94 = 2;
        D_8009CD96 = 4;
        D_8009CDA0 = 0;
        GAME_PLAYER_MODE[0] = 3;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
        D_800A6038 = 3;
        GVARS_USE_ITEM_PENDING = 1;
        GVARS_USE_ITEM_ID = item;
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
        D_8009C98C = GAME_PLAYER_AP + 100000;
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
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0x30;
        PLAYER_STEP = 0;
    }
    GVARS_USE_ITEM_PENDING = 1;
    GVARS_USE_ITEM_ID = item;
    return 1;
}
