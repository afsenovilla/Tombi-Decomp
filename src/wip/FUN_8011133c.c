// FUNC 8011133c 128 X000
/* falta: el juego pone s en a0, o en a1 (move a1,a0 al inicio) y una copia de o en a2 para el sh $zero;
   ademas carga 0x200 antes del primer lhu (no en el delay slot). short s ya da el li 0x200 doble. */
extern unsigned short DAT_8009d610, DAT_8009d670;
extern unsigned char DAT_8009d618;

void FUN_8011133c(char *o)
{
    short s = 0x200;
    volatile unsigned short *k;
    if (DAT_8009d610 == 7)
        s = 0x200 >> (3 - DAT_8009d618);
    k = &DAT_8009d670;
    if (*k & 0x80)
        *(short *)(o + 0x7c) = -s;
    else if (*k & 0x20)
        *(short *)(o + 0x7c) = s;
    else
        *(short *)(o + 0x7c) = 0;
}
