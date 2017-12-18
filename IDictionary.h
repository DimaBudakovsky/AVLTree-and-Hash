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
  //�������� ����� �������
  virtual DictionaryType getType() const = 0;

  //�������� ���-�� ���������, ������� ��������� �������� 
  virtual unsigned int getCount() const = 0;

  //����������� � ������������ ���������� ���������, ������� ����� ��������� � ������� ��� ������������� �� �����������.
  virtual unsigned int getCapacity()  const = 0;

  //������� ����, �������� �� ������������������ ������
  virtual bool isEmpty() const = 0;

  //�������� ����� ������� � ��������� value � ������ key
  virtual void add( const K& key, const V& value ) = 0;

  //��������� ������, ������� ������, ���������� ������
  virtual void clear() = 0;

  //������� ������� ������ � ������ key
  virtual bool erase(const K& key) = 0;

  //���� �� ������� � ������ key
  virtual bool isFind(const K& key) = 0;

  //�������� �������� �������� ������ ��� �������� i � ���������� ret_val
  virtual bool getItem(const K& key, V& value) = 0;

  virtual std::vector<std::pair<K, V>> getArray() = 0;
  virtual void addVec(const std::vector<std::pair<K, V>>& values) = 0;

};