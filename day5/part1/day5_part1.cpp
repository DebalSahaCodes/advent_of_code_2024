#include<fstream>
#include<array>
#include<list>

#include "../part5helper.hpp"


int main()
{
    const std::string fileP = "../pussle_input.txt";
    //const std::string fileP = ("../sample_imput.txt");

    std::vector<RULEDATA> rule_book;
    std::list<PGSVEC> pages_list;
    get_candidate_rules_and_pages(fileP, rule_book, pages_list);

    int middle_sum = 0;
    for(const PGSVEC& pages : pages_list)
    {
        if(check_pg_numbers_vs_rules(pages, rule_book))
        {
            int rem = (pages.size()%2 == 0) ? 0 : 1;
            int midpos = pages.size()/2 + rem;
            auto midnum = pages[midpos - 1];
            std::cout << "Milldle : " << midnum << "\n";
            middle_sum += std::stoi(midnum);
        }
    }
    std::cout <<"\nMiddle Sum: " << middle_sum << "\n";


    return 0;
}
