#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

bool read_lines(const std::string& file_path, std::vector<std::string>& vec)
{
  std::ifstream i_handle(file_path);

  if(!i_handle.is_open())
    return false;

  std::string temp;
  std::string str1;
  while(i_handle >> temp)
  {
    if(!str1.empty() && temp.find(":") != std::string::npos)
    {
      vec.emplace_back(str1);
      str1 = temp;
    }
    else
    {
      str1 += " " + temp;
    }
  }
  if(!str1.empty())
  {
    vec.emplace_back(str1);
  }

  return !vec.empty();
}

enum class OPTYPE
{
  ADD,
  MUL
};

using INPUTDTYPE=std::pair<int, std::vector<int>>;
using INPUTDLIST=std::list<INPUTDTYPE>;

void extract_input_from_lines(
  const std::vector<std::string> lines,
  INPUTDLIST& in_data)
{
  int f1;
  std::vector<int> t_data;
  for(const std::string& line : lines)
  {
    auto temp = line;
    auto pos = temp.find(":");
    const auto p1 = temp.substr(0, pos);
    f1 = std::stoi(p1); //std::cout << "\n adding f1: " << p1;
    temp = temp.substr(pos+1);
    pos = temp.find(" ");
    if(pos==0)
    {
      temp = temp.substr(pos+1);
    }
    while(std::string::npos != temp.find(" "))
    {
      pos = temp.find(" ");
      auto pT = temp.substr(0, pos);
      t_data.push_back(std::stoi(pT)); //std::cout << "; adding to list: " << pT;
      temp = temp.substr(pos+1);
    }
    if(!temp.empty() && std::stoi(temp) != 0)
    {
      t_data.push_back(std::stoi(temp)); //std::cout << "; adding to list: " << temp;
    }
    in_data.push_back({f1, t_data});
    t_data.clear();
  }
}

struct NUMOP_TYPE
{
  std::pair<OPTYPE, int> _op_num;
  int _idx = -1; // to distinguish the operands
  
  NUMOP_TYPE(){}
  NUMOP_TYPE(OPTYPE op, int num, int idx):_idx(idx){_op_num = {op, num};}

  const OPTYPE view_op() const {return _op_num.first;}
  const int view_num() const {return _op_num.second;}
};
using NUMOP_LIST=std::vector<NUMOP_TYPE>;
using SAMPLEVEC=std::vector<NUMOP_LIST>;


// this function create all possible combinations of the input+operator
// for the given list of inputs + list of oerators (+ and *)
bool create_all_possible(
  const std::vector<int> ot_num,
  std::vector<NUMOP_LIST>& all_candidates)
{
  NUMOP_LIST temp_lst;

  const std::vector<OPTYPE> opn_vec = {OPTYPE::ADD, OPTYPE::MUL};

  int idx = 1;
  for(const int on : ot_num)
  {
    for(const auto& op : opn_vec)
    {
      temp_lst.emplace_back(NUMOP_TYPE(op, on, idx++));
    }
  }
  auto t_num = ot_num.begin();
  NUMOP_LIST one_candidate;
  while(!temp_lst.empty())
  {
    auto iter = std::find_if(
                          temp_lst.begin(),
                          temp_lst.end(),
                          [&t_num](const NUMOP_TYPE& np){return np.view_num()==*t_num;});
   if(iter == temp_lst.end())
   {
     all_candidates.push_back(one_candidate);
     one_candidate.clear();
     ++t_num;
   }
   else
   {
     one_candidate.push_back(*iter);
     temp_lst.erase(iter++);
   }
  }
  
  return all_candidates.size() == ot_num.size();
}

int do_opn_on_nums(const NUMOP_LIST& opn_in_lst)
{
  if(opn_in_lst.size()<2)
    return -99;

  int ans=opn_in_lst[0].view_num();
  for(int idx = 1; idx<opn_in_lst.size(); ++idx)
  {
    auto op_i = opn_in_lst[idx];

    switch(op_i.view_op())
    {
      case OPTYPE::ADD: ans = ans + op_i.view_num(); break;
      case OPTYPE::MUL: ans = ans * op_i.view_num(); break;
      default: std::cout << "BAD INPUT FOR OPN";
    }
  }
  return ans;
}

int main()
{
  const auto file_p="../sample_input.txt";
  
  std::vector<std::string> lines;
  if(!read_lines(file_p, lines))
  {
    std::cout <<"\nFailed to read file: " << file_p << "\n";
    return -1;
  }
  else{
    std::cout << "Done reading input file ....\n";
  }
  
  /*for(const auto& l : lines)
    std::cout << l << "\n";*/

  INPUTDLIST in_data;
  extract_input_from_lines(lines, in_data);

  // uncomment for prining read dat
  /*std::cout << "\n\n\n";
  for(const auto in : in_data)
  {
    std::cout << in.first << ":";
    for (const auto s : in.second)
      std::cout << " " << s;
    std::cout << "\n";
  }*/

  for(const auto in : in_data)
  {
    std::vector<NUMOP_LIST> all_can;
    create_all_possible(in.second, all_can);
    for(const auto& ac : all_can)
    {
      const int allopnres = do_opn_on_nums(ac);
      if(in.first == allopnres)
      {
        std::cout << "\n Yes: " << in.first << " ";
      }
      else
      {
        std::cout << "\n No: " << in.first << " ";
      }
      std::cout << " - for all-op result : " << allopnres << "\n";
    }
  }

  return 0;
}
