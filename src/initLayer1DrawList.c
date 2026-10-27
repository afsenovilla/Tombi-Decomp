// FUNC 80017f50 48 MAIN0
// MATCHING 80017f50 48
extern char OBJECT_POOL[];
extern char *CAT2_OBJECT_LIST;
extern char *p260;
extern short s250;
extern short CAT2_OBJECT_COUNT;

void initLayer1DrawList(void)
{
    CAT2_OBJECT_LIST = OBJECT_POOL;
    p260 = OBJECT_POOL;
    s250 = 0;
    CAT2_OBJECT_COUNT = 0;
}
