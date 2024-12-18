#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<map>



void reverse_str(std::string& str)
{
    const int len = str.size() - 1;
    for(int i=0; i<len/2+1; ++i)
    {
        const char c = str[i];

        str[i] = str[len - i];
        str[len - i] = c;
    }
}


bool is_last_chars_chk(std::string& str, const std::string& chk = "mul(")
{
    const size_t len = str.size();

    if(len>=chk.size())
    {
        std::string s1 = str.substr(len - chk.size(), chk.size());
        if(s1 == chk)
        {
            //std::cout << "Checking : " << s1 << "\n";
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}


void trim_pre_mul(std::string& str, const std::string& mul_str)
{
    const auto pos0 = str.find(mul_str);
    if(pos0 != std::string::npos)
    {
        str = str.substr(pos0);
    }
}


bool extract_mul_from_lines(std::list<std::string>& mul_list, std::string str_path = "no__")
{
    const std::string file_path = (str_path == "no__") ? "C:/dsaha/__Work/_advent_of_code/2024_AOCpp/day3/corrupt_mem.txt" : str_path;

    std::string str;

    std::fstream hFile(file_path, std::fstream::in);

    if(!hFile.is_open())
    {
        std::cout << "\nFile not found\n\n";
        return false;
    }
    else
    {
        //const std::string ok_str = "mul(x,x)";
        const std::string min_str = "mul(";

        char char_read;
        std::string str_read;
        std::string str_good;

        bool is_flag = false;

        while(hFile >> char_read)
        {
            str_read += char_read;
            const auto pos0 = str_read.find(min_str);

            if(!is_flag && char_read == '(' && pos0 != std::string::npos)
            {
                // trim leading (not need for trailing as "(" in "mul(" should have been just detected
                // and set flag
                trim_pre_mul(str_read, min_str);
                is_flag = true;
                continue;
            }

            if(is_flag && char_read == ')')
            {
                mul_list.emplace_back(str_read);
                str_read.clear(); // reset-1
                is_flag = false; // reset-2
            }

        }

        // filce-close & if LIST is non-empty then return TRUE
        hFile.close();
        return false == mul_list.empty();
    }
}


bool is_found_do_in_last_4_chars(const std::string& str)
{
    bool result = false;
    if(str.size() >= 4)
    {
        auto str1 = str.substr(str.size() - 4);
        result = str1.find("don't") == std::string::npos
                 && str1.find("do") != std::string::npos;
    }
    return result;
}


bool get_mul_lines_with_do(
        std::map<int, std::string>& map_mul_all,
        std::map<int, std::string>& map_do_don)
{
    const std::string file_path = "C:/dsaha/__Work/_advent_of_code/2024_AOCpp/day3/corrupt_mem.txt";

    std::string str;

    std::fstream hFile(file_path, std::fstream::in);

    if(!hFile.is_open())
    {
        std::cout << "\nFile not found\n\n";
        return false;
    }
    else
    {
        //const std::string ok_str = "mul(x,x)";
        const std::string min_str = "mul(";
        const std::string trg_str = "do";
        const std::string stp_str = "don't";

        char char_read;
        std::string str_read;
        std::string str_good;

        bool is_flag = false;
        bool is_okay = true;

        int pos_in_report;

        while(hFile >> char_read)
        {
            ++pos_in_report;

            str_read += char_read;

            /*if(!is_okay && is_last_chars_chk(str_read, "do"))
            {
                //std::cout << "\nRESETTING .." << str_read << "\n";
                is_okay = true;
            }
            if(is_okay && is_last_chars_chk(str_read, "don't"))
            {
                is_okay = false;
                //std::cout << "\nSTOP DONT .." << str_read << "\n";
                str_read.clear(); // resetting after a "don't" as all prev. should have been processed
            }*/
            if(is_found_do_in_last_4_chars(str_read) && !is_last_chars_chk(str_read, "don't"))
            {
                map_do_don.insert(std::pair<int, std::string>(pos_in_report - 4, "do"));
            }
            else if(is_last_chars_chk(str_read, "don't"))
            {
                map_do_don.insert(std::pair<int, std::string>(pos_in_report - 4, "don't"));
            }

            if(is_flag && is_okay)
            {
                str_good += char_read;
                if(char_read == ')')
                {
                    //mul_list.emplace_back(str_good);
                    map_mul_all.insert(std::pair<int, std::string>(static_cast<int>(pos_in_report - str_good.size()), str_good));

                    str_good.clear(); // reset-1
                    str_read.clear(); // reset-2
                    is_flag = false; // reset-3
                }
            }
            if(is_okay && !is_flag && char_read == '(' && is_last_chars_chk(str_read, "mul("))
            {
                // trim leading (not need for trailing as "(" in "mul(" should have been just detected
                // and set flag
                str_good = "mul(";
                is_flag = true;
            }
        }

        // filce-close & if LIST is non-empty then return TRUE
        hFile.close();
        return !map_mul_all.empty() && !map_do_don.empty();
    }
}

bool is_char_num(const char& s1, int& num)
{
    switch(s1)
    {
        case '0': { num = 0; return true;}
        case '1': { num = 1; return true;}
        case '2': { num = 2; return true;}
        case '3': { num = 3; return true;}
        case '4': { num = 4; return true;}
        case '5': { num = 5; return true;}
        case '6': { num = 6; return true;}
        case '7': { num = 7; return true;}
        case '8': { num = 8; return true;}
        case '9': { num = 9; return true;}
        default: return false;
    }
}

bool is_char_num(const char& s1)
{
    switch(s1)
    {

        case '0': return true;
        case '1': return true;
        case '2': return true;
        case '3': return true;
        case '4': return true;
        case '5': return true;
        case '6': return true;
        case '7': return true;
        case '8': return true;
        case '9': return true;
        default: return false;
    }
}


bool is_number(const std::string& str)
{
    bool isNum = true;
    for(const auto s : str)
    {
        isNum = isNum && is_char_num(s);
    }
    return isNum;
}



bool get_number(const std::string& str, int& num)
{
    const int bkup = num; // to be resigned this bkup if processing fails

    const bool result = is_number(str);

    if(!result)
    {
        num = bkup;
    }
    else
    {
        num = std::stoi(str);
    }

    return result;
}


bool is_valid(const std::string& str, int& prod)
{
    std::string s1 = str;

    // special logic to handle the case of "mul(,;>?mul(108,268)" ....
    bool is_special = false;
    if(str.size() > 5)
    {
        const std::string str3 = s1.substr(4);
        const auto newPos = str3.find("mul(");
        if(newPos != std::string::npos)
        {
            s1 =  str3.substr(newPos);
            //std::cout << "\nPROCESSING SPECIAL CASE : " << s1 << "\n";
            is_special = true;
        }
    }

    std::string mul_p1 = s1.substr(s1.find('(') + 1, s1.find(',') - s1.find('(') - 1);
    std::string mul_p2 = s1.substr(s1.find(',') + 1, s1.find(')') - s1.find(',') - 1);

    if(is_special) std::cout<< s1 << ":" << mul_p1 << ", " << mul_p2 << "\n";

    int num1, num2 = -99;

    if(get_number(mul_p1, num1) && get_number(mul_p2, num2))
    {
        prod = num1*num2;
        std::cout<< mul_p1 << ", " << mul_p2 << ":" << prod << "\n";
        return true;
    }
    else
    {
        return false;
    }
}
