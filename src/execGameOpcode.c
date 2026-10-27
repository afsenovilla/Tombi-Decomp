// FUNC 8003afb4 2888 MAIN0
// MATCHING 8003afb4 2888
#include "plx.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "scriptctx.h"
typedef struct { short s0, s2; } S2;
extern TObj *SCRIPT_OBJECTS[];
extern S2 *PLAYER_H, *PLAYER_D[];
extern short D_800A604C[];
extern unsigned char SCRIPT_FLAGS[];
extern signed char GAME_PLAYER_MODE;
extern unsigned char D_800A60A0, D_800A60A3, D_800A60D6, D_800A60A1;
extern unsigned char GAME_FADE_SCREEN_CONTROL, D_8009C93C, D_8009C976;
extern short D_800A6066;
extern int D_8009C96C;
extern char PLAYER[];
extern short PLAYER_HEALTH[];
extern unsigned char D_8009C971;
extern short D_800A60D2[];
extern unsigned char D_8009C970[];
extern void scriptOpSpawnObject(void);
extern void FUN_80039e54(void);
extern void scriptOpSetPlayerStateEx(void);
extern void objSetAnimFromSubtypeTable(TObj *);
extern void func_800ECBBC(TObj *);
extern void func_800E8348(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_800ecb40(char *, int, int);
extern void awardEventProgress(int, int, int);
extern void printInfoMessage(int, int);
extern void playSFXWithNoteAndVolume(int, int, int);
extern void func_800EDFB8(TObj *, int);
extern void playSFX(int);
extern void increaseMaxHealth(void);
extern void addItemToInventory(int, int, int);
extern void playerSetAnimIfChangedAt(char *, int, int);

static __inline__ int op81(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o;
    int b, a;
    a = g->arg[1];
    o = SCRIPT_OBJECTS[g->arg[0]];
    b = g->arg[2];
    if (o != 0) {
        o->w74 = a;
        o->w76 = b;
        switch (o->type & 0x7f) {
        case 0x18:
            objSetAnimFromSubtypeTable(o);
            break;
        case 0x19:
            func_800ECBBC(o);
            break;
        case 0x2e:
            func_800E8348(o);
            break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op82(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj **p = &SCRIPT_OBJECTS[g->arg[0]];
    TObj *o = *p;
    if (o != 0) {
        o->b04 = 3;
        if (o->d94 != 0) ((TObj *)o->d94)->b04 = 3;
        *p = 0;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op83(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    PLAYER_H->s2 = g->arg[0];
    D_800A604C[1] = g->arg[1];
    PLAYER_D[0]->s2 = g->arg[2];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op84(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    int v;
    g->arg[0] = PLAYER_H->s2;
    g->arg[1] = D_800A604C[1];
    v = PLAYER_D[0]->s2;
    (*(short *)((char *)g + 0x8a))++;
    g->arg[2] = v;
    return 1;
}

static __inline__ int op86(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) g->arg[0] = o->b6a;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op87(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        o->h->p.whole = g->arg[1];
        o->y.p.whole = g->arg[2];
        o->d->p.whole = g->arg[3];
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op88(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        g->arg[0] = o->h->p.whole;
        g->arg[1] = o->y.p.whole;
        g->arg[2] = o->d->p.whole;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op89(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) o->animFrame = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8a(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) g->arg[0] = o->animFrame;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8b(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) g->arg[0] = o->b04 == 2;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8c(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) o->active = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8d(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        FUN_800202b4(o);
        g->arg[0] = o->visible;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8e(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    int v = SCRIPT_FLAGS[g->arg[0]];
    (*(short *)((char *)g + 0x8a))++;
    g->arg[0] = v;
    return 1;
}

static __inline__ int op8f(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    SCRIPT_FLAGS[g->arg[0]] = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op90(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        int t = o->type & 0x7f;
        if (t == 0x18) goto l;
        if (t == 0x19) {
        l:
            g->arg[0] = o->b68;
        }
    } else {
        g->arg[0] = 0;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op91(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    int v = SCRIPT_GLOBALS[g->arg[0]];
    (*(short *)((char *)g + 0x8a))++;
    g->arg[0] = v;
    return 1;
}

static __inline__ int op92(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    SCRIPT_GLOBALS[g->arg[0]] = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op93(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    if (GAME_PLAYER_MODE != 3) FUN_800ecb40(PLAYER, PLAYER_H->s2, D_800A604C[1]);
    g->arg[0] = D_800A60A0;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op94(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if ((GAME_PLAYER_MODE != 3 || D_800A60A3 == 1) && o != 0) o->b6b = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op95(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) {
        switch (o->type & 0x7f) {
        case 0x18:
            o->b0f = g->arg[1];
            break;
        case 0x19:
        case 0x2e:
            {
                int c = g->arg[1];
                ((unsigned char *)o)[0xe] = c;
                switch (c & 0xff) {
                case 1: *(signed char *)&o->b0f = -11; break;
                case 2: o->b0f = 0x10; break;
                }
            }
            break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op96(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    if (GAME_PLAYER_MODE != 3) {
        switch (g->arg[0]) {
        case 0: D_800A60A3 = 0; break;
        case 1: D_800A60A3 = 1; break;
        default: D_800A60A3 = 2; break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op98(void)
{
    char *p = (char *)SCRIPT_CTX;
    *(int *)(p + 0x1190) = D_800A60D6;
    *(short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int op99(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    D_800A6066 = g->arg[0];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9a(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    {
        int a = g->arg[0];
        if (g->arg[1]) awardEventProgress(a, 1, 0);
        else awardEventProgress(a, 0, 0);
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9b(void)
{
    unsigned char *p = (unsigned char *)SCRIPT_CTX;
    *(int *)(p + 0x1190) = D_800A60A1;
    (*(short *)(p + 0x8a))++;
    return 1;
}

static __inline__ int op9c(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    GVARS.b42 = g->arg[0];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9d(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    printInfoMessage(g->arg[0], 2);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9e(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    playSFXWithNoteAndVolume(g->arg[0], g->arg[1], g->arg[2]);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9f(void)
{
    unsigned char *g = (unsigned char *)SCRIPT_CTX;
    unsigned char *p = (unsigned char *)SCRIPT_OBJECTS[*(int *)(g + 0x1190)];
    int f = *(int *)(g + 0x1194);
    if (p != 0 && p[2] == 0x19) {
        if (f == 0) {
            (*(unsigned char **)(p + 0x94))[4] = 3;
            *(int *)(p + 0x94) = 0;
        } else {
            func_800EDFB8((TObj *)p, 1);
        }
    }
    (*(short *)(g + 0x8a))++;
    return 1;
}

static __inline__ int opa0(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    GAME_FADE_SCREEN_CONTROL = 0x10;
    D_8009C93C = g->arg[0];
    D_8009C976 = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa2(void)
{
    char *p = (char *)SCRIPT_CTX;
    int a = *(int *)(p + 0x1190);
    int b = *(int *)(p + 0x1194);
    int c = *(int *)(p + 0x1198);
    if (a < 0) {
        if (c != 0) {
            printInfoMessage(0x15, 3);
            playSFX(10);
        }
        increaseMaxHealth();
    } else {
        addItemToInventory(a, b, c);
    }
    *(unsigned short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int opa3(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) g->arg[0] = o->b69;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa4(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) o->b69 = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa5(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[g->arg[0]];
    if (o != 0) o->b9c = g->arg[1];
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa6(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    playerSetAnimIfChangedAt(PLAYER, (short)g->arg[0], (short)g->arg[1]);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa7(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    g->arg[0] = GV2.bb1;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa8(void)
{
    char *p = (char *)SCRIPT_CTX;
    *(int *)(p + 0x1190) = PLX.b611a;
    *(unsigned short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int opa9(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    unsigned char m = D_8009C971;
    short v = PLAYER_HEALTH[0];
    int a = g->arg[0];
    if (v < m) {
        PLAYER_HEALTH[0] = v + a;
        if (PLAYER_HEALTH[0] > m) PLAYER_HEALTH[0] = m;
        D_800A60D2[0] = PLAYER_HEALTH[0];
        D_8009C970[0] = PLAYER_HEALTH[0];
        g->arg[0] = 0;
    } else {
        g->arg[0] = 1;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opaa(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    g->arg[0] = D_8009C96C;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opab(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    (*(short *)((char *)g + 0x8a))++;
    return 0;
}

static __inline__ int op97(void)
{
    if (GVARS.b3a == 0) return 0;
    scriptOpSetPlayerStateEx();
    return 1;
}

int execGameOpcode(unsigned char op)
{
    int r = 0;

    switch (op) {
    case 0xab:
        r = opab();
        break;
    case 0x80: scriptOpSpawnObject(); r = 1; break;
    case 0x81: r = op81(); break;
    case 0x82: r = op82(); break;
    case 0x83: r = op83(); break;
    case 0x84: r = op84(); break;
    case 0x85: FUN_80039e54(); r = 1; break;
    case 0x86: r = op86(); break;
    case 0x87: r = op87(); break;
    case 0x88: r = op88(); break;
    case 0x89: r = op89(); break;
    case 0x8a: r = op8a(); break;
    case 0x8b: r = op8b(); break;
    case 0x8c: r = op8c(); break;
    case 0x8d: r = op8d(); break;
    case 0x8f: r = op8f(); break;
    case 0x8e: r = op8e(); break;
    case 0x90: r = op90(); break;
    case 0x91: r = op91(); break;
    case 0x92: r = op92(); break;
    case 0x93: r = op93(); break;
    case 0x94: r = op94(); break;
    case 0x95: r = op95(); break;
    case 0x96: r = op96(); break;
    case 0x98: r = op98(); break;
    case 0x99: r = op99(); break;
    case 0x9a: r = op9a(); break;
    case 0x9b: r = op9b(); break;
    case 0x9c: r = op9c(); break;
    case 0x9d: r = op9d(); break;
    case 0x9e: r = op9e(); break;
    case 0x9f: r = op9f(); break;
    case 0xa0: r = opa0(); break;
    case 0xa1: r = op9d(); break;
    case 0xa2: r = opa2(); break;
    case 0xa3: r = opa3(); break;
    case 0xa4: r = opa4(); break;
    case 0xa5: r = opa5(); break;
    case 0xa6: r = opa6(); break;
    case 0xa7: r = opa7(); break;
    case 0xa8: r = opa8(); break;
    case 0xa9: r = opa9(); break;
    case 0xaa: r = opaa(); break;
    case 0x97:
        r = op97();
        break;
    }
    return r;
}
