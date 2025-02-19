#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<chrono>


using STRLST=std::list<std::string>;
using STRITR=std::list<std::string>::iterator;

bool apply_rule_1(STRITR& itr)
{
    //std::cout << "\n ... applying rule 1 ...";
    if (*itr == "0")
    {
        *itr = "1";
        //std::cout << " added " << *itr << ".. ; applied...\n\n";
        return true;
    }
    else
    {
        //std::cout << "; nope...";
        return false;
    }
}

// splits the num-string in two
bool apply_rule_2(STRITR& itr, STRLST& lst)
{
    //std::cout << "\n ... applying rule 2 to \"" << *itr << "\" ... ";

    if(itr->size()%2==0)
    {
        // find middle position 
        auto pos_mid = itr->size()/2; //std::cout<< "; mid pos : " << pos_mid;
        // obtain the later half of string
        auto str2 =itr->substr(pos_mid);
        str2 = std::to_string(std::stoll(str2));
        // make the current string first-half
        *itr=itr->substr(0, pos_mid);
        // find the later-half string insertion position
        auto itr_insert = std::next(itr);
        // insert the later-half string in the list
        lst.insert(itr_insert, str2);

        //std::cout << " ... created " << *itr << " and " << str2 << ".. ; applied...\n\n";
        return true;
    }
    else
    {
        //std::cout << "; nope...";
        return false;
    }
}

void apply_rule_3(STRITR& itr)
{
    //std::cout << "\n ... applying rule 3 ...";
    *itr = std::to_string(std::stoll(*itr)*2024);
    //std::cout << " added " << *itr << "..\n\n";
}


bool get_num_str_list(const std::string&  file_path, STRLST& str_lst)
{
    std::ifstream f_handle(file_path);

    if(!f_handle.is_open())
        return false;

    std::string num_str;
    while(f_handle >> num_str)
    {
        str_lst.emplace_back(num_str);
    }
    f_handle.close();

    return true;
}

void print_stones(const STRLST& strl, const int blink_count)
{
    std::cout << "\nAfter " << blink_count << " blinks, no. of stones : " << strl.size()
              << ", the string is  : \n";

    for(const auto s : strl)
    {
        std::cout << s << " " ;
    }
    std::cout << "\n";
}

int main(int argc, char** argv)
{
    int blink_count = 75;

    std::string file_p="puzzle_input.txt";
    //auto file_p="sample_2.txt";


    auto start_time = std::chrono::high_resolution_clock::now();


    // if user input given then use those
    bool is_given = true;
    int temp_blnk;
    std::string  temp_file;

    if(argc==3)
    {
        try
        {
            temp_blnk = std::stoi(argv[1]);
        }
        catch(...)
        {
            is_given = false;
            std::cout << "\nBad input for blink count\n";
        }

        try
        {
            temp_file = argv[2];
        }
        catch(...)
        {
            is_given = false;
            std::cout << "\nBad input for file path\n";
        }
    }

    if(is_given)
    {
        blink_count = temp_blnk;
        file_p = temp_file;
    }

    STRLST numstr_list;
    if(!get_num_str_list(file_p, numstr_list))
    {
        std::cout << "\n Failed to parse the lines in file: \"" << file_p << "\"\n";
    }

    std::cout << "\nGiven string:";
    for(const auto s : numstr_list)
    {
        std::cout << s << " " ;
    }
    std::cout << "\n";

    int idx_counter = 0;

    while(idx_counter++ < blink_count)
    {
        for(auto itr = numstr_list.begin(); itr != numstr_list.end(); ++itr)
        {
            if(apply_rule_1(itr))
            {
                continue;
            }
            else if(apply_rule_2(itr, numstr_list))
            {
                ++itr;
                continue;
            }
            else
            {
                apply_rule_3(itr);
                //std::cout << "\n";
            }
        }
        print_stones(numstr_list, idx_counter);
    }


    //print_stones(numstr_list, blink_count);


    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\nTime taken: " << duration << " ms\n";

    return 0;
}
