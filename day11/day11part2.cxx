
#include<iostream>
#include<fstream>
#include<string>
#include<string>
#include<list>
#include<vector>
#include<chrono>
#include<algorithm>
#include<execution>
#include<mutex>
#include<sstream>
#include<map>

using STRLST=std::list<std::string>;
using STRITR=std::list<std::string>::iterator;

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
    //std::cout << "\n Adding " << string1 << " and " << string2 << " ....";
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


std::string create_numstream(const std::list<std::string>& numstrlst, std::string& errlst)
{
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


std::string create_numstream(const std::list<size_t>& numlst, std::string& errlst)
{
    std::list<std::string> numstrlst;

    for(const auto n : numlst)
    {
        numstrlst.emplace_back(std::to_string(n));
    }
    errlst += "\nDone converting nums to string";
    return create_numstream(numstrlst, errlst);
}


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


std::map<std::string, std::list<std::string>> g_map_rule2;

// splits the num-string in two
bool apply_rule_2(STRITR& itr, STRLST& lst)
{
    /*std::cout << "\n ... applying rule 2 to \"" << *itr << "\" ... ";
    if(largest_interval<itrpos)
        largest_interval=itrpos;*/

    try
    {
        auto itrbkup = *itr;

        if(itr->size()%2==0)
        {
            // check if the global "map" for rule-2 has the input already
            auto mapI = g_map_rule2.find(*itr);
            if(g_map_rule2.end() != mapI)
            {
                *itr = mapI->second.front();
                try{
                      std::string str2 = mapI->second.back();
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
                catch(...){
                  std::cout<<"\n FAiled to Insert from MAp";
                  throw;
                }
                return true;
            }
          
            // find middle position 
            size_t pos_mid = itr->size()/2;  //std::cout<< "; mid pos : " << pos_mid;
            std::string str2; // used for the later half of string
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
            // create a list of str to store in global-map of rule-2 results
            std::list<std::string> ans;
            // make the current string first-half
            try
            {
                *itr=itr->substr(0, pos_mid);
                // create the result lsit and add to global map; waiting for second-part insert to list isn't needed
                ans.emplace_back(*itr);
                ans.emplace_back(str2);
                g_map_rule2.insert({itrbkup, ans});
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
                std::string err =  "\n Failed to insert \"" + str2 + "\" at pos " + std::to_string(itrpos)
                                   + " i.e. after part-1-str \"" + *itr + "\" of given itr \"" + itrbkup + "\" ..";
                std::cout << err;
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



void do_stone_blinks(STRLST& numstr_list)
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
}


void perform_stone_blinks(const int blink_count, STRLST& numstr_list, int& idx_counter)
{
    std::cout << "\n\t RUNNING THE STRING-LIST BREAK MODE .....\n\n";
    std::string answer; // used to return stones-count

    size_t str_size = 0;
    std::string str_big="";

    // if the lsit size reaches this size then dump the list and use buffered processing
    const size_t break_triger_limit=4000000;
    const size_t each_str_list_size=5000;

    std::list<std::list<std::string>> g_list_liststr;
    std::list<size_t> g_stonescount;

    // flag that sets when to start using the files
    bool is_limit = false;
    bool is_dumped = false;
    size_t n_stonescount = 0;
        
    auto start = std::chrono::high_resolution_clock::now();
    while(idx_counter <= blink_count)
    {
        g_stonescount.clear();

        if(!is_limit)
        {
            do_stone_blinks(numstr_list);
            n_stonescount = numstr_list.size();
        }
        else
        {
            try
            {
                std::for_each(std::execution::par, g_list_liststr.begin(), g_list_liststr.end(),
                [](std::list<std::string>& new_numstr)
                {
                    do_stone_blinks(new_numstr);
                });

                for(const auto& strlst : g_list_liststr)
                    g_stonescount.push_back(strlst.size());
            }
            catch(...)
            {
                throw;
            }
        }
        std::stringstream s;
        str_time_spent(start, s);
        std::cout << "\n\t completed blink  " << idx_counter << " in " << s.str() << " and stones count: ";

        if(is_limit)
        {
            std::string err_list;
            try{
                answer = create_numstream(g_stonescount, err_list);
                err_list.clear();
            }catch(...){
                std::cout << "\n Failed to create num!!!\nErrors: " << err_list;
                std::cout << "\n Stones sum from the files: ";
                for(const size_t s : g_stonescount) answer += std::to_string(s) + "+";
                answer = answer.substr(0, answer.size()-1);
            }
        }
        else{
            answer = std::to_string(n_stonescount);
        }
        std::cout << answer;
        ++idx_counter;

        is_limit = numstr_list.size() >= break_triger_limit;

        if(is_limit && !is_dumped){
            // ============================================
            // limit reached so dump into  list-of-lists 
            // ============================================
            std::cout << "\n\t... breaking input " << numstr_list.size() << " string ";

            std::list<std::string> tmp_strlist;
            for(auto striter=numstr_list.begin(); striter != numstr_list.end(); ++striter)
            {
                tmp_strlist.emplace_back(*striter);

                if(tmp_strlist.size() >= each_str_list_size){
                    g_list_liststr.emplace_back(tmp_strlist);
                    tmp_strlist.clear();
                }
            }
             // last remaining
            if(!tmp_strlist.empty()){
                g_list_liststr.emplace_back(tmp_strlist);
                tmp_strlist.clear();
            }
            std::cout << "into new "<< g_list_liststr.size() << " lists ...";
            is_dumped =  false == g_list_liststr.empty();
            if(!is_dumped)
            {
                std::cout <<"\n FAILEd TO gENERATE files from string..... ABORT!!!!\n";
                throw;
            }
        }

    }
    --idx_counter; // making the counter point to the correct no. of iterations ran in the while-loop
}


bool dump_strlist_to_filepath(
    const std::list<std::string> str_list,
    const std::string& filepath,
    std::string& err)
{
    bool is_done = false;

    try
    {
        std::ofstream fH;
        fH.open(filepath);

        if(fH.is_open())
        {
            for(const auto& s : str_list)
                fH << s + " ";

            fH.close();
            is_done = true;
        }
    }
    catch(...)
    {
        is_done = false;
        err = "\n Failed to write back to the filepath: \"" + filepath + "\"\n";
    }
    return is_done;
}

void do_files_stones_blink(const std::string& filename, std::string& stonescount)
{
    static int breaksize = 10000;
    //breaksize *=2; // every call will use twice the breaksize then the last iteration

    std::list<size_t> g_stonescount;

    std::ifstream fH;
    fH.open(filename);
    if(!fH.is_open())
    {
        std::cout << "\nFAILED TO OPEN THE FILE: \"" << filename << "\nEXITING...\n";
        throw;
    }
    std::string numstr;
    std::list<std::list<std::string>> g_stringlist;

    std::list<std::string>new_numstr;
    while(fH>>numstr)
    {
        new_numstr.emplace_back(numstr);
        if(new_numstr.size()>=breaksize){
            g_stringlist.emplace_back(new_numstr);
            new_numstr.clear();
        }
    }
    if(!new_numstr.empty()){
        g_stringlist.emplace_back(new_numstr);
        new_numstr.clear();
    }
    fH.close();
    std::for_each(std::execution::par, g_stringlist.begin(), g_stringlist.end(),
    [](std::list<std::string>& strlist){
        do_stone_blinks(strlist);
    });
    std::string err_str;
    for(const auto& strlst : g_stringlist) g_stonescount.push_back(strlst.size());
    std::list<std::string> finlist;
    for(const auto& l : g_stringlist)
        for(const auto& s : l)
        finlist.emplace_back(s);
    if(!dump_strlist_to_filepath(finlist, filename, err_str))
    {
        std::cout << err_str;
        throw;
    }
    stonescount = create_numstream(g_stonescount, err_str);
    //std::cout << "\n\t\t file gave " + stonescount + " stones!!";
}

void perform_stone_blinks_in_files(const int blink_count, STRLST& numstr_list, int& idx_counter)
{
    std::string answer; // used to return stones-count

    std::cout << "\n Given blink count: " << blink_count << "\n\n";
    size_t str_size = 0;
    std::string str_big="";

    // if the lsit size reaches this size then dump the list and use buffered processing
    const size_t max_list_size=10000000; // break into files for >= 10 million
    const size_t listdata_size=100000;

    // create a list in which all the dump-files will be stored
    std::list<std::string> g_files_list;
    std::list<std::list<std::string>> g_list_liststr;
    std::list<std::string> s_stonescount;

    // flag that sets when to start using the files
    bool is_limit = false;
    bool is_dumped = false;
    size_t n_stonescount = 0;
    std::string stones;
    auto start = std::chrono::high_resolution_clock::now();

    while(idx_counter <= blink_count)
    {
        s_stonescount.clear();

        if(!is_limit)
        {
            do_stone_blinks(numstr_list);
            n_stonescount = numstr_list.size();
        }
        else
        {
            std::list<std::string> stone_count_list;
            try{
                for(const std::string& fname : g_files_list){
                    std::string stonescount;
                    do_files_stones_blink(fname, stonescount);
                    stone_count_list.push_back(stonescount);
                }
                std::string err;
                s_stonescount.emplace_back(create_numstream(stone_count_list, err));
            }
            catch(...)
            {
                throw;
            }
        }
        std::stringstream s;
        str_time_spent(start, s);
        std::cout << "\n\t completed blink  " << idx_counter << " in " << s.str() << " and stones count: ";

        if(is_limit)
        {
            std::string err_list;

            try
            {
                answer = create_numstream(s_stonescount, err_list);
                err_list.clear();
            }
            catch(...)
            {
                std::cout << "\n Failed to create num!!!\nErrors: " << err_list;
                std::cout << "\n Stones sum from the files: ";
                for(const std::string& s : s_stonescount)
                {
                    answer += s + "+";
                }
                answer = answer.substr(0, answer.size()-1);
            }
        }
        else
        {
            answer = std::to_string(n_stonescount);
        }

        std::cout << answer;
        ++idx_counter;


        is_limit = numstr_list.size()>=max_list_size;

        if(is_limit && !is_dumped)
        {
            std::cout << "\n\t... breaking input " << numstr_list.size() << " string and ";
            // ============================================
            // limit reached so dump into  list-of-lists 
            // ============================================

           std::string file_err;
           std::string filename;
           std::list<std::string> tmp_strlist;
           int idx_file=0; // index increase with each file-name generation

            std::cout << "\n\t\t writing strings to approx. " << (int)(max_list_size/listdata_size) <<" files ....\n\n";
            for(auto striter=numstr_list.begin(); striter != numstr_list.end(); ++striter)
            {
                tmp_strlist.emplace_back(*striter);
        
                if(tmp_strlist.size()>=listdata_size)
                {
                    filename = "inputfile_" + std::to_string(idx_file++) + ".tmp";
                    //std::cout << "\n\t\t writing " << tmp_strlist.size() << " strings to \""<<filename<<"\"";
                    if(!dump_strlist_to_filepath(tmp_strlist, filename, file_err)){
                        std::cout << "FILE WRITE FAIL!!!";
                        throw;
                    }
                    else{
                        g_files_list.emplace_back(filename);
                    }
                    tmp_strlist.clear();
                }
            }
            if(!tmp_strlist.empty()) // last remaining
            {
                //std::cout << "\n\t\t writing " << tmp_strlist.size() << " strings ....";
                filename = "inputfile_" + std::to_string(idx_file++) + ".tmp";
                if(!dump_strlist_to_filepath(tmp_strlist, filename, file_err)){
                    std::cout<<"FILE WRITE LAST FAIL!!!";
                    throw;
                }
                else{
                    g_files_list.emplace_back(filename);
                }
                tmp_strlist.clear();
            }
            // ============================================
            //  dump numstr_list to files 
            // ============================================

            /*// constatnt file name prefix
            const std::string filenameprex="";
            std::cout << "... creating files:\n";
            for(const auto& stringlist : g_list_liststr){
                if(stringlist.empty()) std::cout<<"EMPTYEMPTY!!!";throw;
                const std::string file_name = filenameprex + std::to_string(idx_file++) + ".tmp";                
                std::cout << "\t "<< file_name << "\n";
                std::ofstream fH; fH.open(file_name);
                for(const std::string& str : stringlist) fH << str + "\n";
                fH.close();
                g_files_list.emplace_back(file_name);
            }*/
            is_dumped = false == g_files_list.empty();
            if(!is_dumped)
            {
                std::cout <<"\n FAILEd TO gENERATE files from string..... ABORT!!!!\n";
                throw;
            }

        }
    }
    --idx_counter; // making the counter point to the correct no. of iterations ran in the while-loop
}



int main(int argc, char** argv)
{
    int blink_count = 75;

    std::string file_p = "puzzle_input.txt";

    // if user input given then use those
    bool is_given = false;
    int temp_blnk;
    std::string  temp_file;

    if (argc >= 3)
    {
        try
        {
            temp_blnk = std::stoi(argv[1]);
            is_given = true;
        }
        catch (...)
        {
            std::cout << "\nBad input for blink count\n";
        }

        try
        {
            temp_file = argv[2];
            is_given = is_given && true;
        }
        catch (...)
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
    if (!get_num_str_list(file_p, numstr_list))
    {
        std::cout << "\n FAILED TO PARSE THE LINES IN FILE: \"" << file_p << "\"\n";
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    bool stonesdone = false;
    int curr_idx = 1;

    try
    {
        if(blink_count<=30)
            perform_stone_blinks(blink_count, numstr_list, curr_idx);
        else
            perform_stone_blinks_in_files(blink_count, numstr_list, curr_idx);
    }
    catch (...)
    {
        std::cout << "\n Failed at idx \"" << curr_idx << " \" !!!\n\n";
    }

    /*auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    auto duration__s = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    bool is_sec = duration__s > 0;
    auto duration = (is_sec ? duration__s : duration_ms);

    std::cout << "\nTime taken: " << duration << (is_sec ? " s" : " ms") << "\n";*/
    std::stringstream s;
    str_time_spent(start_time, s);
    std::cout << "\nTime taken: " << s.str() << "\n";

    return 0;
}
