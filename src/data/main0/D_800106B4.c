// DATA 800106b4 2140 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80011790[];
extern char D_800117D4[];
extern char D_80011818[];
extern char D_8001185C[];
extern char D_800118A0[];
extern char D_800118E4[];
extern char D_80011928[];
extern char D_8001196C[];
extern char D_800119B0[];
extern char D_800119F4[];
extern char D_80011A38[];
extern char D_80011A7C[];
extern char D_80011AC0[];
extern char D_80011B04[];
extern char D_80011B48[];
extern char D_80011B8C[];
extern char D_80011B9C[];
extern char D_80011BAC[];
extern char D_80011BBC[];
extern char D_80011BDC[];
extern char D_80011BFC[];
extern char D_80011C1C[];
extern char D_80011C3C[];
extern char D_80011C44[];
extern char D_80011C4C[];
extern char D_80011C54[];
extern char D_80011C6C[];
extern char D_80011C84[];
extern char D_80011C9C[];
extern char D_80011CBC[];
extern char D_80011CDC[];
extern char D_80011CFC[];
extern char D_80011D0C[];
extern char D_80011D1C[];
extern char D_80011D2C[];
extern char D_80011D54[];
extern char D_80011D7C[];
extern void *ptrs_800106B4[];
extern AnimWord anim_80010748[];
extern AnimWord anim_80010768[];
extern AnimWord anim_8001079C[];
extern AnimWord anim_800107A4[];
extern AnimWord anim_800107D4[];
extern AnimWord anim_80010814[];
extern AnimWord anim_80010888[];
extern AnimWord anim_8001090C[];
extern AnimWord anim_80010990[];
extern AnimWord anim_800109A0[];
extern AnimWord anim_800109B0[];
extern AnimWord anim_80010A04[];
extern AnimWord anim_80010A2C[];
extern AnimWord anim_80010A54[];
extern AnimWord anim_80010A9C[];
extern AnimWord anim_80010AD0[];
extern AnimWord anim_80010AE8[];
extern AnimWord anim_80010B10[];
extern AnimWord anim_80010B20[];
extern AnimWord anim_80010B78[];
extern AnimWord anim_80010B80[];
extern AnimWord anim_80010BE0[];
extern AnimWord anim_80010BE8[];
extern AnimWord anim_80010C0C[];
extern AnimWord anim_80010C50[];
extern AnimWord anim_80010CA0[];
extern AnimWord anim_80010CE0[];
extern AnimWord anim_80010CE8[];
extern AnimWord anim_80010D1C[];
extern AnimWord anim_80010D50[];
extern AnimWord anim_80010D68[];
extern AnimWord anim_80010D78[];
extern AnimWord anim_80010DAC[];
extern AnimWord anim_80010DE0[];
extern AnimWord anim_80010DE8[];
extern AnimWord anim_80010DF0[];
extern AnimWord anim_80010DF8[];
extern AnimWord anim_80010E2C[];
extern AnimWord anim_80010E34[];
extern AnimWord anim_80010E5C[];
extern AnimWord anim_80010E80[];
extern AnimWord anim_80010EA0[];
extern AnimWord anim_80010EC0[];
extern AnimWord anim_80010EE0[];
extern AnimWord anim_80010EE8[];
extern AnimWord anim_80010EF0[];
extern AnimWord anim_80010EF8[];
extern AnimWord anim_80010F00[];
extern AnimWord anim_80010F08[];

void *ptrs_800106B4[37] = {
    D_80011A38,
    D_80011A7C,
    D_80011AC0,
    D_80011B04,
    D_80011B48,
    D_80011790,
    D_800117D4,
    D_80011818,
    D_8001185C,
    D_800118A0,
    D_800118E4,
    D_80011928,
    D_8001196C,
    D_800119B0,
    D_800119F4,
    D_80011B8C,
    D_80011B9C,
    D_80011BAC,
    D_80011BBC,
    D_80011BDC,
    D_80011BFC,
    D_80011C1C,
    D_80011C3C,
    D_80011C44,
    D_80011C4C,
    D_80011C54,
    D_80011C6C,
    D_80011C84,
    D_80011C9C,
    D_80011CBC,
    D_80011CDC,
    D_80011CFC,
    D_80011D0C,
    D_80011D1C,
    D_80011D2C,
    D_80011D54,
    D_80011D7C,
};

AnimWord anim_80010748[8] = {
    ANIM(0, 0, 0, 360),
    ANIM(269, 0, 0, 6),
    ANIM(270, 0, 0, 5),
    ANIM(271, 0, 0, 24),
};

AnimWord anim_80010768[13] = {
    ANIM(272, 0, 0, 48),
    ANIM(273, 0, 0, 6),
    ANIM(274, 0, 0, 6),
    ANIM(275, 0, 0, 64),
    ANIM(274, 0, 0, 6),
    ANIM_JUMP(273, 0, 0, 6, anim_80010768),
};

AnimWord anim_8001079C[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_800107A4[12] = {
    ANIM_STOP(2, 0, 0, 60),
    ANIM_STOP(3, 0, 0, 60),
    ANIM_STOP(4, 0, 0, 60),
    ANIM_STOP(5, 0, 0, 60),
    ANIM_STOP(6, 0, 0, 60),
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_800107D4[16] = {
    ANIM(2, 0, 0, 1),
    ANIM(3, 0, 0, 1),
    ANIM(4, 0, 0, 1),
    ANIM(5, 0, 0, 1),
    ANIM(6, 0, 0, 1),
    ANIM(7, 0, 0, 1),
    ANIM_STOP(1, 0, 0, 1),
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_80010814[29] = {
    ANIM(9, 0, 0, 1),
    ANIM(10, 0, 0, 2),
    ANIM(12, 0, 0, 2),
    ANIM(13, 0, 0, 2),
    ANIM(14, 0, 0, 2),
    ANIM(15, 0, 0, 2),
    ANIM(16, 0, 0, 1),
    ANIM(17, 0, 0, 1),
    ANIM(18, 0, 0, 2),
    ANIM(19, 0, 0, 2),
    ANIM(20, 0, 0, 2),
    ANIM(21, 0, 0, 2),
    ANIM(22, 0, 0, 2),
    ANIM_JUMP(24, 0, 0, 1, anim_80010814),
};

AnimWord anim_80010888[33] = {
    ANIM(9, 0, 0, 1),
    ANIM(10, 0, 0, 2),
    ANIM(11, 0, 0, 2),
    ANIM(12, 0, 0, 2),
    ANIM(13, 0, 0, 2),
    ANIM(14, 0, 0, 2),
    ANIM(15, 0, 0, 2),
    ANIM(16, 0, 0, 1),
    ANIM(17, 0, 0, 1),
    ANIM(18, 0, 0, 1),
    ANIM(19, 0, 0, 2),
    ANIM(20, 0, 0, 2),
    ANIM(21, 0, 0, 2),
    ANIM(22, 0, 0, 2),
    ANIM(23, 0, 0, 1),
    ANIM_JUMP(24, 0, 0, 1, anim_80010888),
};

AnimWord anim_8001090C[33] = {
    ANIM(25, 0, 0, 1),
    ANIM(26, 0, 0, 2),
    ANIM(27, 0, 0, 2),
    ANIM(28, 0, 0, 2),
    ANIM(29, 0, 0, 2),
    ANIM(30, 0, 0, 2),
    ANIM(31, 0, 0, 1),
    ANIM(32, 0, 0, 1),
    ANIM(33, 0, 0, 1),
    ANIM(34, 0, 0, 2),
    ANIM(35, 0, 0, 2),
    ANIM(36, 0, 0, 2),
    ANIM(37, 0, 0, 2),
    ANIM(38, 0, 0, 2),
    ANIM(39, 0, 0, 1),
    ANIM_JUMP(40, 0, 0, 1, anim_8001090C),
};

AnimWord anim_80010990[4] = {
    ANIM(41, 0, 0, 3),
    ANIM_STOP(42, 0, 0, 80),
};

AnimWord anim_800109A0[4] = {
    ANIM(41, 0, 0, 3),
    ANIM_STOP(42, 0, 0, 64),
};

AnimWord anim_800109B0[21] = {
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 6),
    ANIM(45, 0, 0, 4),
    ANIM(44, 0, 0, 2),
    ANIM(46, 0, 1, 4),
    ANIM(44, 0, 0, 2),
    ANIM(45, 0, 0, 4),
    ANIM(44, 0, 0, 2),
    ANIM(46, 0, 1, 4),
    ANIM_JUMP(44, 0, 0, 2, anim_800109B0),
};

AnimWord anim_80010A04[10] = {
    ANIM_STOP(47, 7, 0, 60),
    ANIM(48, 0, 0, 6),
    ANIM(49, 0, 0, 5),
    ANIM(50, 0, 0, 4),
    ANIM_STOP(51, 0, 0, 3),
};

AnimWord anim_80010A2C[10] = {
    ANIM(48, 0, 0, 8),
    ANIM_STOP(49, 0, 0, 4),
    ANIM(50, 0, 0, 6),
    ANIM_STOP(51, 0, 0, 20),
    ANIM_STOP(52, 0, 3, 60),
};

AnimWord anim_80010A54[18] = {
    ANIM(53, 1, 0, 10),
    ANIM(54, 1, 0, 255),
    ANIM(55, 1, 0, 6),
    ANIM(56, 1, 0, 24),
    ANIM(58, 1, 0, 16),
    ANIM(57, 1, 0, 16),
    ANIM(58, 1, 0, 8),
    ANIM(59, 1, 0, 4),
    ANIM_STOP(60, 1, 0, 4),
};

AnimWord anim_80010A9C[13] = {
    ANIM(61, 0, 0, 4),
    ANIM(62, 0, 0, 4),
    ANIM(63, 0, 1, 4),
    ANIM(64, 0, 1, 4),
    ANIM(63, 0, 1, 4),
    ANIM_JUMP(62, 0, 0, 4, anim_80010A9C),
};

AnimWord anim_80010AD0[6] = {
    ANIM(65, 0, 0, 4),
    ANIM(66, 0, 0, 4),
    ANIM_STOP(67, 0, 0, 4),
};

AnimWord anim_80010AE8[10] = {
    ANIM(68, 1, 0, 20),
    ANIM(69, 6, 1, 8),
    ANIM(70, 6, 2, 255),
    ANIM_STOP(71, 6, 3, 60),
    ANIM_STOP(68, 1, 0, 60),
};

AnimWord anim_80010B10[4] = {
    ANIM(72, 5, 0, 5),
    ANIM_STOP(73, 5, 0, 5),
};

AnimWord anim_80010B20[22] = {
    ANIM(74, 0, 200, 2),
    ANIM(75, 0, 198, 2),
    ANIM(76, 0, 192, 2),
    ANIM(77, 0, 190, 2),
    ANIM(78, 0, 168, 1),
    ANIM(79, 0, 144, 1),
    ANIM(80, 0, 136, 1),
    ANIM(81, 0, 128, 1),
    ANIM(82, 0, 96, 1),
    ANIM(83, 0, 32, 1),
    ANIM_STOP(84, 0, 0, 1),
};

AnimWord anim_80010B78[2] = {
    ANIM_STOP(97, 7, 0, 2),
};

AnimWord anim_80010B80[24] = {
    ANIM(74, 0, 200, 2),
    ANIM(75, 0, 198, 2),
    ANIM(76, 0, 192, 2),
    ANIM(77, 0, 190, 2),
    ANIM(78, 0, 168, 1),
    ANIM(79, 0, 144, 1),
    ANIM(80, 0, 136, 1),
    ANIM(81, 0, 128, 1),
    ANIM(82, 0, 96, 1),
    ANIM(83, 0, 32, 1),
    ANIM(84, 0, 32, 1),
    ANIM_STOP(85, 0, 208, 1),
};

AnimWord anim_80010BE0[2] = {
    ANIM_STOP(86, 5, 0, 60),
};

AnimWord anim_80010BE8[9] = {
    ANIM(87, 0, 0, 6),
    ANIM(88, 0, 0, 6),
    ANIM(89, 0, 0, 6),
    ANIM_JUMP(88, 0, 0, 6, anim_80010BE8),
};

AnimWord anim_80010C0C[17] = {
    ANIM(92, 0, 0, 5),
    ANIM(91, 0, 0, 5),
    ANIM(90, 0, 0, 5),
    ANIM(91, 0, 0, 5),
    ANIM(92, 0, 0, 5),
    ANIM(93, 0, 0, 5),
    ANIM(94, 0, 0, 5),
    ANIM_JUMP(93, 0, 0, 5, anim_80010C0C),
};

AnimWord anim_80010C50[20] = {
    ANIM(95, 0, -14, 4),
    ANIM(96, 0, -40, 4),
    ANIM(97, 7, 0, 3),
    ANIM(98, 0, 0, 3),
    ANIM_STOP(99, 0, 0, 3),
    ANIM(95, 0, 0, 4),
    ANIM(96, 0, 0, 4),
    ANIM_STOP(97, 0, 0, 4),
    ANIM(98, 0, 0, 4),
    ANIM_STOP(99, 0, 0, 4),
};

AnimWord anim_80010CA0[16] = {
    ANIM(101, 0, 0, 5),
    ANIM(102, 0, 0, 4),
    ANIM(103, 0, 0, 4),
    ANIM(104, 0, 0, 2),
    ANIM_STOP(105, 0, 0, 8),
    ANIM(106, 0, 0, 2),
    ANIM(107, 0, 0, 2),
    ANIM_STOP(108, 0, 0, 4),
};

AnimWord anim_80010CE0[2] = {
    ANIM_STOP(109, 5, 0, 1),
};

AnimWord anim_80010CE8[13] = {
    ANIM(110, 5, 0, 10),
    ANIM(111, 5, 0, 4),
    ANIM(112, 5, 0, 3),
    ANIM(113, 5, 0, 10),
    ANIM(112, 5, 0, 4),
    ANIM_JUMP(111, 5, 0, 3, anim_80010CE8),
};

AnimWord anim_80010D1C[13] = {
    ANIM(110, 5, 0, 4),
    ANIM(111, 5, 0, 4),
    ANIM(112, 5, 0, 2),
    ANIM(113, 5, 0, 4),
    ANIM(112, 5, 0, 2),
    ANIM_JUMP(111, 5, 0, 2, anim_80010D1C),
};

AnimWord anim_80010D50[6] = {
    ANIM(114, 5, 0, 8),
    ANIM_STOP(115, 5, 0, 8),
    ANIM_STOP(116, 0, 0, 60),
};

AnimWord anim_80010D68[4] = {
    ANIM(117, 5, 0, 360),
    ANIM(329, 5, 0, 6),
};

AnimWord anim_80010D78[13] = {
    ANIM(330, 5, 0, 12),
    ANIM(331, 5, 0, 12),
    ANIM(330, 5, 0, 240),
    ANIM(331, 5, 0, 12),
    ANIM(330, 5, 0, 60),
    ANIM_JUMP(331, 5, 0, 12, anim_80010D78),
};

AnimWord anim_80010DAC[13] = {
    ANIM(117, 5, 0, 2),
    ANIM(118, 5, 0, 2),
    ANIM(119, 5, 0, 2),
    ANIM(120, 5, 1, 2),
    ANIM(121, 5, 1, 2),
    ANIM_JUMP(122, 5, 1, 2, anim_80010DAC),
};

AnimWord anim_80010DE0[2] = {
    ANIM_STOP(123, 0, 0, 60),
};

AnimWord anim_80010DE8[2] = {
    ANIM_STOP(124, 0, 0, 60),
};

AnimWord anim_80010DF0[2] = {
    ANIM_STOP(125, 0, 0, 60),
};

AnimWord anim_80010DF8[13] = {
    ANIM(126, 0, 0, 7),
    ANIM(127, 0, 0, 7),
    ANIM(128, 0, 0, 7),
    ANIM(129, 0, 0, 7),
    ANIM(128, 0, 0, 7),
    ANIM_JUMP(127, 0, 0, 7, anim_80010DF8),
};

AnimWord anim_80010E2C[2] = {
    ANIM_STOP(130, 0, 0, 60),
};

AnimWord anim_80010E34[10] = {
    ANIM(131, 0, 0, 8),
    ANIM(132, 0, 0, 255),
    ANIM(133, 0, 0, 3),
    ANIM(134, 0, 0, 3),
    ANIM_STOP(135, 0, 0, 4),
};

AnimWord anim_80010E5C[9] = {
    ANIM(136, 0, 0, 6),
    ANIM(137, 0, 0, 4),
    ANIM(138, 0, 0, 6),
    ANIM_JUMP(137, 0, 0, 4, anim_80010E5C),
};

AnimWord anim_80010E80[8] = {
    ANIM(139, 0, 0, 6),
    ANIM(140, 0, 0, 6),
    ANIM(141, 0, 0, 6),
    ANIM_STOP(140, 0, 0, 6),
};

AnimWord anim_80010EA0[8] = {
    ANIM(142, 0, 0, 6),
    ANIM(143, 0, 0, 6),
    ANIM(144, 0, 0, 6),
    ANIM_STOP(143, 0, 0, 6),
};

AnimWord anim_80010EC0[8] = {
    ANIM(145, 0, 0, 6),
    ANIM(146, 0, 0, 6),
    ANIM(147, 0, 0, 6),
    ANIM_STOP(146, 0, 0, 6),
};

AnimWord anim_80010EE0[2] = {
    ANIM_STOP(148, 0, 0, 60),
};

AnimWord anim_80010EE8[2] = {
    ANIM_STOP(149, 0, 0, 60),
};

AnimWord anim_80010EF0[2] = {
    ANIM_STOP(150, 0, 0, 60),
};

AnimWord anim_80010EF8[2] = {
    ANIM_STOP(151, 0, 0, 60),
};

AnimWord anim_80010F00[2] = {
    ANIM_STOP(152, 0, 0, 60),
};

AnimWord anim_80010F08[2] = {
    ANIM_STOP(153, 0, 0, 60),
};
