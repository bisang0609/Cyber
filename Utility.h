//[*]----------------------------------------------------------------------------------------------------------------[*]
#ifndef __UTILITY_H_INCLUDED__
#define __UTILITY_H_INCLUDED__
//[*]----------------------------------------------------------------------------------------------------------------[*]
#include        "Array.h"
//[*]----------------------------------------------------------------------------------------------------------------[*]
#define 	    M_PI			3.14159265358979323846
#define 	    ABS(x)			(((x)>=0)?(x):(-(x)))
//[*]----------------------------------------------------------------------------------------------------------------[*]
#define		    BUFFER_EMPTY  	(-1)
//[*]----------------------------------------------------------------------------------------------------------------[*]
#define         NOP_DELAY_US(n)         NopDelay(33   *n)
#define         NOP_DELAY_MS(n)         NopDelay(33600*n)
//[*]----------------------------------------------------------------------------------------------------------------[*]
enum {
	BACKSPACE = 0x08,   LF      = 0x0A,   CR    = 0x0D,   CNTLQ = 0x11, 
    CNTLS     = 0x13, 	CNTLZ   = 0x1A,   ESC   = 0x1B,   DEL   = 0x7F
};
//[*]----------------------------------------------------------------------------------------------------------------[*]
extern const unsigned short int	MASK16[];
extern const unsigned long 		MASK  [];
//[*]----------------------------------------------------------------------------------------------------------------[*]
unsigned short  crc16         (char *data_p, unsigned short length);
unsigned int    crc32_bitwise (const void* data, unsigned int length, unsigned int previousCrc32 = 0);		// compute CRC32 (bitwise algorithm)
unsigned int    crc32_halfbyte(const void* data, unsigned int length, unsigned int previousCrc32 = 0);		// compute CRC32 (half-byte algoritm)
unsigned int    crc32_1byte   (const void* data, unsigned int length, unsigned int previousCrc32 = 0);		// compute CRC32 (standard algorithm)

void 			CPUReset   		();
void	 		Buzzer			(int hz, int loop);

void 			MakeUDPCheckSum	(unsigned char *ptr, int Size);
unsigned char 	CheckUDPCheckSum(unsigned char *ptr, int Size);

char *          GetDateVersion  (bool longFormat = true);
char *          GetTime         (bool longFormat = true);

void            GetFileList     (CStringList &List, const char *FMask, bool IncludeDir);
char *          ExtractFileExt  (const char *FullFilename);
char *          ExtractFileName (const char *FullFilename);
char *          ExtractFilePath (const char *FullFilename);
char *          ExtractFileDrv  (const char *FullFilename);
int             GetFileSize     (const char *Filename);
bool            FileExist       (const char *Filename);

char *			UpperStr		(char *str);											       	//  주의 : 원 소스 문자열에 변형이 가해진다.
char *			LowerStr		(char *str);       												//  주의 : 원 소스 문자열에 변형이 가해진다.
char *			trimleft		(char *str, char ch = ' ');       								//  주의 : 원 소스 문자열에 변형이 가해진다.		//2014.04.14.sage5nor.change
char *			trimright		(char *str, char ch = ' ');     								//  주의 : 원 소스 문자열에 변형이 가해진다.		//2014.04.14.sage5nor.change
extern char *   trim            (char *str, char ch = ' ');       								//  주의 : 원 소스 문자열에 변형이 가해진다.		//2014.04.14.sage5nor.change
int				parse_args		(char *cmdline, const char *delim, char **argv, bool trim);     //  주의 : 원 소스 문자열에 변형이 가해진다.
extern int		ParseStr		(char *cmdline, const char *delim, char **argv, bool trim);		//  원 소스 문자열 변형 없음.
char *			substr			(char *Buf, char *str, int stpos, int len = -1);
char *			substr			(           char *str, int stpos, int len = -1);
int             strpos          (const char *Buf, char FindChar);
int             strpos          (const char *Buf, const char *SubStr);
char *			get_entry		(char *cp, char **pNext);										//	Process input string for long or short name entry
bool 			isnumstr		(char *str);
bool 			isboolstr		(char *str);
bool 			IsValidFileName (char *AStr);
char*           CvtValidFileName(char *AStr);
int 			getline			(char *LBuf, unsigned int BufSize, int Key = -1);

unsigned short 	ByteOrder		(unsigned short Data);
void 			NopDelay		(unsigned int   Delay);
void 			LoopDelay		(unsigned int   Delay, void (*LoopFunc)(void));

unsigned int    StrToInt        (char *str);
unsigned int 	StrToInt		(char *str, int NumeralSystem);

double 			fLinearEq		(double x1, double y1, double x2, double y2, double x);
int             CopyFile        (char *SrcFile, char *DstFile);
void 			BitSprint		(char *Dest, unsigned int Value, int Bits);		//2014.04.14.sage5nor.add
void            BitPrint        (unsigned int Value, int Bits);
void            HEXPrintf       (char *Caption, unsigned char *Buffer, short int Size, unsigned char Column = 32);
char*           RemoveColorCode (const char *Text, int Size);
    //[*]----------------------------------------------------------------------------------------------------------------[*]
#endif	//	__UTILITY_H_INCLUDED__
//[*]----------------------------------------------------------------------------------------------------------------[*]
