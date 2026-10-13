// FUNC 8003a4cc 48 MAIN0
// MATCHING 8003a4cc 48
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; } G;
extern G *SCRIPT_CTX;
extern unsigned char SCRIPT_FLAGS[];

void scriptOpReadFlag(void)
{
    G *g = SCRIPT_CTX;
    int v = SCRIPT_FLAGS[g->d1190];
    g->w8a++;
    g->d1190 = v;
}
