// FUNC 8003abc0 144 MAIN0
// MATCHING 8003abc0 144
extern unsigned char *SCRIPT_CTX;
extern unsigned char *SCRIPT_OBJECTS[];
void func_800EDFB8(unsigned char *a, int b);
void func_8003ABC0(void)
{
    unsigned char *g = SCRIPT_CTX;
    unsigned char *p = SCRIPT_OBJECTS[*(int *)(g + 0x1190)];
    int f = *(int *)(g + 0x1194);
    if (p != 0 && p[2] == 0x19) {
        if (f == 0) {
            (*(unsigned char **)(p + 0x94))[4] = 3;
            *(int *)(p + 0x94) = 0;
        } else {
            func_800EDFB8(p, 1);
        }
    }
    (*(short *)(g + 0x8a))++;
}
