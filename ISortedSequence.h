#pragma once
#include <vector>

template <typename V> class ISortedSequence
{
public:
  //�������� ����� ������
  virtual unsigned int getLength() const = 0;
  //������� ����, �������� �� ������������������ ������
  virtual bool isEmpty() const = 0;

  virtual void addValue( const V& value ) = 0;

  //��������� ������, ������� ������, ���������� ������
  virtual void clear() = 0;

  //������� ������� ������������������ � �������� i
  virtual bool eraseItem(const unsigned int& i) = 0;

  virtual bool getFirst(V& value ) = 0;

  virtual bool getLast(V& value ) = 0;

  virtual int indexOf( const V& value ) = 0;

  //�������� �������� �������� ������������������ ��� �������� i � ���������� ret_val
  virtual bool getItemByInx(const unsigned int& i, V& ret_val) = 0;
  //�������� ������������ ������������������ �������� �������� ���������: 
  //[i_begin, i_end] �������� ��������� ��� ���������;
  virtual void getSubSeq(ISortedSequence<V>* sub_seq, unsigned int i_begin, unsigned int i_end) = 0;

  virtual void addVec(const std::vector<V>& values) = 0;
  virtual std::vector<V> getSortedArray() = 0;
};