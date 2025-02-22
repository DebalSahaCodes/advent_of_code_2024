#include<iostream>
#include<fstream>
#include<string>
#include<string>
#include<list>
#include<vector>
#include<chrono>
#include<algorithm>

using STRLST=std::list<std::string>;
using STRITR=std::list<std::string>::iterator;

bool apply_rule_1(STRITR& itr)
{
    try
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
    catch(...)
    {
        std::cout <<"\n FAIL LOG: to apply rule 1 to \"" << *itr << "\" .....\n";
        throw;
    }
}

// splits the num-string in two
bool apply_rule_2(STRITR& itr, STRLST& lst)
{
    //std::cout << "\n ... applying rule 2 to \"" << *itr << "\" ... ";

    try
    {
        auto itrbkup = *itr;

        if(itr->size()%2==0)
        {
            // find middle position 
            size_t pos_mid = itr->size()/2; //std::cout<< "; mid pos : " << pos_mid;
            // obtain the later half of string
            std::string str2;
            try
            {
                str2 =itr->substr(pos_mid);
                str2 = std::to_string(std::stoll(str2));
            }
            catch(...)
            {
                std::cout << "\n Failed to create string part-2 using pos "
                          << pos_mid << " from \"" << itrbkup << "\" ; ";
            }
            // make the current string first-half
            try
            {
                *itr=itr->substr(0, pos_mid);
            }
            catch(...)
            {
                std::cout << "\n Failed to create string part-1 using pos "
                          << pos_mid << " from \"" << *itr << "\" ; ";
            }
            try
            {
                if(itr != std::prev(lst.end()))
                {
                    // find the later-half string insertion position
                    auto itr_insert = std::next(itr);
                    // insert the later-half string in the list
                    lst.insert(itr_insert, str2);
                }
                else
                {
                    lst.emplace_back(str2);
                }
            }
            catch(...)
            {
                std::ptrdiff_t itrpos = std::distance(lst.begin(), std::next(itr));
                std::cout << "\n Failed to insert \"" << str2 
                          << "\" at pos " << itrpos 
                          << " i.e. after part-1-str \"" << *itr
                          << "\" of given itr \"" << itrbkup << "\" ..";
                throw;
            }
        
            //std::cout << " ... created " << *itr << " and " << str2 << ".. ; applied...\n\n";
            return true;
        }
        else
        {
            //std::cout << "; nope...";
            return false;
        }
    }
    catch(...)
    {
        std::cout <<"\n FAIL LOG: to apply rule 2 to \"" << *itr << "\" .....\n";
        throw;
    }
}

void apply_rule_3(STRITR& itr)
{
    try
    {
        //std::cout << "\n ... applying rule 3 ...";
        *itr = std::to_string(std::stoll(*itr)*2024);
        //std::cout << " added " << *itr << "..\n\n";
    }
    catch(...)
    {
        std::cout <<"\n FAIL LOG: to apply rule 3 to \"" << *itr << "\" .....\n";
        throw;
    }
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

void print_stones(const STRLST& strl, const int blink_count, bool is_dump=false)
{
    std::cout << "\nAfter " << blink_count << " blinks, no. of stones : " << strl.size()
              << ", the string is  : \n";

    const std::string sep_str = (is_dump?"\n":" " );

    if(!is_dump)
    {
        for(const auto s : strl)
        {
            std::cout << s << sep_str;
        }
        std::cout << "\n";
    }
    else
    {
        const std::string dump_file_name = "dump_file_at_blink_" + std::to_string(blink_count) + ".txt";
        std::cout << "\n Dumping the last stones string to \"" << dump_file_name << "\"";

        std::ofstream fH;
        fH.open(dump_file_name);

        for(const auto s : strl)
        {
            fH << s + sep_str;
        }

        fH.close();
    }
}


bool dump_strlist_to_filepath(const std::list<std::string> str_list, const std::string& filepath)
{
    bool is_done = false;

    try
    {
        std::ofstream fH;
        fH.open(filepath);

        if(fH.is_open())
        {
            for(const auto& s : str_list)
                fH << s + "\n";

            fH.close();
            is_done = true;
        }
    }
    catch(...)
    {
        std::cout << "\n Failed to write back to the filepath: \"" << filepath << "\"\n";
    }
    return is_done;
}


void do_stone_blinks(STRLST& numstr_list, std::string& str_big)
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
    
        if(str_big.size()<itr->size())
        {
            str_big = *itr;
        }
    }
}


int get_num_from_char(const char& c)
{
    switch(c)
    {
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        default: return 0;
    }
}

char get_char_from_num(int c)
{
    switch(c)
    {
        case 1:return '1';
        case 2:return '2';
        case 3:return '3';
        case 4:return '4';
        case 5:return '5';
        case 6:return '6';
        case 7:return '7';
        case 8:return '8';
        case 9:return '9';
        default: return '0';
    }
}


std::string add_two_strings(const std::string& string1, const std::string& string2, std::string& errlst)
{
    std::cout << "\n Adding " << string1 << " and " << string2 << " ....";
    std::string answerstr;

    // reverse both strings so that adding can be done from last to first
    std::string str1(string1);
    std::string str2(string2);
    std::reverse(str1.begin(), str1.end());
    std::reverse(str2.begin(), str2.end());

    auto itr1=str1.begin();
    auto itr2=str2.begin();
    int i_carry = 0;


    for(; itr1 != str1.end() && itr2 != str2.end(); ++itr1, ++itr2)
    {
        errlst += "\n doing " + std::to_string(i_carry) + " + " + *itr1 + " + " + *itr2 + " :";

        int totd = i_carry + get_num_from_char(*itr1) + get_num_from_char(*itr2);
        i_carry = (int)(totd/10);

        errlst += " and got " + std::to_string(totd) + " with carry-over " + std::to_string(i_carry);
        
        answerstr += get_char_from_num(totd%10);
    }
    // add remaining string
    for(; itr1 != str1.end(); ++itr1)
    {
        int totd = i_carry + get_num_from_char(*itr1);
        i_carry = (int)(totd/10);
        answerstr += get_char_from_num(totd%10);
        errlst += "\n added reamining " + std::to_string(totd%10);
    }
    for(; itr2 != str2.end(); ++itr2)
    {
        int totd = i_carry + get_num_from_char(*itr2);
        i_carry = (int)(totd/10);
        answerstr += get_char_from_num(totd%10);
        errlst += "\n added reamining " + std::to_string(totd%10);
    }

    if(i_carry>9)
    {
        std::cout << "CARRY CAN'T BE BIGGER THAN 9";
        throw;
    }
    else if(i_carry>0)
    {
        answerstr += get_char_from_num(i_carry);
        errlst += "\n added reamining " + std::to_string(i_carry);
    }
    std::reverse(answerstr.begin(), answerstr.end());
    errlst += "\n Therefore " + string1 + " + " + string2 + " = "  + answerstr;
    //std::cout << errlst;
    return answerstr;
}

std::string create_numstream(const std::list<size_t>& numlst, std::string& errlst)
{
    std::list<std::string> numstrlst;

    for(const auto n : numlst)
    {
        numstrlst.emplace_back(std::to_string(n));
    }
    errlst += "\nDone converting nums to string";

    auto is_zero = [](const std::string& str1) -> bool{
        bool is_zero = true;
        for(const auto s : str1)
        {
            is_zero = is_zero && '0' == s;
        }
        return is_zero;
    };


    std::string finalnum;

    for(const std::string& numstr : numstrlst)
    {
        //std::cout << "\n Now adding the numbder: " << numstr;

        if(is_zero(numstr))
        {
            continue;
        }
        else if(finalnum.empty())
        {
            finalnum = numstr;
        }
        else
        {
            finalnum = add_two_strings(finalnum, numstr, errlst);
        }
    }
    return finalnum;
}


void perform_stone_blinks(const int blink_count, STRLST& numstr_list, int& idx_counter)
{
    std::cout << "\n Given blink count: " << blink_count << "\n\n";
    size_t str_size = 0;
    std::string str_big="";

    // if the lsit size reaches this size then dump the list and use buffered processing
    const size_t max_list_size=1000000;
    const size_t filedata_size=50000;

    // create a list in which all the dump-files will be stored
    std::list<std::string> g_files_list;
    std::list<size_t> g_stonescount;

    // flag that sets when to start using the files
    bool is_limit = false;
    size_t n_stonescount = 0;

    while(idx_counter <= blink_count)
    {
        if(!is_limit)
        {
            do_stone_blinks(numstr_list, str_big);
            n_stonescount = numstr_list.size();

            is_limit = numstr_list.size()>=max_list_size;

            if(is_limit)
            {
                // ============================================
                // limit reached so dump numstr_list to files 
                // ============================================

                // constatnt file name prefix
                const std::string filenameprex="day11_data_file";
                int idx_file=0; // index increase with each file-name generation

                std::list<std::string> tmp_strlist;

                for(const std::string& str : numstr_list)
                {
                    tmp_strlist.emplace_back(str);

                    if(tmp_strlist.size()>=filedata_size)
                    {
                        const std::string file_name = filenameprex + std::to_string(idx_file++) + ".tmp";
                        std::cout << "\n\t Dumping to file "<< file_name;

                        std::ofstream fH;
                        fH.open(file_name);
                        for(const std::string s : tmp_strlist)
                            fH << str + "\n";
                        fH.close();

                        // add the filename to files-list (to be used later for files based stones processing)
                        g_files_list.emplace_back(file_name);

                        // clear the temp string to create a new file in next iteration
                        tmp_strlist.clear();
                    }
                }
            }
        }
        else
        {
            // do the processing for each file 
            for(const std::string& fname : g_files_list)
            {
                std::ifstream fH;
                fH.open(fname);
                if(!fH.is_open())
                {
                    std::cout << "\nFAILED TO OPEN THE FILE: \"" << fname << "\nEXITING...\n";
                    throw;
                }
                std::string numstr;
                std::list<std::string> new_numstr;
                while(fH>>numstr)
                {
                    new_numstr.emplace_back(numstr);
                }
                fH.close();

                do_stone_blinks(new_numstr, str_big);
                g_stonescount.push_back(new_numstr.size());

                if(!dump_strlist_to_filepath(new_numstr, fname))
                {
                    throw;
                }
            }
        }

        //print_stones(numstr_list, idx_counter);

        std::cout << "\n\t completed blink  " << idx_counter << " with biggest string \"" << str_big << "\" and stones count: ";

        if(is_limit)
        {
            std::string err_list;
            std::string answer;

            try
            {
                answer = create_numstream(g_stonescount, err_list);
                err_list.clear();
            }
            catch(...)
            {
                std::cout << "\n Failed to create num!!!\nErrors: " << err_list;
                std::cout << "\n Stones sum from the files: ";
                for(const size_t s : g_stonescount)
                {
                    answer += std::to_string(s) + "+";
                }
                answer = answer.substr(0, answer.size()-1);
            }
            std::cout << answer;
        }
        else
        {
            std::cout << n_stonescount;
        }
        ++idx_counter;
    }
    --idx_counter; // making the counter point to the correct no. of iterations ran in the while-loop
}
