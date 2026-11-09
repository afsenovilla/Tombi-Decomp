// FUNC 8005d384 68 MAIN0
// MATCHING 8005d384 68
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl PATCH_OBJ_14\n.type PATCH_OBJ_14,@function\nPATCH_OBJ_14:\n"
"	lw	$2,4212($3)\n"
"	sll	$0,$0,0x0\n"
"	andi	$2,$2,0x80\n"
"	beq	$2,$0,LPATCH_OBJ_14_3c\n"
"	sll	$0,$0,0x0\n"
"LPATCH_OBJ_14_14:\n"
"	lw	$2,4164($3)\n"
"	sll	$0,$0,0x0\n"
"	andi	$2,$2,0x80\n"
"	bne	$2,$0,LPATCH_OBJ_14_14\n"
"	sll	$0,$0,0x0\n"
"	lui	$2,%hi(D_8009BC4C)\n"
"	lw	$2,%lo(D_8009BC4C)($2)\n"
"	sll	$0,$0,0x0\n"
"	jr	$2\n"
"	sll	$0,$0,0x0\n"
"LPATCH_OBJ_14_3c:\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
