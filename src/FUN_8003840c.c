// FUNC 8003840c 264 MAIN0
// MATCHING 8003840c 264
extern unsigned char *DAT_8009f0f0;
extern int DAT_8009d60c;

void FUN_8003840c(unsigned char op)
{
    short tmp;
    int code = DAT_8009d60c;
    unsigned char *o = DAT_8009f0f0;
    unsigned char r;
    unsigned char *p;
    int k;
    switch (op) {
    case 8:
        r = 1;
        break;
    case 9:
    case 11:
        r = o[0x89] == 0;
        break;
    case 10:
    case 12:
        r = o[0x89] != 0;
        break;
    case 14:
        r = o[0x89] == 2;
        break;
    case 16:
        r = o[0x89] != 1;
        break;
    case 13:
        r = o[0x89] == 1;
        break;
    case 15:
        r = o[0x89] != 2;
        break;
    }
    if (r) {
        p = (unsigned char *)(*(unsigned short *)(o + 0x8a) + code) + 1;
        for (k = 0; k < 2; k++)
            ((char *)&tmp)[k] = p[k];
        *(short *)(o + 0x8a) = tmp;
    } else {
        *(short *)(o + 0x8a) += 3;
    }
}
