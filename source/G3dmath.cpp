




#include "stdgfx.h"
#include "g3dmath.h"
#include "lg3dmat.h"
#include <math.h>







VOID InitMatrix ( G3DMATRIX Matrix )
  {
    INT i,j;
    for (i=0;i<4;i++)
      {
        for (j=0;j<4;j++)
          {
            if (i==j)
              Matrix[i][j] = (float)1;
            else    
              Matrix[i][j] = (float)0;
          } 
      } 
  } 

VOID CopyMatrix ( G3DMATRIX Src, G3DMATRIX Dest )
  {
    memcpy ( Dest, Src, 4*4*4 );
  } 

VOID MultMatrix ( G3DMATRIX Mat1, G3DMATRIX Mat2, G3DMATRIX Dest )
  {
    _FLPMultMatrix ( Mat1, Mat2, Dest ); 
  } 

VOID Translate ( G3DMATRIX Matrix, float xt, float yt, float zt )
  {
    G3DMATRIX TMat;
    G3DMATRIX Mat1;
    float a;

    a = (float)1;

    TMat[0][0]=  (float)a; TMat[0][1]=  (float)0; TMat[0][2]=  (float)0; TMat[0][3]=  (float)0;
    TMat[1][0]=  (float)0; TMat[1][1]=  (float)a; TMat[1][2]=  (float)0; TMat[1][3]=  (float)0;
    TMat[2][0]=  (float)0; TMat[2][1]=  (float)0; TMat[2][2]=  (float)a; TMat[2][3]=  (float)0;
    TMat[3][0]=  (float)xt; TMat[3][1]=  (float)yt; TMat[3][2]=  (float)zt; TMat[3][3]=  (float)a;

    MultMatrix ( Matrix, TMat, Mat1 );
    CopyMatrix ( Mat1, Matrix ); 
  } 

VOID Scale ( G3DMATRIX Matrix, float ScaleFactor )
  {
    G3DMATRIX SMat;
    G3DMATRIX Mat1;
    float a;
    float s = ScaleFactor;

    a = (float)1;

    SMat[0][0]=  (float)s; SMat[0][1]=  (float)0; SMat[0][2]=  (float)0; SMat[0][3]=  (float)0;
    SMat[1][0]=  (float)0; SMat[1][1]=  (float)s; SMat[1][2]=  (float)0; SMat[1][3]=  (float)0;
    SMat[2][0]=  (float)0; SMat[2][1]=  (float)0; SMat[2][2]=  (float)s; SMat[2][3]=  (float)0;
    SMat[3][0]=  (float)0; SMat[3][1]=  (float)0; SMat[3][2]=  (float)0; SMat[3][3]=  (float)a;

    MultMatrix ( Matrix, SMat, Mat1 );
    CopyMatrix ( Mat1, Matrix ); 
  } 

VOID RotateXYZ ( G3DMATRIX Matrix, float xa, float ya, float za )
  {
    G3DMATRIX XMat,YMat,ZMat,Mat1,Mat2;
    double Xa,Ya,Za;
    float Sx,Cx,Sy,Cy,Sz,Cz;

    
    Xa = -xa*6.281/360;
    Ya = -ya*6.281/360;
    Za = -za*6.281/360;
    
    Sx = (float)sin ( Xa );
    Cx = (float)cos ( Xa );
    Sy = (float)sin ( Ya );
    Cy = (float)cos ( Ya );
    Sz = (float)sin ( Za );
    Cz = (float)cos ( Za );

    float a;

    a = (float)1;
    XMat[0][0]=  (float)a; XMat[0][1]=  (float)0; XMat[0][2]=  (float)0; XMat[0][3]=  (float)0;
    XMat[1][0]=  (float)0; XMat[1][1]= (float)Cx; XMat[1][2]= (float)Sx; XMat[1][3]=  (float)0;
    XMat[2][0]=  (float)0; XMat[2][1]=(float)-Sx; XMat[2][2]= (float)Cx; XMat[2][3]=  (float)0;
    XMat[3][0]=  (float)0; XMat[3][1]=  (float)0; XMat[3][2]=  (float)0; XMat[3][3]=  (float)a;
    
    YMat[0][0]= (float)Cy; YMat[0][1]=  (float)0; YMat[0][2]=(float)-Sy; YMat[0][3]=  (float)0;
    YMat[1][0]=  (float)0; YMat[1][1]=  (float)a; YMat[1][2]=  (float)0; YMat[1][3]=  (float)0;
    YMat[2][0]= (float)Sy; YMat[2][1]=  (float)0; YMat[2][2]= (float)Cy; YMat[2][3]=  (float)0;
    YMat[3][0]=  (float)0; YMat[3][1]=  (float)0; YMat[3][2]=  (float)0; YMat[3][3]=  (float)a;
    
    ZMat[0][0]= (float)Cz; ZMat[0][1]= (float)Sz; ZMat[0][2]=  (float)0; ZMat[0][3]=  (float)0;
    ZMat[1][0]=(float)-Sz; ZMat[1][1]= (float)Cz; ZMat[1][2]=  (float)0; ZMat[1][3]=  (float)0;
    ZMat[2][0]=  (float)0; ZMat[2][1]=  (float)0; ZMat[2][2]=  (float)a; ZMat[2][3]=  (float)0;
    ZMat[3][0]=  (float)0; ZMat[3][1]=  (float)0; ZMat[3][2]=  (float)0; ZMat[3][3]=  (float)a;

    MultMatrix ( Matrix, XMat, Mat1 );
    MultMatrix ( Mat1, YMat, Mat2 );
    MultMatrix ( Mat2, ZMat, Matrix );
  } 

VOID RotateYXZ ( G3DMATRIX Matrix, float xa, float ya, float za )
  {
    G3DMATRIX XMat,YMat,ZMat,Mat1,Mat2;
    double Xa,Ya,Za;
    float Sx,Cx,Sy,Cy,Sz,Cz;

    
    Xa = -xa*6.281/360;
    Ya = -ya*6.281/360;
    Za = -za*6.281/360;
    
    Sx = (float)sin ( Xa );
    Cx = (float)cos ( Xa );
    Sy = (float)sin ( Ya );
    Cy = (float)cos ( Ya );
    Sz = (float)sin ( Za );
    Cz = (float)cos ( Za );

    float a;

    a = (float)1;
    XMat[0][0]=  (float)a; XMat[0][1]=  (float)0; XMat[0][2]=  (float)0; XMat[0][3]=  (float)0;
    XMat[1][0]=  (float)0; XMat[1][1]= (float)Cx; XMat[1][2]= (float)Sx; XMat[1][3]=  (float)0;
    XMat[2][0]=  (float)0; XMat[2][1]=(float)-Sx; XMat[2][2]= (float)Cx; XMat[2][3]=  (float)0;
    XMat[3][0]=  (float)0; XMat[3][1]=  (float)0; XMat[3][2]=  (float)0; XMat[3][3]=  (float)a;
    
    YMat[0][0]= (float)Cy; YMat[0][1]=  (float)0; YMat[0][2]=(float)-Sy; YMat[0][3]=  (float)0;
    YMat[1][0]=  (float)0; YMat[1][1]=  (float)a; YMat[1][2]=  (float)0; YMat[1][3]=  (float)0;
    YMat[2][0]= (float)Sy; YMat[2][1]=  (float)0; YMat[2][2]= (float)Cy; YMat[2][3]=  (float)0;
    YMat[3][0]=  (float)0; YMat[3][1]=  (float)0; YMat[3][2]=  (float)0; YMat[3][3]=  (float)a;
    
    ZMat[0][0]= (float)Cz; ZMat[0][1]= (float)Sz; ZMat[0][2]=  (float)0; ZMat[0][3]=  (float)0;
    ZMat[1][0]=(float)-Sz; ZMat[1][1]= (float)Cz; ZMat[1][2]=  (float)0; ZMat[1][3]=  (float)0;
    ZMat[2][0]=  (float)0; ZMat[2][1]=  (float)0; ZMat[2][2]=  (float)a; ZMat[2][3]=  (float)0;
    ZMat[3][0]=  (float)0; ZMat[3][1]=  (float)0; ZMat[3][2]=  (float)0; ZMat[3][3]=  (float)a;

    MultMatrix ( Matrix, YMat, Mat1 );
    MultMatrix ( Mat1, XMat, Mat2 );
    MultMatrix ( Mat2, ZMat, Matrix );
  } 








float FLPVectorDot ( FLPVECTOR3D *V1, FLPVECTOR3D *V2 )
  {
    float Product;
    Product = V1->x*V2->x + V1->y*V2->y + V1->z*V2->z;
    return Product;
  } 

float FLPVectorDistance ( FLPVECTOR3D *P1, FLPVECTOR3D *P2 )
  {
    float xd,yd,zd;
    float Distance;
    
    xd = P2->x - P1->x;  
    yd = P2->y - P1->y;  
    zd = P2->z - P1->z;

    Distance = xd*xd + yd*yd + zd*zd;
    Distance = (float)sqrt ( Distance );
    return Distance;
  } 

float FLPVectorMagnitude ( FLPVECTOR3D *V1 )
  {
    float Mag;
    
    Mag = V1->x*V1->x + V1->y*V1->y + V1->z*V1->z;
    Mag = (float)sqrt ( Mag );
    return Mag;
  } 

VOID FLPVectorAdd ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result )
  {
    Result->x = V2->x + V1->x;  
    Result->y = V2->y + V1->y;  
    Result->z = V2->z + V1->z;  
  } 

VOID FLPVectorSub ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result )
  {
    Result->x = V2->x - V1->x;  
    Result->y = V2->y - V1->y;  
    Result->z = V2->z - V1->z;  
  } 

VOID FLPVectorNormalize ( FLPVECTOR3D *V1 )
  {
    float Mag;
    float Ratio;
    
    Mag = FLPVectorMagnitude ( V1 );
    if (Mag==0)
      return;
      
    Ratio = (float)1/Mag;
    V1->x *= Ratio;  
    V1->y *= Ratio;  
    V1->z *= Ratio;  
  } 

VOID FLPVectorCross ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result )
  {
    float x1,y1,z1;  
    float x2,y2,z2;

    x1 = V1->x;
    y1 = V1->y;
    z1 = V1->z;
    
    x2 = V2->x;
    y2 = V2->y;
    z2 = V2->z;
    
    Result->x = (y1)*(z2) - (z1)*(y2);
    Result->y = - ((x1)*(z2) - (z1)*(x2));
    Result->z = (x1)*(y2) - (y1)*(x2);
  } 





