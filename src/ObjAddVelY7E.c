// FUNC 8001fd94 24 MAIN0
// MATCHING 8001fd94 24
typedef struct TObj {
    char pad0[0x14];
    int y;
    char pad1[0x7e - 0x18];
    short velV;
} TObj;

void ObjAddVelY7E(TObj *o)
{
    o->y += o->velV << 8;
}
