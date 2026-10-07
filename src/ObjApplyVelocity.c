// FUNC 8001fce4 48 MAIN0
// MATCHING 8001fce4 48
typedef struct TObj {
    char pad0[0x14];
    int y;
    char pad1[0x40 - 0x18];
    int *h;
    char pad2[0x80 - 0x44];
    short velH;
    short velV;
} TObj;

void ObjApplyVelocity(TObj *o)
{
    *o->h += o->velH << 8;
    o->y += o->velV << 8;
}
