
#include<cmath>
#include <limits>

#include "helper_day11_part2.h"

size_t maxvalue = std::numeric_limits<size_t>::max();

int get_digits_count(size_t iNum){
  int i =0;
  size_t num = iNum;
  while(num>0)
  {
    ++i;
    num = (size_t)(num/10);
    //std::cout << num << " ";
  }
  return i;
}
bool is_even_digits(const size_t iNum){
  return get_digits_count(iNum)%2==0;
}

std::list<std::string> g_loc_lst_file, g_loca_lst1, g_loca_lst2;
std::list<std::list<std::string>> g_a_data, g_b_data;
std::list<std::list<std::string>> g_in_data;
std::list<std::list<std::string>> g_out_data;
std::string iFileName= "puzzle_input.txt";
const size_t g_lst_lst_limit = 10000;// 40 mil
std::mutex g_mutex;

std::string stones_count()
{
  std::string err;
  std::string sc="0";
  for(const auto& s : g_out_data){
    try{
      sc = add_two_strings(sc, std::to_string(s.size()), err);
    }
    catch(...){
      std::cout <<"\n Failed when counting stones ... last value " << sc << "\n";
    }
  }
  return sc;
}

void do_pre_blink(const int blinkIdx){
  static bool flag=true;
  if(blinkIdx>1){
    g_in_data.clear();
    g_in_data = g_out_data;
  }
  g_out_data.clear();
  //std::cout << " input size: " << g_in_data.front().size() << "\n";
  if(blinkIdx==1)
  {
    std::ifstream iFile; iFile.open(iFileName);
    if(!iFile.is_open()){
      std::cout << "\nFAiled to read from file: \"" << iFileName << "\"\n";
    }
    std::string str;
    size_t n_stones=0;
    while(iFile>>str)
    {
      g_loc_lst_file.emplace_back(str);
      ++n_stones;
    }
    std::cout << "read " << n_stones << " stones...";
    g_in_data.emplace_back(g_loc_lst_file);
    g_loc_lst_file.clear();
    iFile.close();
  }
}

void string_mul_2024(const std::string& str1, std::string& str)
{
  str = "0";
  std::string err;
  int m_1024 = 2024/2;
  while(m_1024-->0)
  {
    str = add_two_strings(str, add_two_strings(str1, str1, err), err);
  }
}

void do_read_stone(const std::string& istone, std::mutex& m, std::list<std::string>& c_loca_lst1, bool isEnd)
{
  //static std::list<std::string> c_loca_lst1;
  
  if(c_loca_lst1.size()>=g_lst_lst_limit  || isEnd){
    if(c_loca_lst1.size()>=g_lst_lst_limit)
      //std::cout <<"\n\t ...extending global output storage ...\n\t";
    std::lock_guard<std::mutex> guard(m);
    g_out_data.emplace_back(c_loca_lst1);
    c_loca_lst1.clear();
  }
  if(isEnd)
      return;
  if(istone=="0"){
    // apply RULE 1:
    //std::cout << "\n.. apply rule 1 ...";
    c_loca_lst1.emplace_back("1");
  }
  else if(istone.size()%2==0){
    //std::cout << "\n.. apply rule 2 ...";
    // apply RULE 2try
    try{
      c_loca_lst1.emplace_back(istone.substr(0, istone.size()/2));
    }catch(...){
    std::cout << "\n Failed to break and create part-1 when applying rule-2 to "<< istone;
    }
    try{
      c_loca_lst1.emplace_back(std::to_string(std::stoll(istone.substr(istone.size()/2))));
    }catch(...){
    std::cout << "\n Failed to break and create part-2 when applying rule-2 to "<< istone;
    }
  }
  else{
    // APPLY RULE 3
    //std::cout << "\n.. apply rule 3 ...";
    std::string val_stone_str;
    size_t val_stone_val;
    try{
      val_stone_val = std::stoll(istone);
    }catch(...){
      std::cout << "\n Failed when applying rule-3 .. during std::stoll to "<< istone;
    }
    try{
      val_stone_val = 2024*val_stone_val;
    }catch(...){
      std::cout << "\n Failed when applying rule-3 .. during 2024* "<< istone;
    }
    try{
      val_stone_str = std::to_string(val_stone_val);
    }catch(...){
      std::cout << "\n Failed when applying rule-3 to .. during std::to_string to "<< istone;
    }
    c_loca_lst1.emplace_back(val_stone_str);
  }
  
  //std::cout << "\n";
}
 
 
void do_stones_blink(const int blinkCount)
{
  std::string str_stones;

  for(int i=1; i<=blinkCount; ++i)
  {
    str_stones.clear();
    std::cout <<"\t Blink " << i << ": ";
    auto start = std::chrono::high_resolution_clock::now();

    do_pre_blink(i);

    for_each(
    std::execution::par,
    g_in_data.begin(),
    g_in_data.end(),
    [&](const std::list<std::string>& stones){
      std::list<std::string> loca_lst;
      for(const auto& stone : stones){
        do_read_stone(stone, g_mutex, loca_lst, false);
      }
      do_read_stone("", g_mutex, loca_lst, true);
    });
    
    /*for(const auto& stones : g_in_data){
      for(const auto& stone : stones){
          do_read_stone(stone, g_mutex);
      }
    }*/
    str_stones = stones_count();
    std::stringstream s; 
    str_time_spent(start, s);
    std::cout << " completed in " << s.str() << " and stones: " << str_stones << "\n";
  }
}

int main(int argc, char* argv[])
{
  int blink_count = 2;
  if(argc>=2)
    blink_count = std::stoll(argv[1]);
  do_stones_blink(blink_count);
  return 0;
}
