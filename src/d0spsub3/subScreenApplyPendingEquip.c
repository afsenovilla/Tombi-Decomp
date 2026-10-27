// FUNC 800e8714 280 D0SPSUB3
// MATCHING 800e8714 280
typedef struct { unsigned short a, b; } Pair;
extern Pair D_8007A024[];
extern unsigned char D_8009C990, D_8009CF06, D_8009D006, D_8009D090, GAME_EQUIPPED_PANTS;
extern char D_800A603B;
typedef struct { char p[3]; char b3; char p4[4]; unsigned short w8; } Obj;
extern Obj PLAYER;
extern unsigned short D_800A60FC;
static inline Pair *gp(int i) { return &D_8007A024[i]; }

void subScreenApplyPendingEquip(void)
{
    if (D_8009D006 == 1) {
        GAME_EQUIPPED_PANTS = D_8009D090;
        if (D_8009CF06 == 0) {
            PLAYER.w8 = gp(D_8009C990)->a;
            D_800A603B = 0;
            D_800A60FC = gp(GAME_EQUIPPED_PANTS)->b;
        } else if (D_8009C990 == 1 || D_8009C990 == 2 || D_8009C990 == 3) {
            PLAYER.w8 = gp(D_8009C990)->a;
            D_800A603B = 0;
            D_800A60FC = gp(GAME_EQUIPPED_PANTS)->b;
        } else {
            D_800A603B = 1;
            PLAYER.w8 = D_8007A024[4].a;
            D_800A60FC = D_8007A024[4].b;
        }
        D_8009D006 = 0;
    }
}
