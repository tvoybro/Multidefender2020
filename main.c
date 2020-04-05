// Multimatograf 2020 invitro

#include "Include/neslib.h"
#include "Include/nesdoug.h"
#include "Include/font4x4.h"

#include "Gfx/NAM_multi_logo_A.h"
#include "Gfx/NAM_multi_logo_B.h"
#include "Gfx/logo_scr.h"

#define EQOFFSET 0x20
#define EQ_CHR_OFF 0xCD
#define EQ_CHR_ON 0xA8

extern char scrollText[];

extern unsigned int FT_MUSPOS;
#pragma zpsym ("FT_MUSPOS")

extern unsigned char FT_BUF[];

unsigned char pad_t;
unsigned char tileset;
unsigned int muspos;

const unsigned char palette[16]={
	0x30,0x22,0x13,0x01,
	0x30,0x16,0x13,0x22,
	0x30,0x16,0x31,0x22,
	0x30,0x2A,0x1A,0x01
};

const unsigned char palette_spr[16]={ 
	0x0f,0x00,0x06,0x10,
	0x0f,0x0c,0x0c,0x0c,
	0x0f,0x0c,0x0c,0x0c,
	0x0f,0x14,0x24,0x30
};

unsigned char palRollId = 1;
const unsigned char palRollList[12] = {
	0x3A, 0x2A, 0x1B,
	0x3A, 0x2A, 0x1B,
	0x3A, 0x2A, 0x1B,
	0x3A, 0x2A, 0x1B,
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

const unsigned char spr_covid_19[]={
	- 8,- 8,0x02,0,
	  0,- 8,0x03,0,
	  8,- 8,0x04,0,
	- 8,  0,0x12,0,
	  0,  0,0x13,0,
	  8,  0,0x14,0,
	- 8,  8,0x22,0,
	  0,  8,0x23,0,
	  8,  8,0x24,0,
	128
};


const unsigned char spr_starship[]={

	- 8,- 8,0x82,3,
	  0,- 8,0x83,3,
	- 8,  0,0x92,3,
	  0,  0,0x93,3,
	128

};

unsigned char starship_x, starship_y, starship_state;
unsigned char bullet_x, bullet_y;

const char scrollerData[] = "HELLO WORLD! BONJOUR LE MONDE! HALO A SHAOGHAIL! SALVE MUNDI SINT! HELLO VILAG! KAUPAPA HUA! CIAO MONDO! HEJ VERDEN! SAWUBONA MHLABA! SVEIKA PASAULE! HALO DUNIA! SALU MUNDU! DOMHAN HELLO! HOLA MUNDO! ... END OF SCROLLER ...              ONCE AGAIN:";

//put a string into the nametable
void put_str(unsigned int adr,const char *str)
{
	vram_adr(adr);

	while(1)
	{
		if(!*str) break;

		vram_put((*str++)-0x20);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
	}
}


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
unsigned char i, j, eq_Tile, pad;
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



// держать таблицу из адресов ppu по вертикали для каждого столбца

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
	
	set_nmi_user_call_on();
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
	
	set_nmi_user_call_on();
}

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
		

	xy_split(scrollerPos, 210 - 1 - eq_Noise_Val/* - 2 - eq_Noise_Val*/);

	if (eq_Noise_Val)
		--eq_Noise_Val;
	scrollerPos = (scrollerPos + 2) & 511;


}

const unsigned int sine_Table_Shake[] = {
65532, 65534, 1, 3, 6, 8, 11, 13, 15, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 37, 39, 40, 41, 43, 44, 44, 45, 46, 47, 47, 47, 47, 48, 47, 47, 47, 47, 46, 45, 44, 44, 43, 41, 40, 39, 37, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 15, 13, 11, 8, 6, 3, 1, 65534, 65532, 65529, 65526, 65524, 65521, 65519, 65516, 65514, 65512, 65509, 65507, 65505, 65503, 65501, 65499, 65497, 65495, 65493, 65491, 65490, 65488, 65487, 65486, 65484, 65483, 65483, 65482, 65481, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65481, 65482, 65483, 65483, 65484, 65486, 65487, 65488, 65490, 65491, 65493, 65495, 65497, 65499, 65501, 65503, 65505, 65507, 65509, 65512, 65514, 65516, 65519, 65521, 65524, 65526, 65529
};

// ---------------------------------------
// -------------- VIRUSEZ ----------------
// ---------------------------------------

#define COVIDS_MAX	4

//balls parameters

static unsigned int covids_pointers[COVIDS_MAX];
static unsigned int hotdog_pointer;

const unsigned char burger_pathX1[512] = {
16, 17, 18, 19, 21, 23, 25, 27, 30, 33, 36, 39, 42, 45, 49, 53, 57, 60, 65, 69, 73, 78, 82, 87, 91, 96, 101, 105, 110, 115, 120, 125, 130, 134, 139, 144, 149, 154, 158, 163, 167, 172, 176, 180, 184, 188, 192, 196, 200, 203, 206, 210, 213, 215, 218, 220, 222, 224, 226, 228, 229, 230, 230, 231, 231, 231, 230, 230, 229, 227, 226, 224, 222, 219, 217, 214, 211, 208, 205, 201, 197, 194, 190, 185, 181, 177, 172, 168, 163, 158, 153, 148, 144, 139, 134, 129, 124, 118, 113, 108, 103, 99, 94, 89, 84, 79, 75, 70, 66, 62, 57, 53, 50, 46, 42, 39, 36, 33, 30, 28, 25, 23, 21, 20, 18, 17, 17, 16, 16, 16, 17, 17, 18, 20, 21, 23, 25, 28, 30, 33, 36, 39, 42, 46, 50, 53, 57, 62, 66, 70, 75, 79, 84, 89, 94, 99, 103, 108, 113, 118, 124, 129, 134, 139, 144, 148, 153, 158, 163, 168, 172, 177, 181, 185, 190, 194, 197, 201, 205, 208, 211, 214, 217, 219, 222, 224, 226, 227, 229, 230, 230, 231, 231, 231, 230, 230, 229, 227, 226, 224, 222, 220, 218, 215, 212, 209, 206, 203, 199, 196, 192, 188, 184, 179, 175, 171, 166, 162, 157, 152, 148, 143, 138, 133, 128, 123, 118, 114, 109, 104, 99, 94, 90, 85, 80, 76, 71, 67, 63, 59, 55, 51, 47, 44, 41, 37, 34, 32, 29, 26, 24, 22, 20, 19, 18, 17, 16, 16, 15, 15, 16, 16, 17, 18, 19, 20, 22, 24, 25, 28, 30, 32, 35, 37, 40, 43, 46, 49, 52, 56, 59, 63, 66, 70, 74, 77, 81, 85, 89, 92, 96, 100, 104, 108, 111, 115, 119, 122, 126, 130, 133, 136, 140, 143, 146, 149, 152, 154, 157, 160, 162, 164, 166, 168, 169, 171, 172, 173, 174, 175, 175, 175, 175, 175, 175, 175, 174, 173, 173, 172, 171, 170, 168, 167, 166, 164, 162, 161, 159, 157, 155, 153, 151, 149, 147, 145, 142, 140, 138, 135, 133, 130, 128, 125, 123, 120, 118, 115, 113, 110, 108, 106, 103, 101, 98, 96, 94, 91, 89, 87, 85, 83, 81, 79, 77, 75, 74, 72, 71, 69, 68, 67, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 58, 57, 56, 55, 54, 54, 53, 52, 51, 50, 50, 49, 48, 47, 46, 46, 45, 44, 43, 42, 41, 41, 40, 39, 38, 38, 37, 36, 35, 34, 34, 33, 32, 31, 31, 30, 29, 28, 28, 27, 26, 26, 25, 24, 24, 23, 22, 22, 21, 20, 20, 19, 18, 18, 17, 17, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15
};
const unsigned char burger_pathY1[512] = {
41, 41, 41, 41, 41, 41, 41, 41, 42, 42, 42, 42, 42, 43, 43, 44, 44, 44, 45, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62, 62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 73, 74, 75, 76, 77, 77, 78, 79, 80, 81, 81, 82, 83, 84, 84, 85, 86, 87, 87, 88, 89, 90, 90, 91, 92, 93, 93, 94, 95, 96, 96, 97, 98, 99, 99, 100, 101, 102, 102, 103, 104, 104, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 113, 113, 114, 115, 116, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 125, 125, 126, 127, 128, 129, 129, 130, 131, 131, 132, 133, 133, 134, 135, 135, 136, 137, 137, 138, 139, 139, 140, 140, 141, 141, 142, 143, 143, 144, 144, 145, 145, 146, 146, 147, 148, 148, 149, 149, 150, 150, 151, 151, 152, 153, 153, 154, 154, 155, 155, 156, 157, 157, 158, 159, 159, 160, 161, 161, 162, 163, 163, 164, 165, 165, 166, 167, 168, 169, 170, 170, 171, 172, 173, 174, 175, 176, 177, 179, 180, 181, 182, 183, 184, 185, 187, 188, 189, 190, 191, 192, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 211, 212, 213, 213, 214, 214, 215, 215, 216, 216, 216, 217, 217, 217, 217, 217, 217, 216, 216, 216, 215, 215, 214, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 203, 202, 201, 199, 198, 196, 194, 193, 191, 189, 188, 186, 184, 182, 180, 178, 176, 175, 173, 171, 169, 167, 165, 163, 161, 159, 157, 155, 153, 152, 150, 148, 146, 144, 142, 141, 139, 137, 136, 134, 133, 131, 130, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 118, 117, 116, 115, 115, 114, 113, 112, 112, 111, 110, 109, 109, 108, 107, 107, 106, 105, 104, 104, 103, 102, 102, 101, 100, 100, 99, 98, 98, 97, 97, 96, 95, 95, 94, 94, 93, 93, 92, 92, 91, 91, 90, 90, 90, 89, 89, 89, 88, 88, 88, 87, 87, 87, 87, 87, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 87, 87, 87, 88, 88, 88, 89, 90, 90, 91, 91, 92, 93, 94, 95, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 113, 114, 115, 116, 117, 117, 118, 118, 119, 120, 120, 120, 121, 121, 121, 122, 122, 122, 122, 122, 122, 121, 121, 121, 120, 120, 119, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 103, 102, 100, 99, 97, 95, 94, 92, 90, 89, 87, 85, 84, 82, 80, 79, 77, 75, 73, 72, 70, 68, 67, 65, 64, 62, 61, 59, 58, 56, 55, 54, 52, 51, 50, 49, 48, 47, 46, 45, 44, 44, 43, 42, 42, 42, 41, 41
};
const unsigned char burger_pathX2[512] = {
16, 17, 17, 18, 18, 19, 19, 20, 21, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 38, 40, 42, 44, 46, 49, 51, 53, 56, 58, 60, 63, 65, 68, 70, 73, 75, 78, 80, 82, 85, 87, 90, 92, 94, 97, 99, 101, 103, 105, 108, 110, 111, 113, 115, 117, 119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 131, 133, 134, 135, 136, 138, 139, 141, 142, 144, 146, 147, 149, 151, 153, 155, 157, 159, 161, 164, 166, 168, 170, 172, 174, 177, 179, 181, 183, 185, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207, 209, 210, 212, 213, 215, 216, 217, 219, 220, 220, 221, 222, 223, 223, 224, 224, 224, 224, 224, 223, 223, 222, 221, 220, 219, 217, 215, 213, 211, 209, 206, 203, 200, 197, 194, 190, 187, 183, 179, 175, 171, 167, 163, 158, 154, 149, 145, 140, 136, 131, 127, 122, 117, 113, 108, 103, 99, 94, 90, 85, 81, 77, 73, 68, 65, 61, 57, 53, 50, 46, 43, 40, 37, 35, 32, 30, 28, 26, 24, 23, 21, 20, 20, 19, 19, 19, 19, 20, 21, 22, 23, 25, 27, 29, 31, 33, 36, 38, 41, 44, 47, 51, 54, 58, 61, 65, 69, 73, 77, 81, 85, 89, 94, 98, 103, 107, 111, 116, 120, 125, 129, 134, 138, 142, 147, 151, 155, 159, 164, 168, 171, 175, 179, 182, 186, 189, 192, 196, 198, 201, 204, 206, 208, 210, 212, 214, 215, 216, 217, 218, 219, 219, 220, 221, 222, 222, 223, 224, 224, 225, 226, 227, 227, 228, 228, 229, 230, 230, 231, 231, 232, 233, 233, 234, 234, 234, 235, 235, 236, 236, 236, 237, 237, 237, 238, 238, 238, 238, 238, 239, 239, 239, 239, 239, 239, 239, 239, 239, 238, 238, 238, 238, 238, 237, 237, 237, 236, 236, 235, 235, 234, 234, 233, 232, 231, 230, 229, 227, 225, 223, 221, 218, 215, 213, 210, 206, 203, 200, 196, 192, 189, 185, 181, 177, 172, 168, 164, 159, 155, 150, 146, 141, 137, 132, 127, 123, 118, 113, 109, 104, 100, 95, 91, 86, 82, 78, 74, 70, 66, 62, 58, 54, 51, 48, 44, 41, 38, 36, 33, 31, 29, 27, 25, 23, 22, 21, 20, 19, 19, 19, 19, 19, 19, 20, 20, 21, 22, 23, 24, 26, 27, 29, 30, 32, 34, 36, 38, 41, 43, 45, 48, 50, 53, 55, 58, 61, 63, 66, 69, 71, 74, 77, 80, 82, 85, 88, 91, 93, 96, 98, 101, 103, 105, 108, 110, 112, 114, 116, 118, 119, 121, 122, 124, 125, 126, 127, 128, 128, 129, 129, 129, 129, 129, 128, 128, 127, 126, 125, 124, 123, 122, 120, 119, 117, 115, 113, 111, 109, 107, 105, 103, 101, 98, 96, 93, 91, 88, 86, 83, 81, 78, 75, 73, 70, 68, 65, 62, 60, 57, 55, 52, 50, 48, 45, 43, 41, 39, 36, 34, 33, 31, 29, 27, 26, 24, 23, 22, 21, 20, 19, 18, 18, 17, 17
};
const unsigned char burger_pathY2[512] = {
16, 17, 17, 18, 19, 21, 22, 24, 26, 29, 31, 33, 36, 39, 42, 45, 48, 52, 55, 59, 63, 67, 70, 74, 78, 83, 87, 91, 95, 99, 104, 108, 112, 117, 121, 125, 130, 134, 138, 142, 146, 150, 154, 158, 162, 166, 169, 173, 176, 180, 183, 186, 189, 191, 194, 196, 199, 201, 202, 204, 206, 207, 208, 208, 209, 209, 210, 210, 210, 210, 210, 209, 209, 209, 208, 208, 207, 206, 205, 205, 204, 203, 202, 201, 199, 198, 197, 196, 194, 193, 192, 190, 189, 187, 186, 184, 182, 181, 179, 178, 176, 174, 173, 171, 170, 168, 166, 165, 163, 162, 160, 159, 157, 156, 155, 153, 152, 151, 149, 148, 147, 146, 145, 144, 143, 142, 141, 141, 140, 139, 139, 138, 137, 137, 136, 135, 134, 134, 133, 132, 131, 130, 129, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 82, 81, 80, 79, 78, 77, 77, 76, 75, 74, 73, 72, 70, 69, 68, 67, 66, 65, 63, 62, 61, 60, 59, 57, 56, 55, 53, 52, 51, 50, 48, 47, 46, 45, 43, 42, 41, 40, 39, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 25, 24, 23, 23, 22, 22, 21, 21, 20, 20, 20, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 23, 23, 24, 25, 26, 27, 28, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39, 41, 42, 43, 44, 46, 47, 48, 49, 51, 52, 53, 54, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 73, 74, 75, 75, 76, 77, 77, 77, 78, 79, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 94, 95, 96, 98, 99, 101, 103, 104, 106, 107, 109, 110, 112, 114, 115, 117, 119, 120, 122, 123, 125, 127, 128, 130, 131, 133, 134, 136, 137, 138, 140, 141, 142, 143, 145, 146, 147, 148, 149, 150, 150, 151, 152, 152, 153, 153, 154, 154, 154, 154, 155, 155, 155, 155, 155, 154, 154, 154, 154, 154, 153, 153, 153, 152, 152, 151, 151, 150, 150, 149, 148, 148, 147, 146, 146, 145, 144, 143, 143, 142, 141, 140, 139, 138, 137, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 115, 115, 114, 113, 112, 111, 110, 109, 108, 107, 105, 104, 102, 101, 99, 97, 95, 93, 92, 90, 88, 86, 84, 81, 79, 77, 75, 73, 71, 68, 66, 64, 62, 60, 57, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 30, 28, 27, 25, 24, 23, 22, 20, 19, 19, 18, 17, 17, 16, 16, 16, 15, 15, 16
};

const unsigned char burger_pathX3[512] = {
136, 136, 136, 136, 136, 137, 137, 137, 138, 138, 139, 139, 140, 141, 141, 142, 143, 144, 145, 146, 146, 147, 148, 149, 150, 151, 153, 154, 155, 156, 157, 158, 159, 161, 162, 163, 164, 165, 166, 168, 169, 170, 171, 172, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 189, 190, 191, 191, 192, 193, 193, 194, 194, 194, 195, 195, 195, 195, 195, 195, 196, 196, 196, 195, 195, 195, 195, 195, 195, 194, 194, 194, 193, 193, 192, 192, 192, 191, 191, 190, 189, 189, 188, 188, 187, 186, 186, 185, 184, 183, 183, 182, 181, 180, 180, 179, 178, 177, 176, 176, 175, 174, 173, 172, 171, 171, 170, 169, 168, 167, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 155, 154, 153, 151, 150, 148, 147, 145, 144, 142, 140, 139, 137, 136, 134, 132, 131, 129, 127, 126, 124, 122, 121, 119, 118, 116, 114, 113, 112, 110, 109, 107, 106, 105, 104, 103, 101, 100, 100, 99, 98, 97, 96, 96, 95, 95, 95, 94, 94, 94, 94, 94, 95, 95, 95, 96, 97, 98, 98, 99, 101, 102, 103, 104, 106, 107, 109, 111, 112, 114, 116, 118, 120, 122, 124, 126, 129, 131, 133, 135, 138, 140, 142, 145, 147, 150, 152, 154, 157, 159, 162, 164, 167, 169, 171, 174, 176, 178, 181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 204, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 216, 217, 217, 218, 218, 218, 218, 218, 218, 218, 217, 217, 217, 216, 216, 215, 214, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 203, 202, 201, 200, 198, 197, 196, 194, 193, 192, 190, 189, 187, 186, 185, 183, 182, 180, 179, 178, 176, 175, 174, 172, 171, 170, 169, 168, 167, 165, 164, 163, 162, 162, 161, 160, 159, 158, 157, 156, 155, 153, 152, 150, 149, 147, 145, 143, 141, 139, 137, 135, 132, 130, 128, 125, 123, 120, 118, 115, 113, 110, 108, 105, 103, 100, 98, 95, 93, 90, 88, 85, 83, 81, 78, 76, 74, 72, 70, 68, 66, 64, 62, 61, 59, 58, 56, 55, 54, 53, 52, 51, 50, 50, 50, 49, 49, 49, 50, 50, 51, 51, 52, 53, 54, 56, 57, 59, 60, 62, 64, 66, 68, 70, 72, 75, 77, 80, 82, 85, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 135, 138, 141, 144, 147, 149, 152, 155, 158, 160, 163, 165, 168, 170, 172, 175, 177, 179, 181, 182, 184, 186, 187, 188, 189, 190, 191, 192, 193, 193, 193, 194, 194, 194, 194, 194, 193, 193, 193, 192, 191, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 180, 179, 178, 176, 175, 174, 172, 171, 169, 168, 166, 165, 163, 162, 160, 159, 157, 156, 155, 153, 152, 150, 149, 148, 147, 146, 144, 143, 142, 141, 141, 140, 139, 138, 138, 137, 137, 136, 136, 136
};
const unsigned char burger_pathY3[512] = {
104, 104, 105, 105, 106, 106, 107, 107, 108, 108, 109, 110, 110, 111, 111, 112, 113, 113, 114, 115, 115, 116, 117, 117, 118, 119, 119, 120, 120, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 127, 128, 128, 128, 129, 129, 129, 129, 129, 130, 130, 130, 130, 130, 130, 130, 129, 129, 129, 129, 128, 128, 127, 127, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 88, 87, 86, 85, 85, 84, 83, 83, 82, 81, 81, 80, 80, 80, 79, 79, 79, 78, 78, 78, 78, 78, 78, 78, 78, 78, 79, 79, 79, 80, 80, 80, 81, 81, 82, 82, 83, 84, 84, 85, 86, 86, 87, 88, 89, 89, 90, 91, 92, 93, 94, 95, 96, 97, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 108, 109, 110, 111, 112, 113, 113, 114, 115, 116, 116, 117, 118, 118, 119, 120, 120, 121, 121, 121, 122, 122, 122, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 122, 122, 122, 122, 121, 121, 121, 120, 120, 120, 119, 119, 119, 118, 118, 118, 117, 117, 117, 116, 116, 115, 115, 115, 115, 114, 114, 114, 113, 113, 113, 113, 113, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 113, 113, 113, 113, 113, 113, 114, 114, 114, 114, 114, 115, 115, 115, 115, 116, 116, 116, 116, 117, 117, 117, 117, 118, 118, 118, 119, 119, 119, 120, 120, 120, 120, 121, 121, 121, 122, 122, 122, 123, 123, 123, 123, 124, 124, 124, 125, 125, 125, 126, 126, 126, 126, 127, 127, 127, 127, 128, 128, 128, 128, 129, 129, 129, 130, 130, 131, 131, 132, 132, 133, 134, 135, 135, 136, 137, 138, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 159, 160, 160, 161, 161, 162, 162, 162, 162, 162, 163, 163, 163, 162, 162, 162, 162, 161, 161, 161, 160, 159, 159, 158, 157, 156, 155, 155, 154, 153, 151, 150, 149, 148, 147, 146, 145, 143, 142, 141, 139, 138, 137, 135, 134, 132, 131, 130, 128, 127, 125, 124, 123, 121, 120, 118, 117, 115, 114, 113, 111, 110, 109, 107, 106, 105, 104, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 92, 91, 90, 90, 89, 88, 88, 88, 87, 87, 87, 86, 86, 86, 86, 86, 85, 85, 85, 85, 85, 85, 85, 86, 86, 86, 86, 86, 86, 87, 87, 87, 87, 88, 88, 88, 89, 89, 89, 90, 90, 91, 91, 92, 92, 92, 93, 93, 94, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 100, 101, 101, 101, 102, 102, 103, 103, 104
};

const unsigned char burger_pathXboss[512] = {
7, 6, 6, 5, 5, 4, 4, 4, 4, 4, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 11, 12, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 50, 51, 53, 55, 57, 60, 62, 65, 67, 70, 73, 76, 80, 83, 86, 90, 94, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177, 181, 185, 188, 192, 196, 199, 202, 206, 209, 212, 215, 217, 220, 222, 225, 227, 229, 231, 232, 234, 235, 236, 237, 238, 238, 239, 240, 240, 240, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 240, 240, 239, 239, 239, 238, 237, 237, 236, 236, 235, 234, 233, 233, 232, 231, 230, 229, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 214, 213, 212, 211, 210, 209, 208, 208, 207, 206, 205, 205, 204, 203, 202, 201, 200, 199, 197, 196, 194, 192, 190, 188, 185, 183, 181, 178, 175, 173, 170, 167, 164, 161, 158, 155, 152, 149, 145, 142, 139, 136, 132, 129, 126, 122, 119, 116, 113, 110, 106, 103, 100, 97, 94, 92, 89, 86, 83, 81, 78, 76, 74, 72, 70, 68, 66, 65, 63, 62, 61, 60, 59, 58, 57, 57, 57, 57, 57, 58, 58, 59, 60, 61, 62, 64, 65, 67, 69, 71, 73, 75, 77, 80, 82, 85, 87, 90, 93, 96, 99, 101, 105, 108, 111, 114, 117, 120, 123, 126, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 165, 168, 171, 173, 175, 178, 180, 182, 184, 186, 187, 189, 190, 191, 192, 193, 194, 195, 195, 195, 195, 195, 195, 195, 194, 193, 193, 192, 191, 190, 189, 187, 186, 184, 183, 181, 179, 178, 176, 174, 172, 170, 167, 165, 163, 161, 159, 156, 154, 152, 149, 147, 144, 142, 140, 137, 135, 133, 130, 128, 126, 124, 121, 119, 117, 115, 113, 111, 110, 108, 106, 105, 103, 102, 100, 99, 98, 97, 96, 95, 95, 94, 94, 94, 94, 94, 94, 94, 95, 95, 96, 97, 97, 98, 99, 101, 102, 103, 104, 106, 107, 109, 110, 112, 114, 115, 117, 119, 121, 123, 124, 126, 128, 130, 132, 134, 136, 138, 139, 141, 143, 145, 146, 148, 150, 151, 153, 154, 155, 157, 158, 159, 160, 161, 162, 163, 163, 164, 164, 165, 165, 165, 165, 165, 164, 164, 163, 162, 161, 160, 159, 157, 156, 154, 152, 151, 148, 146, 144, 142, 139, 137, 134, 131, 129, 126, 123, 120, 117, 114, 111, 108, 104, 101, 98, 95, 91, 88, 85, 81, 78, 75, 72, 68, 65, 62, 59, 56, 53, 50, 47, 44, 41, 38, 35, 33, 30, 28, 26, 23, 21, 19, 17, 16, 14, 12, 11, 10, 9, 8
};

const unsigned char burger_pathYboss[512] = {
47, 47, 47, 46, 46, 46, 46, 45, 45, 45, 45, 44, 44, 44, 43, 43, 43, 42, 42, 42, 41, 41, 41, 40, 40, 40, 39, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 35, 35, 35, 34, 34, 34, 34, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 33, 34, 34, 34, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 45, 45, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 45, 45, 45, 45, 44, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 39, 39, 38, 38, 37, 36, 36, 35, 35, 34, 33, 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 26, 25, 25, 25, 25, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 26, 26, 27, 27, 28, 29, 30, 31, 33, 34, 36, 38, 39, 41, 43, 46, 48, 50, 53, 55, 58, 60, 63, 66, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 98, 101, 105, 108, 111, 114, 117, 120, 123, 125, 128, 131, 134, 137, 139, 142, 144, 147, 149, 151, 154, 156, 158, 159, 161, 163, 164, 166, 167, 168, 169, 170, 171, 171, 172, 173, 174, 174, 175, 175, 176, 176, 177, 177, 178, 178, 178, 179, 179, 179, 180, 180, 180, 180, 180, 180, 181, 181, 181, 181, 181, 181, 181, 181, 180, 180, 180, 180, 180, 180, 179, 179, 179, 179, 178, 178, 178, 177, 177, 176, 176, 175, 175, 174, 174, 173, 173, 172, 172, 171, 170, 170, 169, 168, 168, 167, 166, 165, 165, 164, 163, 162, 161, 159, 158, 157, 156, 155, 153, 152, 150, 149, 148, 146, 145, 143, 142, 140, 138, 137, 135, 134, 132, 130, 129, 127, 125, 124, 122, 120, 118, 117, 115, 114, 112, 110, 109, 107, 105, 104, 102, 101, 99, 98, 97, 95, 94, 92, 91, 90, 89, 88, 86, 85, 84, 83, 82, 82, 81, 80, 79, 79, 78, 78, 77, 77, 76, 76, 76, 76, 76, 76, 75, 75, 75, 76, 76, 76, 76, 76, 76, 77, 77, 77, 78, 78, 78, 79, 79, 80, 80, 80, 81, 81, 82, 82, 83, 83, 84, 84, 84, 85, 85, 86, 86, 86, 87, 87, 87, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 88, 88, 88, 88, 87, 87, 87, 86, 86, 85, 85, 84, 84, 83, 82, 82, 81, 80, 80, 79, 78, 77, 77, 76, 75, 74, 73, 73, 72, 71, 70, 69, 68, 68, 67, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 58, 57, 56, 55, 55, 54, 53, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 48, 47, 47
};

unsigned int nesdevPalId = 0;
unsigned int nesdevFaze = 0;
void fx_NesDev(void)
{
	//nesdev
	pal_bg(palNesdev[14]);
	pal_spr(palNesdev[14]);
	cnrom_set_bank(1);
	bank_spr(1);
	vram_adr(NAMETABLE_A);
	vram_unrle(logo_scr);
	ppu_on_all();
	while(nesdevFaze < 4)
	{
		ppu_wait_nmi();
		
		//wait
		if ((nesdevFaze == 0 && nesclock == 96)) {
			nesdevFaze = 1;
		}
		//fade in nesdev
		if ((nesdevFaze == 1)) {
			if (nesdevPalId < 5 && ((nesclock & 3) == 0)) {
				++nesdevPalId;
				pal_bg(palNesdev[nesdevPalId]);
				pal_spr(palNesdev[nesdevPalId]);
			}
			if (nesclock == 160) {
				nesdevFaze = 2;
			}
		}
		//fade in credits
		if ((nesdevFaze == 2)) {
			if (nesdevPalId < 9 && ((nesclock & 3) == 0)) {
				++nesdevPalId;
				pal_bg(palNesdev[nesdevPalId]);
				pal_spr(palNesdev[nesdevPalId]);
			}
			if (nesclock == 159) {
				nesdevFaze = 3;
			}
		}
		//fadeout
		if (nesdevFaze == 3) {
			if (nesdevPalId < 14 && ((nesclock & 3) == 0)) {
				++nesdevPalId;
				pal_bg(palNesdev[nesdevPalId]);
				pal_spr(palNesdev[nesdevPalId]);
			}
			if (nesclock == 64) {
				nesdevFaze = 4;
			}
			
		}
		
		
		++nesclock;

	}
	ppu_off();
}

void main(void)
{
	set_vram_buffer();
	clear_vram_buffer();

	pal_bg(palette);
	pal_spr(palette_spr);
	
	tileset = 0;
	
	cnrom_set_bank(tileset);
	bank_spr(1);

	vram_adr(NAMETABLE_A);
	vram_unrle(NAM_multi_logo_A);
	vram_adr(NAMETABLE_B);
	vram_unrle(NAM_multi_logo_B);
	
	ppu_on_all();

	for(i=0;i<COVIDS_MAX;++i) {
		covids_pointers[i] = i*24;
	}

#define STARSHIP_AUTOPILOT 0b10000000

	starship_x = 100;
	starship_y = 200-8;
	starship_state = 1 | STARSHIP_AUTOPILOT;

	music_play(1);

	while(1)
	{
		muspos = get_mus_pos();
		clear_vram_buffer();

		pad = pad_poll(0);

		if (pad&255)
			starship_state &= (255 ^ STARSHIP_AUTOPILOT);

		if (starship_state&STARSHIP_AUTOPILOT) {
			if (rand8()>127) {
				--starship_x;
				--starship_x;
			} else {
				++starship_x;
				++starship_x;
			}
		} else {
			if (pad&PAD_LEFT) {
				--starship_x;
				--starship_x;
			}
			if (pad&PAD_RIGHT) {
				++starship_x;
				++starship_x;
			}
			if (pad&(PAD_A|PAD_B) && !bullet_y) {
				bullet_x = starship_x-4;
				bullet_y = starship_y-16;
			}
		}


		spr = 4;

		scrollpos = (sine_Table_Shake[logoPos]&0xfffe);

		scroll(scrollpos, 0);

		for(i=0;i<COVIDS_MAX;++i)
		{
			hotdog_pointer = covids_pointers[i];
			spr=oam_meta_spr(burger_pathX1[hotdog_pointer], burger_pathY1[hotdog_pointer], spr, spr_covid_19);
			covids_pointers[i] = (hotdog_pointer + 1) & 511;
		}

		spr=oam_meta_spr(starship_x, starship_y, spr, spr_starship);

		if (bullet_y) {
			spr=oam_spr(bullet_x, bullet_y, 0x84, 3, spr);
			bullet_y -= 4;
			if (bullet_y<4)
				bullet_y = 0;
		}

		oam_spr(20*8, 201, 0x01, 1 | OAM_BEHIND, 0);

//		if (muspos==192)
//			pal_col(0,21);

		if (nesclock&1) {
			++logoPos;
			if (logoPos>127)
				logoPos=0;
		}

		if (!(nesclock&3)) {
			if (--palRollId == 0) {
				palRollId = 9;
			}
			roll_scroll_colors(palRollList[palRollId], palRollList[palRollId+1], palRollList[palRollId+2]);
		}

		fx_EQ();
		fx_SplitScroll();
		++nesclock;

		ppu_wait_nmi();
		oam_clear();
	}
}