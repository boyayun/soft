#include "common.h"
#include "lcddriver.h"
#include "lcd.h"
#include "deveice.h"

/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
	    A
     _  ----------
COL |_| |\   |J  /|
       F| H  |  K |B
     _  |  \ | /  |
COL |_| --G-- --M--
        |   /| \  |
       E|  Q |  N |C
     _  | /  |P  \|   
DP  |_| -----------  
	    D         

 An LCD character coding is based on the following matrix:
      { E , D , P , N   }
      { M , C , COL , DP}
      { B , A , K , J   }
      { G , F , Q , H   }

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26]=
{
    /* A      B      C      D      E      F      G      H      I  */
    0xFE00,0x6714,0x1d00,0x4714,0x9d00,0x9c00,0x3f00,0xfa00,0x0014,
    /* J      K      L      M      N      O      P      Q      R  */
    0x5300,0x9841,0x1900,0x5a48,0x5a09,0x5f00,0xFC00,0x5F01,0xFC01,
    /* S      T      U      V      W      X      Y      Z  */
    0xAF00,0x0414,0x5b00,0x18c0,0x5a81,0x00c9,0x0058,0x05c0
};

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10]=
{
    /* 0      1      2      3      4      5      6      7      8      9  */
    0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
};

LcdParaStuType lcdpara;

static void LCD_Conv_Char_Seg(uint8_t c, u8 point, u8 column, uint8_t* digit)
{
  uint16_t ch = 0 ;
  uint8_t i,j;
  
  switch (c)
    {
    case ' ' : 
      ch = 0x00;
      break;
    
    case '*':
      ch = star;
      break;
                  
	case '?':
      ch = C_UMAP;
      break;
    
    case 'm' :
      ch = C_mMap;
      break;
                  
    case 'n' :
      ch = C_nMap;
      break;					
                  
    case '-' :
      ch = C_minus;
      break;
      
    case '/' :
      ch = C_slatch;
      break;  
      
    case '°' :
      ch = C_percent_1;
      break;  
    case '%' :
      ch = C_percent_2; 
      break;
    case 255 :
      ch = C_full;
      break ;
    
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':			
      ch = NumberMap[c-0x30];		
      break;
          
    default:
      /* The character c is one letter in upper case*/
      if ( (c < 0x5b) && (c > 0x40) )
      {
        ch = CapLetterMap[c-'A'];
      }
      /* The character c is one letter in lower case*/
      if ( (c <0x7b) && ( c> 0x60) )
      {
        ch = CapLetterMap[c-'a'];
      }
      break;
  }
       
  /* Set the digital point can be displayed if the point is on */
  if (point)
  {
    ch |= 0x0002;
  }

  /* Set the "COL" segment in the character that can be displayed if the column is on */
  if (column)
  {
    ch |= 0x0020;
  }		

  for (i = 12,j=0 ;j<4; i-=4,j++)
  {
    digit[j] = (ch >> i) & 0x0f; //To isolate the less signifiant dibit
  }
}


void LCD_GLASS_WriteChar(u8 ch, u8 point, u8 column, u8 position)
{
  uint8_t digit[4];     /* Digit frame buffer */
   
/* To convert displayed character in segment in array digit */
  LCD_Conv_Char_Seg(ch,point,column,digit);
  
  switch (position)
  {
    /* Position 1 on LCD (Digit1)*/
    case 1:
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xcffffffc, (((digit[0]&0x0c)<<26)|(digit[0]&0x03)));	// 1G 1B 1M 1E
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xcffffffc, (((digit[1]&0x0c)<<26)|(digit[1]&0x03)));	// 1F 1A 1C 1D 
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xcffffffc, (((digit[2]&0x0c)<<26)|(digit[2]&0x03)));	// 1Q 1K 1Col 1P 
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xcffffffc, (((digit[3]&0x0c)<<26)|(digit[3]&0x03)));	// 1H 1J 1DP 1N
      break;
    
    /* Position 2 on LCD (Digit2)*/
    case 2:
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xf3ffff03, (((digit[0]&0x0c) << 24)|((digit[0]&0x02) << 6)|((digit[0]&0x01) << 2)));	// 2G 2B 2M 2E
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xf3ffff03, (((digit[1]&0x0c) << 24)|((digit[1]&0x02) << 6)|((digit[1]&0x01) << 2 )));	// 2F 2A 2C 2D 
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xf3ffff03, (((digit[2]&0x0c) << 24)|((digit[2]&0x02) << 6)|((digit[2]&0x01) << 2 )));	// 2Q 2K 2Col 2P 
	  HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xf3ffff03, (((digit[3]&0x0c) << 24)|((digit[3]&0x02) << 6)|((digit[3]&0x01) << 2 )));	// 2H 2J 2DP 2N
      break;
    
    /* Position 3 on LCD (Digit3)*/
    case 3:
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xfcfffcff, (((digit[0]&0x0c) << 22) | ((digit[0]&0x03) << 8))); // 3G 3B 3M 3E
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xfcfffcff, (((digit[1]&0x0c) << 22) | ((digit[1]&0x03) << 8))); // 3F 3A 3C 3D 
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xfcfffcff, (((digit[2]&0x0c) << 22) | ((digit[2]&0x03) << 8))); // 3Q 3K 3Col 3P 
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xfcfffcff, (((digit[3]&0x0c) << 22) | ((digit[3]&0x03) << 8))); // 3H 3J 3DP 3N    
        break;
    
    /* Position 4 on LCD (Digit4)*/
    case 4:
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xffcff3ff, (((digit[0]&0x0c) << 18) | ((digit[0]&0x03) << 10))); // 4G 4B 4M 4E
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xffcff3ff, (((digit[1]&0x0c) << 18) | ((digit[1]&0x03) << 10))); // 4F 4A 4C 4D 
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xffcff3ff, (((digit[2]&0x0c) << 18) | ((digit[2]&0x03) << 10))); // 4Q 4K 4Col 4P
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xffcff3ff, (((digit[3]&0x0c) << 18) | ((digit[3]&0x03) << 10))); // 4H 4J 4DP 4N    
		break;
    
    /* Position 5 on LCD (Digit5)*/
    case 5:
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xfff3cfff, (((digit[0]&0x0c) << 16) | ((digit[0]&0x03) << 12))); // 5G 5B 5M 5E
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xfff3cfff, (((digit[1]&0x0c) << 16) | ((digit[1]&0x03) << 12))); // 5F 5A 5C 5D 
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xfff3cfff, (((digit[2]&0x0c) << 16) | ((digit[2]&0x01) << 12))); // 5Q 5K   5P
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xfff3cfff, (((digit[3]&0x0c) << 16) | ((digit[3]&0x01) << 12))); // 5H 5J   5N    
        break;
    
    /* Position 6 on LCD (Digit6)*/
    case 6:
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER0, 0xfffc3fff, (((digit[0]&0x04) << 15 ) | ((digit[0]&0x08) << 13 ) | ((digit[0]&0x03) << 14))) ; // 6B 6G 6M 6EE
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER2, 0xfffc3fff, (((digit[1]&0x04) << 15 ) | ((digit[1]&0x08) << 13 ) | ((digit[1]&0x03) << 14))) ; // 6A 6F 6C 6D 
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER4, 0xfffc3fff, (((digit[2]&0x04) << 15 ) | ((digit[2]&0x08) << 13 ) | ((digit[2]&0x01) << 14))) ; // 6K 6Q    6P
		HAL_LCD_Write(&hlcd, LCD_RAM_REGISTER6, 0xfffc3fff, (((digit[3]&0x04) << 15 ) | ((digit[3]&0x08) << 13 ) | ((digit[3]&0x01) << 14))) ; // 6J 6H   6N    
        break;
	  
     default:
      break;
  }
  HAL_LCD_UpdateDisplayRequest(&hlcd);
}

void LcdDisplay()
{
	u8 i;

  /* Send the string character by character on lCD */
  for(i=0; i<6; i++)
  {
  	if(0 == lcdpara.disflag[i])
  	{
    	LCD_GLASS_WriteChar(lcdpara.disdata[i], FALSE, FALSE, (i+1));
  	}
	else
	{
		LCD_GLASS_WriteChar(lcdpara.disdata[i], FALSE, TRUE, (i+1));
	}
  }
}

void UIDisplayHandle(u8 disnum)
{
	memset((u8*)&lcdpara.disdata, 0, 6);
	memset((u8*)&lcdpara.disflag, 0, 4);
	
	u8 temp = 0;
	switch(disnum)
	{
#ifdef KONGTIAO	
        case 0:		//ÎÂ¶È
            sprintf(&lcdpara.disdata[0], "TEMP");
            sprintf(&lcdpara.disdata[4], "%02d", devstate.temparature[1]);
            lcdpara.disdata[temp++] = 't';
            lcdpara.disflag[3] = 1;
            break;
			
		case 1:		//Ä£Ê½
			switch(devstate.mode)
			{
				case 1:
					sprintf(lcdpara.disdata, "MODHOT");
					break;

				case 2:
					sprintf(lcdpara.disdata, "MODWET");
					break;

				case 3:
					sprintf(lcdpara.disdata, "MODCOD");
					break;
					
				case 4:
					sprintf(lcdpara.disdata, "MODWIN");
					break;
					
				case 5:
					sprintf(lcdpara.disdata, "MODAUT");
					break;

				default:
					break;
			}
			
			lcdpara.disflag[2] = 1;
			break;
#else
		case 1: 	//Ä£Ê½
			switch(devstate.mode)
			{
				case 1:
					sprintf(lcdpara.disdata, "MODKZE");
					break;

				case 2:
					sprintf(lcdpara.disdata, "MODKZU");
					break;

				case 3:
					sprintf(lcdpara.disdata, "MODDT");
					break;
					
				case 4:
					sprintf(lcdpara.disdata, "MODGSD");
					break;
					
				case 5:
					sprintf(lcdpara.disdata, "MODWGZ");
					break;
					
				case 6:
					sprintf(lcdpara.disdata, "MODZZ");
					break;
				
				case 7:
					sprintf(lcdpara.disdata, "MODSN");
					break;
					
				case 8:
					sprintf(lcdpara.disdata, "MODBW");
					break;
					
				case 9:
					sprintf(lcdpara.disdata, "MODQS");
					break;

				default:
					break;
			}
			
			lcdpara.disflag[2] = 1;
			break;
#endif 
		case 2:		//µçÔ´
			switch(devstate.power)
			{
				case 0:
					sprintf(lcdpara.disdata, "POWOFF");
					break;

				case 1:
					sprintf(lcdpara.disdata, "POWON");
					break;
					
				default:
					break;
			}
			lcdpara.disflag[2] = 1;
			break;
#ifdef	KONGTIAO				
		case 3:		//·çÁ¿
			switch(devstate.wind)
			{
				case 1:
					sprintf(lcdpara.disdata, "WINAUT");
					break;

				case 2:
					sprintf(lcdpara.disdata, "WINLOW");
					break;
					
				case 3:
					sprintf(lcdpara.disdata, "WINMID");
					break;
					
				case 4:
					sprintf(lcdpara.disdata, "WINFAS");
					break;
					
				default:
					break;
			}
			lcdpara.disflag[2] = 1;
			break;
#endif			
//		case 4:		//ÈÕÆÚ Äê
//            {
//                u16 tem = 0;
//                tem = (devstate.date[0]<<8 | devstate.date[1])-2000;
//                sprintf(lcdpara.disdata, "DATE%02d", tem);
//                lcdpara.disflag[3] = 1;
//            }
//			break;
//			
//		case 5:		//ÔÂÈÕ
//			sprintf(lcdpara.disdata, "-%02d-%02d", devstate.date[2], devstate.date[3]);
//			break;
//			
//		case 6:		//Ê±¼ä Ê±
//			sprintf(lcdpara.disdata, "TIME");
//			lcdpara.disflag[3] = 1;
//			break;
//			
//		case 7:		//·ÖÃë
//			sprintf(lcdpara.disdata, "%02d%02d%02d", devstate.time[0], devstate.time[1], devstate.time[2]);
//			lcdpara.disflag[1] = 1;
//			lcdpara.disflag[3] = 1;
//			break;
		default:
			break;
	}
	LcdDisplay();
}

void LcdDisplayHandle()
{
	if(0 != lcdpara.state)
	{
		lcdpara.state = 0;
		UIDisplayHandle(lcdpara.disnum);
//		lcdpara.disnum++;
		switch(lcdpara.disnum)
		{
			case 0:		//ÎÂ¶È
//				lcdpara.disnum = 3;
				break;
				
			case 1:		//Ä£Ê½
//				lcdpara.disnum = 2;
				break;
				
			case 2:		//µçÔ´
//				lcdpara.disnum = 1;
				break;
			
			case 3:		//·çÁ¿
//				lcdpara.disnum = 1;
				break;
			
//			case 4:		//ÈÕÆÚ Äê
//				lcdpara.disnum = 5;
//				break;
//				
//			case 5:		//ÔÂÈÕ
//				lcdpara.disnum = 4;
//				break;
//				
//			case 6:		//Ê±¼ä Ê±
//				lcdpara.disnum = 7;
//				break;
//			
//			case 7:		//·ÖÃë
//				lcdpara.disnum = 6;
//				break;
			
			default:
				break;
		}
//#ifdef KONGTIAO
//		lcdpara.disnum &= 0x03;
//#else
//		lcdpara.disnum &= 0x01;
//#endif
	}
}
