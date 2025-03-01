
#include <cmath>
#include <limits>
#include <map>

#include "day11part2.h"

using MAPTYPE=std::map<std::string, size_t>;

MAPTYPE g_freq_map_i;
MAPTYPE g_freq_map_o;

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


std::string g_puzzle_file= "puzzle_input.txt";
std::mutex g_mutex;

std::string g_stones_count ="0";


void init_stones_from_file(std::list<std::string>& f_stones){
  std::ifstream iFile;
  iFile.open(g_puzzle_file);
  if(!iFile.is_open()){
    std::cout << "\nFAILED to read from file: \"" << g_puzzle_file << "\"\n";
  }
  std::string str;
  size_t n_stones=0;
  while(iFile>>str)
  {
    f_stones.emplace_back(str);
    ++n_stones;
  }
  std::cout << "read " << n_stones << " stones from puzzle file.";
}


void do_stones_count()
{
  g_stones_count.clear();
  std::string err;
  int idx=0;
  for(auto& iter : g_freq_map_o){
    if(idx++==0)
      g_stones_count = std::to_string(iter.second);
    else
      g_stones_count = add_two_strings(g_stones_count, std::to_string(iter.second), err);
    //std::cout << "\"" <<  iter.first <<"\" -> " << iter.second << "\n"; 
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


void apply_rules_to_stone(
  const std::string& istone,
  std::list<std::string>& lst_str)
{
  if(istone=="0"){
    // apply RULE 1:
    //std::cout << "\n.. apply rule 1 ...";
    lst_str.emplace_back("1");
  }
  else if(istone.size()%2==0){
    //std::cout << "\n.. apply rule 2 ...";
    // apply RULE 2try
    size_t pos_mid = istone.size()/2;
    try{
      lst_str.emplace_back(istone.substr(0, pos_mid));
    }catch(...){
    std::cout << "\n Failed to break and create part-1 when applying rule-2 to "<< istone;
    }
    try{
      lst_str.emplace_back(std::to_string(std::stoll(istone.substr(pos_mid))));
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
      val_stone_str = std::to_string(std::stoll(istone)*2024);
    }catch(...){
      std::cout << "\n Failed when applying rule-3 to .. during std::to_string to "<< istone;
    }
    lst_str.emplace_back(val_stone_str);
  }
}
 
 
void do_stones_blink(const int blinkCount)
{
  std::string str_stones;

  std::list<std::string> file_stones;
  init_stones_from_file(file_stones);

  // prepare map (input) for first blink
  for(const auto& s : file_stones)
    add_to_map(s, g_freq_map_i);

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
    [&](const std::pair<std::string, size_t>& stones_freq){
      // take the first element i.e. stone and apply rules to generate string-list
      std::list<std::string> o_str;
      apply_rules_to_stone(stones_freq.first, o_str);
      // acquire lock and then insert the generated string-list to o_map
      std::lock_guard<std::mutex> guard(g_mutex);
        for(size_t i=0; i<stones_freq.second; ++i){
          for(const auto& o_s : o_str){
            add_to_map(o_s, g_freq_map_o);
          }
        }
    });
    
    /*for(const auto& stones : g_in_data){
      for(const auto& stone : stones){
          do_read_stone(stone, g_mutex);
      }
    }*/

    do_stones_count();

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
    std::cout << " completed in " << s.str() << " and stones: " << g_stones_count << "\n";
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
