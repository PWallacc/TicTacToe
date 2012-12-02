#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

using namespace std;

template <class T>
class LinkList
{
   private:
      T           l_data;
      LinkList<T> *next;

   public:
      LinkList();
      LinkList(T data);
      ~LinkList();
   
      T           getData();
      LinkList<T> *getNext();

      void setData(T data);
   
      void display();
      void insert(T data);
      bool isExist(T data);
      void remove(T data);

      bool operator<(T rhs);
      friend bool operator<(T lhs, LinkList<T> rhs);

      bool operator>(T rhs);
      friend bool operator>(T lhs, LinkList<T> rhs);

      bool operator==(T rhs);
      friend bool operator==(T lhs, LinkList<T> rhs);

      bool operator!=(T rhs);
      friend bool operator!=(T lhs, LinkList<T> rhs);

      friend ostream& operator<<(ostream &outputStream, LinkList<T> obj);
};


template <class T>
LinkList<T>::LinkList()
{
   next = NULL;
}


template <class T>
LinkList<T>::LinkList(T data)
{
   l_data = data;
   next = NULL;
}


template <class T>
LinkList<T>::~LinkList()
{
   LinkList<T> *tmp;

   tmp = next;
   while (tmp != NULL)
   {
      next = tmp->next;
      tmp->next = NULL;
      delete tmp;
      tmp = next;
   }
}


template <class T>
T LinkList<T>::getData()
{
   return l_data;
}


template <class T>
LinkList<T> *LinkList<T>::getNext()
{
   return next;
}


template <class T>
void LinkList<T>::setData(T data)
{
   l_data = data;
}

   
template <class T>
void LinkList<T>::display()
{
   LinkList<T> *tmp;

   tmp = next;
   while (tmp != NULL)
   {
      cout << tmp->l_data << endl;
      tmp = tmp->next;
   }
}


template <class T>
void LinkList<T>::insert(T data)
{
   LinkList<T> *newNode, *tmp;

   newNode = new LinkList(data);

   if (next == NULL)
      next = newNode;
   else
   {
      tmp = next;

      while (tmp->next != NULL)
         tmp = tmp->next;
   
      tmp->next = newNode;
   }
}


template <class T>
bool LinkList<T>::isExist(T data)
{
   bool        found = false;
   LinkList<T> *tmp;

   tmp = next;
   while (tmp != NULL)
   {
      if (tmp->l_data == data)
      {
         found = true;
         break;
      }

      tmp = tmp->next;
   }

   return found;
}


template <class T>
void LinkList<T>::remove(T data)
{
   LinkList<T> *tmp, *oneBefore;

   if (!isExist(data))
      return;

   tmp = next;
   oneBefore = next;
   while (tmp != data)
   {
      oneBefore = tmp;
      tmp = tmp->next;
   }

   if (tmp == next)
   {
      next = tmp->next;
      tmp->next = NULL;
      delete tmp;
   }
   else
   {
      if (tmp->next == NULL)
      {
         oneBefore->next = NULL;

         delete tmp;
      }
      else
      {
         oneBefore->next = tmp->next;
         tmp->next = NULL;
         delete tmp;
      }
   }
}


template <class T>
bool LinkList<T>::operator<(T rhs)
{
   return l_data < rhs;
}


template <class T>
bool operator<(T lhs, LinkList<T> rhs)
{
   return lhs < rhs.l_data;
}


template <class T>
bool LinkList<T>::operator>(T rhs)
{
   return l_data > rhs;
}


template <class T>
bool operator>(T lhs, LinkList<T> rhs)
{
   return lhs > rhs.l_data;
}


template <class T>
bool LinkList<T>::operator==(T rhs)
{
   return l_data == rhs;
}


template <class T>
bool operator==(T lhs, LinkList<T> rhs)
{
   return lhs == rhs.l_data;
}


template <class T>
bool LinkList<T>::operator!=(T rhs)
{
   return l_data != rhs;
}


template <class T>
bool operator!=(T lhs, LinkList<T> rhs)
{
   return lhs != rhs.l_data;
}


template <class T>
ostream& operator<<(ostream &outputStream, LinkList<T> obj)
{
   outputStream << obj.l_data;

   return outputStream;
}

#endif