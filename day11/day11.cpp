
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <execution>
#include <chrono>
#include <sstream>

using NUMTYPE = unsigned long long;
using MAPTYPE=std::map<std::string, NUMTYPE>;

MAPTYPE g_freq_map_i;
MAPTYPE g_freq_map_o;

std::string g_puzzle_file= "puzzle_input.txt";
std::mutex g_mutex;


void init_stones_from_file(std::list<std::string>& f_stones){
  std::ifstream iFile;
  iFile.open(g_puzzle_file);
  if(!iFile.is_open()){
    std::cout << "\nFAILED to read from file: \"" << g_puzzle_file << "\"\n";
    throw;
  }
  std::string str;
  NUMTYPE n_stones=0;
  while(iFile>>str)
  {
    f_stones.emplace_back(str);
    ++n_stones;
  }
  std::cout << "read " << n_stones << " stones from puzzle file.\n";
}


void add_to_map(const std::string& str, MAPTYPE& c_freq_map)
{
  auto iter = c_freq_map.find(str);
  if(iter != c_freq_map.end())
  {
    iter->second+=1;
  }
  else{
    c_freq_map.insert({str,1});
  }
}


void set_io_map()
{
  if(!g_freq_map_o.empty()){
    g_freq_map_i.clear();
    g_freq_map_i = g_freq_map_o;
  }
  g_freq_map_o.clear();
}


void add_freq_to_map_for_str(const NUMTYPE freq, const std::string& str, MAPTYPE& l_freq_map)
{
  MAPTYPE::iterator found_i = l_freq_map.find(str);
  if(found_i != l_freq_map.end()){
    found_i->second += freq;
  }
  else{
    l_freq_map.insert({str, freq});
  }
}


void str_time_spent(const std::chrono::time_point<std::chrono::high_resolution_clock>& start, std::stringstream& sstrm)
{
  auto end = std::chrono::high_resolution_clock::now();
  auto tim = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::string unit = " ms";
  if(tim>3600000) // hour
  {
      tim = std::chrono::duration_cast<std::chrono::hours>(end - start).count();
      unit = " hr";
  }
  else if(tim>60000) // min
  {
      tim = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
      unit = " min";
  }
  else if(tim>1000)
  {
      tim = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      unit = " sec";
  }
  sstrm << tim << unit;
}


void do_stones_blink(const int blinkCount)
{
  std::string str_stones;

  std::list<std::string> file_stones;
  init_stones_from_file(file_stones);

  // prepare map (input) for first blink
  for(const auto& s : file_stones)
    add_to_map(s, g_freq_map_i);

  NUMTYPE l_largest_stone;

  for(int i=1; i<=blinkCount; ++i)
  {
    // set input map (may need swapping of output map)
    set_io_map();
    
    str_stones.clear();
    std::cout <<"\t Blink " << i << ": ";
    auto start = std::chrono::high_resolution_clock::now();

    for_each(
    std::execution::par,
    g_freq_map_i.begin(),
    g_freq_map_i.end(),
    [&](const std::pair<std::string, NUMTYPE>& stones_freq){
      std::lock_guard<std::mutex> guard(g_mutex);
      if(stones_freq.first=="0"){
        add_freq_to_map_for_str(stones_freq.second, "1", g_freq_map_o);
      }
      else if(stones_freq.first.size()%2==0){
        size_t mid = stones_freq.first.size()/2;
        std::string p1 = stones_freq.first.substr(0,mid);
        std::string p2 = stones_freq.first.substr(mid);
        add_freq_to_map_for_str(stones_freq.second, p1, g_freq_map_o);
        add_freq_to_map_for_str(stones_freq.second, std::to_string(std::stoll(p2)), g_freq_map_o);
      }
      else{
        /*std::string str_mul_2024=stones_freq.first;
        int i=2024;--i;
        while(i-->0){
          str_mul_2024 = add_two_strings(stones_freq.first, str_mul_2024, std::string()); 
        }
        add_freq_to_map_for_str(stones_freq.second, str_mul_2024, g_freq_map_o);*/
        NUMTYPE new_num = std::stoll(stones_freq.first)*2024;
        std::string new_num_str = std::to_string(std::stoll(stones_freq.first)*2024);
        add_freq_to_map_for_str(stones_freq.second, new_num_str, g_freq_map_o);
        
        // check for largest
        /*size_t size1 = stones_freq.first.size();
        size_t size2 = g_largest_num.size();
        bool is_big_size = size1>size2;
        bool is_same = !is_big_size && size1==size2;
        int first1 = stones_freq.first[0] - '0';
        int first2 = g_largest_num[0] - '0';
        bool is_1_big = is_same && stones_freq.first!="0" && first1>first2;
        if(is_big_size || is_1_big){
          g_largest_num = new_num_str;
        }*/
        if(new_num > l_largest_stone)
          l_largest_stone = new_num;
      }
    });

    unsigned long long l_stones_count=0;
    for(auto& iter : g_freq_map_o){
      l_stones_count += iter.second;
    }

    /*if(i==1 || i==2){
      std::cout << "\n DUMPING STONES..." << "\n";
      std::string err;
      std::list<std::string> numstr_list;
      for(const auto itr : g_freq_map_o)
      {
        int i = itr.second;
        while(i-->0)
          numstr_list.emplace_back(itr.first);
      }
      numstr_list.sort( 
      [](const std::string& s1, const std::string& s2)              {
        return std::stoll(s1) < std::stoll(s2);
      });
      if(!dump_strlist_to_filepath({numstr_list},"blink" + std::to_string(i)+"_stones.txt",err))
        std::cout << "Failed to obtain blink 4 output!!!";
    }*/

    std::stringstream s; 
    str_time_spent(start, s);
    std::cout << " completed in " << s.str()
              << " and largest num of blink \"" << l_largest_stone << "\""
              << " and stones: " << l_stones_count << "\n";
  }
}

int main(int argc, char* argv[])
{
  int blink_count = 4;
  if(argc>=2)
    blink_count = std::stoll(argv[1]);
  do_stones_blink(blink_count);
  return 0;
}
