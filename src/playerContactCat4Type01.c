// FUNC 80124ebc 64 X000
// MATCHING 80124ebc 64
typedef struct S { char pad[0x94]; int f; } S;
extern void playerPushOutOfObjectBox(void);
extern void pushOutOfBoxX(void);
void playerContactCat4Type01(int a, S *b)
{
if (b->f == 0) playerPushOutOfObjectBox(); else pushOutOfBoxX();
}
