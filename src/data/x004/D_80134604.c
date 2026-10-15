// DATA 80134604 1776 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_801352BC[];
extern char D_801352C4[];
extern char D_801352F4[];
extern char D_8013532C[];
extern char D_80135334[];
extern char D_80135364[];
extern char D_8013536C[];
extern char D_8013539C[];
extern char D_801353D4[];
extern char D_801353DC[];
extern void *ptrs_80134604[];
extern void *ptrs_80134608[];
extern void *ptrs_8013460C[];
extern void *ptrs_80134610[];
extern void *ptrs_80134614[];
extern AnimWord anim_80134618[];
extern AnimWord anim_80134630[];
extern AnimWord anim_80134664[];
extern AnimWord anim_801346A8[];
extern AnimWord anim_801346EC[];
extern AnimWord anim_801346F8[];
extern AnimWord anim_80134704[];
extern AnimWord anim_80134738[];
extern AnimWord anim_8013477C[];
extern AnimWord anim_801347C0[];
extern AnimWord anim_801347EC[];
extern AnimWord anim_80134818[];
extern AnimWord anim_80134824[];
extern AnimWord anim_80134848[];
extern AnimWord anim_8013486C[];
extern AnimWord anim_80134888[];
extern AnimWord anim_801348B4[];
extern AnimWord anim_801348F8[];
extern AnimWord anim_8013490C[];
extern AnimWord anim_80134950[];
extern AnimWord anim_801349AC[];
extern AnimWord anim_80134AB0[];
extern AnimWord anim_80134AF4[];
extern AnimWord anim_80134B00[];
extern AnimWord anim_80134B34[];
extern AnimWord anim_80134B68[];
extern void *ptrs_80134B7C[];
extern AnimWord anim_80134B98[];
extern AnimWord anim_80134BBC[];
extern AnimWord anim_80134BE0[];
extern AnimWord anim_80134C00[];
extern AnimWord anim_80134C1C[];
extern AnimWord anim_80134C58[];
extern AnimWord anim_80134C6C[];
extern AnimWord anim_80134C8C[];
extern void *ptrs_80134C94[];
extern AnimWord anim_80134C9C[];
extern AnimWord anim_80134CA4[];
extern void *ptrs_80134CB8[];
extern void *ptrs_80134CE0[];

void *ptrs_80134604[1] = {
    anim_80134AB0,
};

void *ptrs_80134608[1] = {
    anim_80134AF4,
};

void *ptrs_8013460C[1] = {
    anim_80134B00,
};

void *ptrs_80134610[1] = {
    anim_80134B34,
};

void *ptrs_80134614[1] = {
    anim_80134B68,
};

AnimWord anim_80134618[6] = {
    ANIM(24, 0, 0, 5),
    ANIM(25, 0, 0, 5),
    ANIM(26, 0, 0, 8),
};

AnimWord anim_80134630[13] = {
    ANIM(27, 0, 0, 2),
    ANIM(26, 0, 0, 2),
    ANIM(27, 0, 0, 2),
    ANIM(26, 0, 0, 6),
    ANIM(25, 0, 0, 5),
    ANIM_JUMP(24, 0, 0, 5, anim_80134618),
};

AnimWord anim_80134664[17] = {
    ANIM(0, 0, 0, 5),
    ANIM(1, 0, 0, 5),
    ANIM(2, 0, 0, 5),
    ANIM(3, 0, 0, 5),
    ANIM(4, 0, 0, 5),
    ANIM(5, 0, 0, 5),
    ANIM(6, 0, 0, 5),
    ANIM_JUMP(7, 0, 0, 5, anim_80134664),
};

AnimWord anim_801346A8[17] = {
    ANIM(8, 0, 0, 12),
    ANIM(9, 0, 0, 6),
    ANIM(10, 0, 0, 4),
    ANIM(11, 0, 0, 3),
    ANIM(12, 0, 0, 12),
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 4),
    ANIM_JUMP(15, 0, 0, 3, anim_801346A8),
};

AnimWord anim_801346EC[3] = {
    ANIM_JUMP(16, 0, 0, 60, anim_801346EC),
};

AnimWord anim_801346F8[3] = {
    ANIM_JUMP(17, 0, 0, 60, anim_801346F8),
};

AnimWord anim_80134704[13] = {
    ANIM(18, 0, 0, 6),
    ANIM(19, 0, 0, 6),
    ANIM(20, 0, 0, 6),
    ANIM(21, 0, 0, 6),
    ANIM(22, 0, 0, 6),
    ANIM_JUMP(23, 0, 0, 6, anim_80134704),
};

AnimWord anim_80134738[17] = {
    ANIM(28, 0, 0, 5),
    ANIM(29, 0, 0, 5),
    ANIM(30, 0, 0, 5),
    ANIM(31, 0, 0, 5),
    ANIM(32, 0, 0, 5),
    ANIM(33, 0, 0, 5),
    ANIM(34, 0, 0, 5),
    ANIM_JUMP(35, 0, 0, 5, anim_80134738),
};

AnimWord anim_8013477C[17] = {
    ANIM(36, 0, 0, 5),
    ANIM(37, 0, 0, 5),
    ANIM(38, 0, 0, 5),
    ANIM(39, 0, 0, 5),
    ANIM(40, 0, 0, 5),
    ANIM(41, 0, 0, 5),
    ANIM(42, 0, 0, 5),
    ANIM_JUMP(43, 0, 0, 5, anim_8013477C),
};

AnimWord anim_801347C0[11] = {
    ANIM(44, 0, 0, 4),
    ANIM(45, 0, 0, 4),
    ANIM(46, 0, 0, 16),
    ANIM(45, 0, 0, 6),
    ANIM_JUMP(44, 0, 0, 4, anim_801347C0),
};

AnimWord anim_801347EC[11] = {
    ANIM(47, 0, 0, 4),
    ANIM(48, 0, 0, 4),
    ANIM(49, 0, 0, 16),
    ANIM(48, 0, 0, 6),
    ANIM_JUMP(47, 0, 0, 4, anim_801347EC),
};

AnimWord anim_80134818[3] = {
    ANIM_JUMP(66, 0, 0, 60, anim_80134818),
};

AnimWord anim_80134824[9] = {
    ANIM(67, 0, 0, 5),
    ANIM(68, 0, 0, 7),
    ANIM(69, 0, 0, 12),
    ANIM_JUMP(67, 0, 0, 5, anim_80134824),
};

AnimWord anim_80134848[9] = {
    ANIM(70, 0, 0, 20),
    ANIM(71, 0, 0, 8),
    ANIM(72, 0, 0, 12),
    ANIM_JUMP(71, 0, 0, 8, anim_80134848),
};

AnimWord anim_8013486C[7] = {
    ANIM(58, 0, 0, 5),
    ANIM(59, 0, 0, 4),
    ANIM_JUMP(60, 0, 0, 120, anim_8013486C),
};

AnimWord anim_80134888[11] = {
    ANIM(61, 0, 0, 4),
    ANIM(62, 0, 0, 5),
    ANIM(63, 0, 0, 5),
    ANIM(64, 0, 0, 24),
    ANIM_JUMP(65, 0, 0, 120, anim_80134888),
};

AnimWord anim_801348B4[17] = {
    ANIM(50, 0, 0, 5),
    ANIM(51, 0, 0, 5),
    ANIM(52, 0, 0, 5),
    ANIM(53, 0, 0, 5),
    ANIM(54, 0, 0, 5),
    ANIM(55, 0, 0, 5),
    ANIM(56, 0, 0, 5),
    ANIM_JUMP(57, 0, 0, 5, anim_801348B4),
};

AnimWord anim_801348F8[5] = {
    ANIM(73, 0, 0, 8),
    ANIM_JUMP(74, 0, 0, 8, anim_801348F8),
};

AnimWord anim_8013490C[17] = {
    ANIM(83, 0, 0, 8),
    ANIM(84, 0, 0, 8),
    ANIM(85, 0, 0, 8),
    ANIM(86, 0, 0, 8),
    ANIM(87, 0, 0, 8),
    ANIM(88, 0, 0, 8),
    ANIM(89, 0, 0, 8),
    ANIM_JUMP(90, 0, 0, 8, anim_8013490C),
};

AnimWord anim_80134950[23] = {
    ANIM(83, 0, 0, 8),
    ANIM(84, 0, 0, 8),
    ANIM(85, 0, 0, 8),
    ANIM(86, 0, 0, 8),
    ANIM(87, 0, 0, 8),
    ANIM(91, 0, 0, 4),
    ANIM(92, 0, 0, 4),
    ANIM(93, 0, 0, 4),
    ANIM(94, 0, 0, 4),
    ANIM(95, 0, 0, 4),
    ANIM_JUMP(90, 0, 0, 4, anim_80134950),
};

AnimWord anim_801349AC[65] = {
    ANIM(83, 0, 0, 26),
    ANIM(84, 0, 0, 26),
    ANIM(85, 0, 0, 3),
    ANIM(96, 0, 0, 3),
    ANIM(97, 0, 0, 3),
    ANIM(98, 0, 0, 3),
    ANIM(99, 0, 0, 3),
    ANIM(100, 0, 0, 3),
    ANIM(85, 0, 0, 3),
    ANIM(96, 0, 0, 3),
    ANIM(97, 0, 0, 3),
    ANIM(98, 0, 0, 3),
    ANIM(99, 0, 0, 3),
    ANIM(100, 0, 0, 3),
    ANIM(85, 0, 0, 3),
    ANIM(86, 0, 0, 26),
    ANIM(87, 0, 0, 26),
    ANIM(88, 0, 0, 26),
    ANIM(89, 0, 0, 3),
    ANIM(101, 0, 0, 3),
    ANIM(102, 0, 0, 3),
    ANIM(93, 0, 0, 3),
    ANIM(94, 0, 0, 3),
    ANIM(103, 0, 0, 3),
    ANIM(89, 0, 0, 3),
    ANIM(101, 0, 0, 3),
    ANIM(102, 0, 0, 3),
    ANIM(93, 0, 0, 3),
    ANIM(94, 0, 0, 3),
    ANIM(103, 0, 0, 3),
    ANIM(89, 0, 0, 3),
    ANIM_JUMP(90, 0, 0, 26, anim_801349AC),
};

AnimWord anim_80134AB0[17] = {
    ANIM(75, 0, 0, 5),
    ANIM(76, 0, 0, 5),
    ANIM(77, 0, 0, 5),
    ANIM(78, 0, 0, 5),
    ANIM(79, 0, 0, 5),
    ANIM(80, 0, 0, 5),
    ANIM(81, 0, 0, 5),
    ANIM_JUMP(82, 0, 0, 5, anim_80134AB0),
};

AnimWord anim_80134AF4[3] = {
    ANIM_JUMP(76, 0, 0, 60, anim_80134AF4),
};

AnimWord anim_80134B00[13] = {
    ANIM(109, 0, 0, 5),
    ANIM(110, 0, 0, 3),
    ANIM(105, 0, 0, 3),
    ANIM(106, 0, 0, 3),
    ANIM(107, 0, 0, 5),
    ANIM_JUMP(108, 0, 0, 5, anim_80134B00),
};

AnimWord anim_80134B34[13] = {
    ANIM(109, 0, 0, 5),
    ANIM(110, 0, 0, 5),
    ANIM(111, 0, 0, 5),
    ANIM(112, 0, 0, 5),
    ANIM(113, 0, 0, 5),
    ANIM_JUMP(114, 0, 0, 5, anim_80134B34),
};

AnimWord anim_80134B68[5] = {
    ANIM(115, 0, 0, 8),
    ANIM_JUMP(116, 0, 0, 8, anim_80134B68),
};

void *ptrs_80134B7C[7] = {
    anim_80134C8C,
    anim_80134C58,
    anim_80134B98,
    anim_80134BBC,
    anim_80134BE0,
    anim_80134C1C,
    anim_80134C6C,
};

AnimWord anim_80134B98[9] = {
    ANIM(0, 0, 0, 6),
    ANIM(1, 0, 0, 4),
    ANIM(2, 0, 0, 6),
    ANIM_JUMP(1, 0, 0, 4, anim_80134B98),
};

AnimWord anim_80134BBC[9] = {
    ANIM(3, 0, 0, 7),
    ANIM(4, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM_JUMP(4, 0, 0, 7, anim_80134BBC),
};

AnimWord anim_80134BE0[8] = {
    ANIM(6, 0, 0, 12),
    ANIM(7, 0, 0, 5),
    ANIM(8, 0, 0, 3),
    ANIM(9, 0, 0, 3),
};

AnimWord anim_80134C00[7] = {
    ANIM(10, 0, 0, 3),
    ANIM(11, 0, 0, 3),
    ANIM_JUMP(12, 0, 0, 8, anim_80134BE0),
};

AnimWord anim_80134C1C[15] = {
    ANIM(13, 0, 0, 12),
    ANIM(14, 0, 0, 5),
    ANIM(15, 0, 0, 3),
    ANIM(16, 0, 0, 3),
    ANIM(17, 0, 0, 3),
    ANIM(18, 0, 0, 3),
    ANIM_JUMP(19, 0, 0, 8, anim_80134C1C),
};

AnimWord anim_80134C58[5] = {
    ANIM(20, 0, 0, 16),
    ANIM_JUMP(21, 0, 0, 16, anim_80134C58),
};

AnimWord anim_80134C6C[8] = {
    ANIM(22, 0, 0, 16),
    ANIM(23, 0, 0, 4),
    ANIM(24, 0, 0, 6),
    ANIM_STOP(23, 0, 0, 4),
};

AnimWord anim_80134C8C[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

void *ptrs_80134C94[2] = {
    anim_80134C9C,
    anim_80134CA4,
};

AnimWord anim_80134C9C[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80134CA4[5] = {
    ANIM(1, 0, 0, 16),
    ANIM_JUMP(2, 0, 0, 16, anim_80134CA4),
};

void *ptrs_80134CB8[10] = {
    D_801352BC,
    D_801352C4,
    D_801352F4,
    D_8013532C,
    D_80135334,
    D_80135364,
    D_8013536C,
    D_8013539C,
    D_801353D4,
    D_801353DC,
};

void *ptrs_80134CE0[5] = {
    D_801352BC,
    D_801352C4,
    D_801352F4,
    D_8013532C,
    D_80135334,
};
