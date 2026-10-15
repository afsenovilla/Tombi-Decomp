// DATA 8013de5c 1812 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013EA54[];
extern char D_8013EA5C[];
extern char D_8013EBE8[];
extern char D_8013EBF0[];
extern char D_8013EC38[];
extern char D_8013EE08[];
extern char D_8013EE10[];
extern char D_8013EE30[];
extern char D_8013EE38[];
extern char D_8013EF68[];
extern char D_8013EFA0[];
extern char D_8013EFA8[];
extern char D_8013EFB0[];
extern char D_8013EFB8[];
extern char D_8013EFC0[];
extern char D_8013EFC8[];
extern char D_8013EFEC[];
extern char D_8013F010[];
extern char D_8013F018[];
extern char D_8013F020[];
extern AnimWord anim_8013DE5C[];
extern AnimWord anim_8013DE80[];
extern AnimWord anim_8013DEC4[];
extern AnimWord anim_8013DEE4[];
extern AnimWord anim_8013DF04[];
extern AnimWord anim_8013DF0C[];
extern AnimWord anim_8013DF30[];
extern AnimWord anim_8013DF48[];
extern AnimWord anim_8013DF7C[];
extern AnimWord anim_8013DF84[];
extern AnimWord anim_8013DFE4[];
extern AnimWord anim_8013DFF4[];
extern AnimWord anim_8013DFFC[];
extern AnimWord anim_8013E004[];
extern AnimWord anim_8013E00C[];
extern AnimWord anim_8013E02C[];
extern AnimWord anim_8013E034[];
extern AnimWord anim_8013E054[];
extern AnimWord anim_8013E05C[];
extern AnimWord anim_8013E064[];
extern AnimWord anim_8013E06C[];
extern AnimWord anim_8013E074[];
extern AnimWord anim_8013E07C[];
extern AnimWord anim_8013E084[];
extern AnimWord anim_8013E08C[];
extern AnimWord anim_8013E0D0[];
extern AnimWord anim_8013E0F0[];
extern AnimWord anim_8013E0F8[];
extern AnimWord anim_8013E118[];
extern AnimWord anim_8013E138[];
extern AnimWord anim_8013E158[];
extern AnimWord anim_8013E168[];
extern AnimWord anim_8013E188[];
extern AnimWord anim_8013E190[];
extern AnimWord anim_8013E198[];
extern AnimWord anim_8013E1A0[];
extern AnimWord anim_8013E1E4[];
extern AnimWord anim_8013E204[];
extern AnimWord anim_8013E20C[];
extern AnimWord anim_8013E22C[];
extern AnimWord anim_8013E250[];
extern AnimWord anim_8013E270[];
extern AnimWord anim_8013E278[];
extern AnimWord anim_8013E280[];
extern AnimWord anim_8013E288[];
extern AnimWord anim_8013E290[];
extern AnimWord anim_8013E2D4[];
extern AnimWord anim_8013E2F4[];
extern AnimWord anim_8013E30C[];
extern AnimWord anim_8013E33C[];
extern AnimWord anim_8013E35C[];
extern AnimWord anim_8013E37C[];
extern AnimWord anim_8013E3DC[];
extern AnimWord anim_8013E3E4[];
extern AnimWord anim_8013E3F8[];
extern AnimWord anim_8013E418[];
extern AnimWord anim_8013E420[];
extern AnimWord anim_8013E428[];
extern AnimWord anim_8013E458[];
extern AnimWord anim_8013E460[];
extern AnimWord anim_8013E468[];
extern AnimWord anim_8013E470[];
extern AnimWord anim_8013E494[];
extern AnimWord anim_8013E4F4[];
extern AnimWord anim_8013E4FC[];
extern AnimWord anim_8013E504[];
extern AnimWord anim_8013E50C[];
extern void *ptrs_8013E514[];
extern void *ptrs_8013E51C[];
extern void *ptrs_8013E524[];
extern void *ptrs_8013E528[];
extern void *ptrs_8013E530[];
extern void *ptrs_8013E538[];
extern void *ptrs_8013E540[];
extern void *ptrs_8013E544[];
extern void *ptrs_8013E548[];
extern void *ptrs_8013E558[];
extern void *ptrs_8013E568[];

AnimWord anim_8013DE5C[9] = {
    ANIM(29, 0, 0, 10),
    ANIM(30, 0, 0, 6),
    ANIM(31, 0, 0, 10),
    ANIM_JUMP(30, 0, 0, 6, anim_8013DE5C),
};

AnimWord anim_8013DE80[17] = {
    ANIM(38, 0, 0, 3),
    ANIM(39, 0, 0, 2),
    ANIM(40, 0, 0, 2),
    ANIM(41, 0, 0, 2),
    ANIM(48, 0, 0, 3),
    ANIM(49, 0, 0, 2),
    ANIM(50, 0, 0, 2),
    ANIM_JUMP(51, 0, 0, 2, anim_8013DE80),
};

AnimWord anim_8013DEC4[8] = {
    ANIM(52, 0, 0, 40),
    ANIM(53, 0, 0, 8),
    ANIM(54, 0, 0, 40),
    ANIM_STOP(53, 0, 0, 8),
};

AnimWord anim_8013DEE4[8] = {
    ANIM(55, 0, 0, 40),
    ANIM(56, 0, 0, 8),
    ANIM(57, 0, 0, 40),
    ANIM_STOP(56, 0, 0, 8),
};

AnimWord anim_8013DF04[2] = {
    ANIM_STOP(73, 0, 0, 60),
};

AnimWord anim_8013DF0C[9] = {
    ANIM(69, 0, 0, 20),
    ANIM(70, 0, 0, 5),
    ANIM(71, 0, 0, 20),
    ANIM_JUMP(70, 0, 0, 5, anim_8013DF0C),
};

AnimWord anim_8013DF30[6] = {
    ANIM(58, 0, 0, 6),
    ANIM(59, 0, 0, 10),
    ANIM_STOP(60, 0, 0, 60),
};

AnimWord anim_8013DF48[13] = {
    ANIM(61, 0, 0, 24),
    ANIM(62, 0, 0, 6),
    ANIM(63, 0, 0, 4),
    ANIM(64, 0, 0, 24),
    ANIM(63, 0, 0, 6),
    ANIM_JUMP(62, 0, 0, 4, anim_8013DF48),
};

AnimWord anim_8013DF7C[2] = {
    ANIM_STOP(72, 0, 0, 60),
};

AnimWord anim_8013DF84[24] = {
    ANIM(65, 0, 0, 4),
    ANIM(66, 0, 0, 6),
    ANIM(67, 0, 0, 12),
    ANIM(68, 0, 0, 6),
    ANIM(65, 0, 0, 4),
    ANIM(66, 0, 0, 6),
    ANIM(67, 0, 0, 12),
    ANIM(68, 0, 0, 6),
    ANIM(65, 0, 0, 4),
    ANIM(66, 0, 0, 6),
    ANIM(67, 0, 0, 12),
    ANIM_STOP(68, 0, 0, 6),
};

AnimWord anim_8013DFE4[4] = {
    ANIM(74, 0, 0, 10),
    ANIM_STOP(75, 0, 0, 24),
};

AnimWord anim_8013DFF4[2] = {
    ANIM_STOP(76, 0, 0, 8),
};

AnimWord anim_8013DFFC[2] = {
    ANIM_STOP(78, 0, 0, 8),
};

AnimWord anim_8013E004[2] = {
    ANIM_STOP(80, 0, 0, 60),
};

AnimWord anim_8013E00C[8] = {
    ANIM(81, 0, 0, 8),
    ANIM(83, 0, 0, 8),
    ANIM(84, 0, 0, 8),
    ANIM_STOP(85, 0, 0, 8),
};

AnimWord anim_8013E02C[2] = {
    ANIM_STOP(86, 0, 0, 60),
};

AnimWord anim_8013E034[8] = {
    ANIM(113, 0, 0, 20),
    ANIM(114, 0, 0, 5),
    ANIM(115, 0, 0, 20),
    ANIM_STOP(114, 0, 0, 5),
};

AnimWord anim_8013E054[2] = {
    ANIM_STOP(116, 0, 0, 60),
};

AnimWord anim_8013E05C[2] = {
    ANIM_STOP(117, 0, 0, 60),
};

AnimWord anim_8013E064[2] = {
    ANIM_STOP(118, 0, 0, 60),
};

AnimWord anim_8013E06C[2] = {
    ANIM_STOP(156, 0, 0, 60),
};

AnimWord anim_8013E074[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8013E07C[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8013E084[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8013E08C[17] = {
    ANIM(3, 0, 0, 7),
    ANIM(4, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM(6, 0, 0, 7),
    ANIM(7, 0, 0, 7),
    ANIM(6, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM_JUMP(4, 0, 0, 7, anim_8013E08C),
};

AnimWord anim_8013E0D0[8] = {
    ANIM(8, 0, 0, 20),
    ANIM(9, 0, 0, 5),
    ANIM(10, 0, 0, 20),
    ANIM_STOP(9, 0, 0, 5),
};

AnimWord anim_8013E0F0[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8013E0F8[8] = {
    ANIM(12, 0, 0, 40),
    ANIM(13, 0, 0, 8),
    ANIM(14, 0, 0, 40),
    ANIM_STOP(13, 0, 0, 8),
};

AnimWord anim_8013E118[8] = {
    ANIM(15, 0, 0, 40),
    ANIM(16, 0, 0, 8),
    ANIM(17, 0, 0, 40),
    ANIM_STOP(16, 0, 0, 8),
};

AnimWord anim_8013E138[8] = {
    ANIM(18, 0, 0, 4),
    ANIM(19, 0, 0, 6),
    ANIM(20, 0, 0, 12),
    ANIM_STOP(19, 0, 0, 6),
};

AnimWord anim_8013E158[4] = {
    ANIM(22, 0, 0, 10),
    ANIM_STOP(23, 0, 0, 24),
};

AnimWord anim_8013E168[8] = {
    ANIM(24, 0, 0, 10),
    ANIM(25, 0, 0, 6),
    ANIM(26, 0, 0, 10),
    ANIM_STOP(25, 0, 0, 6),
};

AnimWord anim_8013E188[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_8013E190[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8013E198[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_8013E1A0[17] = {
    ANIM(30, 0, 0, 7),
    ANIM(31, 0, 0, 7),
    ANIM(32, 0, 0, 7),
    ANIM(33, 0, 0, 7),
    ANIM(34, 0, 0, 7),
    ANIM(35, 0, 0, 7),
    ANIM(36, 0, 0, 7),
    ANIM_JUMP(37, 0, 0, 7, anim_8013E1A0),
};

AnimWord anim_8013E1E4[8] = {
    ANIM(38, 0, 0, 20),
    ANIM(39, 0, 0, 5),
    ANIM(40, 0, 0, 20),
    ANIM_STOP(39, 0, 0, 5),
};

AnimWord anim_8013E204[2] = {
    ANIM_STOP(41, 0, 0, 60),
};

AnimWord anim_8013E20C[8] = {
    ANIM(42, 0, 0, 40),
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 40),
    ANIM_STOP(43, 0, 0, 8),
};

AnimWord anim_8013E22C[9] = {
    ANIM(45, 0, 0, 40),
    ANIM(46, 0, 0, 8),
    ANIM(47, 0, 0, 40),
    ANIM_JUMP(46, 0, 0, 8, anim_8013E22C),
};

AnimWord anim_8013E250[8] = {
    ANIM(48, 0, 0, 4),
    ANIM(49, 0, 0, 6),
    ANIM(50, 0, 0, 12),
    ANIM_STOP(51, 0, 0, 6),
};

AnimWord anim_8013E270[2] = {
    ANIM_STOP(52, 0, 0, 60),
};

AnimWord anim_8013E278[2] = {
    ANIM_STOP(53, 0, 0, 60),
};

AnimWord anim_8013E280[2] = {
    ANIM_STOP(54, 0, 0, 60),
};

AnimWord anim_8013E288[2] = {
    ANIM_STOP(55, 0, 0, 60),
};

AnimWord anim_8013E290[17] = {
    ANIM(56, 0, 0, 3),
    ANIM(57, 0, 0, 2),
    ANIM(58, 0, 0, 2),
    ANIM(59, 0, 0, 2),
    ANIM(60, 0, 0, 3),
    ANIM(61, 0, 0, 2),
    ANIM(62, 0, 0, 2),
    ANIM_JUMP(63, 0, 0, 2, anim_8013E290),
};

AnimWord anim_8013E2D4[8] = {
    ANIM(64, 0, 0, 20),
    ANIM(65, 0, 0, 5),
    ANIM(66, 0, 0, 20),
    ANIM_STOP(65, 0, 0, 5),
};

AnimWord anim_8013E2F4[6] = {
    ANIM(81, 0, 0, 6),
    ANIM(82, 0, 0, 10),
    ANIM_STOP(83, 0, 0, 60),
};

AnimWord anim_8013E30C[12] = {
    ANIM(67, 0, 0, 24),
    ANIM(68, 0, 0, 6),
    ANIM(69, 0, 0, 4),
    ANIM(70, 0, 0, 24),
    ANIM(69, 0, 0, 6),
    ANIM_STOP(68, 0, 0, 4),
};

AnimWord anim_8013E33C[8] = {
    ANIM(71, 0, 0, 40),
    ANIM(72, 0, 0, 8),
    ANIM(73, 0, 0, 40),
    ANIM_STOP(72, 0, 0, 8),
};

AnimWord anim_8013E35C[8] = {
    ANIM(74, 0, 0, 40),
    ANIM(75, 0, 0, 8),
    ANIM(76, 0, 0, 40),
    ANIM_STOP(75, 0, 0, 8),
};

AnimWord anim_8013E37C[24] = {
    ANIM(77, 0, 0, 4),
    ANIM(78, 0, 0, 6),
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

AnimWord anim_8013E3DC[2] = {
    ANIM_STOP(84, 0, 0, 60),
};

AnimWord anim_8013E3E4[5] = {
    ANIM(85, 0, 0, 10),
    ANIM_JUMP(86, 0, 0, 24, anim_8013E3E4),
};

AnimWord anim_8013E3F8[8] = {
    ANIM(87, 0, 0, 10),
    ANIM(88, 0, 0, 6),
    ANIM(89, 0, 0, 10),
    ANIM_STOP(88, 0, 0, 6),
};

AnimWord anim_8013E418[2] = {
    ANIM_STOP(95, 0, 0, 8),
};

AnimWord anim_8013E420[2] = {
    ANIM_STOP(96, 0, 0, 8),
};

AnimWord anim_8013E428[12] = {
    ANIM(90, 0, 0, 24),
    ANIM(91, 0, 0, 4),
    ANIM(92, 0, 0, 3),
    ANIM(93, 0, 0, 2),
    ANIM(94, 0, 0, 16),
    ANIM_STOP(91, 0, 0, 4),
};

AnimWord anim_8013E458[2] = {
    ANIM_STOP(99, 0, 0, 8),
};

AnimWord anim_8013E460[2] = {
    ANIM_STOP(100, 0, 0, 8),
};

AnimWord anim_8013E468[2] = {
    ANIM_STOP(101, 0, 0, 8),
};

AnimWord anim_8013E470[9] = {
    ANIM(102, 0, 0, 6),
    ANIM(103, 0, 0, 8),
    ANIM(104, 0, 0, 6),
    ANIM_JUMP(105, 0, 0, 8, anim_8013E470),
};

AnimWord anim_8013E494[24] = {
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

AnimWord anim_8013E4F4[2] = {
    ANIM_STOP(110, 0, 0, 60),
};

AnimWord anim_8013E4FC[2] = {
    ANIM_STOP(111, 0, 0, 60),
};

AnimWord anim_8013E504[2] = {
    ANIM_STOP(112, 0, 0, 60),
};

AnimWord anim_8013E50C[2] = {
    ANIM_STOP(106, 0, 0, 60),
};

void *ptrs_8013E514[2] = {
    D_8013EFA0,
    D_8013EFA8,
};

void *ptrs_8013E51C[2] = {
    D_8013EFB0,
    D_8013EFB8,
};

void *ptrs_8013E524[1] = {
    D_8013EFC0,
};

void *ptrs_8013E528[2] = {
    D_8013EFC8,
    D_8013EFEC,
};

void *ptrs_8013E530[2] = {
    D_8013F010,
    D_8013F018,
};

void *ptrs_8013E538[2] = {
    D_8013F020,
    D_8013EBE8,
};

void *ptrs_8013E540[1] = {
    D_8013EBF0,
};

void *ptrs_8013E544[1] = {
    D_8013EC38,
};

void *ptrs_8013E548[4] = {
    D_8013EE38,
    D_8013EE08,
    D_8013EE10,
    D_8013EE30,
};

void *ptrs_8013E558[4] = {
    D_8013EF68,
    D_8013EF68,
    D_8013EF68,
    D_8013EF68,
};

void *ptrs_8013E568[2] = {
    D_8013EA54,
    D_8013EA5C,
};
