//file       ABLTree.h
//brief      реализация классов,поворотов ,добавления,удаления и поиска в АВЛ-дереве
//author     Будаковский Дмитрий
//date       27.11.2017
//version    2.0
//std        1.0


#include <iostream>
#include <sstream>
#include <queue>
#include <iomanip>
#include "IDictionary.h"
#include "ISortedSequence.h"
using namespace std;

#pragma once

template <typename K, typename V> class TNode
{
public:
  TNode() : p_right(nullptr), p_left(nullptr)
  {
  }

  TNode(const K& key, const V& val) : m_key(key), p_value(new V(val)), p_right(nullptr), p_left(nullptr), m_right_height(0), m_left_height(0)
  {
  }

  virtual ~TNode()
  {
    p_right = nullptr;
    p_left = nullptr;
    if (p_value != nullptr)
      delete p_value;
  }

  K getKey()
  {
    return m_key;
  }

  V getValue()
  {
    return *p_value;
  }

  TNode* getRight()
  {
    return p_right;
  }

  TNode* getLeft()
  {
    return p_left;
  }

  int getRightHeight()
  {
    return m_right_height;
  }

  int getLeftHeight()
  {
    return m_left_height;
  }

  void setKey(const K& k)
  {
    m_key = k;
  }

  void setValue(const V& v)
  {
    *p_value = v;
  }

  void setRight(TNode* pR)
  {
    p_right=pR;
  }

  void setLeft(TNode* pL)
  {
    p_left = pL;
  }

  void setRightHeight(int h)
  {
    m_right_height = h;
  }

  void setLeftHeight(int h)
  {
    m_left_height = h;
  }

private:

  K m_key;
  V* p_value;
  TNode* p_right; // указатель направо
  TNode* p_left;  // указатель налево
  int m_right_height;      //высота правого поддерева
  int m_left_height;      //высота левого поддерева
};


template <typename K, typename V> class AVLTree : public IDictionary<K, V>, public ISortedSequence<V>
{
private:
  unsigned int n_elements;// колличество элементов в дереве
  TNode<K, V>* p_root_tree;// корень дерева
public:

  AVLTree() : p_root_tree(nullptr), n_elements(0)
  {
  }

  ~AVLTree()
  {
    clear();
  }

  DictionaryType getType() const
  {
    return DictionaryType::AVL;
  }


  void add( const K& key, const V& value ) //добавление вершины v 
  {
    if ( p_root_tree == nullptr ) // в случае если дерево незаполнено
    {
      TNode<K, V>* pNew = new TNode<K, V>( key, value); 
      p_root_tree = pNew;
    }
    else
    {
      int flag = 0; // показывает добавили ли мы элемент ( flag=1) или нет( flag=0)
      TNode<K, V>** array;
      int depth;// глубина поддерева
      if( key >= p_root_tree->getKey() ) //определяем максимальную глубину поддерева
      {
        depth = p_root_tree->getRightHeight() + 1;
      }
      else
      {
        depth  = p_root_tree->getLeftHeight() + 1;
      }
      array = new TNode<K, V>*[depth];// вспомогательный массив для расстановки "баланса" и для балансировки

      int j(0);
      for(TNode<K, V>* i = p_root_tree ; ( ( i != nullptr ) && ( flag == 0 ) ) ; j++ )
      {
        if( j >= (int)n_elements )
        {
          cout << "Ошибка появляются лишние вершины\n";
        }
        array[j] = i;// те вершины,которые мы проходим,записываем в наш массив чтобы по ним вернуться к корню,на пути рассавляя высоты
        if( key >= i->getKey() )//идём направо
        {
          i = i->getRight();
          if (i == nullptr)// записываем новую вершину
          {
            TNode<K, V>* pNew = new TNode<K, V>( key, value);
            array[j]->setRight( pNew );
            array[j]->setRightHeight( array[j]->getRightHeight() + 1 );
            flag = 1;
          }
        } 
        else if( key < i->getKey() )
        {

          i = i->getLeft();
          if (i == nullptr)// записываем новую вершину
          {
            TNode<K, V>* pNew = new TNode<K, V>( key, value);
            array[j]->setLeft( pNew );
            array[j]->setLeftHeight( array[j]->getLeftHeight() + 1 );
            flag = 1;
          }
        }
      }
      j--;// ? иначе в цикле выходим за границы массива arrey
      if( ( array[j]->getRight() == nullptr ) || ( array[j]->getLeft() == nullptr ) )
      {
        j--;
        int flag = 0;
        for( ; j >= 0 ; j--)//в цикле поднимаемся по дереву от добавленной вершины , по пути балансируя его
        {
          if( ( array[j]->getLeft() == array[j+1] ) && ( array[j+1]->getRightHeight() != array[j+1]->getLeftHeight() ) && flag == 0 )// если flag == 1 ,то перебалансировка была произведена
          {
            TNode<K, V>* k;
            k = fBalance( &array[j+1] );
            if( k )//если поворот был
            {
              array[j]->setLeft( k );
              int max = k->getRightHeight();
              if( max < k->getLeftHeight() )
              {
                max = k->getLeftHeight();
              }
              flag = 1;
              array[j]->setLeftHeight( max + 1 );
            }
            else if ( array[j+1]->getRightHeight() != array[j+1]->getLeftHeight() )// если глубины поддеревьев не совпадают изменяем высоты поддеревьев
            {
              int max = array[j+1]->getRightHeight();
              if( max < array[j+1]->getLeftHeight() )
              {
                max = array[j+1]->getLeftHeight();
              }
              array[j]->setLeftHeight( max + 1 );
            }
          }
          if( ( array[j]->getRight() == array[j+1] ) && ( array[j+1]->getRightHeight() != array[j+1]->getLeftHeight() ) && flag == 0 )//аналогично для правого поддерева
          {
            TNode<K, V>* k;
            k = fBalance( &array[j+1] );
            if( k )
            {
              array[j]->setRight( k );
              int max = k->getRightHeight();
              if( max < k->getLeftHeight() )
              {
                max = k->getLeftHeight();
              }
              flag = 1;
              array[j]->setRightHeight( max + 1 );
            }
            else if( array[j+1]->getRightHeight() != array[j+1]->getLeftHeight())
            {
              int max = array[j+1]->getRightHeight();
              if( max < array[j+1]->getLeftHeight() )
              {
                max = array[j+1]->getLeftHeight();
              }
              array[j]->setRightHeight( max + 1 );
            }
          }
        }
      }
      //for(int k=0;k<pN;k++)
      //{
      //	arrey[k]=0;
      //}
      delete [] array;// удаляем массив освобождаем память
    }
    fBalance(&p_root_tree);//балансируем вершину ,если нужно
    n_elements++;// увеличиваем колличество вершин
  }

  void addValue( const V& value )
  {
    add( value, value );
  }

  //получить кол-во элементов, которое физически хранится 
  unsigned int getLength() const
  {
    return n_elements;
  }

  unsigned int getCount() const
  {
    return n_elements;
  }

  unsigned int getCapacity()  const
  {
    return n_elements;
  }

  bool isEmpty() const
  {
    return (n_elements == 0);
  }

  bool fDelete(const K& key)
  {
    bool res(false);
    std::vector<std::pair<K, V>> buff = getValues();
    bool isFind(false);
    for (std::vector<std::pair<K, V>>::iterator it = buff.begin(); it != buff.end() && !isFind; )
    {
      if (it->first == key)
      {
        it = buff.erase(it);
        isFind = true;
      }
      else
        it++;
    }
      res = isFind;
      clear();
      addVec(buff);
//     unsigned int inx(0);
//     K* keys = new K[n_elements];
//     V* mass = new V[n_elements];
//     recPreFullOrderN(p_root_tree, keys, mass, inx, n_elements);
//     for (unsigned int i(0); i < n_elements; i++)
//       if (keys[i] != key)
//         buff.push_back(std::pair<K, V>(keys[i], mass[i]));
//       else
//         res = true;
// 
//     clear();
//     addVec(buff);
// 
//     delete [] keys;
//     delete [] mass;

    return res;
  }

  //удалить элемент с ключём key
  bool erase(const K& key)
  {
    /*
    bool res(true);
    if(p_root_tree != 0)
    {
      TNode<K, V>* l = nullptr;//указатель на элемент ,который мы ищем
      TNode<K, V>** array;// вспомогательный массив
      int depth;// глубина поддерева
      if(key >= p_root_tree->getKey())
      {
        depth = p_root_tree->getRightHeight() + 1;
      }
      else
      {
        depth  = p_root_tree->getLeftHeight() + 1;
      }
      array = new TNode<K, V>*[depth];// вспомогательный массив для балансировки поиска замены и т.д
      array[0] = p_root_tree;// записываем элементы , начиная с корня дерева
      int flag1 = 0;
      int j = 0;
      for(TNode<K, V>* i = p_root_tree ; ( ( i != nullptr ) && ( flag1 == 0 ) ) ; ++j )//по сути та же самая функция fFind() , только здесь мы ещё записываем ивсе вершины,через которые проходим 
      {
        if( key == i->getKey() )
        {
          flag1 = 1;
          l = i;
          array[j] = i;
        }
        if( key > i->getKey() )
        {
          array[j] = i;
          i = i->getRight();
          if (i == nullptr)
          {
            flag1 = 1;
            l = nullptr;
          }
        } 
        else if( key < i->getKey() )
        {
          array[j] = i;
          i = i->getLeft();
          if (i == nullptr)
          {
            flag1 = 1;
            l = nullptr;
          }
        }
      }
      if( l == nullptr )
      {
        //cout << "Error AVL: Don't find element " << key << "\n";
        return false;
      }
      else// ищем самый левый элемент правого поддерева(наша "замена" элемента l,который нужно удалить)
      {
        for(TNode<K, V>* i = l->getRight() ; i != nullptr ; j++ )
        {
          array[j] = i;
          i = i->getLeft();
        }

        j--;
        l->setKey( array[j]->getKey() );//помещаем значение нашей замены по адресу l
        if( j == 0 )//если у нас есть только корень и один лист(предельные случаи)
        {
          if(array[j]->getLeft() == nullptr)
          {
            p_root_tree = nullptr;
            delete array[j];//~TNode();
            array[j] = nullptr;
          }
          else
          {
            p_root_tree=array[j]->getLeft();
            delete array[j];//->~TNode();
            array[j] = nullptr;
          }
        }
        else//остальные случаи
        {
          if( ( array[j]->getRight() == nullptr ) && ( array[j]->getLeft() == nullptr ) )// если крайний левый элемент правого поддерева лист
          {
            if( j >= 1 )
            {
              if( array[j-1]->getLeft() == array[j] )
              {
                array[j-1]->setLeft(nullptr);
                array[j-1]->setLeftHeight(0);
              }
              else// вырожденный случай , всё правое поддерево - одна вершина
              {
                array[j-1]->setRight(nullptr);
                array[j-1]->setRightHeight(0);
              }
            }
          }
          else//если наш крайний элемент("замена") имеет потомка 
          {
            if( array[j]->getRight() != nullptr )
            {
              array[j-1]->setRight( array[j]->getRight() );
              array[j-1]->setLeftHeight( 1 );
            }
            else
            {
              array[j-1]->setLeft( array[j]->getLeft() );
              array[j-1]->setLeftHeight( 1 );
            }

          }
        }

        /////////идём вверх по массиву arrey ,восстанавливая высоты поддеревьев и проводя балансировки если нужно
        j--;
        j--;
        int flag = 0;
        if( p_root_tree != 0 )//дерево не пусто
        {
          if( p_root_tree->getRight() != 0 )
          {
            for( ; j >= 0 ; j-- )
            {
              if( array[j]->getLeft() == array[j + 1] )
              {
                TNode<K, V>* k;
                k = fBalance( &array[j + 1] );
                if( k )
                {
                  array[j]->setLeft( k );
                  int max = k->getRightHeight();
                  if( max < k->getLeftHeight() )
                  {
                    max = k->getLeftHeight();
                  }
                  array[j]->setLeftHeight( max + 1 );
                }
                else
                {
                  int max = array[j + 1]->getRightHeight();
                  if( max < array[j + 1]->getLeftHeight() )
                  {
                    max = array[j + 1]->getLeftHeight();
                  }
                  array[j]->setLeftHeight( max + 1 );
                }
              }
              if( array[j]->getRight() == array[j + 1] )
              {
                TNode<K, V>* k;
                k = fBalance( &array[j + 1] );
                if( k )
                {
                  array[j]->setRight( k );
                  int max = k->getRightHeight();
                  if( max < k->getLeftHeight() )
                  {
                    max = k->getLeftHeight();
                  }
                  array[j]->setRightHeight( max + 1 );
                }
                else
                {
                  int max = array[j + 1]->getRightHeight();
                  if( max < array[j + 1]->getLeftHeight() )
                  {
                    max = array[j + 1]->getLeftHeight();
                  }
                  array[j]->setRightHeight(max + 1);
                }
              }
            }
          }
        }
        if(p_root_tree!=nullptr)
        {
          fBalance(&p_root_tree);
        }
        n_elements--;
        delete [] array;
        //////////////////////////////////////////////////////////

      }
    }
    else//случай ,если дерево пусто
    {
      cout << "Empty\n";
    }
    return res;
    */
      return fDelete(key);
  }

  bool eraseItem(const unsigned int& i)
  {
    bool res(false);
    unsigned int inx(0);
    if ( i < n_elements ) 
    {
      V* mass = new V[i + 1];
      recPreOrderN(p_root_tree, mass, inx, i + 1);
      res = erase(mass[i]);
      delete [] mass;
    }

    return res;
  }

  TNode<K, V>* findNode( const K& key )// поиск вершины со значением v
  {
    int flag = 0;// если flag == 1 то вершина найдена
    if(p_root_tree != 0)//
    {
      for( TNode<K, V>* i = p_root_tree; ( ( i != nullptr ) && ( flag == 0 ) )  ;  )
      {
        if(key == i->getKey()) // вершина найдена
        {
          flag = 1;
          return i;
        }
        if( key > i->getKey() )//вершина справа
        {
          i = i->getRight();// идём направо
          if (i == nullptr)
          {
            flag=1;
            return nullptr;
          }
        } 
        else if( key < i->getKey() )//вершина слева
        {

          i = i->getLeft();//идём налево
          if (i == nullptr)
          {
            flag = 1;
            return nullptr;
          }
        }
      }
    }

    return nullptr;
  }

  //есть ли элемент с ключём key
  bool isFind(const K& key)
  {
    return (findNode( key ) != nullptr);
  }

  //записать значение элемента списка под индексом i в переменную ret_val
  bool getItem(const K& key, V& value)
  {
    bool res(false);
    TNode<K, V>* find_node = findNode( key );
    if (find_node != nullptr)
    {
      value = find_node->getValue();
      res = true;
    }

    return res;
  }

  void addVec(const std::vector<V>& values)
  {
    for (std::vector<V>::const_iterator it = values.begin(); it != values.end(); it++)
      addValue(*it);
  }

  void addVec(const std::vector<std::pair<K, V>>& values)
  {
    for (std::vector<std::pair<K, V>>::const_iterator it = values.begin(); it != values.end(); it++)
      add(it->first, it->second);
  }

  std::vector<V> getSortedArray()
  {
    std::vector<V> res;
    unsigned int inx(0);
    V* mass = new V[n_elements];
    recPreOrderN(p_root_tree, mass, inx, n_elements);
    for (unsigned int i(0); i < n_elements; i++)
      res.push_back(mass[i]);

    delete [] mass;

    return res;
  }

  std::vector<std::pair<K, V>> getArray()
  {
    std::vector<std::pair<K, V>> seq;
    unsigned int inx(0);
    V* mass = new V[n_elements];
    recPreOrderN(p_root_tree, mass, inx, n_elements);
    for (unsigned int i(0); i < n_elements; i++)
      seq.push_back(std::pair<K, V>(mass[i], mass[i]));

    delete [] mass;

    return seq;
  }

  bool getItemByInx(const unsigned int& i, V& ret_val)
  {
    bool res(false);
    unsigned int inx(0);
    if ( i < n_elements ) 
    {
      V* mass = new V[i + 1];
      recPreOrderN(p_root_tree, mass, inx, i + 1);
      ret_val = mass[i];
      res = true;
      delete [] mass;
    }

    return res;
  }

  bool getFirst(V& value )
  {
    bool res(false);
    if (n_elements > 0)
    {
      TNode<K, V>* current = p_root_tree;
      while(current->getLeft() != nullptr)
        current = current->getLeft();
      
      value = current->getValue();
      res = true;
    }

    return res;
  }

  bool getLast(V& value )
  {
    bool res(false);
    if (n_elements > 0)
    {
      TNode<K, V>* current = p_root_tree;
      while(current->getRight() != nullptr)
        current = current->getRight();

      value = current->getValue();
      res = true;
    }

    return res;
  }

  //отчистить список, удалить данные, освободить память
  void clear()
  {
    if(p_root_tree != nullptr)
    {
      int max = p_root_tree -> getLeftHeight();
      if( p_root_tree -> getRightHeight() > max )
      {
        max = p_root_tree -> getRightHeight();
      }
      TNode<K, V>**  array = new TNode<K, V>*[max+1];
      array[0] = p_root_tree;
      int i( 0 );
      while( n_elements > 0 && i > 0)
      {
        if( array[i]->getLeft() != nullptr )
        {
          i++;
          array[i] = array[i - 1]->getLeft();
        }
        else if(array[i]->getRight()!=nullptr)
        {
          i++;
          array[i] = array[i - 1]->getRight();
        }
        if( ( array[i]->getLeft() == nullptr ) && ( array[i] -> getRight() == nullptr) )
        {
          if(i!=0)
          {
            if(array[i-1]->getLeft() == array[i])
            {
              array[i-1]->setLeft(nullptr);
            }
            else
            {
              array[i-1]->setRight(nullptr);
            }
          }
          delete array[i];
          array[i] = nullptr;
          i--;
          n_elements--;

        }
      }
      delete [] array;
      p_root_tree = nullptr;
    }
  }

  int indexOf( const V& value )
  {
    int res(-1);
    bool isFind(false);
    unsigned int inx(0);
    recPreOrderV(p_root_tree, inx, value, isFind);
    if (isFind)
      res = (int)inx;

    return res;
  }

  void getSubSeq(ISortedSequence<V>* sub_list, unsigned int i_begin, unsigned int i_end)
  {    
    V* mass = new V[i_end + 1];
    if ( i_begin < n_elements && i_end < n_elements) 
    {
      
      unsigned int inx(0);
      recPreOrderN(p_root_tree, mass, inx, n_elements);
      if (sub_list != nullptr)
        for (unsigned int i(i_begin); i <= i_end; i++)
          sub_list->addValue(mass[i]);
      
    }
    //delete [] mass;

  }

  friend ostream& operator << (ostream& out, AVLTree<K, V>& tree)
  {
     printPretty(tree.p_root_tree, 6, 2, out);
     return out;
  }

private:
  //балансировка дерева
  TNode<K, V>* fBalance( TNode<K, V>** p ) //вращение дерева , подвешанного к элементу **p
  {

    int balance = (*p)->getRightHeight() - (*p)->getLeftHeight(); // определяем переменную баланс как разность между высотами правого и левого поддеревьев
    if( ( balance == 0 ) || ( balance == 1 ) || ( balance == -1 ) ) // если дерево сбалансировано ,то возвращаем NULL в знак того , что ничего не изменилось 
    {
      return nullptr;
    }
    else // требуется балансировка см Википедию http://ru.wikipedia.org/wiki/%D0%90%D0%92%D0%9B-%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE
    {
      ///////////////////////Малое правое вращение/////////////////////////////////
      if( ( balance == 2 ) && ( (*p)->getRight()->getRightHeight() >= (*p)->getRight()->getLeftHeight()) )
      {	
        // Вращение                                                       A               B
        TNode<K, V>* pBuffer = (*p);//A                                  / \             / \                                              /
        (*p) = (*p)->getRight(); //новая вершина B                      L   B   ---->   A   R
        pBuffer->setRight( (*p)->getLeft() );//под A подвесили С           / \         / \                                              /
        (*p) -> setLeft( pBuffer );//под B подвесили A слева              C   R       L   C

        //// Меняем значения высот дерева на новое,чтобы потом не тратить время на обход дерева
        int buffer = (*p)->getLeftHeight();// здесь ещё лежит старое значение глубины левого поддерева L у вершины B
        int max = (*p)->getLeft()->getLeftHeight();//выбираем поддерево максимальной глубины при вершине A(L или С)
        (*p)->getLeft()->setRightHeight( buffer );
        if( (*p)->getLeft()->getRightHeight() > max )
        {
          max = (*p)->getLeft()->getRightHeight();
        }
        (*p)->setLeftHeight( max + 1 );// присваиваем новое значение высоты левого поддерева при вершине B(c учётом A)
        return (*p);// возвращаем указатель на новую вершину
      }
      //////////////////////Большое правое вращение////////////////////////////////
      if( ( balance == 2 ) && ( (*p)->getRight()->getRightHeight() < (*p)->getRight()->getLeftHeight()) )
      {
        //Вращение
        TNode<K, V>* pBuffer = (*p);//записываем В буфер A                                                 A                          C
        (*p) = (*p)->getRight()->getLeft();//новый корень-вершина С                                       / \                       /   \ 
        pBuffer->getRight()->setLeft( (*p)->getRight() );       //под вершину B слева вешаем вершину N   L   B     ------>         A     B
        (*p) -> setRight( pBuffer->getRight() );//под новый корень С вешаем вершину B справа                / \                   / \   / \ 
        pBuffer -> setRight( (*p)->getLeft() );//под вершину A справа вешаем поддерево M                   C   R                 L  M  N   R 
        (*p) -> setLeft( pBuffer );//под новый корень С вешаем вершину A                                  / \
        //                                                                                               M   N
        ////Меняем значения высот дерева на новое,чтобы потом не тратить время на обход дерева
        (*p)->getLeft()->setRightHeight( (*p)->getLeftHeight() );
        (*p)->getRight()->setLeftHeight( (*p)->getRightHeight() );
        int max = (*p)->getLeft()->getLeftHeight(); 
        if( (*p)->getLeft()->getRightHeight() > max )
        {
          max = (*p)->getLeft()->getRightHeight();
        }
        (*p)->setLeftHeight( max + 1 );
        max = (*p)->getRight()->getLeftHeight();
        if( (*p)->getRight()->getRightHeight() > max )
        {
          max = (*p)->getRight()->getRightHeight();
        }
        (*p)->setRightHeight( max+1 );
        return ( *p );
      }
      //////////////////////Малое левое вращение///////////////////////////////////
      if( ( balance == -2) && ( (*p)->getLeft()->getRightHeight() <= (*p)->getLeft()->getLeftHeight() ) )
      {
        TNode<K, V>* pBuffer = (*p);// элемент A                                               A             B
        (*p) = (*p)->getLeft();// элемент B стал корневым                                     / \           / \ 
        pBuffer->setLeft( (*p)->getRight() );//к элементу A подвешиваем поддерево C          B   R  -----> L   A
        (*p)->setRight( pBuffer ); // к элементу B справа подвешивается элемент A           / \               / \
        //                                                                                 L   C             C   R
        ////Меняем значения высот дерева на новое,чтобы потом не тратить время на обход дерева
        int buffer = (*p)->getRightHeight();// здесь ещё лежит старое значение
        int max=(*p)->getRight()->getRightHeight();
        (*p)->getRight()->setLeftHeight( buffer );
        if( (*p)->getRight()->getLeftHeight() > max)
        {
          max = (*p)->getRight()->getLeftHeight();
        }
        (*p)->setRightHeight( max + 1 );
        return (*p);
      }
      ///////////////////// Большое левое вращение/////////////////////////////////
      if( ( balance == -2 ) && ( (*p)->getLeft()->getRightHeight() > (*p)->getLeft()->getLeftHeight() ) )
      {
        TNode<K, V>* pBuffer = (*p); // записываем в буфер старый корень A                                 A                  C
        (*p) = (*p)->getLeft()->getRight();// новый корень дерева С                                       / \               /   \ 
        pBuffer->getLeft()->setRight( (*p)->getLeft() );//к вершине B справа вешаем поддерево M          B   R             B     A
        (*p)->setLeft( pBuffer->getLeft() );//к новому корню С слева подвешиваем вершину B              / \     ------>   / \   / \ 
        pBuffer->setLeft( (*p)->getRight() );// к вершине A слева подвешиваем поддерево N              L   C             L  M  N   R
        (*p)->setRight( pBuffer );// к новому корню С справа вешаем вершину A                             / \
        //                                                                                               M   N
        ////Меняем значения высот дерева на новое,чтобы потом не тратить время на обход дерева
        (*p)->getLeft()->setRightHeight( (*p)->getLeftHeight() );
        (*p)->getRight()->setLeftHeight( (*p)->getRightHeight() );
        int max = (*p)->getLeft()->getLeftHeight();
        if( (*p)->getLeft()->getRightHeight() > max)
        {
          max = (*p)->getLeft()->getRightHeight();
        }
        (*p)->setLeftHeight( max + 1 );
        max = (*p)->getRight()->getLeftHeight();
        if( (*p)->getRight()->getRightHeight() > max )
        {
          max = (*p)->getRight()->getRightHeight();
        }
        (*p)->setRightHeight( max + 1 );
        return (*p);
      }
    }
    return nullptr;
  }

  void recPreOrderN(TNode<K, V>* node, V* sorted_mass, unsigned int& inx, unsigned int N)
  {
    if (inx < N)
    {
      if (node->getLeft() != nullptr) 
        recPreOrderN(node->getLeft(), sorted_mass, inx, N);

      sorted_mass[inx] = node->getValue();
      inx++;

      if (node->getRight() != nullptr) 
        recPreOrderN(node->getRight(), sorted_mass, inx, N);
    }

  }

  void recPreFullOrderN(TNode<K, V>* node, K* sorted_key, V* sorted_mass, unsigned int& inx, unsigned int N)
  {
    if (inx < N)
    {
      if (node->getLeft() != nullptr) 
        recPreFullOrderN(node->getLeft(), sorted_key, sorted_mass, inx, N);

      if (node != nullptr)
      {
        sorted_key[inx] = node->getKey();
        sorted_mass[inx] = node->getValue();
        inx++;
      }


      if (node->getRight() != nullptr) 
        recPreFullOrderN(node->getRight(), sorted_key, sorted_mass, inx, N);
    }

  }

  //Проставляем индекс до того как найдём значение value
  void recPreOrderV(TNode<K, V>* node, unsigned int& inx, const V& value, bool& isFind)
  {
    if (!isFind)
    {
      if (node->getLeft() != nullptr) 
        recPreOrderV(node->getLeft(), inx, value, isFind);

      if (node->getValue() == value)
        isFind = true;
      else
        inx++;

      if (node->getRight() != nullptr) 
        recPreOrderV(node->getRight(), inx, value, isFind);
    }

  }


// Приватные функции-друзья для вывода дерева
/*********************************************************************************************************************/
  friend int maxHeight(TNode<K, V>* node) 
  {
    if (node == nullptr) 
      return 0;

    int leftHeight = maxHeight(node->getLeft());
    int rightHeight = maxHeight(node->getRight());
    return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
  }

  // Вывод ветвей дерева ( /    \ ) в поток
  friend void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TNode<K, V>*>& nodesQueue, ostream& out) 
  {
    deque<TNode<K, V>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel / 2; i++) 
    {  
      out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
      out << setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
    }
    out << endl;
  }

  // Вывод ветвей и узлов дерева  ( /    \ ) в поток
  friend void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<TNode<K, V>*>& nodesQueue, ostream& out) 
  {
    deque<TNode<K, V>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) 
    {
      out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->getLeft()) ? setfill('_') : setfill(' '));
      out << setw(branchLen+2);
      if (*iter != nullptr)
        out << (*iter)->getValue();
      else
        out << " ";
      out << ((*iter && (*iter)->getRight()) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
    }
    out << endl;
  }

  // Вывод листьев дерева , только для нижнего уровня
  friend void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<TNode<K, V>*>& nodesQueue, ostream& out) 
  {
    deque<TNode<K, V>*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) 
    {
      if (i == 0)
        out << setw(indentSpace+2);
      else
        out << setw(2*level+2);

      if ((*iter) != nullptr)
        out << (*iter)->getValue();
      else
        out << " ";
    }
    out << endl;
  }

  // Форматированный вывод бинарного дерева в поток
  // level  Контролирует, насколько широко дерево должно быть разреженным (например, уровень 1 имеет минимальное пространство между узлами, а уровень 2 имеет большее пространство между узлами)
  // indentSpace  пространство отступа влево (например, indentSpace == 0 означает, что самый низкий уровень левого узла будет придерживаться левого края)
  friend void printPretty(TNode<K, V>* root, int level, int indentSpace, ostream& out) 
  {
    int h = maxHeight(root);
    int nodesInThisLevel = 1;

    int branchLen = 2*((int)pow(2.0,h)-1) - (3-level)*(int)pow(2.0,h-1);  // eq of the length of branch for each node of each level
    int nodeSpaceLen = 2 + (level+1)*(int)pow(2.0,h);  // distance between left neighbor node's right arm and right neighbor node's left arm
    int startLen = branchLen + (3-level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)

    deque<TNode<K, V>*> nodesQueue;
    nodesQueue.push_back(root);
    for (int r = 1; r < h; r++) 
    {
      printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
      branchLen = branchLen/2 - 1;
      nodeSpaceLen = nodeSpaceLen/2 + 1;
      startLen = branchLen + (3-level) + indentSpace;
      printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

      for (int i = 0; i < nodesInThisLevel; i++) 
      {
        TNode<K, V>* currNode = nodesQueue.front();
        nodesQueue.pop_front();
        if (currNode) 
        {
          nodesQueue.push_back(currNode->getLeft());
          nodesQueue.push_back(currNode->getRight());
        } 
        else 
        {
          nodesQueue.push_back(nullptr);
          nodesQueue.push_back(nullptr);
        }
      }
      nodesInThisLevel *= 2;
    }
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
    printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
  }
  /*********************************************************************************************************************/
  struct view_data
  {
    int level;
    bool is_left;
    view_data(int level_, bool is_left_) : level(level_), is_left(is_left_) {}
  };

  std::vector<std::pair<K, V>> getValues()
  {
    std::queue<std::pair<TNode<K, V>*, view_data>> q;
    q.push (std::pair<TNode<K, V>*, view_data>(p_root_tree, view_data(0, false)));
    std::vector<std::vector<std::pair<TNode<K, V>*, view_data>>> tree_view;

    int level(-1);
    while (q.size() != 0) 
    {
      std::pair<TNode<K, V>*, view_data> current_node = q.front();
      
      q.pop();
      if (level != current_node.second.level)
      {
        tree_view.push_back(std::vector<std::pair<TNode<K, V>*, view_data>>());
        level = current_node.second.level;
      }
//       if (level > 0)
//         if (current_node.second.is_left)
//           out << "/.";
//         else
//           out << "\\.";
      
     // out << (current_node.first->getValue()) << " ";
      tree_view[tree_view.size() - 1].push_back(current_node);
      
      //���� ���� ����� ���������, �� �������� ��� � ������� ��� ���������� ���������
      if (current_node.first->getLeft() != nullptr)
        q.push (std::pair<TNode<K, V>*, view_data>(current_node.first->getLeft(), view_data(current_node.second.level + 1, true)));
      //���� ���� ������ ���������, �� �������� ��� � ������� ��� ���������� ���������
      if (current_node.first->getRight() != nullptr)
        q.push (std::pair<TNode<K, V>*, view_data>(current_node.first->getRight(), view_data(current_node.second.level + 1, false)));
    }

    std::vector<std::pair<K, V>> out;
    for (std::vector<std::vector<std::pair<TNode<K, V>*, view_data>>>::iterator it = tree_view.begin(); it != tree_view.end(); it++)
      for (std::vector<std::pair<TNode<K, V>*, view_data>>::iterator jt = it->begin(); jt != it->end(); jt++)
      out.push_back(std::pair<K, V>(jt->first->getKey(), jt->first->getValue()));

    return out;
  }
};
