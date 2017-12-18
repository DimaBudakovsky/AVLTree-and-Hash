//file       Student.h
//brief      реализация классов Студент
//author     Будаковский Дмитрий
//date       27.11.2017
//version    2.0
//std        1.0

#pragma once
#include <string>
#include <iostream>
#include <cmath>
#include <map>

double ddelta = 10000;

struct StudentBase
{
public:
  std::string full_name;
  int age;
  double score;
  long long number_phone;

public:
  StudentBase() : full_name(""), age(0), score(0), number_phone(0) 
  {
  }

  StudentBase(std::string full_name_, int age_, double score_, long long number_phone_) : full_name(full_name_), age(age_), score(score_), number_phone(number_phone_) 
  {
  }

  ~StudentBase() 
  {
  }

  friend bool operator == (const StudentBase &a, const StudentBase &b)
  {
    return (a.age == b.age && (a.score < b.score + (1.0/ddelta) && a.score > b.score - (1.0/ddelta)) && a.number_phone == b.number_phone && a.full_name.compare(b.full_name) == 0);
  }

  friend bool operator != (const StudentBase &a, const StudentBase &b)
  {
    return !(a == b);
  }

};

std::string toString(const int& val)
{
  std::stringstream ss;
  ss << val;
  return ss.str();
}

std::vector<std::string> generateRandomString(const int& N)
{
  std::vector<std::string> res;
  std::map<std::string, int> map_string;
  char random[28] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g',
                     'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', '.'};

  srand((unsigned int)time(nullptr));
  for (int l(0); l < N; l++)
  {
    std::string str("");
    bool is_new(false);
    while (!is_new)
    {
      //генерация ФИО
      str = "";
      int size = rand()%10;
      if (size == 0)
        size = 5;

      for (int j = 0; j < size; j++)
      {
        std::string str_add("");
        str_add = random[rand()%28];
        str += str_add;
      }

      if (map_string.find(str) == map_string.end())
      {
        map_string[str] = 1;
        is_new = true;
      }
    }
    res.push_back(str);
  }
  return res;
}

std::vector<int> generateRandomInt(const int& N)
{
  std::vector<int> res;
  std::map<int, int> map_random_int;

  for (int l(0); l < N; l++)
  {
    int random_int(0);
    srand((unsigned int)time(nullptr));
    std::string str("");
    bool is_new(false);
    while (!is_new)
    {
      random_int = rand(); //чтобы больше вариантов
      if (map_random_int.find(random_int) == map_random_int.end())
      {
        map_random_int[random_int] = 1;
        is_new = true;
      }
    }
    res.push_back(random_int);
  }
  
  return res;
}

//Генерируем уникального студента
std::vector<StudentBase> generateRandomStudents(const int& N)
{
  std::vector<StudentBase> generated_students;

  std::cout << "int" << endl;
  std::vector<int> temp_random_int;
  if (N <= 1000)
    temp_random_int = generateRandomInt(N);
  else
  {
    for (int i(0); i < N; i++)
      temp_random_int.push_back(i);

    srand(unsigned(time(0)));
    random_shuffle(temp_random_int.begin(), temp_random_int.end()); // перемешиваеаем
  }
  std::cout << "int size " << temp_random_int.size() << endl;

  std::cout << "string" << endl;
  std::vector<std::string> temp_random_str;
  if (N <= 1000)
    temp_random_str = generateRandomString(N);
  else
  {
    temp_random_str = generateRandomString(1000);
    std::vector<std::string> temp_temp_random_str;
    for (int i(0); i < N; i++)
      temp_temp_random_str.push_back(temp_random_str[temp_random_int[i]%1000] + toString(temp_random_int[i]));

    temp_random_str = temp_temp_random_str;
  }
  srand(unsigned(time(0)));
  random_shuffle(temp_random_str.begin(), temp_random_str.end()); // перемешиваеаем
  std::cout << "string size " << temp_random_str.size() << endl;

//   std::map<std::string, int> map_string;
//   for (int l(0); l < N; l++)
//   {
//     if (map_string.find(temp_random_str[l]) == map_string.end())
//       map_string[temp_random_str[l]] = 1;
//     else
//       std::cout << "For String don't work" << endl;
//   }

  std::cout << "double" << endl;
  std::vector<double> temp_random_double;
  for (int i(0); i < N; i++)
  {
    int mantissa(0);
    if (i > 1)
      mantissa = (temp_random_int[i - 1]%(int)ddelta);

    temp_random_double.push_back(temp_random_int[i]+((double)(mantissa%(int)ddelta))/ddelta);
  }
  srand(unsigned(time(0)));
  random_shuffle(temp_random_double.begin(), temp_random_double.end()); // перемешиваеаем
  std::cout << "double size " << temp_random_double.size() << endl;

//   for (int i(0); i < N; i++)
//   {
//     for (int j(0); j < N; j++)
//     {
//       if (i != j && temp_random_double[i] < temp_random_double[j] + 1./ddelta && temp_random_double[i] > temp_random_double[j] - 1./ddelta)
//         std::cout << "For Double don't work" << endl;
//     }
//   }

  std::cout << "long" << endl;
  std::vector<long long> temp_random_long;
  for (int i(0); i < N; i++)
  {
    long long m = 8455;
    m *= (long long)std::pow(10, 7);
    long long val = m + (long long)temp_random_int[i];
    temp_random_long.push_back(val);
  }

  srand(unsigned(time(0)));
  random_shuffle(temp_random_long.begin(), temp_random_long.end()); // перемешиваеаем
  std::cout << "long size " << temp_random_long.size() << endl;

  for (int i(0); i < N; i++)
    generated_students.push_back(StudentBase(temp_random_str[i], temp_random_int[i], temp_random_double[i], temp_random_long[i]));

  return generated_students;
}

template<typename V> V convertStudent(StudentBase& stb)
{  
  V st = *(static_cast<V*>(&stb));
  return st;
}

struct StudentByAge : public StudentBase
{
public:
  friend bool operator <(const StudentByAge &a, const StudentByAge &b)
  {
    return a.age < b.age;
  }

  friend bool operator >(const StudentByAge &a, const StudentByAge &b)
  {
    return a.age > b.age;
  }

  friend bool operator <=(const StudentByAge &a, const StudentByAge &b)
  {
    return !(a > b);
  }

  friend bool operator >=(const StudentByAge &a, const StudentByAge &b)
  {
    return !(a < b);
  }

//   friend std::ostream &operator << (std::ostream& out, StudentByAge& st)
//   {
//     out << st.age;
//     return out;
//   }

  friend std::ostream &operator << (std::ostream& out, const StudentByAge& st)
  {
    out << st.age;
    return out;
  }
};

struct StudentByFullName : public StudentBase
{
public:
  friend bool operator <(const StudentByFullName &a, const StudentByFullName &b)
  {
    return a.full_name.compare(b.full_name) < 0;
  }

  friend bool operator >(const StudentByFullName &a, const StudentByFullName &b)
  {
    return a.full_name.compare(b.full_name) > 0;
  }

  friend bool operator <=(const StudentByFullName &a, const StudentByFullName &b)
  {
    return !(a > b);
  }

  friend bool operator >=(const StudentByFullName &a, const StudentByFullName &b)
  {
    return !(a < b);
  }

  friend std::ostream &operator << (std::ostream& out, const StudentByFullName& st)
  {
    out << st.full_name;
    return out;
  }
};

struct StudentByScore : public StudentBase
{
public:

  friend bool operator <(const StudentByScore &a, const StudentByScore &b)
  {
    return a.score + 1./ddelta < b.score;
  }

  friend bool operator >(const StudentByScore &a, const StudentByScore &b)
  {
    return a.score > b.score + 1./ddelta;
  }

  friend bool operator <=(const StudentByScore &a, const StudentByScore &b)
  {
    return !(a > b);
  }

  friend bool operator >=(const StudentByScore &a, const StudentByScore &b)
  {
    return !(a < b);
  }

  friend std::ostream &operator << (std::ostream& out, const StudentByScore& st)
  {
    out << st.score;
    return out;
  }
};


struct StudentByPhoneNumber : public StudentBase
{
public:

  friend bool operator <(const StudentByPhoneNumber &a, const StudentByPhoneNumber &b)
  {
    return a.number_phone < b.number_phone;
  }

  friend bool operator >(const StudentByPhoneNumber &a, const StudentByPhoneNumber &b)
  {
    return a.number_phone > b.number_phone;
  }

  friend bool operator <=(const StudentByPhoneNumber &a, const StudentByPhoneNumber &b)
  {
    return !(a > b);
  }

  friend bool operator >=(const StudentByPhoneNumber &a, const StudentByPhoneNumber &b)
  {
    return !(a < b);
  }

  friend std::ostream &operator << (std::ostream& out, const StudentByPhoneNumber& st)
  {
    out << st.number_phone;
    return out;
  }
};