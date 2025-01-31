#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

// The given file is read and the extracted lines are stored in the passed vector/container 
bool read_lines(const std::string& file_path, std::vector<std::string>& vec)
{
  std::ifstream i_handle(file_path);

  if(!i_handle.is_open())
    return false;

  // read the characters in the istream object using the string object temp
  // and use the "str1" sting object to create the elements to be added to the container
  std::string temp;
  std::string str1;
  while(i_handle >> temp)
  {
    if(!str1.empty() && temp.find(":") != std::string::npos)
    {
      // if the characters read from stream has the char ":" then
      // push that to container; add the chars in temp (just read from stream) to str1
      vec.emplace_back(str1);
      str1 = temp;
    }
    else
    {
      // if not the case above then keep adding the chars read from stream i.e. temp
      // to the "str1" object
      str1 += " " + temp;
    }
  }

  // if characters read from the stream are still there in the
  // string object then insert that to the vector
  if(!str1.empty())
  {
    vec.emplace_back(str1);
  }

  return !vec.empty(); // returns TRUE if the container is filled
}


using NUMBERTYPE=long long;
using INDATATYPE=std::pair<NUMBERTYPE, std::vector<NUMBERTYPE>>;
using INDATALIST=std::list<INDATATYPE>;

const NUMBERTYPE get_number_from_string(const std::string& str1)
{
  return std::stoll(str1);
}

void extract_input_from_lines(
  const std::vector<std::string> lines,
  INDATALIST& in_data)
{
  NUMBERTYPE f1;
  std::vector<NUMBERTYPE> t_data;
  for(const std::string& line : lines)
  {
    auto temp = line;
    auto pos = temp.find(":");
    const auto p1 = temp.substr(0, pos);
    f1 = get_number_from_string(p1); std::cout << "\n adding f1: " << p1<< "\n";
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
      t_data.push_back(get_number_from_string(pT)); std::cout << ";  " << pT;
      temp = temp.substr(pos+1);
    }
    if(!temp.empty() && get_number_from_string(temp) != 0)
    {
      t_data.push_back(get_number_from_string(temp)); std::cout << "; " << temp;
    }
    in_data.push_back({f1, t_data});
    t_data.clear();
    std::cout << "; done adding...\n";
  }
}


struct PorcessingTree
{
  /*
   This class represents a tree structure to store the results all the 
   results possible from the various operations to be done on the various
   int data that are given as the operands.
   For example: In the line "3267: 81 40 27"
   the operands are "81" , "40" and "27"
   We have two types of operators: ADD and MUL
   so if have to build the tree with all possible values that the operations
   of these operators on these operands can generate then the tree (binary, balance)
   will look like this
                               
                             [81]      <----------------------- layer-0
                             __|___
                        +40/       \x40
                       [121]       [3240]   <------------------ layer-1
                       _|_           __|___
                   +27/   \x27   +27/      \x27
                  [148]  [3267]  [3267]  [87480]  <------------ layer-2

   The last level in the above tree shows all possible solutions that can be produced
   all the tree operands using either ADD(+) and/or MUL(x) operators
   The desired result is "3267" and it is indeed one of the elements in the last layer
   Therefore, it is possible to obtain the desirted-result and the given operands.
   So, we can create such a tree and compare the desired reult with all the elements in
   the last layer and if a match is found then the possibility is YES else No.
    (possiblity here means the possibility of obtaining the desired result using the operands)
   */

   private:

   int _layer_idx = 0; // number of layers (0 be default)
   std::vector<NUMBERTYPE> _tree; // the entire tree is stored a linear vector

   public:
   
   PorcessingTree(){}
   ~PorcessingTree(){}


   // view last-layer of tree (returns onst ref)
   // used to add new elemnt (create new nodes ) 
   const std::vector<NUMBERTYPE> view_last_layer_of_tree() const
   {
     std::vector<NUMBERTYPE> ans;

     // count the number of elements in the last layer
     // since its a binary tree the elements count will be power of 2  
     // It is raised to the power as the layer-index i.e. _layer_idx
     int nElements = 1;
     int iPow2 = _layer_idx;
     while(iPow2>0)
     {
       nElements = nElements * 2;
       --iPow2;
     }

     if(_tree.empty())
     {
       // if tree is empty then there is no layer and return empty vector
       return ans;
     }
     else
     {
       //std::cout << "\n Inserting in layer : " << _layer_idx;
       // else; read the last layer i.e. from 0 till the number of elements in last layer
       // since done in reverse, perform reverse() operation on vector to obtain correct order
       auto itr = std::prev(_tree.end());
       for(int i=0; i<nElements; ++i, --itr)
         ans.push_back(*itr);
      std::reverse(ans.begin(), ans.end());
      return ans;
     }
   }

  // views the last layer and then prints it
   void print_last_layer_tree() const
   {
     auto data = view_last_layer_of_tree();
     std::cout << "\nPrinting last layer: ";
     for(const auto i: data)
     {
       std::cout << " " << i;
     }
     std::cout << "\n";
   }


   // the insert operation adds a new layer to the tree
   // by adding two new nodes for each element in last layer
   // So currently tree is at layer-1 (see diagram) then this layer has 2 nodes
   // In this case, insert operation will add 2 nodes to left-node and two to right-node
   void insert_in_tree(NUMBERTYPE in)
   {
     if(_tree.empty())
     {
       _tree.push_back(in);
     }
     else
     {
       const std::vector<NUMBERTYPE>& last_layer_data = this->view_last_layer_of_tree();
       std::vector<NUMBERTYPE> new_layer;
       for (const auto d : last_layer_data)
       {
         new_layer.push_back(d + in);
         new_layer.push_back(d * in);
       }
       std::copy(new_layer.begin(), new_layer.end(), std::back_inserter(_tree));
       ++_layer_idx;// very important stpe; incrmeent layer index
     }
   }

   // to be only called when all elements are inserted in the tree
   bool is_result_in_tree(const NUMBERTYPE result)
   {
     bool answer = false;
     const std::vector<NUMBERTYPE>& last_data = this->view_last_layer_of_tree();
     for (const auto d : last_data)
     {
       if(d == result)
       {
         answer = true;
         break;
       }
     }
     return answer;
   }
};

/*const NUMBERTYPE sum_of_numbers(const std::vector<NUMBERTYPE>& nums)
{
  NUMBERTYPE sum = 0;
  for(const NUMBERTYPE n : nums)
    sum += n;
  return sum;
}
*/

int main(int argc, char* argv[])
{
  auto file_p="../puzzle_input.txt";
  //auto file_p="../sample_input.txt";

  std::vector<std::string> lines;
  if(!read_lines(file_p, lines))
  {
    std::cout <<"\nFailed to read file: " << file_p << "\n";
    return -1;
  }
  else{
    std::cout << "Done reading input file ....\n";
  }
  
  for(const auto& l : lines)
    std::cout << l << "\n";

  INDATALIST in_data;
  extract_input_from_lines(lines, in_data);
  std::cout << "\n\n";

  NUMBERTYPE resultingSum = 0;

  for(const auto in : in_data)
  {
    PorcessingTree pTree;

    for(const auto& in : in.second)
    {
      pTree.insert_in_tree(in);
      pTree.print_last_layer_tree();
    }
    std::string ans ="No";
    if(pTree.is_result_in_tree(in.first))
    {
      resultingSum+= in.first;
      ans = "Yes";
    }
    std::cout << "\n" << ans << "Result for \"" 
              << in.first << "\" is: "  << ans;
    std::cout << "\n" << "\n Current solution: " << resultingSum <<"\n\n";

  }

  return 0;
}
