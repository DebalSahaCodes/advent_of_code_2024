#include<iostream>
#include<fstream>
#include<string>
#include<list>

#include"day3_lib.hpp"


int main()
{
    std::list<std::string> data_raw;

    std::cout << "Try reading from file ...\n";

    int iCount = 0;
    int answer = 0;

    if(extract_mul_from_lines(data_raw))
    {
        std::cout << "Reading from file done\n";

        for(const auto s : data_raw)
        {
            //std:: cout << s << "\n";
            int prod = -99;
            if(!is_valid(s, prod) || prod<0)
            {
                // check if another "mul("
                // create string back-wards
                if(s.size() > 6)
                {
                    std::string snew;
                    int len = s.size() -1;
                    while(len >= 0)
                    {
                        snew += s[len];
                        --len;
                        if(snew.size()>4 && snew.find("(lum") != std::string::npos)
                        {
                            reverse_str(snew);
                            std::cout << snew << "\n";
                            break;
                        }
                    }

                    if(is_valid(snew, prod) && prod>=0)
                    {
                        //std::cout<<snew + " : " <<prod << "\n";
                        answer += prod;
                        ++iCount;
                    }
                }
            }
            else
            {
                //std::cout<<s + " : " <<prod << "\n";
                answer += prod;
                ++iCount;
            }
        }
    }

    std::cout <<"\n Count: " << iCount;
    std::cout <<"\n Total: " << answer;

    return 0;
}
