#include <string>
#include <iostream>
#include "AVLTree.h"
#include "HashTable.h"
#include "Student.h"
#include <Windows.h>
#include <stdint.h> 
#include <sys/timeb.h>
#include <algorithm>
#include <functional>

int gettimeofday(struct timeval * tp, struct timezone * tzp) 
{ 
  // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's 
  static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL); 

  SYSTEMTIME system_time; 
  FILETIME file_time; 
  uint64_t time; 

  GetSystemTime(&system_time); 
  SystemTimeToFileTime(&system_time, &file_time); 
  time = ((uint64_t)file_time.dwLowDateTime); 
  time += ((uint64_t)file_time.dwHighDateTime) << 32; 

  tp->tv_sec = (long)((time - EPOCH) / 10000000L); 
  tp->tv_usec = (long)(system_time.wMilliseconds * 1000); 
  return 0; 
} 

double wtime() 
{ 
  double t(.0);
  static int sec = -1; 
  struct timeval tv; 
  gettimeofday(&tv, 0); 
  if (sec < 0) sec = tv.tv_sec; 
  t = (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec; 

  return t;
}

template<typename V> bool testAvl(AVLTree<V, V>& avlc, std::vector<V> stb, std::vector<V>& sorted_stb, double& add_time, double& find_time, double& sorted_time, double& erase_time)
{
  bool res(true);
  AVLTree<V, V> avl;
  double add_time_start = wtime();
  for (std::vector<V>::iterator it = stb.begin(); it != stb.end(); it++)
    avl.addValue(*it);

  double add_time_end = wtime();
  add_time = add_time_end - add_time_start;

  double find_time_start = wtime();
  for (std::vector<V>::iterator it = stb.begin(); it != stb.end(); it++)
    res = res && avl.isFind(*it);

  double find_time_end = wtime();
  find_time = find_time_end - find_time_start;

  double sorted_time_start = wtime();
  sorted_stb = avl.getSortedArray();
  double sorted_time_end = wtime();
  sorted_time = sorted_time_end - sorted_time_start;

  
  double erase_time_start = wtime();
//   for (std::vector<V>::iterator it = stb.begin(); it != stb.end(); it++)
//     /*res = res && *//*avl.erase(*it);*/
//   avl.fDelete(*it);
  avl.clear();

  double erase_time_end = wtime();
  erase_time = erase_time_end - erase_time_start;

  return res;
}

template<typename V> void testSorted(std::vector<StudentBase>& stb)
{  
  std::vector<V> students;
  for (std::vector<StudentBase>::iterator it = stb.begin(); it != stb.end(); it++)
    students.push_back(convertStudent<V>(*it));

  std::vector<V> students_sorted = students;// отсортированный массив
  std::sort(students_sorted.begin(), students_sorted.end());

  std::vector<V> students_sorted_rev = students;// отсортированный в обратном порядке массив
  std::sort(students_sorted_rev.begin(), students_sorted_rev.end(), std::greater<V>());

  AVLTree<V, V> avl;
  std::vector<V> res_students_sort;
  double add_time(0.);
  double find_time(0.);
  double sorted_time(0.);
  double erase_time(0.);
  bool res_random_test_avl = testAvl<V>(avl, students, res_students_sort, add_time, find_time, sorted_time, erase_time);
  if (res_random_test_avl)
  {
    bool sort_res(true);
    if (students_sorted.size() == res_students_sort.size())
      for (int i(0); i < (int)students_sorted.size(); i++)
        sort_res = sort_res && (students_sorted[i] == res_students_sort[i]);
    else
      sort_res = false;

    res_random_test_avl = sort_res;

    if (!sort_res)
      std::cout << "Test Avl tree was failed in random sequence: Sorted array is't equal" << std::endl;
    else
      std::cout << "Test Avl tree was successed in random sequence: Add Time: " << add_time << " ,Find Time: " << find_time << " ,Sort Time: " << sorted_time << /*" ,Erase Time: " << erase_time <<*/ std::endl;
  }
  else
    std::cout << "Test Avl tree was failed in random sequence: in random sequence in find test or erase test" << std::endl;

  bool res_sorted_test_avl = testAvl<V>(avl, students_sorted, res_students_sort, add_time, find_time, sorted_time, erase_time);
  if (res_sorted_test_avl)
  {
    bool sort_res(true);
    if (students_sorted.size() == res_students_sort.size())
      for (int i(0); i < (int)students_sorted.size(); i++)
        sort_res = sort_res && (students_sorted[i] == res_students_sort[i]);
    else
    sort_res = false;

    res_sorted_test_avl = sort_res;

    if (!sort_res)
      std::cout << "Test Avl tree was failed in sorted sequence: Sorted array is't equal" << std::endl;
    else
      std::cout << "Test Avl tree was successed in sorted sequence: Add Time: " << add_time << " ,Find Time: " << find_time << " ,Sort Time: " << sorted_time << /*" ,Erase Time: " << erase_time <<*/ std::endl;
  }
  else
    std::cout << "Test Avl tree was failed in sorted sequence: in random sequence in find test or erase test" << std::endl;


  bool res_sorted_rev_test_avl = testAvl<V>(avl, students_sorted_rev, res_students_sort, add_time, find_time, sorted_time, erase_time);
  if (res_sorted_rev_test_avl)
  {
    bool sort_res(true);
    if (students_sorted.size() == res_students_sort.size())
      for (int i(0); i < (int)students_sorted.size(); i++)
        sort_res = sort_res && (students_sorted[i] == res_students_sort[i]);
    else
    sort_res = false;

    res_sorted_rev_test_avl = sort_res;

    if (!sort_res)
      std::cout << "Test Avl tree was failed in sorted by greater sequence: Sorted array is't equal" << std::endl;
    else
      std::cout << "Test Avl tree was successed in sorted by greater sequence: Add Time: " << add_time << " ,Find Time: " << find_time << " ,Sort Time: " << sorted_time << /*" ,Erase Time: " << erase_time <<*/ std::endl;
  }
  else
    std::cout << "Test Avl tree was failed in sorted by greater sequence: in random sequence in find test or erase test" << std::endl;

}

template<typename K, typename V> bool testHashAtom(CHashTable<K, V>* p_hash, std::vector<std::pair<K, V>> stb, double& add_time, double& find_time, double& erase_time)
{
  bool res(true);
  double add_time_start = wtime();
  for (std::vector<std::pair<K, V>>::iterator it = stb.begin(); it != stb.end(); it++)
    p_hash->add(it->first, it->second);

  double add_time_end = wtime();
  add_time = add_time_end - add_time_start;

  double find_time_start = wtime();
  for (std::vector<std::pair<K, V>>::iterator it = stb.begin(); it != stb.end(); it++)
    res = res && p_hash->isFind(it->first);

  double find_time_end = wtime();
  find_time = find_time_end - find_time_start;


  double erase_time_start = wtime();
  for (std::vector<std::pair<K, V>>::iterator it = stb.begin(); it != stb.end(); it++)
    res = res && p_hash->erase(it->first);

  double erase_time_end = wtime();
  erase_time = erase_time_end - erase_time_start;

  return res;
}

bool testHash(std::vector<StudentBase>& stb, HashType type_hash_f)
{
  bool res(true);
  CHashTable<int, StudentBase> hash_int(type_hash_f);
  std::vector<std::pair<int, StudentBase>> st_by_age;
  for (std::vector<StudentBase>::iterator it = stb.begin(); it != stb.end(); it++)
    st_by_age.push_back(std::pair<int, StudentBase>(it->age, *it));

  double add_time(0.);
  double find_time(0.);
  double erase_time(0.);

  std::cout << "**********************CHashTable<int, StudentByAge>*****************************\n";
  if (testHashAtom<int, StudentBase>(&hash_int, st_by_age, add_time, find_time, erase_time))
    std::cout << "Test HashTable was successed: Add Time: " << add_time << " ,Find Time: " << find_time  << " ,Erase Time: " << erase_time << std::endl;
  else
    std::cout << "Test HashTable tree was failed: ERROR in find test or erase test" << std::endl;

  CHashTable<std::string, StudentBase> hash_string(type_hash_f);
  std::vector<std::pair<std::string, StudentBase>> st_by_full_name;
  for (std::vector<StudentBase>::iterator it = stb.begin(); it != stb.end(); it++)
    st_by_full_name.push_back(std::pair<std::string, StudentBase>(it->full_name, *it));

  std::cout << "**********************CHashTable<std::string, StudentBase>*****************************\n";
  if (testHashAtom<std::string, StudentBase>(&hash_string, st_by_full_name, add_time, find_time, erase_time))
    std::cout << "Test HashTable was successed: Add Time: " << add_time << " ,Find Time: " << find_time  << " ,Erase Time: " << erase_time << std::endl;
  else
    std::cout << "Test HashTable tree was failed: ERROR in find test or erase test" << std::endl;

  CHashTable<double, StudentBase> hash_double(type_hash_f);
  std::vector<std::pair<double, StudentBase>> st_by_score;
  for (std::vector<StudentBase>::iterator it = stb.begin(); it != stb.end(); it++)
    st_by_score.push_back(std::pair<double, StudentBase>(it->score, *it));

  std::cout << "**********************CHashTable<double, StudentBase>*****************************\n";
  if (testHashAtom<double, StudentBase>(&hash_double, st_by_score, add_time, find_time, erase_time))
    std::cout << "Test HashTable was successed: Add Time: " << add_time << " ,Find Time: " << find_time  << " ,Erase Time: " << erase_time << std::endl;
  else
    std::cout << "Test HashTable tree was failed: ERROR in find test or erase test" << std::endl;

  CHashTable<long long, StudentBase> hash_long(type_hash_f);
  std::vector<std::pair<long long, StudentBase>> st_by_phone_number;
  for (std::vector<StudentBase>::iterator it = stb.begin(); it != stb.end(); it++)
    st_by_phone_number.push_back(std::pair<long long, StudentBase>(it->number_phone, *it));

  std::cout << "**********************CHashTable<long, StudentBase>*****************************\n";
  if (testHashAtom<long long, StudentBase>(&hash_long, st_by_phone_number, add_time, find_time, erase_time))
    std::cout << "Test HashTable was successed: Add Time: " << add_time << " ,Find Time: " << find_time  << " ,Erase Time: " << erase_time << std::endl;
  else
    std::cout << "Test HashTable tree was failed: ERROR in find test or erase test" << std::endl;

  return res;
}

void CTestAll()
{
  unsigned int n_elements(1000);
  std::cout << "Enter amount of elements for test all:" << std::endl;
  std::cin >> n_elements;
  if (n_elements < 1)
  {
    std::cout << "Incorrect amount of elements for test all is " << n_elements << std::endl;
    return;
  }
  std::cout << "Amount of elements for test all is " << n_elements << std::endl;

  std::cout << "Generete test elements: " << std::endl;
  //массив в случайном порядке
  std::vector<StudentBase> rand_vec = generateRandomStudents(n_elements);

  std::cout << "**********************StudentByAge*****************************\n";
  testSorted<StudentByAge>(rand_vec);
  std::cout << "**********************StudentByFullName*****************************\n";
  testSorted<StudentByFullName>(rand_vec);
  std::cout << "**********************StudentByScore*****************************\n";
  testSorted<StudentByScore>(rand_vec);
  std::cout << "**********************StudentByPhoneNumber*****************************\n";
  testSorted<StudentByPhoneNumber>(rand_vec);

  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!HashType::Additive!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
  testHash(rand_vec, HashType::Additive);
  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!HashType::AdditiveModified!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
  testHash(rand_vec, HashType::AdditiveModified);
  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!HashType::XorPearson!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
  testHash(rand_vec, HashType::XorPearson);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

enum COMMANDS 
{ 
  Add = 0, 
  Find = 1, 
  Erase = 2, 
  Clear = 3, 
  Sort = 4, 
  GetSubSeq = 5, 
  Test = 6,
  Help = 7,
  View = 8,
  End = 9
};


void CClear(IDictionary<int, int>* p_data)
{
  std::cout << "Clear:" << std::endl;
  p_data->clear();
}

void CErase(IDictionary<int, int>* p_data)
{
  int inx(-1);
  std::cout << "Erase: <int> item's index param" << std::endl;
  std::cin >> inx;
  std::cout << "Result: ";
  if (p_data->erase(inx))
    std::cout << "True\n";
  else
    std::cout << "False\n";
}

void CFind(IDictionary<int, int>* p_data)
{
  int key(-1);
  std::cout << "Find: <int> key for finding" << std::endl;
  std::cin >> key;
  std::cout << "Result: ";
  if (p_data->isFind(key))
    std::cout << "True\n";
  else
    std::cout << "False\n";
}

void CAdd(IDictionary<int, int>* p_data)
{
  int value;
  std::cout << "Add: <int> key, value" << std::endl;
  std::cin >> value;
  p_data->add(value, value);
}


void CSort(IDictionary<int, int>* p_data)
{
  if (p_data->getType() == DictionaryType::AVL)
  {
    AVLTree<int, int>* avl = dynamic_cast<AVLTree<int, int>*>(p_data);
    std::vector<int> sorted_arr = avl->getSortedArray();
    std::cout << "Sorted array:\n";
    for (std::vector<int>::iterator i = sorted_arr.begin(); i != sorted_arr.end(); i++)
      std::cout << *i << ", ";

    std::cout << "\n";
  }
  else
    std::cout << "Only for AVL Type!\n";
}

void CGetSubSeq(IDictionary<int, int>*& p_data)
{
  if (p_data->getType() == DictionaryType::AVL)
  {
    int inx_begin(-1), inx_end(-1);
    std::cout << "GetSubSeq: <int> is first index of sub_seq, <int> is last index of sub_seq" << std::endl;
    std::cin >> inx_begin;
    std::cin >> inx_end;

    AVLTree<int, int>* avl = dynamic_cast<AVLTree<int, int>*>(p_data);
    AVLTree<int, int>* sub_avl = new AVLTree<int, int>;
    avl->getSubSeq(sub_avl, inx_begin, inx_end);
    avl->clear();
    delete avl;
    p_data = sub_avl;

  }
  else
    std::cout << "Only for AVL Type!\n";
}

void CHelp()
{
  std::string str_command[10] = 
  { 
    "Add",
    "Find",
    "Erase",
    "Clear",
    "Sort",
    "GetSubSeq",
    "Test",
    "Help",
    "View",
    "End"
  };
  for (int i(0); i < 10; i++)
    std::cout << "Command numer <" << i << "> is func " << str_command[i] << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  DictionaryType type(DictionaryType::No);
  IDictionary<int, int>* p_data = nullptr;
  while (type == No)
  {
    int int_type(0);
    std::cout << "Enter type of Data Structure: 1 - AVL, 2 - Hash; " << std::endl;
    std::cin >> int_type;
    switch (int_type)
    {
    case 1:
      type = DictionaryType::AVL;
      std::cout << "AVL" << std::endl;;
      break;
    case 2:
      type = DictionaryType::HashTable;
      std::cout << "HashTable" << std::endl;;
      break;
    default:
      std::cout << "Unknown Type!" << std::endl;;
      break;
    }
    std::cout << std::endl;
  }
  if (type == DictionaryType::AVL)
  {
    p_data = new AVLTree<int, int>;
  }
  else if (type ==  DictionaryType::HashTable)
  {
     p_data = new CHashTable<int, int>(HashType::XorPearson);
  }
  bool isEnd(false);
  CHelp();
  while(!isEnd)
  {
    int inx_command(-1);
    std::cin >> inx_command;
    COMMANDS command = (COMMANDS)inx_command;
    switch (command)
    {
      case Add:
        CAdd(p_data);
        break;
      case Find:
        CFind(p_data);
        break;
      case Erase:
        CErase(p_data);
        break;
      case Clear:
        CClear(p_data);
        break;
      case Sort:
        CSort(p_data);
        break;
      case GetSubSeq:
        CGetSubSeq(p_data);
        break;
      case Test:
        CTestAll();
        break;
      case Help:
        CHelp();
        break;
      case View:
        if (p_data != nullptr)
        {
          if (type == DictionaryType::AVL)
            std::cout << *dynamic_cast<AVLTree<int, int>*>(p_data);
          else if (type == DictionaryType::HashTable)
            std::cout << *dynamic_cast<CHashTable<int, int>*>(p_data);
        }
        break;
      case End:
        isEnd = true;
        break;
      default:
        std::cout << "Unknown command! Delete brain\n";
        break;
    }
  }

  if (p_data != nullptr)
  {
    if (type == DictionaryType::AVL)
      delete dynamic_cast<AVLTree<int, int>*>(p_data);
    else if (type == DictionaryType::HashTable)
      delete dynamic_cast<CHashTable<int, int>*>(p_data);
  }

  int a(0);
  std::cin >> a;

  return 0;
}

void main33()
{
  CTestAll();
  StudentByAge st;
  st.full_name = "FIO";
  st.age = 22;
  st.score = 3;
  st.number_phone = 123456;
  //cout << st;

  AVLTree<StudentByAge, StudentByAge> iavl;

  CHashTable<int, StudentByAge> hash/*(HashType::AdditiveModified)*/;
  vector<unsigned int> vals;

  int dcd(0);

  
  for (unsigned int i(0); i < 7; i++)
  {
    int age = rand()%1000;

    st.age = age;
    iavl.addValue(st);
    
  }
  std::cout << iavl;
  std::vector<StudentByAge> students = iavl.getSortedArray();
  for (std::vector<StudentByAge>::iterator it = students.begin(); it != students.end(); it++)
    cout << it->age << ", ";

  cout << endl;

  AVLTree<StudentByFullName, StudentByFullName> stree;
  std::cout << "Generate random students"  << endl;
  double start = wtime();
  std::vector<StudentBase> stb_vec = generateRandomStudents(30);
  double end = wtime();
  std::cout << "Generated " << stb_vec.size() << " students. It's time is " << end - start << endl;
//   int y(0);
//   std::cin >> y;
  CHashTable<StudentBase, StudentBase> hash_st_by_st;
  for (unsigned int i(0); i < stb_vec.size(); i++)
  {
    hash_st_by_st.add(stb_vec[i], stb_vec[i]);
    StudentByFullName stfn = convertStudent<StudentByFullName>(stb_vec[i]);
    stree.addValue(stfn);
  }

  for (unsigned int i(0); i < stb_vec.size(); i++)
  {
    bool what = hash_st_by_st.isFind(stb_vec[i]);
    int y(0);
  }

  std::cout << stree;
  
  
  st.age = 5;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 45;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 463;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 32;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 28;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 1;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 31;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 34;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 333;
  iavl.addValue(st);
  std::cout << iavl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 332;
  iavl.addValue(st);
  std::cout << iavl;
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 500;
  iavl.addValue(st);
  hash.add(st.age, st);

  st.age = 1900;
  iavl.addValue(st);
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 4000;
  iavl.addValue(st);
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  st.age = 0;
  iavl.addValue(st);
  hash.add(st.age, st);
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;

  
  std::cout << iavl;
  st.age = 32;
  iavl.erase(st);
  std::cout << iavl;
  //std::cout << hash;
  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  std::vector<int> ages;
  for (int i(0); i < 1000000; i++)
  {
    st.age = rand() * 10 + rand() % 10;
      if (!hash.isFind(st.age))
      {
        hash.add(st.age, st);
        ages.push_back(st.age);
      }
  }
  for (std::vector<int>::iterator it = ages.begin(); it != ages.end(); it++)
  { 
    std::vector<int>::iterator jt = it;
    if (++jt == ages.end())
      int yui = 0;

    if (!hash.erase(*it))
      cout << "Key: " << *it << " is not found" << endl;
  }

  std::cout << hash;
  std::cout << " collisions " << hash.getNumberCollisions() << " size " << hash.getCapacity() << std::endl;
  TNode<StudentByAge, StudentByAge>*pEl = iavl.findNode( st );
  /*std::cout << pEl;*/
  int N = iavl.getCount();
  TNode<StudentByAge, StudentByAge>* pRoot = iavl.findNode(st);
  iavl.erase(st);
  
}