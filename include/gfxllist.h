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


#ifndef __GFXLLIST_H__
#define __GFXLLIST_H__

#include "stdgfx.h"
#include "gfxadst.h"

template <class Type>
class LISTOBJECT : public DATAOBJECT
  {
    public :
      LISTOBJECT<Type> *PrevObject;
      LISTOBJECT<Type> *NextObject;
      Type Data;

      LISTOBJECT () : DATAOBJECT ()
        {
          PrevObject = NextObject = NULL;
        }; 

      virtual ~LISTOBJECT ()
        {
          if (NextObject!=NULL)
            delete NextObject;
          NextObject = PrevObject = NULL;
        }; 
  }; 

template <class Type>
class LINKEDLIST : public ADST
  {
    protected :
      LISTOBJECT<Type> *Head;
      LISTOBJECT<Type> *Tail;

    public :
      LINKEDLIST () : ADST ()
        {
          Head = Tail = NULL;
        }; 

      virtual ~LINKEDLIST ()
        {
          while (Head!=NULL)
            {
              RemoveNode ( Head );
            } 
          Head = Tail = NULL;
        }; 

      LISTOBJECT<Type>* GetHead () { return Head; };
      LISTOBJECT<Type>* GetTail () { return Tail; };

      BOOLEAN AddObject ( Type Data )
        {
          LISTOBJECT<Type> *NewNode;

          NewNode = new LISTOBJECT<Type> ();
          if (NewNode==NULL)
            return FALSE;

          NewNode->Data = Data;

          
          if (Head==NULL) 
            {
              Head = NewNode;
              Tail = Head;
              NumObjects = 1;
            } 
          else
            {
              Tail->NextObject = NewNode;
              NewNode->PrevObject = Tail;
              Tail = NewNode;
              NumObjects++;
            } 
          return TRUE;
        }; 

      VOID DiscardNode ( LISTOBJECT<Type> *Node )
        {
          if (Node==NULL)
            return;
          if (Node->PrevObject==NULL) 
            {
              Head = Node->NextObject;
              if (Head==NULL)
                Tail = NULL;
              else
                Head->PrevObject = NULL;
            } 
          else
            {
              Node->PrevObject->NextObject = Node->NextObject;
              if (Node->NextObject==NULL) 
                {
                  Tail = Node->PrevObject;
                } 
              else
                {
                  Node->NextObject->PrevObject =
                                      Node->PrevObject;
                } 
            } 
          Node->NextObject = NULL;
          Node->PrevObject = NULL;
          NumObjects--;
        }; 

      VOID RemoveNode ( LISTOBJECT<Type>* Node )
        {
          if (Node==NULL)
            return;
          DiscardNode ( Node );
          delete Node;
        }; 

      VOID ClearAllNode ()
        {
          while (Head!=NULL)
            {
              if (Head->Data!=NULL)
                delete Head->Data;
              Head->Data = NULL;
              RemoveNode ( Head );  
            } 
          Head = Tail = NULL;  
        }; 
  }; 

#endif
