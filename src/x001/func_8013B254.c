// FUNC 8013b254 228 X001
// MATCHING 8013b254 228
#include "gcd.h"
extern void func_8013A8CC(void);
extern void func_8013AF10(void);
extern void func_8013B0F4(void);
extern void func_8013AAB8(void);

void func_8013B254(void)
{
    switch (GCD.bce59) {
    case 1:
        switch (GCD.bcede) {
        case 0:
            func_8013A8CC();
            break;
        case 1:
            func_8013AAB8();
            break;
        }
        break;
    case 2:
        if (GCD.bcede == 0) func_8013AF10();
        break;
    case 3:
        if (GCD.bcede == 0) func_8013B0F4();
        break;
    case 0xff:
        func_8013AAB8();
        break;
    }
}
