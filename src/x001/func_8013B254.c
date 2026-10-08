// FUNC 8013b254 228 X001
// MATCHING 8013b254 228
extern unsigned char D_8009CE59;
extern unsigned char D_8009CEDE;
extern void func_8013A8CC(void);
extern void func_8013AF10(void);
extern void func_8013B0F4(void);
extern void func_8013AAB8(void);

void func_8013B254(void)
{
    switch (D_8009CE59) {
    case 1:
        switch (D_8009CEDE) {
        case 0:
            func_8013A8CC();
            break;
        case 1:
            func_8013AAB8();
            break;
        }
        break;
    case 2:
        if (D_8009CEDE == 0) func_8013AF10();
        break;
    case 3:
        if (D_8009CEDE == 0) func_8013B0F4();
        break;
    case 0xff:
        func_8013AAB8();
        break;
    }
}
