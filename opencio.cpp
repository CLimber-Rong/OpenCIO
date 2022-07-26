/*
	Name: Rong
	Copyright: Rong
	Author: Rong
	Date: 24/07/22 12:51
	Description: Write OpenCIO here
*/

/*
 * The Address Of Repos&Doc
 * github.com/Climber-Rong/opencio
*/


/*Check the type of system*/
#include<iostream>
#ifdef __WIN32__
//Win32 platform 
#include <windows.h>
#include<conio.h> 
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#else
//linux/unix platform
#include<curses.h>
#include<termios.h>
#endif
#include <iostream>

using namespace std;

#define SGR_BackgroundMode 10
#define SGR_ForeColorMode 0
typedef int SGR_Code;
enum SelectGraphicRenditionMode{
	SGR_Strong=1,
	SGR_Italic=3,
	SGR_UnderLine=4,
	SGR_Flashing=5,
	SGR_Inverse=7,
	SGR_DeleteLine=9,
	SGR_DefaultFont=10,
	SGR_CloseItalic=23,
	SGR_CloseUnderLine,
	SGR_CloseFlashing,
	SGR_CloseInverse,
	SGR_CloseDeleteLine=29,
	SGR_NormalFont=22,
	SGR_Black=30,
	SGR_Red,
	SGR_Green,
	SGR_Yellow,
	SGR_Blue,
	SGR_Magenta,
	SGR_Cyan,
	SGR_White,
	SGR_Gray,
	SGR_DefalutForeColor=39,
	SGR_DefaultBackgroundColoe=49,
	SGR_UpLine=53,
	SGR_CloseUpLine=55,
	SGR_LightRed=90,
	SGR_LightGreen,
	SGR_LightYellow,
	SGR_LightBlue,
	SGR_LightMagenta,
	SGR_LightCyan,
	SGR_LightWhite
};

enum CLEAN_SCREEEN_MODE{
	CLSMode_FromCursorToEnd = 0,
	CLSMode_FromCursorToTop,
	CLSMode_AllScreen
};

typedef int CleanScreenMode;

enum CLEAN_LINE_MODE_TYPE{
	CLLMode_FromCursorToEnd = 0,
	CLLMode_FromCursorToTop,
	CLLMode_AllLine 
};

typedef int CleanLineMode;

typedef struct TERMINAL_CURSOR_POSITION_TPYE{
	int row;
	int column;
}TerminalCursorPosition;

class OpenCIO{
	protected:
		/*#ifndef __WIN32__
		static struct termios initial_settings, new_settings;
		static int peek_character = -1; */
	public:
		void CursorUp(unsigned row){	printf("%c[%dA",0x1B,row);	}
		void CursorDown(unsigned row){	printf("%c[%dB",0x1B,row);	}
		void CursorForward(unsigned column)	{	printf("%c[%dC",0x1B,column);}
		void CursorBack(unsigned column){	printf("%c[%dD",0x1B,column);	}
		void CursorNextLine(unsigned row){	printf("%c[%dE",0x1B,row);	}
		void CursorPreciousLine(unsigned row){	printf("%c[%dF",0x1B,row);	}
		void CursorHorizontalAbsolute(int column){	printf("%c[%dG",0x1B,column);	}
		void CursorPosition(int row,int column){	printf("%c[%d;%dH",0x1B,row,column);	}
		void CleanScreen(CleanScreenMode mode){	printf("%c[%dJ",0x1B,mode);	}
		void CleanLine(CleanLineMode mode){	printf("%c[%dK",0x1B,mode);	}
		/*TerminalCursorPosition DeviceStatusReport()
		{
			printf("%c[6n",0x1B);
			freopen("./.tmp","w",stdout);
			
			FILE* file;
			file=fopen("./.tmp","r");
			char* s="";
			char* buffer;
			while(fread(buffer,1,32,file)==32){
				strcat(s,buffer);
			}
			TerminalCursorPosition pos;
			scanf("^[[%d;%dR",&pos.row,&pos.column);
			//defer
			remove("./.tmp");
			#ifdef __WIN32__
			freopen("CON","a",stdout);
			#else
			freopen("dev/tty","a",stdout);
			#endif
			return pos;
		}*/
		void SaveCursorPosition(){	printf("%c[S",0x1B);	}
		void RestoreCursorPosition(){	printf("%c[T",0x1B);	}
		void SetBackgroundColor(SGR_Code* code){	*code+=SGR_BackgroundMode;	}
		void SelectGraphicRendition(SGR_Code code){	printf("%c[%dm",0x1B,code);	}
		#ifdef __WIN32__
		bool OpenANSIControl(){
			//Get handle of Command
			HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
			if(hstd==INVALID_HANDLE_VALUE){
				//Cannot get handle
				return false;
			}
			//Get the mode of Command
			DWORD dwMode = 0;
			if(!GetConsoleMode(hstd,&dwMode)){
				//Cannot get mode
				return false;
			}
			//Open Virtual Terminal
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if(!SetConsoleMode(hstd,dwMode)){
				return false;
			}
			//Done
			return true;
		}
		#endif
		//1.2.2.0
		bool isKeyEvent(void)
		{
			#ifdef __WIN32__
			return _kbhit();
			#else
			if(*stdin.buffer=='\0')	return false;
			else					return true;
			#endif
		}
		char GetKeyEvent(void)
		{
			return getch();
		}
};
