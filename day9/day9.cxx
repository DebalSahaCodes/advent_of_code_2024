#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<chrono>
#include<memory>

class FileMap
{
    std::vector<std::string> _vec_memory;

    public:

    FileMap(){}
    ~FileMap(){}

    

};

using STRTYPE=std::string;
using STRVECTYPE=std::vector<std::string>;

bool is_char_same_as_strtype(const char& c, const STRTYPE& str)
{
    return str.size()>=1 && str[0]==c;
}
const STRTYPE to_strtype(const char& c)
{
    return std::string({c});
}
void add_to_strvec(const int i, STRVECTYPE& strvec)
{
    strvec.emplace_back(std::to_string(i));
}
void add_to_strvec(const char& c, STRVECTYPE& strvec)
{
    strvec.emplace_back(std::string{c});
}

int get_int_from_char(const char& ch)
{
    switch(ch)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default: 
        {
            std::cout << "BAD BAD!!!!";
            throw;
        }
    }
}

long long get_num_from_str(const STRTYPE& str1)
{
    return std::stoll(str1);
}



bool get_file_line(const std::string& file_path, std::string& line)
{
    std::ifstream iFile(file_path);

    if(!iFile.is_open())
        return false;

    std::string str;
    while(iFile>>str)
    {
        line += str;
    }
    return true;
}

void expand_string(const char space_symbol, const std::string& fix_str, STRVECTYPE& out_str)
{
    //std::cout << "\n Expanding line: " << fix_str << "\n";
    bool is_file=true;
    int i_file=0;

    STRTYPE c_add;

    for(const char& c : fix_str)
    {
        int idx = get_int_from_char(c);

        if(is_file)
        {
            //std::cout << "\tAdding \"" << i_file << "\" " << idx << " times \n";
            for(; idx>0; --idx)
                add_to_strvec(i_file, out_str);
            ++i_file;
        }
        else
        {
            //std::cout << "\tAdding \"" << space_symbol << "\" " << idx << " times \n";
            for(; idx>0; --idx)
                add_to_strvec(space_symbol, out_str);
        }

        is_file=!is_file; // toggle flag
    }
}

void process_string(const char space_symbol, STRVECTYPE& string_to_process)
{
    long long n_space=0;
    long long n_files=0;
    for(const auto& s : string_to_process)
    {
        if(is_char_same_as_strtype(space_symbol, s))
            ++n_space;
        else
            ++n_files;
    }
    const long long n_total=n_files+n_space;


    auto itr_files=std::prev(string_to_process.end()); // start from the end of the vector

    for(; itr_files !=string_to_process.begin(); --itr_files)
    {
        //std::cout << "\n Processing " << string_to_process[idx] ;
        if(!is_char_same_as_strtype(space_symbol, *itr_files))
        {
            auto itr_space = std::find(string_to_process.begin(), string_to_process.end(), std::string({space_symbol}));

            const auto idx_space = std::distance(string_to_process.begin(), itr_space);
            const auto idx_files = std::distance(string_to_process.begin(), itr_files);

            if(idx_space>=n_files)
            {
                break;
            }

            if(itr_space==string_to_process.end())
            {
                std::cout << "\n PROBLEMATIC STRING.........\n\n";
                throw;
            }

            //std::cout << "Replacing " << *itr_files << " at pos " << idx_files << " with " << *itr_space << " at " << idx_space << "\n";

            *itr_space = *itr_files;
            *itr_files = to_strtype(space_symbol);
        }
    }
}

const long long get_checksum(const char sapce_symbol, const STRVECTYPE& sortedline)
{
    long long cks=0;
    long long idx=0;
    for(const auto& s : sortedline)
    {
        if(is_char_same_as_strtype(sapce_symbol, s))
            break;
        else
        {
            const auto i_add = idx*get_num_from_str(s);
            //std::cout << "Adding  " << idx << " * " << get_int_from_char(s) << "  : " << i_add << "\n";
            cks += i_add;
            ++idx;
        }
    }
    return cks;
}

int main(int argc, char** argv)
{
    const auto t0 = std::chrono::high_resolution_clock::now();

    const std::string pFile="puzzle_input.txt";
    //const std::string pFile="sample_input.txt";

    std::string input_line;
    if(!get_file_line(pFile, input_line))
    {
        std::cout<<"\nFailed to read from file : " + pFile << "\n";
        return -1;
    }

    constexpr char space_symbol = '.';

    STRVECTYPE expanded_tr;
    expand_string(space_symbol, input_line, expanded_tr);
    std::cout << "After extracting files and spaces: \"" ;
    for(const auto& s : expanded_tr)
        std::cout << s;
    std::cout << "\"\n";

    const auto timeTaken1 =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "Time taken: " << timeTaken1 << " ms";//<< " microseconds \n";

    process_string(space_symbol, expanded_tr);
    std::cout<<"\nAfter processing files and spaces: \"" ;
    for(const auto& s : expanded_tr)
        std::cout << s;
    std::cout << "\"\n";

    const auto chksum = get_checksum(space_symbol, expanded_tr);
    std::cout<<"\n check-sum from string: " << chksum << "\n";

    const auto timeTaken2 =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "\n\n Time taken: " << timeTaken2 << " ms \n";//<< " microseconds \n";

    return 0;
}

