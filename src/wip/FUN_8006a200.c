// FUNC 8006a200 40 MAIN0
extern volatile unsigned short *PTR_JOY;

void FUN_8006a200(void)
{
    volatile unsigned short *p = PTR_JOY;
    while ((p[2] & 2) == 0)
        ;
}
