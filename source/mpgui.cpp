




#include "stdgfx.h"
#include "mpgui.h"

void ProcessArgs ( int argc, char **argv )
  {
    INT i;
    INT Pos=0;
    NumArgs = argc-1;
         for (i=1;i<argc;i++)
      {
        INT j=0;
        while (argv[i][j]!=NULL)
          {
            CmdLine[Pos] = argv[i][j];
            Pos++;
            j++;
          } 
        CmdLine[Pos] = ' ';
        Pos++;
      } 
  } 

void InitApp ()
  {
    InitWinHashTable ();
    #if defined (__FOROS2__)
      hAB = WinInitialize ( 0 );
      hMQ = WinCreateMsgQueue ( hAB, 0 );
    #elif defined (__FORUNIX__)
    #endif
  } 

VOID PrepareArg ( STRING args )
  {
    INT i,Pos;

    i = 0;
    Pos = 0;
    NumArgs = 0;
    while (args[i]!=NULL)
      {
        while ((args[i]==' '))
          i++;
        if (args[i]==NULL)
          break;
          
        while ((args[i]!=' ')&&(args[i]!=NULL))
          {
            CmdLine[Pos++] = args[i++];  
          } 
        NumArgs++;  
        CmdLine[Pos++] = ' ';  
      } 
    if (Pos>0)  
      CmdLine[Pos-1] = NULL;
    else
      CmdLine[Pos] = NULL;  
  } 

LONG GetNumArgs ()
  {
    return NumArgs;    
  } 

VOID GetArg ( LONG Index, STRING Str )
  {
    if (Str==NULL)
      return;
      
    if ((Index>=NumArgs)||(Index<0))
      {
        Str[0] = NULL;  
      } 

    INT i,Count;

    i = 0;
    Count = 0;
    while (Count!=Index)
      {
        if (CmdLine[i]==' ')
          Count++;
        i++;  
      } 

    INT j;
    j = 0;
    while ((CmdLine[i]!=' ')&&(CmdLine[i]!=NULL))
      Str[j++] = CmdLine[i++];
    Str[j] = NULL;  
  } 

  








