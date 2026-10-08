// FUNC 8011de84 812 X000
// MATCHING 8011de84 812
typedef struct {
    char pad0[0x10];
    int a;          /* +0x10 */
    int y;          /* +0x14 */
    char pad18[0x16];
    unsigned short animFrame; /* +0x2e */
    int d30;
    int d34;
    char pad38[0x34];
    short box0;     /* +0x6c */
    short box1;     /* +0x6e */
    short box2;
    short box3;     /* +0x72 */
    char pad74[8];
    short velX;     /* +0x7c */
    short velY;     /* +0x7e */
    char pad80[0xc];
    int d8c;        /* +0x8c */
    char pad90[4];
    struct O *d94;  /* +0x94 */
    char pad98[0xd];
    signed char ba5; /* +0xa5 */
} O;
typedef struct O { char pad0[0x10]; short a_lo, a_hi; short y_lo, y_hi; } P;
extern short D_8007A1F0[];
extern short D_8007A5F0[];
extern short D_8007A208[];
extern short D_8007A1F4[];
extern short D_8007A5F4[];

void func_8011DE84(O *o)
{
    int s, c;
    P *q;
    o->d8c = (0x80 - o->animFrame) << 4;
    s = (o->ba5 * D_8007A1F0[o->animFrame]) << 4;
    c = (o->ba5 * D_8007A5F0[o->animFrame]) << 4;
    q = o->d94;
    *(int *)&q->y_lo = o->y + s + (o->velY << 16);
    o->d34 = q->y_hi;
    *(int *)&q->a_lo = o->a + c;
    o->d30 = q->a_hi;
    o->box1 = c < 0 ? -c >> 16 : c >> 16;
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + (s < 0 ? -s >> 16 : s >> 16);
}

void func_8011DF5C(O *o)
{
    int s, c;
    P *q;
    o->d8c = (0x80 - o->animFrame) << 4;
    s = (o->ba5 * D_8007A208[o->animFrame]) << 4;
    c = o->ba5 << 16;
    q = o->d94;
    *(int *)&q->y_lo = o->y + s + (o->velY << 16);
    o->d34 = q->y_hi;
    *(int *)&q->a_lo = o->a + c;
    o->d30 = q->a_hi;
    o->box1 = c < 0 ? -c >> 16 : c >> 16;
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + (s < 0 ? -s >> 16 : s >> 16);
}

void func_8011E028(O *o)
{
    int s, c;
    P *q;
    o->d8c = (0x80 - o->animFrame) << 4;
    s = (o->ba5 * D_8007A1F4[o->animFrame]) << 4;
    c = (o->ba5 * D_8007A5F4[o->animFrame]) << 4;
    o->d34 = (o->y + s) >> 16;
    o->d30 = (o->a + c) >> 16;
    o->box1 = c < 0 ? -c >> 16 : c >> 16;
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + (s < 0 ? -s >> 16 : s >> 16);
}

void func_8011E0EC(O *o)
{
    int s, c;
    P *q;
    o->d8c = (0x80 - o->animFrame) << 4;
    s = (o->ba5 * D_8007A1F0[o->animFrame]) << 4;
    c = (o->ba5 * D_8007A5F0[o->animFrame]) << 4;
    o->d34 = (o->y + s) >> 16;
    o->d30 = (o->a + c) >> 16;
    o->box1 = c < 0 ? -c >> 16 : c >> 16;
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + (s < 0 ? -s >> 16 : s >> 16);
}
