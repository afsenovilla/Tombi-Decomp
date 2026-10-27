// FUNC 800ee9cc 112 X000
// MATCHING 800ee9cc 112
typedef struct TO {
    char p0[0x14];
    int y;
    char p1[0x40 - 0x18];
    int *h;
    char p2[0xb2 - 0x44];
    short w2;
    char p3[0xb6 - 0xb4];
    short w6;
} TO;
extern short GVARS_PLAYER_DRIFT_H, GVARS_PLAYER_DRIFT_Y;
extern void g(TO *o, int a, int b);
extern void ObjApplyVelocity(TO *o);

static __inline__ void add(TO *p)
{
    *p->h += GVARS_PLAYER_DRIFT_H << 8;
    p->y += GVARS_PLAYER_DRIFT_Y << 8;
}

void playerMoveByAngleSpeed(TO *o)
{
    g(o, o->w6, o->w2);
    add(o);
    ObjApplyVelocity(o);
}
