/* GCD (0x8009CD00, size 0x400): global variables. Fields are named by their address (b=u8, s=s16/u16, i=int, p=pointer + low 16 bits of the
   address) until their meaning is known; types are the ones the sources declared. Plain C types, self-contained. */
#ifndef _TOMBA_GCD_H
#define _TOMBA_GCD_H
typedef struct GCDBlock {
    unsigned char _pad0[0x94];   /* 0x0 */
    unsigned short scd94;   /* 0x94 0x8009CD94 */
    unsigned char _pad96[0x2];   /* 0x96 */
    unsigned char bcd98;   /* 0x98 0x8009CD98 */
    unsigned char bcd99;   /* 0x99 0x8009CD99 */
    unsigned char bcd9a;   /* 0x9a 0x8009CD9A */
    unsigned char _pad9b[0x1];   /* 0x9b */
    unsigned char bcd9c;   /* 0x9c 0x8009CD9C */
    unsigned char bcd9d;   /* 0x9d 0x8009CD9D */
    unsigned char bcd9e;   /* 0x9e 0x8009CD9E */
    unsigned char _pad9f[0x3];   /* 0x9f */
    unsigned char bcda2;   /* 0xa2 0x8009CDA2 */
    unsigned char _pada3[0x2];   /* 0xa3 */
    unsigned char bcda5;   /* 0xa5 0x8009CDA5 */
    unsigned char bcda6;   /* 0xa6 0x8009CDA6 */
    unsigned char bcda7;   /* 0xa7 0x8009CDA7 */
    unsigned char _pada8[0x1];   /* 0xa8 */
    unsigned char bcda9;   /* 0xa9 0x8009CDA9 */
    unsigned char _padaa[0x1];   /* 0xaa */
    unsigned char bcdab;   /* 0xab 0x8009CDAB */
    unsigned char bcdac;   /* 0xac 0x8009CDAC */
    unsigned char bcdad;   /* 0xad 0x8009CDAD */
    unsigned char bcdae;   /* 0xae 0x8009CDAE */
    unsigned char _padaf[0x2];   /* 0xaf */
    unsigned char bcdb1;   /* 0xb1 0x8009CDB1 */
    unsigned char bcdb2;   /* 0xb2 0x8009CDB2 */
    unsigned char _padb3[0x1];   /* 0xb3 */
    unsigned char bcdb4;   /* 0xb4 0x8009CDB4 */
    unsigned char bcdb5;   /* 0xb5 0x8009CDB5 */
    unsigned char _padb6[0x2];   /* 0xb6 */
    unsigned char bcdb8;   /* 0xb8 0x8009CDB8 */
    unsigned char _padb9[0x1];   /* 0xb9 */
    unsigned char bcdba;   /* 0xba 0x8009CDBA */
    unsigned char bcdbb;   /* 0xbb 0x8009CDBB */
    unsigned char bcdbc;   /* 0xbc 0x8009CDBC */
    unsigned char _padbd[0x2];   /* 0xbd */
    unsigned char bcdbf;   /* 0xbf 0x8009CDBF */
    unsigned char bcdc0;   /* 0xc0 0x8009CDC0 */
    unsigned char bcdc1;   /* 0xc1 0x8009CDC1 */
    unsigned char _padc2[0x1];   /* 0xc2 */
    unsigned char bcdc3;   /* 0xc3 0x8009CDC3 */
    unsigned char _padc4[0x1];   /* 0xc4 */
    unsigned char bcdc5;   /* 0xc5 0x8009CDC5 */
    unsigned char _padc6[0x1];   /* 0xc6 */
    unsigned char bcdc7;   /* 0xc7 0x8009CDC7 */
    unsigned char bcdc8;   /* 0xc8 0x8009CDC8 */
    unsigned char bcdc9;   /* 0xc9 0x8009CDC9 */
    unsigned char bcdca;   /* 0xca 0x8009CDCA */
    unsigned char bcdcb;   /* 0xcb 0x8009CDCB */
    unsigned char bcdcc;   /* 0xcc 0x8009CDCC */
    unsigned char bcdcd;   /* 0xcd 0x8009CDCD */
    unsigned char bcdce;   /* 0xce 0x8009CDCE */
    unsigned char _padcf[0x3];   /* 0xcf */
    unsigned char bcdd2;   /* 0xd2 0x8009CDD2 */
    unsigned char _padd3[0x2];   /* 0xd3 */
    unsigned char bcdd5;   /* 0xd5 0x8009CDD5 */
    unsigned char bcdd6;   /* 0xd6 0x8009CDD6 */
    unsigned char bcdd7;   /* 0xd7 0x8009CDD7 */
    unsigned char _padd8[0x1];   /* 0xd8 */
    unsigned char bcdd9;   /* 0xd9 0x8009CDD9 */
    unsigned char _padda[0x1];   /* 0xda */
    unsigned char bcddb;   /* 0xdb 0x8009CDDB */
    unsigned char bcddc;   /* 0xdc 0x8009CDDC */
    unsigned char bcddd;   /* 0xdd 0x8009CDDD */
    unsigned char bcdde;   /* 0xde 0x8009CDDE */
    unsigned char _paddf[0x3];   /* 0xdf */
    unsigned char bcde2;   /* 0xe2 0x8009CDE2 */
    unsigned char bcde3;   /* 0xe3 0x8009CDE3 */
    unsigned char _pade4[0x1];   /* 0xe4 */
    unsigned char bcde5;   /* 0xe5 0x8009CDE5 */
    unsigned char _pade6[0x7];   /* 0xe6 */
    unsigned char bcded;   /* 0xed 0x8009CDED */
    unsigned char _padee[0x3];   /* 0xee */
    unsigned char bcdf1;   /* 0xf1 0x8009CDF1 */
    unsigned char bcdf2;   /* 0xf2 0x8009CDF2 */
    unsigned char bcdf3;   /* 0xf3 0x8009CDF3 */
    unsigned char _padf4[0x1];   /* 0xf4 */
    unsigned char bcdf5;   /* 0xf5 0x8009CDF5 */
    unsigned char _padf6[0xf];   /* 0xf6 */
    unsigned char bce05;   /* 0x105 0x8009CE05 */
    unsigned char _pad106[0x4];   /* 0x106 */
    unsigned char bce0a;   /* 0x10a 0x8009CE0A */
    unsigned char bce0b;   /* 0x10b 0x8009CE0B */
    unsigned char bce0c;   /* 0x10c 0x8009CE0C */
    unsigned char bce0d;   /* 0x10d 0x8009CE0D */
    unsigned char _pad10e[0x7];   /* 0x10e */
    unsigned char bce15;   /* 0x115 0x8009CE15 */
    unsigned char bce16;   /* 0x116 0x8009CE16 */
    unsigned char bce17;   /* 0x117 0x8009CE17 */
    unsigned char bce18;   /* 0x118 0x8009CE18 */
    unsigned char _pad119[0x1];   /* 0x119 */
    unsigned char bce1a;   /* 0x11a 0x8009CE1A */
    unsigned char bce1b;   /* 0x11b 0x8009CE1B */
    unsigned char bce1c;   /* 0x11c 0x8009CE1C */
    unsigned char _pad11d[0x2];   /* 0x11d */
    unsigned char bce1f;   /* 0x11f 0x8009CE1F */
    unsigned char _pad120[0x2];   /* 0x120 */
    unsigned char bce22;   /* 0x122 0x8009CE22 */
    unsigned char bce23;   /* 0x123 0x8009CE23 */
    unsigned char bce24;   /* 0x124 0x8009CE24 */
    unsigned char _pad125[0x3];   /* 0x125 */
    unsigned char bce28;   /* 0x128 0x8009CE28 */
    unsigned char _pad129[0xb];   /* 0x129 */
    unsigned char bce34;   /* 0x134 0x8009CE34 */
    unsigned char _pad135[0x5];   /* 0x135 */
    unsigned char bce3a;   /* 0x13a 0x8009CE3A */
    unsigned char bce3b;   /* 0x13b 0x8009CE3B */
    unsigned char _pad13c[0x1];   /* 0x13c */
    unsigned char bce3d;   /* 0x13d 0x8009CE3D */
    unsigned char _pad13e[0x2];   /* 0x13e */
    unsigned char bce40;   /* 0x140 0x8009CE40 */
    unsigned char bce41;   /* 0x141 0x8009CE41 */
    unsigned char _pad142[0x2];   /* 0x142 */
    unsigned char bce44;   /* 0x144 0x8009CE44 */
    unsigned char _pad145[0x3];   /* 0x145 */
    unsigned char bce48;   /* 0x148 0x8009CE48 */
    unsigned char _pad149[0x1];   /* 0x149 */
    unsigned char bce4a;   /* 0x14a 0x8009CE4A */
    unsigned char _pad14b[0x2];   /* 0x14b */
    unsigned char bce4d;   /* 0x14d 0x8009CE4D */
    unsigned char bce4e;   /* 0x14e 0x8009CE4E */
    unsigned char bce4f;   /* 0x14f 0x8009CE4F */
    unsigned char bce50;   /* 0x150 0x8009CE50 */
    unsigned char bce51;   /* 0x151 0x8009CE51 */
    unsigned char bce52;   /* 0x152 0x8009CE52 */
    unsigned char bce53;   /* 0x153 0x8009CE53 */
    unsigned char bce54;   /* 0x154 0x8009CE54 */
    unsigned char bce55;   /* 0x155 0x8009CE55 */
    unsigned char bce56;   /* 0x156 0x8009CE56 */
    unsigned char bce57;   /* 0x157 0x8009CE57 */
    unsigned char bce58;   /* 0x158 0x8009CE58 */
    unsigned char bce59;   /* 0x159 0x8009CE59 */
    unsigned char _pad15a[0x1];   /* 0x15a */
    unsigned char bce5b;   /* 0x15b 0x8009CE5B */
    unsigned char _pad15c[0x1];   /* 0x15c */
    unsigned char bce5d;   /* 0x15d 0x8009CE5D */
    unsigned char _pad15e[0x3];   /* 0x15e */
    unsigned char bce61;   /* 0x161 0x8009CE61 */
    unsigned char bce62;   /* 0x162 0x8009CE62 */
    unsigned char bce63;   /* 0x163 0x8009CE63 */
    unsigned char _pad164[0x47];   /* 0x164 */
    unsigned char bceab;   /* 0x1ab 0x8009CEAB */
    unsigned char _pad1ac[0x1];   /* 0x1ac */
    unsigned char bcead;   /* 0x1ad 0x8009CEAD */
    unsigned char bceae;   /* 0x1ae 0x8009CEAE */
    unsigned char bceaf;   /* 0x1af 0x8009CEAF */
    unsigned char bceb0;   /* 0x1b0 0x8009CEB0 */
    unsigned char _pad1b1[0xb];   /* 0x1b1 */
    unsigned char bcebc;   /* 0x1bc 0x8009CEBC */
    unsigned char bcebd;   /* 0x1bd 0x8009CEBD */
    unsigned char _pad1be[0x1];   /* 0x1be */
    unsigned char bcebf;   /* 0x1bf 0x8009CEBF */
    unsigned char _pad1c0[0x1];   /* 0x1c0 */
    unsigned char bcec1;   /* 0x1c1 0x8009CEC1 */
    unsigned char bcec2;   /* 0x1c2 0x8009CEC2 */
    unsigned char bcec3;   /* 0x1c3 0x8009CEC3 */
    unsigned char bcec4;   /* 0x1c4 0x8009CEC4 */
    unsigned char bcec5;   /* 0x1c5 0x8009CEC5 */
    unsigned char bcec6;   /* 0x1c6 0x8009CEC6 */
    unsigned char bcec7;   /* 0x1c7 0x8009CEC7 */
    unsigned char _pad1c8[0x4];   /* 0x1c8 */
    unsigned char bcecc;   /* 0x1cc 0x8009CECC */
    unsigned char bcecd;   /* 0x1cd 0x8009CECD */
    unsigned char _pad1ce[0x10];   /* 0x1ce */
    unsigned char bcede;   /* 0x1de 0x8009CEDE */
    unsigned char bcedf;   /* 0x1df 0x8009CEDF */
    unsigned char _pad1e0[0x1];   /* 0x1e0 */
    unsigned char bcee1;   /* 0x1e1 0x8009CEE1 */
    unsigned char bcee2;   /* 0x1e2 0x8009CEE2 */
    unsigned char _pad1e3[0xa];   /* 0x1e3 */
    unsigned char bceed;   /* 0x1ed 0x8009CEED */
    unsigned char _pad1ee[0x1];   /* 0x1ee */
    unsigned char bceef;   /* 0x1ef 0x8009CEEF */
    unsigned char bcef0;   /* 0x1f0 0x8009CEF0 */
    unsigned char _pad1f1[0x5];   /* 0x1f1 */
    unsigned char bcef6;   /* 0x1f6 0x8009CEF6 */
    unsigned char bcef7;   /* 0x1f7 0x8009CEF7 */
    unsigned char _pad1f8[0x3];   /* 0x1f8 */
    unsigned char bcefb;   /* 0x1fb 0x8009CEFB */
    unsigned char bcefc;   /* 0x1fc 0x8009CEFC */
    unsigned char _pad1fd[0x3];   /* 0x1fd */
    unsigned char bcf00;   /* 0x200 0x8009CF00 */
    unsigned char _pad201[0x2];   /* 0x201 */
    unsigned char bcf03;   /* 0x203 0x8009CF03 */
    unsigned char bcf04;   /* 0x204 0x8009CF04 */
    unsigned char _pad205[0x1];   /* 0x205 */
    unsigned char bcf06;   /* 0x206 0x8009CF06 */
    unsigned char _pad207[0x3];   /* 0x207 */
    unsigned char bcf0a;   /* 0x20a 0x8009CF0A */
    unsigned char bcf0b;   /* 0x20b 0x8009CF0B */
    unsigned char bcf0c;   /* 0x20c 0x8009CF0C */
    unsigned char bcf0d;   /* 0x20d 0x8009CF0D */
    unsigned char bcf0e;   /* 0x20e 0x8009CF0E */
    unsigned char bcf0f;   /* 0x20f 0x8009CF0F */
    unsigned char bcf10;   /* 0x210 0x8009CF10 */
    unsigned char bcf11;   /* 0x211 0x8009CF11 */
    unsigned char _pad212[0xa];   /* 0x212 */
    unsigned char bcf1c;   /* 0x21c 0x8009CF1C */
    unsigned char _pad21d[0x7];   /* 0x21d */
    unsigned char bcf24;   /* 0x224 0x8009CF24 */
    unsigned char _pad225[0x4];   /* 0x225 */
    unsigned char bcf29;   /* 0x229 0x8009CF29 */
    unsigned char _pad22a[0x3];   /* 0x22a */
    unsigned char bcf2d;   /* 0x22d 0x8009CF2D */
    unsigned char bcf2e;   /* 0x22e 0x8009CF2E */
    unsigned char bcf2f;   /* 0x22f 0x8009CF2F */
    unsigned char _pad230[0x98];   /* 0x230 */
    unsigned char bcfc8;   /* 0x2c8 0x8009CFC8 */
    unsigned char _pad2c9[0x1];   /* 0x2c9 */
    unsigned char bcfca;   /* 0x2ca 0x8009CFCA */
    unsigned char bcfcb;   /* 0x2cb 0x8009CFCB */
    unsigned char bcfcc;   /* 0x2cc 0x8009CFCC */
    unsigned char bcfcd;   /* 0x2cd 0x8009CFCD */
    unsigned char bcfce;   /* 0x2ce 0x8009CFCE */
    unsigned char bcfcf;   /* 0x2cf 0x8009CFCF */
    short scfd0;   /* 0x2d0 0x8009CFD0 */
    short scfd2;   /* 0x2d2 0x8009CFD2 */
    short scfd4;   /* 0x2d4 0x8009CFD4 */
    unsigned char bcfd6;   /* 0x2d6 0x8009CFD6 */
    unsigned char _pad2d7[0x2];   /* 0x2d7 */
    unsigned char bcfd9;   /* 0x2d9 0x8009CFD9 */
    unsigned char bcfda;   /* 0x2da 0x8009CFDA */
    unsigned char bcfdb;   /* 0x2db 0x8009CFDB */
    unsigned char bcfdc;   /* 0x2dc 0x8009CFDC */
    unsigned char bcfdd;   /* 0x2dd 0x8009CFDD */
    unsigned char bcfde;   /* 0x2de 0x8009CFDE */
    unsigned char bcfdf;   /* 0x2df 0x8009CFDF */
    unsigned char bcfe0;   /* 0x2e0 0x8009CFE0 */
    unsigned char bcfe1;   /* 0x2e1 0x8009CFE1 */
    short scfe2;   /* 0x2e2 0x8009CFE2 */
    unsigned char _pad2e4[0x1];   /* 0x2e4 */
    unsigned char bcfe5;   /* 0x2e5 0x8009CFE5 */
    unsigned char _pad2e6[0x4];   /* 0x2e6 */
    unsigned char bcfea;   /* 0x2ea 0x8009CFEA */
    unsigned char _pad2eb[0x2];   /* 0x2eb */
    unsigned char bcfed;   /* 0x2ed 0x8009CFED */
    unsigned char bcfee;   /* 0x2ee 0x8009CFEE */
    unsigned char _pad2ef[0x1];   /* 0x2ef */
    unsigned char bcff0;   /* 0x2f0 0x8009CFF0 */
    unsigned char bcff1;   /* 0x2f1 0x8009CFF1 */
    unsigned char bcff2;   /* 0x2f2 0x8009CFF2 */
    unsigned char bcff3;   /* 0x2f3 0x8009CFF3 */
    unsigned char bcff4;   /* 0x2f4 0x8009CFF4 */
    unsigned char bcff5;   /* 0x2f5 0x8009CFF5 */
    unsigned char _pad2f6[0x1];   /* 0x2f6 */
    unsigned char bcff7;   /* 0x2f7 0x8009CFF7 */
    unsigned char bcff8;   /* 0x2f8 0x8009CFF8 */
    unsigned char bcff9;   /* 0x2f9 0x8009CFF9 */
    unsigned char bcffa;   /* 0x2fa 0x8009CFFA */
    unsigned char bcffb;   /* 0x2fb 0x8009CFFB */
    unsigned char bcffc;   /* 0x2fc 0x8009CFFC */
    unsigned char bcffd;   /* 0x2fd 0x8009CFFD */
    unsigned char _pad2fe[0x2];   /* 0x2fe */
    unsigned char bd000;   /* 0x300 0x8009D000 */
    unsigned char bd001;   /* 0x301 0x8009D001 */
    unsigned char bd002;   /* 0x302 0x8009D002 */
    unsigned char bd003;   /* 0x303 0x8009D003 */
    unsigned char bd004;   /* 0x304 0x8009D004 */
    unsigned char _pad305[0x1];   /* 0x305 */
    unsigned char bd006;   /* 0x306 0x8009D006 */
    unsigned char bd007;   /* 0x307 0x8009D007 */
    unsigned char bd008;   /* 0x308 0x8009D008 */
    unsigned char _pad309[0x1];   /* 0x309 */
    unsigned char bd00a;   /* 0x30a 0x8009D00A */
    unsigned char _pad30b[0x2];   /* 0x30b */
    unsigned char bd00d;   /* 0x30d 0x8009D00D */
    unsigned char bd00e;   /* 0x30e 0x8009D00E */
    unsigned char _pad30f[0x67];   /* 0x30f */
    unsigned char bd076;   /* 0x376 0x8009D076 */
    unsigned char bd077;   /* 0x377 0x8009D077 */
    unsigned char bd078;   /* 0x378 0x8009D078 */
    unsigned char _pad379[0x1];   /* 0x379 */
    unsigned char bd07a;   /* 0x37a 0x8009D07A */
    unsigned char bd07b;   /* 0x37b 0x8009D07B */
    unsigned char bd07c;   /* 0x37c 0x8009D07C */
    unsigned char bd07d;   /* 0x37d 0x8009D07D */
    unsigned char bd07e;   /* 0x37e 0x8009D07E */
    unsigned char _pad37f[0x2];   /* 0x37f */
    unsigned char bd081;   /* 0x381 0x8009D081 */
    unsigned char bd082;   /* 0x382 0x8009D082 */
    unsigned char bd083;   /* 0x383 0x8009D083 */
    unsigned char bd084;   /* 0x384 0x8009D084 */
    unsigned char _pad385[0x1];   /* 0x385 */
    unsigned char bd086;   /* 0x386 0x8009D086 */
    unsigned char bd087;   /* 0x387 0x8009D087 */
    unsigned char bd088;   /* 0x388 0x8009D088 */
    unsigned char _pad389[0x7];   /* 0x389 */
    unsigned char bd090;   /* 0x390 0x8009D090 */
    unsigned char bd091;   /* 0x391 0x8009D091 */
    unsigned char bd092;   /* 0x392 0x8009D092 */
    unsigned char bd093;   /* 0x393 0x8009D093 */
    unsigned char bd094;   /* 0x394 0x8009D094 */
    unsigned char bd095;   /* 0x395 0x8009D095 */
    unsigned char _pad396[0xe];   /* 0x396 */
    unsigned char bd0a4;   /* 0x3a4 0x8009D0A4 */
    unsigned char _pad3a5[0x2];   /* 0x3a5 */
    unsigned char bd0a7;   /* 0x3a7 0x8009D0A7 */
    unsigned char _pad3a8[0x1];   /* 0x3a8 */
    unsigned char bd0a9;   /* 0x3a9 0x8009D0A9 */
    unsigned char bd0aa;   /* 0x3aa 0x8009D0AA */
    unsigned char _pad3ab[0x5];   /* 0x3ab */
    unsigned short sd0b0;   /* 0x3b0 0x8009D0B0 */
    unsigned char bd0b2;   /* 0x3b2 0x8009D0B2 */
    unsigned char _pad3b3[0x1];   /* 0x3b3 */
    unsigned char bd0b4;   /* 0x3b4 0x8009D0B4 */
    unsigned char _pad3b5[0x2];   /* 0x3b5 */
    unsigned char bd0b7;   /* 0x3b7 0x8009D0B7 */
    unsigned char bd0b8;   /* 0x3b8 0x8009D0B8 */
    unsigned char bd0b9;   /* 0x3b9 0x8009D0B9 */
    unsigned char bd0ba;   /* 0x3ba 0x8009D0BA */
    unsigned char bd0bb;   /* 0x3bb 0x8009D0BB */
    unsigned char bd0bc;   /* 0x3bc 0x8009D0BC */
    unsigned char _pad3bd[0x1];   /* 0x3bd */
    unsigned char bd0be;   /* 0x3be 0x8009D0BE */
    unsigned char bd0bf;   /* 0x3bf 0x8009D0BF */
    unsigned char bd0c0;   /* 0x3c0 0x8009D0C0 */
    unsigned char bd0c1;   /* 0x3c1 0x8009D0C1 */
    unsigned char _pad3c2[0x2];   /* 0x3c2 */
    unsigned char bd0c4;   /* 0x3c4 0x8009D0C4 */
    unsigned char bd0c5;   /* 0x3c5 0x8009D0C5 */
    unsigned char bd0c6;   /* 0x3c6 0x8009D0C6 */
    unsigned char _pad3c7[0x1];   /* 0x3c7 */
    unsigned char bd0c8;   /* 0x3c8 0x8009D0C8 */
    unsigned char bd0c9;   /* 0x3c9 0x8009D0C9 */
    unsigned char bd0ca;   /* 0x3ca 0x8009D0CA */
    unsigned char bd0cb;   /* 0x3cb 0x8009D0CB */
    unsigned char bd0cc;   /* 0x3cc 0x8009D0CC */
    unsigned char bd0cd;   /* 0x3cd 0x8009D0CD */
    unsigned char bd0ce;   /* 0x3ce 0x8009D0CE */
    unsigned char bd0cf;   /* 0x3cf 0x8009D0CF */
    unsigned char bd0d0;   /* 0x3d0 0x8009D0D0 */
    unsigned char bd0d1;   /* 0x3d1 0x8009D0D1 */
    unsigned char _pad3d2[0x7];   /* 0x3d2 */
    unsigned char bd0d9;   /* 0x3d9 0x8009D0D9 */
    unsigned char bd0da;   /* 0x3da 0x8009D0DA */
    unsigned char _pad3db[0x7];   /* 0x3db */
    unsigned char bd0e2;   /* 0x3e2 0x8009D0E2 */
    unsigned char _pad3e3[0x3];   /* 0x3e3 */
    unsigned char bd0e6;   /* 0x3e6 0x8009D0E6 */
    unsigned char bd0e7;   /* 0x3e7 0x8009D0E7 */
    unsigned char _pad3e8[0x13];   /* 0x3e8 */
    unsigned char bd0fb;   /* 0x3fb 0x8009D0FB */
    unsigned char bd0fc;   /* 0x3fc 0x8009D0FC */
    unsigned char bd0fd;   /* 0x3fd 0x8009D0FD */
    unsigned char _pad3fe[0x2];   /* 0x3fe */
} GCDBlock;
extern GCDBlock GCD;
#endif
