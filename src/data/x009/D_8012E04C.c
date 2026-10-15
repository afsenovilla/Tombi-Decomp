// DATA 8012e04c 3640 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8012EECC[];
extern void *ptrs_8012E04C[];
extern void *ptrs_8012E050[];
extern void *ptrs_8012E058[];
extern void *ptrs_8012E05C[];
extern void *ptrs_8012E060[];
extern void *ptrs_8012E06C[];
extern void *ptrs_8012E070[];
extern void *ptrs_8012E074[];
extern void *ptrs_8012E078[];
extern void *ptrs_8012E07C[];
extern void *ptrs_8012E084[];
extern void *ptrs_8012E0A0[];
extern void *ptrs_8012E0BC[];
extern void *ptrs_8012E0D8[];
extern void *ptrs_8012E0F4[];
extern AnimWord anim_8012E1AC[];
extern AnimWord anim_8012E1B4[];
extern AnimWord anim_8012E1BC[];
extern AnimWord anim_8012E1C4[];
extern AnimWord anim_8012E208[];
extern AnimWord anim_8012E228[];
extern AnimWord anim_8012E230[];
extern AnimWord anim_8012E250[];
extern AnimWord anim_8012E270[];
extern AnimWord anim_8012E290[];
extern AnimWord anim_8012E2A0[];
extern AnimWord anim_8012E2C0[];
extern AnimWord anim_8012E2C8[];
extern AnimWord anim_8012E2D0[];
extern AnimWord anim_8012E2D8[];
extern AnimWord anim_8012E31C[];
extern AnimWord anim_8012E33C[];
extern AnimWord anim_8012E344[];
extern AnimWord anim_8012E364[];
extern AnimWord anim_8012E388[];
extern AnimWord anim_8012E3A8[];
extern AnimWord anim_8012E3B0[];
extern AnimWord anim_8012E3B8[];
extern AnimWord anim_8012E3C0[];
extern AnimWord anim_8012E3C8[];
extern AnimWord anim_8012E40C[];
extern AnimWord anim_8012E42C[];
extern AnimWord anim_8012E444[];
extern AnimWord anim_8012E474[];
extern AnimWord anim_8012E494[];
extern AnimWord anim_8012E4B4[];
extern AnimWord anim_8012E4C4[];
extern AnimWord anim_8012E514[];
extern AnimWord anim_8012E51C[];
extern AnimWord anim_8012E530[];
extern AnimWord anim_8012E550[];
extern AnimWord anim_8012E558[];
extern AnimWord anim_8012E560[];
extern AnimWord anim_8012E590[];
extern AnimWord anim_8012E598[];
extern AnimWord anim_8012E5A0[];
extern AnimWord anim_8012E5A8[];
extern AnimWord anim_8012E5CC[];
extern AnimWord anim_8012E62C[];
extern AnimWord anim_8012E634[];
extern AnimWord anim_8012E63C[];
extern AnimWord anim_8012E644[];
extern AnimWord anim_8012E64C[];
extern AnimWord anim_8012E654[];
extern AnimWord anim_8012E65C[];
extern AnimWord anim_8012E664[];
extern AnimWord anim_8012E66C[];
extern AnimWord anim_8012E674[];
extern AnimWord anim_8012E67C[];
extern AnimWord anim_8012E684[];
extern AnimWord anim_8012E68C[];
extern AnimWord anim_8012E694[];
extern AnimWord anim_8012E69C[];
extern AnimWord anim_8012E6B0[];
extern AnimWord anim_8012E6D0[];
extern AnimWord anim_8012E6E0[];
extern AnimWord anim_8012E6E8[];
extern AnimWord anim_8012E6F0[];
extern AnimWord anim_8012E6F8[];
extern AnimWord anim_8012E700[];
extern AnimWord anim_8012E728[];
extern AnimWord anim_8012E738[];
extern AnimWord anim_8012E740[];
extern AnimWord anim_8012E748[];
extern AnimWord anim_8012E750[];
extern AnimWord anim_8012E758[];
extern AnimWord anim_8012E760[];
extern AnimWord anim_8012E768[];
extern AnimWord anim_8012E770[];
extern AnimWord anim_8012E778[];
extern AnimWord anim_8012E780[];
extern AnimWord anim_8012E788[];
extern AnimWord anim_8012E790[];
extern AnimWord anim_8012E798[];
extern AnimWord anim_8012E7B0[];
extern AnimWord anim_8012E7D0[];
extern AnimWord anim_8012E7D8[];
extern AnimWord anim_8012E7F4[];
extern AnimWord anim_8012E7FC[];
extern AnimWord anim_8012E804[];
extern AnimWord anim_8012E80C[];
extern AnimWord anim_8012E814[];
extern AnimWord anim_8012E81C[];
extern AnimWord anim_8012E824[];
extern AnimWord anim_8012E82C[];
extern AnimWord anim_8012E834[];
extern AnimWord anim_8012E83C[];
extern AnimWord anim_8012E844[];
extern AnimWord anim_8012E84C[];
extern AnimWord anim_8012E854[];
extern AnimWord anim_8012E85C[];
extern AnimWord anim_8012E864[];
extern AnimWord anim_8012E86C[];
extern AnimWord anim_8012E874[];
extern AnimWord anim_8012E87C[];
extern AnimWord anim_8012E884[];
extern AnimWord anim_8012E88C[];
extern AnimWord anim_8012E894[];
extern AnimWord anim_8012E89C[];
extern AnimWord anim_8012E8A4[];
extern AnimWord anim_8012E8AC[];
extern AnimWord anim_8012E8B4[];
extern AnimWord anim_8012E8BC[];
extern AnimWord anim_8012E8C4[];
extern AnimWord anim_8012E8CC[];
extern AnimWord anim_8012E8D4[];
extern AnimWord anim_8012E8DC[];
extern AnimWord anim_8012E8E4[];
extern AnimWord anim_8012E8EC[];
extern AnimWord anim_8012E8F4[];
extern AnimWord anim_8012E8FC[];
extern AnimWord anim_8012E904[];
extern AnimWord anim_8012E90C[];
extern AnimWord anim_8012E914[];
extern AnimWord anim_8012E91C[];
extern AnimWord anim_8012E924[];
extern AnimWord anim_8012E92C[];
extern AnimWord anim_8012E934[];
extern AnimWord anim_8012E93C[];
extern AnimWord anim_8012E94C[];
extern AnimWord anim_8012E954[];
extern AnimWord anim_8012E95C[];
extern AnimWord anim_8012E964[];
extern AnimWord anim_8012E96C[];
extern AnimWord anim_8012E974[];
extern AnimWord anim_8012E97C[];
extern AnimWord anim_8012E984[];
extern AnimWord anim_8012E98C[];
extern AnimWord anim_8012E994[];
extern AnimWord anim_8012E99C[];
extern AnimWord anim_8012E9A4[];
extern AnimWord anim_8012E9AC[];
extern AnimWord anim_8012E9B4[];
extern void *ptrs_8012E9BC[];
extern void *ptrs_8012E9C0[];
extern void *ptrs_8012E9C4[];
extern void *ptrs_8012E9C8[];
extern void *ptrs_8012E9CC[];
extern void *ptrs_8012E9D0[];
extern void *ptrs_8012E9D4[];
extern void *ptrs_8012E9D8[];
extern void *ptrs_8012E9DC[];
extern void *ptrs_8012E9E0[];
extern void *ptrs_8012E9E4[];
extern void *ptrs_8012E9E8[];
extern void *ptrs_8012E9EC[];
extern void *ptrs_8012E9F0[];
extern void *ptrs_8012E9F8[];
extern void *ptrs_8012E9FC[];
extern void *ptrs_8012EA00[];
extern void *ptrs_8012EA04[];
extern void *ptrs_8012EA10[];
extern void *ptrs_8012EA14[];
extern void *ptrs_8012EA1C[];
extern void *ptrs_8012EA20[];
extern void *ptrs_8012EA24[];
extern void *ptrs_8012EA30[];
extern void *ptrs_8012EA38[];
extern void *ptrs_8012EA3C[];
extern void *ptrs_8012EA40[];
extern void *ptrs_8012EA44[];
extern AnimWord anim_8012EA48[];
extern AnimWord anim_8012EA50[];
extern AnimWord anim_8012EA58[];
extern AnimWord anim_8012EA7C[];
extern AnimWord anim_8012EAA0[];
extern AnimWord anim_8012EAC0[];
extern AnimWord anim_8012EAE0[];
extern AnimWord anim_8012EB00[];
extern AnimWord anim_8012EB20[];
extern AnimWord anim_8012EB40[];
extern AnimWord anim_8012EB60[];
extern AnimWord anim_8012EB68[];
extern AnimWord anim_8012EB70[];
extern AnimWord anim_8012EB78[];
extern AnimWord anim_8012EB80[];
extern AnimWord anim_8012EB88[];
extern AnimWord anim_8012EBC8[];
extern AnimWord anim_8012EC08[];
extern AnimWord anim_8012EC4C[];
extern AnimWord anim_8012EC98[];
extern AnimWord anim_8012ECA8[];
extern AnimWord anim_8012ECB0[];
extern AnimWord anim_8012ECC0[];
extern AnimWord anim_8012ECD8[];
extern AnimWord anim_8012ECEC[];
extern AnimWord anim_8012ECF4[];
extern AnimWord anim_8012ECFC[];
extern AnimWord anim_8012ED04[];
extern AnimWord anim_8012ED0C[];
extern AnimWord anim_8012ED44[];
extern AnimWord anim_8012ED90[];
extern AnimWord anim_8012EDD4[];
extern AnimWord anim_8012EE30[];
extern AnimWord anim_8012EE54[];
extern AnimWord anim_8012EE70[];
extern void *ptrs_8012EE80[];

void *ptrs_8012E04C[1] = {
    anim_8012E69C,
};

void *ptrs_8012E050[2] = {
    anim_8012E6B0,
    anim_8012E738,
};

void *ptrs_8012E058[1] = {
    anim_8012E844,
};

void *ptrs_8012E05C[1] = {
    anim_8012E7D0,
};

void *ptrs_8012E060[3] = {
    anim_8012E7D8,
    anim_8012E798,
    anim_8012E7B0,
};

void *ptrs_8012E06C[1] = {
    anim_8012E780,
};

void *ptrs_8012E070[1] = {
    anim_8012E788,
};

void *ptrs_8012E074[1] = {
    anim_8012E790,
};

void *ptrs_8012E078[1] = {
    anim_8012E92C,
};

void *ptrs_8012E07C[2] = {
    anim_8012E934,
    anim_8012E93C,
};

void *ptrs_8012E084[7] = {
    anim_8012E84C,
    anim_8012E854,
    anim_8012E85C,
    anim_8012E864,
    anim_8012E86C,
    anim_8012E874,
    anim_8012E87C,
};

void *ptrs_8012E0A0[7] = {
    anim_8012E884,
    anim_8012E88C,
    anim_8012E894,
    anim_8012E89C,
    anim_8012E8A4,
    anim_8012E8AC,
    anim_8012E8B4,
};

void *ptrs_8012E0BC[7] = {
    anim_8012E8BC,
    anim_8012E8C4,
    anim_8012E8CC,
    anim_8012E8D4,
    anim_8012E8DC,
    anim_8012E8E4,
    anim_8012E8EC,
};

void *ptrs_8012E0D8[7] = {
    anim_8012E8F4,
    anim_8012E8FC,
    anim_8012E904,
    anim_8012E90C,
    anim_8012E914,
    anim_8012E91C,
    anim_8012E924,
};

void *ptrs_8012E0F4[46] = {
    anim_8012E1AC,
    anim_8012E1B4,
    anim_8012E1BC,
    anim_8012E1C4,
    anim_8012E208,
    anim_8012E228,
    anim_8012E230,
    anim_8012E250,
    anim_8012E270,
    anim_8012E290,
    anim_8012E2A0,
    anim_8012E2C0,
    anim_8012E2C8,
    anim_8012E2D0,
    anim_8012E2D8,
    anim_8012E31C,
    anim_8012E33C,
    anim_8012E344,
    anim_8012E364,
    anim_8012E388,
    anim_8012E3A8,
    anim_8012E3B0,
    anim_8012E3B8,
    anim_8012E3C0,
    anim_8012E3C8,
    anim_8012E40C,
    anim_8012E42C,
    anim_8012E444,
    anim_8012E474,
    anim_8012E494,
    anim_8012E4B4,
    anim_8012E514,
    anim_8012E51C,
    anim_8012E530,
    anim_8012E550,
    anim_8012E558,
    anim_8012E560,
    anim_8012E590,
    anim_8012E598,
    anim_8012E5A0,
    anim_8012E5A8,
    anim_8012E5CC,
    anim_8012E62C,
    anim_8012E634,
    anim_8012E63C,
    anim_8012E644,
};

AnimWord anim_8012E1AC[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012E1B4[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8012E1BC[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8012E1C4[17] = {
    ANIM(3, 0, 0, 7),
    ANIM(4, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM(6, 0, 0, 7),
    ANIM(7, 0, 0, 7),
    ANIM(6, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM_JUMP(4, 0, 0, 7, anim_8012E1C4),
};

AnimWord anim_8012E208[8] = {
    ANIM(8, 0, 0, 20),
    ANIM(9, 0, 0, 5),
    ANIM(10, 0, 0, 20),
    ANIM_STOP(9, 0, 0, 5),
};

AnimWord anim_8012E228[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8012E230[8] = {
    ANIM(12, 0, 0, 40),
    ANIM(13, 0, 0, 8),
    ANIM(14, 0, 0, 40),
    ANIM_STOP(13, 0, 0, 8),
};

AnimWord anim_8012E250[8] = {
    ANIM(15, 0, 0, 40),
    ANIM(16, 0, 0, 8),
    ANIM(17, 0, 0, 40),
    ANIM_STOP(16, 0, 0, 8),
};

AnimWord anim_8012E270[8] = {
    ANIM(18, 0, 0, 4),
    ANIM(19, 0, 0, 6),
    ANIM(20, 0, 0, 12),
    ANIM_STOP(19, 0, 0, 6),
};

AnimWord anim_8012E290[4] = {
    ANIM(22, 0, 0, 10),
    ANIM_STOP(23, 0, 0, 24),
};

AnimWord anim_8012E2A0[8] = {
    ANIM(24, 0, 0, 10),
    ANIM(25, 0, 0, 6),
    ANIM(26, 0, 0, 10),
    ANIM_STOP(25, 0, 0, 6),
};

AnimWord anim_8012E2C0[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_8012E2C8[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8012E2D0[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_8012E2D8[17] = {
    ANIM(30, 0, 0, 7),
    ANIM(31, 0, 0, 7),
    ANIM(32, 0, 0, 7),
    ANIM(33, 0, 0, 7),
    ANIM(34, 0, 0, 7),
    ANIM(35, 0, 0, 7),
    ANIM(36, 0, 0, 7),
    ANIM_JUMP(37, 0, 0, 7, anim_8012E2D8),
};

AnimWord anim_8012E31C[8] = {
    ANIM(38, 0, 0, 20),
    ANIM(39, 0, 0, 5),
    ANIM(40, 0, 0, 20),
    ANIM_STOP(39, 0, 0, 5),
};

AnimWord anim_8012E33C[2] = {
    ANIM_STOP(41, 0, 0, 60),
};

AnimWord anim_8012E344[8] = {
    ANIM(42, 0, 0, 40),
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 40),
    ANIM_STOP(43, 0, 0, 8),
};

AnimWord anim_8012E364[9] = {
    ANIM(45, 0, 0, 40),
    ANIM(46, 0, 0, 8),
    ANIM(47, 0, 0, 40),
    ANIM_JUMP(46, 0, 0, 8, anim_8012E364),
};

AnimWord anim_8012E388[8] = {
    ANIM(48, 0, 0, 4),
    ANIM(49, 0, 0, 6),
    ANIM(50, 0, 0, 12),
    ANIM_STOP(51, 0, 0, 6),
};

AnimWord anim_8012E3A8[2] = {
    ANIM_STOP(52, 0, 0, 60),
};

AnimWord anim_8012E3B0[2] = {
    ANIM_STOP(53, 0, 0, 60),
};

AnimWord anim_8012E3B8[2] = {
    ANIM_STOP(54, 0, 0, 60),
};

AnimWord anim_8012E3C0[2] = {
    ANIM_STOP(55, 0, 0, 60),
};

AnimWord anim_8012E3C8[17] = {
    ANIM(56, 0, 0, 3),
    ANIM(57, 0, 0, 2),
    ANIM(58, 0, 0, 2),
    ANIM(59, 0, 0, 2),
    ANIM(60, 0, 0, 3),
    ANIM(61, 0, 0, 2),
    ANIM(62, 0, 0, 2),
    ANIM_JUMP(63, 0, 0, 2, anim_8012E3C8),
};

AnimWord anim_8012E40C[8] = {
    ANIM(64, 0, 0, 20),
    ANIM(65, 0, 0, 5),
    ANIM(66, 0, 0, 20),
    ANIM_STOP(65, 0, 0, 5),
};

AnimWord anim_8012E42C[6] = {
    ANIM(81, 0, 0, 6),
    ANIM(82, 0, 0, 10),
    ANIM_STOP(83, 0, 0, 60),
};

AnimWord anim_8012E444[12] = {
    ANIM(67, 0, 0, 24),
    ANIM(68, 0, 0, 6),
    ANIM(69, 0, 0, 4),
    ANIM(70, 0, 0, 24),
    ANIM(69, 0, 0, 6),
    ANIM_STOP(68, 0, 0, 4),
};

AnimWord anim_8012E474[8] = {
    ANIM(71, 0, 0, 40),
    ANIM(72, 0, 0, 8),
    ANIM(73, 0, 0, 40),
    ANIM_STOP(72, 0, 0, 8),
};

AnimWord anim_8012E494[8] = {
    ANIM(74, 0, 0, 40),
    ANIM(75, 0, 0, 8),
    ANIM(76, 0, 0, 40),
    ANIM_STOP(75, 0, 0, 8),
};

AnimWord anim_8012E4B4[4] = {
    ANIM(77, 0, 0, 4),
    ANIM(78, 0, 0, 6),
};

AnimWord anim_8012E4C4[20] = {
    ANIM(79, 0, 0, 12),
    ANIM(80, 0, 0, 6),
    ANIM(77, 0, 0, 4),
    ANIM(78, 0, 0, 6),
    ANIM(79, 0, 0, 12),
    ANIM(80, 0, 0, 6),
    ANIM(77, 0, 0, 4),
    ANIM(78, 0, 0, 6),
    ANIM(79, 0, 0, 12),
    ANIM_STOP(80, 0, 0, 6),
};

AnimWord anim_8012E514[2] = {
    ANIM_STOP(84, 0, 0, 60),
};

AnimWord anim_8012E51C[5] = {
    ANIM(85, 0, 0, 10),
    ANIM_JUMP(86, 0, 0, 24, anim_8012E51C),
};

AnimWord anim_8012E530[8] = {
    ANIM(87, 0, 0, 10),
    ANIM(88, 0, 0, 6),
    ANIM(89, 0, 0, 10),
    ANIM_STOP(88, 0, 0, 6),
};

AnimWord anim_8012E550[2] = {
    ANIM_STOP(95, 0, 0, 8),
};

AnimWord anim_8012E558[2] = {
    ANIM_STOP(96, 0, 0, 8),
};

AnimWord anim_8012E560[12] = {
    ANIM(90, 0, 0, 24),
    ANIM(91, 0, 0, 4),
    ANIM(92, 0, 0, 3),
    ANIM(93, 0, 0, 2),
    ANIM(94, 0, 0, 16),
    ANIM_STOP(91, 0, 0, 4),
};

AnimWord anim_8012E590[2] = {
    ANIM_STOP(99, 0, 0, 8),
};

AnimWord anim_8012E598[2] = {
    ANIM_STOP(100, 0, 0, 8),
};

AnimWord anim_8012E5A0[2] = {
    ANIM_STOP(101, 0, 0, 8),
};

AnimWord anim_8012E5A8[9] = {
    ANIM(102, 0, 0, 6),
    ANIM(103, 0, 0, 8),
    ANIM(104, 0, 0, 6),
    ANIM_JUMP(105, 0, 0, 8, anim_8012E5A8),
};

AnimWord anim_8012E5CC[24] = {
    ANIM(107, 0, 0, 16),
    ANIM(108, 0, 0, 5),
    ANIM(109, 0, 0, 16),
    ANIM(108, 0, 0, 5),
    ANIM(107, 0, 0, 16),
    ANIM(108, 0, 0, 5),
    ANIM(109, 0, 0, 16),
    ANIM(108, 0, 0, 5),
    ANIM(107, 0, 0, 16),
    ANIM(108, 0, 0, 5),
    ANIM(109, 0, 0, 16),
    ANIM_STOP(108, 0, 0, 5),
};

AnimWord anim_8012E62C[2] = {
    ANIM_STOP(110, 0, 0, 60),
};

AnimWord anim_8012E634[2] = {
    ANIM_STOP(111, 0, 0, 60),
};

AnimWord anim_8012E63C[2] = {
    ANIM_STOP(112, 0, 0, 60),
};

AnimWord anim_8012E644[2] = {
    ANIM_STOP(106, 0, 0, 60),
};

AnimWord anim_8012E64C[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012E654[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8012E65C[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8012E664[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8012E66C[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8012E674[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8012E67C[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_8012E684[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_8012E68C[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_8012E694[2] = {
    ANIM_STOP(16, 0, 0, 60),
};

AnimWord anim_8012E69C[5] = {
    ANIM(17, 0, 0, 20),
    ANIM_JUMP(18, 0, 0, 20, anim_8012E69C),
};

AnimWord anim_8012E6B0[8] = {
    ANIM(17, 0, 0, 5),
    ANIM(19, 0, 0, 5),
    ANIM(20, 0, 0, 5),
    ANIM(21, 0, 0, 5),
};

AnimWord anim_8012E6D0[4] = {
    ANIM(22, 0, 0, 5),
    ANIM_STOP(23, 0, 0, 5),
};

AnimWord anim_8012E6E0[2] = {
    ANIM_STOP(24, 0, 0, 60),
};

AnimWord anim_8012E6E8[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

AnimWord anim_8012E6F0[2] = {
    ANIM_STOP(26, 0, 0, 60),
};

AnimWord anim_8012E6F8[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_8012E700[10] = {
    ANIM(28, 0, 0, 16),
    ANIM(29, 0, 0, 6),
    ANIM(30, 0, 0, 6),
    ANIM(31, 0, 0, 60),
    ANIM(30, 0, 0, 6),
};

AnimWord anim_8012E728[4] = {
    ANIM(29, 0, 0, 6),
    ANIM_STOP(28, 0, 0, 6),
};

AnimWord anim_8012E738[2] = {
    ANIM_STOP(32, 0, 0, 2),
};

AnimWord anim_8012E740[2] = {
    ANIM_STOP(33, 0, 0, 2),
};

AnimWord anim_8012E748[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8012E750[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8012E758[2] = {
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_8012E760[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_8012E768[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_8012E770[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8012E778[2] = {
    ANIM_STOP(12, 0, 0, 60),
};

AnimWord anim_8012E780[2] = {
    ANIM_STOP(44, 0, 0, 60),
};

AnimWord anim_8012E788[2] = {
    ANIM_STOP(45, 0, 0, 60),
};

AnimWord anim_8012E790[2] = {
    ANIM_STOP(46, 0, 0, 60),
};

AnimWord anim_8012E798[6] = {
    ANIM(48, 0, 0, 4),
    ANIM(49, 0, 0, 4),
    ANIM_STOP(50, 0, 0, 4),
};

AnimWord anim_8012E7B0[8] = {
    ANIM(53, 0, 0, 10),
    ANIM(51, 0, 0, 10),
    ANIM(52, 0, 0, 10),
    ANIM_STOP(51, 0, 0, 10),
};

AnimWord anim_8012E7D0[2] = {
    ANIM_STOP(54, 0, 0, 60),
};

AnimWord anim_8012E7D8[7] = {
    ANIM(55, 0, 0, 6),
    ANIM(57, 0, 0, 6),
    ANIM_JUMP(56, 0, 0, 6, anim_8012E7D8),
};

AnimWord anim_8012E7F4[2] = {
    ANIM_STOP(58, 0, 0, 60),
};

AnimWord anim_8012E7FC[2] = {
    ANIM_STOP(59, 0, 0, 60),
};

AnimWord anim_8012E804[2] = {
    ANIM_STOP(60, 0, 0, 60),
};

AnimWord anim_8012E80C[2] = {
    ANIM_STOP(61, 0, 0, 60),
};

AnimWord anim_8012E814[2] = {
    ANIM_STOP(62, 0, 0, 60),
};

AnimWord anim_8012E81C[2] = {
    ANIM_STOP(63, 0, 0, 60),
};

AnimWord anim_8012E824[2] = {
    ANIM_STOP(64, 0, 0, 60),
};

AnimWord anim_8012E82C[2] = {
    ANIM_STOP(65, 0, 0, 60),
};

AnimWord anim_8012E834[2] = {
    ANIM_STOP(66, 0, 0, 60),
};

AnimWord anim_8012E83C[2] = {
    ANIM_STOP(67, 0, 0, 60),
};

AnimWord anim_8012E844[2] = {
    ANIM_STOP(68, 0, 0, 60),
};

AnimWord anim_8012E84C[2] = {
    ANIM_STOP(101, 0, 0, 60),
};

AnimWord anim_8012E854[2] = {
    ANIM_STOP(102, 0, 0, 60),
};

AnimWord anim_8012E85C[2] = {
    ANIM_STOP(103, 0, 0, 60),
};

AnimWord anim_8012E864[2] = {
    ANIM_STOP(104, 0, 0, 60),
};

AnimWord anim_8012E86C[2] = {
    ANIM_STOP(105, 0, 0, 60),
};

AnimWord anim_8012E874[2] = {
    ANIM_STOP(106, 0, 0, 60),
};

AnimWord anim_8012E87C[2] = {
    ANIM_STOP(107, 0, 0, 60),
};

AnimWord anim_8012E884[2] = {
    ANIM_STOP(108, 0, 0, 60),
};

AnimWord anim_8012E88C[2] = {
    ANIM_STOP(109, 0, 0, 60),
};

AnimWord anim_8012E894[2] = {
    ANIM_STOP(110, 0, 0, 60),
};

AnimWord anim_8012E89C[2] = {
    ANIM_STOP(111, 0, 0, 60),
};

AnimWord anim_8012E8A4[2] = {
    ANIM_STOP(112, 0, 0, 60),
};

AnimWord anim_8012E8AC[2] = {
    ANIM_STOP(113, 0, 0, 60),
};

AnimWord anim_8012E8B4[2] = {
    ANIM_STOP(114, 0, 0, 60),
};

AnimWord anim_8012E8BC[2] = {
    ANIM_STOP(94, 0, 0, 60),
};

AnimWord anim_8012E8C4[2] = {
    ANIM_STOP(95, 0, 0, 60),
};

AnimWord anim_8012E8CC[2] = {
    ANIM_STOP(96, 0, 0, 60),
};

AnimWord anim_8012E8D4[2] = {
    ANIM_STOP(97, 0, 0, 60),
};

AnimWord anim_8012E8DC[2] = {
    ANIM_STOP(98, 0, 0, 60),
};

AnimWord anim_8012E8E4[2] = {
    ANIM_STOP(99, 0, 0, 60),
};

AnimWord anim_8012E8EC[2] = {
    ANIM_STOP(100, 0, 0, 60),
};

AnimWord anim_8012E8F4[2] = {
    ANIM_STOP(87, 0, 0, 60),
};

AnimWord anim_8012E8FC[2] = {
    ANIM_STOP(88, 0, 0, 60),
};

AnimWord anim_8012E904[2] = {
    ANIM_STOP(89, 0, 0, 60),
};

AnimWord anim_8012E90C[2] = {
    ANIM_STOP(90, 0, 0, 60),
};

AnimWord anim_8012E914[2] = {
    ANIM_STOP(91, 0, 0, 60),
};

AnimWord anim_8012E91C[2] = {
    ANIM_STOP(92, 0, 0, 60),
};

AnimWord anim_8012E924[2] = {
    ANIM_STOP(93, 0, 0, 60),
};

AnimWord anim_8012E92C[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012E934[2] = {
    ANIM_STOP(1, 0, 0, 4),
};

AnimWord anim_8012E93C[4] = {
    ANIM(2, 0, 0, 12),
    ANIM_STOP(3, 0, 0, 12),
};

AnimWord anim_8012E94C[2] = {
    ANIM_STOP(73, 0, 0, 60),
};

AnimWord anim_8012E954[2] = {
    ANIM_STOP(74, 0, 0, 60),
};

AnimWord anim_8012E95C[2] = {
    ANIM_STOP(75, 0, 0, 60),
};

AnimWord anim_8012E964[2] = {
    ANIM_STOP(76, 0, 0, 60),
};

AnimWord anim_8012E96C[2] = {
    ANIM_STOP(77, 0, 0, 60),
};

AnimWord anim_8012E974[2] = {
    ANIM_STOP(78, 0, 0, 60),
};

AnimWord anim_8012E97C[2] = {
    ANIM_STOP(79, 0, 0, 60),
};

AnimWord anim_8012E984[2] = {
    ANIM_STOP(80, 0, 0, 60),
};

AnimWord anim_8012E98C[2] = {
    ANIM_STOP(81, 0, 0, 60),
};

AnimWord anim_8012E994[2] = {
    ANIM_STOP(82, 0, 0, 60),
};

AnimWord anim_8012E99C[2] = {
    ANIM_STOP(83, 0, 0, 60),
};

AnimWord anim_8012E9A4[2] = {
    ANIM_STOP(84, 0, 0, 60),
};

AnimWord anim_8012E9AC[2] = {
    ANIM_STOP(85, 0, 0, 60),
};

AnimWord anim_8012E9B4[2] = {
    ANIM_STOP(86, 0, 0, 60),
};

void *ptrs_8012E9BC[1] = {
    anim_8012EA48,
};

void *ptrs_8012E9C0[1] = {
    anim_8012EA50,
};

void *ptrs_8012E9C4[1] = {
    anim_8012EA58,
};

void *ptrs_8012E9C8[1] = {
    anim_8012EA7C,
};

void *ptrs_8012E9CC[1] = {
    anim_8012EAA0,
};

void *ptrs_8012E9D0[1] = {
    anim_8012EAC0,
};

void *ptrs_8012E9D4[1] = {
    anim_8012EAE0,
};

void *ptrs_8012E9D8[1] = {
    anim_8012EB00,
};

void *ptrs_8012E9DC[1] = {
    anim_8012EB20,
};

void *ptrs_8012E9E0[1] = {
    anim_8012EB40,
};

void *ptrs_8012E9E4[1] = {
    anim_8012EB60,
};

void *ptrs_8012E9E8[1] = {
    anim_8012EB68,
};

void *ptrs_8012E9EC[1] = {
    anim_8012EB70,
};

void *ptrs_8012E9F0[2] = {
    anim_8012EB78,
    anim_8012EB78,
};

void *ptrs_8012E9F8[1] = {
    anim_8012EB80,
};

void *ptrs_8012E9FC[1] = {
    anim_8012EB88,
};

void *ptrs_8012EA00[1] = {
    anim_8012EBC8,
};

void *ptrs_8012EA04[3] = {
    anim_8012EC4C,
    anim_8012EC98,
    anim_8012ECA8,
};

void *ptrs_8012EA10[1] = {
    anim_8012ECB0,
};

void *ptrs_8012EA14[2] = {
    anim_8012ECC0,
    anim_8012ECD8,
};

void *ptrs_8012EA1C[1] = {
    anim_8012ECEC,
};

void *ptrs_8012EA20[1] = {
    anim_8012ECF4,
};

void *ptrs_8012EA24[3] = {
    anim_8012ECFC,
    anim_8012ED04,
    anim_8012ED0C,
};

void *ptrs_8012EA30[2] = {
    anim_8012EDD4,
    anim_8012EE30,
};

void *ptrs_8012EA38[1] = {
    anim_8012EE54,
};

void *ptrs_8012EA3C[1] = {
    anim_8012EE70,
};

void *ptrs_8012EA40[1] = {
    anim_8012EC08,
};

void *ptrs_8012EA44[1] = {
    anim_8012ED90,
};

AnimWord anim_8012EA48[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012EA50[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8012EA58[9] = {
    ANIM(2, 0, 0, 2),
    ANIM(3, 0, 0, 2),
    ANIM(4, 0, 0, 2),
    ANIM_JUMP(3, 0, 0, 2, anim_8012EA58),
};

AnimWord anim_8012EA7C[9] = {
    ANIM(5, 0, 0, 12),
    ANIM(6, 0, 0, 6),
    ANIM(7, 0, 0, 14),
    ANIM_JUMP(6, 0, 0, 2, anim_8012EA7C),
};

AnimWord anim_8012EAA0[8] = {
    ANIM(1, 0, 0, 3),
    ANIM(8, 0, 0, 3),
    ANIM(9, 0, 0, 2),
    ANIM_STOP(2, 0, 0, 1),
};

AnimWord anim_8012EAC0[8] = {
    ANIM(2, 0, 0, 3),
    ANIM(9, 0, 0, 3),
    ANIM(8, 0, 0, 2),
    ANIM_STOP(1, 0, 0, 1),
};

AnimWord anim_8012EAE0[8] = {
    ANIM(2, 0, 0, 3),
    ANIM(10, 0, 0, 3),
    ANIM(11, 0, 0, 2),
    ANIM_STOP(5, 0, 0, 1),
};

AnimWord anim_8012EB00[8] = {
    ANIM(5, 0, 0, 3),
    ANIM(11, 0, 0, 3),
    ANIM(10, 0, 0, 2),
    ANIM_STOP(2, 0, 0, 1),
};

AnimWord anim_8012EB20[8] = {
    ANIM(5, 0, 0, 3),
    ANIM(12, 0, 0, 3),
    ANIM(13, 0, 0, 2),
    ANIM_STOP(1, 0, 0, 1),
};

AnimWord anim_8012EB40[8] = {
    ANIM(1, 0, 0, 3),
    ANIM(13, 0, 0, 3),
    ANIM(12, 0, 0, 2),
    ANIM_STOP(5, 0, 0, 1),
};

AnimWord anim_8012EB60[2] = {
    ANIM_STOP(80, 0, 0, 16),
};

AnimWord anim_8012EB68[2] = {
    ANIM_STOP(81, 0, 0, 8),
};

AnimWord anim_8012EB70[2] = {
    ANIM_STOP(82, 0, 0, 8),
};

AnimWord anim_8012EB78[2] = {
    ANIM_STOP(83, 0, 0, 60),
};

AnimWord anim_8012EB80[2] = {
    ANIM_STOP(14, 0, 0, 10),
};

AnimWord anim_8012EB88[16] = {
    ANIM(15, 0, 0, 1),
    ANIM(14, 0, 0, 1),
    ANIM(16, 0, 0, 1),
    ANIM(14, 0, 0, 1),
    ANIM(15, 0, 0, 2),
    ANIM(14, 0, 0, 1),
    ANIM(16, 0, 0, 2),
    ANIM_STOP(14, 0, 0, 1),
};

AnimWord anim_8012EBC8[16] = {
    ANIM(14, 0, 0, 2),
    ANIM(15, 0, 0, 3),
    ANIM(20, 0, 0, 4),
    ANIM(21, 0, 0, 5),
    ANIM(22, 0, 0, 4),
    ANIM(23, 0, 0, 3),
    ANIM(24, 0, 0, 2),
    ANIM_STOP(38, 0, 0, 3),
};

AnimWord anim_8012EC08[17] = {
    ANIM(38, 0, 0, 3),
    ANIM(24, 0, 0, 2),
    ANIM(23, 0, 0, 3),
    ANIM(22, 0, 0, 4),
    ANIM(21, 0, 0, 5),
    ANIM(20, 0, 0, 4),
    ANIM(15, 0, 0, 3),
    ANIM_JUMP(14, 0, 0, 2, anim_8012EB80),
};

AnimWord anim_8012EC4C[19] = {
    ANIM(39, 0, 0, 3),
    ANIM(40, 0, 0, 3),
    ANIM(41, 0, 0, 3),
    ANIM(42, 0, 0, 3),
    ANIM(43, 0, 0, 3),
    ANIM(44, 0, 0, 3),
    ANIM(18, 0, 0, 7),
    ANIM(19, 0, 0, 6),
    ANIM_JUMP(18, 0, 0, 7, anim_8012EC4C),
};

AnimWord anim_8012EC98[4] = {
    ANIM(25, 0, 0, 4),
    ANIM(26, 0, 0, 4),
};

AnimWord anim_8012ECA8[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_8012ECB0[4] = {
    ANIM(28, 0, 0, 5),
    ANIM_STOP(29, 0, 0, 5),
};

AnimWord anim_8012ECC0[6] = {
    ANIM(30, 0, 0, 4),
    ANIM(31, 0, 0, 4),
    ANIM(32, 0, 0, 4),
};

AnimWord anim_8012ECD8[5] = {
    ANIM(33, 0, 0, 6),
    ANIM_JUMP(34, 0, 0, 6, anim_8012ECD8),
};

AnimWord anim_8012ECEC[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_8012ECF4[2] = {
    ANIM_STOP(30, 0, 0, 8),
};

AnimWord anim_8012ECFC[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_8012ED04[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_8012ED0C[14] = {
    ANIM(74, 0, 0, 4),
    ANIM(75, 0, 0, 4),
    ANIM(76, 0, 0, 4),
    ANIM(77, 0, 0, 4),
    ANIM(78, 0, 0, 4),
    ANIM(79, 0, 0, 4),
    ANIM(46, 0, 0, 6),
};

AnimWord anim_8012ED44[19] = {
    ANIM(66, 0, 0, 6),
    ANIM(67, 0, 0, 6),
    ANIM(68, 0, 0, 6),
    ANIM(69, 0, 0, 6),
    ANIM(70, 0, 0, 6),
    ANIM(71, 0, 0, 6),
    ANIM(72, 0, 0, 6),
    ANIM(73, 0, 0, 6),
    ANIM_JUMP(49, 0, 0, 6, anim_8012ED0C),
};

AnimWord anim_8012ED90[17] = {
    ANIM(67, 0, 0, 3),
    ANIM(68, 0, 0, 3),
    ANIM(69, 0, 0, 3),
    ANIM(70, 0, 0, 3),
    ANIM(71, 0, 0, 3),
    ANIM(72, 0, 0, 3),
    ANIM(73, 0, 0, 3),
    ANIM_JUMP(49, 0, 0, 3, anim_8012ED90),
};

AnimWord anim_8012EDD4[23] = {
    ANIM(50, 0, 0, 3),
    ANIM(51, 0, 0, 3),
    ANIM(50, 0, 0, 3),
    ANIM(51, 0, 0, 3),
    ANIM(50, 0, 0, 3),
    ANIM(51, 0, 0, 3),
    ANIM(50, 0, 0, 3),
    ANIM(51, 0, 0, 3),
    ANIM(52, 0, 0, 4),
    ANIM(53, 0, 0, 8),
    ANIM_JUMP_STOP(54, 0, 0, 2, anim_8012EDD4),
};

AnimWord anim_8012EE30[9] = {
    ANIM(55, 0, 0, 12),
    ANIM(56, 0, 0, 6),
    ANIM(57, 0, 0, 12),
    ANIM_JUMP_STOP(56, 0, 0, 6, anim_8012EE30),
};

AnimWord anim_8012EE54[7] = {
    ANIM(58, 0, 0, 5),
    ANIM(59, 0, 0, 5),
    ANIM_JUMP(60, 0, 0, 5, anim_8012EE54),
};

AnimWord anim_8012EE70[4] = {
    ANIM(61, 0, 0, 5),
    ANIM_STOP(62, 0, 0, 5),
};

void *ptrs_8012EE80[1] = {
    D_8012EECC,
};
