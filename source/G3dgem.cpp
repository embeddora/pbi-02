




#include "stdgfx.h"
#include "g3dgem.h"

struct GEMFACETYPE
  {
    INT Num;
    INT PointList[16];
  }; 

G3DGEMTOOL::G3DGEMTOOL () : G3DFILETOOL ()
  {
  } 
  
G3DGEMTOOL::~G3DGEMTOOL ()
  {
  } 
  
BOOLEAN G3DGEMTOOL::LoadPoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
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
                                 
BOOLEAN G3DGEMTOOL::LoadFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                                LONG NumFaces )
  {
    LONG i;
    G3DPOLY *PolyList;
    GEOMETRYDATA *GeometryData;
    LONG NewFaceNum;

    GEMFACETYPE *Faces;
    Faces = new GEMFACETYPE [NumFaces];
    if (Faces==NULL)
      return FAILURE;
      
    NewFaceNum = 0;
    INT j;
    for (i=0;i<NumFaces;i++)
      {
        fscanf ( f, "%d ", &(Faces[i].Num) );

        for (j=0;j<Faces[i].Num-1;j++)
          {
            fscanf ( f, "%d ", &(Faces[i].PointList[j]) );
          } 
        fscanf ( f, "%d\n", &(Faces[i].PointList[j]) );
        NewFaceNum += 1 + Faces[i].Num - 3;
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
            PolyList->Points[0] = Faces[i].PointList[0] - 1;
            PolyList->Points[1] = Faces[i].PointList[j-1] - 1;
            PolyList->Points[2] = Faces[i].PointList[j] - 1;
            PolyList->TexCoords[0].x = (float)0; PolyList->TexCoords[0].y = (float)0;
            PolyList->TexCoords[1].x = (float)0; PolyList->TexCoords[1].y = (float)0;
            PolyList->TexCoords[2].x = (float)0; PolyList->TexCoords[2].y = (float)0;
            PolyList++;
          } 
      } 

    delete Faces;
    Faces = NULL;
    return SUCCESS;      
  } 
                                
BOOLEAN G3DGEMTOOL::Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    INT NumPoints,NumFaces,Total;
    
    fscanf ( f, "%d %d %d\n", &NumPoints, &NumFaces, &Total );
    
    G3DPOLYHEDRON *Object = new G3DPOLYHEDRON ();
    if (Object==NULL)
      return FAILURE;

    Object->TextureMapped = FALSE;  
    if (LoadPoints ( f, Object, NumPoints, Scale )==FAILURE)
      {
        delete Object;
        return FAILURE;
      } 
      
    if (LoadFaces ( f, Object, NumFaces )==FAILURE)
      {
        delete Object;
        return FAILURE;
      } 
    
    Object->ClockWised = ClockWise;
    ObjectList->AddObject ( Object );  
    return SUCCESS;
  } 
  
BOOLEAN G3DGEMTOOL::Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
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

