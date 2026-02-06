//[*]----------------------------------------------------------------------------------------------------------------[*]
#include 	<stdio.h>
#include 	<string.h>
#include 	<stdarg.h>
#include 	<stdlib.h>

#include 	"Types.h"
#include 	"Utility.h"
#include 	"IniFile.h"
#include    "Change_Common.h"
//[*]----------------------------------------------------------------------------------------------------------------[*]
//      CIniValue Class Functions
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue::~CIniValue()
{
    if(ValueName != NULL)  	delete	ValueName;
    if(Value     != NULL)  	delete	Value;          if(Value2    != NULL)  	delete	Value2;
    if(EditFmt   != NULL)  	delete	EditFmt;        if(EditFmt2  != NULL)  	delete	EditFmt2;
	Type 		  = 0;                              Type2		  = 0;
	ValueResult	  = NULL;                     	    ValueResult2  = NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue::CIniValue()
{
    ValueName	    = NULL;
    Value		    = NULL;     Value2		 = NULL;
	ValueResult	    = NULL;     ValueResult2 = NULL;
	EditFmt		    = NULL;     EditFmt2     = NULL;
	Type 		    = 0;        Type2 		 = 0;
	Min			    = 0;        Min2		 = 0;
	Max			    = 0;        Max2		 = 0;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue::CIniValue(char *Name, char *value)
{
    ValueName	    = NULL;
    Value		    = NULL;     Value2		 = NULL;
	ValueResult	    = NULL;     ValueResult2 = NULL;
	EditFmt		    = NULL;     EditFmt2     = NULL;
	Type 		    = 0;        Type2 		 = 0;
	Min			    = 0;        Min2		 = 0;
	Max			    = 0;        Max2		 = 0;

    Set(Name, value);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniValue::Set(char *AName, char *AValue)
{
    if(ValueName != NULL && strcmp(ValueName, AName ) != 0)     {   delete	ValueName;  ValueName   = NULL;   }
    if(Value     != NULL && strcmp(Value    , AValue) != 0)     {   delete	Value;      Value       = NULL;   }
    if(ValueName == NULL)   {   ValueName = new char[strlen(AName )+1];     memset(ValueName, 0x00, strlen(AName )+1);  SAFE_STRCPY_PTR(ValueName, AName,strlen(AName )+1 );      }
    
    if(Value     == NULL)   {
        int      argc       = 0;
        char	*argv[2]    = { 0x00, 0x00 };
        
        for(int i=0; i<int(strlen(AValue)); i++)  {
            if(AValue[i] == ',')    {
                argc = i;
                break;
            }
        }
        if(argc != 0)   {   AValue[argc] = 0x00;    argv[1] = &AValue[argc+1];  argv[1] = trimright(trimleft(argv[1]));     }
                                                    argv[0] = &AValue[0];       argv[0] = trimright(trimleft(argv[0]));
        if(argv[0] != NULL) {
            Value  = new char[strlen(argv[0])+1];
            memset(Value    , 0x00, strlen(argv[0])+1);  
            SAFE_STRCPY_PTR(Value    , argv[0],strlen(argv[0])+1);     
        }
        
        if(argv[1] != NULL)     Set2(AName, argv[1]);
    }

    return  this;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniValue::Set2(char *AName, char *AValue)
{
    if(Value2    != NULL && strcmp(Value2   , AValue ) != 0)     {   delete	Value2;     Value2      = NULL;   }
    if(Value2    == NULL)   {   Value2    = new char[strlen(AValue )+1];     memset(Value2   , 0x00, strlen(AValue )+1);  SAFE_STRCPY_PTR(Value2   , AValue,strlen(AValue )+1);   }

    return  this;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
int CIniValue::GetFmtValue(char *Result, size_t nSize)
{
    char    FmtStr  [20];
    char    ValueStr[40]; 
    char    Temp[200];
    char   *argv[20] = { 0, };
    char    argc, EdtLength = strlen(EditFmt), IsHex = 0;

    if(Value != NULL)   {
        switch(Type)       {
            case TYPE_INT   :   for(int i=0;i<EdtLength; i++)   {   
                                    if(EditFmt[i] == 'H')   {   IsHex = 1;  break;  }  
                                }
                                if(IsHex)   sprintf(FmtStr  ,  "%%0%dX", EdtLength);
                                else        sprintf(FmtStr  ,  "%%%dd" , EdtLength);

                                if(IsHex)   sprintf(ValueStr, FmtStr  , ABS(atoi(Value)));   
                                else        sprintf(ValueStr, FmtStr  , ABS(atoi(Value)));
                                strncpy(Result , ValueStr, EdtLength);
                                if(EditFmt[0] == '*')   {
                                    if(atoi(Value ) < 0)    Result[0] = '-';
                                    else                    Result[0] = '+';
                                }
                                break;
            
            case TYPE_FLOAT :
            case TYPE_DOUBLE:   sprintf(FmtStr  , "%%%d.%df", EdtLength, EdtLength - (strpos(EditFmt, '.') + 1));
                                sprintf(ValueStr, FmtStr    , ABS(atof(Value)));
                                strncpy(Result  , ValueStr  , EdtLength);

                                if(EditFmt[0] == '*')   {
                                    if(atof(Value) < 0)     Result[0] = '-';
                                    else                    Result[0] = '+';
                                }
                                break;
            
            case TYPE_MACADDRESS:
            case TYPE_IPADDRESS :
            case TYPE_STRING:   for(int i=0; i<EdtLength; i++)	{
                                    if(i < strlen(Value))   Result[i] = Value[i];
                                    else	                Result[i] = '_';
                                }
                                break;
                
			case TYPE_CHECK	:   SAFE_STRCPY(Temp,EditFmt);
                                argc = parse_args(Temp, "|", argv, true);
                            
                                if(argc == 2)   {
                                    if(isnumstr(Value))	{
                                        if(atoi(Value) > 0 && atoi(Value) < argc)           SAFE_STRCPY_PTR(Result, argv[atoi(Value)], nSize);
                                        else                                                SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                    }   else    {
                                        SAFE_STRCPY(ValueStr, Value);
                                        if     (strcmp(UpperStr(ValueStr), "ON"    ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "HIGH"  ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "TRUE"  ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "ENABLE") == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else												SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                    }
                                }   else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                break;
			case TYPE_COMBO	:
                                SAFE_STRCPY(Temp		, EditFmt);
                                argc = parse_args(Temp, "|", argv, true);
                                if(isnumstr(Value))	    {
                                    if(atoi(Value) > 0 && atoi(Value) < argc)               SAFE_STRCPY_PTR(Result, argv[atoi(Value)], nSize);
                                    else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);          
                                }   else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);          
                                break;
        }
	}	else	{
        if(Type != TYPE_CHECK && Type != TYPE_COMBO)	    {
            for(int i=0; i<EdtLength; i++)	{
                if     (EditFmt[i] == '#')	Result[i] = '_';
                if     (EditFmt[i] == 'H')	Result[i] = '_';
                else if(EditFmt[i] == '*')	Result[i] = '+';
                else if(EditFmt[i] == 'A')  Result[i] = ' ';
                else						Result[i] = EditFmt[i];
                Result[i+1] = 0x00;
            }
        }	else	{
            if(Type == TYPE_CHECK || Type == TYPE_COMBO)	{
                SAFE_STRCPY(Temp		, EditFmt);
                argc = parse_args(Temp, "|", argv, true);
                SAFE_STRCPY_PTR(Result, argv[0], nSize);				
            }
        }
	}
	
    if(Type != TYPE_CHECK && Type != TYPE_COMBO)	{
        for(int i=0; i<EdtLength; i++)	{
            if(EditFmt[i] != '#' && EditFmt[i] != '*' && EditFmt[i] != 'A' && EditFmt[i] != 'H')	{
                Result[i] = EditFmt[i];
            }
        }
        Result[EdtLength] = 0x00;
    }
    
    if(Value  != NULL)                      return 1;
    
    return 0;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
int CIniValue::GetFmtValue2(char *Result, size_t nSize)
{
    char    FmtStr  [20];
    char    ValueStr[40]; 
    char    Temp[200];
    char   *argv[20] = { 0, };
    char    argc, EdtLength = strlen(EditFmt2), IsHex = 0;

    if(Value2 != NULL)   {
        switch(Type2)       {
            case TYPE_INT   :   
                                for(int i=0;i<EdtLength; i++)   {   
                                    if(EditFmt2[i] == 'H')   {   IsHex = 1;  break;  }  
                                }
                                
                                if(IsHex)   sprintf(FmtStr  ,  "%%0%dX", EdtLength);
                                else        sprintf(FmtStr  ,  "%%%dd" , EdtLength);

                                if(IsHex)   sprintf(ValueStr, FmtStr  , ABS(atoi(Value2)));   
                                else        sprintf(ValueStr, FmtStr  , ABS(atoi(Value2)));
                                
                                strncpy(Result , ValueStr, EdtLength);
                                
                                if(EditFmt2[0] == '*')   {
                                    if(atoi(Value2) < 0)    Result[0] = '-';
                                    else                    Result[0] = '+';
                                }
                                break;
            
            case TYPE_FLOAT :
            case TYPE_DOUBLE:   sprintf(FmtStr   , "%%%d.%df", EdtLength, EdtLength - (strpos(EditFmt2, '.') + 1));
                                sprintf(ValueStr, FmtStr    , ABS(atof(Value2)));
                                strncpy(Result  , ValueStr , EdtLength);

                                if(EditFmt2[0] == '*')   {
                                    if(atof(Value2) < 0)    Result[0] = '-';
                                    else                    Result[0] = '+';
                                }
                                break;
            
            case TYPE_STRING:   for(int i=0; i<EdtLength; i++)	{
                                    if(i < strlen(Value2))  Result[i] = Value2[i];
                                    else	                Result[i] = '_';
                                }
                                break;
                
			case TYPE_CHECK	:   SAFE_STRCPY(Temp		, EditFmt2);
                                argc = parse_args(Temp, "|", argv, true);
                            
                                if(argc == 2)   {
                                    if(isnumstr(Value2))	{
                                        if(atoi(Value2) > 0 && atoi(Value2) < argc)         SAFE_STRCPY_PTR(Result, argv[atoi(Value2)], nSize);
                                        else                                                SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                    }   else    {
                                        SAFE_STRCPY(ValueStr, Value2);
                                        if     (strcmp(UpperStr(ValueStr), "ON"    ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "HIGH"  ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "TRUE"  ) == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else if(strcmp(UpperStr(ValueStr), "ENABLE") == 0)	SAFE_STRCPY_PTR(Result, argv[1], nSize);
                                        else												SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                    }
                                }   else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);
                                break;
			case TYPE_COMBO	:
                                SAFE_STRCPY(Temp		, EditFmt2);
                                argc = parse_args(Temp, "|", argv, true);
                                if(isnumstr(Value2))	    {
                                    if(atoi(Value2) > 0 && atoi(Value2) < argc)             SAFE_STRCPY_PTR(Result, argv[atoi(Value2)], nSize);
                                    else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);          
                                }   else                                                    SAFE_STRCPY_PTR(Result, argv[0], nSize);          
                                break;
        }
	}	else	{
        if(Type2 != TYPE_CHECK && Type2 != TYPE_COMBO)	    {
            for(int i=0; i<EdtLength; i++)	{
                if     (EditFmt2[i] == '#')	    Result[i] = '_';
                if     (EditFmt2[i] == 'H')	    Result[i] = '_';
                else if(EditFmt2[i] == '*')	    Result[i] = '+';
                else if(EditFmt2[i] == 'A')     Result[i] = ' ';
                else						    Result[i] = EditFmt2[i];
                Result[i+1] = 0x00;
            }
        }	else	{
            if(Type2 == TYPE_CHECK || Type2 == TYPE_COMBO)	{
                SAFE_STRCPY(Temp		, EditFmt2);
                argc = parse_args(Temp, "|", argv, true);
                SAFE_STRCPY_PTR(Result, argv[0], nSize);
            }
        }
	}
	
    if(Type2 != TYPE_CHECK && Type2 != TYPE_COMBO)	{
        for(int i=0; i<EdtLength; i++)	{
            if(EditFmt2[i] != '#' && EditFmt2[i] != '*' && EditFmt2[i] != 'A' && EditFmt2[i] != 'H')	{
                Result[i] = EditFmt2[i];
            }
        }
        Result[EdtLength] = 0x00;
    }
    
    if(Value2 != NULL)                      return 1;
    
    return 0;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniValue::SaveFile(FILE *fp)
{
	if(ValueName == NULL)   return;
    if(Value     == NULL)   return;
    
    if(Value2 != NULL) 	fprintf(fp, "%-30s = %s, %s\r\n"   , ValueName, Value, Value2);
    else                fprintf(fp, "%-30s = %s\r\n"       , ValueName, Value);
    fflush(fp);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
//      CIniSection Class Functions
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection::~CIniSection()
{
    if(Child != NULL)   {
        for(int i=0; i<ArraySize; i++)	{
            if(Child[i] != NULL)    delete Child[i];
        }
        delete  []  Child;
    }
    if(SectionName != NULL) delete  SectionName;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection::CIniSection()
{
    ArraySize   = 0;
    Child       = NULL;
    SectionName = NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection::CIniSection(char *Name)
{
    ArraySize   = 0;
    Child       = NULL;
    SectionName = NULL;

    Set(Name);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection *CIniSection::Set(char *Name)
{
    if(SectionName != NULL)	delete	SectionName;
    SectionName = new char[strlen(Name)+1];
    SAFE_STRCPY_PTR(SectionName, Name, strlen(Name)+1);

    if(Child == NULL)    {
        ArraySize 	= DEFAULT_ARRAY_SIZE;
        Child       = new CIniValue *[ArraySize];
        for(int i=0; i<ArraySize; i++) 	Child[i] = NULL;
    }
    return  this;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Find(char *Name)
{
    for(int i=0; i<ArraySize; i++)	{
        if(Child[i] != NULL)	{
            if(strcmp(Name, Child[i]->ValueName) == 0)	{
                return Child[i];
            }
        }
    }

    return NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
int CIniSection::FindIndex(char *Name)
{
    for(int i=0; i<ArraySize; i++)	{
        if(Child[i] != NULL)	{
            if(strcmp(Name, Child[i]->ValueName) == 0)	return i;
        }
    }
    
    return -1;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniSection::Del(char *Name)
{
    int Index = FindIndex(Name);
    if(Index != -1)   {
        delete  Child[Index];
        Child[Index] = NULL;
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue * CIniSection::Add(char *Name, char *Value)
{
    CIniValue *p = Find(Name);
    if(p != NULL)   return  p->Set(Name, Value);

    for(int i=0; i<ArraySize; i++)	{
        if(Child[i] == NULL)	{
            Child[i] = new CIniValue(Name, Value);
            return Child[i];
        }
    }

    CIniValue **NewArray = new CIniValue *[ArraySize + DEFAULT_ARRAY_SIZE];

    for(int i=0; i<ArraySize + DEFAULT_ARRAY_SIZE; i++) {
        if(i < ArraySize)   NewArray[i] = Child[i];
        else                NewArray[i] = NULL;
    }

    delete [] Child;
    Child = NewArray;

    p = Child[ArraySize] = new CIniValue(Name, Value);
    ArraySize += DEFAULT_ARRAY_SIZE;

    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Add (char *Name, int    Value)    {	    return  Set(Name, Value);	        }
CIniValue *CIniSection::Add (char *Name, float  Value)    {	    return  Set(Name, Value); 	        }
CIniValue *CIniSection::Add (char *Name, double Value)    {	    return  Set(Name, Value); 	        }
CIniValue *CIniSection::Add (char *Name, bool   Value)    {	    return  Set(Name, Value);	        }
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Set(char *Name, char *Value)
{
    CIniValue *p = Find(Name);
    if(p != NULL)   return  p->Set(Name, Value);
    else            return     Add(Name, Value);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Set(char *Name, char   *Value, int Type)
{
    CIniValue *p = Find(Name);
    if(p != NULL)   {
        if     (Type == TYPE_MACADDRESS)    {   char Temp[50];  sprintf(Temp, "%02X:%02X:%02X:%02X:%02X:%02X", Value[0], Value[1], Value[2], Value[3], Value[4], Value[5]);     return  p->Set(Name, Temp);     }
        else if(Type == TYPE_IPADDRESS )    {   char Temp[50];  sprintf(Temp, "%03d.%03d.%03d.%03d"          , Value[0], Value[1], Value[2], Value[3]);                         return  p->Set(Name, Temp);     }
        else                                {   return  p->Set(Name, Value);    }
    }   else    {
        if     (Type == TYPE_MACADDRESS)    {   char Temp[50];  sprintf(Temp, "%02X:%02X:%02X:%02X:%02X:%02X", Value[0], Value[1], Value[2], Value[3], Value[4], Value[5]);     return  Add(Name, Temp);        }
        else if(Type == TYPE_IPADDRESS )    {   char Temp[50];  sprintf(Temp, "%03d.%03d.%03d.%03d", Value[0], Value[1], Value[2], Value[3]);                                   return  Add(Name, Temp);        }
        else                                {   return  Add(Name, Value);       }
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Set (char *Name, int    Value)    {     char    B1[100];    sprintf(B1, "%d", Value);       return  Set(Name, B1);      }
CIniValue *CIniSection::Set (char *Name, float  Value)    {     char    B1[100];    sprintf(B1, "%f", Value);       return  Set(Name, B1);      }
CIniValue *CIniSection::Set (char *Name, double Value)    {     char    B1[100];    sprintf(B1, "%f", Value);       return  Set(Name, B1);      }
CIniValue *CIniSection::Set (char *Name, bool   Value)    {	    return  Set(Name, (Value ? "TRUE" : "FALSE"));                                  }
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Set2(char *Name, char  *Value)
{
    CIniValue *p = Find(Name);
    if(p != NULL)   return  p->Set2(Name, Value);
    return     NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Set2(char *Name, int    Value)    {	    char    B1[100];    sprintf(B1, "%d", Value);       return  Set2(Name, B1);     }
CIniValue *CIniSection::Set2(char *Name, float  Value)    {     char    B1[100];    sprintf(B1, "%f", Value);       return  Set2(Name, B1);     }
CIniValue *CIniSection::Set2(char *Name, double Value)    {     char    B1[100];    sprintf(B1, "%f", Value);       return  Set2(Name, B1);     }
CIniValue *CIniSection::Set2(char *Name, bool   Value)    {	    return  Set2(Name, (Value ? "TRUE" : "FALSE"));                                 }
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, char    *Value, char  *Default)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);

    strcpy(Value, p->Value);
//  *Value = p->Value;
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, char    *Value, char  *Default)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default);
                                if(p->Value2 == NULL)           Set2(Name, Default);

    strcpy(Value, p->Value2);
    
//  *Value = p->Value;
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, char    *Value, char  *Default, char *AFmt)
{
    CIniValue *p = Find(Name);          if(p == NULL)   p = Add(Name, Default);
    
    if     (strcmp(AFmt, "###.###.###.###"  ) == 0)     p->Type = TYPE_IPADDRESS;
    else if(strcmp(AFmt, "HH:HH:HH:HH:HH:HH") == 0)     p->Type = TYPE_MACADDRESS;
    else                                                p->Type = TYPE_STRING;

    p->ValueResult 	            = (void *)Value;
	p->EditFmt 	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);

    if(p->Type == TYPE_IPADDRESS)  {
        char    Temp[50];
        char   *argv[10];
        SAFE_STRCPY(Temp, p->Value);
        int argc = parse_args(Temp, ".", argv, true);

        ((unsigned char *)p->ValueResult)[0] = atoi(argv[0]);
        ((unsigned char *)p->ValueResult)[1] = atoi(argv[1]);
        ((unsigned char *)p->ValueResult)[2] = atoi(argv[2]);
        ((unsigned char *)p->ValueResult)[3] = atoi(argv[3]);
    }
    else if(p->Type == TYPE_MACADDRESS)  {
        char    Temp[50];
        char   *argv[10];
        SAFE_STRCPY(Temp, p->Value);
        int argc = parse_args(Temp, ":", argv, true);

        ((unsigned char *)p->ValueResult)[0] = strtoul(argv[0], NULL, 16);
        ((unsigned char *)p->ValueResult)[1] = strtoul(argv[1], NULL, 16);
        ((unsigned char *)p->ValueResult)[2] = strtoul(argv[2], NULL, 16);
        ((unsigned char *)p->ValueResult)[3] = strtoul(argv[3], NULL, 16);
        ((unsigned char *)p->ValueResult)[4] = strtoul(argv[4], NULL, 16);
        ((unsigned char *)p->ValueResult)[5] = strtoul(argv[5], NULL, 16);
    }   
    else    {   
        strcpy((char *)p->ValueResult, p->Value);   
    }
    
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, char    *Value, char  *Default, char *AFmt)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default, AFmt);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    p->Type2                    = TYPE_STRING;
    p->ValueResult2	            = (void *)Value;
	if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);
	strcpy((char *)p->ValueResult2, p->Value2);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, int    *Value, int    Default)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    *Value                      = atoi(p->Value);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, int    *Value, int    Default)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    *Value                      = atoi(p->Value2);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, int    *Value, int    Default, char *AFmt, int AMin, int AMax)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    p->Type                     = TYPE_INT;
    p->ValueResult 	            = (void *)Value;
	p->Max			            = AMax;
	p->Min		        	    = AMin;
    if(p->EditFmt != NULL)      {   delete  p->EditFmt;     p->EditFmt = NULL; }            //  2014.04.24  URIDURI
	p->EditFmt 	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);
   *(int *)(p->ValueResult)     = atoi(p->Value);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, int    *Value, int    Default, char *AFmt, int AMin, int AMax)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default, AFmt, AMin, AMax);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    p->Type2                    = TYPE_INT;
    p->ValueResult2 	        = (void *)Value;
	p->Max2			            = AMax;
	p->Min2		        	    = AMin;
    if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);
   *(int *)(p->ValueResult2)    = atoi(p->Value2);

	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, int    *Value, int    Default, char *AFmt)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    p->Type                     = TYPE_COMBO;
    p->ValueResult 	            = (void *)Value;
    if(p->EditFmt != NULL)      {   delete  p->EditFmt;     p->EditFmt = NULL; }            //  2014.04.24  URIDURI
	p->EditFmt 	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);
   *(int *)(p->ValueResult)     = atoi(p->Value);
    
    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, int    *Value, int    Default, char *AFmt)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default, AFmt);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    p->Type2                    = TYPE_COMBO;
    p->ValueResult2	            = (void *)Value;
	if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);
   *(int *)(p->ValueResult2)    = atoi(p->Value2);
    
    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, float  *Value, float  Default)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    *Value                      = atof(p->Value);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, float  *Value, float  Default)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    *Value                      = atof(p->Value2);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, float  *Value, float Default, char *AFmt, float AMin, float AMax)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    p->Type                     = TYPE_FLOAT;
    p->ValueResult 	            = (void *)Value;
	p->Max			            = AMax;
	p->Min		        	    = AMin;
    if(p->EditFmt != NULL)      {   delete  p->EditFmt;     p->EditFmt = NULL; }            //  2014.04.24  URIDURI
	p->EditFmt 	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);
   *(float *)(p->ValueResult)   = atof(p->Value);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, float  *Value, float Default, char *AFmt, float AMin, float AMax)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get( Name, Value, Default, AFmt, AMin, AMax);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    p->Type2                    = TYPE_FLOAT;
    p->ValueResult2	            = (void *)Value;
	p->Max2			            = AMax;
	p->Min2		        	    = AMin;
    if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);
   *(float *)(p->ValueResult2)  = atof(p->Value2);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, double *Value, double Default)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    *Value                      = atof(p->Value);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, double *Value, double Default)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default);
                                if(p->Value2 == NULL)           Set2(Name, Default);

    *Value                      = atof(p->Value2);
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, double  *Value, double Default, char *AFmt, double AMin, double AMax)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    p->Type                     = TYPE_DOUBLE;
    p->ValueResult 	            = (void *)Value;
	p->Max			            = AMax;
	p->Min		        	    = AMin;
    if(p->EditFmt != NULL)      {   delete  p->EditFmt;     p->EditFmt = NULL; }            //  2014.04.24  URIDURI
	p->EditFmt 	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);
   *(double *)(p->ValueResult)  = atof(p->Value);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, double  *Value, double Default, char *AFmt, double AMin, double AMax)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default, AFmt, AMin, AMax);
                                if(p->Value2 == NULL)           Set2(Name, Default);

    p->Type2                    = TYPE_DOUBLE;
    p->ValueResult2	            = (void *)Value;
	p->Max2			            = AMax;
	p->Min2		        	    = AMin;
    if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	    = new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);
   *(double *)(p->ValueResult2) = atof(p->Value2);
	return	p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, bool   *Value, bool   Default)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);

	if(isnumstr(p->Value))	{	    *Value = atoi(p->Value);	}	
	else					{	    
		if	   (strcmp(p->Value, "ON"    ) == 0)	*Value = true;
		else if(strcmp(p->Value, "HIGH"  ) == 0)	*Value = true;
		else if(strcmp(p->Value, "TRUE"  ) == 0)	*Value = true;
		else if(strcmp(p->Value, "ENABLE") == 0)	*Value = true;                          //  2014.04.24  URIDURI
		else									    *Value = false;
	}
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, bool   *Value, bool   Default)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default);
                                if(p->Value2 == NULL)           Set2(Name, Default);

	if(isnumstr(p->Value2))	{	    *Value = atoi(p->Value2);	}	
	else					{	    
		if	   (strcmp(p->Value2, "ON"    ) == 0)   *Value = true;
		else if(strcmp(p->Value2, "HIGH"  ) == 0)   *Value = true;
		else if(strcmp(p->Value2, "TRUE"  ) == 0)   *Value = true;
		else if(strcmp(p->Value2, "ENABLE") == 0)	*Value = true;                          //  2014.04.24  URIDURI
		else									    *Value = false;                         //  2014.04.24  URIDURI
	}
    return  p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get(char *Name, bool   *Value, bool   Default, char *AFmt)
{
    CIniValue *p = Find(Name);  if(p == NULL)   p = Add(Name, Default);
    
    p->Type                 = TYPE_CHECK;
    p->ValueResult 	        = (void *)Value;
    if(p->EditFmt != NULL)      {   delete  p->EditFmt;     p->EditFmt = NULL; }            //  2014.04.24  URIDURI
	p->EditFmt 	        	= new char[strlen(AFmt)+1];		strcpy(p->EditFmt, AFmt);

	if(isnumstr(p->Value))	{	                    *(bool *)(p->ValueResult) = atoi(p->Value);    	}	
    else	                {
		if	   (strcmp(p->Value, "ON"    ) == 0)	*(bool *)(p->ValueResult) = true;
		else if(strcmp(p->Value, "HIGH"  ) == 0)	*(bool *)(p->ValueResult) = true;
		else if(strcmp(p->Value, "TRUE"  ) == 0)	*(bool *)(p->ValueResult) = true;
		else if(strcmp(p->Value, "ENABLE") == 0)	*(bool *)(p->ValueResult) = true;
		else							    		*(bool *)(p->ValueResult) = false;
	}
    
    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniValue *CIniSection::Get2(char *Name, bool   *Value, bool   Default, char *AFmt)
{
    CIniValue *p = Find(Name);  if(p         == NULL)   return  Get (Name, Value, Default, AFmt);
                                if(p->Value2 == NULL)           Set2(Name, Default);
    
    p->Type2                = TYPE_CHECK;
    p->ValueResult2	        = (void *)Value;
    if(p->EditFmt2 != NULL)     {   delete  p->EditFmt2;    p->EditFmt2 = NULL; }           //  2014.04.24  URIDURI
	p->EditFmt2	        	= new char[strlen(AFmt)+1];		strcpy(p->EditFmt2, AFmt);

	if(isnumstr(p->Value2))	    {           	    *(bool *)(p->ValueResult2) = atoi(p->Value2);	}	
    else	                    {
		if	   (strcmp(p->Value2, "ON"    ) == 0)	*(bool *)(p->ValueResult2) = true;
		else if(strcmp(p->Value2, "HIGH"  ) == 0)	*(bool *)(p->ValueResult2) = true;
		else if(strcmp(p->Value2, "TRUE"  ) == 0)	*(bool *)(p->ValueResult2) = true;
		else if(strcmp(p->Value2, "ENABLE") == 0)	*(bool *)(p->ValueResult2) = true;
		else							    		*(bool *)(p->ValueResult2) = false;
	}
    
    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniSection::View()
{
    printf("[%s]\r\n", SectionName);

    for(int i=0; i<ArraySize; i++)	{
        if(Child[i] != NULL)	{
            printf("%-30s = ", Child[i]->ValueName);
            if(Child[i]->Value  != NULL)    printf("%s " , Child[i]->Value );
            if(Child[i]->Value2 != NULL)    printf(", %s", Child[i]->Value2);
            printf("\r\n");
        }
    }
    printf(" \r\n");
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniSection::SaveFile(FILE *fp)
{
	if(SectionName != NULL)	{
    	fprintf(fp, "[%s]\r\n", SectionName);
        for(int i=0; i<ArraySize; i++)	{
            if(Child[i] != NULL)	Child[i]->SaveFile(fp);
        }
    	fprintf(fp, "\r\n");
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
//      CIniFile Class Functions
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniFile::~CIniFile()
{
    if(Section != NULL) {
        for(int i=0; i<ArraySize; i++)	{
            if(Section[i] != NULL) 	delete Section[i];
        }
        delete  []  Section;
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniFile::CIniFile()
{
    ArraySize = DEFAULT_ARRAY_SIZE;
    Section   = new CIniSection *[ArraySize];
    for(int i=0; i<ArraySize; i++) 	Section[i] = NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniFile::Clear()
{
    if(Section != NULL) {
        for(int i=0; i<ArraySize; i++)	{
            if(Section[i] != NULL) 	    delete Section[i];
            Section[i] = NULL;
        }
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection* CIniFile::Find(char *Name)
{
    for(int i=0; i<ArraySize; i++)	{
        if(Section[i] != NULL)	{
            if(strcmp(Name, Section[i]->SectionName) == 0)	return Section[i];
        }
    }
    return NULL;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
int CIniFile::FindIndex(char *Name)
{
    for(int i=0; i<ArraySize; i++)	{
        if(Section[i] != NULL)	{
            if(strcmp(Name, Section[i]->SectionName) == 0)	return i;
        }
    }
    return -1;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection* CIniFile::Add(char *Name)
{
    CIniSection *p = Find(Name);
    if(p != NULL)   return  p->Set(Name);

    for(int i=0; i<ArraySize; i++)	{
        if(Section[i] == NULL)	{
            Section[i] = new CIniSection(Name);
            return Section[i];
        }
    }

    CIniSection	**NewArray = new CIniSection *[ArraySize + DEFAULT_ARRAY_SIZE];

    for(int i=0; i<ArraySize + DEFAULT_ARRAY_SIZE; i++) {
        if(i < ArraySize)   NewArray[i] = Section[i];
        else                NewArray[i] = NULL;
    }

    delete [] Section;
    Section = NewArray;

    p = Section[ArraySize] = new CIniSection(Name);
    ArraySize += DEFAULT_ARRAY_SIZE;

    return p;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniFile::Del(char *Name)
{
    int Index = FindIndex(Name);
    if(Index != -1)   {
        delete  Section[Index];
        Section[Index] = NULL;
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
CIniSection* CIniFile::Set(char *Name)
{
    CIniSection *p = Find(Name);
    if(p != NULL)   return  p->Set(Name);
    else            return     Add(Name);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniFile::View()
{
    printf("\r\n");
    for(int i=0; i<ArraySize; i++)	{
        if(Section[i] != NULL)    Section[i]->View();
    }
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
void CIniFile::SaveFile(const char *FileName)
{
	FILE	*fp = fopen(FileName, "wt");
    for(int i=0; i<ArraySize; i++)	{
        if(Section[i] != NULL)    Section[i]->SaveFile(fp);
    }
    fflush(fp);    
    fclose(fp);
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
int CIniFile::LoadFile(const char *FileName)
{
    int		        argc;
    char	        *argv[2], Buffer[256];
    CIniSection     *Section = NULL;

    Clear();

    FILE	*fp = fopen(FileName,  "rt");
    if(fp == NULL)              return  0;        //  File Not Found

    while(!feof(fp))	{
        if(fgets(Buffer, 256, fp) == NULL) 			continue;
        if(Buffer[0] == '/' && Buffer[1] == '/')	continue;
        
        argc = 0;
        for(int i=0; i<int(strlen(Buffer)); i++)  {  if(Buffer[i] == '=')    {   argc = i;   break;  }   }

        if(argc != 0)   {
            Buffer[argc] = 0x00;
            argv[0] = &Buffer[     0];  argv[0] = trimright(trimleft(argv[0]));
            argv[1] = &Buffer[argc+1];  argv[1] = trimright(trimleft(argv[1]));
            if(Section != NULL) Section->Add(argv[0], argv[1]);
        }   else        {
            argv[0] = &Buffer[0];       argv[0] = trimright(trimleft(argv[0]));
            if(argv[0][0] == '[' && argv[0][strlen(argv[0])-1] == ']')  {
                Section = Add(substr(argv[0], 1, strlen(argv[0])-2));
            }
        }
    }

    fclose(fp);
    return 1;
}
//[*]----------------------------------------------------------------------------------------------------------------[*]
