




#include "stdgfx.h"
#include "g3dasc.h"

#include "stdlib.h"


G3DASCTOOL::G3DASCTOOL () : G3DFILETOOL ()
  {
  } 

G3DASCTOOL::~G3DASCTOOL ()
  {
  } 

INT G3DASCTOOL::ReadASCToken ( FILEHANDLE f, STRING String  )
  {
    return fscanf ( f, "%s", String );  
  } 

INT G3DASCTOOL::GoToASCToken ( FILEHANDLE f, STRING String )
  {
    BOOLEAN Found=FALSE;
    CHAR Str[128];

    while (!Found)
      {
        if (ReadASCToken ( f, Str  )==EOF)
          return EOF;
        if (strcmp(String,Str)==0)
          Found = TRUE;
      } 
    return 0;  
  } 

INT G3DASCTOOL::GetASCNumber ( FILEHANDLE f, double *Number )
  {
    CHAR Str[128];
    INT Result;

    Result = fscanf ( f, "%s", Str );
    *Number = atof ( Str );
    return Result;
  } 

INT G3DASCTOOL::GetASCInt ( FILEHANDLE f, LONG *Number )
  {
    return fscanf ( f, "%d", Number );  
  } 

INT G3DASCTOOL::GetASCName ( FILEHANDLE f, STRING Name )
  {
    CHAR Str[128];
    INT Result;
    
    Result = fscanf ( f, "%s", Str );

    if (Result==EOF)
      return Result;
      
    INT i=0;
    INT Count=0;
    while (Count<32)
      {
        if (Str[i]=='"')
          {
          } 
        else if (Str[i]==NULL)
          {
            Name[Count] = NULL;
            break;
          } 
        else
          Name[Count++] = Str[i];
        i++;
      } 
    Name[Count] = NULL;
    return Result;
  } 

BOOLEAN G3DASCTOOL::LoadASCPolyObject ( FILEHANDLE f,
                                        G3DPOLYHEDRON *Object, 
                                        double Scale )
  {
    double x,y,z;
    LONG NumVertex,NumFaces;
    FLPVECTOR3D *PointList;
    G3DPOLY *PolyList;
    FLPVECTOR2D *TexList;
    LONG P1,P2,P3;
    double u,v;
    GEOMETRYDATA *GeometryData;
        
    if (GoToASCToken( f,"Vertices:" )==EOF)
      return FAILURE;
    else
      {
        GetASCInt ( f, &NumVertex );
        GoToASCToken( f,"Faces:" );
        GetASCInt ( f, &NumFaces );

        TexList = new FLPVECTOR2D [NumVertex];
        if (TexList==NULL)
          return FAILURE;

        Object->CreateGeometryData ( NumVertex, NumFaces );    
        GeometryData = Object->GetGeometryData ();    
        PointList = GeometryData->FLPLocalPoints;
        PolyList = GeometryData->Polys;
            
        GoToASCToken ( f,"Vertex" );
        GoToASCToken ( f, "list:" );
        LONG i;
        CHAR NumStr[128];        
                  
        strcpy (NumStr,"");
        
        for (i=0;i<(LONG)NumVertex;i++)
          {
            while (strcmp(NumStr,"Vertex"))
              ReadASCToken( f,NumStr );

            sprintf ( NumStr, "%d:", i );

            GoToASCToken ( f, NumStr );

            ReadASCToken ( f, NumStr );
            if (strlen(NumStr)>2)
              x = atof ( NumStr+2 );
            else
              GetASCNumber ( f, &x );

            ReadASCToken ( f, NumStr );
            if (strlen(NumStr)>2)
              y = atof ( NumStr+2 );
            else
              GetASCNumber ( f, &y );

            ReadASCToken ( f, NumStr );
            if (strlen(NumStr)>2)
              z = atof ( NumStr+2 );
            else
              GetASCNumber ( f, &z );

            PointList[i].x = (float)(x*Scale);
            PointList[i].y = (float)(z*Scale);
            PointList[i].z = (float)(y*Scale);
            
            u = v = 0;
            ReadASCToken ( f, NumStr );
            if (strncmp(NumStr,"U:",2)==0) 
              {
                Object->TextureMapped = TRUE;
                if (strlen(NumStr)>2)
                  u = atof ( NumStr+2 );
                else
                  GetASCNumber ( f, &u );

                ReadASCToken ( f, NumStr );
                if (strlen(NumStr)>2)
                  v = atof ( NumStr+2 );
                else
                  GetASCNumber ( f, &v );
              } 
                          
            if (u<0)
              u = -u;
            if (v<0)
              v = -v;

            double IntPtr;
            if (u>1.0)
              u = modf ( u, &IntPtr );
            if (v>1.0)
              v = modf ( v, &IntPtr );
            
            TexList[i].x = (float)(u*255*65536);  
            TexList[i].y = (float)(v*255*65536);  
          } 
              
        if (strcmp(NumStr,"Face"))
          GoToASCToken( f,"Face" );
        if (GoToASCToken ( f, "list:" )==EOF)
          return FAILURE;
            
        for (i=0;i<(LONG)NumFaces;i++)
          {
            GoToASCToken( f,"Face" );
            CHAR Str[128];
            sprintf ( Str, "%d:", i );
            GoToASCToken ( f, Str );
            
            ReadASCToken ( f, Str );
            P1 = atoi ( Str+2 );

            ReadASCToken ( f, Str );
            P2 = atoi ( Str+2 );

            ReadASCToken ( f, Str );
            P3 = atoi ( Str+2 );

            PolyList[i].Points[0] = (LONG)P1;
            PolyList[i].Points[1] = (LONG)P2;
            PolyList[i].Points[2] = (LONG)P3;
            
            PolyList[i].TexCoords[0].x = TexList[P1].x;
            PolyList[i].TexCoords[0].y = TexList[P1].y;
            PolyList[i].TexCoords[1].x = TexList[P2].x;
            PolyList[i].TexCoords[1].y = TexList[P2].y;
            PolyList[i].TexCoords[2].x = TexList[P3].x;
            PolyList[i].TexCoords[2].y = TexList[P3].y;
          } 
        delete TexList;  
      } 
    
    return SUCCESS;  
  } 

BOOLEAN G3DASCTOOL::Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    G3DOBJECT *Object;

    BOOLEAN Done=FALSE;

    while (Done!=TRUE)
      {
        CHAR Name[32];
    
        if (GoToASCToken( f,"Named" )==EOF)
          {
            break;
          } 

        if (GoToASCToken( f,"object:" )==EOF)
          {
            break;
          } 
            
        if (GetASCName( f,Name )==EOF)
          {
            return FAILURE;
          } 
      
        if (GoToASCToken( f,"Tri-mesh," )==EOF)
          {
            break;
          } 
      
        Object = new G3DPOLYHEDRON ();
          
        Object->SetName ( Name );
        
        if (LoadASCPolyObject (f, (G3DPOLYHEDRON*)Object, Scale )==FAILURE)
          {
            delete Object;  
            return FAILURE;
          } 

        ((G3DPOLYHEDRON*)Object)->ClockWised = ClockWise;
        ObjectList->AddObject ( Object );  
      } 
        
    return SUCCESS;
  } 

BOOLEAN G3DASCTOOL::Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    if (f)
      {}
    if (ObjectList)
      {}
    if (ClockWise)
      {}
    if (Scale)
      {}
    return FAILURE;          
  } 



