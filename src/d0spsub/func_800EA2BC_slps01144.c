// VERSION SLPS_01144 dspsub 800EA2BC 4244
#if defined(VERSION_SLPS_01144)
// JP twin of PAL d0spsub func_800E89F8 (item use). JP differences: no subScreenApplyPendingEquip/Gauges calls
// and no D_8009D090/PLAYER.b03 bookkeeping in the 0x18/0x1C/0x1D/0x20 cases; case 0x60 inlines an older
// ApplyPendingEquip (no else arm) and case 0x84 saves/restores the gauges inline; the toggles 0x62..0x64 also
// store 0x3C to their gauge byte on the way off and drop the PLAYER_STATE/item stores; 0x15..0x17 and the TIM
// cases only set the mode byte; 0x05/0x7C/0x97/0x98 drop the mode store; the default switch has item 8
// (func_8001C988(0xA9, 1)). Match notes: the 48-byte frame comes from the inline gp() calls (each integration
// leaves an 8-byte slot): 0x60 uses gp() in its second arm only (the different RTL stops the cross-jump of the
// two arms, the shared wc4 store still merges), 0x84's restore arm uses gp() for both reads. Case 0x84's save arm
// addresses every global as a GAME field (one base symbol: only the first access, the flag test, gets an `la`,
// and the in-struct stores keep the retail order); the restore arm uses the separate D_8009E940/E944/EC3x
// structs. Retail order of the save arm: copies interleaved, then 0x3C/9 pairs per gauge, then mode 3.
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
extern Pal D_80076B84[];
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

typedef struct {          /* the player object from +8 (JP 800A7B78) */
    unsigned short w08;
    char p0a[0x98 - 0xA];
    unsigned short w98;
    char p9a[0xC4 - 0x9A];
    unsigned short wc4;
    char pc6[0xCC - 0xC6];
    unsigned char bcc;
} Pl;
extern Pl D_800A7B80;
typedef struct { unsigned char b98, b99, b9a; } T98;
typedef struct { unsigned char b9c, b9d, b9e; } T9C;
typedef struct { unsigned char b8a, b8b; } T8A;
typedef struct { unsigned char b8c, b8d, b8e, b8f; } T8C;
extern T98 D_8009E940; extern T9C D_8009E944;
typedef struct {
    char p00[0x10]; unsigned char b10, b11;
    char p12[0x24 - 0x12]; int ev;   /* PAL 8009C984 */
    char p28[0x8]; unsigned char b30; /* PAL 8009C990 */
    char p31[0x438 - 0x31];
    unsigned char b98, b99, b9a, b9b, b9c, b9d, b9e; /* PAL 8009CD98 */
    char p43f[0x69B - 0x43F]; unsigned char bcffb; /* PAL 8009CFFB */
    char p69c[0x729 - 0x69C];
    unsigned char b89, b8a, b8b, b8c, b8d, b8e, b8f, b90; /* PAL 8009D089 */
    char p731[0x953 - 0x731]; unsigned char m3; /* PAL 8009D2B3 */
} GamePart;
extern GamePart GAME;
extern unsigned char D_8009EC31, D_8009EC38; extern T8A D_8009EC32; extern T8C D_8009EC34;

static inline Pal *gp(int i) { return &D_80076B84[i]; }
extern unsigned char D_800A7B78;
extern unsigned char D_800A7B7C;
extern unsigned char D_800A7B7D;
extern unsigned char D_800A7B7E;
extern unsigned short D_800A7C12;
extern unsigned char D_800A7C41;
extern unsigned char D_8009EAAE;
extern unsigned char D_8009EE58[];
extern unsigned char D_8009E4E8, D_8009E4E9;
extern unsigned char D_800B2F3C, D_800B2F3D, D_800B2F3E, D_800B2F40, D_800B2F41, D_800B2F42;
extern short D_8009E93C, D_8009E93E, D_8009E948;
extern unsigned char D_8009E962;
extern unsigned char D_8009EABB;
extern unsigned short D_8009E508;
extern int D_8009E514, D_8009E534;
extern unsigned char D_8009EAA2, D_8009E4E7, D_8009EAAF, D_8009EAB0, D_8009EAB1;
extern char *D_1F800334;
#define P334 (*(char * volatile *)&D_1F800334)
extern void playSFX(int id);
extern void loadCompressedTIM(int n);
extern void removeItemFromInventory(int id, int n);
extern void func_8001C988(int a, int b);
extern Obj *allocObjectLayer4(void);

int func_800EA2BC(int item)
{
    Obj *o;
    switch (item) {
    case 0xA:
        if (GCD.bcef7) GCD.bcef7 = 0;
        else GCD.bcef7 = 1;
        playSFX(0x19);
        if (item != 0x94) {
            D_800A7B7C = 1;
            D_800A7B7D = 0x30;
            D_800A7B7E = 0;
        }
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        return 2;
    case 0xC:
        if (GCD.bcff8) GCD.bcff8 = 0;
        else GCD.bcff8 = 1;
        playSFX(0x19);
        if (item != 0x94) {
            D_800A7B7C = 1;
            D_800A7B7D = 0x30;
            D_800A7B7E = 0;
        }
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        return 2;
    case 0x14:
        playSFX(0x19);
        if (item != 0x94) {
            D_800A7B7C = 1;
            D_800A7B7D = 0x30;
            D_800A7B7E = 0;
        }
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        if (GCD.bcef8) {
            GCD.bcef8 = 0;
            D_800A7C41 = 0;
            return 3;
        }
        GCD.bcef8 = 1;
        D_800A7C41 = 1;
        return 1;
    case 0x82:
        switch (D_8009EE58[1]) {
        case 0:
            D_8009EE58[1] = 1;
            break;
        case 1:
            D_8009EE58[1] = 2;
            break;
        case 2:
            D_8009EE58[1] = 1;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x92:
        switch (D_8009EE58[1]) {
        case 0:
            D_8009EE58[1] = 0;
            break;
        case 1:
            D_8009EE58[1] = 0;
            break;
        case 2:
            D_8009EE58[1] = 0;
            break;
        }
        playSFX(0x19);
        return 1;
    case 0x18:
        D_8009EE58[3] = 2;
        if (D_8009EAAE) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[GAME.b30].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        playSFX(0x19);
        return 1;
    case 0x1C:
        D_8009EE58[3] = 3;
        if (D_8009EAAE) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[GAME.b30].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        playSFX(0x19);
        return 1;
    case 0x1D:
        D_8009EE58[3] = 1;
        if (D_8009EAAE) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[GAME.b30].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        playSFX(0x19);
        return 1;
    case 0x20:
        D_8009EE58[3] = 0;
        if (D_8009EAAE) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[GAME.b30].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        playSFX(0x19);
        return 1;
    case 0x60:
        if (GCD.bd006 == 0) {
            GCD.bd006 = 1;
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
            playSFX(0x19);
            return 1;
        }
        if (D_8009EAAE == 0) {
            D_800A7B80.w08 = gp(GAME.b30)->a;
            D_800A7B80.wc4 = gp(D_8009EE58[3])->b;
        } else if (GAME.b30 == 1 || GAME.b30 == 2 || GAME.b30 == 3) {
            D_800A7B80.w08 = gp(GAME.b30)->a;
            D_800A7B80.wc4 = gp(D_8009EE58[3])->b;
        }
        GCD.bd006 = 0;
        playSFX(0x19);
        return 4;
    case 0x84:
        if (GAME.bcffb == 0) {
            GAME.b89 = GAME.b9c;
            GAME.b8a = GAME.b98;
            GAME.b8b = GAME.b9d;
            GAME.b8c = GAME.b99;
            GAME.b8d = GAME.b9e;
            GAME.b8e = GAME.b9a;
            GAME.b8f = GAME.ev;
            GAME.b90 = GAME.m3;
            GAME.b9c = 0x3C;
            GAME.b98 = 9;
            GAME.b9d = 0x3C;
            GAME.b99 = 9;
            GAME.b9e = 0x3C;
            GAME.b9a = 9;
            GAME.m3 = 3;
            GAME.ev = GAME.ev | 0x40;
            D_800A7B80.w08 = D_80076B84[GAME.b30].a;
            D_800A7B80.wc4 = D_80076B84[GAME.m3].b;
            GAME.bcffb = 1;
            playSFX(0x19);
            return 1;
        }
        D_8009E944.b9c = D_8009EC31;
        D_8009E940.b98 = D_8009EC32.b8a;
        D_8009E944.b9d = D_8009EC32.b8b;
        D_8009E940.b99 = D_8009EC34.b8c;
        D_8009E944.b9e = D_8009EC34.b8d;
        D_8009E940.b9a = D_8009EC34.b8e;
        GAME.ev = D_8009EC34.b8f;
        D_8009EE58[3] = D_8009EC38;
        D_800A7B80.w08 = gp(GAME.b30)->a;
        D_800A7B80.wc4 = gp(D_8009EE58[3])->b;
        GAME.bcffb = 0;
        playSFX(0x19);
        return 5;
    case 0x62:
        if (GAME.b30 == 1) {
            GAME.b30 = 0;
            if (D_8009EAAE || GCD.bd006) {
                D_800A7B80.w08 = D_80076B84[4].a;
                D_800A7B80.wc4 = D_80076B84[4].b;
            } else {
                {
                    Pal *t = D_80076B84; asm volatile("" : "=r"(t) : "0"(t));
                    D_800A7B80.w08 = t->a;
                    t += D_8009EE58[3];
                    D_800A7B80.wc4 = t->b;
                }
            }
            D_8009E944.b9c = 0x3C;
            D_800A7B80.bcc = 0;
            D_800B2F3C = 0;
            return 2;
        }
        GAME.b30 = 1;
        if (D_8009EAAE || GCD.bd006) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[1].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        D_8009E944.b9c = 0x3C;
        D_800B2F40 = 6;
        D_800B2F3C = 0;
        D_800B2F3D = 0;
        D_800B2F3E = 0;
        D_800A7B80.bcc |= 1;
        playSFX(0x19);
        return 1;
    case 0x63:
        if (GAME.b30 == 2) {
            GAME.b30 = 0;
            if (D_8009EAAE || GCD.bd006) {
                D_800A7B80.w08 = D_80076B84[4].a;
                D_800A7B80.wc4 = D_80076B84[4].b;
            } else {
                {
                    Pal *t = D_80076B84; asm volatile("" : "=r"(t) : "0"(t));
                    D_800A7B80.w08 = t->a;
                    t += D_8009EE58[3];
                    D_800A7B80.wc4 = t->b;
                }
            }
            D_8009E944.b9e = 0x3C;
            D_800A7B80.bcc = 0;
            D_800B2F3E = 0;
            return 2;
        }
        GAME.b30 = 2;
        if (D_8009EAAE || GCD.bd006) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[2].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        D_8009E944.b9e = 0x3C;
        D_800A7B80.bcc = 1;
        D_800B2F42 = 6;
        D_800B2F3C = 0;
        D_800B2F3D = 0;
        D_800B2F3E = 0;
        playSFX(0x19);
        return 1;
    case 0x64:
        if (GAME.b30 == 3) {
            GAME.b30 = 0;
            if (D_8009EAAE || GCD.bd006) {
                D_800A7B80.w08 = D_80076B84[4].a;
                D_800A7B80.wc4 = D_80076B84[4].b;
            } else {
                {
                    Pal *t = D_80076B84; asm volatile("" : "=r"(t) : "0"(t));
                    D_800A7B80.w08 = t->a;
                    t += D_8009EE58[3];
                    D_800A7B80.wc4 = t->b;
                }
            }
            D_8009E944.b9d = 0x3C;
            D_800A7B80.bcc = 0;
            D_800B2F3D = 0;
            return 2;
        }
        GAME.b30 = 3;
        if (D_8009EAAE || GCD.bd006) {
            D_800A7B80.w08 = D_80076B84[4].a;
            D_800A7B80.wc4 = D_80076B84[4].b;
        } else {
            D_800A7B80.w08 = D_80076B84[3].a;
            D_800A7B80.wc4 = D_80076B84[D_8009EE58[3]].b;
        }
        D_8009E944.b9d = 0x3C;
        D_800A7B80.bcc = 1;
        D_800B2F41 = 6;
        D_800B2F3C = 0;
        D_800B2F3D = 0;
        D_800B2F3E = 0;
        playSFX(0x19);
        return 1;
    case 0x1B:
        D_8009EE58[2] = 0;
        loadCompressedTIM(0);
        playSFX(0x19);
        return 1;
    case 0x21:
        D_8009EE58[2] = 8;
        loadCompressedTIM(1);
        playSFX(0x19);
        return 1;
    case 0x22:
        D_8009EE58[2] = 9;
        loadCompressedTIM(2);
        playSFX(0x19);
        return 1;
    case 0x15:
        D_8009EE58[2] = 5;
        playSFX(0x19);
        return 1;
    case 0x16:
        D_8009EE58[2] = 6;
        playSFX(0x19);
        return 1;
    case 0x17:
        D_8009EE58[2] = 7;
        playSFX(0x19);
        return 1;
    case 0x5:
        D_8009E93E = 3;
        D_8009E93C = 0;
        D_8009E948 = 0;
        D_800A7B7C = 1;
        D_800A7B7D = 0x30;
        D_800A7B7E = 0;
        D_800A7B78 = 3;
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        return 1;
    case 0x7C:
        D_8009E93C = 4;
        D_8009E93E = 0xF;
        D_8009E948 = 0;
        D_800A7B7C = 1;
        D_800A7B7D = 0x30;
        D_800A7B7E = 0;
        D_800A7B78 = 3;
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        return 1;
    case 0x97:
        D_8009E93C = 0xA;
        D_8009E93E = 8;
        D_8009E948 = 0;
        D_800A7B7C = 1;
        D_800A7B7D = 0x30;
        D_800A7B7E = 0;
        D_800A7B78 = 3;
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
        return 1;
    case 0x98:
        D_8009E93C = 2;
        D_8009E93E = 4;
        D_8009E948 = 0;
        D_800A7B7C = 1;
        D_800A7B7D = 0x30;
        D_800A7B7E = 0;
        D_800A7B78 = 3;
        D_8009E4E8 = 1;
        D_8009E4E9 = item;
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
            D_8009E962 = 2;
            return 1;
        }
        return 1;
    case 0x6F:
        D_800A7B80.w98 += 4;
        if (GAME.b11 < (short)D_800A7B80.w98) D_800A7B80.w98 = GAME.b11;
        D_800A7C12 = D_800A7B80.w98;
        GAME.b10 = D_800A7B80.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x9A:
        D_8009EABB = 0;
        D_800A7B80.w98 = GAME.b11;
        D_800A7C12 = D_800A7B80.w98;
        GAME.b10 = D_800A7B80.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x1E:
        if (D_8009E508 == 0x13) break;
    case 0x70:
        D_800A7B80.w98 += 8;
        if (GAME.b11 < (short)D_800A7B80.w98) D_800A7B80.w98 = GAME.b11;
        D_800A7C12 = D_800A7B80.w98;
        GAME.b10 = D_800A7B80.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x1F:
        if (D_8009E508 == 0x13) break;
    case 0x71:
        D_800A7B80.w98 = GAME.b11;
        D_800A7C12 = D_800A7B80.w98;
        GAME.b10 = D_800A7B80.w98;
        removeItemFromInventory(item, 1);
        return 1;
    case 0x3F:
        D_8009E534 = D_8009E514 + 100000;
        func_8001C988(0xF, 1);
        removeItemFromInventory(0x3F, 1);
        D_8009EAA2 = 1;
        return 1;
    default:
        if (item == 3 && D_8009E508 == 0) D_8009E4E7 = 1;
        switch (item) {
        case 8:
            func_8001C988(0xA9, 1);
            break;
        case 0x75:
            removeItemFromInventory(0x75, 1);
            break;
        case 0x9C:
            D_8009EAAF = 1;
            removeItemFromInventory(0x9C, 1);
            break;
        case 0x9D:
            D_8009EAB0 = 1;
            removeItemFromInventory(0x9D, 1);
            break;
        case 0x9E:
            D_8009EAB1 = 1;
            removeItemFromInventory(0x9E, 1);
            break;
        case 0x3C:
            if (D_8009E508 == 0x11) removeItemFromInventory(0x3C, 1);
            break;
        }
        break;
    }
    if (item != 0x94) {
        D_800A7B7C = 1;
        D_800A7B7D = 0x30;
        D_800A7B7E = 0;
    }
    D_8009E4E8 = 1;
    D_8009E4E9 = item;
    return 1;
}
#endif
