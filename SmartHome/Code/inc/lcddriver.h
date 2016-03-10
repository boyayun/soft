#ifndef __LCDDRIVER_H
#define __LCDDRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32l1xx_hal.h"
#include "common.h"

/* Macros used for set/reset bar LCD bar */
#define BAR0_ON  t_bar[1] |= 8
#define BAR0_OFF t_bar[1] &= ~8
#define BAR1_ON  t_bar[0] |= 8
#define BAR1_OFF t_bar[0] &= ~8
#define BAR2_ON  t_bar[1] |= 2
#define BAR2_OFF t_bar[1] &= ~2
#define BAR3_ON t_bar[0]  |= 2 
#define BAR3_OFF t_bar[0] &= ~2 

/* code for 'µ' character */
#define C_UMAP 0x6084

/* code for 'm' character */
#define C_mMap 0xb210

/* code for 'n' character */
#define C_nMap 0x2210

/* constant code for '*' character */
#define star 0xA0DD

/* constant code for '-' character */
#define C_minus 0xA000

/* constant code for '/' */
#define C_slatch  0x00c0

/* constant code for ∞ */
#define C_percent_1 0xec00

/* constant code  for small o */
#define C_percent_2 0xb300

#define C_full 0xffdd

typedef struct
{
	char disdata[7];		//œ‘ æ ˝æ›
	u8 disflag[4];		//œ‘ æ±Íº«
	u8 disnum;			//œ‘ æΩÁ√Ê ˝(0--7)
	u8 state;			// 0:≤ªœ‘ æ 1:∏¸–¬œ‘ æ
}LcdParaStuType;

extern LcdParaStuType lcdpara;
void LcdDisplayHandle();

void LCD_bar(void);
void LCD_GLASS_Init(void);
void LCD_GLASS_WriteChar(uint8_t ch, u8 point, u8 column, u8 position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void LCD_GLASS_WriteTime(char a, uint8_t posi, u8 column);
void LCD_GLASS_Configure_GPIO(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCDDRIVER_H */
