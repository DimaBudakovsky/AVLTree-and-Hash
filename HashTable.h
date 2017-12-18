#pragma once

#include <iostream>
#include <fstream>
#include "IDictionary.h"


template <typename K, typename V> class TElement// класс TElement (простой односвязный список для учёта коллизий хеш-таблицы)
{
  private:
    K m_key;// ключ, наприме ФИО студента
    V p_value;// значение, например предмет - оценка
    TElement* p_next;//указатель на следующий элемент


  public:
    TElement() : p_next(0)
    {
      p_value = V();
    }

    TElement(K key , V val , TElement* pNext)  //конструктор k-ключ(имя студента), v1-предмет , v2 - оценка
    {
      p_next = pNext;
      m_key = key;
      p_value = val;
    }

    ~TElement()
    { 
      setNext(nullptr);
    }

    void setValue( V val )//кладёт значение в сообтветствующий элемент массива значений Value[]
    {                     //добавить пару "предмет-оценка"
      p_value = val;
    }

    K getKey()
    {
      return m_key;
    }

    V getValue()
    {
      return p_value;
    }

    void setNext(TElement* el)
    {
      p_next = el;
    }

    TElement* getNext()
    {
      return p_next;
    }
};

enum HashType
{
  Additive = 0,
  AdditiveModified=1,
  XorPearson=2
};

template <typename K, typename V> class CHashTable : public IDictionary<K, V>
{
  private:
    HashType m_hash_type;
    TElement<K, V>** m_hash_table;//массив списков
    int m_capacity;//высота хэш-таблицы
    int m_count; // число элементов по факту хранения
    const double m_reconstruction_coeff; //q, во сколько расширять, сжимать вместимость хеш-таблицы
    const double m_criterion_coeff;//p критерий перестроение(1/доля заполненности), если m_count <= m_capacity/m_criterion_coeff, то m_capacity /=  m_reconstruction_coeff;
                                                            //если m_count == m_capacity, то m_capacity *=  m_reconstruction_coeff;

    bool isFindEl(const K& key, TElement<K, V>* ret_el)
    {
      bool isFind(false);
      unsigned int index = HashFunction( key );//находим номер ячейки
      TElement<K, V>* current = m_hash_table[index];
      while (current != nullptr && !isFind)//идём по списку и ищем элемент
      {
        if (current->getKey() != key)
          current = current->getNext();
        else
          isFind = true;
      }

      if (isFind)
        ret_el = current;


      return isFind;
    }

    void restructure(int new_capacity)
    {
      cout << "restructure old_capacity: " << m_capacity << " collisions: " << getNumberCollisions();

      //сохранение всех узлов
      std::vector<TElement<K, V>*> data;
      for (int i(0); i < m_capacity; i++)
        if (m_hash_table[i] != nullptr)
        {
          TElement<K, V>* current = m_hash_table[i];
          while (current != nullptr)
          {
            data.push_back(current);
            current = current->getNext();
          }
        }

      m_capacity = new_capacity;

      //перераспределение памяти общего массива
      delete [] m_hash_table;
      m_hash_table = new TElement<K, V>*[m_capacity];
      //инициализация
      int len = m_capacity - 1;
      while(len >= 0)
      {
        m_hash_table[len] = nullptr;
        len--;
      }
      //привязка узлов к новой хеш-таблицы
      for (std::vector<TElement<K, V>*>::iterator it = data.begin(); it != data.end(); it++)
      {
        unsigned int index = HashFunction( (*it)->getKey() );//находим номер ячейки
        if( m_hash_table[index] != nullptr ) //случай коллизии
          (*it)->setNext(m_hash_table[index]);
        else
          (*it)->setNext(nullptr);// обнуляем старые связи

        m_hash_table[index] = (*it);//новое начало
      }

      cout << " new_capacity: " << m_capacity << " collisions: " << getNumberCollisions() << endl;
    }

  public:

    CHashTable( HashType hash_type = HashType::XorPearson, int len = 10) : m_count(0), m_reconstruction_coeff(2), m_criterion_coeff(10.), m_hash_type(hash_type)
    {
      m_capacity = len;
      m_hash_table = new TElement<K, V>*[len];
      len--;
      while(len >= 0)
      {
        m_hash_table[len] = nullptr;
        len--;
      }
    }

    ~CHashTable()
    {
      clear();
      delete [] m_hash_table;
    }

    unsigned int HashFunction( K key )//хэш-функция по заданному ключу возвращает номер ячейки 
    {
      unsigned char* key_data = reinterpret_cast<unsigned char*>(&key);
      unsigned int size = sizeof(K);
      char* buff = nullptr;
      if (typeid(K).hash_code() == typeid(std::string).hash_code())
      {
        std::string* p_str = reinterpret_cast<std::string*>(&key);
        size = p_str->size();
        buff = new char[size];
        const char* buf2 = p_str->c_str();
        for (int i(0); i < size; i++)
          buff[i] = buf2[i];

        key_data = reinterpret_cast<unsigned char*>(buff);
      }

      unsigned int inx(0);//номер ячеёки
      unsigned char h(0), h1(0), h2(0), h3(0), h4(0);

      // 0-255 shuffled in any (random) order suffices
      unsigned char rand8[256] = {98, 6, 85, 150, 36, 23, 112, 164, 135, 207, 169, 5, 26, 64, 165, 219,                    //  1
                                  61, 20, 68, 89, 130, 63, 52, 102, 24, 229, 132, 245, 80, 216, 195, 115,                  //  2
                                  90, 168, 156, 203, 177, 120, 2, 190, 188, 7, 100, 185, 174, 243, 162, 10,                //  3
                                  237, 18, 253, 225, 8, 208, 172, 244, 255, 126, 101, 79, 145, 235, 228, 121,              //  4
                                  123, 251, 67, 250, 161, 0, 107, 97, 241, 111, 181, 82, 249, 33, 69, 55,                  //  5
                                  59, 153, 29, 9, 213, 167, 84, 93, 30, 46, 94, 75, 151, 114, 73, 222,                     //  6
                                  197, 96, 210, 45, 16, 227, 248, 202, 51, 152, 252, 125, 81, 206, 215, 186,               //  7
                                  39, 158, 178, 187, 131, 136, 1, 49, 50, 17, 141, 91, 47, 129, 60, 99,                    //  8
                                  154, 35, 86, 171, 105, 34, 38, 200, 147, 58, 77, 118, 173, 246, 76, 254,                 //  9
                                  133, 232, 196, 144, 198, 124, 53, 4, 108, 74, 223, 234, 134, 230, 157, 139,              // 10
                                  189, 205, 199, 128, 176, 19, 211, 236, 127, 192, 231, 70, 233, 88, 146, 44,              // 11
                                  183, 201, 22, 83, 13, 214, 116, 109, 159, 32, 95, 226, 140, 220, 57, 12,                 // 12
                                  221, 31, 209, 182, 143, 92, 149, 184, 148, 62, 113, 65, 37, 27, 106, 166,                // 13
                                  3, 14, 204, 72, 21, 41, 56, 66, 28, 193, 40, 217, 25, 54, 179, 117,                      // 14
                                  238, 87, 240, 155, 180, 170, 242, 212, 191, 163, 78, 218, 137, 194, 175, 110,            // 15
                                  43, 119, 224, 71, 122, 142, 42, 160, 104, 48, 247, 103, 15, 11, 138, 239                 // 16
                                 };

      switch (m_hash_type)
      {
        case HashType::Additive:
          for(unsigned int i(0) ; i < size ; i++ )
            inx += key_data[i];// аддитивный+побитовый сдвиг

          break;
        case HashType::AdditiveModified:
          for(unsigned int i(0) ; i < size ; i++ )
            inx += (inx << 8) + key_data[i];// аддитивный+побитовый сдвиг

          break;
        case HashType::XorPearson:
          for (unsigned int j = 0; j < 4; ++j) 
          {
            h = rand8[(key_data[0] + j) % 256];
            for (unsigned int i = 1; i < size; ++i) 
            {
              h = rand8[h ^ key_data[i]];
            }
            inx = inx | ((unsigned int)h << 8 * j);
          }
          break;
      }

      inx = inx%m_capacity;

      if (buff != nullptr)
        delete [] buff;

      return inx;
    }

    int getNumberCollisions() const
    {
      int n_collisions(0);
      for (int i(0); i < m_capacity; i++)
      {
        if (m_hash_table[i] != nullptr)
        {
          int n_elements(0);
          TElement<K, V>* current = m_hash_table[i];
          while (current != nullptr)
          {
            n_elements++;
            current = current->getNext();
          }
          if (n_elements > 1)
            n_collisions += n_elements - 1;
        }

      }
      return n_collisions;
    }

    void add( const K& key, const V& value )
    {
      unsigned int index = HashFunction( key );//находим номер ячейки
      TElement<K, V> *m = new TElement<K, V>( key , value , 0 );
      if( m_hash_table[index] != nullptr ) //случай коллизии
      {
        if( !isFind(key) )
        {
          m->setNext(m_hash_table[index]);
          m_hash_table[index] = m;
          m_count++;
        }
        else
          delete m;
      }
      else
      {
        m_hash_table[index] = m;//новое начало
        m_count++;
      }

      //Проверка на реконструкцию хеш-таблицы
      if (m_count >= m_capacity)//расширяем хеш таблицу
        restructure((int)(m_capacity * m_reconstruction_coeff));
      else if  (m_count < ((double)m_capacity)/m_criterion_coeff)//сжимаем хеш таблицу
        restructure((int)(m_capacity/m_reconstruction_coeff));
    }

    bool erase(const K& key)
    {
      unsigned int index = HashFunction( key );//находим номер ячейки
      TElement<K, V>* pBegin = m_hash_table[index];

      bool isFind(false);
      TElement<K, V>* current = pBegin;
      TElement<K, V>* previous = nullptr;
      while (current != nullptr && !isFind)//идём по списку и ищем элемент
      {
        if (current->getKey() != key)
        {
          previous = current;
          current = current->getNext();
        }
        else
          isFind = true;
      }

      if (isFind)
      {
        if (previous != nullptr)
          previous->setNext(current->getNext());
        else
          m_hash_table[index] = current->getNext();

        m_count--;
        delete current;

        //Проверка на реконструкцию хеш-таблицы
        if (m_count >= m_capacity)//расширяем хеш таблицу
          restructure((int)(m_capacity * m_reconstruction_coeff));
        else if  (m_count <= ((double)m_capacity)/m_criterion_coeff)//сжимаем хеш таблицу
          restructure((int)(m_capacity/m_reconstruction_coeff));
      }

      return isFind;
    }

    bool isFind(const K& key)
    {
      TElement<K, V>* ret_el = nullptr;
      return isFindEl(key, ret_el);
    }

    void clear()
    {
      for(int i = 0 ; i < m_capacity ; i++)
      {
        TElement<K, V>* current = m_hash_table[i];
        m_hash_table[i] = nullptr;
        while(current != nullptr)
        {
          TElement<K, V>* del = current;
          current = current->getNext();
          delete del;
        }
      }
      m_count = 0;
    }

    DictionaryType getType() const
    {
      return DictionaryType::HashTable;
    }

    //получить кол-во элементов, которое физически хранится 
    unsigned int getCount() const
    {
      return m_count;
    }

    //Вместимость – максимальное количество элементов, которое можно поместить в таблицу без необходимости ее перестройки.
    unsigned int getCapacity()  const
    {
      return m_capacity;
    }

    //Признак того, является ли последовательность пустой
    bool isEmpty() const
    {
      return (m_count == 0);
    }

    //записать значение элемента списка под индексом i в переменную ret_val
    bool getItem(const K& key, V& value)
    {
      TElement<K, V>* finded_el(nullptr);
      bool res = isFindEl(key, finded_el);
      if (finded_el != nullptr)
        value = finded_el->getValue();
      else
        res = false;

      return res;
    }

    std::vector<std::pair<K, V>> getArray()
    {
      std::vector<std::pair<K, V>> seq;
      for (int i(0); i < m_capacity; i++)
      {
        if (m_hash_table[i] != nullptr)
        {
          TElement<K, V>* current = m_hash_table[i];
          while (current != nullptr)
          {
            seq.push_back(std::pair<K, V>(current->getKey(), current->getValue()));
            current = current->getNext();
          }
        }
      }
      return seq;
    }

    void addVec(const std::vector<std::pair<K, V>>& values)
    {
      for (std::vector<std::pair<K, V>>::const_iterator it = values.begin(); it != values.end(); it++)
      {
        unsigned int index = HashFunction( it->first );//находим номер ячейки
        TElement<K, V> *m = new TElement<K, V>( it->first , it->second , 0 );
        if( m_hash_table[index] != nullptr ) //случай коллизии
        {
          if( !isFind(it->first) )
          {
            m->setNext(m_hash_table[index]);
            m_count++;
            m_hash_table[index] = m;
          }
          else
            delete m;
        }
        else
        {
          m_hash_table[index] = m;//новое начало
          m_count++;
        }
      }

      //Проверка на реконструкцию хеш-таблицы
      if (m_count >= m_capacity)//расширяем хеш таблицу
        restructure((int)(m_capacity * m_reconstruction_coeff));
      else if  (m_count <= ((double)m_capacity)/m_criterion_coeff)//сжимаем хеш таблицу
        restructure((int)(m_capacity/m_reconstruction_coeff));
    }

    friend ostream& operator << (ostream& out, CHashTable<K, V>& hash)
    {
      for (int i(0); i < hash.m_capacity; i++)
      {
        if (hash.m_hash_table[i] != nullptr)
        {
          out << "Hash[" << i << "] : ";
          TElement<K, V>* current = hash.m_hash_table[i];
          while (current != nullptr)
          {
            out << current->getKey();
            current = current->getNext();
            if (current != nullptr)
              out << ", ";
          }
          out << endl;

        }
      }

      return out;
    }
};

