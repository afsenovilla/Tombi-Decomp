// FUNC 800277f8 24 MAIN0
// MATCHING 800277f8 24
extern short *PLAYER_D;
typedef struct S { char pad[0x38]; short *p; } S;
void syncObjectTargetY(S *s)
{
    s->p[1] = PLAYER_D[1];
}
