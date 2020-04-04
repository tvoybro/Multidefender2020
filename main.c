// Multimatograf 2020 invitro

#include "Include/neslib.h"
#include "Include/nesdoug.h"
#include "Include/font4x4.h"

#include "Gfx/NAM_multi_logo_A.h"
#include "Gfx/NAM_multi_logo_B.h"

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
unsigned int nt_Offset;
unsigned char i, eq_Tile;
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

void fx_EQ(void)
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
			scrollerAddr = NAMETABLE_A + 32*26;
		else
			scrollerAddr = NAMETABLE_B + 32*26;

		scrollerAddr += (scrollerCharPos & 15) << 1;

		_fs = 0;
		while(tbl_ascii[_fs] != scrollSym) 
			++_fs;

		multi_vram_buffer_horz(tbl_alphabet[_fs], 2, scrollerAddr);
		multi_vram_buffer_horz(tbl_alphabet[_fs]+2, 2, scrollerAddr+32);
		scrollerCharPos = (scrollerCharPos + 1) & 31;
	}

	if (FT_BUF[0x0a]==6 && !eq_Noise_Val)
		eq_Noise_Val = 7;

	split(scrollerPos - eq_Noise_Val, 0);
	if (eq_Noise_Val && scrollerPos&1)
		--eq_Noise_Val;
	scrollerPos = (scrollerPos + 1) & 511;
}

const unsigned int sine_Table_Shake[] = {
65532, 65534, 1, 3, 6, 8, 11, 13, 15, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 37, 39, 40, 41, 43, 44, 44, 45, 46, 47, 47, 47, 47, 48, 47, 47, 47, 47, 46, 45, 44, 44, 43, 41, 40, 39, 37, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 15, 13, 11, 8, 6, 3, 1, 65534, 65532, 65529, 65526, 65524, 65521, 65519, 65516, 65514, 65512, 65509, 65507, 65505, 65503, 65501, 65499, 65497, 65495, 65493, 65491, 65490, 65488, 65487, 65486, 65484, 65483, 65483, 65482, 65481, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65480, 65481, 65482, 65483, 65483, 65484, 65486, 65487, 65488, 65490, 65491, 65493, 65495, 65497, 65499, 65501, 65503, 65505, 65507, 65509, 65512, 65514, 65516, 65519, 65521, 65524, 65526, 65529
};

void main(void)
{
	set_vram_buffer();
	clear_vram_buffer();

	pal_bg(palette);
	
	tileset = 0;
	
	cnrom_set_bank(tileset);
	bank_spr(1);

	vram_adr(NAMETABLE_A);
	vram_unrle(NAM_multi_logo_A);
	vram_adr(NAMETABLE_B);
	vram_unrle(NAM_multi_logo_B);
	
	ppu_on_all();//enable rendering

	music_play(1);

	while(1)
	{
		muspos = get_mus_pos();
		clear_vram_buffer();

		scrollpos = (sine_Table_Shake[logoPos]&0xfffe);

		scroll(scrollpos, 0);

		oam_spr(15*8, 200, 0x01, 1 | OAM_BEHIND, 0);

//		if (muspos==192)
//			pal_col(0,21);

		if (nesclock&1) {
			++logoPos;
			if (logoPos>127)
				logoPos=0;
		}

		fx_EQ();
		fx_SplitScroll();
		++nesclock;

		ppu_wait_nmi();
	}
}