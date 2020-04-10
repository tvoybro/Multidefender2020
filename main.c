// Multimatograf 2020 gamevitro

#include "Include/neslib.h"
#include "Include/nesdoug.h"
#include "Include/font4x4.h"

#define high_byte(a) *((unsigned char*)&a+1)
#define low_byte(a) *((unsigned char*)&a)

#define COVIDS_MAX				6

#define SFX_SHOT 				0x00
#define SFX_COVID_ELIMINATED 	0x01
#define SFX_COVID_RESPAWN		0x02
#define SFX_TELEGA_FLY			0x03
#define SFX_TELEGA_OUT			0x04
#define SFX_BOSS_SPAWN			0x05
#define SFX_BOSS_HIT			0x06
#define SFX_BOSS_DEFEATED		0x07

#define MUS_PATTERN				448

#define EQOFFSET 				0x20
#define EQ_CHR_OFF 				0xCD
#define EQ_CHR_ON 				0xA8

#define STARSHIP_AUTOPILOT 		0b10000000
#define CMD_LEFT		 		0x01
#define CMD_RIGHT		 		0x02

#define BOSS_START		 		0x01
#define BOSS_KILLED		 		0xff
#define BOSS_DEFEATED	 		0xfe


#define BOSS_MAX_HP				6

unsigned char isNtsc;


#pragma bss-name (push,"ZEROPAGE")
unsigned char hiTextY;
unsigned char hiTextX;
unsigned int hiPage;
unsigned int hiPointer;
unsigned char covidQty, covidLiveQty;
unsigned char logoPos, logoX, nesclock = 0;
unsigned char ishighscore, isboss;
unsigned char hs_strings_y;
unsigned int highscore_timer;

unsigned char i = 0;
unsigned char j = 0;

unsigned char fxFaze = 0;
unsigned char tileset;
unsigned int muspos;
unsigned int bossAttractTimer = 0;
unsigned char buffa[64];

unsigned int starship_x, starship_newx;
signed int starship_vx;
unsigned char starship_y, starship_state, starship_toX, starship_pause, starship_flame, starship_stunned, starship_command, starship_x8;
unsigned char bullet_x, bullet_y, bullet_timeout;
unsigned char _fs = 0;
unsigned char scrollPointer = 0;
unsigned char scrollSym = 0;
unsigned char scrollerCharPos = 0;
unsigned int scrollerCharIndex = 0;
unsigned int scrollerPos = 0;
unsigned int scrollerAddr = 0;
unsigned int scrollpos = 0;

// Boss vars
unsigned int bossIndex = 0;
unsigned char bossAttack = 0;
unsigned char bossAttackTimeout = 255;
unsigned char bossCovidY = 255;
unsigned char bossFlash = 0;
unsigned char bossCovidX1;
unsigned char bossCovidX2;
unsigned char bossCovidX3;
unsigned char bossX, bossY;
unsigned char bossDefeatedCounter;
unsigned char bossDefeatedPhase;

#pragma bss-name (pop)


unsigned char bossExplodeX[] = { 0, 0, 0, 0 };
unsigned char bossExplodeY[] = { 0, 0, 0, 0 };


unsigned char highscore_strings_offsets[8] = { 0, 2, 4, 6, 8, 10, 12, 14 };
unsigned char bossHealth = 15;

extern unsigned char FT_BUF[];
extern unsigned char NAM_krujeva[];
extern unsigned char NAM_multi_logo_A[];
extern unsigned char NAM_multi_logo_B[];
extern unsigned char NAM_nesdev_A[];


unsigned char paletteId = 0;

const unsigned char paletteIn[6][16]={
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


const unsigned char palette[16]={
	0x30,0x22,0x13,0x01,
	0x30,0x16,0x13,0x22,
	0x30,0x16,0x31,0x22,
	0x30,0x2A,0x1A,0x01
};



unsigned char palRollId = 0;
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
const unsigned char palette_spr_init[16]={
	0x0f,0x00,0x06,0x10,	0x0f,0x0f,0x16,0x30,	0x0f,0x0f,0x16,0x26,	0x21,0x14,0x24,0x30
};
const unsigned char palette_spr[5][4]={
	{0x0f,0x00,0x06,0x10},
	{0x0f,0x10,0x06,0x36},
	{0x0f,0x27,0x06,0x30},
	{0x0f,0x27,0x16,0x30},
	{0x0f,0x38,0x26,0x30},
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
	
/*
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
*/
};

const unsigned char covid19_0_data[]={

	  0,  0,0x02,0,
	  8,  0,0x03,0,
	 16,  0,0x04,0,
	  0, 8,0x12,0,
	  8, 8,0x13,0,
	 16, 8,0x14,0,
	  0, 16,0x22,0,
	  8, 16,0x23,0,
	 16, 16,0x24,0,
	128

};

const unsigned char covid19_1_data[]={

	  0,  0,0x05,0,
	 8,  0,0x06,0,
	 16,  0,0x07,0,
	  0, 8,0x15,0,
	 8, 8,0x16,0,
	 16, 8,0x17,0,
	  0, 16,0x25,0,
	 8, 16,0x26,0,
	 16, 16,0x27,0,
	128

};

const unsigned char covid19_2_data[]={

	  0,  0,0x08,0,
	 8,  0,0x09,0,
	 16,  0,0x0a,0,
	  0, 8,0x18,0,
	 8, 8,0x19,0,
	 16, 8,0x1a,0,
	  0, 16,0x28,0,
	 8, 16,0x29,0,
	 16, 16,0x2a,0,
	128

};

const unsigned char* const seq_covid19[]={

	covid19_0_data,
	covid19_1_data,
	covid19_2_data

};

const unsigned char covid_explode_0_data[]={

	  8,  8,0x7d,3,
	128

};

const unsigned char covid_explode_1_data[]={

	  4,  4,0xfe,3,
	 12,  4,0xfe,3|OAM_FLIP_H,
	  4, 12,0xfe,3|OAM_FLIP_V,
	 12, 12,0xfe,3|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_2_data[]={

	  4,  4,0xff,3,
	 12,  4,0xf0,3,
	  4, 12,0xf1,3,
	 12, 12,0xf2,3,
	128

};

const unsigned char covid_explode_3_data[]={

	  4,  4,0xf3,3,
	 12,  4,0xf4,3,
	  4, 12,0xf5,3,
	 12, 12,0xf6,3,
	128

};

const unsigned char covid_explode_4_data[]={

	  4,  4,0xf2,3|OAM_FLIP_H|OAM_FLIP_V,
	 12,  4,0xf1,3|OAM_FLIP_H|OAM_FLIP_V,
	  4, 12,0xf0,3|OAM_FLIP_H|OAM_FLIP_V,
	 12, 12,0xff,3|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_5_data[]={

	 12, 12,0xf3,3|OAM_FLIP_H|OAM_FLIP_V,
	  4, 12,0xf4,3|OAM_FLIP_H|OAM_FLIP_V,
	 12,  4,0xf5,3|OAM_FLIP_H|OAM_FLIP_V,
	  4,  4,0xf6,3|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char covid_explode_6_data[]={

	  4,  4,0xf7,3,
	 12,  4,0xf8,3,
	  4, 12,0xf9,3,
	 12, 12,0xfa,3,
	128

};

const unsigned char covid_explode_7_data[]={

	  8,  8,0xfb,3,
	128

};

const unsigned char covid_explode_8_data[]={

	  8,  8,0xfc,3,
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

const unsigned char* const boss_explode[]={
	covid_explode_0_data,
	covid_explode_1_data,
	covid_explode_2_data,
	covid_explode_3_data,
	covid_explode_4_data,
	covid_explode_5_data,
	covid_explode_6_data,
	covid_explode_7_data,
	covid_explode_8_data
};


const unsigned char spr_starship[]={

	- 8,- 8,0xeb,3,
	  0,- 8,0xec,3,
	- 8,  0,0xed,3,
	  0,  0,0xee,3,
	128

};

const unsigned char const starship_pal[] = {
	0x17, 0x27, 0x05, 0x27
};

const char scrollerData[] =  {
"HEY MAN! DID YOU WASH YOUR HANDS? HAVE YOU STOCKED UP ON TOILET PAPER? HOPE YOU WIPED OUT YOUR HANDS AND CARTRIDGE WITH DEMO-SPIRIT BEFORE INSERTING IT INTO THE CONSOLE? OKAY!"
" SO, RIGHT AFTER THE END OF THE VIRUS APOCALYPSE, WE ARE WAITING FOR ALL SURVIVORS AT MULTIMATOGRAF 2020 THIS SUMMER! THIS IS AN OLDSCHOOL-AIMED DEMOPARTY TAKING PLACE IN VOLOGDA, RUSSIA."
" WE HAVE YOUR FAVORITE COMPOS: OLDSCHOOL DEMO, INTRO 256B, GRAPHICS, MUSIC. ANIMATION, WILD, ASCII AND ANSI GRAPHICS, TINY MP3. REMOTE ENTRIES ARE ALLOWED. A HAPPY AND HEALTHY DEMOPARTY SPECIAL FOR YOU!"
" AND FOR NOW, STAY HOME AND MAKE A PRODS!            ARE YOU STILL READING? PRESS START AND KICK SOME ASS!                                                 "
};
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

void restoreBossPalette(void){
	pal_col(21, 0x0f);
	pal_col(22, 0x16);
	pal_col(25, 0x0f);
	pal_col(26, 0x16);
	pal_col(27, 0x26);
}

void chr_to_nametable(unsigned int nametable, unsigned char *src) {
	for (i=0;i<16;++i) {
		vram_adr((int) src+(i*64));
		vram_read(buffa, 64);
		vram_adr(nametable+(i*64));
		vram_write(buffa, 64);
	}
}

void fx_EQ(void)
{

	if (ishighscore)
		return;

	if (eq_Triangle_Volume && (nesclock&1)==0) {
		--eq_Triangle_Volume;
	}
	
	if ((FT_BUF[6]&1) && eq_Triangle_Volume < 3)
		eq_Triangle_Volume = 5;

	if (eq_noise_approx[FT_BUF[9] & 0x0f] > eq_Noise_Volume) {
		eq_Noise_Volume = 1 + eq_noise_approx[FT_BUF[9] & 0x0f];
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

	//gray_line();
	xy_split(scrollerPos, 210 + 5 - huita[eq_Noise_Val]/* - 2 - eq_Noise_Val*/);

	if (eq_Noise_Val)
		--eq_Noise_Val;
	scrollerPos = (scrollerPos + 1) & 511;


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

const unsigned char telegaPal[4] = {
	0x0F,0x1C,0x21,0x30
};

unsigned char nesDevPlayCtrl = 0;

void fx_NesDev(void)
{
	//nesdev
	pal_clear();
	//pal_bg(palNesdev[14]);
	pal_spr(telegaPal);
	pal_spr_bright(0);
	cnrom_set_bank(1);
	bank_spr(0);
	chr_to_nametable(NAMETABLE_A, NAM_nesdev_A);
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
				pal_spr_bright(telegaPalId > 4 ? 4 : telegaPalId);
				if (telegaX < 192+8) {
					telegaX += 4;	// 2 4
					telegaY -= 3;	// 1 3
				}
				if ((nesclock & 3) == 0 && telegaPalId < 6) {
					++telegaPalId;
					//pal_spr(telegaPal[telegaPalId - 1]);
					
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
			// sound telega out
			if ((nesDevPlayCtrl&4) == 0) {
				nesDevPlayCtrl |= 4;
				sfx_play(SFX_TELEGA_OUT,0);
			}
			
			if (nesdevPalId < 14 && ((nesclock & 3) == 0)) {
				++nesdevPalId;
				pal_bg_bright(14 - nesdevPalId);
				//pal_bg(palNesdev[nesdevPalId]);
			}
			if (nesclock == 128) {
				nesdevFaze = 4;
			}

			// telega out
			telegaX += 4;
			telegaY -= 3;
			
			if ((nesclock&1) && telegaPalId > 0) {
				--telegaPalId;
				pal_spr_bright(telegaPalId > 4 ? 4 : telegaPalId);
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

const unsigned char krujBgPal[8] = {
	0x0F,0x0F,0x20,0x30,	0x0F,0x20,0x20,0x30
	/*{0x0F,0x0F,0x22,0x32,	0x0F,0x22,0x22,0x32},
	{0x0F,0x0F,0x22,0x32,	0x0F,0x22,0x22,0x32},
	{0x0F,0x0F,0x22,0x22,	0x0F,0x22,0x22,0x22},
	{0x0F,0x0F,0x12,0x12,	0x0F,0x12,0x12,0x12},
	{0x0F,0x0F,0x02,0x02,	0x0F,0x02,0x02,0x02},
	{0x0F,0x0F,0x0F,0x0F,	0x0F,0x0F,0x0F,0x0F},*/
};
const unsigned char krujSprPal[16] = {
	0x0F,0x30,0x0F,0x0F,
	0x0F,0x30,0x30,0x0F,
	0x0F,0x30,0x30,0x30,
	0x0F,0x0F,0x30,0x16
};

unsigned char krujAnimaId1 = 0;
unsigned char krujAnimaLenId1 = 0;
unsigned char krujAnimaId2 = 0;
unsigned char krujAnimaLenId2 = 0;
unsigned char krujAnimaId3 = 0;
unsigned char krujAnimaLenId3 = 0;

unsigned char krujOffest2 = 3;

const unsigned char krujAnimaLen1[43] = {
4, 2, 3, 3, 3, 4, 4, 4, 6, 6, 4, 6, 4, 4, 6, 4, 6, 4, 6, 2, 2, 4, 4, 4, 6, 4, 4, 3, 3, 3, 10, 10, 6, 6, 12, 8, 4, 6, 6, 4, 12, 6, 10
};
const unsigned char krujAnimaChr1[222][3] = {
{33, 169, 177}, {33, 201, 192}, {33, 170, 178}, {33, 168, 176}, {33, 202, 193}, {33, 200, 191}, {33, 232, 9}, {33, 233, 10}, {33, 234, 11}, {34, 8, 190}, {34, 9, 23}, {34, 10, 191}, {34, 40, 192}, {34, 41, 36}, {34, 42, 193}, {34, 40, 194}, {34, 72, 195}, {34, 42, 196}, {34, 74, 197}, {34, 39, 198}, {34, 71, 199}, {34, 43, 200}, {34, 75, 201}, {34, 38, 202}, {34, 70, 46}, {34, 44, 203}, {34, 76, 52}, {34, 44, 39}, {34, 45, 27}, {34, 77, 204}, {34, 38, 33}, {34, 37, 32}, {34, 69, 205}, {34, 12, 206}, {34, 13, 14}, {33, 237, 1}, {34, 6, 207}, {34, 5, 19}, {33, 229, 6}, {33, 236, 13}, {34, 12, 26}, {33, 230, 7}, {34, 6, 20}, {33, 235, 12}, {34, 11, 25}, {34, 10, 24}, {33, 231, 8}, {34, 7, 21}, {34, 8, 22}, {34, 42, 37}, {34, 43, 38}, {34, 40, 35}, {34, 39, 34}, {34, 74, 50}, {34, 75, 51}, {34, 72, 48}, {34, 71, 47}, {34, 106, 63}, {34, 107, 208}, {34, 139, 0}, {34, 104, 61}, {34, 103, 209}, {34, 135, 0}, {34, 139, 210}, {34, 140, 211}, {34, 135, 212}, {34, 134, 213}, {34, 171, 214}, {34, 172, 87}, {34, 140, 215}, {34, 167, 216}, {34, 166, 81}, {34, 134, 217}, {34, 173, 75}, {34, 205, 218}, {34, 165, 80}, {34, 197, 219}, {34, 174, 76}, {34, 206, 220}, {34, 164, 79}, {34, 196, 221}, {34, 175, 0}, {34, 163, 0}, {34, 175, 77}, {34, 163, 78}, {34, 143, 66}, {34, 131, 67}, {34, 111, 55}, {34, 99, 56}, {34, 110, 0}, {34, 100, 0}, {34, 100, 57}, {34, 68, 44}, {34, 110, 54}, {34, 78, 41}, {34, 69, 45}, {34, 101, 58}, {34, 77, 40}, {34, 109, 53}, {34, 103, 60}, {34, 102, 59}, {34, 134, 68}, {34, 107, 64}, {34, 108, 65}, {34, 140, 74}, {34, 139, 73}, {34, 138, 72}, {34, 135, 69}, {34, 136, 70}, {34, 171, 86}, {34, 170, 85}, {34, 167, 82}, {34, 168, 83}, {34, 200, 96}, {34, 201, 0}, {34, 202, 98}, {34, 232, 109}, {34, 233, 110}, {34, 234, 111}, {35, 8, 122}, {35, 9, 123}, {35, 10, 124}, {35, 42, 136}, {35, 43, 222}, {35, 40, 134}, {35, 39, 223}, {35, 138, 175}, {35, 170, 187}, {35, 171, 188}, {35, 136, 173}, {35, 168, 186}, {35, 167, 185}, {35, 39, 133}, {35, 71, 146}, {35, 70, 0}, {35, 43, 137}, {35, 75, 150}, {35, 76, 0}, {35, 139, 176}, {35, 107, 163}, {35, 135, 172}, {35, 103, 159}, {35, 76, 151}, {35, 70, 145}, {35, 108, 164}, {35, 140, 224}, {35, 102, 158}, {35, 134, 225}, {35, 77, 139}, {35, 78, 0}, {35, 69, 144}, {35, 68, 0}, {35, 109, 152}, {35, 101, 157}, {35, 78, 226}, {35, 79, 0}, {35, 46, 0}, {35, 68, 227}, {35, 67, 0}, {35, 36, 0}, {35, 100, 228}, {35, 132, 229}, {35, 131, 0}, {35, 110, 230}, {35, 142, 231}, {35, 143, 0}, {35, 79, 232}, {35, 47, 233}, {35, 46, 234}, {35, 67, 235}, {35, 35, 236}, {35, 36, 237}, {35, 143, 238}, {35, 131, 239}, {35, 15, 116}, {35, 3, 117}, {35, 175, 180}, {35, 163, 181}, {34, 227, 104}, {34, 195, 91}, {34, 239, 103}, {34, 207, 90}, {34, 238, 0}, {34, 228, 0}, {34, 238, 102}, {34, 206, 89}, {34, 205, 240}, {34, 228, 105}, {34, 196, 92}, {34, 197, 241}, {34, 205, 88}, {34, 237, 101}, {34, 197, 93}, {34, 229, 106}, {35, 13, 114}, {35, 14, 0}, {35, 45, 0}, {35, 5, 119}, {35, 4, 0}, {35, 37, 0}, {35, 141, 165}, {35, 173, 178}, {35, 174, 179}, {35, 133, 170}, {35, 165, 183}, {35, 164, 182}, {35, 37, 131}, {35, 36, 130}, {35, 45, 126}, {35, 46, 127}, {35, 142, 166}, {35, 132, 169}, {35, 78, 140}, {35, 79, 141}, {35, 47, 128}, {35, 68, 143}, {35, 67, 142}, {35, 35, 129}, {35, 111, 154}, {35, 143, 167}, {35, 99, 155}, {35, 131, 168}
};
const unsigned char krujAnimaSpr1[222][4] = {
{104, 2, 0, 72}, {112, 3, 0, 72}, {104, 4, 0, 80}, {104, 4, 64, 64}, {112, 5, 0, 80}, {112, 5, 64, 64}, {120, 6, 0, 64}, {120, 7, 0, 72}, {120, 6, 64, 80}, {128, 8, 0, 64}, {128, 9, 0, 72}, {128, 8, 64, 80}, {136, 10, 0, 64}, {136, 11, 0, 72}, {136, 10, 64, 80}, {136, 12, 0, 64}, {144, 13, 0, 64}, {136, 12, 64, 80}, {144, 13, 64, 80}, {136, 14, 0, 56}, {144, 15, 0, 56}, {136, 14, 64, 88}, {144, 15, 64, 88}, {136, 16, 0, 48}, {144, 17, 0, 48}, {136, 16, 64, 96}, {144, 17, 64, 96}, {136, 18, 0, 96}, {136, 19, 0, 104}, {144, 20, 0, 104}, {136, 18, 64, 48}, {136, 19, 64, 40}, {144, 20, 64, 40}, {128, 21, 0, 96}, {128, 22, 0, 104}, {120, 23, 0, 104}, {128, 21, 64, 48}, {128, 22, 64, 40}, {120, 23, 64, 40}, {120, 24, 0, 96}, {128, 25, 0, 96}, {120, 24, 64, 48}, {128, 25, 64, 48}, {120, 26, 0, 88}, {128, 27, 0, 88}, {128, 28, 0, 80}, {120, 26, 64, 56}, {128, 27, 64, 56}, {128, 28, 64, 64}, {136, 29, 0, 80}, {136, 30, 0, 88}, {136, 29, 64, 64}, {136, 30, 64, 56}, {144, 31, 0, 80}, {144, 32, 0, 88}, {144, 31, 64, 64}, {144, 32, 64, 56}, {152, 33, 0, 80}, {152, 34, 0, 88}, {160, 35, 0, 88}, {152, 33, 64, 64}, {152, 34, 64, 56}, {160, 35, 64, 56}, {160, 36, 0, 88}, {160, 37, 0, 96}, {160, 36, 64, 56}, {160, 37, 64, 48}, {168, 38, 0, 88}, {168, 39, 0, 96}, {160, 40, 0, 96}, {168, 38, 64, 56}, {168, 39, 64, 48}, {160, 40, 64, 48}, {168, 41, 0, 104}, {176, 42, 0, 104}, {168, 41, 64, 40}, {176, 42, 64, 40}, {168, 43, 0, 112}, {176, 44, 0, 112}, {168, 43, 64, 32}, {176, 44, 64, 32}, {168, 45, 0, 120}, {168, 45, 64, 24}, {168, 46, 0, 120}, {168, 46, 64, 24}, {160, 47, 0, 120}, {160, 47, 64, 24}, {152, 48, 0, 120}, {152, 48, 64, 24}, {152, 49, 0, 112}, {152, 49, 64, 32}, {152, 50, 0, 32}, {144, 51, 0, 32}, {152, 50, 64, 112}, {144, 51, 64, 112}, {144, 52, 0, 40}, {152, 53, 0, 40}, {144, 52, 64, 104}, {152, 53, 64, 104}, {152, 54, 0, 56}, {152, 55, 0, 48}, {160, 56, 0, 48}, {152, 54, 64, 88}, {152, 55, 64, 96}, {160, 56, 64, 96}, {160, 57, 0, 88}, {160, 58, 0, 80}, {160, 57, 64, 56}, {160, 58, 64, 64}, {168, 59, 0, 88}, {168, 60, 0, 80}, {168, 59, 64, 56}, {168, 60, 64, 64}, {176, 61, 0, 64}, {176, 62, 0, 72}, {176, 61, 64, 80}, {184, 63, 0, 64}, {184, 64, 0, 72}, {184, 63, 64, 80}, {192, 65, 0, 64}, {192, 66, 0, 72}, {192, 65, 64, 80}, {200, 67, 0, 80}, {200, 68, 0, 88}, {200, 67, 64, 64}, {200, 68, 64, 56}, {224, 69, 0, 80}, {232, 70, 0, 80}, {232, 71, 0, 88}, {224, 69, 64, 64}, {232, 70, 64, 64}, {232, 71, 64, 56}, {200, 72, 0, 56}, {208, 73, 0, 56}, {208, 74, 0, 48}, {200, 72, 64, 88}, {208, 73, 64, 88}, {208, 74, 64, 96}, {224, 75, 0, 88}, {216, 76, 0, 88}, {224, 75, 64, 56}, {216, 76, 64, 56}, {208, 77, 0, 96}, {208, 77, 64, 48}, {216, 78, 0, 96}, {224, 79, 0, 96}, {216, 78, 64, 48}, {224, 79, 64, 48}, {208, 80, 0, 104}, {208, 81, 0, 112}, {208, 80, 64, 40}, {208, 81, 64, 32}, {216, 82, 0, 104}, {216, 82, 64, 40}, {208, 83, 0, 112}, {208, 84, 0, 120}, {200, 85, 0, 112}, {208, 83, 64, 32}, {208, 84, 64, 24}, {200, 85, 64, 32}, {216, 86, 0, 32}, {224, 87, 0, 32}, {224, 88, 0, 24}, {216, 86, 64, 112}, {224, 87, 64, 112}, {224, 88, 64, 120}, {208, 89, 0, 120}, {200, 90, 0, 120}, {200, 91, 0, 112}, {208, 89, 64, 24}, {200, 90, 64, 24}, {200, 91, 64, 32}, {224, 92, 0, 120}, {224, 92, 64, 24}, {192, 93, 0, 120}, {192, 93, 64, 24}, {232, 94, 0, 120}, {232, 94, 64, 24}, {184, 95, 0, 24}, {176, 96, 0, 24}, {184, 95, 64, 120}, {176, 96, 64, 120}, {184, 97, 0, 112}, {184, 97, 64, 32}, {184, 98, 0, 112}, {176, 99, 0, 112}, {176, 100, 0, 104}, {184, 98, 64, 32}, {176, 99, 64, 32}, {176, 100, 64, 40}, {176, 101, 0, 104}, {184, 102, 0, 104}, {176, 101, 64, 40}, {184, 102, 64, 40}, {192, 103, 0, 104}, {192, 104, 0, 112}, {200, 105, 0, 104}, {192, 103, 64, 40}, {192, 104, 64, 32}, {200, 105, 64, 40}, {224, 106, 0, 104}, {232, 107, 0, 104}, {232, 108, 0, 112}, {224, 106, 64, 40}, {232, 107, 64, 40}, {232, 108, 64, 32}, {200, 109, 0, 40}, {200, 110, 0, 32}, {200, 109, 64, 104}, {200, 110, 64, 112}, {224, 111, 0, 112}, {224, 111, 64, 32}, {208, 112, 0, 112}, {208, 113, 0, 120}, {200, 114, 0, 120}, {208, 112, 64, 32}, {208, 113, 64, 24}, {200, 114, 64, 24}, {216, 115, 0, 120}, {224, 116, 0, 120}, {216, 115, 64, 24}, {224, 116, 64, 24}
};
const unsigned char krujAnimaLen2[41] = {
2, 4, 4, 2, 4, 4, 4, 2, 6, 6, 2, 2, 4, 6, 4, 4, 2, 2, 4, 6, 4, 6, 6, 2, 4, 4, 4, 4, 6, 8, 4, 12, 8, 4, 6, 4, 4, 4, 6, 12, 8
};
const unsigned char krujAnimaChr2[194][3] = {
{33, 239, 3}, {33, 240, 4}, {33, 206, 184}, {33, 207, 196}, {33, 208, 197}, {33, 209, 187}, {33, 174, 198}, {33, 175, 181}, {33, 176, 182}, {33, 177, 199}, {33, 143, 157}, {33, 144, 158}, {33, 111, 200}, {33, 112, 201}, {33, 110, 0}, {33, 113, 0}, {33, 110, 202}, {33, 78, 203}, {33, 113, 204}, {33, 81, 205}, {33, 77, 206}, {33, 109, 142}, {33, 82, 207}, {33, 114, 147}, {33, 115, 208}, {33, 108, 209}, {33, 139, 166}, {33, 140, 167}, {33, 148, 162}, {33, 147, 161}, {33, 179, 174}, {33, 172, 180}, {33, 173, 168}, {33, 174, 169}, {33, 141, 155}, {33, 178, 173}, {33, 177, 172}, {33, 146, 160}, {33, 142, 156}, {33, 145, 159}, {33, 110, 143}, {33, 113, 146}, {33, 78, 130}, {33, 79, 131}, {33, 80, 132}, {33, 81, 133}, {33, 46, 210}, {33, 49, 211}, {33, 17, 212}, {33, 14, 213}, {33, 45, 214}, {33, 50, 215}, {33, 13, 216}, {32, 237, 92}, {33, 18, 217}, {32, 242, 97}, {33, 19, 111}, {32, 243, 98}, {33, 12, 115}, {32, 236, 104}, {32, 244, 99}, {32, 235, 103}, {32, 245, 100}, {32, 234, 102}, {33, 21, 112}, {33, 22, 113}, {33, 10, 114}, {33, 9, 113}, {33, 54, 125}, {33, 53, 124}, {33, 41, 125}, {33, 42, 126}, {33, 85, 137}, {33, 74, 139}, {33, 75, 140}, {33, 43, 127}, {33, 84, 136}, {33, 52, 123}, {33, 51, 122}, {33, 83, 135}, {33, 82, 134}, {33, 44, 128}, {33, 76, 141}, {33, 77, 129}, {33, 49, 120}, {33, 50, 121}, {33, 18, 110}, {33, 46, 117}, {33, 45, 116}, {33, 13, 105}, {33, 17, 109}, {33, 14, 106}, {32, 238, 93}, {32, 239, 94}, {32, 240, 95}, {32, 241, 96}, {32, 206, 80}, {32, 207, 81}, {32, 208, 82}, {32, 209, 83}, {32, 174, 67}, {32, 175, 68}, {32, 176, 69}, {32, 177, 70}, {32, 142, 54}, {32, 143, 55}, {32, 144, 56}, {32, 145, 57}, {32, 110, 41}, {32, 111, 42}, {32, 112, 43}, {32, 113, 44}, {32, 14, 2}, {32, 17, 5}, {32, 109, 40}, {32, 77, 27}, {32, 114, 45}, {32, 82, 32}, {32, 13, 1}, {32, 45, 14}, {32, 18, 6}, {32, 50, 19}, {32, 76, 39}, {32, 83, 33}, {32, 44, 26}, {32, 51, 20}, {32, 74, 218}, {32, 75, 38}, {32, 107, 219}, {32, 85, 220}, {32, 84, 34}, {32, 116, 221}, {32, 20, 0}, {32, 52, 21}, {32, 53, 222}, {32, 11, 0}, {32, 43, 25}, {32, 42, 223}, {32, 117, 224}, {32, 118, 225}, {32, 106, 226}, {32, 105, 225}, {32, 10, 227}, {32, 9, 228}, {32, 21, 229}, {32, 22, 228}, {32, 137, 62}, {32, 138, 63}, {32, 150, 62}, {32, 149, 61}, {32, 181, 230}, {32, 182, 75}, {32, 214, 88}, {32, 170, 231}, {32, 169, 75}, {32, 201, 88}, {32, 181, 74}, {32, 213, 87}, {32, 170, 76}, {32, 202, 89}, {32, 203, 90}, {32, 212, 86}, {32, 211, 85}, {32, 204, 91}, {32, 171, 77}, {32, 172, 78}, {32, 180, 73}, {32, 179, 72}, {32, 139, 64}, {32, 140, 65}, {32, 108, 52}, {32, 148, 60}, {32, 147, 59}, {32, 115, 46}, {32, 116, 47}, {32, 117, 48}, {32, 85, 35}, {32, 107, 51}, {32, 106, 50}, {32, 74, 37}, {32, 20, 232}, {32, 21, 9}, {32, 53, 22}, {32, 11, 233}, {32, 10, 11}, {32, 42, 24}, {32, 86, 36}, {32, 118, 49}, {32, 73, 36}, {32, 105, 49}, {32, 22, 10}, {32, 54, 23}, {32, 9, 10}, {32, 41, 23}
};
const unsigned char krujAnimaSpr2[194][4] = {
{120, 117, 0, 120}, {120, 118, 0, 128}, {112, 119, 0, 112}, {112, 120, 0, 120}, {112, 120, 64, 128}, {112, 119, 64, 136}, {104, 121, 0, 112}, {104, 122, 0, 120}, {104, 122, 64, 128}, {104, 121, 64, 136}, {96, 123, 0, 120}, {96, 123, 64, 128}, {88, 124, 0, 120}, {88, 124, 64, 128}, {88, 125, 0, 112}, {88, 125, 64, 136}, {88, 126, 0, 112}, {80, 127, 0, 112}, {88, 126, 64, 136}, {80, 127, 64, 136}, {80, 128, 0, 104}, {88, 129, 0, 104}, {80, 128, 64, 144}, {88, 129, 64, 144}, {88, 130, 0, 152}, {88, 130, 64, 96}, {96, 131, 0, 88}, {96, 132, 0, 96}, {96, 131, 64, 160}, {96, 132, 64, 152}, {104, 133, 0, 152}, {104, 133, 64, 96}, {104, 134, 0, 104}, {104, 135, 0, 112}, {96, 136, 0, 104}, {104, 134, 64, 144}, {104, 135, 64, 136}, {96, 136, 64, 144}, {96, 137, 0, 112}, {96, 137, 64, 136}, {88, 138, 0, 112}, {88, 138, 64, 136}, {80, 139, 0, 112}, {80, 140, 0, 120}, {80, 140, 64, 128}, {80, 139, 64, 136}, {72, 141, 0, 112}, {72, 141, 64, 136}, {64, 142, 0, 136}, {64, 142, 64, 112}, {72, 143, 0, 104}, {72, 143, 64, 144}, {64, 144, 0, 104}, {56, 145, 0, 104}, {64, 144, 64, 144}, {56, 145, 64, 144}, {64, 146, 0, 152}, {56, 147, 0, 152}, {64, 146, 64, 96}, {56, 147, 64, 96}, {56, 148, 0, 160}, {56, 148, 64, 88}, {56, 149, 0, 168}, {56, 149, 64, 80}, {64, 150, 0, 168}, {64, 151, 0, 176}, {64, 150, 64, 80}, {64, 151, 0, 72}, {72, 152, 0, 176}, {72, 153, 0, 168}, {72, 152, 0, 72}, {72, 153, 64, 80}, {80, 154, 0, 168}, {80, 154, 64, 80}, {80, 155, 0, 88}, {72, 156, 0, 88}, {80, 155, 64, 160}, {72, 156, 64, 160}, {72, 157, 0, 152}, {80, 158, 0, 152}, {80, 159, 0, 144}, {72, 157, 64, 96}, {80, 158, 64, 96}, {80, 159, 64, 104}, {72, 160, 0, 136}, {72, 161, 0, 144}, {64, 162, 0, 144}, {72, 160, 64, 112}, {72, 161, 64, 104}, {64, 162, 64, 104}, {64, 163, 0, 136}, {64, 163, 64, 112}, {56, 164, 0, 112}, {56, 165, 0, 120}, {56, 165, 64, 128}, {56, 164, 64, 136}, {48, 166, 0, 112}, {48, 167, 0, 120}, {48, 167, 64, 128}, {48, 166, 64, 136}, {40, 168, 0, 112}, {40, 169, 0, 120}, {40, 169, 64, 128}, {40, 168, 64, 136}, {32, 170, 0, 112}, {32, 171, 0, 120}, {32, 171, 64, 128}, {32, 170, 64, 136}, {24, 172, 0, 112}, {24, 173, 0, 120}, {24, 174, 0, 128}, {24, 172, 64, 136}, {0, 175, 0, 112}, {0, 175, 64, 136}, {24, 176, 0, 104}, {16, 177, 0, 104}, {24, 176, 64, 144}, {16, 177, 64, 144}, {0, 178, 0, 104}, {8, 179, 0, 104}, {0, 178, 64, 144}, {8, 179, 64, 144}, {16, 180, 0, 96}, {16, 180, 64, 152}, {8, 181, 0, 96}, {8, 181, 64, 152}, {16, 182, 0, 80}, {16, 183, 0, 88}, {24, 184, 0, 88}, {16, 182, 64, 168}, {16, 183, 64, 160}, {24, 184, 64, 160}, {0, 0, 0, 160}, {8, 185, 0, 160}, {8, 186, 0, 168}, {0, 0, 0, 88}, {8, 185, 64, 88}, {8, 186, 64, 80}, {24, 187, 0, 168}, {24, 188, 0, 176}, {24, 187, 64, 80}, {24, 188, 0, 72}, {0, 189, 0, 80}, {0, 190, 0, 72}, {0, 189, 64, 168}, {0, 190, 0, 176}, {32, 191, 0, 72}, {32, 192, 0, 80}, {32, 191, 0, 176}, {32, 192, 64, 168}, {40, 193, 0, 168}, {40, 194, 0, 176}, {48, 195, 0, 176}, {40, 193, 64, 80}, {40, 194, 0, 72}, {48, 195, 0, 72}, {40, 196, 0, 168}, {48, 197, 0, 168}, {40, 196, 64, 80}, {48, 197, 64, 80}, {48, 198, 0, 88}, {48, 198, 64, 160}, {48, 199, 0, 152}, {48, 199, 64, 96}, {40, 200, 0, 88}, {40, 201, 0, 96}, {40, 200, 64, 160}, {40, 201, 64, 152}, {32, 202, 0, 88}, {32, 203, 0, 96}, {24, 0, 0, 96}, {32, 202, 64, 160}, {32, 203, 64, 152}, {24, 0, 0, 152}, {24, 204, 0, 160}, {24, 205, 0, 168}, {16, 206, 0, 168}, {24, 204, 64, 88}, {24, 205, 64, 80}, {16, 206, 64, 80}, {0, 207, 0, 160}, {0, 208, 0, 168}, {8, 209, 0, 168}, {0, 207, 64, 88}, {0, 208, 64, 80}, {8, 209, 64, 80}, {16, 210, 0, 176}, {24, 211, 0, 176}, {16, 210, 0, 72}, {24, 211, 0, 72}, {0, 212, 0, 176}, {8, 213, 0, 176}, {0, 212, 0, 72}, {8, 213, 0, 72}
};
const unsigned char krujAnimaLen3[10] = {
14, 9, 9, 14, 14, 12, 10, 10, 12, 12
};
const unsigned char krujAnimaChr3[116][3] = {
{34, 14, 15}, {34, 15, 16}, {34, 16, 17}, {34, 17, 18}, {34, 46, 28}, {34, 47, 29}, {34, 48, 30}, {34, 49, 31}, {34, 78, 242}, {34, 79, 42}, {34, 80, 43}, {34, 81, 243}, {34, 111, 244}, {34, 112, 245}, {33, 72, 234}, {33, 73, 138}, {33, 74, 235}, {33, 104, 150}, {33, 105, 151}, {33, 106, 152}, {33, 136, 163}, {33, 137, 164}, {33, 138, 165}, {33, 85, 234}, {33, 86, 138}, {33, 87, 235}, {33, 117, 150}, {33, 118, 151}, {33, 119, 152}, {33, 149, 163}, {33, 150, 164}, {33, 151, 165}, {34, 1, 15}, {34, 2, 16}, {34, 3, 17}, {34, 4, 18}, {34, 33, 28}, {34, 34, 29}, {34, 35, 30}, {34, 36, 31}, {34, 65, 242}, {34, 66, 42}, {34, 67, 43}, {34, 68, 243}, {34, 98, 244}, {34, 99, 245}, {34, 27, 15}, {34, 28, 16}, {34, 29, 17}, {34, 30, 18}, {34, 59, 28}, {34, 60, 29}, {34, 61, 30}, {34, 62, 31}, {34, 91, 242}, {34, 92, 42}, {34, 93, 43}, {34, 94, 243}, {34, 124, 244}, {34, 125, 245}, {32, 15, 236}, {32, 16, 237}, {32, 46, 238}, {32, 47, 16}, {32, 48, 17}, {32, 49, 239}, {32, 78, 240}, {32, 79, 29}, {32, 80, 30}, {32, 81, 241}, {32, 111, 242}, {32, 112, 243}, {35, 41, 135}, {35, 72, 246}, {35, 73, 148}, {35, 74, 247}, {35, 104, 160}, {35, 105, 161}, {35, 106, 162}, {35, 136, 248}, {35, 137, 174}, {35, 138, 249}, {35, 54, 135}, {35, 85, 246}, {35, 86, 148}, {35, 87, 247}, {35, 117, 160}, {35, 118, 161}, {35, 119, 162}, {35, 149, 248}, {35, 150, 174}, {35, 151, 249}, {32, 2, 236}, {32, 3, 237}, {32, 33, 238}, {32, 34, 16}, {32, 35, 17}, {32, 36, 239}, {32, 65, 240}, {32, 66, 29}, {32, 67, 30}, {32, 68, 241}, {32, 98, 242}, {32, 99, 243}, {32, 28, 236}, {32, 29, 237}, {32, 59, 238}, {32, 60, 16}, {32, 61, 17}, {32, 62, 239}, {32, 91, 240}, {32, 92, 29}, {32, 93, 30}, {32, 94, 241}, {32, 124, 242}, {32, 125, 243}
};


const unsigned char krujAnimaFrame3[10] = {
0,3,7, 9,10,    14,17,21, 23,24
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
unsigned char kmfList3[20] = { 0xe1,0xfa, 0xc6,0x15, 0xe1,0x15, 0xab,0x30, 0xc6,0x30, 0xe1,0x30, 0x90,0x4b, 0xab,0x4b, 0xc6,0x4b, 0xe1,0x4b };
unsigned char kmfList4[20] = { 0x19,0xfa, 0x19,0x15, 0x34,0x15, 0x19,0x30, 0x34,0x30, 0x4f,0x30, 0x19,0x4b, 0x34,0x4b, 0x4f,0x4b, 0x6a,0x4b };

const unsigned char kmfDir1[30] = {0xff,0xff, 0xfe,0xff, 0xfd,0xff, 0xfc,0xff, 0xff,0xfe, 0xfe,0xfe, 0xfd,0xfe, 0xfc,0xfe, 0xff,0xfd, 0xfe,0xfd, 0xfd,0xfd, 0xfc,0xfd, 0xff,0xfc, 0xfe,0xfc, 0xfd,0xfc };
const unsigned char kmfDir2[30] = {0x4,0xff, 0x3,0xff, 0x2,0xff, 0x1,0xff, 0x4,0xfe, 0x3,0xfe, 0x2,0xfe, 0x1,0xfe, 0x4,0xfd, 0x3,0xfd, 0x2,0xfd, 0x1,0xfd, 0x3,0xfc, 0x2,0xfc, 0x1,0xfc };
const unsigned char kmfDir3[20] = {0xfc,0x4, 0xfd,0x3, 0xfc,0x3, 0xfe,0x2, 0xfd,0x2, 0xfc,0x2, 0xff,0x1, 0xfe,0x1, 0xfd,0x1, 0xfc,0x1 };
unsigned char kmfDir4[20] = {0x4,0x4, 0x4,0x3, 0x3,0x3, 0x4,0x2, 0x3,0x2, 0x2,0x2, 0x4,0x1, 0x3,0x1, 0x2,0x1, 0x1,0x1 };

unsigned char kmfStep = 11;

void fx_Krujeva(void)
{
	
	isNtsc = ppu_system() == 0 ? 0 : 1;
	pal_clear();
	pal_spr(krujSprPal);
	pal_bg(krujBgPal);
	cnrom_set_bank(1);
	bank_spr(1);
	chr_to_nametable(NAMETABLE_A, NAM_krujeva);
	cnrom_set_bank(3);
	oam_clear();
	oam_spr(2, 118, 0xFF, 3 | OAM_BEHIND, 0); //253 244 219 210
	set_nmi_user_call_on(2);
	ppu_on_all();

	ppu_wait_nmi();
	pal_bright(4);
	music_play(0);

	while (krujFrm < (isNtsc ? 48 : 46)) //(isNtsc ? 48 : 46))
	{
		++nesclock;
		oam_spr(2, 118, 0xFF, 3 | OAM_BEHIND, 0); //253 244 219 210
		ppu_wait_nmi();
		clear_vram_buffer();
		
		
		if (krujFrm > 44 && krujBgPalId < 4) {
			if ((nesclock&3) == 0) {
				++krujBgPalId;
				//pal_bg(krujBgPal[krujBgPalId]);
				pal_bg_bright(4- krujBgPalId);
			}
		}

		// krujeva 1
		if (++krujWait >= krujCheck[isNtsc][krujCheckId]) {
			krujWait = 0;
			if (++krujPalId == 3) {
				krujPalId = 0;
				krujFrm++;
				//do char 1
				if (krujAnimaLenId1 < 43) {
					j = krujAnimaLen1[krujAnimaLenId1];
					for (i = 0; i < j; i++) {
						krujChr = krujAnimaChr1[krujAnimaId1][2];
						if (krujChr != 0) {
							krujHi = 256 * krujAnimaChr1[krujAnimaId1][0];
							krujLo = krujAnimaChr1[krujAnimaId1][1];

							one_vram_buffer(krujChr, krujHi + krujLo);
							one_vram_buffer(krujChr, krujHi + krujLo + 13);
							krujX = krujAnimaSpr1[krujAnimaId1][3];

							if (krujX <= 256-8-208) {
								one_vram_buffer(krujChr, krujHi + krujLo + 26);
							}
							if (krujX >= 13*8) {
								one_vram_buffer(krujChr, krujHi + krujLo - 13);
							}
						}
						++krujAnimaId1;
					}
					++krujAnimaLenId1;
				}
			}
			krujCheckId ^= 1;
		}
		
		// krujeva 2
		if (krujWait == 2 && krujPalId == 0) {
			//do char 2
			if (krujOffest2) {
				--krujOffest2;
			} else {
				if (krujAnimaLenId2 < 41) {
					j = krujAnimaLen2[krujAnimaLenId2];
					for (i = 0; i < j; i++) {
						krujChr = krujAnimaChr2[krujAnimaId2][2];
						if (krujChr != 0) {
							krujHi = 256 * krujAnimaChr2[krujAnimaId2][0];
							krujLo = krujAnimaChr2[krujAnimaId2][1];
							one_vram_buffer(krujChr, krujHi + krujLo);
							krujX = krujAnimaSpr2[krujAnimaId2][3];
							if (krujX < (32-13)*8) {
								one_vram_buffer(krujChr, krujHi + krujLo + 13);
							}
							if (krujX >= 13*8) {
								one_vram_buffer(krujChr, krujHi + krujLo - 13);
							}
						}
						++krujAnimaId2;
					}
					++krujAnimaLenId2;
				}
			}
		}
		
		// flowers
		if (krujWait == 2 && krujPalId == 0) {
			if (krujAnimaLenId3 < 10 && krujFrm >= krujAnimaFrame3[krujAnimaLenId3]) {
				j = krujAnimaLen3[krujAnimaLenId3];
				for (i = 0; i < j; i++) {
					krujChr = krujAnimaChr3[krujAnimaId3][2];
					krujHi = 256 * krujAnimaChr3[krujAnimaId3][0];
					krujLo = krujAnimaChr3[krujAnimaId3][1];
					one_vram_buffer(krujChr, krujHi + krujLo);
					++krujAnimaId3;
				}
				++krujAnimaLenId3;
			}
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

		// do sprite 1
		if (krujWait == 1) {
			if (krujAnimaLenId1 < 43) {
				spr = 4;
				j = krujAnimaLen1[krujAnimaLenId1];
				for (i = 0; i < j; i++) {
					// +0=y +1=spr +2=attr +3=x
					// oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);
					krujChr = krujAnimaSpr1[krujAnimaId1+i][1];
					if (krujChr != 0) {
						krujY = krujAnimaSpr1[krujAnimaId1+i][0] - 1;
						krujX = krujAnimaSpr1[krujAnimaId1+i][3];
						krujAttr = krujPalId + krujAnimaSpr1[krujAnimaId1+i][2];
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

		// do sprite 2
		if (krujWait == 3 && krujOffest2 == 0) {
			if (krujAnimaLenId2 < 41) {
				j = krujAnimaLen2[krujAnimaLenId2];
				for (i = 0; i < j; i++) {
					// +0=y +1=spr +2=attr +3=x
					// oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);
					krujChr = krujAnimaSpr2[krujAnimaId2+i][1];
					if (krujChr != 0) {
						krujY = krujAnimaSpr2[krujAnimaId2+i][0] - 1;
						krujX = krujAnimaSpr2[krujAnimaId2+i][3];
						krujAttr = krujPalId + krujAnimaSpr2[krujAnimaId2+i][2];
						spr = oam_spr(krujX, krujY, krujChr, krujAttr, spr);
						if (krujX <= 256-8-104) {
							spr = oam_spr(krujX + 104, krujY, krujChr, krujAttr, spr);
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
unsigned int points = 0;
unsigned char points_array[3] = {0, 0, 0};
unsigned char covid_x, covid_y, covids_hit, covids_phase, covid_frame, covids_rate, damage_lag;
unsigned char covids_states[COVIDS_MAX];

const unsigned char *covidXtable;
const unsigned char *covidYtable;


void galagaInit(void) {
	starship_x = 100*256;
	starship_y = 200-8;
	starship_state = 1 | STARSHIP_AUTOPILOT;
}

void covidsInit(unsigned char phase) {
	covidLiveQty = COVIDS_MAX;
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

void earnpoint(unsigned char pluspoint) {
	if (pluspoint)
		++points;
	else
		--points;
	if (points>999)
		points=0;
	points_array[0]=0xc0+points/100;
	points_array[1]=0xc0+points/10%10;
	points_array[2]=0xc0+points%10; 
}

void fx_galaga(void) {


	pad_prev=pad_trigger(0);
	pad = pad_poll(0);

	spr=oam_spr(256-8-24,16,points_array[0],3,spr);
	spr=oam_spr(256-8-16,16,points_array[1],3,spr);
	spr=oam_spr(256-8-8,16,points_array[2],3,spr);

	// Disable autopilot if any joypad button pressed
	if (pad_prev&PAD_START) {
		if (starship_state&STARSHIP_AUTOPILOT) {
			points = 999;
			earnpoint(1);
			starship_state &= (255 ^ STARSHIP_AUTOPILOT);
			sfx_play(SFX_TELEGA_FLY,0);
			pad_prev = 0;
			isboss = 0;
			covidsInit(covids_phase);
			covidQty = 0;
			fxFaze = 0;
		}
	}

	starship_x8 = high_byte(starship_x);
	starship_command = 0;

	// Autopilot
	if (starship_state&STARSHIP_AUTOPILOT && !starship_pause) {
		if (starship_x8 < starship_toX) {
			starship_command = CMD_RIGHT;
		} else {
			if (starship_x8 > starship_toX) {
				starship_command = CMD_LEFT;
			}
		}
		if (covidQty == COVIDS_MAX || isboss) {
			if (covidLiveQty && starship_x8==starship_toX && !bullet_y) {
				bullet_y = starship_y-16;
				bullet_x = starship_x8-4;
				starship_pause = 30 + (rand8()&7);
			}
		}
	} else {
	// Manual controls
		if (!starship_stunned) {


			if (pad&PAD_LEFT) {
				starship_command = CMD_LEFT;
			}
			if (pad&PAD_RIGHT) {
				starship_command = CMD_RIGHT;
			}
			if (pad_prev&(PAD_A|PAD_B) && !bullet_y && !bullet_timeout) {
				sfx_play(SFX_SHOT,0);
				bullet_y = starship_y-16;
				bullet_x = starship_x8-4;
				bullet_timeout = 30;
			}
			if (pad_prev&PAD_START) {
				starship_state |= STARSHIP_AUTOPILOT;
				points = 999;
				earnpoint(1);
				sfx_play(SFX_TELEGA_FLY,0);
			}
		}
	}

	if (starship_command==CMD_LEFT) {
		if (starship_vx>-500) {
			starship_vx-=48;
		}
	} else {
		if (starship_vx < 0) {
			starship_vx += 48;
			if (starship_vx > 0) 
				starship_vx = 0;
		}
	}

	if (starship_command==CMD_RIGHT) {
		if (starship_vx<500) {
			starship_vx+=48;
		}
	} else
		if (starship_vx > 0) {
			starship_vx -= 48;
			if (starship_vx < 0)
				starship_vx = 0;
		}


	starship_newx = starship_x + starship_vx;
	if (high_byte(starship_newx)<8 || high_byte(starship_newx)>256-8) {
		starship_vx = 0;	
	} else
		starship_x += starship_vx;

	if (starship_pause)
		--starship_pause;
	if (starship_stunned) {
		--starship_stunned;
		if (!starship_stunned)
			damage_lag = 0;
	}

	if (!(nesclock&3)) {
		pal_col(30, starship_pal[starship_flame]);
		starship_flame = (starship_flame + 1) & 3;
	}

	if (starship_stunned) {
		if (nesclock&2)
			spr=oam_meta_spr(starship_x8, starship_y, spr, spr_starship);
	} else {
		spr=oam_meta_spr(starship_x8, starship_y, spr, spr_starship);
	}

	if (bullet_timeout)
		--bullet_timeout;

	if (bullet_y) {
		spr=oam_spr(bullet_x, bullet_y, 0xef, 3, spr);
		bullet_y -= 4;
		if (bullet_y<4)
			bullet_y = 0;
	}
}

void fx_Covid19(void) {

	// Processing Covid-19 viruses
	for(i=0;i<COVIDS_MAX;++i)
	{
		covid_pointer = covids_pointers[i];
		covid_x = covidXtable[covid_pointer];
		covid_y = covidYtable[covid_pointer];

		if (!covids_states[i]) {
			if (i < covidQty) {
				spr=oam_meta_spr(covid_x, covid_y, spr, seq_covid19[covid_frame]);
			}
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
					if (starship_state&STARSHIP_AUTOPILOT) {
						pal_col(27, 0x04);
						pal_col(25, 0x30);
						ishighscore = 1;
						highscore_timer = 60*6;
					}
					covidQty = 0;
				}
			}
			
		}

		if (i < covidQty && bullet_x>covid_x && bullet_x<covid_x+24 && bullet_y>covid_y && bullet_y<covid_y+24 && !covids_states[i]) {
			if (!(starship_state&STARSHIP_AUTOPILOT))
				sfx_play(SFX_COVID_ELIMINATED,1);
			covids_states[i] = 1;
			--covidLiveQty;
			bullet_y = 0;
			earnpoint(1);
		}
	}

	if (!(nesclock&3)) {
		++covid_frame;
	}
	
	if (covid_frame>2)
		covid_frame=0;

	if (covidQty < COVIDS_MAX && eq_Noise_Val > 5 && !ishighscore) {
		++covidQty;
	}

}

const unsigned char highscore_table_0_data[]={

	  0,  0,0xd8,2,
	 16,  0,0xd9,2,
	 48,  0,0xe3,2,
	 64,  0,0xd3,2,
	 80,  0,0xdf,2,
	 96,  0,0xe2,2,
	 112,  0,0xd5,2,
	128

};

const unsigned char highscore_table_1_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc1,2,
	128

};

const unsigned char highscore_table_2_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc2,2,
	128

};

const unsigned char highscore_table_3_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc3,2,
	128

};

const unsigned char highscore_table_4_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc4,2,
	128

};

const unsigned char highscore_table_5_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc5,2,
	128

};

const unsigned char highscore_table_6_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc6,2,
	128

};

const unsigned char highscore_table_7_data[]={

	  0,  0,0xe0,2,
	 16,  0,0xdc,2,
	 32,  0,0xd1,2,
	 48,  0,0xe9,2,
	 64,  0,0xd5,2,
	 80,  0,0xe2,2,
	 112,  0,0xc7,2,
	128

};

const unsigned char* const highscore_table[]={

	highscore_table_0_data,
	highscore_table_1_data,
	highscore_table_2_data,
	highscore_table_3_data,
	highscore_table_4_data,
	highscore_table_5_data,
	highscore_table_6_data,
	highscore_table_7_data

};


const unsigned int sineTableTextBobbling[32]={
0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 8, 
7, 7, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 
0
};

const char greets_list[]={
	"SHIRU   "
	"NINTENDO"
	"THESUPER"
	"DEMARCHE"
	"EXCESSTM"
	"SANDS   "

	"KPACKU  "
	"FENOMEN "
	"MEGUS   "
	"SERZHSFT"
	"GEMBABOY"
	" DR. MAX"

	"QUITE   "
	"R.M.D.A "
	"HOOYPROG"
	"   VERVE"
	"SPECCYPL"
	"STARDUST"

	"TITAN   "
	"ABYSS   "
	"ERRORSFT"
	"   BOOZE"
	" LOONIES"
	" KEWLERS"

	"COCOON  "
	"ATEBIT  "
	"FAIRLGHT"
	"SPACEBLS"
	"ASD, LSD"
	"MFX, TBL"

	"RZR1911 "
	"CNSPRCY "
	"HAUJOBB "	
	"  CENSOR"
	" FRBRSCH"
	"    CNCD"

	"MAYHEM  "
	"EPHIDRNA"
	"LOGICOMA"
	"BITFELLS"
	" MERCURY"
	"   ELUDE"
};

void fx_highscore(void) {

	scroll(256, 0);
	hs_strings_y = 40;
	hiPage = hiPointer;
	for (i=0; i<6; ++i) {
		hiTextY = hs_strings_y + *(sineTableTextBobbling + highscore_strings_offsets[i]);
		hiTextX = 20;

		if (i>2)
			hiTextX += 100;

		for (j=0; j<8; ++j) {
			oam_spr(hiTextX, hiTextY, greets_list[hiPage+j]+144, 2, spr);
			spr+=4;
			hiTextX += 16;
		}
		if (nesclock&1) {
			++highscore_strings_offsets[i];
			highscore_strings_offsets[i] &= 31;
		}
		hs_strings_y += 16;
		
		if (i==2)
			hs_strings_y += 24;
		
		hiPage += 8;
	}
	--highscore_timer;
	if (!highscore_timer) {
		hiPointer += 6*8;
		if (hiPointer>6*48)
			hiPointer = 0;

		pal_col(27, 0x26);
		pal_col(25, 0x0f);


		ishighscore = 0;
		starship_x = 100*256;
		fxFaze++;
		if (!(fxFaze&3)) {
			isboss = 1;
			bossAttack = 0;
			bossCovidY = 255;
			bossAttackTimeout = 255;
			restoreBossPalette();
			bossHealth = 15;
			bossAttractTimer = 60*25;
		}
	}
}


const unsigned char boss_0_data[]={
	-32,- 6,0x30,2,
	-24,- 2,0x31,2,
	-16,  0,0x32,2,
	- 8,  0,0x33,2,
	- 1,  0,0x33,2|OAM_FLIP_H,
	  7,  0,0x32,2|OAM_FLIP_H,
	 15,- 2,0x31,2|OAM_FLIP_H,
	 23,- 6,0x30,2|OAM_FLIP_H,
	-32,  2,0x40,2,
	-24,  6,0x41,2,
	-16,  8,0x42,2,
	- 8,  8,0x43,1,
	- 1,  8,0x43,1|OAM_FLIP_H,
	  7,  8,0x42,2|OAM_FLIP_H,
	 15,  6,0x41,2|OAM_FLIP_H,
	 23,  2,0x40,2|OAM_FLIP_H,
	-32, 10,0x50,2,
	-24, 14,0x51,2,
	-16, 16,0x52,2,
	- 8, 16,0x53,2,
	- 1, 16,0x53,2|OAM_FLIP_H,
	  7, 16,0x52,2|OAM_FLIP_H,
	 15, 14,0x51,2|OAM_FLIP_H,
	 23, 10,0x50,2|OAM_FLIP_H,
	- 8, 24,0x63,2,
	- 1, 24,0x63,2|OAM_FLIP_H,
	  7, 24,0x62,2|OAM_FLIP_H,
	-16, 24,0x62,2,
	128
};

const unsigned char boss_1_data[]={
	-24,- 2,0x35,2,
	-16,- 2,0x32,2,
	- 8,- 2,0x33,2,
	- 1,- 2,0x33,2|OAM_FLIP_H,
	  7,- 2,0x36,2|OAM_FLIP_H,
	 15,- 2,0x35,2|OAM_FLIP_H,
	-32,  6,0x44,2,
	-24,  6,0x45,2,
	-16,  6,0x46,2,
	- 8,  6,0x43,1,
	- 1,  6,0x43,1|OAM_FLIP_H,
	  7,  6,0x46,2|OAM_FLIP_H,
	 15,  6,0x45,2|OAM_FLIP_H,
	 23,  6,0x44,2|OAM_FLIP_H,
	-32, 14,0x54,2,
	-24, 14,0x55,2,
	-16, 14,0x56,2,
	- 8, 14,0x53,2,
	- 1, 14,0x53,2|OAM_FLIP_H,
	  7, 14,0x56,2|OAM_FLIP_H,
	 15, 14,0x55,2|OAM_FLIP_H,
	 23, 14,0x54,2|OAM_FLIP_H,
	- 8, 22,0x63,2,
	- 1, 22,0x63,2|OAM_FLIP_H,
	  6, 23,0x62,2|OAM_FLIP_H,
	-15, 23,0x62,2,
	128
};

const unsigned char* const boss_list[]={
	boss_0_data,
	boss_1_data
};

void hitPlayer(void) {
	++damage_lag;
	if (damage_lag>1)
		return;
	if (points)
		earnpoint(0);
	starship_stunned = 60*2;
	starship_vx = 0;
	if (!(starship_state&STARSHIP_AUTOPILOT))
		sfx_play(SFX_COVID_ELIMINATED, 0);
	if (bossHealth<BOSS_MAX_HP) {
		++bossHealth;
		if (bossHealth==6)
			restoreBossPalette();
	}
}

void bossFight(void)
{

	if (isboss==BOSS_KILLED) {
		music_stop();
		bossX -= 32;
		bossDefeatedCounter = 0;
		bossDefeatedPhase = 0;
		--isboss;
	} 
	if (isboss==BOSS_DEFEATED) {
		if (!bossDefeatedCounter) {
			for (i=0; i<4; ++i) {
				bossExplodeX[i] = rand8()&31;
				bossExplodeY[i] = rand8()&15;
			}
			sfx_play(SFX_COVID_ELIMINATED,0);
		} else {
			for (i=0; i<4; ++i)
				spr=oam_meta_spr(bossX + bossExplodeX[i], bossY + bossExplodeY[i], spr, boss_explode[bossDefeatedCounter]);
		}
		if (!(nesclock&1))
			++bossDefeatedCounter;
		if (bossDefeatedCounter>8) {
			earnpoint(1);
			++bossDefeatedPhase;
			bossDefeatedCounter = 0;
		}
		if (bossDefeatedPhase>10) {
			isboss = 0;
			music_play(0);
		}
	}
	if (isboss==BOSS_START) {
		bossX = 56 + (2 * covidXtable[bossIndex])/3;
		bossY = covidYtable[bossIndex]+18;
		
		spr = oam_meta_spr(bossX, bossY, spr, boss_list[(nesclock&(bossAttack ? 4 : 8)) ? 1 : 0]);
		

		if (bossAttack) {
			if (bossAttack == 1) {
				//do attack
				bossCovidX1 = bossX;
				bossCovidX2 = bossX - 16;
				bossCovidX3 = bossX + 16;
				bossCovidY = covidYtable[bossIndex] + 8;
				bossAttackTimeout = 255;
				if (!(starship_state&STARSHIP_AUTOPILOT))
					sfx_play(SFX_COVID_RESPAWN,1);
			}
			--bossAttack;

			if (!(nesclock&2))
				pal_col(22, 0x01);
			else
				pal_col(22, 0x25);


		} else {

			starship_toX = bossX;
			if (bossY>0x87) {
				if (starship_x8>bossX)
					starship_toX = bossX+40;
				else
					starship_toX = bossX-40;
			}
			pal_col(22, 0x16);
			bossIndex = (bossIndex + 1) & 511;
			if (bossAttackTimeout) {
				--bossAttackTimeout;
			} else {
				if ( covidYtable[bossIndex] < 55 && eq_Noise_Val > 5) {
					bossAttack = 60;
					if (rand8()>127)
						starship_toX = 0;
					else
						starship_toX = 255;
				}
			}
			if (!bossAttractTimer) {
				isboss=0;
				ishighscore = 1;
				pal_col(27, 0x04);
				pal_col(25, 0x30);
				highscore_timer = 60*6;
			}
		}

		if (isboss) {
			//boss collision
			if (!starship_stunned && starship_x8>bossX-12 && starship_x8<bossX+24-12) {
				if (bossY >= 160) {
					hitPlayer();
				}
			}

			//boss covids
			if (bossCovidY < 200) {
				spr = oam_meta_spr(bossCovidX1, bossCovidY + 24, spr, seq_covid19[covid_frame]);
				spr = oam_meta_spr(bossCovidX2, bossCovidY, spr, seq_covid19[covid_frame]);
				spr = oam_meta_spr(bossCovidX3, bossCovidY, spr, seq_covid19[covid_frame]);

				// Check collision

				if (starship_y<bossCovidY+24 && starship_y>bossCovidY) {
					if ((starship_x8<bossCovidX2+24 && starship_x8>bossCovidX2) || (starship_x8<bossCovidX3+24 && starship_x8>bossCovidX3)) {
						hitPlayer();
					}
				}

				if (starship_y<bossCovidY+48 && starship_y>bossCovidY+24) {
					if (starship_x8<bossCovidX1+24 && starship_x8>bossCovidX1) {
						hitPlayer();
					}
				}

				bossCovidX2 -= 1;
				bossCovidX3 += 1;
				bossCovidY += 4;

				if (!(nesclock&3)) {
					++covid_frame;
				}
				
				if (covid_frame>2)
					covid_frame=0;

			}
			
			if (bullet_x>bossX-16 && bullet_x<bossX+16 && bullet_y>bossY && bullet_y<bossY+24) {
				bullet_y = 0;
				if (!(starship_state&STARSHIP_AUTOPILOT))
					sfx_play(SFX_BOSS_HIT,0);
				earnpoint(1);
				bossFlash = 5;
				pal_col(21, 0x30);
				pal_col(25, 0x30);
				pal_col(26, 0x30);
				pal_col(27, 0x30);
				if (bossHealth && !(starship_state&STARSHIP_AUTOPILOT))
					--bossHealth;
				if (!bossHealth) {
					isboss = 0xff;
					return;
				}
			}

			if (bossFlash) {
				--bossFlash;
				if (!bossFlash) {
					pal_col(21, 0x0f);
					pal_col(25, 0x0f);
					pal_col(26, 0x16);
					pal_col(27, 0x26);
				}
			}
			
			// blinking boss if low hp
			if (bossHealth < (starship_state&STARSHIP_AUTOPILOT ? 3 : 6) && !bossFlash) {
				if (!(nesclock&4)) {
					restoreBossPalette();
				}
				else {
					pal_col(21, 0x0f);
					pal_col(22, 0x26);
					pal_col(25, 0x0f);
					pal_col(26, 0x26);
					pal_col(27, 0x37);
				}
			}
		}
	}
	if (bossAttractTimer && starship_state&STARSHIP_AUTOPILOT) {
		--bossAttractTimer;
	}
}


void main(void)
{
	
	set_vram_buffer();
	clear_vram_buffer();
 	
	fx_NesDev();
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

	cnrom_set_bank(1);
 	chr_to_nametable(NAMETABLE_A, NAM_multi_logo_A);
 	chr_to_nametable(NAMETABLE_B, NAM_multi_logo_B);

	pal_bg(paletteIn[0]);
	pal_spr(palette_spr_init);	
	pal_bg_bright(4);
	cnrom_set_bank(0);
	bank_spr(1);
	ppu_on_all();

	galagaInit();
	covidsInit(0);

	music_stop();
	music_play(1);

	while(1)
	{
		//ishighscore = 1;
		//isboss = 1;
		//bossAttractTimer = 60*30;

		muspos = get_mus_pos();
		clear_vram_buffer();

		spr = 4;
		
		// side spr - sprites
		if (!isboss) {
			spr = oam_spr(1, 12*8-1, 0x10, 1 | OAM_FLIP_V | OAM_FLIP_H, spr);
			spr = oam_spr(256-8, 13*8-1, 0x10, 1, spr);
		}

		if (ishighscore) {
			fx_highscore();
		} else {
			scrollpos = (sine_Table_Shake[logoPos]&0xfffe);
			scroll(scrollpos, 0);
		

			//corona spr blink
			
			paletteSprId = eq_Noise_Val > 4 ? 4 : paletteSprId;
			pal_col(16+0, palette_spr[paletteSprId][0]);
			pal_col(16+1, palette_spr[paletteSprId][1]);
			pal_col(16+2, palette_spr[paletteSprId][2]);
			pal_col(16+3, palette_spr[paletteSprId][3]);
			if (paletteSprId && (nesclock&7) == 0) {
				--paletteSprId;
			}

			if (isboss)
				bossFight();

			if (!isboss && muspos > MUS_PATTERN*2 - (MUS_PATTERN/4))
				fx_Covid19();

			if (muspos > MUS_PATTERN*3)
				fx_galaga();

			fx_EQ();
		}


		oam_spr(20*8, 201, 0x01, 1 | OAM_BEHIND, 0);

		if (nesclock&1 && muspos > MUS_PATTERN) {
			++logoPos;
			if (logoPos>127)
				logoPos=0;
		}

		// galaga engine blink
		if ((nesclock&3) == 0) {
			palSamoletId ^= 1;
			pal_col(16+14, palSamolet[palSamoletId]);
		}

		pal_bg(paletteIn[paletteId]);
		// side spr - palette
		if (!isboss) {
			pal_col(16+7, paletteIn[paletteId][10]);
		}


		// scroll palette roll
		if (paletteId == 6 && (nesclock&15) == 0) {
			if (++palRollId >= 48) {
				palRollId = 0;
			}
		}
		i = palRollId + 1;
		if (i >= 48) {
			i = 0;
		}
		pal_col(13, palRollList[palRollId]);
		pal_col(14, palRollList[i]);

		//fade in background		
		if ((nesclock&1) == 0 && paletteId < 6) {
			++paletteId;
		}

		fx_SplitScroll();
		++nesclock;

		ppu_wait_nmi();
		oam_clear();
	}
}