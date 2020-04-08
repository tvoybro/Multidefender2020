// Multimatograf 2020 invitro

#include "Include/neslib.h"
#include "Include/nesdoug.h"
#include "Include/font4x4.h"

#include "Gfx/NAM_multi_logo_A.h"
#include "Gfx/NAM_multi_logo_B.h"
#include "Gfx/logo_scr.h"
#include "Gfx/kruj_nametable.h"

#define COVIDS_MAX				6

#define SFX_SHOT 				0x00
#define SFX_COVID_ELIMINATED 	0x01
#define SFX_COVID_RESPAWN		0x02
#define SFX_TELEGA_FLY			0x03

#define EQOFFSET 				0x20
#define EQ_CHR_OFF 				0xCD
#define EQ_CHR_ON 				0xA8

#define STARSHIP_AUTOPILOT 		0b10000000

unsigned char isNtsc;

extern char scrollText[];

extern unsigned int FT_MUSPOS;
#pragma zpsym ("FT_MUSPOS")

extern unsigned char FT_BUF[];

unsigned char tileset;
unsigned int muspos;


const unsigned char palBlink[16]={
	0x30,0x30,0x30,0x30,
	0x30,0x30,0x30,0x30,
	0x30,0x30,0x30,0x30,
	0x30,0x30,0x30,0x30
};

unsigned char paletteId = 0;
unsigned char paletteIn[6][16]={
	{0x0F,0x0F,0x0F,0x0F,
	 0x0F,0x30,0x0F,0x0F,
	 0x0F,0x30,0x0F,0x0F,
	 0x0F,0x0F,0x0F,0x0F},
	{0x02,0x0F,0x0F,0x0F,
	 0x02,0x30,0x0F,0x0F,
	 0x02,0x30,0x0F,0x0F,
	 0x02,0x0F,0x0F,0x0F},
	{0x12,0x0F,0x0F,0x0F,
	 0x12,0x30,0x0F,0x0F,
	 0x12,0x30,0x01,0x0F,
	 0x12,0x0F,0x0F,0x0F},
	{0x22,0x0F,0x0F,0x0F,
	 0x22,0x36,0x0F,0x0F,
	 0x22,0x36,0x11,0x0F,
	 0x22,0x0A,0x0F,0x0F},
	{0x32,0x02,0x03,0x0F,
	 0x32,0x26,0x03,0x02,
	 0x32,0x26,0x21,0x02,
	 0x32,0x1A,0x0A,0x0F},
	{0x30,0x22,0x13,0x01,
	 0x30,0x16,0x13,0x22,
	 0x30,0x16,0x31,0x22,
	 0x30,0x2A,0x1A,0x01},
};


unsigned char palette[16]={
	0x30,0x22,0x13,0x01,
	0x30,0x16,0x13,0x22,
	0x30,0x16,0x31,0x22,
	0x30,0x2A,0x1A,0x01
};

unsigned char i = 0;
unsigned char j = 0;

unsigned char palRollId1 = 0;
unsigned char palRollId2 = 1;
const unsigned char palRollList[48] = {
0x21,
0x22,
0x22,
0x23,
0x23,
0x24,
0x24,
0x25,
0x25,
0x26,
0x26,
0x27,
0x27,
0x28,
0x28,
0x29,
0x29,
0x2A,
0x2A,
0x2B,
0x2B,
0x2C,
0x2C,
0x3C,
0x3C,
0x3B,
0x3B,
0x3A,
0x3A,
0x39,
0x39,
0x38,
0x38,
0x37,
0x37,
0x36,
0x36,
0x35,
0x35,
0x34,
0x34,
0x33,
0x33,
0x32,
0x32,
0x31,
0x31,
0x21
};


unsigned char paletteSprId = 0;
unsigned char palette_spr[5][16]={
	{0x0f,0x00,0x06,0x10,0x0f,0x0c,0x0c,0x0c,0x0f,0x0f,0x21,0x30,0x21,0x14,0x24,0x30},
	{0x0f,0x10,0x06,0x36,0x0f,0x0c,0x0c,0x0c,0x0f,0x0f,0x21,0x30,0x21,0x14,0x24,0x30},
	{0x0f,0x27,0x06,0x30,0x0f,0x0c,0x0c,0x0c,0x0f,0x0f,0x21,0x30,0x21,0x14,0x24,0x30},
	{0x0f,0x27,0x16,0x30,0x0f,0x0c,0x0c,0x0c,0x0f,0x0f,0x21,0x30,0x21,0x14,0x24,0x30},
	{0x0f,0x38,0x26,0x30,0x0f,0x0c,0x0c,0x0c,0x0f,0x0f,0x21,0x30,0x21,0x14,0x24,0x30},
};
unsigned char palSamoletId = 0;
const unsigned char palSamolet[2] = {0x24, 0x1e};

const unsigned char palNesdev2[3][16] = {
	{0x0F,0x12,0x22,0x30,
	0x0F,0x32,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},

	{0x0F,0x32,0x22,0x30,
	0x0F,0x02,0x32,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},

	{0x0F,0x12,0x32,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},
};

const unsigned char palNesdev[15][16] = {
	//fade_in-0
	{0x0F,0x32,0x32,0x32,
	0x0F,0x32,0x32,0x32,
	0x0F,0x0F,0x32,0x32,
	0x0F,0x0F,0x0F,0x0F},
	//fade_in-1
	{0x0F,0x30,0x30,0x30,
	0x0F,0x30,0x30,0x30,
	0x0F,0x0F,0x30,0x30,
	0x0F,0x0F,0x0F,0x0F},
	//fade_in-2
	{0x0F,0x30,0x30,0x30,
	0x0F,0x32,0x30,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},
	//fade_in-3
	{0x0F,0x32,0x30,0x30,
	0x0F,0x22,0x32,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},
	//fade_in-4
	{0x0F,0x22,0x32,0x30,
	0x0F,0x12,0x22,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},
	//main-5
	{0x0F,0x12,0x22,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x0F},

	//text-in-6
	{0x0F,0x12,0x22,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x0F,0x0F,0x02},
	//text-in-7
	{0x0F,0x12,0x22,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x02,0x02,0x12},
	//text-in-8
	{0x0F,0x12,0x22,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x12,0x12,0x22},
	//text-in-9
	{0x0F,0x12,0x22,0x30,
	0x0F,0x02,0x12,0x30,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x00,0x10,0x30},
	
	
	//fadeout-10
	{0x0F,0x12,0x22,0x32,
	0x0F,0x02,0x12,0x32,
	0x0F,0x0F,0x22,0x32,
	0x0F,0x22,0x22,0x32},
	//fadeout-11
	{0x0F,0x02,0x12,0x22,
	0x0F,0x0F,0x02,0x22,
	0x0F,0x0F,0x12,0x22,
	0x0F,0x12,0x12,0x22},
	//fadeout-12
	{0x0F,0x0F,0x02,0x12,
	0x0F,0x0F,0x0F,0x12,
	0x0F,0x0F,0x02,0x12,
	0x0F,0x02,0x02,0x12},
	//fadeout-13
	{0x0F,0x0F,0x0F,0x02,
	0x0F,0x0F,0x0F,0x02,
	0x0F,0x0F,0x0F,0x02,
	0x0F,0x0F,0x0F,0x02},
	//fadeout-14
	{0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F},
};

/*const unsigned char spr_covid_19[]={
	  0,  0,0x02,0,
	  8,  0,0x03,0,
	 16,  0,0x04,0,
	  0,  8,0x12,0,
	  8,  8,0x13,0,
	 16,  8,0x14,0,
	  0, 16,0x22,0,
	  8, 16,0x23,0,
	 16, 16,0x24,0,
	128
};
*/

const unsigned char covid19_0_data[]={

	  4,  4,0x02,0,
	 12,  4,0x03,0,
	 20,  4,0x04,0,
	  4, 12,0x12,0,
	 12, 12,0x13,0,
	 20, 12,0x14,0,
	  4, 20,0x22,0,
	 12, 20,0x23,0,
	 20, 20,0x24,0,
	128

};

const unsigned char covid19_1_data[]={

	  4,  4,0x05,0,
	 12,  4,0x06,0,
	 20,  4,0x07,0,
	  4, 12,0x15,0,
	 12, 12,0x16,0,
	 20, 12,0x17,0,
	  4, 20,0x25,0,
	 12, 20,0x26,0,
	 20, 20,0x27,0,
	128

};

const unsigned char covid19_2_data[]={

	  4,  4,0x08,0,
	 12,  4,0x09,0,
	 20,  4,0x0a,0,
	  4, 12,0x18,0,
	 12, 12,0x19,0,
	 20, 12,0x1a,0,
	  4, 20,0x28,0,
	 12, 20,0x29,0,
	 20, 20,0x2a,0,
	128

};

const unsigned char* const seq_covid19[]={

	covid19_0_data,
	covid19_1_data,
	covid19_2_data

};

const unsigned char covid_explode_0_data[]={

	  8,  8,0x7d,2,
	128

};

const unsigned char covid_explode_1_data[]={

	  4,  4,0xfe,2,
	 12,  4,0xfe,2|OAM_FLIP_H,
	  4, 12,0xfe,2|OAM_FLIP_V,
	 12, 12,0xfe,2|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_2_data[]={

	  4,  4,0xff,2,
	 12,  4,0xf0,2,
	  4, 12,0xf1,2,
	 12, 12,0xf2,2,
	128

};

const unsigned char covid_explode_3_data[]={

	  4,  4,0xf3,2,
	 12,  4,0xf4,2,
	  4, 12,0xf5,2,
	 12, 12,0xf6,2,
	128

};

const unsigned char covid_explode_4_data[]={

	  4,  4,0xf2,2|OAM_FLIP_H|OAM_FLIP_V,
	 12,  4,0xf1,2|OAM_FLIP_H|OAM_FLIP_V,
	  4, 12,0xf0,2|OAM_FLIP_H|OAM_FLIP_V,
	 12, 12,0xff,2|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_5_data[]={

	 12, 12,0xf3,2|OAM_FLIP_H|OAM_FLIP_V,
	  4, 12,0xf4,2|OAM_FLIP_H|OAM_FLIP_V,
	 12,  4,0xf5,2|OAM_FLIP_H|OAM_FLIP_V,
	  4,  4,0xf6,2|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_6_data[]={

	  4,  4,0xf7,2,
	 12,  4,0xf8,2,
	  4, 12,0xf9,2,
	 12, 12,0xfa,2,
	128

};

const unsigned char covid_explode_7_data[]={

	  8,  8,0xfb,2,
	128

};

const unsigned char covid_explode_8_data[]={

	  8,  8,0xfc,2,
	128

};

const unsigned char* const covid_explode[]={
	covid_explode_0_data,
	covid_explode_0_data,
	covid_explode_0_data,
	covid_explode_0_data,
	covid_explode_0_data,
	covid_explode_1_data,
	covid_explode_1_data,
	covid_explode_1_data,
	covid_explode_1_data,
	covid_explode_2_data,
	covid_explode_2_data,
	covid_explode_2_data,
	covid_explode_2_data,
	covid_explode_3_data,
	covid_explode_3_data,
	covid_explode_3_data,
	covid_explode_3_data,
	covid_explode_4_data,
	covid_explode_4_data,
	covid_explode_4_data,
	covid_explode_4_data,
	covid_explode_5_data,
	covid_explode_5_data,
	covid_explode_5_data,
	covid_explode_5_data,
	covid_explode_6_data,
	covid_explode_6_data,
	covid_explode_6_data,
	covid_explode_6_data,
	covid_explode_7_data,
	covid_explode_7_data,
	covid_explode_7_data,
	covid_explode_7_data,
	covid_explode_8_data,
	covid_explode_8_data,
	covid_explode_8_data,
	covid_explode_8_data

};


const unsigned char spr_starship[]={

	- 8,- 8,0x82,3,
	  0,- 8,0x83,3,
	- 8,  0,0x92,3,
	  0,  0,0x93,3,
	128

};

const unsigned char const starship_pal[] = {
	0x17, 0x27, 0x05, 0x27
};

unsigned char starship_x, starship_y, starship_state, starship_toX, starship_pause, starship_flame;
unsigned char bullet_x, bullet_y;

const char scrollerData[] = "HELLO WORLD! BONJOUR LE MONDE! HALO A SHAOGHAIL! SALVE MUNDI SINT! HELLO VILAG! KAUPAPA HUA! CIAO MONDO! HEJ VERDEN! SAWUBONA MHLABA! SVEIKA PASAULE! HALO DUNIA! SALU MUNDU! DOMHAN HELLO! HOLA MUNDO! ... END OF SCROLLER ...              ONCE AGAIN:";


const unsigned char bus_conflict[4]={ 0x00,0x01,0x02,0x03 };
void cnrom_set_bank(unsigned char bank)
{
	unsigned char *ptr;
	
	bank&=3;	//есть платы, у которых больше 4 банков, например 16, но для простоты пока так
	
	ptr=(unsigned char*)&bus_conflict[bank];
	
	*ptr=bank;
}


unsigned char _fs = 0;
unsigned char scrollPointer = 0;
unsigned char scrollSym = 0;
unsigned char scrollerCharPos = 0;
unsigned char scrollerCharIndex = 0;
unsigned int scrollerPos = 0;
unsigned int scrollerAddr = 0;

unsigned int scrollpos = 0;
unsigned char logoPos, logoX, nesclock = 0;

// -----------------------------------------------------------------------------------------------------------------

unsigned char eq_Pulse1_Volume = 0;		// vol = FT_BUF[0]&0b00001111 -> 00 min 0f max -> table offset = vol div 3
unsigned char eq_Pulse2_Volume = 0;		// FT_BUF[3]&0b00001111 -> 00 min 0f max
unsigned char eq_Triangle_Volume = 0;			// FT_BUF[6]&0b00001111 -> 0f enabled 00 disabled
unsigned char eq_Noise_Volume = 0;		// FT_BUF[9]&0b00001111 -> 00 min 0f max
unsigned char eq_Noise_Val = 0;		// FT_BUF[9]&0b00001111 -> 00 min 0f max
unsigned char eq_Noise_Val_prev = 0;		// FT_BUF[9]&0b00001111 -> 00 min 0f max
unsigned char spr;
unsigned int nt_Offset;
unsigned char i, j, eq_Tile, pad, pad_prev;
unsigned char eqValues[4][5]={
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};
const unsigned int eq_pulse1_approx[15] = {
	0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06
};

const unsigned int eq_noise_approx[15] = {
	0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x06
};

// -----------------------------------------------------------------------------------------------------------------
const unsigned int eq_Tri_NT[5] = {
	NAMETABLE_A-EQOFFSET+0x28e, NAMETABLE_A-EQOFFSET+0x24e, NAMETABLE_A-EQOFFSET+0x20e, NAMETABLE_A-EQOFFSET+0x1ce, NAMETABLE_A-EQOFFSET+0x18e
};

const unsigned int eq_Noise_NT[6] = {
	NAMETABLE_A-EQOFFSET+0x28c, NAMETABLE_A-EQOFFSET+0x24c, NAMETABLE_A-EQOFFSET+0x20c, NAMETABLE_A-EQOFFSET+0x1cc, NAMETABLE_A-EQOFFSET+0x18c, NAMETABLE_A-EQOFFSET+0x14c
};

const unsigned int eq_Pulse1left_NT[7] = {
	NAMETABLE_A-EQOFFSET+0x28a, NAMETABLE_A-EQOFFSET+0x24a, NAMETABLE_A-EQOFFSET+0x20a, NAMETABLE_A-EQOFFSET+0x1ca, NAMETABLE_A-EQOFFSET+0x18a, NAMETABLE_A-EQOFFSET+0x14a, NAMETABLE_A-EQOFFSET+0x10a
};
const unsigned int eq_Pulse1right_NT[7] = {
	NAMETABLE_A-EQOFFSET+0x294, NAMETABLE_A-EQOFFSET+0x254, NAMETABLE_A-EQOFFSET+0x214, NAMETABLE_A-EQOFFSET+0x1d4, NAMETABLE_A-EQOFFSET+0x194, NAMETABLE_A-EQOFFSET+0x154, NAMETABLE_A-EQOFFSET+0x114
};

const unsigned int eq_Pulse2left_NT[7] = {
	NAMETABLE_A-EQOFFSET+0x248, NAMETABLE_A-EQOFFSET+0x208, NAMETABLE_A-EQOFFSET+0x1c8, NAMETABLE_A-EQOFFSET+0x188, NAMETABLE_A-EQOFFSET+0x148, NAMETABLE_A-EQOFFSET+0x108, NAMETABLE_A-EQOFFSET+0x0c8
};

const unsigned int eq_Pulse2right_NT[7] = {
	NAMETABLE_A-EQOFFSET+0x248+14, NAMETABLE_A-EQOFFSET+0x208+14, NAMETABLE_A-EQOFFSET+0x1c8+14, NAMETABLE_A-EQOFFSET+0x188+14, NAMETABLE_A-EQOFFSET+0x148+14, NAMETABLE_A-EQOFFSET+0x108+14, NAMETABLE_A-EQOFFSET+0x0c8+14
};

/*void fx_EQ(void)
{
	if (eq_Triangle_Volume) {
		--eq_Triangle_Volume;
	}
	
	if (FT_BUF[6] & 1)
		eq_Triangle_Volume = 5;

	eq_Noise_Volume = eq_noise_approx[FT_BUF[9] & 0x0f];
	eq_Pulse1_Volume = eq_pulse1_approx[FT_BUF[0] & 0x0f];
	eq_Pulse2_Volume = eq_pulse1_approx[FT_BUF[3] & 0x0f];


	for (i=0; i<5; ++i){
		eqValues[0][4-i] = i>=eq_Triangle_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[1][4-i] = i>=eq_Noise_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[2][4-i] = i>=eq_Pulse1_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[3][4-i] = i>=eq_Pulse2_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
	}
	
	set_nmi_user_call_on(1);
}
*/

void fx_EQ(void)
{
	if (eq_Triangle_Volume && (nesclock&3)==0) {
		--eq_Triangle_Volume;
	}
	
	if (FT_BUF[6] & 1)
		eq_Triangle_Volume = 5;

	if (eq_noise_approx[FT_BUF[9] & 0x0f] > eq_Noise_Volume) {
		eq_Noise_Volume = eq_noise_approx[FT_BUF[9] & 0x0f];
	} else if (eq_Noise_Volume && (nesclock&3)==0) {
		--eq_Noise_Volume;
	}
	if (eq_pulse1_approx[FT_BUF[0] & 0x0f] > eq_Pulse1_Volume) {
		eq_Pulse1_Volume = eq_pulse1_approx[FT_BUF[0] & 0x0f];
	} else if (eq_Pulse1_Volume && (nesclock&3)==0) {
		--eq_Pulse1_Volume;
	}
	if (eq_pulse1_approx[FT_BUF[3] & 0x0f] > eq_Pulse2_Volume) {
		eq_Pulse2_Volume = eq_pulse1_approx[FT_BUF[3] & 0x0f];
	} else if (eq_Pulse2_Volume && (nesclock&3)==0) {
		--eq_Pulse2_Volume;
	}

	for (i=0; i<5; ++i){
		eqValues[0][4-i] = i>=eq_Triangle_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[1][4-i] = i>=eq_Noise_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[2][4-i] = i>=eq_Pulse1_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
		eqValues[3][4-i] = i>=eq_Pulse2_Volume ? EQ_CHR_ON : EQ_CHR_OFF;
	}
	
	set_nmi_user_call_on(1);
}

const unsigned char const huita[] = {
	3, 5, 4, 6, 5, 9, 6, 8
};

void fx_SplitScroll(void)
{
	if (!(scrollerPos&15)) {
		scrollSym = scrollerData[scrollerCharIndex];
		if (!scrollSym) {
			scrollSym = ' ';
			scrollerCharIndex=0;
		} else
			++scrollerCharIndex;
		if (scrollerCharPos & 0x10)
			scrollerAddr = NAMETABLE_A + 32*27;
		else
			scrollerAddr = NAMETABLE_B + 32*27;

		scrollerAddr += (scrollerCharPos & 15) << 1;

		_fs = 0;
		while(tbl_ascii[_fs] != scrollSym) 
			++_fs;

		multi_vram_buffer_horz(tbl_alphabet[_fs], 2, scrollerAddr);
		multi_vram_buffer_horz(tbl_alphabet[_fs]+2, 2, scrollerAddr+32);
		scrollerCharPos = (scrollerCharPos + 1) & 31;
	}

	if ((FT_BUF[9] & 0x0f)>9 && !eq_Noise_Val) {
		eq_Noise_Val = 7;
	}

	xy_split(scrollerPos, 210 + 5 - huita[eq_Noise_Val]/* - 2 - eq_Noise_Val*/);

	if (eq_Noise_Val)
		--eq_Noise_Val;
	scrollerPos = (scrollerPos + 2) & 511;


}

const unsigned int sine_Table_Shake[] = {
508, 510, 513, 515, 518, 520, 523, 525, 527, 530, 532, 534, 536, 538, 540, 542,
544, 546, 548, 549, 551, 552, 553, 555, 556, 556, 557, 558, 559, 559, 559, 559,
560, 559, 559, 559, 559, 558, 557, 556, 556, 555, 553, 552, 551, 549, 548, 546,
544, 542, 540, 538, 536, 534, 532, 530, 527, 525, 523, 520, 518, 515, 513, 510,
508, 505, 502, 500, 497, 495, 492, 490, 488, 485, 483, 481, 479, 477, 475, 473,
471, 469, 467, 466, 464, 463, 462, 460, 459, 459, 458, 457, 456, 456, 456, 456,
456, 456, 456, 456, 456, 457, 458, 459, 459, 460, 462, 463, 464, 466, 467, 469,
471, 473, 475, 477, 479, 481, 483, 485, 488, 490, 492, 495, 497, 500, 502, 505
};

const unsigned char spr_telega[]={
	- 8,- 8,0x52, 0,
	  0,- 8,0x53, 0,
	- 8,  0,0x54, 0,
	  0,  0,0x55, 0,
	128
};

unsigned char nesdevPalId2 = 0;
unsigned char nesdevPalId2Dir = 0;
unsigned char nesdevPalId = 0;
unsigned char nesdevFaze = 0;
unsigned char telegaX = 64;		//64+8+4;	//64;4
unsigned char telegaY = 128+64-16;	//128+32-4;	//128+64;3
unsigned char telegaPalId = 0;

const unsigned char telegaPal[6][4] = {
	{0x0F,0x0F,0x0F,0x0F},
	{0x0F,0x02,0x02,0x02},
	{0x0F,0x02,0x02,0x12},
	{0x0F,0x02,0x12,0x21},
	{0x0F,0x1C,0x21,0x31},
	{0x0F,0x1C,0x21,0x30}
};

unsigned char nesDevPlayCtrl = 0;

void fx_NesDev(void)
{
	//nesdev
	pal_bg(palNesdev[14]);
	pal_spr(palNesdev[14]);
	cnrom_set_bank(1);
	bank_spr(0);
	vram_adr(NAMETABLE_A);
	vram_unrle(logo_scr);
	ppu_on_all();
	
	while(nesdevFaze < 4)
	{
		ppu_wait_nmi();
		oam_clear();
		oam_meta_spr(telegaX, telegaY, 0, spr_telega);

		//wait
		if ((nesdevFaze == 0 && nesclock == 96)) {
			nesdevFaze = 1;
		}
		
		// sound nesdev - variant 1
		if ((nesDevPlayCtrl&1) == 0 && nesclock == 64) {
			//nesDevPlayCtrl |= 1;
			//music_play(2);
		}
		
		
		//fade in nesdev
		if ((nesdevFaze == 1)) {
			
			// sound nesdev - variant 2
			if ((nesDevPlayCtrl&1) == 0) {
				nesDevPlayCtrl |= 1;
				music_play(2);
			}
			
			if (nesdevPalId < 5 && ((nesclock & 1) == 0)) {
				++nesdevPalId;
				pal_bg(palNesdev[nesdevPalId]);
			}
			// nesdev blink
			if (nesclock >= 128 && (nesclock & 3) == 0) {
				if (nesdevPalId2 < 3) {
				pal_bg(palNesdev2[nesdevPalId2]);
				nesdevPalId2++;
				} else {
					pal_bg(palNesdev[nesdevPalId]);
				}
			}

			if (nesclock == 0) {
				nesdevFaze = 2;
			}
			if (nesclock >= 220) {
				if (nesdevPalId < 9 && ((nesclock & 3) == 0)) {
					++nesdevPalId;
					pal_bg(palNesdev[nesdevPalId]);
				}
			}
			
			if (nesclock > 192) {
				// sound telega
				if ((nesDevPlayCtrl&2) == 0) {
					nesDevPlayCtrl |= 2;
					music_stop();
					sfx_play(SFX_TELEGA_FLY, 0);
				}

				// telega in
				if (telegaX < 192+8) {
					telegaX += 4;	// 2 4
					telegaY -= 3;	// 1 3
				}
				if ((nesclock & 3) == 0 && telegaPalId < 6) {
					++telegaPalId;
					pal_spr(telegaPal[telegaPalId - 1]);
				}
			}
		}
		//fade in credits
		if ((nesdevFaze == 2)) {
			if (nesclock > 32) {
				if (nesdevPalId < 9 && ((nesclock & 3) == 0)) {
					++nesdevPalId;
					pal_bg(palNesdev[nesdevPalId]);
				}
				if (nesclock == 255) {
					nesdevFaze = 3;
				}
			}

		}
		//fadeout
		
		if (nesdevFaze == 3) {
			if (nesdevPalId < 14 && ((nesclock & 3) == 0)) {
				++nesdevPalId;
				pal_bg(palNesdev[nesdevPalId]);
			}
			if (nesclock == 128) {
				nesdevFaze = 4;
			}

			// telega out
			telegaX += 4;
			telegaY -= 3;
			if ((nesclock&1) && telegaPalId > 0) {
				--telegaPalId;
				pal_spr(telegaPal[telegaPalId]);
			}
			
		}
		++nesclock;
	}
	ppu_off();
}

unsigned char krujWait = 0;
unsigned char krujFrm = 0;
unsigned char krujPalId = 0;
unsigned char krujBgPalId = 0;
unsigned char krujWait2 = 0;

const unsigned char krujBgPal[8][8] = {
	{0x0F,0x0F,0x20,0x30,	0x0F,0x20,0x20,0x30},
	{0x0F,0x0F,0x22,0x32,	0x0F,0x22,0x22,0x32},
	{0x0F,0x0F,0x22,0x32,	0x0F,0x22,0x22,0x32},
	{0x0F,0x0F,0x22,0x22,	0x0F,0x22,0x22,0x22},
	{0x0F,0x0F,0x12,0x12,	0x0F,0x12,0x12,0x12},
	{0x0F,0x0F,0x02,0x02,	0x0F,0x02,0x02,0x02},
	{0x0F,0x0F,0x0F,0x0F,	0x0F,0x0F,0x0F,0x0F},
};
const unsigned char krujSprPal[16] = {
	0x0F,0x30,0x0F,0x0F,
	0x0F,0x30,0x30,0x0F,
	0x0F,0x30,0x30,0x30,
	0x0F,0x0F,0x30,0x16
};

unsigned char krujAnimaId = 0;
unsigned char krujAnimaLenId = 0;
const unsigned char krujAnimaLen[43] = {
4, 2, 3, 3, 3, 4, 4, 4, 6, 6, 4, 6, 4, 4, 6, 4, 6, 4, 6, 2, 2, 4, 4, 4, 6, 4, 4, 3, 3, 3, 10, 10, 6, 6, 12, 8, 4, 6, 6, 4, 12, 6, 10
};
const unsigned char krujAnimaSpr[222][4] = {
{104, 2, 0, 72}, {112, 3, 0, 72}, {104, 4, 0, 80}, {104, 4, 64, 64}, {112, 5, 0, 80}, {112, 5, 64, 64}, {120, 6, 0, 64}, {120, 7, 0, 72}, {120, 6, 64, 80}, {128, 8, 0, 64}, {128, 9, 0, 72}, {128, 8, 64, 80}, {136, 10, 0, 64}, {136, 11, 0, 72}, {136, 10, 64, 80}, {136, 12, 0, 64}, {144, 13, 0, 64}, {136, 12, 64, 80}, {144, 13, 64, 80}, {136, 14, 0, 56}, {144, 15, 0, 56}, {136, 14, 64, 88}, {144, 15, 64, 88}, {136, 16, 0, 48}, {144, 17, 0, 48}, {136, 16, 64, 96}, {144, 17, 64, 96}, {136, 18, 0, 96}, {136, 19, 0, 104}, {144, 20, 0, 104}, {136, 18, 64, 48}, {136, 19, 64, 40}, {144, 20, 64, 40}, {128, 21, 0, 96}, {128, 22, 0, 104}, {120, 23, 0, 104}, {128, 21, 64, 48}, {128, 22, 64, 40}, {120, 23, 64, 40}, {120, 24, 0, 96}, {128, 25, 0, 96}, {120, 24, 64, 48}, {128, 25, 64, 48}, {120, 26, 0, 88}, {128, 27, 0, 88}, {128, 28, 0, 80}, {120, 26, 64, 56}, {128, 27, 64, 56}, {128, 28, 64, 64}, {136, 29, 0, 80}, {136, 30, 0, 88}, {136, 29, 64, 64}, {136, 30, 64, 56}, {144, 31, 0, 80}, {144, 32, 0, 88}, {144, 31, 64, 64}, {144, 32, 64, 56}, {152, 33, 0, 80}, {152, 34, 0, 88}, {160, 35, 0, 88}, {152, 33, 64, 64}, {152, 34, 64, 56}, {160, 35, 64, 56}, {160, 36, 0, 88}, {160, 37, 0, 96}, {160, 36, 64, 56}, {160, 37, 64, 48}, {168, 38, 0, 88}, {168, 39, 0, 96}, {160, 40, 0, 96}, {168, 38, 64, 56}, {168, 39, 64, 48}, {160, 40, 64, 48}, {168, 41, 0, 104}, {176, 42, 0, 104}, {168, 41, 64, 40}, {176, 42, 64, 40}, {168, 43, 0, 112}, {176, 44, 0, 112}, {168, 43, 64, 32}, {176, 44, 64, 32}, {168, 45, 0, 120}, {168, 45, 64, 24}, {168, 46, 0, 120}, {168, 46, 64, 24}, {160, 47, 0, 120}, {160, 47, 64, 24}, {152, 48, 0, 120}, {152, 48, 64, 24}, {152, 49, 0, 112}, {152, 49, 64, 32}, {152, 50, 0, 32}, {144, 51, 0, 32}, {152, 50, 64, 112}, {144, 51, 64, 112}, {144, 52, 0, 40}, {152, 53, 0, 40}, {144, 52, 64, 104}, {152, 53, 64, 104}, {152, 54, 0, 56}, {152, 55, 0, 48}, {160, 56, 0, 48}, {152, 54, 64, 88}, {152, 55, 64, 96}, {160, 56, 64, 96}, {160, 57, 0, 88}, {160, 58, 0, 80}, {160, 57, 64, 56}, {160, 58, 64, 64}, {168, 59, 0, 88}, {168, 60, 0, 80}, {168, 59, 64, 56}, {168, 60, 64, 64}, {176, 61, 0, 64}, {176, 62, 0, 72}, {176, 61, 64, 80}, {184, 63, 0, 64}, {184, 64, 0, 72}, {184, 63, 64, 80}, {192, 65, 0, 64}, {192, 66, 0, 72}, {192, 65, 64, 80}, {200, 67, 0, 80}, {200, 68, 0, 88}, {200, 67, 64, 64}, {200, 68, 64, 56}, {224, 69, 0, 80}, {232, 70, 0, 80}, {232, 71, 0, 88}, {224, 69, 64, 64}, {232, 70, 64, 64}, {232, 71, 64, 56}, {200, 72, 0, 56}, {208, 73, 0, 56}, {208, 74, 0, 48}, {200, 72, 64, 88}, {208, 73, 64, 88}, {208, 74, 64, 96}, {224, 75, 0, 88}, {216, 76, 0, 88}, {224, 75, 64, 56}, {216, 76, 64, 56}, {208, 77, 0, 96}, {208, 77, 64, 48}, {216, 78, 0, 96}, {224, 79, 0, 96}, {216, 78, 64, 48}, {224, 79, 64, 48}, {208, 80, 0, 104}, {208, 81, 0, 112}, {208, 80, 64, 40}, {208, 81, 64, 32}, {216, 82, 0, 104}, {216, 82, 64, 40}, {208, 83, 0, 112}, {208, 84, 0, 120}, {200, 85, 0, 112}, {208, 83, 64, 32}, {208, 84, 64, 24}, {200, 85, 64, 32}, {216, 86, 0, 32}, {224, 87, 0, 32}, {224, 88, 0, 24}, {216, 86, 64, 112}, {224, 87, 64, 112}, {224, 88, 64, 120}, {208, 89, 0, 120}, {200, 90, 0, 120}, {200, 91, 0, 112}, {208, 89, 64, 24}, {200, 90, 64, 24}, {200, 91, 64, 32}, {224, 92, 0, 24}, {224, 92, 64, 120}, {192, 93, 0, 120}, {192, 93, 64, 24}, {232, 94, 0, 120}, {232, 94, 64, 24}, {184, 95, 0, 24}, {176, 96, 0, 24}, {184, 95, 64, 120}, {176, 96, 64, 120}, {184, 97, 0, 112}, {184, 97, 64, 32}, {184, 98, 0, 112}, {176, 99, 0, 112}, {176, 100, 0, 104}, {184, 98, 64, 32}, {176, 99, 64, 32}, {176, 100, 64, 40}, {176, 101, 0, 104}, {184, 102, 0, 104}, {176, 101, 64, 40}, {184, 102, 64, 40}, {192, 103, 0, 104}, {192, 104, 0, 112}, {200, 105, 0, 104}, {192, 103, 64, 40}, {192, 104, 64, 32}, {200, 105, 64, 40}, {224, 106, 0, 104}, {232, 107, 0, 104}, {232, 108, 0, 112}, {224, 106, 64, 40}, {232, 107, 64, 40}, {232, 108, 64, 32}, {200, 109, 0, 40}, {200, 110, 0, 32}, {200, 109, 64, 104}, {200, 110, 64, 112}, {224, 111, 0, 112}, {224, 111, 64, 32}, {208, 112, 0, 112}, {208, 113, 0, 120}, {200, 114, 0, 120}, {208, 112, 64, 32}, {208, 113, 64, 24}, {200, 114, 64, 24}, {216, 115, 0, 120}, {224, 116, 0, 120}, {216, 115, 64, 24}, {224, 116, 64, 24}
};
const unsigned char krujAnimaChr[222][3] = {
{33, 169, 177}, {33, 201, 192}, {33, 170, 178}, {33, 168, 176}, {33, 202, 193}, {33, 200, 191}, {33, 232, 9}, {33, 233, 10}, {33, 234, 11}, {34, 8, 190}, {34, 9, 23}, {34, 10, 191}, {34, 40, 192}, {34, 41, 36}, {34, 42, 193}, {34, 40, 194}, {34, 72, 195}, {34, 42, 196}, {34, 74, 197}, {34, 39, 198}, {34, 71, 199}, {34, 43, 200}, {34, 75, 201}, {34, 38, 202}, {34, 70, 46}, {34, 44, 203}, {34, 76, 52}, {34, 44, 39}, {34, 45, 27}, {34, 77, 204}, {34, 38, 33}, {34, 37, 32}, {34, 69, 205}, {34, 12, 206}, {34, 13, 14}, {33, 237, 1}, {34, 6, 207}, {34, 5, 19}, {33, 229, 6}, {33, 236, 13}, {34, 12, 26}, {33, 230, 7}, {34, 6, 20}, {33, 235, 12}, {34, 11, 25}, {34, 10, 24}, {33, 231, 8}, {34, 7, 21}, {34, 8, 22}, {34, 42, 37}, {34, 43, 38}, {34, 40, 35}, {34, 39, 34}, {34, 74, 50}, {34, 75, 51}, {34, 72, 48}, {34, 71, 47}, {34, 106, 63}, {34, 107, 208}, {34, 139, 0}, {34, 104, 61}, {34, 103, 209}, {34, 135, 0}, {34, 139, 210}, {34, 140, 211}, {34, 135, 212}, {34, 134, 213}, {34, 171, 214}, {34, 172, 87}, {34, 140, 215}, {34, 167, 216}, {34, 166, 81}, {34, 134, 217}, {34, 173, 75}, {34, 205, 218}, {34, 165, 80}, {34, 197, 219}, {34, 174, 76}, {34, 206, 220}, {34, 164, 79}, {34, 196, 221}, {34, 175, 0}, {34, 163, 0}, {34, 175, 77}, {34, 163, 78}, {34, 143, 66}, {34, 131, 67}, {34, 111, 55}, {34, 99, 56}, {34, 110, 0}, {34, 100, 0}, {34, 100, 57}, {34, 68, 44}, {34, 110, 54}, {34, 78, 41}, {34, 69, 45}, {34, 101, 58}, {34, 77, 40}, {34, 109, 53}, {34, 103, 60}, {34, 102, 59}, {34, 134, 68}, {34, 107, 64}, {34, 108, 65}, {34, 140, 74}, {34, 139, 73}, {34, 138, 72}, {34, 135, 69}, {34, 136, 70}, {34, 171, 86}, {34, 170, 85}, {34, 167, 82}, {34, 168, 83}, {34, 200, 96}, {34, 201, 0}, {34, 202, 98}, {34, 232, 109}, {34, 233, 110}, {34, 234, 111}, {35, 8, 122}, {35, 9, 123}, {35, 10, 124}, {35, 42, 136}, {35, 43, 222}, {35, 40, 134}, {35, 39, 223}, {35, 138, 175}, {35, 170, 187}, {35, 171, 188}, {35, 136, 173}, {35, 168, 186}, {35, 167, 185}, {35, 39, 133}, {35, 71, 146}, {35, 70, 0}, {35, 43, 137}, {35, 75, 150}, {35, 76, 0}, {35, 139, 176}, {35, 107, 163}, {35, 135, 172}, {35, 103, 159}, {35, 76, 151}, {35, 70, 145}, {35, 108, 164}, {35, 140, 224}, {35, 102, 158}, {35, 134, 225}, {35, 77, 139}, {35, 78, 0}, {35, 69, 144}, {35, 68, 0}, {35, 109, 152}, {35, 101, 157}, {35, 78, 226}, {35, 79, 0}, {35, 46, 0}, {35, 68, 227}, {35, 67, 0}, {35, 36, 0}, {35, 100, 228}, {35, 132, 229}, {35, 131, 0}, {35, 110, 230}, {35, 142, 231}, {35, 143, 0}, {35, 79, 232}, {35, 47, 233}, {35, 46, 234}, {35, 67, 235}, {35, 35, 236}, {35, 36, 237}, {35, 131, 238}, {35, 143, 239}, {35, 15, 116}, {35, 3, 117}, {35, 175, 180}, {35, 163, 181}, {34, 227, 104}, {34, 195, 91}, {34, 239, 103}, {34, 207, 90}, {34, 238, 0}, {34, 228, 0}, {34, 238, 102}, {34, 206, 89}, {34, 205, 240}, {34, 228, 105}, {34, 196, 92}, {34, 197, 241}, {34, 205, 88}, {34, 237, 101}, {34, 197, 93}, {34, 229, 106}, {35, 13, 114}, {35, 14, 0}, {35, 45, 0}, {35, 5, 119}, {35, 4, 0}, {35, 37, 0}, {35, 141, 165}, {35, 173, 178}, {35, 174, 179}, {35, 133, 170}, {35, 165, 183}, {35, 164, 182}, {35, 37, 131}, {35, 36, 130}, {35, 45, 126}, {35, 46, 127}, {35, 142, 166}, {35, 132, 169}, {35, 78, 140}, {35, 79, 141}, {35, 47, 128}, {35, 68, 143}, {35, 67, 142}, {35, 35, 129}, {35, 111, 154}, {35, 143, 167}, {35, 99, 155}, {35, 131, 168}
};


unsigned char krujCheckId = 0;
const unsigned char krujCheck[2][2] = { {4,3}, {4,4} };

unsigned char krujChr = 0;
unsigned int krujHi = 0;
unsigned char krujLo = 0;
unsigned char krujX = 0;
unsigned char krujY = 0;
unsigned char krujAttr = 0;

unsigned char krujFill = 0;
unsigned char krujFillAdr = 0;
unsigned char krujFillVal1 = 0b01010101;
unsigned char krujFillVal2 = 0b01010101;

//unsigned char kmfList1[30] = { 0x8c,0x7a, 0xa8,0x7a, 0xc4,0x7a, 0xe0,0x7a, 0x8c,0x96, 0xa8,0x96, 0xc4,0x96, 0xe0,0x96, 0x8c,0xb2, 0xa8,0xb2, 0xc4,0xb2, 0xe0,0xb2, 0x8c,0xce, 0xa8,0xce, 0xc4,0xce };
//unsigned char kmfList2[30] = { 0x10,0x7a, 0x2c,0x7a, 0x48,0x7a, 0x64,0x7a, 0x10,0x96, 0x2c,0x96, 0x48,0x96, 0x64,0x96, 0x10,0xb2, 0x2c,0xb2, 0x48,0xb2, 0x64,0xb2, 0x2c,0xce, 0x48,0xce, 0x64,0xce };
//unsigned char kmfList3[20] = { 0xe0,0xFD, 0xc4,0x1a, 0xe0,0x1a, 0xa8,0x36, 0xc4,0x36, 0xe0,0x36, 0x8c,0x52, 0xa8,0x52, 0xc4,0x52, 0xe0,0x52 };
//unsigned char kmfList4[20] = { 0x10,0xFD, 0x10,0x1a, 0x2c,0x1a, 0x10,0x36, 0x2c,0x36, 0x48,0x36, 0x10,0x52, 0x2c,0x52, 0x48,0x52, 0x64,0x52 };
unsigned char kmfList1[30] = { 0x90,0x71, 0xab,0x71, 0xc6,0x71, 0xe1,0x71, 0x90,0x8c, 0xab,0x8c, 0xc6,0x8c, 0xe1,0x8c, 0x90,0xa7, 0xab,0xa7, 0xc6,0xa7, 0xe1,0xa7, 0x90,0xc2, 0xab,0xc2, 0xc6,0xc2 };
unsigned char kmfList2[30] = { 0x19,0x71, 0x34,0x71, 0x4f,0x71, 0x6a,0x71, 0x19,0x8c, 0x34,0x8c, 0x4f,0x8c, 0x6a,0x8c, 0x19,0xa7, 0x34,0xa7, 0x4f,0xa7, 0x6a,0xa7, 0x34,0xc2, 0x4f,0xc2, 0x6a,0xc2 };
unsigned char kmfList3[30] = { 0xe1,0xfa, 0xc6,0x15, 0xe1,0x15, 0xab,0x30, 0xc6,0x30, 0xe1,0x30, 0x90,0x4b, 0xab,0x4b, 0xc6,0x4b, 0xe1,0x4b };
unsigned char kmfList4[30] = { 0x19,0xfa, 0x19,0x15, 0x34,0x15, 0x19,0x30, 0x34,0x30, 0x4f,0x30, 0x19,0x4b, 0x34,0x4b, 0x4f,0x4b, 0x6a,0x4b };

unsigned char kmfDir1[30] = {0xff,0xff, 0xfe,0xff, 0xfd,0xff, 0xfc,0xff, 0xff,0xfe, 0xfe,0xfe, 0xfd,0xfe, 0xfc,0xfe, 0xff,0xfd, 0xfe,0xfd, 0xfd,0xfd, 0xfc,0xfd, 0xff,0xfc, 0xfe,0xfc, 0xfd,0xfc };
unsigned char kmfDir2[30] = {0x4,0xff, 0x3,0xff, 0x2,0xff, 0x1,0xff, 0x4,0xfe, 0x3,0xfe, 0x2,0xfe, 0x1,0xfe, 0x4,0xfd, 0x3,0xfd, 0x2,0xfd, 0x1,0xfd, 0x3,0xfc, 0x2,0xfc, 0x1,0xfc };
unsigned char kmfDir3[20] = {0xfc,0x4, 0xfd,0x3, 0xfc,0x3, 0xfe,0x2, 0xfd,0x2, 0xfc,0x2, 0xff,0x1, 0xfe,0x1, 0xfd,0x1, 0xfc,0x1 };
unsigned char kmfDir4[20] = {0x4,0x4, 0x4,0x3, 0x3,0x3, 0x4,0x2, 0x3,0x2, 0x2,0x2, 0x4,0x1, 0x3,0x1, 0x2,0x1, 0x1,0x1 };

unsigned char kmfStep = 11;

void fx_Krujeva(void)
{
	pal_bg(krujBgPal[0]);
	pal_spr(krujSprPal);
	cnrom_set_bank(3);
	bank_spr(1);
	vram_adr(NAMETABLE_A);
	vram_unrle(kruj_nametable);
	set_nmi_user_call_on(2);
	oam_clear();
	oam_spr(253, 118, 0xFF, 3 | OAM_BEHIND, 0);
	ppu_on_all();
	music_play(0);
	isNtsc = ppu_system() == 0 ? 0 : 1;

	while (krujFrm < (isNtsc ? 48 : 46)) //(isNtsc ? 48 : 46))
	{
		++nesclock;
		oam_spr(2, 118, 0xFF, 3 | OAM_BEHIND, 0); //244 219 210
		ppu_wait_nmi();
		clear_vram_buffer();
		
		
		if (krujFrm > 44 && krujBgPalId < 6) {
			if ((nesclock&3) == 0) {
				++krujBgPalId;
				pal_bg(krujBgPal[krujBgPalId]);
			}
		}

		// krujeva
		if (++krujWait >= krujCheck[isNtsc][krujCheckId]) {
			krujWait = 0;
			if (++krujPalId == 3) {
				krujPalId = 0;
				krujFrm++;
				//do spr
				if (krujAnimaLenId < 43) {
					j = krujAnimaLen[krujAnimaLenId];
					for (i = 0; i < j; i++) {
						krujChr = krujAnimaChr[krujAnimaId][2];
						if (krujChr != 0) {
							krujHi = 256 * krujAnimaChr[krujAnimaId][0];
							krujLo = krujAnimaChr[krujAnimaId][1];

							one_vram_buffer(krujChr, krujHi + krujLo);
							one_vram_buffer(krujChr, krujHi + krujLo + 13);
							krujX = krujAnimaSpr[krujAnimaId][3];

							if (krujX <= 256-8-208) {
								one_vram_buffer(krujChr, krujHi + krujLo + 26);
							}
							if (krujX >= 13*8) {
								one_vram_buffer(krujChr, krujHi + krujLo - 13);
							}
						}
						++krujAnimaId;
					}
					++krujAnimaLenId;
				}
			}
			krujCheckId ^= 1;
		}

		// fill bg attr
		krujFill = 0;
		if (krujWait == 2 && krujFrm > 26) {
			if (++krujWait2 == 3) {
				krujWait2 = 0;
				krujFillVal1 ^= 0b01010000;
				krujFillVal2 ^= 0b00000101;
				if (krujFillVal1 == 0b00000101) {
					if (krujFillAdr <= 8*4) {
						krujFillAdr += 8;
					}
				}
				if (krujFillAdr <= 8*4) {
					krujFill = 1;
				} else {
					krujFillAdr = 128;
				}
			}
		}
		
		//mflogo
		if (krujFrm > 43) {
			if (krujWait == 0) {
				spr = 4;
				
				j = 0;
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList1[j++], kmfList1[j++], 0x01, 3, spr);
				
				j = 0;
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList2[j++], kmfList2[j++], 0x01, 3, spr);
				
				j = 0;
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList3[j++], kmfList3[j++], 0x01, 3, spr);

				j = 0;
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
				spr = oam_spr(kmfList4[j++], kmfList4[j++], 0x01, 3, spr);
			}
			if (krujWait == 1) {
				if (kmfStep) {
					j = 29;
					while (j) {
						kmfList1[j] += kmfDir1[j--];
					}
					kmfList1[j] += kmfDir1[j];
					j = 19;
					while (j) {
						kmfList3[j] += kmfDir3[j--];
					}
					kmfList3[j] += kmfDir3[j];
				}
			}
			if (krujWait == 2) {
				if (kmfStep) {
					j = 29;
					while (j) {
						kmfList2[j] += kmfDir2[j--];
					}
					kmfList2[j] += kmfDir2[j];
					j = 19;
					while (j) {
						kmfList4[j] += kmfDir4[j--];
					}
					kmfList4[j] += kmfDir4[j];
					--kmfStep;
				}
			}
		}

		split_krujeva();

		if (krujWait == 1) {
			if (krujAnimaLenId < 43) {
				spr = 4;
				j = krujAnimaLen[krujAnimaLenId];
				for (i = 0; i < j; i++) {
					// +0=y +1=spr +2=attr +3=x
					// oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);
					krujChr = krujAnimaSpr[krujAnimaId+i][1];
					if (krujChr != 0) {
						krujY = krujAnimaSpr[krujAnimaId+i][0] - 1;
						krujX = krujAnimaSpr[krujAnimaId+i][3];
						krujAttr = krujPalId + krujAnimaSpr[krujAnimaId+i][2];
						spr = oam_spr(krujX, krujY, krujChr, krujAttr, spr);
						spr = oam_spr(krujX + 104, krujY, krujChr, krujAttr, spr);
						if (krujX <= 256-8-208) {
							spr = oam_spr(krujX + 208, krujY, krujChr, krujAttr, spr);
						}
						if (krujX >= 13*8) {
							spr = oam_spr(krujX - 13*8, krujY, krujChr, krujAttr, spr);
						}
					}
				}
			}
		}
		//gray_line();
		
	}
}

// ---------------------------------------
// -------------- VIRUSEZ ----------------
// ---------------------------------------

const unsigned char covid_pathX1[512] = {
8, 7, 7, 7, 6, 6, 7, 7, 7, 8, 9, 9, 10, 12, 13, 14, 16, 17, 19, 20, 22, 24, 26, 28, 30, 32, 35, 37, 39, 41, 44, 46, 49, 51, 54, 56, 59, 61, 64, 66, 69, 71, 74, 76, 79, 81, 84, 86, 88, 90, 93, 95, 97, 99, 101, 102, 104, 106, 107, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 121, 122, 123, 125, 127, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 149, 151, 153, 155, 157, 160, 162, 164, 166, 169, 171, 173, 175, 177, 180, 182, 184, 186, 188, 190, 191, 193, 195, 196, 198, 200, 201, 202, 203, 205, 206, 207, 207, 208, 209, 209, 209, 210, 210, 210, 209, 209, 209, 208, 207, 206, 205, 203, 201, 199, 197, 195, 192, 190, 187, 184, 180, 177, 174, 170, 166, 162, 158, 154, 150, 146, 142, 137, 133, 128, 124, 120, 115, 110, 106, 101, 97, 92, 88, 83, 79, 75, 70, 66, 62, 58, 54, 50, 47, 43, 40, 36, 33, 30, 27, 24, 22, 19, 17, 15, 14, 12, 11, 10, 9, 8, 8, 8, 8, 9, 10, 11, 12, 13, 15, 17, 19, 22, 24, 27, 30, 33, 36, 40, 43, 47, 51, 55, 59, 63, 67, 71, 76, 80, 85, 89, 94, 99, 103, 108, 113, 117, 122, 127, 131, 136, 141, 145, 150, 154, 158, 163, 167, 171, 175, 178, 182, 186, 189, 192, 195, 198, 201, 203, 206, 208, 210, 212, 213, 214, 215, 216, 217, 217, 217, 217, 217, 217, 216, 216, 215, 215, 214, 213, 212, 210, 209, 208, 206, 205, 203, 201, 199, 197, 195, 193, 191, 189, 187, 185, 183, 180, 178, 176, 173, 171, 168, 166, 164, 161, 159, 156, 154, 152, 149, 147, 145, 142, 140, 138, 136, 134, 132, 130, 128, 126, 124, 123, 121, 120, 118, 117, 116, 115, 114, 113, 112, 111, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 82, 81, 80, 79, 78, 78, 77, 76, 76, 75, 75, 74, 74, 73, 73, 72, 72, 72, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72, 72, 72, 73, 74, 75, 76, 77, 78, 79, 81, 82, 84, 85, 87, 89, 91, 93, 95, 97, 99, 102, 104, 106, 108, 111, 113, 116, 118, 120, 123, 125, 128, 130, 133, 135, 137, 140, 142, 144, 147, 149, 151, 153, 155, 157, 159, 161, 162, 164, 165, 167, 168, 169, 171, 172, 172, 173, 174, 174, 175, 175, 175, 175, 174, 174, 173, 173, 172, 170, 169, 168, 166, 164, 162, 160, 158, 156, 153, 151, 148, 146, 143, 140, 137, 134, 131, 127, 124, 121, 117, 114, 110, 107, 103, 100, 96, 93, 89, 86, 82, 79, 75, 71, 68, 65, 61, 58, 55, 51, 48, 45, 42, 39, 36, 33, 31, 28, 26, 24, 21, 19, 17, 16, 14, 12, 11, 10, 9
};
const unsigned char covid_pathY1[512] = {
152, 152, 151, 151, 150, 149, 148, 147, 145, 144, 142, 140, 139, 136, 134, 132, 130, 127, 125, 122, 119, 117, 114, 111, 108, 105, 102, 99, 96, 93, 89, 86, 83, 80, 77, 74, 71, 67, 64, 61, 58, 55, 53, 50, 47, 44, 42, 39, 37, 35, 32, 30, 28, 26, 25, 23, 22, 20, 19, 18, 17, 17, 16, 16, 16, 16, 16, 17, 17, 18, 19, 20, 22, 23, 25, 26, 28, 30, 32, 34, 36, 39, 41, 44, 46, 49, 52, 55, 57, 60, 63, 66, 69, 72, 76, 79, 82, 85, 88, 91, 94, 97, 100, 103, 106, 109, 112, 115, 118, 121, 123, 126, 128, 131, 133, 135, 137, 139, 141, 143, 144, 146, 147, 148, 150, 150, 151, 152, 152, 152, 152, 152, 152, 152, 151, 151, 150, 150, 149, 148, 147, 146, 144, 143, 142, 140, 139, 137, 136, 134, 132, 130, 129, 127, 125, 123, 121, 119, 117, 115, 112, 110, 108, 106, 104, 102, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 72, 70, 68, 67, 65, 64, 62, 61, 60, 59, 58, 57, 56, 55, 55, 54, 54, 53, 53, 52, 52, 52, 52, 52, 52, 52, 52, 52, 53, 53, 53, 54, 54, 54, 55, 55, 56, 57, 57, 58, 59, 59, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 73, 74, 75, 76, 77, 78, 79, 80, 80, 81, 82, 83, 84, 84, 85, 86, 86, 87, 87, 88, 89, 89, 90, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99, 100, 101, 102, 104, 105, 106, 107, 108, 109, 111, 112, 113, 114, 115, 117, 118, 119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 131, 132, 133, 134, 134, 135, 136, 137, 138, 139, 139, 140, 141, 141, 142, 142, 143, 143, 143, 144, 144, 144, 144, 144, 144, 144, 144, 143, 143, 143, 142, 142, 141, 140, 140, 139, 138, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 126, 125, 124, 123, 121, 120, 119, 118, 116, 115, 114, 112, 111, 110, 108, 107, 106, 104, 103, 102, 101, 99, 98, 97, 96, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 82, 81, 80, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 61, 60, 59, 58, 57, 56, 55, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 39, 38, 37, 37, 36, 35, 35, 34, 34, 34, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 35, 36, 36, 37, 38, 40, 41, 42, 44, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 66, 68, 71, 73, 76, 78, 81, 83, 86, 89, 91, 94, 96, 99, 102, 104, 107, 109, 112, 114, 117, 119, 122, 124, 126, 128, 131, 133, 135, 137, 138, 140, 142, 143, 145, 146, 147, 148, 149, 150, 151, 151, 152, 152, 152
};

const unsigned char covid_pathX2[512] = {
8, 8, 7, 7, 7, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9, 10, 10, 11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 66, 67, 68, 69, 69, 70, 71, 72, 72, 73, 74, 74, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 83, 83, 84, 85, 86, 86, 87, 88, 89, 89, 90, 91, 92, 92, 93, 94, 95, 96, 96, 97, 98, 99, 99, 100, 101, 102, 102, 103, 104, 105, 106, 106, 107, 108, 109, 109, 110, 111, 112, 112, 113, 114, 115, 116, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 125, 125, 126, 127, 128, 128, 129, 130, 130, 131, 132, 133, 133, 134, 135, 136, 136, 137, 138, 139, 139, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 148, 148, 149, 150, 151, 151, 152, 153, 154, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 174, 175, 176, 178, 179, 181, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 195, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 206, 207, 208, 208, 209, 209, 210, 210, 210, 210, 211, 211, 210, 210, 210, 210, 209, 209, 208, 207, 206, 205, 203, 201, 200, 197, 195, 192, 190, 187, 184, 180, 177, 173, 170, 166, 162, 158, 154, 150, 146, 141, 137, 133, 128, 124, 119, 114, 110, 105, 101, 96, 92, 87, 83, 78, 74, 70, 65, 61, 57, 53, 49, 46, 42, 39, 35, 32, 29, 26, 24, 21, 19, 17, 15, 13, 12, 10, 9, 9, 8, 8, 8, 8, 9, 10, 11, 12, 14, 16, 18, 21, 23, 26, 29, 32, 35, 39, 43, 46, 50, 54, 59, 63, 67, 72, 76, 81, 86, 91, 96, 101, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 164, 169, 174, 178, 182, 187, 191, 195, 198, 202, 206, 209, 212, 215, 218, 220, 223, 225, 227, 228, 230, 231, 232, 233, 233, 233, 234, 233, 233, 233, 232, 232, 231, 230, 228, 227, 226, 224, 222, 221, 219, 217, 215, 212, 210, 208, 205, 203, 200, 198, 195, 192, 190, 187, 184, 181, 178, 175, 173, 170, 167, 164, 161, 158, 156, 153, 150, 147, 145, 142, 140, 137, 135, 132, 130, 128, 126, 124, 122, 121, 119, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 102, 101, 99, 98, 96, 94, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 27, 25, 23, 22, 20, 19, 18, 16, 15, 14, 13, 12, 11, 10, 9, 9
};
const unsigned char covid_pathY2[512] = {
8, 8, 9, 10, 11, 12, 13, 14, 15, 17, 19, 21, 22, 24, 27, 29, 31, 34, 36, 39, 41, 44, 47, 50, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82, 86, 89, 92, 94, 97, 100, 103, 106, 109, 111, 114, 117, 119, 121, 124, 126, 128, 130, 132, 134, 135, 137, 138, 140, 141, 142, 142, 143, 144, 144, 144, 144, 144, 144, 143, 143, 142, 141, 140, 139, 138, 136, 135, 133, 132, 130, 128, 126, 124, 122, 120, 118, 116, 113, 111, 109, 106, 104, 101, 99, 96, 94, 91, 89, 86, 84, 82, 79, 77, 74, 72, 70, 67, 65, 63, 61, 59, 57, 55, 53, 52, 50, 49, 47, 46, 45, 44, 43, 42, 41, 41, 40, 40, 40, 40, 40, 41, 41, 42, 43, 44, 45, 46, 47, 49, 50, 52, 53, 55, 57, 59, 61, 63, 65, 67, 70, 72, 74, 77, 79, 81, 84, 86, 89, 91, 94, 96, 99, 101, 104, 106, 108, 111, 113, 115, 118, 120, 122, 124, 126, 128, 130, 132, 133, 135, 136, 138, 139, 140, 141, 142, 143, 143, 144, 144, 144, 144, 144, 144, 143, 143, 142, 141, 140, 139, 137, 136, 134, 132, 130, 128, 126, 124, 122, 120, 117, 115, 112, 109, 107, 104, 101, 98, 95, 92, 90, 87, 84, 81, 78, 74, 71, 68, 65, 62, 60, 57, 54, 51, 48, 45, 43, 40, 37, 35, 33, 30, 28, 26, 24, 22, 20, 18, 17, 15, 14, 13, 12, 11, 10, 9, 9, 9, 8, 8, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26, 28, 29, 30, 32, 33, 34, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 52, 53, 54, 56, 57, 58, 60, 61, 62, 63, 64, 65, 67, 68, 69, 69, 70, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 78, 79, 79, 80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 88, 89, 89, 90, 90, 90, 91, 91, 91, 92, 92, 92, 92, 93, 93, 93, 93, 94, 94, 94, 94, 94, 95, 95, 95, 95, 95, 95, 95, 95, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 95, 95, 95, 95, 95, 95, 94, 94, 94, 94, 93, 93, 93, 93, 92, 92, 92, 91, 91, 91, 90, 90, 90, 89, 89, 89, 88, 88, 88, 87, 87, 87, 86, 86, 86, 85, 85, 85, 84, 84, 84, 83, 83, 82, 82, 82, 82, 81, 81, 81, 80, 80, 80, 79, 79, 78, 77, 77, 76, 75, 74, 73, 72, 71, 69, 68, 67, 65, 64, 62, 61, 59, 58, 56, 55, 53, 51, 50, 48, 46, 44, 43, 41, 39, 38, 36, 34, 33, 31, 30, 28, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 10, 9, 9, 8, 8, 8, 8, 8, 8
};

const unsigned char covid_pathX3[512] = {
112, 111, 111, 110, 109, 108, 108, 107, 106, 105, 104, 104, 103, 102, 101, 100, 100, 99, 98, 97, 96, 95, 95, 94, 93, 92, 91, 91, 90, 89, 88, 88, 87, 86, 86, 85, 84, 84, 83, 83, 82, 82, 81, 81, 80, 80, 80, 79, 79, 79, 79, 78, 78, 78, 78, 78, 78, 78, 78, 78, 79, 79, 79, 80, 80, 80, 81, 82, 82, 83, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 98, 99, 100, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 115, 116, 117, 118, 120, 121, 122, 123, 124, 126, 127, 128, 129, 130, 131, 132, 134, 135, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 144, 145, 146, 146, 147, 147, 148, 148, 149, 149, 149, 150, 150, 150, 151, 151, 151, 151, 151, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 152, 152, 152, 152, 153, 153, 153, 153, 154, 154, 155, 155, 155, 156, 156, 157, 157, 158, 158, 159, 159, 160, 160, 161, 161, 162, 162, 163, 164, 164, 165, 165, 165, 166, 166, 167, 167, 168, 168, 169, 169, 169, 170, 170, 170, 170, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 170, 170, 169, 169, 169, 168, 167, 167, 166, 164, 163, 162, 160, 159, 157, 155, 153, 151, 149, 147, 144, 142, 139, 137, 134, 131, 128, 125, 122, 120, 117, 113, 110, 107, 104, 101, 98, 95, 92, 89, 86, 82, 79, 76, 73, 70, 67, 65, 62, 59, 56, 54, 51, 49, 46, 44, 42, 40, 38, 36, 34, 32, 31, 29, 28, 27, 26, 25, 25, 24, 24, 23, 23, 23, 23, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 30, 31, 33, 34, 35, 36, 38, 39, 41, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 74, 76, 78, 80, 81, 83, 85, 86, 88, 90, 91, 93, 94, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 111, 112, 114, 116, 117, 119, 121, 123, 125, 127, 130, 132, 134, 136, 139, 141, 144, 146, 149, 151, 154, 157, 159, 162, 164, 167, 170, 172, 175, 177, 180, 182, 185, 187, 190, 192, 194, 197, 199, 201, 203, 205, 207, 209, 210, 212, 214, 215, 217, 218, 219, 220, 221, 222, 223, 223, 224, 224, 224, 224, 224, 224, 223, 223, 222, 221, 220, 219, 218, 217, 216, 214, 213, 211, 209, 208, 206, 204, 202, 200, 198, 195, 193, 191, 189, 186, 184, 181, 179, 177, 174, 172, 169, 167, 164, 162, 159, 157, 154, 152, 149, 147, 145, 142, 140, 138, 136, 134, 132, 130, 128, 126, 124, 123, 121, 120, 118, 117, 116, 115, 114, 113
};
const unsigned char covid_pathY3[512] = {
72, 72, 72, 73, 73, 73, 73, 73, 74, 74, 74, 74, 75, 75, 75, 75, 76, 76, 76, 77, 77, 77, 77, 78, 78, 78, 79, 79, 79, 80, 80, 81, 81, 81, 82, 82, 82, 83, 83, 84, 84, 85, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 92, 92, 93, 93, 94, 95, 95, 96, 96, 97, 98, 98, 99, 99, 100, 101, 101, 102, 102, 103, 103, 103, 104, 104, 105, 105, 105, 106, 106, 107, 107, 107, 108, 108, 108, 109, 109, 109, 109, 110, 110, 110, 111, 111, 111, 111, 112, 112, 112, 112, 113, 113, 113, 113, 114, 114, 114, 114, 115, 115, 115, 116, 116, 116, 117, 117, 117, 118, 118, 118, 119, 119, 120, 120, 120, 121, 121, 122, 122, 123, 124, 124, 125, 126, 126, 127, 128, 128, 129, 130, 131, 131, 132, 133, 134, 134, 135, 136, 137, 137, 138, 139, 139, 140, 141, 141, 142, 143, 143, 144, 144, 145, 145, 146, 146, 147, 147, 147, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 148, 147, 147, 146, 146, 145, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 134, 133, 132, 130, 129, 127, 125, 124, 122, 120, 119, 117, 115, 113, 111, 109, 107, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 79, 77, 75, 73, 71, 70, 68, 66, 65, 63, 62, 60, 59, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 48, 47, 46, 46, 45, 45, 44, 43, 43, 42, 42, 41, 40, 39, 39, 38, 37, 37, 36, 35, 35, 34, 33, 32, 32, 31, 30, 30, 29, 28, 28, 27, 27, 26, 25, 25, 24, 24, 23, 23, 23, 22, 22, 21, 21, 21, 21, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 23, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 35, 37, 38, 40, 42, 44, 47, 49, 51, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76, 79, 81, 84, 87, 89, 92, 95, 97, 100, 103, 105, 108, 110, 112, 115, 117, 119, 121, 124, 126, 127, 129, 131, 133, 134, 136, 137, 138, 140, 141, 142, 142, 143, 143, 144, 144, 144, 144, 144, 143, 143, 142, 141, 140, 139, 138, 136, 135, 133, 132, 130, 128, 126, 124, 122, 120, 117, 115, 113, 110, 108, 105, 102, 100, 97, 94, 92, 89, 86, 83, 80, 78, 75, 72, 69, 66, 64, 61, 58, 56, 53, 50, 48, 45, 43, 41, 38, 36, 34, 32, 30, 28, 26, 25, 23, 22, 20, 19, 18, 17, 16, 15, 15, 14, 14, 14, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 33, 35, 36, 38, 39, 41, 42, 44, 45, 47, 48, 50, 51, 52, 54, 55, 57, 58, 59, 60, 62, 63, 64, 65, 66, 67, 68, 69, 69, 70, 71, 71, 71, 72
};

const unsigned char covid_pathX4[512] = {
48, 47, 47, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 46, 46, 46, 47, 47, 48, 48, 49, 50, 50, 51, 52, 53, 54, 54, 55, 56, 57, 58, 59, 60, 61, 62, 62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 74, 74, 74, 74, 74, 74, 73, 73, 73, 72, 71, 71, 70, 69, 68, 67, 66, 65, 64, 63, 61, 60, 59, 57, 56, 55, 53, 52, 50, 49, 47, 46, 44, 43, 41, 39, 38, 36, 35, 33, 32, 30, 29, 27, 26, 24, 23, 22, 20, 19, 18, 17, 16, 14, 13, 12, 12, 11, 10, 9, 9, 8, 7, 7, 7, 6, 6, 6, 6, 6, 7, 7, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 23, 26, 28, 31, 35, 38, 41, 45, 49, 53, 56, 61, 65, 69, 73, 78, 82, 87, 91, 96, 101, 105, 110, 115, 119, 124, 129, 134, 138, 143, 147, 152, 156, 161, 165, 169, 173, 177, 181, 185, 189, 192, 196, 199, 202, 205, 208, 211, 213, 215, 217, 219, 221, 222, 223, 224, 225, 225, 225, 225, 225, 225, 224, 223, 222, 221, 220, 218, 217, 215, 213, 211, 209, 207, 204, 202, 199, 197, 194, 191, 188, 185, 182, 179, 176, 173, 170, 167, 164, 161, 157, 154, 151, 148, 145, 142, 139, 136, 133, 130, 127, 124, 122, 119, 117, 114, 112, 110, 108, 106, 104, 103, 101, 100, 99, 98, 97, 97, 96, 96, 96, 96, 96, 97, 97, 98, 99, 100, 101, 102, 104, 105, 107, 108, 110, 112, 114, 116, 118, 120, 122, 124, 127, 129, 131, 134, 136, 139, 141, 144, 147, 149, 152, 154, 157, 160, 162, 165, 167, 170, 172, 175, 177, 179, 182, 184, 186, 188, 190, 192, 194, 196, 198, 199, 201, 202, 203, 205, 206, 207, 207, 208, 209, 209, 209, 209, 209, 209, 209, 209, 208, 208, 207, 206, 206, 205, 204, 203, 201, 200, 199, 197, 196, 195, 193, 191, 190, 188, 186, 184, 183, 181, 179, 177, 175, 173, 171, 169, 167, 165, 163, 161, 159, 157, 155, 153, 151, 149, 147, 146, 144, 142, 140, 139, 137, 135, 134, 132, 131, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 120, 119, 119, 119, 119, 119, 119, 119, 120, 120, 120, 121, 122, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 135, 136, 137, 138, 140, 141, 142, 143, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 160, 161, 161, 162, 162, 163, 163, 163, 163, 163, 163, 163, 162, 162, 161, 160, 159, 159, 157, 156, 155, 154, 153, 151, 150, 148, 147, 145, 143, 141, 139, 138, 136, 134, 132, 129, 127, 125, 123, 121, 119, 116, 114, 112, 110, 107, 105, 103, 100, 98, 96, 94, 91, 89, 87, 85, 83, 80, 78, 76, 74, 72, 70, 68, 67, 65, 63, 61, 60, 58, 57, 55, 54, 53, 52, 51, 50, 49
};

const unsigned char covid_pathY4[512] = {
56, 56, 56, 57, 57, 58, 58, 59, 60, 61, 61, 62, 63, 65, 66, 67, 68, 69, 71, 72, 74, 75, 77, 78, 80, 81, 83, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 106, 107, 109, 111, 112, 114, 116, 117, 119, 120, 122, 123, 125, 126, 127, 128, 130, 131, 132, 133, 134, 134, 135, 136, 136, 137, 137, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 137, 137, 136, 136, 135, 135, 134, 133, 132, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 121, 120, 119, 118, 117, 116, 114, 113, 112, 111, 110, 108, 107, 106, 105, 104, 103, 101, 100, 99, 98, 97, 96, 95, 94, 94, 93, 92, 91, 90, 90, 89, 89, 88, 87, 87, 86, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 75, 74, 73, 71, 70, 69, 67, 66, 64, 63, 61, 60, 58, 57, 55, 54, 52, 51, 49, 48, 46, 45, 43, 42, 40, 39, 37, 36, 35, 33, 32, 31, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 19, 18, 18, 17, 17, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 23, 24, 25, 26, 26, 27, 28, 29, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 46, 47, 48, 49, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 100, 101, 103, 104, 106, 107, 109, 110, 112, 113, 115, 116, 118, 119, 121, 122, 123, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 136, 137, 137, 138, 138, 138, 138, 138, 138, 138, 138, 138, 137, 137, 136, 135, 134, 133, 132, 131, 130, 128, 127, 125, 123, 122, 120, 118, 116, 114, 112, 110, 107, 105, 103, 101, 98, 96, 93, 91, 88, 86, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 58, 56, 53, 51, 49, 46, 44, 42, 40, 38, 36, 34, 32, 30, 29, 27, 25, 24, 23, 21, 20, 19, 18, 18, 17, 16, 16, 16, 16, 16, 16, 16, 16, 17, 18, 18, 19, 20, 21, 23, 24, 25, 27, 28, 30, 32, 33, 35, 37, 39, 41, 43, 45, 47, 49, 52, 54, 56, 58, 60, 63, 65, 67, 69, 72, 74, 76, 78, 80, 83, 85, 87, 89, 91, 93, 94, 96, 98, 100, 101, 103, 104, 105, 106, 108, 109, 110, 110, 111, 112, 112, 112, 113, 113, 113, 113, 112, 112, 112, 111, 111, 110, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 98, 97, 96, 95, 93, 92, 90, 89, 88, 86, 85, 83, 82, 80, 79, 78, 76, 75, 73, 72, 71, 70, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 59, 58, 58, 57, 57, 56, 56, 56, 56
};

unsigned int covids_pointers[COVIDS_MAX];
unsigned int covid_pointer;
unsigned char covid_x, covid_y, covids_hit, covids_phase, covid_frame, covids_rate;
unsigned char covids_states[COVIDS_MAX];

const unsigned char *covidXtable;
const unsigned char *covidYtable;


void galagaInit() {
	starship_x = 100;
	starship_y = 200-8;
	starship_state = 1 | STARSHIP_AUTOPILOT;
}

void covidsInit(unsigned char phase) {
	if (!(starship_state&STARSHIP_AUTOPILOT))
		sfx_play(SFX_COVID_RESPAWN,2);
	covids_rate = 24 + (rand8()&15);
	for(i=0;i<COVIDS_MAX;++i) {
		covids_pointers[i] = i*covids_rate;
		covids_states[i] = 0;
		covids_hit = 0;
	}
	switch (phase) {
		case 0:
			covidXtable = covid_pathX1;
			covidYtable = covid_pathY1;
			break;
		case 1:
			covidXtable = covid_pathX2;
			covidYtable = covid_pathY2;
			break;
		case 2:
			covidXtable = covid_pathX3;
			covidYtable = covid_pathY3;
			break;
		case 3:
			covidXtable = covid_pathX4;
			covidYtable = covid_pathY4;
			break;
	};
	covid_frame = 0;
}

void fx_galaga() {
	
	pad_prev=pad_trigger(0);
	pad = pad_poll(0);

	// Disable autopilot if any joypad button pressed
	if (pad&255)
		starship_state &= (255 ^ STARSHIP_AUTOPILOT);

	// Autopilot
	if (starship_state&STARSHIP_AUTOPILOT && !starship_pause) {
		if (starship_x < starship_toX) {
			if (starship_x<256-8) {
				++starship_x;
				++starship_x;
			}
		} else {
			if (starship_x>8) {
				--starship_x;
				--starship_x;
			}
		}
		if (starship_x==starship_toX && !bullet_y) {
			bullet_y = starship_y-16;
			bullet_x = starship_x-4;
			starship_pause = 30 + (rand8()&7);
		}
	} else {
	// Manual controls
		if (pad&PAD_LEFT && starship_x>8) {
			--starship_x;
			--starship_x;
		}
		if (pad&PAD_RIGHT && starship_x<256-8) {
			++starship_x;
			++starship_x;
		}
		if (pad_prev&(PAD_A|PAD_B) && !bullet_y) {
			sfx_play(SFX_SHOT,0);
			bullet_y = starship_y-16;
			bullet_x = starship_x-4;
		}
	}

	if (starship_pause)
		--starship_pause;

	// Processing Covid-19 viruses
	for(i=0;i<COVIDS_MAX;++i)
	{
		covid_pointer = covids_pointers[i];
		covid_x = covidXtable[covid_pointer];
		covid_y = covidYtable[covid_pointer];

		if (!covids_states[i]) {
			spr=oam_meta_spr(covid_x, covid_y, spr, seq_covid19[covid_frame]);
			covids_pointers[i] = (covid_pointer + 1) & 511;
			starship_toX = covid_x + 12;
		} else {
			if (covids_states[i]<27+9) {
				spr=oam_meta_spr(covid_x, covid_y, spr, covid_explode[covids_states[i]]);
				++covids_states[i];
			}

			if (covids_states[i]==27+9) {
				++covids_hit;
				++covids_states[i];
				if (covids_hit==COVIDS_MAX) {
					covids_phase = (covids_phase + 1) & 3;
					covidsInit(covids_phase);
				}
			}
			
		}

		if (bullet_x>covid_x && bullet_x<covid_x+24 && bullet_y>covid_y && bullet_y<covid_y+24 && !covids_states[i]) {
			if (!(starship_state&STARSHIP_AUTOPILOT))
				sfx_play(SFX_COVID_ELIMINATED,1);
			covids_states[i] = 1;
			bullet_y = 0;
		}
	}

	if (!(nesclock&3)) {
		++covid_frame;
		pal_col(30, starship_pal[starship_flame]);
		starship_flame = (starship_flame + 1) & 3;
	}
	
	if (covid_frame>2)
		covid_frame=0;

	spr=oam_meta_spr(starship_x, starship_y, spr, spr_starship);

	if (bullet_y) {
		spr=oam_spr(bullet_x, bullet_y, 0x84, 3, spr);
		bullet_y -= 4;
		if (bullet_y<4)
			bullet_y = 0;
	}

}

void main(void)
{
	set_vram_buffer();
	clear_vram_buffer();
 	
 
 	fx_NesDev();
 	
	vram_adr(NAMETABLE_B);
	vram_unrle(NAM_multi_logo_A);

 	fx_Krujeva();

	oam_spr(255, 0, 0xFF, 3 | OAM_BEHIND, 0); //244 219 210
	set_nmi_user_call_off();

	ppu_wait_nmi();
	ppu_wait_nmi();
	ppu_wait_nmi();
	ppu_wait_nmi();

 	ppu_off();
	scrollpos = (sine_Table_Shake[logoPos]&0xfffe);
	scroll(scrollpos, 0);
	oam_clear();
 
	vram_adr(NAMETABLE_A);
	vram_unrle(NAM_multi_logo_A);
	//vram_adr(NAMETABLE_B);
	//vram_unrle(NAM_multi_logo_B);	
	pal_bg(paletteIn[0]);
	pal_spr(palette_spr[0]);	
	cnrom_set_bank(0);
	bank_spr(1);
	ppu_on_all();

	galagaInit();
	covidsInit(0);

	music_stop();
	music_play(1);
	
	while(1)
	{

		muspos = get_mus_pos();
		clear_vram_buffer();

		spr = 4;

		scrollpos = (sine_Table_Shake[logoPos]&0xfffe);
		scroll(scrollpos, 0);

		fx_galaga();

		oam_spr(20*8, 201, 0x01, 1 | OAM_BEHIND, 0);

//		if (muspos==192)
//			pal_col(0,21);

		if (nesclock&1 && muspos > 1124) {
			++logoPos;
			if (logoPos>127)
				logoPos=0;
		}

		if ((nesclock&1) == 0 && paletteId < 6) {
			pal_bg(paletteIn[paletteId]);
			++paletteId;
		}

		if (paletteId == 6 && (nesclock&15) == 0) {
			if (++palRollId1 >= 48) {
				palRollId1 = 0;
			}
			if (++palRollId2 >= 48) {
				palRollId2 = 0;
			}
			//roll_scroll_colors(palRollList[palRollId1], palRollList[palRollId2], palRollList[palRollId2]);
			palette[13] = palRollList[palRollId1];
			palette[14] = palRollList[palRollId2];
			pal_bg(palette);
		}
		
		if ((nesclock&3) == 0) {
			palSamoletId ^= 1;
			palette_spr[0][14] = palSamolet[palSamoletId];
			palette_spr[1][14] = palSamolet[palSamoletId];
			palette_spr[2][14] = palSamolet[palSamoletId];
			palette_spr[3][14] = palSamolet[palSamoletId];
			palette_spr[4][14] = palSamolet[palSamoletId];
		}
		
		paletteSprId = eq_Noise_Val > 4 ? 4 : paletteSprId;
		pal_spr(palette_spr[paletteSprId]);
		if (paletteSprId && (nesclock&7) == 0) {
			--paletteSprId;
		}

		fx_EQ();
		fx_SplitScroll();
		++nesclock;

		ppu_wait_nmi();
		oam_clear();
	}
}