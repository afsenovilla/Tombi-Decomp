// FUNC 800335d4 100 MAIN0
// MATCHING 800335d4 100
typedef struct P { short x, y; } P;
typedef struct O { char p0[0x16]; short s16; char p1[0x40 - 0x18]; short *h; } O;
extern unsigned short PLAYER_Y;
extern short *PLAYER_H;
extern short angleBetweenPoints(P, P);
short angleToPlayer(O *o)
{
    P b, a;
    a.x = o->h[1];
    a.y = o->s16;
    b.x = PLAYER_H[1];
    b.y = PLAYER_Y;
    return angleBetweenPoints(a, b);
}
