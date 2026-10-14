/* SCRIPT_CTX (0x8009F0F0 -> the running script context, e.g. D_8009F840) as seen from sources that include tobj.h
   (plain C types). Same layout as `ScriptContext` in game.h (state/cmpFlag/pc/sp/stack/vars); the opcode arguments and
   the result live at +0x1190.. as ints (`arg[0]` = result), which game.h's ScriptContext shows as short result / unk1192. */
#ifndef _TOMBA_SCRIPTCTX_H
#define _TOMBA_SCRIPTCTX_H
typedef struct ScriptCtx {
    short words[0x40];          /* 0x0000 */
    void *p80;                  /* 0x0080 */
    unsigned char *p84;        /* 0x0084 */
    unsigned char state;        /* 0x0088 */
    unsigned char cmpFlag;      /* 0x0089 */
    unsigned short pc;          /* 0x008A */
    unsigned short sp;          /* 0x008C */
    unsigned short pad8e;       /* 0x008E */
    int stack[0x400];       /* 0x0090 */
    int vars[0x40];             /* 0x1090 */
    int arg[8];                 /* 0x1190 */
} ScriptCtx;
extern ScriptCtx *SCRIPT_CTX;
#endif
