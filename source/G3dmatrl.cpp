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
#include "g3dmatrl.h"







G3DMATERIAL::G3DMATERIAL ()
  {
    Texture = NULL;
    Ambient = 0;
    Transparency = 0;
    Attributes = 0;
    Name[0] = 0;
    ID = 0;
  } 

G3DMATERIAL::~G3DMATERIAL ()
  {
  } 
  
VOID G3DMATERIAL::SetTexture ( ANIMIMAGE *Image )
  {
    Texture = Image;
  } 

VOID G3DMATERIAL::SetName ( STRING NewName )
  {
    strcpy ( Name, NewName );  
  } 
  
VOID G3DMATERIAL::SetID ( LONG NewID )
  {
    ID = NewID;  
  } 
  







G3DMATERIALLIB::G3DMATERIALLIB ()
  {
    Materials = NULL;
    NumMaterials = 0;
    Textures = NULL;
    NumTextures = 0;
  } 

G3DMATERIALLIB::~G3DMATERIALLIB ()
  {
    DestroyMaterials ();
    DestroyTextures ();
  } 
      
VOID G3DMATERIALLIB::CreateMaterials ( LONG Num )
  {
    DestroyMaterials ();
    
    Materials = new G3DMATERIAL* [Num];
    if (Materials==NULL)
      return;
      
    LONG i;
    
    NumMaterials = Num;
    for (i=0;i<NumMaterials;i++)
      {
        Materials[i] = new G3DMATERIAL ();
      } 
  } 

VOID G3DMATERIALLIB::DestroyMaterials ()
  {
    LONG i;
    if (Materials!=NULL)
      {
        for (i=0;i<NumMaterials;i++)
          {
            if (Materials[i]!=NULL)
              delete Materials[i];
          } 
        delete Materials;
      } 
    NumMaterials = 0;
    Materials = NULL;        
  } 

VOID G3DMATERIALLIB::CreateTextures ( LONG Num )
  {
    DestroyTextures ();
    
    Textures = new ANIMIMAGE* [Num];
    if (Textures==NULL)
      return;
      
    LONG i;
    
    NumTextures = Num;
    for (i=0;i<NumTextures;i++)
      {
        Textures[i] = new ANIMIMAGE ();
      } 
  } 

VOID G3DMATERIALLIB::DestroyTextures ()
  {
    LONG i;
    if (Textures!=NULL)
      {
        for (i=0;i<NumTextures;i++)
          {
            if (Textures[i]!=NULL)
              delete Textures[i];
          } 
        delete Textures;
      } 
    NumTextures = 0;
    Textures = NULL;        
  } 

G3DMATERIAL* G3DMATERIALLIB::FindMaterialByName ( STRING SearchName )
  {
    LONG i;
    
    for (i=0;i<NumMaterials;i++)
      {
        if (strcmp(SearchName,Materials[i]->GetName())==0)
          return Materials[i];           
      } 
    return NULL;  
  } 

G3DMATERIAL* G3DMATERIALLIB::FindMaterialByID ( LONG SearchID )
  {
    LONG i;
    
    for (i=0;i<NumMaterials;i++)
      {
        if (SearchID==Materials[i]->GetID())
          return Materials[i];           
      } 
    return NULL;  
  } 

BOOLEAN G3DMATERIALLIB::LoadTexture ( LONG Index, STRING FileName, BOOLEAN FileType,
                                      RGBPALETTE *Pal )
  {
    if ((Index<0)||(Index>=NumTextures))
      return FAILURE;

    BOOLEAN Result = FAILURE;

    ANIMIMAGE *Texture = GetTexture ( Index );
    Texture->Create ( IMAGE_8BIT, 256, 256 );
    
    RGBPALETTE *LoadedPal = NULL;
    COLORTABLE *MatchTable = NULL;
    FLICFILE *Flic = NULL;
    
    switch (FileType)
      {
        case TEXTURE_STATIC :
          LoadedPal = new RGBPALETTE ();
          Result = Grafix.LoadImage ( FileName, Texture, LoadedPal );
          break;  
        case TEXTURE_FLIC :
          Flic = new FLICFILE ();
          Result = Flic->Load ( FileName );
          Flic->GetFirstPalette ();
          LoadedPal = Flic->GetPalette ();
          break;  
        case TEXTURE_ANIM :
          break;  
      } 

    LONG i;
    if (Result==SUCCESS)
      {
        if (Pal!=NULL)
          {
            if (Pal->IsIdentical(LoadedPal)==FALSE)
              {
                if (FileType==TEXTURE_STATIC)
                  {
                    MatchTable= new COLORTABLE ();  
                    MatchTable->CopyPalette ( Pal );  
                    MatchTable->CreateMatchTable ( LoadedPal );
                    Grafix.AdjustImageSize ( Texture, 256, 256 );
                    Grafix.ConvertImage ( Texture, MatchTable );
                  } 
                else if (FileType==TEXTURE_FLIC)
                  {
                    Flic->Convert ( Pal );  
                    Flic->SetFrame ( 0 );
                    Texture->CreateBaseImage ();
                    Texture->CreateFrames ( Flic->GetNumFrames() );
                    Grafix.SetScaleFactor ( 256, Flic->GetImageWidth(),
                                            256, Flic->GetImageHeight() );                                            
                    for (i=0;i<Flic->GetNumFrames();i++)
                      {
                        Flic->PlayFrame ( TRUE );
                        Grafix.ScaleImage  ( Flic->GetImage (), 0, 0,                        
                                             Flic->GetImageWidth(), Flic->GetImageHeight(),
                                             Texture, 0, 0 );
                        Texture->RecordFrame ( i, Texture, 0, 0 );
                      } 
                    Grafix.SetScaleFactor ( 1, 1, 1, 1 );
                  } 
              } 
          } 
      } 

    if (Flic!=NULL)
      {
        delete Flic;
        LoadedPal = NULL;
      } 

    if (LoadedPal!=NULL)  
      delete LoadedPal;

    if (MatchTable!=NULL)        
      delete MatchTable;

    return Result;  
  } 
