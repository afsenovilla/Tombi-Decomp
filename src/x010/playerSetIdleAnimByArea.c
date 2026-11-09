// FUNC 800eebc4 124 X010
// MATCHING 800eebc4 124
typedef struct S { char pad[0x24]; void *anim; char pad2[0x8c - 0x28]; int v; } S;
extern unsigned short GAME;
extern short GVARS_PLAYER_DRIFT_H;
extern char DAT_800112e0[];
extern char PLAYER_FALL_ANIM[];
extern void func_8001fe6c(S *s);
extern void func_8001fe94(S *s, int n);

void playerSetIdleAnimByArea(S *s)
{
    if (GAME == 3 && GVARS_PLAYER_DRIFT_H != 0) {
        s->anim = DAT_800112e0;
        func_8001fe6c(s);
    } else {
        s->anim = PLAYER_FALL_ANIM;
        func_8001fe94(s, 3);
    }
    s->v = 0;
}
