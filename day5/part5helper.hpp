#include<iostream>
#include<fstream>
#include<vector>
#include<string>


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
using PGSVECLST = std::list<PGSVEC>;
struct RULEDATA;
using RULESBOOK = std::list<RULEDATA>;

struct RULEDATA
{
    std::string _pg1st;
    std::string _pg2nd;

    static const RULEDATA make_rule(const std::string& inSTR)
    {
        RULEDATA rule;
        const size_t pos = inSTR.find("|");

        if(pos != std::string::npos)
        {
            auto str1 = inSTR.substr(0, pos);
            auto str2 = inSTR.substr(pos+1, inSTR.size() - pos - 1);
            if(get_num(str1) && get_num(str2))
            {
                rule._pg1st = str1;
                rule._pg2nd = str2;
            }
            else
            {
                std::cout << "Bad rule candidate: \"" + inSTR + "\"\n";
            }
        }
        return rule;
    }

    static bool is_according_to_rule(const std::pair<PAGENUM, PAGENUM>& pgPair, const RULEDATA& ruleD)
    {
        const auto pg1st = pgPair.first;
        const auto pg2nd = pgPair.second;

        //std::cout << "\nPage 1: " << pg1st << "\n";
        //std::cout << "\nPage 2: " << pg2nd << "\n\n";

        bool isNotOK = pg1st == ruleD._pg2nd && pg2nd == ruleD._pg1st;
        return !isNotOK;
    }

    RULEDATA(){}
    RULEDATA(const RULEDATA& data)
    {
        this->_pg1st = data._pg1st;
        this->_pg2nd = data._pg2nd;
    }
    RULEDATA& operator=(const RULEDATA& data)
    {
        this->_pg1st = data._pg1st;
        this->_pg2nd = data._pg2nd;
    }
    void print_rule() const
    {
        std::cout << _pg1st << "|" << _pg2nd;
    }
};



bool compare_pg1_lt_pg2_using_rules(
    const PAGENUM& pg1,
    const PAGENUM& pg2,
    const RULESBOOK& rule_book)
{
    bool isDone = false;
    bool isLess = false;
    auto ruleIter0 = rule_book.begin();
    for(; ruleIter0!=rule_book.end() || !isDone; ++ruleIter0)
    {
        if(ruleIter0->_pg1st == pg1 && ruleIter0->_pg2nd == pg2)
        {
            isLess = true;
            isDone = true;
        }
        else if(ruleIter0->_pg1st == pg2 && ruleIter0->_pg2nd == pg1)
        {
            isDone = true;
        }
    }
    if(!isDone) std::cout << "Couldn't find";
    return isLess;
}

bool check_pg_numbers_vs_rules(
    const PGSVEC& pages,
    const RULESBOOK& rule_book,
    const int nRules,
    bool doPrint)
{
    int till = (nRules>=0 && nRules <= rule_book.size()) ? nRules : static_cast<int>(rule_book.size());
    if(doPrint) std::cout << "\nUsing \"till\" value : " << till << "\n";

    if(pages.empty())
    {
        std::cout << "\nEMPTY PAGES VEC GIVEN TO CHECK!!!\n";
        return false;
    }
    else if(pages.size() == 1)
    {
        std::cout << "\nSINGLE PAGE IN THE PAGES VEC!\n";
        return true;
    }
    bool isOK = true;
    std::string log;
    for(int pos1=0; pos1 < pages.size()-1; ++pos1)
    {
        for(int pos2=pos1+1; pos2 < pages.size(); ++pos2)
        {
            const auto p1 = pages[pos1];
            const auto p2 = pages[pos2];

            std::pair<PAGENUM, PAGENUM> p({p1, p2}); 

            bool isDone = false;
            int iRule = 0;

            auto ruleIter = rule_book.begin();
            for(; ruleIter != rule_book.end() && !isDone; ++ruleIter)
            {
                ++iRule;
                auto rule = *ruleIter;

                if(doPrint)
                {
                    log = "\t" + std::to_string(iRule) + " - Checking  \"" + p1 + " & " + p2 + "\" with  rule \"";
                    log += rule._pg1st + "|" + rule._pg2nd;
                    log += "\" : ";
                }

                isOK = isOK && RULEDATA::is_according_to_rule(p, rule);

                std::string isOkRes = (isOK) ? "Good" : "Bad";
                if(doPrint) log += isOkRes + "\n";

                if(iRule == till)
                {
                    isDone=true;// to end this iteration;
                    if(doPrint) log += "Done\n";
                }
                if(!isOK)
                {
                    std::cout << "LOG: " <<  log;
                    return false;
                }
            }
            //if(doPrint) std::cout << "\n";
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
    if(!str.empty())
    {
        page_vec.emplace_back(str);
        str.clear();
    }
    return page_vec;
}


void get_rules_and_pages_vec_from_file(
    const std::string& file_path,
    std::vector<std::string>& candidate_rules,
    std::vector<std::string>& candidate_pagno)
{
    std::ifstream iFile(file_path);
    if(!iFile.is_open())
    {
        std::cout << "\n Failed to Open File: \"" << file_path << "\"\n";
        return;
    }

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
    const std::vector<std::string>& c_rules,
    RULESBOOK& rule_book)
{
    for(const auto s : c_rules)
    {
        //std::cout << "\nMaking rule from " << s ;
        rule_book.push_back(RULEDATA::make_rule(s));
    }
}

void make_pages_list_from_strvec(
    const std::vector<std::string>& candidate_str,
    PGSVECLST& pages_list)
{
    for(const auto pg : candidate_str)
    {
        pages_list.emplace_back(get_pages_vec(pg));
    }
}

void get_candidate_rules_and_pages(
    const std::string& fileP, 
    RULESBOOK& rule_book,
    PGSVECLST& pages_list,
    PGSVEC& ranks_list = PGSVEC())
{
    PGSVEC candidate_pagno;
    PGSVEC& candidate_rules = ranks_list;

    /*
    Read from file the given lists
    ==============================
    list in sec-1: rules, each line has "|"
    list in sec-2: page numbers, each line has numbers separated by ","
    */
    get_rules_and_pages_vec_from_file(fileP, candidate_rules, candidate_pagno);

    //Now generate a list of all the rules
    //stored inside the RULESBOOK container
    make_rules_book_from_strvec(candidate_rules, rule_book);


    //Extract all list of page numebrs 
    make_pages_list_from_strvec(candidate_pagno, pages_list);
}



void print_pages(const PGSVEC& pg)
{
    for(const auto& s : pg)
        std::cout << s << " ";
    std::cout<< "\n";
}
