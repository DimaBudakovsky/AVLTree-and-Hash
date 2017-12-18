#pragma once
#include <vector>

template <typename V> class ISortedSequence
{
public:
  //получить длину списка
  virtual unsigned int getLength() const = 0;
  //Признак того, является ли последовательность пустой
  virtual bool isEmpty() const = 0;

  virtual void addValue( const V& value ) = 0;

  //отчистить список, удалить данные, освободить память
  virtual void clear() = 0;

  //удалить элемент последовательности с индексом i
  virtual bool eraseItem(const unsigned int& i) = 0;

  virtual bool getFirst(V& value ) = 0;

  virtual bool getLast(V& value ) = 0;

  virtual int indexOf( const V& value ) = 0;

  //записать значение элемента последовательности под индексом i в переменную ret_val
  virtual bool getItemByInx(const unsigned int& i, V& ret_val) = 0;
  //получить подмножество последовательности согласно заданным свойствам: 
  //[i_begin, i_end] диапазон элементов для обработки;
  virtual void getSubSeq(ISortedSequence<V>* sub_seq, unsigned int i_begin, unsigned int i_end) = 0;

  virtual void addVec(const std::vector<V>& values) = 0;
  virtual std::vector<V> getSortedArray() = 0;
};