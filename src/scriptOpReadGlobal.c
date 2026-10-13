// FUNC 8003a570 48 MAIN0
// MATCHING 8003a570 48
typedef struct {
    char pad[0x8a];
    unsigned short w8a;
    char pad2[0x1190 - 0x8c];
    int d1190;
} S8003A570;

extern S8003A570 *SCRIPT_CTX;
extern unsigned char SCRIPT_GLOBALS[];

void scriptOpReadGlobal(void)
{
    S8003A570 *p = SCRIPT_CTX;
    p->d1190 = SCRIPT_GLOBALS[p->d1190];
    p->w8a++;
}
