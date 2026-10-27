// FUNC 80021aec 52 MAIN0
// MATCHING 80021aec 52
typedef struct E { char pad[6]; unsigned short v; } E;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern char D_8007882C[];
extern char D_8007885C[];
extern char D_80078884[];
extern char D_800788B4[];
extern char D_800788E4[];
static E *DAT_80078b7c[5] = {
    (E *)D_8007882C, (E *)D_8007885C, (E *)D_80078884, (E *)D_800788B4,
    (E *)D_800788E4,
};
typedef struct S { char pad[0x32]; unsigned short v; } S;
void loadSectionHeight(S *o)
{
int a = GAME; int b = GAME_SELECTED_SECTION; unsigned short *p = &DAT_80078b7c[a][b].v; o->v = *p;
}
