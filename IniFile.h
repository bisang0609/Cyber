//[*]----------------------------------------------------------------------------------------------------------------[*]
#ifndef     __INIFILE_H_INCLUDED__
#define     __INIFILE_H_INCLUDED__
//[*]----------------------------------------------------------------------------------------------------------------[*]
#ifndef DEFAULT_ARRAY_SIZE
	#define	DEFAULT_ARRAY_SIZE	    10
#endif
//[*]----------------------------------------------------------------------------------------------------------------[*]
#define     TYPE_NONE      	     	0x00
#define     TYPE_INT       	     	0x01
#define     TYPE_FLOAT          	0x02
#define     TYPE_DOUBLE         	0x03
#define     TYPE_STRING         	0x04
#define     TYPE_CHECK          	0x05
#define     TYPE_COMBO          	0x06
#define     TYPE_PASS           	0x07
#define     TYPE_MACADDRESS        	0x08
#define     TYPE_IPADDRESS        	0x09
#define     TYPE_READONLY         	0x80
//[*]----------------------------------------------------------------------------------------------------------------[*]
class CIniValue
{
	public	:
		char 		    *ValueName;
        char             Type       ,    Type2;
		char    		*Value      ,   *Value2;
		char    		*EditFmt    ,   *EditFmt2;
		void 			*ValueResult,   *ValueResult2;
		double			 Min        ,    Min2;
		double			 Max        ,    Max2;
		
	public	:
	   ~CIniValue                   ();
		CIniValue                   ();
		CIniValue                   (char *Name, char *value);
        CIniValue 	*   Set         (char *Name, char *value);
        CIniValue 	*   Set2        (char *Name, char *value);

        void			SaveFile	(FILE *fp);
		int 			GetFmtValue(char *Result, size_t nSize);
		int 			GetFmtValue2(char *Result, size_t nSize);
    
}   __attribute__ ((packed));
//[*]----------------------------------------------------------------------------------------------------------------[*]
class CIniSection
{
	public	:
    	int		    	ArraySize;
		char 	        *SectionName;
	    CIniValue	    **Child;

    public	:
	   ~CIniSection                 ();
		CIniSection                 ();
		CIniSection                 (char *Name);
        CIniSection *   Set         (char *Name);

    public	:
        int             FindIndex   (char *Name);
	    CIniValue   *   Find        (char *Name);

	    CIniValue   *   Add			(char *Name, char   *Value);
	    CIniValue   *   Add			(char *Name, int     Value);
	    CIniValue   *   Add			(char *Name, float   Value);
	    CIniValue   *   Add			(char *Name, double  Value);
	    CIniValue   *   Add			(char *Name, bool    Value);

	    CIniValue   *   Set			(char *Name, char   *Value);
	    CIniValue   *   Set			(char *Name, char   *Value, int Type);
	    CIniValue   *   Set			(char *Name, int     Value);
	    CIniValue   *   Set			(char *Name, float   Value);
	    CIniValue   *   Set			(char *Name, double  Value);
	    CIniValue   *   Set			(char *Name, bool    Value);

	    CIniValue   *   Set2		(char *Name, char   *Value);
	    CIniValue   *   Set2		(char *Name, int     Value);
	    CIniValue   *   Set2		(char *Name, float   Value);
	    CIniValue   *   Set2		(char *Name, double  Value);
	    CIniValue   *   Set2		(char *Name, bool    Value);

	    CIniValue   *   Get			(char *Name, char   *Value, char  *Default);
	    CIniValue   *   Get			(char *Name, int    *Value, int    Default);
	    CIniValue   *   Get			(char *Name, float  *Value, float  Default);
	    CIniValue   *   Get			(char *Name, double *Value, double Default);
	    CIniValue   *   Get			(char *Name, bool   *Value, bool   Default);

	    CIniValue   *   Get2		(char *Name, char   *Value, char  *Default);
	    CIniValue   *   Get2		(char *Name, int    *Value, int    Default);
	    CIniValue   *   Get2		(char *Name, float  *Value, float  Default);
	    CIniValue   *   Get2		(char *Name, double *Value, double Default);
	    CIniValue   *   Get2		(char *Name, bool   *Value, bool   Default);

	    CIniValue   *   Get			(char *Name, char   *Value, char  *Default, char *AFmt);
	    CIniValue   *   Get			(char *Name, int    *Value, int    Default, char *AFmt, int    AMin, int    AMax);
	    CIniValue   *   Get			(char *Name, float  *Value, float  Default, char *AFmt, float  AMin, float  AMax);
	    CIniValue   *   Get			(char *Name, double *Value, double Default, char *AFmt, double AMin, double AMax);
		CIniValue   *   Get			(char *Name, bool   *Value, bool   Default, char *AFmt);
		CIniValue   *   Get			(char *Name, int    *Value, int    Default, char *AFmt);

	    CIniValue   *   Get2		(char *Name, char   *Value, char  *Default, char *AFmt);
	    CIniValue   *   Get2		(char *Name, int    *Value, int    Default, char *AFmt, int    AMin, int    AMax);
	    CIniValue   *   Get2		(char *Name, float  *Value, float  Default, char *AFmt, float  AMin, float  AMax);
	    CIniValue   *   Get2		(char *Name, double *Value, double Default, char *AFmt, double AMin, double AMax);
		CIniValue   *   Get2		(char *Name, bool   *Value, bool   Default, char *AFmt);
		CIniValue   *   Get2		(char *Name, int    *Value, int    Default, char *AFmt);

        void            Del         (char *Name);
        void			SaveFile	(FILE *fp);
		void            View        ();
}   __attribute__ ((packed));
//[*]----------------------------------------------------------------------------------------------------------------[*]
class CIniFile
{
	public:
    	int				ArraySize;
		CIniSection		**Section;

	public:
	   ~CIniFile();
		CIniFile();
        int             FindIndex   (char *Name);
        CIniSection*    Find        (char *Name);
        CIniSection*    Add         (char *Name);
        CIniSection*    Set         (char *Name);
        void            Del         (char *Name);
		void            View        ();
        void            Clear       ();

        void            SaveFile    (const char   *FileName);
        int             LoadFile    (const char   *FileName);
}   __attribute__ ((packed));
//[*]----------------------------------------------------------------------------------------------------------------[*]
#endif
//[*]----------------------------------------------------------------------------------------------------------------[*]
