




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
