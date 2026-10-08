// FUNC 800ee680 80 X010
// MATCHING 800ee680 80
typedef struct S { char pad0[0x40]; int *h; char pad1[0x7c-0x44]; short velH; } S;
extern void FUN_8010eaf8(S *);
extern void FUN_8010f0f4(S *, int *);
extern void FUN_8001fd94(S *);
void ObjMotionStep(S *o)
{
int *h; FUN_8010eaf8(o); h = o->h; *h += o->velH << 8; FUN_8010f0f4(o, h); FUN_8001fd94(o);
}
