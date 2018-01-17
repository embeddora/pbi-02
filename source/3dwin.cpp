


#include "stdgfx.h"
#include "mpgui.h"
#include "mpg3d.h"
#include "dlgs.h"

#define MENU_FILE_OPEN  101
#define MENU_FILE_EXIT  102

#define MENU_DETAIL_FACE 210
  #define MENU_DETAIL_FACE_TEXTURE   211
  #define MENU_DETAIL_FACE_SOLID     212
  #define MENU_DETAIL_FACE_WIREFRAME 213

#define MENU_DETAIL_SHADE 220
  #define MENU_DETAIL_SHADE_GOURAUD  221
  #define MENU_DETAIL_SHADE_FLAT     222
  #define MENU_DETAIL_SHADE_NONE     223

#define MENU_MATERIAL_LOAD             310
  #define MENU_MATERIAL_LOAD_STATIC    311
  #define MENU_MATERIAL_LOAD_ANIMATED  312
  
#define MENU_MATERIAL_TRANSPARENCY      320
  #define MENU_MATERIAL_TRANSPARENCY_10 321  
  #define MENU_MATERIAL_TRANSPARENCY_20 322
  #define MENU_MATERIAL_TRANSPARENCY_30 323  
  #define MENU_MATERIAL_TRANSPARENCY_40 324  
  #define MENU_MATERIAL_TRANSPARENCY_50 325  
  #define MENU_MATERIAL_TRANSPARENCY_60 326  
  #define MENU_MATERIAL_TRANSPARENCY_70 327  
  #define MENU_MATERIAL_TRANSPARENCY_80 328  
  #define MENU_MATERIAL_TRANSPARENCY_90 329  
  #define MENU_MATERIAL_TRANSPARENCY_00 330
    
#define MENU_MATERIAL_AMBIENT           331 

#define MENU_ENVIRONMENT_BACKGROUND     
  #define MENU_ENVIRONMENT_BACKGROUND_LOAD    411
  #define MENU_ENVIRONMENT_BACKGROUND_STATIC  412
  
#define MENU_ENVIRONMENT_DEPTH_CUE   421
#define MENU_ENVIRONMENT_FOG         431

#define MENU_ROTATION_X  510
  #define MENU_ROTATION_X_INC          511
  #define MENU_ROTATION_X_DEC          512
  #define MENU_ROTATION_X_STOP         513
#define MENU_ROTATION_Y  520
  #define MENU_ROTATION_Y_INC          521
  #define MENU_ROTATION_Y_DEC          522
  #define MENU_ROTATION_Y_STOP         523
#define MENU_ROTATION_Z  530
  #define MENU_ROTATION_Z_INC          531
  #define MENU_ROTATION_Z_DEC          532
  #define MENU_ROTATION_Z_STOP         533

#define MENU_HELP_ABOUT              601







class MAINFRAME : public GUIFRAME
  {
    protected :
      GUIMENU *Menu;
      G3DSYSTEM *G3D;
      G3DCAMERA *Camera;
      COLORTABLE *ShadeTable;
      COLORTABLE *HazeTable;
      COLORTABLE *BlendTable;
      RGBPALETTE *Pal;
      IMAGE *VScreen;
      IMAGE *BGImage;
      LONG BGColor;
      float NearClipZ;
      float ViewDistance;
      float MaxVisibleDistance;
      float U0,V0;
      float U1,V1;
      float U2,V2;
      float ObjPosX,ObjPosY,ObjPosZ;
      float ObjRotX,ObjRotY,ObjRotZ;
      float LightPosX,LightPosY,LightPosZ;
      INT AmbientRed,AmbientGreen,AmbientBlue;
      INT BGRed,BGGreen,BGBlue;
      LONG NumAnimatedFrames;
      LONG CurAnimatedFrame;
      
      BOOLEAN MapTexture;
      BOOLEAN Initialized;
      BOOLEAN TimerOn;
      BOOLEAN ObjectLoaded;
      BOOLEAN UseBG;
      BOOLEAN AnimatedTexture;

      BOOLEAN Init3D ();
      
      BOOLEAN Load3DFile ( STRING FileName );
      VOID OnFileLoad ();
      VOID OnFog ();
      VOID OnTransparent ( LONG Code );
      
      VOID OnDetail ( LONG Code );
      
      VOID OnLoadTextureStatic ();
      VOID OnLoadTextureAnimated ();
      VOID OnAmbient ();

      VOID OnLoadBG ();
      VOID OnBGStatic ();

      VOID OnDepthCue ();

      VOID OnRotation ( LONG Code );

      VOID OnAbout ();
      
      VOID HandleInput ();
      
      virtual LONG OnCreate ();
      virtual LONG OnCommand ( LONG Command );
      virtual LONG OnClose ();
      virtual LONG OnTimer ();
      virtual LONG OnPosChanging ( WINPOSCHANGE *WinPosChange );
      virtual LONG OnSize ( LONG Width, LONG Height );
      virtual LONG OnPaint ( HDISPLAY hDisplay );
      
    public :
      MAINFRAME ();        
      virtual ~MAINFRAME ();
  }; 

MAINFRAME::MAINFRAME ()
  {
    Menu = new GUIMENU ();

    G3D = NULL;
    Camera = NULL;
    ShadeTable = NULL;
    BlendTable = NULL;
    HazeTable = NULL;
    BGImage = NULL;

    NearClipZ = (float)10;
    ViewDistance = (float)200;
    MaxVisibleDistance = (float)3000;

    U0 = (float)0; V0 = (float)0;
    U1 = (float)0; V1 = (float)0.5;
    U2 = (float)0.5; V2 = (float)0.5;

    ObjPosX = (float)0;
    ObjPosY = (float)0; 
    ObjPosZ = (float)500;

    ObjRotX = (float)3;
    ObjRotY = (float)3;
    ObjRotZ = (float)3;

    LightPosX = (float)0;
    LightPosY = (float)0;
    LightPosZ = (float)0;

    AmbientRed = 255;
    AmbientGreen = 0;
    AmbientBlue = 0;
    
    BGRed = 128;
    BGGreen = 128;
    BGBlue = 128;
    
    TimerOn = FALSE;
    MapTexture = FALSE;
    ObjectLoaded = FALSE;
    Initialized = FALSE;
    UseBG = FALSE;

    BGColor = 0;

    NumAnimatedFrames = 0;
    CurAnimatedFrame = 0;
    AnimatedTexture = FALSE;
  } 

MAINFRAME::~MAINFRAME ()
  {
    if (G3D!=NULL)
      delete G3D;
    G3D = NULL;

    if (Camera!=NULL)
      delete Camera;
    Camera = NULL;
    
    if (ShadeTable!=NULL)
      delete ShadeTable;
    ShadeTable = NULL;

    if (VScreen!=NULL)
      delete VScreen;
    VScreen = NULL;

    if (BGImage!=NULL)
      delete BGImage;
    BGImage = NULL;    
  } 

BOOLEAN MAINFRAME::Init3D ()
  {
    G3D = new G3DSYSTEM ();
    Camera = new G3DCAMERA ();
    
    ShadeTable = new COLORTABLE ();  
    BlendTable = new COLORTABLE ();  
    HazeTable = new COLORTABLE ();
    VScreen = new IMAGE ( IMAGE_BOTTOMUP );

    if (ShadeTable->Load ( "./data/color.tbl" )==FAILURE)
      {
        printf("Error loading Shade Table");
        return FAILURE;
      } 
    if (BlendTable->Load ( "./data/blend.tbl" )==FAILURE)
      {
        printf("Error loading Blend Table");
        return FAILURE;
      } 
    if (HazeTable->Load ( "./data/whaze.tbl" )==FAILURE)
      {
        printf("Error loading Haze Table");
        return FAILURE;
      } 

    Pal = ShadeTable->GetPalette ();
    RGBCOLOR Col;
    Col.Red = (BYTE)BGRed;
    Col.Green = (BYTE)BGGreen;
    Col.Blue = (BYTE)BGBlue;
    BGColor = Pal->GetClosestColor ( Col );
    
    G3D->SetBlendTable ( BlendTable );
    G3D->SetHazing ( FALSE, (float)100, HazeTable );
    G3D->SetNearClipZ ( NearClipZ );
    G3D->SetViewDistance ( ViewDistance );

    G3DMATERIALLIB *MaterialLib = new G3DMATERIALLIB ();
    MaterialLib->CreateTextures ( 1 );
    MaterialLib->CreateMaterials ( 1 );    
    G3D->SetMaterialLib ( MaterialLib );
    
    
    G3DMATERIAL *Material;
    Material = MaterialLib->GetMaterial ( 0 );
    Material->SetID ( 0 );
    RGBCOLOR AmbientColor;
    AmbientColor.Red = (BYTE)AmbientRed;
    AmbientColor.Green = (BYTE)AmbientGreen;
    AmbientColor.Blue = (BYTE)AmbientBlue;
    Material->Transparency = 0;
    Material->Ambient = Pal->GetClosestColor ( AmbientColor );

    
    MaterialLib->LoadTexture ( 0, "./data/checker.pcx", TEXTURE_STATIC, ShadeTable->GetPalette() );      
    Material->SetTexture ( MaterialLib->GetTexture(0) );

    
    Camera->SetPosition ( (float)0, (float)0, (float)0 );
    Camera->SetAngle ( (float)0, (float)0, (float)0 );

    Initialized = TRUE;
    return SUCCESS;
  } 

BOOLEAN MAINFRAME::Load3DFile ( STRING FileName )
  {      
    G3DWORLD *World = new G3DWORLD;            
    if (World==NULL)
      return FAILURE;
    
    LINKEDLIST<G3DOBJECT*> *ObjectList = new LINKEDLIST<G3DOBJECT*> ();
    if (G3D->Load3DFile ( FileName, ObjectList, FALSE, 1.0 )==FAILURE)
      {
        printf("Error loading 3D", "Error occured at loading this file");
        delete ObjectList;
        delete World;
        return FAILURE;
      } 
      
    G3DLIGHT *Light = new G3DLIGHT ();

    Light->SetName ( "Light01" );
    Light->SetPosition ( LightPosX, LightPosY, LightPosZ );
    Light->SetShadeTable ( ShadeTable );
    Light->SetMaxIntensity ( (float)63 );
    Light->SetMinIntensity ( (float)0 );

    World->AddObject ( Light );

    G3DMATERIAL *Material = G3D->FindMaterialByIndex ( 0 );
    
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT *Object;
    G3DPOLYHEDRON *PolyObject;
    
    ObjectNode = ObjectList->GetHead ();
    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data;
        
        if (Object->GetObjectType()==OBJECT_TYPE_POLYHEDRON)
          {
            Object->SetMaxVisibleDistance ( MaxVisibleDistance );
            Object->SetPosition ( ObjPosX, ObjPosY, ObjPosZ );
            Object->SetID ( 1 );

            PolyObject = (G3DPOLYHEDRON*)Object;  
            PolyObject->SetLightSource ( Light );

            PolyObject->SetShadeFlags ( MAX_DETAIL, SHADE_GOURAUD );
            PolyObject->SetFaceFlags ( MAX_DETAIL, FACE_TEXTURE );
            PolyObject->ComputeCenter ();
            PolyObject->ComputeRadius ();

            float Radius;
            float ScaleFactor;
            Radius = PolyObject->GetRadius ();
            if (Radius==0)
              Radius = (float)1;
            ScaleFactor = (float)(250 / Radius);
            PolyObject->SetScaleFactor ( ScaleFactor );
            PolyObject->SetMaterial ( Material );
            if (PolyObject->TextureMapped!=TRUE)
              PolyObject->MapTexture ( U0, V0, U1, V1, U2, V2 );
            
            World->AddObject ( PolyObject );
            ObjectNode->Data = NULL;
            break;
          } 
        ObjectNode = ObjectNode->NextObject;  
      } 
      
    ObjectList->ClearAllNode ();  
    delete ObjectList;

    G3D->SetWorld ( World, TRUE );
    G3D->Init ();
    return SUCCESS;
  } 

VOID MAINFRAME::OnFileLoad ()
  {
    if (TimerOn)
      KillTimer ();

#if 1        
    GUIFILEDIALOG *FileDialog;
    FileDialog = new GUIFILEDIALOG ( FD_OPEN );
    FileDialog->CreateExtensions ( 5 );
    FileDialog->SetExtension ( 0, "*.3DS", "Autodesk 3D Studio files (*.3DS)" );
    FileDialog->SetExtension ( 1, "*.ASC", "ASCII files (*.ASC)" );
    FileDialog->SetExtension ( 2, "*.GEO", "View Space files (*.GEO)" );
    FileDialog->SetExtension ( 3, "*.GEM", "Geometry files (*.GEM)" );
    FileDialog->SetExtension ( 4, "*.*", "All files (*.*)" );

    LONG Result;    
    Result = FileDialog->Execute ( this, "File Open" );
    if (Result)
      {
        CHAR FileName[128];
        FileDialog->GetFileName ( FileName );  
        if (Load3DFile ( FileName )==SUCCESS)
          {
            TimerOn = TRUE;
            ObjectLoaded = TRUE;
            SetTimer ( 0 );  
          } 
      } 
      
    delete FileDialog;
#endif 

  } 

VOID MAINFRAME::OnDetail ( LONG Code )
  {
    switch (Code)
      {
        case MENU_DETAIL_FACE_TEXTURE :
          G3D->SetFaceFlags ( FACE_TEXTURE );
          break;  
        case MENU_DETAIL_FACE_SOLID :
          G3D->SetFaceFlags ( FACE_SOLID );
          break;  
        case MENU_DETAIL_FACE_WIREFRAME :
          G3D->SetFaceFlags ( FACE_WIREFRAME );
          break;  

        case MENU_DETAIL_SHADE_GOURAUD :
          G3D->SetShadeFlags ( SHADE_GOURAUD );
          break;  
        case MENU_DETAIL_SHADE_FLAT :
          G3D->SetShadeFlags ( SHADE_FLAT );
          break;  
        case MENU_DETAIL_SHADE_NONE :
          G3D->SetShadeFlags ( SHADE_NONE );
          break;  
      } 
  } 

VOID MAINFRAME::OnLoadTextureStatic ()
  {
    if (TimerOn)
      KillTimer ();
        
#if 1
    GUIFILEDIALOG *FileDialog;
    FileDialog = new GUIFILEDIALOG ( FD_OPEN );
    FileDialog->CreateExtensions ( 4 );
    FileDialog->SetExtension ( 0, "*.BMP", "Windows BMP (*.BMP)" );
    FileDialog->SetExtension ( 1, "*.GIF", "CompuServ GIF (*.GIF)" );
    FileDialog->SetExtension ( 2, "*.PCX", "Z Soft PCX (*.PCX)" );
    FileDialog->SetExtension ( 3, "*.*", "All files (*.*)" );

    LONG Result;    
    Result = FileDialog->Execute ( this, "File Open" );
    if (Result)
      {
        CHAR FileName[128];
        FileDialog->GetFileName ( FileName );

        G3DMATERIALLIB *MaterialLib = G3D->GetMaterialLib ();
        
        
        if (MaterialLib->LoadTexture ( 0, FileName, TEXTURE_STATIC, Pal )==SUCCESS)
          {
            AnimatedTexture = FALSE;    
            NumAnimatedFrames = 0;
            CurAnimatedFrame = 0;
          } 
        else  
          {
            printf ( "Can not load this Texture!" );
          } 
      } 
      
    if (TimerOn)
      SetTimer ( 0 );  
    delete FileDialog;      
#endif 
  } 

VOID MAINFRAME::OnLoadTextureAnimated ()
  {
    if (TimerOn)
      KillTimer ();
#if 1        
    GUIFILEDIALOG *FileDialog;
    FileDialog = new GUIFILEDIALOG ( FD_OPEN );
    FileDialog->CreateExtensions ( 3 );
    FileDialog->SetExtension ( 0, "*.FLI", "Autodesk FLI (*.FLI)" );
    FileDialog->SetExtension ( 1, "*.FLC", "Autodesk FLC (*.FLC)" );
    FileDialog->SetExtension ( 2, "*.*", "All files (*.*)" );

    LONG Result;    
    Result = FileDialog->Execute ( this, "File Open" );
    if (Result)
      {
        CHAR FileName[128];
        FileDialog->GetFileName ( FileName );

        G3DMATERIALLIB *MaterialLib = G3D->GetMaterialLib ();
        
        
        if (MaterialLib->LoadTexture ( 0, FileName, TEXTURE_FLIC, Pal )==SUCCESS)
          {
            AnimatedTexture = TRUE;    
            NumAnimatedFrames = (MaterialLib->GetTexture(0))->GetNumFrames();
            CurAnimatedFrame = 0;
          } 
        else  
          {
            printf ( "Can not load this Texture!" );
          } 
      } 
      
    if (TimerOn)
      SetTimer ( 0 );  
    delete FileDialog;      
#endif 
  } 

VOID MAINFRAME::OnAmbient ()
  {
    AMBIENTDLG *AmbientDlg;

    if (TimerOn)
      KillTimer ();
#if 0      
    AmbientDlg = new AMBIENTDLG ();
    if (AmbientDlg->Execute ( this, "AMBIENT_DLG" ))
      {
        RGBCOLOR Col;
        AmbientDlg->GetRGB ( &Col );
        G3DMATERIAL *Material = G3D->FindMaterialByID ( 0 );
        
        Material->Ambient = Pal->GetClosestColor ( Col );  
      } 
      
    if (TimerOn)
      SetTimer (0);
      
    delete AmbientDlg;
#endif 
  } 

VOID MAINFRAME::OnTransparent ( LONG Code )
  {
    LONG Transparency;
    G3DMATERIAL *Material = G3D->FindMaterialByID ( 0 );

    if (Material==NULL)
      return;
      
    if (Code==MENU_MATERIAL_TRANSPARENCY_00)
      {
        Material->Attributes &= ~MATERIAL_TRANSPARENT;
      } 
    else
      {
        Transparency = Code - MENU_MATERIAL_TRANSPARENCY;

        Material->Attributes |= MATERIAL_TRANSPARENT;
        Material->Transparency = Transparency;  
      } 
  } 

VOID MAINFRAME::OnFog ()
  {
    FOGDLG *FogDlg;

    if (TimerOn)
      KillTimer ();

#if 0      
    FogDlg = new FOGDLG ();
    if (FogDlg->Execute ( this, "FOG_DLG" ))
      {
        G3D->SetHazing ( FogDlg->IsFogOn (), FogDlg->GetFogScale(), HazeTable );
        RGBCOLOR Col;
        Col.Red = 255;
        Col.Green = 255;
        Col.Blue = 255;
        BGColor = Pal->GetClosestColor ( Col );
        if (FogDlg->IsFogOn())
          UseBG = FALSE;
        else
          {
            if (BGImage!=NULL)
              UseBG = TRUE;
            else
              {
                RGBCOLOR Col;
                Col.Red = (BYTE)BGRed;
                Col.Green = (BYTE)BGGreen;
                Col.Blue = (BYTE)BGBlue;
                BGColor = Pal->GetClosestColor ( Col );  
              } 
          } 
      } 
      
    if (TimerOn)
      SetTimer (0);
      
    delete FogDlg;
#endif 
  } 

VOID MAINFRAME::OnLoadBG ()
  {
    if (TimerOn)
      KillTimer ();

#if 1
    if (BGImage==NULL)
      BGImage = new IMAGE ( IMAGE_TOPDOWN );
          
    GUIFILEDIALOG *FileDialog;
    FileDialog = new GUIFILEDIALOG ( FD_OPEN );
    FileDialog->CreateExtensions ( 4 );
    FileDialog->SetExtension ( 0, "*.BMP", "Windows BMP (*.BMP)" );
    FileDialog->SetExtension ( 1, "*.GIF", "CompuServ GIF (*.GIF)" );
    FileDialog->SetExtension ( 2, "*.PCX", "Z Soft PCX (*.PCX)" );
    FileDialog->SetExtension ( 3, "*.*", "All files (*.*)" );

    LONG Result;    
    Result = FileDialog->Execute ( this, "File Open" );
    if (Result)
      {
        CHAR FileName[128];
        FileDialog->GetFileName ( FileName );

        
        RGBPALETTE *NewPal = new RGBPALETTE ();
        if (Grafix.LoadImage ( FileName, BGImage, NewPal )==SUCCESS)
          {
            UseBG = TRUE;  
            HDISPLAY hDisplay = GetDisplay ( hClient );  
            Grafix.AdjustImageSize ( BGImage, Grafix.GetWidth(hDisplay), Grafix.GetHeight(hDisplay) );                                   
            ReleaseDisplay ( hClient, hDisplay );

            if (Pal->IsIdentical(NewPal)!=TRUE)
              {
                COLORTABLE *MatchTable = new COLORTABLE ();
                MatchTable->CopyPalette ( Pal );            
                MatchTable->CreateMatchTable ( NewPal );
                Grafix.ConvertImage ( BGImage, MatchTable );
                delete MatchTable;
              } 
          } 
        else  
          {
            printf (  "Can not load this Image!" );
          } 
        delete NewPal;  
      } 

    if (TimerOn)
      SetTimer ( 0 );
    delete FileDialog;      
#endif 
  } 

VOID MAINFRAME::OnBGStatic ()
  {
    BGCOLORDLG *BGColorDlg;

    if (TimerOn)
      KillTimer ();
#if 0      
    BGColorDlg = new BGCOLORDLG ();
    if (BGColorDlg->Execute ( this, "BGRGB_DLG" ))
      {
        if (BGImage!=NULL)
          delete BGImage;
        BGImage = NULL;
        UseBG = FALSE;
        RGBCOLOR Col;
        BGColorDlg->GetRGB ( &Col );
        BGRed = Col.Red;
        BGGreen = Col.Green;
        BGBlue = Col.Blue;
        BGColor = Pal->GetClosestColor ( Col );  
      } 
      
    if (TimerOn)
      SetTimer (0);
      
    delete BGColorDlg;
#endif 
  } 

VOID MAINFRAME::OnDepthCue ()
  {
    DEPTHDLG *DepthDlg;
#if 0
    if (TimerOn)
      KillTimer ();
      
    DepthDlg = new DEPTHDLG ();
    if (DepthDlg->Execute ( this, "DEPTH_DLG" ))
      {
        G3D->SetDepthCueing ( DepthDlg->IsDepthCueOn (), DepthDlg->GetDepthCueScale() );  
      } 
      
    if (TimerOn)
      SetTimer (0);
      
    delete DepthDlg;
#endif 
  } 
  
VOID MAINFRAME::OnRotation ( LONG Code )
  {
    switch (Code)
      {
        case MENU_ROTATION_X_INC :
          ObjRotX += (float)1.0;
          break;  
        case MENU_ROTATION_X_DEC :
          ObjRotX -= (float)1.0;
          break;  
        case MENU_ROTATION_X_STOP :
          ObjRotX = (float)0;
          break;
          
        case MENU_ROTATION_Y_INC :
          ObjRotY += (float)1.0;
          break;          
        case MENU_ROTATION_Y_DEC :
          ObjRotY -= (float)1.0;
          break;  
        case MENU_ROTATION_Y_STOP :
          ObjRotY = (float)0;
          break;
          
        case MENU_ROTATION_Z_INC :
          ObjRotZ += (float)1.0;
          break;  
        case MENU_ROTATION_Z_DEC :
          ObjRotZ -= (float)1.0;
          break;  
        case MENU_ROTATION_Z_STOP :
          ObjRotZ = (float)0;
          break;  
      } 
  } 

VOID MAINFRAME::OnAbout ()
  {
    if (TimerOn)
      KillTimer ();

  } 

LONG MAINFRAME::OnCreate ()
  {
    Menu->Load ( "MAIN_MENU" );
    SetMenu ( Menu );

    if (Init3D()!=SUCCESS)
      {
        printf("Error occured at initialization");
        ExitApplication (-1);
      } 
    return 0;  
  } 

LONG MAINFRAME::OnCommand ( LONG Command )
  {
    switch (Command)
      {
        case MENU_FILE_OPEN :
          OnFileLoad ();
          break;  
        case MENU_FILE_EXIT :

          break;

        case MENU_DETAIL_FACE_TEXTURE :
        case MENU_DETAIL_FACE_SOLID :
        case MENU_DETAIL_FACE_WIREFRAME :
        case MENU_DETAIL_SHADE_GOURAUD :
        case MENU_DETAIL_SHADE_FLAT :
        case MENU_DETAIL_SHADE_NONE :
          OnDetail ( Command );
          break;
                    
        case MENU_MATERIAL_LOAD_STATIC :
          OnLoadTextureStatic ();
          break;  
        case MENU_MATERIAL_LOAD_ANIMATED :          
          OnLoadTextureAnimated ();
          break;
          
        case MENU_MATERIAL_TRANSPARENCY_10 :          
        case MENU_MATERIAL_TRANSPARENCY_20 :          
        case MENU_MATERIAL_TRANSPARENCY_30 :          
        case MENU_MATERIAL_TRANSPARENCY_40 :          
        case MENU_MATERIAL_TRANSPARENCY_50 :          
        case MENU_MATERIAL_TRANSPARENCY_60 :          
        case MENU_MATERIAL_TRANSPARENCY_70 :          
        case MENU_MATERIAL_TRANSPARENCY_80 :          
        case MENU_MATERIAL_TRANSPARENCY_90 :          
        case MENU_MATERIAL_TRANSPARENCY_00 :
          OnTransparent ( Command );
          break;

        case MENU_MATERIAL_AMBIENT :
          OnAmbient ();
          break;
          
        case MENU_ENVIRONMENT_BACKGROUND_LOAD :
          OnLoadBG ();
          break;  
        case MENU_ENVIRONMENT_BACKGROUND_STATIC :
          OnBGStatic ();
          break;  
        case MENU_ENVIRONMENT_DEPTH_CUE :
          OnDepthCue ();
          break;  
        case MENU_ENVIRONMENT_FOG :
          OnFog ();
          break;

        case MENU_ROTATION_X_INC :
        case MENU_ROTATION_X_DEC :
        case MENU_ROTATION_X_STOP :
        case MENU_ROTATION_Y_INC :
        case MENU_ROTATION_Y_DEC :
        case MENU_ROTATION_Y_STOP :
        case MENU_ROTATION_Z_INC :
        case MENU_ROTATION_Z_DEC :
        case MENU_ROTATION_Z_STOP :
          OnRotation ( Command );
          break;
        case MENU_HELP_ABOUT :
          OnAbout ();
          break;  
      } 
    return 0;  
  } 

LONG MAINFRAME::OnClose ()
  {
    if (TimerOn)      
      KillTimer ();  
  
    printf("Exit this demo?\n");
      
    if (TimerOn)
      SetTimer (0);
      
    return SKIPDEFPROC;  
  } 

VOID MAINFRAME::HandleInput ()
  {
    BOOLEAN IsShift;
    IsShift = Input.IsKeyDown(SC_LSHIFT) || Input.IsKeyDown(SC_RSHIFT);
    
    float SpeedRatio = (float)1.0;
    if (IsShift)
      {
        SpeedRatio = (float)2.0;  
      } 
    
    COLLIDEDATA CollideList[5];
    FLPVECTOR3D Position,EndPt;
    FLPVECTOR3D Dir;

    Position = Camera->GetWorldPosition();
    
    if (Input.IsKeyDown(SC_RIGHT))
      {
        Camera->Rotate ( (float)0, (float)(-3*SpeedRatio), (float)0 );  
      } 
    if (Input.IsKeyDown(SC_LEFT))
      {
        Camera->Rotate ( (float)0, (float)(3*SpeedRatio), (float)0 );  
      } 
    if (Input.IsKeyDown(SC_UP))
      {
        Camera->Rotate ( (float)(-3*SpeedRatio), (float)0, (float)0 );  
      } 
    if (Input.IsKeyDown(SC_DOWN))
      {
        Camera->Rotate ( (float)(3*SpeedRatio), (float)0, (float)0 );  
      } 

    LONG CollisionCount;
      
    if (Input.IsKeyDown(SC_A))
      {
        Dir.x = (float)0;
        Dir.y = (float)0;
        Dir.z = (float)(1*20);

        EndPt = G3D->ComputeNextPos ( Position, Camera->GetAngle(), Dir );
        CollisionCount = G3D->CheckCollision ( Position, EndPt, CollideList, (LONG)5, 
                                                                             (float)1.6, (float)10 );
        if (CollisionCount==0)
          Camera->Move ( EndPt.x-Position.x, EndPt.y-Position.y, EndPt.z-Position.z );
      } 
    if (Input.IsKeyDown(SC_Z))
      {
        Dir.x = (float)0;
        Dir.y = (float)0;
        Dir.z = (float)(-1*20);

        EndPt = G3D->ComputeNextPos ( Position, Camera->GetAngle(), Dir );  
        CollisionCount = G3D->CheckCollision ( Position, EndPt, CollideList, (LONG)5, 
                                                                             (float)1.6, (float)10 );
        if (CollisionCount==0)
          Camera->Move ( EndPt.x-Position.x, EndPt.y-Position.y, EndPt.z-Position.z );
      } 
  } 

LONG MAINFRAME::OnTimer ()
  {
    if (ObjectLoaded)
      {
        G3DMATERIALLIB *MaterialLib = G3D->GetMaterialLib();
        ANIMIMAGE *Texture = MaterialLib->GetTexture ( 0 );

        if ((BGImage!=NULL)&&(UseBG))
          {
            Grafix.CopyImage ( BGImage, 0, 0, BGImage->GetWidth(), BGImage->GetHeight(),
                               VScreen, 0, 0 );
          } 
        else
          {
            VScreen->Clear ( BGColor );
          } 
          
        G3DOBJECT *Object = G3D->FindObjectByID ( 1 );
        if (Object!=NULL)
          {
            Object->Rotate ( ObjRotX, ObjRotY, ObjRotZ );
            if (AnimatedTexture)
              {
                Texture->SetFrame ( CurAnimatedFrame );  
                CurAnimatedFrame++;
                if (CurAnimatedFrame>=NumAnimatedFrames)
                  CurAnimatedFrame = 0;
              } 
          } 
        HandleInput ();  
        G3D->ShowView ( Camera );
        HDISPLAY hDisplay = GetDisplay ( hClient );
        Grafix.DisplayImage ( hDisplay, VScreen, 0, 0, VScreen->GetWidth(), VScreen->GetHeight(),
                              0, 0 );
        ReleaseDisplay ( hClient, hDisplay ); 
      } 
    return 0;  
  } 

LONG MAINFRAME::OnPosChanging ( WINPOSCHANGE *WinPosChange )
  {
    LONG Wd;
    Wd = WinPosChange->Width;
    Wd = ((Wd+3)/4)*4;
    WinPosChange->Width = Wd;  
    return 0;  
  } 

LONG MAINFRAME::OnSize ( LONG Width, LONG Height )
  {
    if (Width&&Height)
      {}  
    if (Initialized)
      {
        HDISPLAY hDisplay = GetDisplay ( hClient );
        Grafix.SetPalette ( hDisplay, Pal );  
        VScreen->Create ( hDisplay, IMAGE_8BIT,
                          Grafix.GetWidth(hDisplay), Grafix.GetHeight(hDisplay) );
        G3D->SetDestination ( VScreen );
        G3D->SetScreenCenter ( VScreen->GetWidth()/2, VScreen->GetHeight()/2 );
        if (BGImage!=NULL)
          {
            Grafix.AdjustImageSize ( BGImage, Grafix.GetWidth(hDisplay), Grafix.GetHeight(hDisplay) );                                   
          } 
        ReleaseDisplay ( hClient, hDisplay ); 
      } 
    return 0;  
  } 

LONG MAINFRAME::OnPaint ( HDISPLAY hDisplay )
  {
    Grafix.SetPalette ( hDisplay, Pal );  
    return 0;      
  } 







int main (int argc, char * * argv ) {

    MAINFRAME *MainFrame = new MAINFRAME ();
     MainFrame->Create ( "Multi-Platform 3D Demo", WINPOS_DEFAULT, WINPOS_DEFAULT, WINPOS_DEFAULT, WINPOS_DEFAULT, GUI_DESKTOP );
    DISPLAYDATA Data;
    Data.UseDirectDraw = FALSE;
    Data.hMainWindow = MainFrame->GetHandle ();

    GUIAPP DemoApp;
    DemoApp.Run ( &Data, MainFrame );

    return 0;

}
  
  
