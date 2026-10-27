// FUNC 800e94ac 416 I0NFO2
// MATCHING 800e94ac 416
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800187E4(TObj *o);
extern signed char GAME_PLAYER_MODE[];
extern int SPR_DATA[];
extern unsigned char PLAYER_VISIBLE;
typedef struct { unsigned short w; short idx; char *p; int pad; } Ent94AC;
extern Ent94AC TOPDOWN_SHADOW_DEFS[];

void topdownShadowHandler(TObj *o)
{
    TObj *p;
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        if (o->subtype == 0) {
            o->w1e = TOPDOWN_SHADOW_DEFS[0].w;
            o->d3c = SPR_DATA[TOPDOWN_SHADOW_DEFS[o->subtype].idx];
            o->anim = *(void **)(TOPDOWN_SHADOW_DEFS[o->subtype].p + 0x80);
            func_8001FE6C(o);
            o->b04++;
        }
        break;
    case 1:
        if (o->subtype == 0) {
            func_800202B4(o);
            p = (TObj *)o->d90;
            o->a.raw = p->a.raw;
            if (GAME_PLAYER_MODE[0] != 3) {
                o->y.raw = p->y.raw;
            }
            o->b.raw = p->b.raw;
            if (p->category == 0) {
                o->visible = PLAYER_VISIBLE;
            } else {
                o->visible = p->visible;
            }
            o->b0f = 0x10;
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_800187E4(o);
        break;
    }
}
