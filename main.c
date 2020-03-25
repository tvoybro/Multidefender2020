// Multimatograf 2020 invitro

#include "Include/neslib.h"
#include "Include/nesdoug.h"
#include "Include/font4x4.h"

extern char scrollText[];

extern unsigned int FT_MUSPOS;
#pragma zpsym ("FT_MUSPOS")

unsigned char pad_t;
unsigned char tileset;
unsigned int muspos;

const unsigned char palette[16]={ 0x0f,0x13,0x22,0x31,0x0f,0x00,0x10,0x30,0x0f,0x06,0x16,0x26,0x0f,0x09,0x19,0x29 };

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

void fx_SplitScroll(void)
{
	if (!(scrollerPos&15)) {
		scrollSym = scrollerData[scrollerCharIndex];
		if (!scrollSym) {
			scrollSym = ' ';
			scrollerCharIndex=0;
		} else
			++scrollerCharIndex;
		if (scrollerCharPos & 0b00010000)
			scrollerAddr = NAMETABLE_A + 32*26;
		else
			scrollerAddr = NAMETABLE_B + 32*26;

		scrollerAddr += (scrollerCharPos & 15) << 1;

		_fs = 0;
		while(tbl_ascii[_fs] != scrollSym) 
			++_fs;

		clear_vram_buffer();
		multi_vram_buffer_horz(tbl_alphabet[_fs], 2, scrollerAddr);
		multi_vram_buffer_horz(tbl_alphabet[_fs]+2, 2, scrollerAddr+32);
		scrollerCharPos = (scrollerCharPos + 1) & 31;
	}
	scroll(scrollerPos, 0);
	scrollerPos = (scrollerPos + 1) & 511;
}

void main(void)
{
	set_vram_buffer();
	clear_vram_buffer();

	pal_bg(palette);
	
	tileset = 0;
	
	cnrom_set_bank(tileset);
	
	ppu_on_all();//enable rendering

	music_play(0);

	while(1)
	{
		// muspos = get_mus_pos();

		fx_SplitScroll();

		ppu_wait_nmi();
	}
}