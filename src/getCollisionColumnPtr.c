// FUNC 8003f1bc 68 MAIN0
// MATCHING 8003f1bc 68
extern char *COLLISION_COLUMN_TABLE[];
char *getCollisionColumnPtr(short idx, unsigned char n)
{
    char *p;
    int o;
    p = COLLISION_COLUMN_TABLE[n];
    o = *(unsigned short *)(p + 8);
    p = p + o;
    o = *(unsigned short *)(p + (idx / 8 << 1));
    return p + o;
}
