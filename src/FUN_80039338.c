// FUNC 80039338 584 MAIN0
// MATCHING 80039338 584
extern unsigned short DAT_8009c960, DAT_8009c962, DAT_8009f838;
extern int DAT_8009d69c;
extern unsigned char DAT_8009cdb1, DAT_8009cdb8, DAT_8009cdb9, DAT_8009cedd;
extern void FUN_8004f38c(int, int);

void FUN_80039338(void)
{
    DAT_8009d69c = 0;
    switch (DAT_8009c960 + DAT_8009f838) {
    case 0:
        switch (DAT_8009c962) {
        case 0:
        case 1:
        case 2:
            FUN_8004f38c(15, 6);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 1:
    case 7:
        switch (DAT_8009c962) {
        case 0:
        case 1:
            if (DAT_8009cdb1 != 0xff) {
                FUN_8004f38c(15, 0);
                DAT_8009d69c = 1;
            }
            if (DAT_8009cdb8 == 0xff) {
                if (DAT_8009cdb9 == 0) {
                    FUN_8004f38c(15, 0);
                    DAT_8009d69c = 1;
                } else if (DAT_8009cedd == 2) {
                    FUN_8004f38c(15, 0);
                    DAT_8009d69c = 1;
                }
            }
            break;
        case 2:
            FUN_8004f38c(15, 5);
            DAT_8009d69c = 1;
            break;
        case 3:
            FUN_8004f38c(15, 4);
            DAT_8009d69c = 1;
            break;
        case 4:
            FUN_8004f38c(15, 3);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 2:
        switch (DAT_8009c962) {
        case 0:
            if (DAT_8009cdb8 == 1)
                FUN_8004f38c(15, 8);
            else
                FUN_8004f38c(15, 1);
            DAT_8009d69c = 2;
            break;
        case 1:
            FUN_8004f38c(15, 2);
            DAT_8009d69c = 1;
            break;
        case 2:
            FUN_8004f38c(15, 7);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 19:
        switch (DAT_8009c962) {
        case 0:
            if (DAT_8009cdb8 == 1)
                FUN_8004f38c(15, 8);
            else
                FUN_8004f38c(15, 1);
            DAT_8009d69c = 2;
            break;
        case 1:
            FUN_8004f38c(15, 2);
            DAT_8009d69c = 1;
            break;
        }
        break;
    }
}
