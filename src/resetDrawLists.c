// FUNC 80017678 288 MAIN0
// MATCHING 80017678 288
extern char CAT7_OBJECT_LIST_BUF, OBJECT_POOL, CAT4_OBJECT_LIST_BUF, CAT5_OBJECT_LIST_BUF, ONSCREEN_OBJECT_LIST_BUF, CAT3_OBJECT_LIST_BUF, CAT8_OBJECT_LIST_BUF;

void resetDrawLists(void)
{
    *(void **)0x1f800218 = &ONSCREEN_OBJECT_LIST_BUF;
    *(void **)0x1f80025c = &ONSCREEN_OBJECT_LIST_BUF;
    *(void **)0x1f80021c = &OBJECT_POOL;
    *(void **)0x1f800260 = &OBJECT_POOL;
    *(void **)0x1f800220 = &CAT3_OBJECT_LIST_BUF;
    *(void **)0x1f800264 = &CAT3_OBJECT_LIST_BUF;
    *(void **)0x1f800224 = &CAT4_OBJECT_LIST_BUF;
    *(void **)0x1f800268 = &CAT4_OBJECT_LIST_BUF;
    *(void **)0x1f800230 = &CAT8_OBJECT_LIST_BUF;
    *(void **)0x1f800274 = &CAT8_OBJECT_LIST_BUF;
    *(void **)0x1f800228 = &CAT5_OBJECT_LIST_BUF;
    *(void **)0x1f80026c = &CAT5_OBJECT_LIST_BUF;
    *(short *)0x1f80024e = 0;
    *(short *)0x1f800244 = 0;
    *(short *)0x1f800250 = 0;
    *(short *)0x1f800246 = 0;
    *(short *)0x1f800252 = 0;
    *(short *)0x1f80024a = 0;
    *(short *)0x1f800254 = 0;
    *(short *)0x1f80024c = 0;
    *(short *)0x1f800258 = 0;
    *(short *)0x1f800242 = 0;
    *(short *)0x1f800256 = 0;
    *(short *)0x1f800248 = 0;
    *(void **)0x1f80022c = &CAT7_OBJECT_LIST_BUF;
    *(void **)0x1f800270 = &CAT7_OBJECT_LIST_BUF;
    *(short *)0x1f80025a = 0;
    *(short *)0x1f800240 = 0;
}
