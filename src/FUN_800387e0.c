// FUNC 800387e0 244 MAIN0
// MATCHING 800387e0 244
extern unsigned char *DAT_8009f0f0;
extern unsigned char *DAT_8009d60c;

void FUN_800387e0(unsigned char op)
{
    unsigned char *b = DAT_8009f0f0;
    unsigned char *code = DAT_8009d60c;
    unsigned char idx = code[*(unsigned short *)(b + 0x8a) + 1];
    unsigned char *q;
    int v;

    switch (op) {
    case 0x18:
        *(int *)(idx * 4 + (int)b + 0x1090) += 1;
        break;
    case 0x19:
        *(int *)(idx * 4 + (int)b + 0x1090) -= 1;
        break;
    case 0x1a:
        *(int *)(idx * 4 + (int)b + 0x1090) = ~*(int *)(idx * 4 + (int)b + 0x1090);
        break;
    case 0x1b:
        *(int *)(idx * 4 + (int)b + 0x1090) = -*(int *)(idx * 4 + (int)b + 0x1090);
        break;
    }
    v = *(int *)(idx * 4 + (int)b + 0x1090);
    q = DAT_8009f0f0;
    if (v == 0) {
        q[0x89] = 0;
    } else {
        q[0x89] = v < 0 ? 1 : 2;
    }
    *(short *)(b + 0x8a) = *(short *)(b + 0x8a) + 2;
}
