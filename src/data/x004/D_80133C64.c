// DATA 80133c64 2392 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80134618[];
extern char D_80134664[];
extern void *ptrs_80133C64[];
extern AnimWord anim_80133C74[];
extern AnimWord anim_80133CB8[];
extern AnimWord anim_80133CFC[];
extern AnimWord anim_80133D40[];
extern AnimWord anim_80133D50[];
extern AnimWord anim_80133D58[];
extern AnimWord anim_80133D60[];
extern AnimWord anim_80133D68[];
extern AnimWord anim_80133D8C[];
extern AnimWord anim_80133DB0[];
extern AnimWord anim_80133DB8[];
extern AnimWord anim_80133E04[];
extern AnimWord anim_80133E28[];
extern AnimWord anim_80133E30[];
extern AnimWord anim_80133E74[];
extern AnimWord anim_80133E84[];
extern AnimWord anim_80133E98[];
extern AnimWord anim_80133EA0[];
extern unsigned int data_80133EBC[];
extern unsigned int data_80133F3C[];
extern unsigned int data_80133FCC[];
extern unsigned int data_8013405C[];
extern unsigned int data_80134080[];
extern unsigned int data_80134084[];
extern unsigned int data_801340AC[];
extern unsigned int data_801340EC[];
extern AnimWord anim_8013413C[];
extern AnimWord anim_801341A0[];
extern AnimWord anim_801341A8[];
extern AnimWord anim_801341BC[];
extern AnimWord anim_801341D0[];
extern AnimWord anim_801341E0[];
extern AnimWord anim_80134200[];
extern AnimWord anim_80134208[];
extern AnimWord anim_80134210[];
extern AnimWord anim_80134218[];
extern AnimWord anim_80134220[];
extern AnimWord anim_80134254[];
extern AnimWord anim_80134298[];
extern AnimWord anim_801342CC[];
extern AnimWord anim_801342F0[];
extern AnimWord anim_80134304[];
extern AnimWord anim_80134328[];
extern AnimWord anim_8013434C[];
extern AnimWord anim_80134370[];
extern AnimWord anim_80134398[];
extern AnimWord anim_801343C0[];
extern AnimWord anim_801343E4[];
extern AnimWord anim_801343F4[];
extern AnimWord anim_801343FC[];
extern AnimWord anim_80134404[];
extern AnimWord anim_80134414[];
extern AnimWord anim_80134438[];
extern AnimWord anim_8013445C[];
extern AnimWord anim_80134480[];
extern AnimWord anim_801344A0[];
extern AnimWord anim_801344A8[];
extern AnimWord anim_801344C0[];
extern AnimWord anim_801344D0[];
extern AnimWord anim_801344F8[];
extern AnimWord anim_80134508[];
extern AnimWord anim_80134520[];
extern AnimWord anim_80134530[];
extern AnimWord anim_80134538[];
extern AnimWord anim_80134540[];
extern AnimWord anim_80134548[];
extern AnimWord anim_8013456C[];
extern AnimWord anim_80134574[];
extern AnimWord anim_8013457C[];
extern void *ptrs_801345B4[];
extern void *ptrs_801345B8[];

void *ptrs_80133C64[4] = {
    anim_80134480,
    anim_801344A0,
    anim_8013457C,
    0,
};

AnimWord anim_80133C74[17] = {
    ANIM(2, 0, 0, 5),
    ANIM(3, 0, 0, 5),
    ANIM(4, 0, 0, 5),
    ANIM(5, 0, 0, 5),
    ANIM(6, 0, 0, 5),
    ANIM(7, 0, 0, 5),
    ANIM(8, 0, 0, 5),
    ANIM_JUMP(9, 0, 0, 5, anim_80133C74),
};

AnimWord anim_80133CB8[17] = {
    ANIM(10, 1, 0, 2),
    ANIM(11, 1, 0, 2),
    ANIM(12, 1, 0, 2),
    ANIM(13, 1, 0, 2),
    ANIM(14, 1, 0, 2),
    ANIM(15, 1, 0, 2),
    ANIM(16, 1, 0, 2),
    ANIM_JUMP(17, 1, 0, 2, anim_80133CB8),
};

AnimWord anim_80133CFC[17] = {
    ANIM(18, 11, 0, 3),
    ANIM(19, 11, 0, 3),
    ANIM(20, 11, 0, 3),
    ANIM(21, 11, 0, 3),
    ANIM(22, 11, 0, 3),
    ANIM(23, 11, 0, 3),
    ANIM(24, 11, 0, 3),
    ANIM_JUMP(25, 11, 0, 3, anim_80133CFC),
};

AnimWord anim_80133D40[4] = {
    ANIM(26, 12, 0, 10),
    ANIM_STOP(27, 12, 0, 60),
};

AnimWord anim_80133D50[2] = {
    ANIM_STOP(28, 12, 0, 60),
};

AnimWord anim_80133D58[2] = {
    ANIM_STOP(0, 0, 0, 5),
};

AnimWord anim_80133D60[2] = {
    ANIM_STOP(1, 0, 0, 5),
};

AnimWord anim_80133D68[9] = {
    ANIM(35, 1, 0, 20),
    ANIM(36, 1, 0, 4),
    ANIM(37, 2, 0, 20),
    ANIM_JUMP(36, 1, 0, 4, anim_80133D68),
};

AnimWord anim_80133D8C[9] = {
    ANIM(32, 3, 0, 10),
    ANIM(33, 3, 0, 4),
    ANIM(34, 2, 0, 10),
    ANIM_JUMP(33, 3, 0, 4, anim_80133D8C),
};

AnimWord anim_80133DB0[2] = {
    ANIM(37, 2, 0, 5),
};

AnimWord anim_80133DB8[19] = {
    ANIM(39, 5, 0, 15),
    ANIM(38, 4, 0, 6),
    ANIM(39, 5, 0, 6),
    ANIM(40, 4, 0, 6),
    ANIM(39, 5, 0, 6),
    ANIM(38, 4, 0, 6),
    ANIM(39, 5, 0, 6),
    ANIM(40, 4, 0, 6),
    ANIM_JUMP(39, 5, 0, 6, anim_80133DB8),
};

AnimWord anim_80133E04[9] = {
    ANIM(42, 10, 0, 4),
    ANIM(43, 10, 0, 4),
    ANIM(44, 10, 0, 4),
    ANIM_JUMP(43, 10, 0, 4, anim_80133E04),
};

AnimWord anim_80133E28[2] = {
    ANIM_STOP(45, 10, 0, 1),
};

AnimWord anim_80133E30[17] = {
    ANIM(53, 28, 0, 6),
    ANIM(54, 28, 0, 6),
    ANIM(55, 28, 0, 6),
    ANIM(56, 28, 0, 6),
    ANIM(57, 28, 0, 6),
    ANIM(56, 28, 0, 6),
    ANIM(55, 28, 0, 6),
    ANIM_JUMP(54, 28, 0, 6, anim_80133E30),
};

AnimWord anim_80133E74[4] = {
    ANIM(46, 29, 0, 8),
    ANIM_STOP(47, 29, 0, 60),
};

AnimWord anim_80133E84[5] = {
    ANIM(48, 37, 0, 32),
    ANIM_JUMP(49, 37, 0, 8, anim_80133E84),
};

AnimWord anim_80133E98[2] = {
    ANIM(50, 37, 0, 20),
};

AnimWord anim_80133EA0[7] = {
    ANIM(51, 37, 0, 20),
    ANIM(52, 37, 0, 15),
    ANIM_JUMP(51, 37, 0, 10, anim_80133EA0),
};

unsigned int data_80133EBC[32] = {
    0x2F003A, 0x60000, 0x2F003B, 0x6110E,
    0x2F003A, 0x60000, 0x2F003B, 0x6110D,
    0x2F003A, 0xA0000, 0x30003E, 0xA0000,
    0x30003F, 0x6110E, 0x30003E, 0x60000,
    0x30003F, 0x6110D, 0x30003E, 0xA0000,
    0x31003C, 0xA0000, 0x31003D, 0x6110E,
    0x31003C, 0x60000, 0x31003D, 0x6110D,
    0x31003C, 0xA0000, 0x30003E, 0x800A0000,
};

unsigned int data_80133F3C[36] = {
    0x2F003A, 0xA0000, 0x30003E, 0xA0000,
    0x30003F, 0x6110E, 0x30003E, 0x60000,
    0x30003F, 0x6110D, 0x30003E, 0xA0000,
    0x2F003A, 0xA0000, 0x2F003B, 0x6110E,
    0x2F003A, 0x60000, 0x2F003B, 0x6110D,
    0x2F003A, 0xA0000, 0x30003E, 0xA0000,
    0x31003C, 0xA0000, 0x31003D, 0x6110E,
    0x31003C, 0x60000, 0x31003D, 0x6110D,
    0x31003C, 0xA0000, 0x30003E, 0x800A0000,
};

unsigned int data_80133FCC[36] = {
    0x2F003A, 0xA0000, 0x30003E, 0xA0000,
    0x31003C, 0xA0000, 0x31003D, 0x6110E,
    0x31003C, 0x60000, 0x31003D, 0x6110D,
    0x31003C, 0xA0000, 0x30003E, 0xA0000,
    0x2F003A, 0xA0000, 0x2F003B, 0x6110E,
    0x2F003A, 0x60000, 0x2F003B, 0x6110D,
    0x2F003A, 0xA0000, 0x30003E, 0xA0000,
    0x30003F, 0x6110E, 0x30003E, 0x60000,
    0x30003F, 0x6110D, 0x30003E, 0x800A0000,
};

unsigned int data_8013405C[9] = {
    0x320042, 0xA0000, 0x320043, 0x6110E,
    0x320042, 0x60000, 0x320043, 0x6110D,
    0x320042,
};

unsigned int data_80134080[1] = {
    0x800A0000,
};

unsigned int data_80134084[10] = {
    0x330040, 0x60000, 0x330041, 0x6110E,
    0x330040, 0x60000, 0x330041, 0x6110D,
    0x330040, 0x80060000,
};

unsigned int data_801340AC[16] = {
    0x30003E, 0x300000, 0x340044, 0xA0000,
    0x330040, 0xA0000, 0x330041, 0xA110E,
    0x330040, 0x60000, 0x330041, 0xA110D,
    0x330040, 0xA0000, 0x340044, 0x800A0000,
};

unsigned int data_801340EC[20] = {
    0x2F003A, 0xC0000, 0x2F003B, 0x6110E,
    0x2F003A, 0x60000, 0x2F003B, 0x6110D,
    0x2F003A, 0xC0000, 0x340044, 0xC0000,
    0x340045, 0x6110E, 0x340044, 0x60000,
    0x340045, 0x6110D, 0x340044, 0x800C0000,
};

AnimWord anim_8013413C[25] = {
    ANIM(70, 53, 512, 8),
    ANIM(71, 53, 512, 8),
    ANIM(72, 53, 512, 8),
    ANIM(73, 53, 512, 8),
    ANIM(74, 53, 516, 8),
    ANIM(75, 53, 516, 8),
    ANIM(76, 53, 516, 8),
    ANIM(77, 53, 512, 8),
    ANIM(78, 53, 512, 8),
    ANIM(79, 53, 516, 8),
    ANIM(80, 53, 516, 8),
    ANIM_JUMP(81, 53, 516, 8, anim_8013413C),
};

AnimWord anim_801341A0[2] = {
    ANIM_STOP(82, 26, 0, 5),
};

AnimWord anim_801341A8[5] = {
    ANIM(83, 38, 0, 12),
    ANIM_JUMP(84, 38, 0, 8, anim_801341A8),
};

AnimWord anim_801341BC[5] = {
    ANIM(85, 38, 0, 12),
    ANIM_JUMP(86, 38, 0, 8, anim_801341BC),
};

AnimWord anim_801341D0[4] = {
    ANIM(87, 42, 0, 6),
    ANIM_STOP(88, 42, 0, 60),
};

AnimWord anim_801341E0[8] = {
    ANIM(89, 6, 0, 15),
    ANIM(90, 6, 0, 15),
    ANIM(91, 6, 0, 15),
    ANIM_STOP(92, 6, 0, 1),
};

AnimWord anim_80134200[2] = {
    ANIM_STOP(93, 23, 0, 60),
};

AnimWord anim_80134208[2] = {
    ANIM_STOP(94, 24, 0, 60),
};

AnimWord anim_80134210[2] = {
    ANIM_STOP(95, 24, 0, 8),
};

AnimWord anim_80134218[2] = {
    ANIM_STOP(96, 24, 0, 8),
};

AnimWord anim_80134220[13] = {
    ANIM(97, 35, 0, 45),
    ANIM(98, 35, 0, 8),
    ANIM(97, 35, 0, 20),
    ANIM(98, 35, 0, 8),
    ANIM(97, 35, 0, 6),
    ANIM_JUMP(98, 35, 0, 8, anim_80134220),
};

AnimWord anim_80134254[17] = {
    ANIM(99, 35, 0, 48),
    ANIM(100, 35, 0, 8),
    ANIM(101, 35, 0, 8),
    ANIM(100, 35, 0, 8),
    ANIM(99, 35, 0, 16),
    ANIM(100, 35, 0, 8),
    ANIM(101, 35, 0, 8),
    ANIM_JUMP(100, 35, 0, 8, anim_80134254),
};

AnimWord anim_80134298[13] = {
    ANIM(102, 35, 0, 15),
    ANIM(103, 35, 0, 5),
    ANIM(104, 35, 0, 5),
    ANIM(105, 35, 0, 15),
    ANIM(104, 35, 0, 5),
    ANIM_JUMP(103, 35, 0, 5, anim_80134298),
};

AnimWord anim_801342CC[9] = {
    ANIM(106, 36, 0, 14),
    ANIM(107, 36, 0, 4),
    ANIM(108, 36, 0, 14),
    ANIM_JUMP(107, 36, 0, 4, anim_801342CC),
};

AnimWord anim_801342F0[5] = {
    ANIM(109, 36, 0, 12),
    ANIM_JUMP(112, 36, 0, 60, anim_80134304),
};

AnimWord anim_80134304[9] = {
    ANIM(112, 36, 0, 5),
    ANIM(111, 36, 0, 5),
    ANIM(110, 36, 0, 5),
    ANIM_JUMP(111, 36, 0, 5, anim_80134304),
};

AnimWord anim_80134328[9] = {
    ANIM(113, 40, 0, 32),
    ANIM(114, 40, 0, 10),
    ANIM(115, 40, 0, 56),
    ANIM_JUMP(114, 40, 0, 10, anim_80134328),
};

AnimWord anim_8013434C[9] = {
    ANIM(116, 41, 0, 20),
    ANIM(117, 41, 0, 15),
    ANIM(118, 41, 0, 15),
    ANIM_JUMP(117, 41, 0, 10, anim_8013434C),
};

AnimWord anim_80134370[10] = {
    ANIM(119, 0, 0, 6),
    ANIM(120, 0, 0, 8),
    ANIM(121, 0, 0, 5),
    ANIM(122, 0, 0, 5),
    ANIM_STOP(123, 0, 0, 5),
};

AnimWord anim_80134398[10] = {
    ANIM(124, 6, 0, 8),
    ANIM(125, 6, 0, 8),
    ANIM(126, 7, 0, 8),
    ANIM(127, 8, 0, 8),
    ANIM(128, 8, 0, 8),
};

AnimWord anim_801343C0[9] = {
    ANIM(129, 9, 0, 8),
    ANIM(130, 9, 0, 8),
    ANIM(129, 9, 0, 8),
    ANIM_JUMP(131, 9, 0, 8, anim_801343C0),
};

AnimWord anim_801343E4[4] = {
    ANIM(132, 20, 0, 12),
    ANIM_STOP(133, 21, 0, 60),
};

AnimWord anim_801343F4[2] = {
    ANIM_STOP(134, 22, 0, 60),
};

AnimWord anim_801343FC[2] = {
    ANIM(135, 30, 0, 6),
};

AnimWord anim_80134404[4] = {
    ANIM(136, 30, 0, 6),
    ANIM_STOP(137, 31, 0, 30),
};

AnimWord anim_80134414[9] = {
    ANIM(138, 32, 0, 12),
    ANIM(139, 33, 0, 12),
    ANIM(140, 33, 0, 12),
    ANIM_JUMP(139, 33, 0, 12, anim_80134414),
};

AnimWord anim_80134438[9] = {
    ANIM(143, 34, 0, 3),
    ANIM(144, 34, 0, 6),
    ANIM(143, 34, 0, 3),
    ANIM_JUMP(142, 34, 0, 6, anim_80134438),
};

AnimWord anim_8013445C[9] = {
    ANIM(146, 42, 0, 10),
    ANIM(145, 42, 0, 4),
    ANIM(147, 42, 0, 10),
    ANIM_JUMP(145, 42, 0, 4, anim_8013445C),
};

AnimWord anim_80134480[8] = {
    ANIM(124, 6, 0, 6),
    ANIM(125, 6, 0, 6),
    ANIM(126, 7, 0, 6),
    ANIM_STOP(127, 8, 0, 15),
};

AnimWord anim_801344A0[2] = {
    ANIM_STOP(148, 46, 0, 5),
};

AnimWord anim_801344A8[6] = {
    ANIM(156, 13, 0, 8),
    ANIM(157, 13, 0, 8),
    ANIM_STOP(158, 13, 0, 8),
};

AnimWord anim_801344C0[4] = {
    ANIM(159, 14, 0, 8),
    ANIM_STOP(160, 15, 0, 20),
};

AnimWord anim_801344D0[10] = {
    ANIM(160, 15, 0, 8),
    ANIM(161, 15, 0, 8),
    ANIM(162, 15, 0, 8),
    ANIM(161, 15, 0, 8),
    ANIM_STOP(163, 15, 0, 60),
};

AnimWord anim_801344F8[4] = {
    ANIM(164, 15, 0, 8),
    ANIM_STOP(165, 15, 0, 10),
};

AnimWord anim_80134508[6] = {
    ANIM(166, 16, 0, 12),
    ANIM(167, 16, 0, 8),
    ANIM_STOP(168, 16, 0, 60),
};

AnimWord anim_80134520[4] = {
    ANIM(169, 17, 0, 8),
    ANIM_STOP(170, 17, 0, 12),
};

AnimWord anim_80134530[2] = {
    ANIM_STOP(171, 18, 0, 3),
};

AnimWord anim_80134538[2] = {
    ANIM_STOP(172, 18, 0, 20),
};

AnimWord anim_80134540[2] = {
    ANIM_STOP(159, 19, 0, 20),
};

AnimWord anim_80134548[9] = {
    ANIM(177, 27, 0, 15),
    ANIM(178, 27, 0, 8),
    ANIM(179, 27, 0, 15),
    ANIM_JUMP(178, 27, 0, 8, anim_80134548),
};

AnimWord anim_8013456C[2] = {
    ANIM_STOP(180, 13, 0, 5),
};

AnimWord anim_80134574[2] = {
    ANIM_STOP(181, 39, 0, 60),
};

AnimWord anim_8013457C[14] = {
    ANIM(149, 0, 0, 4),
    ANIM(150, 0, 0, 4),
    ANIM(151, 0, 0, 4),
    ANIM(152, 0, 0, 4),
    ANIM(153, 0, 0, 4),
    ANIM(154, 0, 0, 4),
    ANIM_STOP(155, 0, 0, 4),
};

void *ptrs_801345B4[1] = {
    D_80134618,
};

void *ptrs_801345B8[1] = {
    D_80134664,
};
