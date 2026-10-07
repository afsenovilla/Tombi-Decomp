// FUNC 80122990 72 X000
// MATCHING 80122990 72
extern unsigned short DAT_8009c962;
extern unsigned char DAT_8009cda5;
extern unsigned short DAT_8009c982;

void FUN_80122990(char *o)
{
    if (DAT_8009c962 == 3 && DAT_8009cda5 == 0xff && DAT_8009c982 == 0)
        *(short *)(o + 0x20) = 0x8c;
}
