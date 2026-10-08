// FUNC 8010afc4 140 X005
// MATCHING 8010afc4 140
typedef struct D { char pad0[8]; char a; char pad1[0x20-9]; short b; char pad2[0x2c-0x22]; short c; short d; } D;
typedef struct O { char pad0[6]; unsigned char n; char pad1[0x24-7]; void *fn; char pad2[0x7c-0x28]; short s7c; short s7e; char pad3[0x8c-0x80]; int v8c; char pad4[0x9c-0x90]; char c9c; char pad5[0xa5-0x9d]; char ca5; char pad6[0xac-0xa6]; char cac; char pad7[0xb0-0xad]; short sb0; short sb2; } O;
extern D *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern void FUN_8001fe6c(void);
extern void FUN_80010748(void);
void FUN_8010afc4(O *o)
{
DAT_8009c330->a = 0;
o->ca5 = 0;
o->c9c = 0;
o->cac = 0;
o->sb2 = 0;
o->s7c = 0;
o->s7e = 0;
DAT_8009c330->b = 0;
DAT_8009c330->c = 0;
DAT_8009c330->d = 0;
o->fn = FUN_80010748;
FUN_8001fe6c();
o->v8c = DAT_801152e8[o->sb0];
o->n++;
}
