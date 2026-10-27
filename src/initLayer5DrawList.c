// FUNC 800180f8 48 MAIN0
// MATCHING 800180f8 48
extern char DAT_800b1190[];
extern char *CAT5_OBJECT_LIST;
extern char *DAT_1f80026c;
extern short DAT_1f800256;
extern short CAT5_OBJECT_COUNT;
void initLayer5DrawList(void)
{
    CAT5_OBJECT_LIST = DAT_800b1190;
    DAT_1f80026c = DAT_800b1190;
    DAT_1f800256 = 0;
    CAT5_OBJECT_COUNT = 0;
}
