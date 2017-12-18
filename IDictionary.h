#pragma once

enum DictionaryType
{
  No = 0,
  HashTable=1,
  AVL=2
};

template <typename K, typename V> class IDictionary
{
public:
  //получить длину спискаь
  virtual DictionaryType getType() const = 0;

  //получить кол-во элементов, которое физически хранится 
  virtual unsigned int getCount() const = 0;

  //Вместимость – максимальное количество элементов, которое можно поместить в таблицу без необходимости ее перестройки.
  virtual unsigned int getCapacity()  const = 0;

  //Признак того, является ли последовательность пустой
  virtual bool isEmpty() const = 0;

  //добавить новый элемент с значением value и ключём key
  virtual void add( const K& key, const V& value ) = 0;

  //отчистить список, удалить данные, освободить память
  virtual void clear() = 0;

  //удалить элемент списка с ключём key
  virtual bool erase(const K& key) = 0;

  //есть ли элемент с ключём key
  virtual bool isFind(const K& key) = 0;

  //записать значение элемента списка под индексом i в переменную ret_val
  virtual bool getItem(const K& key, V& value) = 0;

  virtual std::vector<std::pair<K, V>> getArray() = 0;
  virtual void addVec(const std::vector<std::pair<K, V>>& values) = 0;

};