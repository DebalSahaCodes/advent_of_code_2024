#pragma once

#include<fstream>
#include<string>
#include<list>
#include<vector>

template<typename T>
bool get_vector_from_file(const std::string& file_path, std::list<std::vector<T>>& data_from_file)
{
    std::string str;

    std::ifstream hFile(file_path);

    bool result = false;

    if(!hFile.is_open())
    {
        return false;
    }
    else
    {
        while(getline(hFile, str))
        {
            std::vector<T> vec;

            while(!str.empty())
            {
                std::string str1 = str.substr(0, str.find(' '));
                vec.push_back(std::stoi(str1));
                
                str = str.substr(str.find(' ') + 1, str.size() - str1.size());

                result = true;
            }

            data_from_file.push_back(vec);
        }

        hFile.close();

        return result;
    }
}


void test_fetching_data_from_file()
{
    const std::string& str_path = "C:/dsaha/__Work/_advent_of_code/2024_AOCpp/day2/day_2_report.txt";

    std::list<std::vector<int>> file_data;

    std::cout << "\n";

    if(get_vector_from_file<int>(str_path, file_data))
    {
        for(const auto vec : file_data)
        {
            for(const int i : vec)
            {
                std:: cout << i << " ";
            }
            std::cout << "\n";
        }
    }
}
