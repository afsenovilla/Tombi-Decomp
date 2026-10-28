// FUNC 800f5014 100 X000
// MATCHING 800f5014 100
// probably: g(0,0,0xff,2); h(3,a); copies GRABBED_OBJECT->b0c to PLAYER_EXT->b0a; unreferenced
typedef struct { char p[0xa]; char b0a; char b0b; char b0c; } S;
extern S *GRABBED_OBJECT, *PLAYER_EXT;
extern void g(int a, int b, int c, int d);
extern void h(int a, int b);

void copyGrabbedObjB0cToPlayerExt(short a)
{
    g(0, 0, 0xff, 2);
    h(3, a);
    PLAYER_EXT->b0a = GRABBED_OBJECT->b0c;
}
