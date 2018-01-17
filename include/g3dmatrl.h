




#ifndef __G3DMATERIAL_H__
#define __G3DMATERIAL_H__

#include "stdgfx.h"
#include "g3ddef.h"

#define MATERIAL_SPECULAR     ((LONG)1<<0)
#define MATERIAL_REFRACTIVE   ((LONG)1<<1)
#define MATERIAL_TRANSPARENT  ((LONG)1<<2)
#define MATERIAL_TRANSLUENT   ((LONG)1<<3)

#define TEXTURE_STATIC     1
#define TEXTURE_FLIC       2
#define TEXTURE_ANIM       3

class G3DMATERIAL
  {
    protected :  
      CHAR Name[32];
      LONG ID;
      ANIMIMAGE *Texture;
      
    public :
      LONG Ambient;
      BOOLEAN Animated;
      LONG Attributes;
      LONG Transparency;
      
      G3DMATERIAL ();
      virtual ~G3DMATERIAL ();       
      
      VOID SetTexture ( ANIMIMAGE *Image );
      ANIMIMAGE* GetTexture () { return Texture; };
      
      VOID SetName ( STRING NewName );
      VOID SetID ( LONG NewID );

      STRING GetName () { return Name; };
      LONG GetID () { return ID; };
  }; 

class G3DMATERIALLIB
  {
    protected :
      LONG NumMaterials;
      G3DMATERIAL **Materials;
      LONG NumTextures;
      ANIMIMAGE **Textures;
    
    public :
      G3DMATERIALLIB ();
      virtual ~G3DMATERIALLIB ();
      
      VOID CreateMaterials ( LONG Num );
      VOID DestroyMaterials ();
      G3DMATERIAL* GetMaterial ( LONG Index ) { return Materials[Index]; };      

      LONG GetNumMaterials () { return NumMaterials; };
      LONG GetNumTextures () { return NumTextures; };
            
      VOID CreateTextures ( LONG Num );
      VOID DestroyTextures ();
      ANIMIMAGE* GetTexture ( LONG Index ) { return Textures[Index]; };
      BOOLEAN LoadTexture ( LONG Index, STRING FileName, BOOLEAN FileType,
                            RGBPALETTE *Pal );
            
      G3DMATERIAL* FindMaterialByName ( STRING SearchName );
      G3DMATERIAL* FindMaterialByID ( LONG SearchID );
  }; 

#endif


