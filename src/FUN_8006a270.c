// FUNC 8006a270 132 MAIN0
extern void FUN_8006adc8(char *);
extern void FUN_8006addc(char *, int);
extern void FUN_8006ae1c(char *, int);

void FUN_8006a270(char *o)
{
    switch ((unsigned char)o[0x46]) {
    case 2:
        FUN_8006adc8(o);
        break;
    case 3:
        FUN_8006addc(o, (unsigned char)o[0xe4]);
        break;
    case 4:
        FUN_8006ae1c(o, (unsigned char)o[0x47]);
        break;
    }
}
