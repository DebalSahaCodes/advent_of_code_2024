#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

bool get_arrays(std::vector<int>& v1, std::vector<int>& v2)
{
    std::string str;
    std::ifstream hFile("array_file.txt");
    if(!hFile.is_open())
    {
        return false;
    }
    else
    {
        while(getline(hFile, str))
        {
            if(!str.empty())
            {
                std::string str1;

                str1 = str.substr(0, str.find(' '));
                v1.push_back(std::stoi(str1));
                
                str1 = str.substr(str.find(' '), str.size()-1);
                v2.push_back(std::stoi(str1));
            }
        }
        hFile.close();
        return false==v2.empty() && false==v1.empty();
    }
}

void print_vec(const std::vector<int>& vec)
{
    int iCol = 15;
    std::cout << "\nPrinting array: ";
    for(const int v: vec)
    {
        ++iCol;
        if(iCol >= 15)
        {
            std::cout << "\n";
            iCol = 0;
        }
        std::cout << v << " ";
    }
    
}
    

void sort(std::vector<int>& vec)
{
    for(int i=1; i<vec.size(); ++i)
    {
        const int candidate = vec[i];

        int j = i - 1;
        while(j>0 && vec[j]>candidate)
        {
            vec[j+1] = vec[j];
            --j;
        }
        vec[j] = candidate;
    }
}

int main()
{
    std::vector<int> vec1;
    std::vector<int> vec2;

    get_arrays(vec1, vec2);

    std::cout << "Size of vec1: " << vec1.size();
    std::cout << "\nSize of vec2: " << vec2.size();

    //print_vec(vec1);
    //print_vec(vec2);

    std::cout << "\nsorting vec1 ...";
    //sort(vec1);
    //sort(vec2);
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    //std::cout << "\nPost sorting the vec1 ...";
    //print_vec(vec1);
    //print_vec(vec2);

    std::cout << "\nGetting  the deiff b/w vec1 & vec2 ...";

    int result = 0;

    for(int iCount = 0; iCount < vec1.size(); ++iCount)
    {
        int diff = vec1[iCount] - vec2[iCount];
        diff = (diff >= 0) ? diff : -1*diff;
        result += diff;
    }
    std::cout << "\n\nResult: " << result << "\n\n";
    return 0;
}
