/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */


#include "stdgfx.h"
#include "mpgui.h"
#include "mpg3d.h"

#include "stdlib.h"


#include <termios.h>
#include <stdio.h>

static struct termios old, neue;


void initTermios(int echo) 
{
  tcgetattr(0, &old); 
  neue = old; 
  neue.c_lflag &= ~ICANON; 
  neue.c_lflag &= echo ? ECHO : ~ECHO; 
  tcsetattr(0, TCSANOW, &neue); 
}


void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}


char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}


char getch(void) 
{
  return getch_(0);
}


BOOLEAN ReadConfigFile ( STRING FileName );
VOID HandleInput ( G3DSYSTEM* G3D, G3DCAMERA *Camera );
BOOLEAN Create3DDatabase ( G3DSYSTEM *G3D );
VOID DoBenchMark ( G3DSYSTEM *G3D );

IMAGE *VScreen;
INT VideoMode;
BOOLEAN Debug;
float ViewDistance;
float NearClipZ;
BOOLEAN DepthCueing = FALSE;
float DepthScale = 1;
BOOLEAN DoHaze = FALSE;
COLORTABLE *HazeTable;
CHAR HazeTableName[128];
float HazeScale = 1;

CHAR ObjectFileName[128];
float ScaleFactor = 1.0;
BOOLEAN ClockWise = FALSE;
float ObjPosX,ObjPosY,ObjPosZ;
float ObjRotX,ObjRotY,ObjRotZ;
float MaxVisibleDistance;
BOOLEAN LoadAll;

CHAR TextureFileName[128];
BOOLEAN MapTexture;
float U0,V0;
float U1,V1;
float U2,V2;
LONG TextureType;
ANIMIMAGE *Texture;
LONG TextureNumFrames=0;
LONG CurTextureFrame=0;

CHAR ShadeTableFileName[128];
COLORTABLE *ShadeTable;
float LightPosX,LightPosY,LightPosZ;
float LightMaxIntensity;
float LightMinIntensity;
INT AmbientRed,AmbientGreen,AmbientBlue;
BOOLEAN FollowCamera=TRUE;
G3DLIGHT *MyLight;

float CameraPosX,CameraPosY,CameraPosZ;
float CameraAngX,CameraAngY,CameraAngZ;
float AddX,AddY,AddZ;

BOOLEAN BGBitMap;
CHAR BGFileName[128];
IMAGE *BGImage;
INT BGRed,BGGreen,BGBlue;
BYTE BGColor;

BOOLEAN UseTransparent;
COLORTABLE *BlendTable;
CHAR BlendFileName[128];

BOOLEAN ReadConfigFile ( STRING FileName )
  {
    FILEHANDLE f;
    

    f = File.Open ( FileName, OPEN_READ );
    if (f==NULL)
      {
        printf ("Can not open this file!\n" );
        return FAILURE;
      } 

    CHAR Str[128];
    printf ( "Screen Section\n" );
    fscanf ( f, "SCREEN\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    MODE = %s\n", Str );    
    if (strcmp(Str,"320x200x256")==0)
      VideoMode = M320x200x256;
    else if (strcmp(Str,"640x400x256")==0)
      VideoMode = M640x400x256;
    else if (strcmp(Str,"640x480x256")==0)
      VideoMode = M640x480x256;
    else if (strcmp(Str,"800x600x256")==0)
      VideoMode = M800x600x256;
    else if (strcmp(Str,"1024x768x256")==0)
      VideoMode = M1024x768x256;
    else
      {
        printf ("Unknown Video Mode!\n" );
        return FAILURE;
      } 
    printf ( "  Screen = %s\n", Str );

    fscanf ( f, "    DEBUG = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      Debug = TRUE;
    else
      Debug = FALSE;

    printf ( "  Debug = %s\n", Str );

    fscanf ( f, "    VIEW DISTANCE = %f\n", &ViewDistance );
    printf ( "  View Distance = %f\n", ViewDistance );

    fscanf ( f, "    NEAR CLIP Z = %f\n", &NearClipZ );
    printf ( "  Near Clip Z = %f\n", NearClipZ );
    
    fscanf ( f, "    DEPTH CUE = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      DepthCueing = TRUE;
    else
      DepthCueing = FALSE;
    printf ( "  Depth Cueing = %s\n", Str );

    fscanf ( f, "    DEPTH SCALE = %f\n", &DepthScale );
    printf ( "  Depth Scale = %f\n", DepthScale );
      
    fscanf ( f, "    HAZE = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      DoHaze = TRUE;
    else
      DoHaze = FALSE;
    printf ( "  Haze = %s\n", Str );

    fscanf ( f, "    HAZE FILE = %s\n", HazeTableName );
    printf ( "  HAZE FILE = %s\n", HazeTableName );

    fscanf ( f, "    HAZE SCALE = %f\n", &HazeScale );
    printf ( "  Haze File = %f\n", HazeScale );
      
    fscanf ( f, "  END\n" );      
    fscanf ( f, "\n" );
    printf ( "End of Screen Section\n\n" );
    
    printf ( "Object Section\n" );
    fscanf ( f, "OBJECT\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    FILE = %s\n", ObjectFileName );
    printf ( "  Object File = %s\n", ObjectFileName );

    fscanf ( f, "    POSITION = %f %f %f\n", &ObjPosX, &ObjPosY, &ObjPosZ );    
    printf ( "  Position = %f %f %f\n", ObjPosX, ObjPosY, ObjPosZ );

    fscanf ( f, "    SCALE = %f\n", &ScaleFactor );
    printf ( "  Scale = %f\n", ScaleFactor );

    fscanf ( f, "    MAX VISIBLE DISTANCE = %f\n", &MaxVisibleDistance );
    printf ( "  Max Visible Distance = %f\n", MaxVisibleDistance );

    fscanf ( f, "    ROTATE = %f %f %f\n", &ObjRotX, &ObjRotY, &ObjRotZ );
    printf ( "  Rotate = %f %f %f\n", ObjRotX, ObjRotY, ObjRotZ );

    fscanf ( f, "    LOAD ALL = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      LoadAll = TRUE;
    else
      LoadAll = FALSE;  
    printf ( "  Load All = %s\n", Str );

    fscanf ( f, "    CLOCKWISE = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      ClockWise = TRUE;
    else
      ClockWise = FALSE;  
    printf ( "  Clock Wise = %s\n", Str );

    fscanf ( f, "  END\n" );
    fscanf ( f, "\n" );
    printf ( "End of Object Section\n\n" );
    
    printf ( "Material Section\n" );
    fscanf ( f, "MATERIAL\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    FILE = %s\n", TextureFileName );
    printf ( "  Texture File %s\n", TextureFileName );

    fscanf ( f, "    TYPE = %s\n", Str );    
    if (strcmp(Str,"STATIC")==0)
      TextureType = TEXTURE_STATIC;
    else if (strcmp(Str,"FLIC")==0)
      TextureType = TEXTURE_FLIC;
    else if (strcmp(Str,"ANIM")==0)
      TextureType = TEXTURE_ANIM;
    else
      TextureType = 0;  
    printf ( "  Type %s\n", Str );

    fscanf ( f, "    AMBIENT = %d %d %d\n", &AmbientRed, &AmbientGreen, &AmbientBlue );
    printf ( "  Ambient = %d %d %d\n", AmbientRed, AmbientGreen, AmbientBlue );

    fscanf ( f, "    MAP TEXTURE = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      MapTexture = TRUE;
    else
      MapTexture = FALSE;  
    printf ( "  Map Texture = %s\n", Str );

    fscanf ( f, "    U0 V0 = %f %f\n", &U0, &V0 );
    printf ( "U0 = %f  V0 = %f\n", U0, V0 );
    fscanf ( f, "    U1 V1 = %f %f\n", &U1, &V1 );
    printf ( "U1 = %f  V1 = %f\n", U1, V1 );
    fscanf ( f, "    U2 V2 = %f %f\n", &U2, &V2 );
    printf ( "U2 = %f  V2 = %f\n", U2, V2 );
    
    fscanf ( f, "    TRANSPARENT = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      UseTransparent = TRUE;
    else
      UseTransparent = FALSE;  
    printf ( "  Transparent = %s\n", Str );

    fscanf ( f, "  END\n" );
    fscanf ( f, "\n" );
    printf ( "End of Material Section\n\n" );
        
    printf ( "Light Section\n" );
    fscanf ( f, "LIGHT\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    FILE = %s\n", ShadeTableFileName );
    printf ( "  Shade Table File = %s\n", ShadeTableFileName );

    fscanf ( f, "    POSITION = %f %f %f\n", &LightPosX, &LightPosY, &LightPosZ );
    printf ( "  Position = %f %f %f\n", LightPosX, LightPosY, LightPosZ );

    fscanf ( f, "    FOLLOW CAMERA = %s\n", Str );
    if (strcmp(Str,"YES")==0)
      FollowCamera = TRUE;
    else
      FollowCamera = FALSE;  
    printf ( "  FOLLOW CAMERA = %s\n", Str );
    
    fscanf ( f, "    MAX INTENSITY = %f\n", &LightMaxIntensity );
    printf ( "  Max Intensity = %f\n", LightMaxIntensity );

    fscanf ( f, "    MIN INTENSITY = %f\n", &LightMinIntensity );
    printf ( "  Min Intensity = %f\n", LightMinIntensity );

    fscanf ( f, "  END\n" );
    fscanf ( f, "\n" );
    printf ( "End of Light Section\n\n" );
        
    printf ( "Camera Section\n" );
    fscanf ( f, "CAMERA\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    POSITION = %f %f %f\n", &CameraPosX, &CameraPosY, &CameraPosZ );
    printf ( "  Position = %f %f %f\n", CameraPosX, CameraPosY, CameraPosZ );

    fscanf ( f, "    ANGLE = %f %f %f\n", &CameraAngX, &CameraAngY, &CameraAngZ );
    printf ( "  Angle = %f %f %f\n", CameraAngX, CameraAngY, CameraAngZ );

    fscanf ( f, "  END\n" );
    fscanf ( f, "\n" );
    printf ( "End of Camera Section\n\n" );

    printf ( "Background Section\n" );
    fscanf ( f, "BACKGROUND\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    FILE = %s\n", BGFileName );
    if (strcmp(BGFileName,"NULL")==0)
      BGBitMap = FALSE;
    else
      BGBitMap = TRUE;
    printf ( "  Background File Name = %s\n", BGFileName );

    fscanf ( f, "    COLOR = %d %d %d\n", &BGRed, &BGGreen, &BGBlue );
    printf ( "  Color = %d %d %d\n", BGRed, BGGreen, BGBlue );

    fscanf ( f, "  END\n" );
    fscanf ( f, "\n" );
    printf ( "End of Background Section\n\n" );

    printf ( "Color Blending Section\n" );
    fscanf ( f, "COLOR BLENDING\n" );
    fscanf ( f, "  BEGIN\n" );
    fscanf ( f, "    FILE = %s\n", BlendFileName );
    printf ( "  Blend File Name = %s\n", BlendFileName );

    fscanf ( f, "  END\n" );
    printf ( "End of Color Blending Section\n\n" );

    File.Close ( f );

    return SUCCESS;    
  } 

BOOLEAN BenchMark = FALSE;

int main (int argc, char * * argv ) {

    U0 = V0 = (float)0;
    U1 = (float)0;
    V1 = (float)0.25;
    U2 = V2 = (float)0.25;
    
    VScreen = new IMAGE ( IMAGE_TOPDOWN );
    BGImage = new IMAGE ( IMAGE_TOPDOWN );
    ShadeTable = new COLORTABLE ();
    BlendTable = new COLORTABLE ();
    HazeTable = new COLORTABLE ();

    if (GetNumArgs()==0)
      {
        printf ("You must specify config file!\n" );

      } 
    if (GetNumArgs()==2)
      {
        CHAR ExStr[128];
        GetArg ( 1, ExStr );
        
        if (strcmp( ExStr, "/b")==0)
          BenchMark = TRUE; 
      } 

    CHAR ConfigFile[128]= "face.cfg";
    

    printf ( "Initializing please wait ...\n\n" );
    if (ReadConfigFile ( ConfigFile )==FAILURE)
      {
        printf ("Error reading config file!\n" );
        exit (0);
      } 
    else
      {
        printf ("Config File Loaded.\n\n" );
      } 
    
    G3DSYSTEM G3D;
    G3DCAMERA Camera;
    

printf ("Camera.Move CameraPosX=%d, CameraPosY=%d, CameraPosZ=%d  \n" , CameraPosX, CameraPosY, CameraPosZ );
    Camera.Move ( CameraPosX, CameraPosY, CameraPosZ );
printf ("Camera.SetAngle CameraAngX=%d, CameraAngY=%d, CameraAngZ=%d  \n" , CameraAngX, CameraAngY, CameraAngZ );
    Camera.SetAngle ( CameraAngX, CameraAngY, CameraAngZ );

    G3D.SetDestination ( VScreen );
    G3D.SetViewDistance ( ViewDistance );
    G3D.SetNearClipZ ( NearClipZ );
    G3D.SetDepthCueing ( DepthCueing, DepthScale );
    
    if (Create3DDatabase(&G3D)==FAILURE)
      {
        printf ("Error creating 3D environment!\n" );
        exit (0);
      } 
    
    G3D.Init ();

    DISPLAYDATA DisplayData;

    DisplayData.Mode = VideoMode;


    Grafix.SetDisplay ( &DisplayData );

printf(" >> F  4, about to set palette ....  ShadeTable->GetPalette () = %p \n", ShadeTable->GetPalette () );
    Grafix.SetPalette ( HVGA, ShadeTable->GetPalette () );


    if (Debug)
      VScreen->SetDebugFlag ( TRUE );
    
printf(" >> F  6, Grafix.GetWidth ( HVGA ) = %d     Grafix.GetHeight ( HVGA ) = %d \n", Grafix.GetWidth ( HVGA ), Grafix.GetHeight ( HVGA ) );
    VScreen->Create ( IMAGE_8BIT, Grafix.GetWidth ( HVGA ), Grafix.GetHeight ( HVGA ) );

    G3D.SetScreenCenter ( VScreen->GetWidth()/2, VScreen->GetHeight()/2 );
    Input.Init ();


    LONG Count;
    Count = 0;    

    MYTIMER MyTimer;
    MyTimer.StartTimer ();

    G3DOBJECT *Object;
    Object = G3D.FindObjectByID ( 1 );


    IMAGE *DisplayBG = NULL;
    if (BGBitMap)
      {
printf(" >> F  12\n");
        DisplayBG = new IMAGE ( IMAGE_TOPDOWN );
printf(" >> F  13, VScreen->GetWidth() = %d , VScreen->GetHeight() = %d \n", VScreen->GetWidth(), VScreen->GetHeight() );
        DisplayBG->Create ( IMAGE_8BIT, VScreen->GetWidth(), VScreen->GetHeight() );
printf(" >> F  14\n");
        Grafix.SetScaleFactor ( DisplayBG->GetWidth(), BGImage->GetWidth(),
                                DisplayBG->GetHeight(), BGImage->GetHeight() );
printf(" >> F  15\n");
        Grafix.ScaleImage ( BGImage, 0, 0, BGImage->GetWidth(), BGImage->GetHeight(),
                            DisplayBG, 0, 0 );

printf(" >> F  16\n");
        Grafix.SetScaleFactor ( 1, 1, 1, 1 );
printf(" >> F  17\n");
      } 

    if (BenchMark)
      {
printf(" >> F  0-18\n");
        DoBenchMark ( &G3D );
printf(" >> F  0-19\n");
        exit(0);
      } 

int _t = 0;

    
    while (_t--)
      {
printf(" >> F  18\n");
        AddX = AddY = AddZ = 0;
        Count++;
printf(" >> F  19\n");
        if (BGBitMap!=TRUE)
	{
printf(" >> BGBitMap  is not TRUE \n");
          VScreen->Clear ( BGColor );
	}
        else
          {
printf(" >> BGBitMap  is not TRUE \n");
            Grafix.CopyImage ( DisplayBG, 0, 0, DisplayBG->GetWidth(),
                               DisplayBG->GetHeight(),
                               VScreen, 0, 0 );
          } 
printf(" >> F  29\n");
        if (Object!=NULL)
          Object->Rotate ( ObjRotX, ObjRotY, ObjRotZ );
        if (TextureNumFrames)
          {
            Texture->SetFrame ( CurTextureFrame++ );
            if (CurTextureFrame>=TextureNumFrames)
              CurTextureFrame = 0;
          } 

printf(" >> F  39\n");
        HandleInput ( &G3D, &Camera );
printf(" >> F  40\n");
        G3D.ShowView ( &Camera );
printf(" >> F  41, VScreen->GetWidth() = %d , VScreen->GetHeight() = %d \n", VScreen->GetWidth(), VScreen->GetHeight() );
            Grafix.DisplayImage ( HVGA, VScreen, 0, 0, VScreen->GetWidth(),
                              VScreen->GetHeight(), 0, 0 );
printf(" >> F  42\n");

      } 
    MyTimer.EndTimer ();


printf(" >> F  43\n");
    Input.DeInit ();
printf(" >> F  44  ATTENTION: Skitting      <<ResetDisplay()>> .... \n");    

printf(" >> F  45\n");    
    float FrameRate;
printf(" >> F  46\n");    
    FrameRate = MyTimer.GetFrameRate ( Count );
printf(" >> F  47\n");    
    printf ( "Frame rate was %4.2f fps.\n", FrameRate );


    delete VScreen;
    delete ShadeTable;
    if (DisplayBG!=NULL)
      delete DisplayBG;
    delete BGImage;  
    return 0;

}
  
VOID HandleInput ( G3DSYSTEM* G3D, G3DCAMERA *Camera )
  {
    if (Input.IsKeyDown(SC_F))
      {
        G3D->SetShadeFlags ( SHADE_FLAT );  
      } 
    else if (Input.IsKeyDown(SC_G))
      {
        G3D->SetShadeFlags ( SHADE_GOURAUD );  
      } 
    else if (Input.IsKeyDown(SC_N))
      {
        G3D->SetShadeFlags ( SHADE_NONE );  
      } 

    if (Input.IsKeyDown(SC_S))
      {
        G3D->SetFaceFlags ( FACE_SOLID );  
      } 
    else if (Input.IsKeyDown(SC_T))
      {
        G3D->SetFaceFlags ( FACE_TEXTURE );  
      } 
    else if (Input.IsKeyDown(SC_W))
      {
        G3D->SetFaceFlags ( FACE_WIREFRAME );  
      } 

    G3DMATERIALLIB *MaterialLib;
    G3DMATERIAL *Material;

    MaterialLib = G3D->GetMaterialLib ();
    Material = MaterialLib->GetMaterial ( 0 );
      
    BOOLEAN IsShift;
    IsShift = Input.IsKeyDown(SC_LSHIFT) || Input.IsKeyDown(SC_RSHIFT);
    
    if (Input.IsKeyDown(SC_USCORE))
      {
        if (Material->Transparency<10-1)
          Material->Transparency++;  
      } 
      
    if (Input.IsKeyDown(SC_EQUAL))
      {
        if (Material->Transparency>0)
          Material->Transparency--;  
      } 
    
    float SpeedRatio = 1.0;
    if (IsShift)
      {
        SpeedRatio = 2.0;  
      } 
    
    COLLIDEDATA CollideList[5];
    FLPVECTOR3D Position,EndPt;
    FLPVECTOR3D Dir;

    Position = Camera->GetWorldPosition();
    
    if (Input.IsKeyDown(SC_RIGHT))
      {
        Camera->Rotate ( 0, -3*SpeedRatio, 0 );  
      } 
    if (Input.IsKeyDown(SC_LEFT))
      {
        Camera->Rotate ( 0, 3*SpeedRatio, 0 );  
      } 
    if (Input.IsKeyDown(SC_UP))
      {
        Camera->Rotate ( -3*SpeedRatio, 0, 0 );  
      } 
    if (Input.IsKeyDown(SC_DOWN))
      {
        Camera->Rotate ( 3*SpeedRatio, 0, 0 );  
      } 

    LONG CollisionCount;
      
    if (Input.IsKeyDown(SC_A))
      {
        Dir.x = 0;
        Dir.y = 0;
        Dir.z = 1*20;

        EndPt = G3D->ComputeNextPos ( Position, Camera->GetAngle(), Dir );
        CollisionCount = G3D->CheckCollision ( Position, EndPt, CollideList, 5, 1.6, 10 );
        if (CollisionCount==0)
          Camera->Move ( EndPt.x-Position.x, EndPt.y-Position.y, EndPt.z-Position.z );
      } 
    if (Input.IsKeyDown(SC_Z))
      {
        Dir.x = 0;
        Dir.y = 0;
        Dir.z = -1*20;

        EndPt = G3D->ComputeNextPos ( Position, Camera->GetAngle(), Dir );  
        CollisionCount = G3D->CheckCollision ( Position, EndPt, CollideList, 5, 1.6, 10 );
        if (CollisionCount==0)
          Camera->Move ( EndPt.x-Position.x, EndPt.y-Position.y, EndPt.z-Position.z );
      } 

    if (FollowCamera)
      {
        Position = Camera->GetWorldPosition();
        MyLight->SetPosition ( Position.x, Position.y, Position.z );  
      } 
  } 
  
BOOLEAN Create3DDatabase ( G3DSYSTEM *G3D )
  {
    G3DWORLD *NewWorld;
    RGBPALETTE *LocalPal = new RGBPALETTE ();
    COLORTABLE *MatchTable = new COLORTABLE ();

    
    NewWorld = new G3DWORLD ();
    
    printf ("\n**** Creating World ****\n\n" );

    
    if (BlendTable->Load ( BlendFileName )==FAILURE)
      {
        printf ("Error loading Blending Table!\n" );
        exit (0);
      } 
    else
      {
        printf ("Blend File Loaded.\n" );
      } 
    G3D->SetBlendTable ( BlendTable );

    if (DoHaze)
      {
        if (HazeTable->Load ( HazeTableName )==FAILURE)
          {
            printf ("Error loading Hazing Table!\n" );
            exit (0);
          } 
        else
          {
            printf ("Haze File Loaded.\n" );
          } 
      } 
    else
      printf ("Haze is disabled.\n" );

    G3D->SetHazing ( DoHaze, HazeScale, HazeTable );

    
    G3DLIGHT *Light;
    Light = new G3DLIGHT ();
    Light->SetName ( "Light01" );
    Light->SetPosition ( LightPosX, LightPosY, LightPosZ );
    if (ShadeTable->Load ( ShadeTableFileName )==FAILURE)
      {
        printf ("Can not load shade table! : %s\n", ShadeTableFileName );
        return FAILURE;  
      } 
    else
      {
        printf ("Shade File Loaded.\n" );
      } 

    Light->SetMaxIntensity ( LightMaxIntensity ); 
    Light->SetMinIntensity ( LightMinIntensity ); 
    Light->SetShadeTable ( ShadeTable );    
    NewWorld->AddObject ( Light );
    MyLight = Light;

    
    RGBCOLOR BG;
    BG.Red = (BYTE)BGRed;
    BG.Green = (BYTE)BGGreen;
    BG.Blue = (BYTE)BGBlue;
    BGColor = (BYTE)((ShadeTable->GetPalette())->GetClosestColor ( BG ));
    if (BGBitMap)
      {
        if (Grafix.LoadImage ( BGFileName, BGImage, LocalPal )==FAILURE)
          {
            printf ("Can not load Background Image! : %s\n", BGFileName );
            return FAILURE;  
          } 
        else
          {
            printf ("Background Image Loaded.\n" );
          } 

        MatchTable->CopyPalette ( ShadeTable->GetPalette() );
        MatchTable->CreateMatchTable ( LocalPal );
        Grafix.ConvertImage ( BGImage, MatchTable );
      } 

    
    G3DMATERIALLIB *MaterialLib;
    MaterialLib = new G3DMATERIALLIB ();
    MaterialLib->CreateMaterials ( 1 );
    MaterialLib->CreateTextures ( 1 );
    G3D->SetMaterialLib ( MaterialLib );

    
    G3DMATERIAL *Material;
    Material = MaterialLib->GetMaterial ( 0 );
    RGBCOLOR AmbientColor;
    AmbientColor.Red = (BYTE)AmbientRed;
    AmbientColor.Green = (BYTE)AmbientGreen;
    AmbientColor.Blue = (BYTE)AmbientBlue;
    if (UseTransparent)
      Material->Attributes = MATERIAL_TRANSPARENT;
    Material->Transparency = 0;
    Material->Ambient = (ShadeTable->GetPalette())->GetClosestColor ( AmbientColor );

    
    if (MaterialLib->LoadTexture ( 0, TextureFileName, TextureType, ShadeTable->GetPalette() )==FAILURE)
      {
        printf ("Can not load this Texture! : %s\n", TextureFileName );
        return FAILURE;  
      } 
    else
      {
        printf ("Texture Loaded.\n" );
      } 
      
    Texture = MaterialLib->GetTexture ( 0 );
    TextureNumFrames = Texture->GetNumFrames ();
    Material->SetTexture ( Texture );
    
    
    LINKEDLIST <G3DOBJECT*> *ObjectList;
    ObjectList = new LINKEDLIST<G3DOBJECT*> ();
    
    if (G3D->Load3DFile ( ObjectFileName, ObjectList, ClockWise, ScaleFactor )==FAILURE)
      {
        printf ("Can not load 3D object! : %s\n", ObjectFileName );
        return FAILURE;  
      } 
    else
      {
        printf ("3D Object(s) Loaded.\n" );
      } 
    
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT *Object;
    G3DPOLYHEDRON *PolyObject;
    
    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data;
        Object->SetMaxVisibleDistance ( MaxVisibleDistance );
        if (Object->GetObjectType()==OBJECT_TYPE_POLYHEDRON)
          {
            Object->SetPosition ( ObjPosX, ObjPosY, ObjPosZ );
            Object->SetID ( 1 );

            PolyObject = (G3DPOLYHEDRON*)Object;  
            PolyObject->SetLightSource ( Light );

            PolyObject->SetShadeFlags ( MAX_DETAIL, SHADE_GOURAUD );
            PolyObject->SetFaceFlags ( MAX_DETAIL, FACE_TEXTURE );
            
            PolyObject->SetMaterial ( Material );
            if (MapTexture)
              PolyObject->MapTexture ( U0, V0, U1, V1, U2, V2 );

            NewWorld->AddObject ( PolyObject );
            ObjectNode->Data = NULL;

            if (!LoadAll)
              break;
          } 
                    
        ObjectNode = ObjectNode->NextObject;              
      } 
       
    ObjectList->ClearAllNode ();
    delete ObjectList;
    
    delete LocalPal;
    delete MatchTable;
    
    printf ("\n**** World Is Created ****\n" );

    G3D->SetWorld ( NewWorld, TRUE );

    return SUCCESS;
  } 
                                                          
VOID DoBenchMark ( G3DSYSTEM *G3D )
  {
    float FrameRate1,FrameRate2;

    LONG Count;
    MYTIMER MyTimer;

    G3D->SetViewPort ( 0, 0, 319, 199 );

    FrameRate1 = FrameRate2 = 0;

    FXPPOINT2D FxpPoints[3];
    
    FxpPoints[0].x=150; FxpPoints[0].y=50;
      FxpPoints[0].u = (LONG)(U0*255*65536); FxpPoints[0].v = (LONG)(V0*255*65536);
      FxpPoints[0].I = (LONG)0; FxpPoints[0].Haze = (LONG)0;
       
    FxpPoints[1].x=50; FxpPoints[1].y=150;   
      FxpPoints[1].u = (LONG)(U1*255*65536); FxpPoints[1].v = (LONG)(V1*255*65536);
      FxpPoints[1].I = (LONG)32; FxpPoints[1].Haze = (LONG)32;
      
    FxpPoints[2].x=150; FxpPoints[2].y=150;
      FxpPoints[2].u = (LONG)(U2*255*65536); FxpPoints[2].v = (LONG)(V2*255*65536);
      FxpPoints[2].I = (LONG)63; FxpPoints[2].Haze = (LONG)63;

    MyTimer.StartTimer ();
    Count = 0;
    while (Input.IsKeyDown(SC_ESC)!=TRUE)
      {
        Count++;
        G3D->TestDrawTriangle ( FxpPoints, Texture, 63, Count, SHADE_NONE,
                                FACE_TEXTURE, NULL,
                                ShadeTable, NULL ); 
      } 
    MyTimer.EndTimer ();

    FrameRate1 = MyTimer.GetFrameRate ( Count );

    
    while (Input.IsKeyDown(SC_ESC))
      {}

    FxpPoints[0].x <<= 16;
    FxpPoints[1].x <<= 16;
    FxpPoints[2].x <<= 16;

    FxpPoints[0].I <<= 16;
    FxpPoints[1].I <<= 16;
    FxpPoints[2].I <<= 16;

    MyTimer.StartTimer ();
    Count = 0;
    while (Input.IsKeyDown(SC_ESC)!=TRUE)
      {
        Count++;
        G3D->TestDrawTriangle2 ( FxpPoints, Texture, 63, Count, SHADE_NONE,
                                 FACE_TEXTURE, NULL,
                                 ShadeTable, NULL ); 
      } 
    MyTimer.EndTimer ();

    FrameRate2 = MyTimer.GetFrameRate ( Count );

    Input.DeInit ();
    Grafix.ResetDisplay ();
      
    printf ( "1. Frame rate was %4.2f fps.\n", FrameRate1 );
    printf ( "2. Frame rate was %4.2f fps.\n", FrameRate2 );
    getch ();

    delete VScreen;
    delete ShadeTable;
    delete BlendTable;
    delete HazeTable;
    delete BGImage;  
  } 
  
