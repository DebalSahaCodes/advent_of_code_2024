#include<iostream>
#include<vector>
#include<string>
#include<fstream>

int is_numeral(const char& c)
{
    switch(c)
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
        default : return -99;
    }
}


int get_num(const std::string& str)
{
    bool isOK = true;
    for (const auto c : str)
    {
        isOK = isOK && is_numeral(c);
    }
    return (isOK) ? std::stoi(str) : -99;
}


using PAGENUM = std::string; // page-no.
using PGSVEC = std::vector<PAGENUM>;
struct RULEDATA;
using RULVEC = std::vector<RULEDATA>;

struct RULEDATA
{
    std::string _pg_before;
    std::string _pg_after;
    std::string _pg_number;

    static void make_rule(const std::string& inSTR, RULEDATA& rule1, RULEDATA& rule2)
    {
        const size_t pos = inSTR.find("|");

        if(pos != std::string::npos)
        {
            auto str1 = inSTR.substr(0, pos);
            auto str2 = inSTR.substr(pos+1, inSTR.size() - pos - 1);
            if(get_num(str1) && get_num(str2))
            {
                rule1._pg_number  = str1;
                rule1._pg_after = str2;

                rule2._pg_number   = str2;
                rule2._pg_before = str1;
            }
            else
            {
                std::cout << "Bad rule candidate: \"" + inSTR + "\"\n";
            }
        }
    }

    static bool is_according_to_rule(const std::pair<PAGENUM, PAGENUM>& pgPair, const RULEDATA& ruleD)
    {
        const auto pg1st = pgPair.first;
        const auto pg2nd = pgPair.second;

        //std::cout << "\nPage 1: " << pg1st << "\n";
        //std::cout << "\nPage 2: " << pg2nd << "\n\n";

        if(pg1st == pg2nd || pg1st != ruleD._pg_number && pg2nd != ruleD._pg_number)
        {
            return true;
        }
        else
        {
            bool isOK = pg1st == ruleD._pg_number && (pg2nd == ruleD._pg_after  || pg2nd != ruleD._pg_before)
                    ||  pg2nd == ruleD._pg_number && (pg1st == ruleD._pg_before || pg2nd != ruleD._pg_after);
            return isOK;
        }
    }

    RULEDATA(){}
    RULEDATA(const RULEDATA& data)
    {
        this->_pg_before = data._pg_before;
        this->_pg_after = data._pg_after;
        this->_pg_number = data._pg_number;
    }
    RULEDATA& operator=(const RULEDATA& data)
    {
        this->_pg_before = data._pg_before;
        this->_pg_after = data._pg_after;
        this->_pg_number = data._pg_number;
    }
};



bool check_pg_numbers_vs_rules(
    const PGSVEC& pages,
    const RULVEC& rule_book)
{
    if(pages.empty())
    {
        return false;
    }
    else if(pages.size() == 1)
    {
        return true;
    }
    bool isOK = true;
    for(int i=0; i<pages.size()-1; ++i)
    {
        const auto p1 = pages[i];
        const auto p2 = pages[i+1];

        std::pair<PAGENUM, PAGENUM> p({p1, p2}); 

        for(const auto rule : rule_book)
        {
            //std::cout << "Checking for rule: " << 
            isOK = isOK && RULEDATA::is_according_to_rule(p, rule);
        }
    }
    return isOK;
}


PGSVEC get_pages_vec(const std::string& page_str)
{
    PGSVEC page_vec;

    std::string str;

    for(const auto c : page_str)
    {
        if(!str.empty() && c == ',')
        {
            page_vec.emplace_back(str);
            str.clear();
        }
        else
        {
            str += c;
        }
    }
    return page_vec;
}


void get_rules_and_pages_vec_from_file(
    const std::string& file_path,
    std::vector<std::string>& candidate_rules,
    std::vector<std::string>& candidate_pagno)
{
    std::ifstream iFile(file_path);
    
    std::string str;
    while(iFile >> str)
    {
        if(str.find("|") != std::string::npos)
        {
            candidate_rules.emplace_back(str);
        }
        else if(str.find(",") != std::string::npos)
        {
            candidate_pagno.emplace_back(str);
        }
    }
    iFile.close();
}


void make_rules_book_from_strvec(
    const std::vector<std::string>& crules,
    std::vector<RULEDATA>& rule_book)
{
    for(const auto s : crules)
    {
        RULEDATA rule1, rule2;

        RULEDATA::make_rule(s , rule1, rule2);

        rule_book.push_back(rule1);
        rule_book.push_back(rule2);
    }
}

void make_pages_list_from_strvec(
    const std::vector<std::string>& candidate_str,
    std::list<PGSVEC>& pages_list)
{
    for(const auto pg : candidate_str)
    {
        pages_list.emplace_back(get_pages_vec(pg));
    }
}

void get_candidate_rules_and_pages(
    const std::string& fileP, 
    std::vector<RULEDATA>& rule_book,
    std::list<PGSVEC>& pages_list)
{
    std::vector<std::string> candidate_rules;
    std::vector<std::string> candidate_pagno;

    /*
    Read from file the given lists
    ==============================
    list in sec-1: rules, each line has "|"
    list in sec-2: page numbers, each line has numbers separated by ","
    */
    get_rules_and_pages_vec_from_file(fileP, candidate_rules, candidate_pagno);

    //Now generate a list of all the rules
    //stored inside the RULEBOOK container
    make_rules_book_from_strvec(candidate_rules, rule_book);


    //Extract all list of page numebrs 
    make_pages_list_from_strvec(candidate_pagno, pages_list);
}
