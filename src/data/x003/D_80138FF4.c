// DATA 80138ff4 2188 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_801398B8[];
extern char D_801398C0[];
extern char D_801398D8[];
extern char D_801398E0[];
extern char D_801398E8[];
extern char D_80139910[];
extern AnimWord anim_80138FF4[];
extern AnimWord anim_80138FFC[];
extern AnimWord anim_80139010[];
extern AnimWord anim_80139018[];
extern AnimWord anim_80139020[];
extern AnimWord anim_80139028[];
extern AnimWord anim_80139030[];
extern AnimWord anim_80139054[];
extern AnimWord anim_8013905C[];
extern AnimWord anim_80139080[];
extern AnimWord anim_80139088[];
extern AnimWord anim_80139090[];
extern AnimWord anim_80139098[];
extern AnimWord anim_801390A0[];
extern AnimWord anim_801390E0[];
extern AnimWord anim_80139128[];
extern AnimWord anim_80139150[];
extern AnimWord anim_80139158[];
extern AnimWord anim_80139160[];
extern AnimWord anim_80139168[];
extern AnimWord anim_80139178[];
extern AnimWord anim_801391AC[];
extern AnimWord anim_801391E0[];
extern AnimWord anim_801391E8[];
extern AnimWord anim_801391F0[];
extern AnimWord anim_80139200[];
extern AnimWord anim_80139208[];
extern AnimWord anim_80139244[];
extern AnimWord anim_8013924C[];
extern AnimWord anim_80139254[];
extern AnimWord anim_8013925C[];
extern AnimWord anim_80139264[];
extern AnimWord anim_8013926C[];
extern AnimWord anim_80139274[];
extern AnimWord anim_8013927C[];
extern AnimWord anim_80139284[];
extern AnimWord anim_8013928C[];
extern AnimWord anim_80139294[];
extern AnimWord anim_8013929C[];
extern AnimWord anim_801392A4[];
extern AnimWord anim_801392AC[];
extern AnimWord anim_801392B4[];
extern AnimWord anim_801392BC[];
extern AnimWord anim_801392C4[];
extern AnimWord anim_801392CC[];
extern AnimWord anim_801392D4[];
extern AnimWord anim_801392DC[];
extern AnimWord anim_801392E4[];
extern AnimWord anim_801392EC[];
extern AnimWord anim_801392F4[];
extern AnimWord anim_801392FC[];
extern AnimWord anim_80139304[];
extern AnimWord anim_8013930C[];
extern AnimWord anim_80139314[];
extern AnimWord anim_8013931C[];
extern AnimWord anim_80139324[];
extern AnimWord anim_8013932C[];
extern AnimWord anim_80139334[];
extern AnimWord anim_8013933C[];
extern AnimWord anim_80139344[];
extern AnimWord anim_8013934C[];
extern AnimWord anim_80139354[];
extern AnimWord anim_8013935C[];
extern AnimWord anim_80139364[];
extern AnimWord anim_8013936C[];
extern AnimWord anim_80139374[];
extern AnimWord anim_8013937C[];
extern AnimWord anim_801393B4[];
extern AnimWord anim_801393D4[];
extern AnimWord anim_801393DC[];
extern AnimWord anim_801393E4[];
extern AnimWord anim_801393EC[];
extern AnimWord anim_801393F4[];
extern AnimWord anim_801393FC[];
extern AnimWord anim_80139404[];
extern AnimWord anim_8013940C[];
extern AnimWord anim_80139414[];
extern AnimWord anim_8013941C[];
extern AnimWord anim_80139424[];
extern AnimWord anim_8013942C[];
extern AnimWord anim_80139434[];
extern AnimWord anim_80139464[];
extern AnimWord anim_80139478[];
extern AnimWord anim_80139480[];
extern AnimWord anim_80139488[];
extern AnimWord anim_80139490[];
extern AnimWord anim_80139498[];
extern AnimWord anim_801394C8[];
extern AnimWord anim_801394D0[];
extern AnimWord anim_801394E0[];
extern AnimWord anim_801394E8[];
extern AnimWord anim_801394F8[];
extern void *ptrs_80139500[];
extern void *ptrs_8013950C[];
extern void *ptrs_80139510[];
extern void *ptrs_80139514[];
extern void *ptrs_80139520[];
extern void *ptrs_80139524[];
extern void *ptrs_80139528[];
extern void *ptrs_80139530[];
extern void *ptrs_80139538[];
extern void *ptrs_80139570[];
extern void *ptrs_80139574[];
extern void *ptrs_80139578[];
extern AnimWord anim_8013957C[];
extern AnimWord anim_80139584[];
extern AnimWord anim_801395A8[];
extern AnimWord anim_801395B0[];
extern AnimWord anim_801395C0[];
extern AnimWord anim_801395D0[];
extern AnimWord anim_80139604[];
extern AnimWord anim_80139648[];
extern AnimWord anim_80139670[];
extern AnimWord anim_80139678[];
extern AnimWord anim_80139680[];
extern AnimWord anim_801396B4[];
extern AnimWord anim_801396BC[];
extern AnimWord anim_801396C4[];
extern AnimWord anim_801396F8[];
extern AnimWord anim_80139710[];
extern AnimWord anim_80139718[];
extern AnimWord anim_80139720[];
extern AnimWord anim_80139728[];
extern AnimWord anim_80139738[];
extern AnimWord anim_80139740[];
extern AnimWord anim_80139748[];
extern AnimWord anim_80139750[];
extern AnimWord anim_80139758[];
extern AnimWord anim_80139760[];
extern AnimWord anim_801397A0[];
extern AnimWord anim_801397B8[];
extern AnimWord anim_801397DC[];
extern AnimWord anim_80139800[];
extern AnimWord anim_80139824[];
extern AnimWord anim_80139848[];
extern void *ptrs_80139864[];
extern void *ptrs_8013986C[];
extern void *ptrs_80139870[];
extern void *ptrs_80139874[];
extern void *ptrs_80139878[];
extern void *ptrs_8013987C[];

AnimWord anim_80138FF4[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80138FFC[5] = {
    ANIM(1, 0, 0, 16),
    ANIM_JUMP(2, 0, 0, 16, anim_80138FFC),
};

AnimWord anim_80139010[2] = {
    ANIM_STOP(1, 4, 0, 60),
};

AnimWord anim_80139018[2] = {
    ANIM_STOP(2, 5, 0, 60),
};

AnimWord anim_80139020[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80139028[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_80139030[9] = {
    ANIM(7, 0, 0, 2),
    ANIM(8, 0, 0, 2),
    ANIM(9, 0, 0, 2),
    ANIM_JUMP(10, 0, 0, 2, anim_80139030),
};

AnimWord anim_80139054[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8013905C[9] = {
    ANIM(12, 0, 0, 2),
    ANIM(13, 0, 0, 2),
    ANIM(14, 0, 0, 2),
    ANIM_JUMP(15, 0, 0, 2, anim_8013905C),
};

AnimWord anim_80139080[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_80139088[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_80139090[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_80139098[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_801390A0[16] = {
    ANIM(18, 0, 0, 1),
    ANIM(29, 1, 0, 1),
    ANIM(30, 2, 0, 1),
    ANIM(20, 3, 0, 1),
    ANIM(19, 4, 0, 1),
    ANIM(20, 3, 0, 16),
    ANIM(19, 4, 0, 3),
    ANIM_STOP(18, 0, 0, 3),
};

AnimWord anim_801390E0[18] = {
    ANIM(18, 0, 0, 1),
    ANIM(29, 1, 0, 1),
    ANIM(30, 2, 0, 1),
    ANIM(31, 5, 0, 2),
    ANIM(20, 3, 0, 1),
    ANIM(21, 6, 0, 16),
    ANIM(20, 3, 0, 3),
    ANIM(19, 4, 0, 3),
    ANIM_STOP(18, 0, 0, 3),
};

AnimWord anim_80139128[10] = {
    ANIM_STOP(32, 0, 0, 60),
    ANIM(12, 0, 0, 2),
    ANIM(13, 0, 0, 2),
    ANIM(14, 0, 0, 2),
    ANIM_STOP(15, 0, 0, 2),
};

AnimWord anim_80139150[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_80139158[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_80139160[2] = {
    ANIM_STOP(38, 0, 0, 60),
};

AnimWord anim_80139168[4] = {
    ANIM_STOP(16, 0, 0, 60),
    ANIM_STOP(39, 0, 0, 60),
};

AnimWord anim_80139178[13] = {
    ANIM(41, 0, 0, 12),
    ANIM(42, 0, 0, 12),
    ANIM(43, 0, 0, 12),
    ANIM(44, 0, 0, 12),
    ANIM(45, 0, 0, 12),
    ANIM_JUMP(46, 0, 0, 12, anim_80139178),
};

AnimWord anim_801391AC[13] = {
    ANIM(47, 0, 0, 12),
    ANIM(48, 0, 0, 12),
    ANIM(49, 0, 0, 12),
    ANIM(50, 0, 0, 12),
    ANIM(51, 0, 0, 12),
    ANIM_JUMP(52, 0, 0, 12, anim_801391AC),
};

AnimWord anim_801391E0[2] = {
    ANIM_STOP(53, 0, 0, 60),
};

AnimWord anim_801391E8[2] = {
    ANIM_STOP(54, 0, 0, 60),
};

AnimWord anim_801391F0[4] = {
    ANIM_STOP(55, 0, 0, 60),
    ANIM_STOP(56, 0, 0, 60),
};

AnimWord anim_80139200[2] = {
    ANIM_STOP(57, 0, 0, 60),
};

AnimWord anim_80139208[15] = {
    ANIM(58, 0, 0, 6),
    ANIM(59, 0, 0, 6),
    ANIM(60, 0, 0, 6),
    ANIM(61, 0, 0, 6),
    ANIM(62, 0, 0, 6),
    ANIM_JUMP(63, 0, 0, 6, anim_80139208),
    ANIM_STOP(64, 0, 0, 60),
};

AnimWord anim_80139244[2] = {
    ANIM_STOP(66, 0, 0, 60),
};

AnimWord anim_8013924C[2] = {
    ANIM_STOP(64, 0, 0, 60),
};

AnimWord anim_80139254[2] = {
    ANIM_STOP(67, 0, 0, 60),
};

AnimWord anim_8013925C[2] = {
    ANIM_STOP(70, 0, 0, 60),
};

AnimWord anim_80139264[2] = {
    ANIM_STOP(85, 0, 0, 60),
};

AnimWord anim_8013926C[2] = {
    ANIM_STOP(86, 0, 0, 60),
};

AnimWord anim_80139274[2] = {
    ANIM_STOP(87, 0, 0, 60),
};

AnimWord anim_8013927C[2] = {
    ANIM_STOP(88, 0, 0, 60),
};

AnimWord anim_80139284[2] = {
    ANIM_STOP(89, 0, 0, 60),
};

AnimWord anim_8013928C[2] = {
    ANIM_STOP(90, 0, 0, 60),
};

AnimWord anim_80139294[2] = {
    ANIM_STOP(91, 0, 0, 60),
};

AnimWord anim_8013929C[2] = {
    ANIM_STOP(92, 0, 0, 60),
};

AnimWord anim_801392A4[2] = {
    ANIM_STOP(93, 0, 0, 60),
};

AnimWord anim_801392AC[2] = {
    ANIM_STOP(94, 0, 0, 60),
};

AnimWord anim_801392B4[2] = {
    ANIM_STOP(95, 0, 0, 60),
};

AnimWord anim_801392BC[2] = {
    ANIM_STOP(96, 0, 0, 60),
};

AnimWord anim_801392C4[2] = {
    ANIM_STOP(97, 0, 0, 60),
};

AnimWord anim_801392CC[2] = {
    ANIM_STOP(98, 0, 0, 60),
};

AnimWord anim_801392D4[2] = {
    ANIM_STOP(78, 0, 0, 60),
};

AnimWord anim_801392DC[2] = {
    ANIM_STOP(79, 0, 0, 60),
};

AnimWord anim_801392E4[2] = {
    ANIM_STOP(80, 0, 0, 60),
};

AnimWord anim_801392EC[2] = {
    ANIM_STOP(81, 0, 0, 60),
};

AnimWord anim_801392F4[2] = {
    ANIM_STOP(82, 0, 0, 60),
};

AnimWord anim_801392FC[2] = {
    ANIM_STOP(83, 0, 0, 60),
};

AnimWord anim_80139304[2] = {
    ANIM_STOP(84, 0, 0, 60),
};

AnimWord anim_8013930C[2] = {
    ANIM_STOP(71, 0, 0, 60),
};

AnimWord anim_80139314[2] = {
    ANIM_STOP(72, 0, 0, 60),
};

AnimWord anim_8013931C[2] = {
    ANIM_STOP(73, 0, 0, 60),
};

AnimWord anim_80139324[2] = {
    ANIM_STOP(74, 0, 0, 60),
};

AnimWord anim_8013932C[2] = {
    ANIM_STOP(75, 0, 0, 60),
};

AnimWord anim_80139334[2] = {
    ANIM_STOP(76, 0, 0, 60),
};

AnimWord anim_8013933C[2] = {
    ANIM_STOP(77, 0, 0, 60),
};

AnimWord anim_80139344[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8013934C[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_80139354[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8013935C[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_80139364[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8013936C[2] = {
    ANIM_STOP(24, 0, 0, 60),
};

AnimWord anim_80139374[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

AnimWord anim_8013937C[14] = {
    ANIM(26, 0, 0, 3),
    ANIM(27, 0, 0, 3),
    ANIM(28, 0, 0, 3),
    ANIM(29, 0, 0, 3),
    ANIM(28, 0, 0, 3),
    ANIM(27, 0, 0, 3),
    ANIM_STOP(26, 0, 0, 3),
};

AnimWord anim_801393B4[8] = {
    ANIM(30, 0, 0, 60),
    ANIM(31, 0, 0, 8),
    ANIM(32, 0, 0, 8),
    ANIM_STOP(33, 0, 0, 8),
};

AnimWord anim_801393D4[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_801393DC[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_801393E4[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_801393EC[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_801393F4[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_801393FC[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80139404[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8013940C[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_80139414[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8013941C[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_80139424[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8013942C[2] = {
    ANIM_STOP(26, 0, 0, 60),
};

AnimWord anim_80139434[12] = {
    ANIM(27, 0, 0, 8),
    ANIM(28, 0, 0, 8),
    ANIM(29, 0, 0, 8),
    ANIM(30, 0, 0, 8),
    ANIM(31, 0, 0, 8),
    ANIM_STOP(32, 0, 0, 8),
};

AnimWord anim_80139464[5] = {
    ANIM(33, 0, 0, 16),
    ANIM_JUMP(34, 0, 0, 16, anim_80139464),
};

AnimWord anim_80139478[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_80139480[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_80139488[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_80139490[2] = {
    ANIM_STOP(51, 0, 0, 60),
};

AnimWord anim_80139498[12] = {
    ANIM(38, 0, 0, 5),
    ANIM(39, 0, 0, 5),
    ANIM(40, 0, 0, 5),
    ANIM(41, 0, 0, 5),
    ANIM(42, 0, 0, 5),
    ANIM_STOP(43, 0, 0, 5),
};

AnimWord anim_801394C8[2] = {
    ANIM_STOP(44, 0, 0, 60),
};

AnimWord anim_801394D0[4] = {
    ANIM(45, 0, 0, 4),
    ANIM_STOP(46, 0, 0, 60),
};

AnimWord anim_801394E0[2] = {
    ANIM_STOP(47, 0, 0, 16),
};

AnimWord anim_801394E8[4] = {
    ANIM(48, 0, 0, 12),
    ANIM_STOP(49, 0, 0, 12),
};

AnimWord anim_801394F8[2] = {
    ANIM_STOP(50, 0, 0, 60),
};

void *ptrs_80139500[3] = {
    anim_8013957C,
    anim_80139584,
    anim_801395A8,
};

void *ptrs_8013950C[1] = {
    anim_801395D0,
};

void *ptrs_80139510[1] = {
    anim_80139604,
};

void *ptrs_80139514[3] = {
    anim_80139670,
    anim_80139678,
    anim_80139758,
};

void *ptrs_80139520[1] = {
    anim_80139680,
};

void *ptrs_80139524[1] = {
    anim_801396B4,
};

void *ptrs_80139528[2] = {
    anim_801396BC,
    anim_801396C4,
};

void *ptrs_80139530[2] = {
    anim_801397B8,
    anim_801396F8,
};

void *ptrs_80139538[14] = {
    anim_80139740,
    anim_80139760,
    anim_80139738,
    anim_80139728,
    anim_80139720,
    anim_80139718,
    anim_80139710,
    anim_80139748,
    anim_80139750,
    anim_801397DC,
    anim_80139800,
    anim_80139824,
    anim_80139848,
    anim_801395B0,
};

void *ptrs_80139570[1] = {
    anim_801395C0,
};

void *ptrs_80139574[1] = {
    anim_801397A0,
};

void *ptrs_80139578[1] = {
    anim_80139648,
};

AnimWord anim_8013957C[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80139584[9] = {
    ANIM(1, 0, 0, 16),
    ANIM(0, 0, 0, 12),
    ANIM(2, 0, 0, 16),
    ANIM_JUMP(0, 0, 0, 12, anim_80139584),
};

AnimWord anim_801395A8[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_801395B0[4] = {
    ANIM(54, 0, 0, 6),
    ANIM_STOP(55, 0, 0, 6),
};

AnimWord anim_801395C0[4] = {
    ANIM(55, 0, 0, 6),
    ANIM_STOP(54, 0, 0, 6),
};

AnimWord anim_801395D0[13] = {
    ANIM(4, 0, 0, 12),
    ANIM(5, 0, 0, 6),
    ANIM(6, 0, 0, 8),
    ANIM(7, 0, 0, 12),
    ANIM(8, 0, 0, 6),
    ANIM_JUMP(9, 0, 0, 8, anim_801395D0),
};

AnimWord anim_80139604[17] = {
    ANIM(10, 0, 0, 3),
    ANIM(11, 0, 0, 3),
    ANIM(12, 0, 0, 3),
    ANIM(13, 0, 0, 3),
    ANIM(14, 0, 0, 3),
    ANIM(15, 0, 0, 3),
    ANIM(16, 0, 0, 3),
    ANIM_JUMP(17, 0, 0, 3, anim_80139604),
};

AnimWord anim_80139648[10] = {
    ANIM(10, 0, 0, 8),
    ANIM(11, 0, 0, 8),
    ANIM(12, 0, 0, 8),
    ANIM(13, 0, 0, 8),
    ANIM_STOP(10, 0, 0, 8),
};

AnimWord anim_80139670[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_80139678[2] = {
    ANIM_STOP(18, 0, 0, 60),
};

AnimWord anim_80139680[13] = {
    ANIM(20, 0, 0, 5),
    ANIM(21, 0, 0, 2),
    ANIM(22, 0, 0, 3),
    ANIM(23, 0, 0, 5),
    ANIM(22, 0, 0, 2),
    ANIM_JUMP(21, 0, 0, 3, anim_80139680),
};

AnimWord anim_801396B4[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_801396BC[2] = {
    ANIM_STOP(30, 0, 0, 60),
};

AnimWord anim_801396C4[13] = {
    ANIM(31, 0, 0, 12),
    ANIM(32, 0, 0, 8),
    ANIM(33, 0, 0, 2),
    ANIM(34, 0, 0, 12),
    ANIM(33, 0, 0, 8),
    ANIM_JUMP(32, 0, 0, 2, anim_801396C4),
};

AnimWord anim_801396F8[6] = {
    ANIM(35, 0, 0, 3),
    ANIM(36, 0, 0, 2),
    ANIM(37, 0, 0, 2),
};

AnimWord anim_80139710[2] = {
    ANIM(38, 0, 0, 1),
};

AnimWord anim_80139718[2] = {
    ANIM(39, 0, 0, 1),
};

AnimWord anim_80139720[2] = {
    ANIM(40, 0, 0, 1),
};

AnimWord anim_80139728[4] = {
    ANIM(41, 0, 0, 1),
    ANIM(42, 0, 0, 8),
};

AnimWord anim_80139738[2] = {
    ANIM_STOP(24, 0, 0, 10),
};

AnimWord anim_80139740[2] = {
    ANIM(43, 0, 0, 3),
};

AnimWord anim_80139748[2] = {
    ANIM(44, 0, 0, 2),
};

AnimWord anim_80139750[2] = {
    ANIM(45, 0, 0, 1),
};

AnimWord anim_80139758[2] = {
    ANIM_STOP(19, 0, 0, 10),
};

AnimWord anim_80139760[16] = {
    ANIM(46, 0, 0, 3),
    ANIM(47, 0, 0, 3),
    ANIM(48, 0, 0, 3),
    ANIM(49, 0, 0, 3),
    ANIM(50, 0, 0, 3),
    ANIM(51, 0, 0, 3),
    ANIM(52, 0, 0, 3),
    ANIM_STOP(53, 0, 0, 3),
};

AnimWord anim_801397A0[6] = {
    ANIM(65, 0, 0, 6),
    ANIM(66, 0, 0, 6),
    ANIM_STOP(67, 0, 0, 6),
};

AnimWord anim_801397B8[9] = {
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM_JUMP(28, 0, 0, 8, anim_801397B8),
};

AnimWord anim_801397DC[9] = {
    ANIM(56, 0, 0, 8),
    ANIM(57, 0, 0, 8),
    ANIM(58, 0, 0, 8),
    ANIM_JUMP(57, 0, 0, 8, anim_801397DC),
};

AnimWord anim_80139800[9] = {
    ANIM(59, 0, 0, 8),
    ANIM(60, 0, 0, 8),
    ANIM(61, 0, 0, 8),
    ANIM_JUMP(60, 0, 0, 8, anim_80139800),
};

AnimWord anim_80139824[9] = {
    ANIM(62, 0, 0, 8),
    ANIM(63, 0, 0, 8),
    ANIM(64, 0, 0, 8),
    ANIM_JUMP(63, 0, 0, 8, anim_80139824),
};

AnimWord anim_80139848[7] = {
    ANIM(68, 0, 0, 8),
    ANIM(69, 0, 0, 8),
    ANIM_JUMP(70, 0, 0, 8, anim_80139848),
};

void *ptrs_80139864[2] = {
    D_801398B8,
    D_801398B8,
};

void *ptrs_8013986C[1] = {
    D_801398C0,
};

void *ptrs_80139870[1] = {
    D_801398D8,
};

void *ptrs_80139874[1] = {
    D_801398E0,
};

void *ptrs_80139878[1] = {
    D_801398E8,
};

void *ptrs_8013987C[1] = {
    D_80139910,
};
