// VERSION SLPS_01144 optsub00 800E9F58 240
#if defined(VERSION_SLPS_01144)
// JP buttonConfigMenuStep (PAL o0ptsub0 800e8618, USA 800E7960): pad bits 0x40 / 0x20
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
asm(".set TC_,0x1f8001d4");
asm(".set JS_,0x1f8001fc");
extern S *TC_;
extern unsigned short JS_;
extern short D_800A0F6C; /* button config index */
extern void func_80021DF4(void); /* loadButtonConfig */
extern void func_8001E2D4(int id); /* playSFX */
extern void func_8001E910(int a, int b); /* playSFXAndSetNote */
extern void func_8001DAF0(void); /* updateSound */
extern void buttonConfigMenuDraw(void);

int func_800E9F58(void)
{
    unsigned short b;
    switch (TC_->w4e) {
    case 0:
        TC_->w4e++;
        break;
    case 1:
        b = JS_;
        if (b & 0x40) {
            func_8001E910(10, 10);
            TC_->w4e = 0;
            TC_->w4c--;
        } else if (b & 0x20) {
            func_8001E2D4(8);
            if (++D_800A0F6C >= 6) D_800A0F6C = 0;
            func_80021DF4();
        }
        func_8001DAF0();
        break;
    }
    buttonConfigMenuDraw();
    return 0;
}
#endif
