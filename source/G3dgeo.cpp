




#include "stdgfx.h"
#include "g3dgeo.h"

struct GEOFACETYPE
  {
    INT Num;
    INT PointList[16];
  }; 

G3DGEOTOOL::G3DGEOTOOL () : G3DFILETOOL ()
  {
  } 
  
G3DGEOTOOL::~G3DGEOTOOL ()
  {
  } 
  
BOOLEAN G3DGEOTOOL::LoadPoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                                 LONG NumPoints, double Scale )
  {
    LONG i;
    float x,y,z;
    FLPVECTOR3D *PointList;
    GEOMETRYDATA *GeometryData;
    
    Object->CreateGeometryPoints(NumPoints);
    
    GeometryData = Object->GetGeometryData ();
    PointList = GeometryData->FLPLocalPoints;

    if (PointList==NULL)
      return FAILURE;
          
    for (i=0;i<NumPoints;i++)
      {
        fscanf ( f, "%f %f %f\n", &x, &y, &z );
        PointList[i].x = (float)(x*Scale);
        PointList[i].y = (float)(y*Scale);
        PointList[i].z = (float)(z*Scale);
      } 
      
    return SUCCESS;  
  } 
                                 
BOOLEAN G3DGEOTOOL::LoadFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object )
  {
    G3DPOLY *PolyList;
    GEOMETRYDATA *GeometryData;
    LONG NewFaceNum;

    BOOLEAN Done=FALSE;
    INT NumPoints;
    INT Dummy;
    CHAR DummyChar;
    LONG NumFaces;
    
    NewFaceNum = NumFaces = 0;
    if (NumFaces)
      {}  
    
    GEOFACETYPE *Faces;

    INT i,j;
    LONG Position;

    Position = File.GetPos ( f );

    while (!Done)
      {
        if (fscanf(f,"%d",&NumPoints)==EOF)
          Done = TRUE;
        else 
          {
            DummyChar = (CHAR)File.GetCh ( f );
            while (DummyChar==' ')
              DummyChar = (CHAR)File.GetCh ( f );
            if (DummyChar==EOF)
              Done = TRUE;

            if ((DummyChar>='0')&&(DummyChar<='9')&&(NumPoints>=3))
              {
                File.Seek ( f, -1, FROM_CURRENT );
                for (j=0;j<NumPoints;j++)
                  {
                    if (fscanf ( f, "%d ", &Dummy )==EOF)
                      return FAILURE;
                  } 
                fscanf ( f, "%d\n", &Dummy );
                NumFaces++;  
              } 
            else
              {
                DummyChar = (CHAR)File.GetCh ( f );
                while ((DummyChar!='\n')&&(DummyChar!=EOF))
                  DummyChar = (CHAR)File.GetCh ( f );
                if (DummyChar==EOF)
                  {
                    delete Faces;
                    return FAILURE;
                  } 
              } 
          } 
      } 
    
    File.Seek ( f, Position, FROM_BEGIN );

    Done = FALSE;
    Faces = new GEOFACETYPE [NumFaces];
    if (Faces==NULL)
      return FAILURE;

    i = 0;
    while ((i<NumFaces)&&(!Done))
      {
        if (fscanf(f,"%d",&NumPoints)==EOF)
          {
            delete Faces;
            return FAILURE;
          } 
        else 
          {
            DummyChar = (CHAR)File.GetCh ( f );
            while (DummyChar==' ')
              DummyChar = (CHAR)File.GetCh ( f );
            if (DummyChar==EOF)
              {
                delete Faces;
                return FAILURE;
              } 

            if ((DummyChar>='0')&&(DummyChar<='9')&&(NumPoints>=3))
              {
                File.Seek ( f, -1, FROM_CURRENT );
                Faces[i].Num = NumPoints;
                for (j=0;j<Faces[i].Num;j++)
                  {
                    fscanf ( f, "%d ", &(Faces[i].PointList[j]) );
                  } 
                fscanf ( f, "%d\n", &Dummy );
                NewFaceNum+=1+Faces[i].Num-3;
                i++;
              } 
            else
              {
                DummyChar = (CHAR)File.GetCh ( f );
                while ((DummyChar!='\n')&&(DummyChar!=EOF))
                  DummyChar = (CHAR)File.GetCh ( f );
                if (DummyChar==EOF)
                  {
                    delete Faces;
                    return FAILURE;
                  } 
              } 
          } 
      } 
    
    Object->CreateGeometryPolys ( NewFaceNum );
      
    GeometryData = Object->GetGeometryData ();
    PolyList = GeometryData->Polys;
      
    if (PolyList==NULL)  
      {
        delete Faces;
        return FAILURE;
      } 
      
    for (i=0;i<NumFaces;i++)
      {
        for (j=2;j<Faces[i].Num;j++)
          {
            PolyList->Points[0] = Faces[i].PointList[0];
            PolyList->Points[1] = Faces[i].PointList[j];
            PolyList->Points[2] = Faces[i].PointList[j-1];  
            PolyList->TexCoords[0].x = (float)0; PolyList->TexCoords[0].y = (float)0;
            PolyList->TexCoords[1].x = (float)0; PolyList->TexCoords[1].y = (float)0;
            PolyList->TexCoords[2].x = (float)0; PolyList->TexCoords[2].y = (float)0;
            PolyList++;
          } 
      } 

    delete Faces;

    return SUCCESS;      
  } 
                                
BOOLEAN G3DGEOTOOL::Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    INT NumPoints;
    CHAR IDStr[4];
    
    fscanf ( f, "%s\n", IDStr );
    
    if (strncmp(IDStr,"3DG1",4)!=0)
      return FAILURE;
      
    fscanf ( f, "%d\n", &NumPoints );
    
    G3DPOLYHEDRON *Object = new G3DPOLYHEDRON ();
    if (Object==NULL)
      return FAILURE;
      
    Object->TextureMapped = FALSE;
    
    if (LoadPoints ( f, Object, NumPoints, Scale )==FAILURE)
      {
        delete Object;
        return FAILURE;
      } 
      
    if (LoadFaces ( f, Object )==FAILURE)
      {
        delete Object;
        return FAILURE;
      } 
    
    Object->ClockWised = ClockWise;
    ObjectList->AddObject ( Object );  
    return SUCCESS;
  } 
  
BOOLEAN G3DGEOTOOL::SavePoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                                 LONG NumPoints, double Scale )
  {
    LONG i;
    float x,y,z;
    FLPVECTOR3D *PointList;
    GEOMETRYDATA *GeometryData;
    
    GeometryData = Object->GetGeometryData ();
    PointList = GeometryData->FLPLocalPoints;

    if (PointList==NULL)
      return FAILURE;
          
    for (i=0;i<NumPoints;i++)
      {
        x = (float)(PointList[i].x*Scale);
        y = (float)(PointList[i].y*Scale);
        z = (float)(PointList[i].z*Scale);
        fprintf ( f, "%f %f %f\n", x, y, z );
      } 
      
    return SUCCESS;  
  } 
                                 
BOOLEAN G3DGEOTOOL::SaveFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object )
  {
    LONG i;
    INT P1,P2,P3;
    G3DPOLY *PolyList;
    GEOMETRYDATA *GeometryData;
    LONG NumPolys;
    
    GeometryData = Object->GetGeometryData ();
    PolyList = GeometryData->Polys;
    NumPolys = GeometryData->NumPolys;

    if (PolyList==NULL)
      return FAILURE;
          
    for (i=0;i<NumPolys;i++)
      {
        P1 = PolyList[i].Points[0];  
        P2 = PolyList[i].Points[1];  
        P3 = PolyList[i].Points[2];  
        fprintf ( f, "3 %d %d %d 15\n", P1, P2, P3 );
      } 
      
    return SUCCESS;  
  } 
                                 
BOOLEAN G3DGEOTOOL::Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    INT NumPoints;
    CHAR IDStr[5];

    LISTOBJECT<G3DOBJECT*> *ObjectNode = ObjectList->GetHead();
    if (ObjectNode==NULL)
      return FAILURE;
      
    G3DPOLYHEDRON *Object = (G3DPOLYHEDRON*)(ObjectNode->Data);
    if (Object==NULL)
      return FAILURE;

    GEOMETRYDATA *GeometryData = Object->GetGeometryData ();
    NumPoints = GeometryData->NumPoints;
    
    strcpy ( IDStr, "3DG1" );
    fprintf ( f, "%s\n", IDStr );          
    fprintf ( f, "%d\n", NumPoints );
    
    if (SavePoints ( f, Object, NumPoints, Scale )==FAILURE)
      {
        return FAILURE;
      } 
      
    if (SaveFaces ( f, Object )==FAILURE)
      {
        return FAILURE;
      } 

    if (ClockWise)
      {}  
    return SUCCESS;
  } 

