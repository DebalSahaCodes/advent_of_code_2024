#include<list>
#include<map>
#include<algorithm>

#include "../part5helper.hpp"


bool fix_page_by_rank_here(PGSVEC& pages, const RULESBOOK& rule_book)
{
    auto pgsLessThan = [&rule_book](const PAGENUM& pg1, const PAGENUM& pg2) -> bool
    {
        return compare_pg1_lt_pg2_using_rules(pg1, pg2, rule_book);
    };
    
    std::sort(pages.begin(), pages.end(), pgsLessThan);
    return check_pg_numbers_vs_rules(pages, rule_book, rule_book.size(), true);
}

int main()
{
    const std::string fileP = "../puzzle_input.txt";

    PGSVEC rankPairs;
    RULESBOOK rule_book;
    PGSVECLST pages_list;

    get_candidate_rules_and_pages(fileP, rule_book, pages_list, rankPairs);

    std::cout <<"\nRULE BOOK SIZE: " << rule_book.size() << "\n";

    PGSVEC ranked_pages;
    int middle_sum = 0;
    for(auto& pages : pages_list)
    {
        std::cout << "\nGiven pages : ";
        print_pages(pages);

        if(!check_pg_numbers_vs_rules(pages, rule_book, rule_book.size(), true))
        {
            std::cout <<"\n Fixing the pages ...\n";
            //if(fix_page_by_rank(pages))
            if(fix_page_by_rank_here(pages, rule_book))
            {
                std::cout << "Fixed pages : ";
                print_pages(pages);
    
                int rem = (pages.size()%2 == 0) ? 0 : 1;
                int midpos = pages.size()/2 + rem;
                auto midnum = pages[midpos - 1];
                std::cout << "Mid for pages: " << midnum << "\n";
                middle_sum += std::stoi(midnum);
            }
            else
            {
                print_pages(pages);
                std::cout << "FAILED TO FIX!!\n";
            }
        }
    }
    std::cout <<"\nMiddle Sum for bad ordered: " << middle_sum << "\n";

    return 0;

}
