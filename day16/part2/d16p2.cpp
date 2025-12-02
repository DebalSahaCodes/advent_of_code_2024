#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <list>
#include <map>
#include <algorithm>
#include <memory>

bool g_PRINT = true; 

std::string g_iFileName="puzzle.txt";

std::map<std::pair<int,int>,std::pair<char,int>> g_maze;

std::tuple<int, int, char> S_pos;
std::tuple<int, int, char> E_pos;

int g_xMax=-1;
int g_yMax=-1;

std::list<int> g_scores;


using DTYPE = char; // for direction
using STYPE = int; // for score
using MTYPE = std::tuple<int,int,DTYPE>;

struct Node{
    MTYPE* m_curr = nullptr;
    Node*  m_prev = nullptr;
};

using RTYPE = std::tuple<Node*, STYPE>;
using LTYPE = std::list<RTYPE>;


std::list<RTYPE> g_LIST_FoundE;
std::list<std::list<MTYPE*>> g_path_ptrs;
//std::map<std::pait<int,int>,char> g_DICT;

std::string get_str(const std::tuple<int, int, char>& tup){
    return "(" + std::to_string(std::get<0>(tup)) + ","
           + std::to_string(std::get<1>(tup)) + ","
           + std::get<2>(tup) + ")";
}


void extract_path(Node* nc, std::list<MTYPE*>& ls){
    while(nc){
        ls.push_back(nc->m_curr);
        nc = nc->m_prev;
    }
    if(true || g_PRINT) std::cout << "\n Paths extracted: " << ls.size();
}



void CLEAR_LTYPE(LTYPE& l_lst){
    if(g_PRINT) std::cout << "\n\t: Cleaning LTYPE of size " << l_lst.size() << "  ...";
    for(RTYPE& i_lst : l_lst){
        Node* m_ptr = std::get<0>(i_lst);
        if(m_ptr){
            if(g_PRINT) std::cout << "\n\t\t: deleting " << get_str(*m_ptr->m_curr);
            delete m_ptr;
            m_ptr = nullptr;
        }
    }
    l_lst.clear();
}





void do_union(){
    
    // -----------------------------
    //   DO UNION 
    // -----------------------------

    using MazePosType=std::pair<int,int>;
    std::list<MazePosType> m_posvalues;

    bool has_SPOS= false;
    bool has_EPOS= false;

    for(std::list<MTYPE*>p_lst : g_path_ptrs)
    {
        for(MTYPE* p_ptr : p_lst)
        {
            MazePosType p1;
            p1.first  = std::get<0>(*p_ptr);
            p1.second = std::get<1>(*p_ptr);

            bool is_present = false;

            for(const MazePosType& po : m_posvalues){
                if(po.first == p1.first && po.second == p1.second){
                    is_present = true;
                    break;
                }
            }

            if(!is_present)
            {
                if(!has_SPOS)
                {
                    has_SPOS = std::get<0>(p1) == std::get<0>(S_pos);
                    has_SPOS = has_SPOS && std::get<1>(p1) == std::get<1>(S_pos);
                }
                if(!has_EPOS)
                {
                    has_EPOS = std::get<1>(p1) == std::get<1>(E_pos);
                    has_EPOS = has_EPOS && std::get<1>(p1) == std::get<1>(E_pos);
                }
                m_posvalues.push_back(p1);
            }
        }
    }

    m_posvalues.sort();
    //int idxN=0;
    //for(const auto& val : m_posvalues){
    //    std::cout << "\n" << idxN++ << ": (" << val.first << "," << val.second << ")";
    //}

    auto total = m_posvalues.size();
    if(!has_SPOS){ total += 1;}
    if(!has_EPOS){ total += 1;}
    std::cout << "\nGood common Tiles in MAZE: " << total;
}



void initialize()
{
    std::ifstream fHandle(g_iFileName);

    if(!fHandle.is_open()){
        std::cout << "\nFAILED TO OPEN FILE: \"" << g_iFileName <<"\"\n";
    }

    int idx=0;
    int idy=0;
    int itr=0;
    char ch;
    while(fHandle.get(ch))
    {
        if(ch=='\n'){
            idx=0;
            ++idy;
            continue;
        }
        auto pos = std::make_pair(idx, idy);
        g_maze.insert({pos, std::make_pair(ch,0)});
        //std::cout << "\n" << itr++ << ": " << pos.m_x << "," << pos.m_y << " : " << ch;
        if(ch=='S'){
            S_pos=std::make_tuple(idx,idy,'S');
        }
        else if(ch=='E'){
            E_pos=std::make_tuple(idx,idy,'E');
        }
        ++idx;
    }
    fHandle.close();

    std::cout << "\n Size of g-MAZE: " << g_maze.size() << "\n";

    g_xMax=idx-1;
    g_yMax=idy;

    std::cout<<"g_xMax:" << g_xMax << "\n";
    std::cout<<"g_yMax:" << g_yMax << "\n";
    std::cout<<"S-Pos:" << get_str(S_pos) << "\n";
    std::cout<<"E-Pos:" << get_str(E_pos) << "\n";

}




LTYPE get_next(RTYPE& given_v)
{
    LTYPE v_list;
    Node* ptr_pos01 = std::get<0>(given_v);

    if(ptr_pos01 == nullptr){std::cout<<"\n BLUNDER BLUNDER!!!\n\n"; return v_list;}


    const int     p1x = std::get<0>(*(ptr_pos01->m_curr));
    const int     p1y = std::get<1>(*(ptr_pos01->m_curr));
    if(g_PRINT) std::cout << "\n\t: processing p1 " << get_str(*ptr_pos01->m_curr) << "\n";

    const STYPE score     = std::get<1>(given_v);
    const STYPE min_score = (g_scores.empty()) ? 0 : g_scores.front();
    if(g_PRINT) std::cout << "\n\t score: " << score << " and min. " << min_score;

    const char char_p1 = g_maze[std::make_pair(p1x,p1y)].first;
    STYPE&    score_p1 = g_maze[std::make_pair(p1x,p1y)].second;

    const bool found_E = char_p1=='E';
    const bool found_H = char_p1=='#';
    bool found_MIN = !g_scores.empty() && score >= g_scores.front();

    if(false){
        // do another current min. score for the position in MAZE based check
        // if for the given position in maze there exists a smaller score that means
        // the current path is not worth traversing
        if(score_p1 ==0 || score < score_p1){
            // this branch updates the MAZE position score with the current min. score
            score_p1=score;
        } else if(score_p1 !=0 && score >= score_p1){
            // this branch sets FLAG when current min. is smaller than the this iteration score
            found_MIN=true;
        }
    }


    const bool is_end_condition = found_E || found_H || found_MIN;
    if(found_E) {
        std::cout << "\n FOUND \"E\".. adding score-" << g_scores.size() << " -> " << score << "\n";
        g_scores.push_back(score);
        g_scores.sort();
        g_LIST_FoundE.push_back(std::make_tuple(ptr_pos01, score));
    }
    else if(found_MIN){
        if(g_PRINT) std::cout << " not better than current min. " << g_scores.front() << "\n";
    }

    if(false == is_end_condition){

        std::list<std::tuple<MTYPE, STYPE>> nxs_list;
        const char nxd=std::get<2>(*(ptr_pos01->m_curr));

        // POULATE NEXT 3 : A BLOCK OF ITS OWN
        {
            if(nxd=='n'){ // don't add SOUTH to avoid reversal
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x+1,p1y,'e'), score+1001));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y-1,'n'), score+1));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x-1,p1y,'w'), score+1001));
            }
            else if(nxd=='e' || nxd=='S'){ // don't add WEST to avoid reversal
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y+1,'s'), score+1001));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x+1,p1y,'e'), score+1));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y-1,'n'), score+1001));
            }
            else if(nxd=='s'){ // don't add NORTH to avoid reversal
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x-1,p1y,'w'), score+1001));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y+1,'s'), score+1));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x+1,p1y,'e'), score+1001));
            }
            else if(nxd=='w'){ // don't add EAST to avoid reversal
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y-1,'n'), score+1001));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x-1,p1y,'w'), score+1));
                nxs_list.push_back(std::make_tuple(std::make_tuple(p1x,p1y+1,'s'), score+1001));
            } else{
                std::cout << "BAD BAD CHOISE FOR NEXT!!!";
                nxs_list.clear();
            }
        }

        // validate elements in list and filter invalids; then recurse for each
        for(const auto& possible_n : nxs_list)
        {
            const MTYPE& p_next = std::get<0>(possible_n);
            STYPE       p_score = std::get<1>(possible_n);

            if(ptr_pos01 != nullptr && ptr_pos01->m_prev != nullptr &&
               std::get<0>(p_next) == std::get<0>(*(ptr_pos01->m_prev->m_curr)) && 
               std::get<1>(p_next) == std::get<1>(*(ptr_pos01->m_prev->m_curr)) &&
               std::get<2>(p_next) == std::get<2>(*(ptr_pos01->m_prev->m_curr))){
                std::cout << "\n Reversal FOund!!" << get_str(*(ptr_pos01->m_prev->m_curr)) << " vs. " << get_str(p_next);
                continue;
            }else if(std::get<0>(p_next)<0 || std::get<0>(p_next)>g_xMax){
                std::cout << "\n\t: X out of bound\n";
                continue;
            }else if(std::get<1>(p_next)<0 || std::get<1>(p_next)>g_yMax){
                std::cout << "\n\t: Y out of bpund!!\n";
                continue;
            }else{
                if(g_PRINT) std::cout << "\n\t: adding " << get_str(*ptr_pos01->m_curr)  << " -> " << get_str(p_next) << " score: " << p_score <<"\n";
                Node* p_node = new Node;
                p_node->m_curr = new MTYPE(p_next);
                p_node->m_prev = ptr_pos01;
                v_list.push_back({p_node, p_score});
            }
        }
    }

    //std::cout << "\33[2K" << "\r";
    return v_list;
}


int main(){
    initialize();

    LTYPE u_List;

    Node* S_pos_node = new Node;
    S_pos_node->m_curr = new MTYPE(S_pos);

    u_List.push_back({S_pos_node,0});

    while(true){

        LTYPE c_List;

        for(RTYPE& u : u_List)
        {
            LTYPE& x_List = get_next(u);

            if(!x_List.empty()){
                if(g_PRINT) std::cout << "\n GOT BACK " << x_List.size() << " RETURNS!!";
                std::copy(x_List.begin(), x_List.end(), std::back_inserter(c_List));
            } else{
                if(g_PRINT) std::cout << "\n RETURNED EMPTY!!";
            }
        }

        //CLEAR_LTYPE(u_List); 
        u_List.clear();

        if(c_List.empty()){
            std::cout << "\n BREAKING ON EMPTY!!";
            break;
        }
        else{
            //std::cout << "\n\t: Here is what is returned: ";
            //LTYPE::iterator c_iter = c_List.begin();
            //for(; c_iter != c_List.end(); ++c_iter){
            //    std::cout << "\n\t: " << get_str(*(std::get<0>(*c_iter)->m_curr));
            //}
            //std::cout << "\n";
        }

        std::copy(c_List.begin(), c_List.end(), std::back_inserter(u_List));

        //CLEAR_LTYPE(c_List);
        c_List.clear();
    }

    const auto minSCORE = g_scores.front();
    std::cout << "\n\n MIN SCORE: " << (g_scores.empty() ? 0 : minSCORE) ;


    std::cout << "\n\n EXTRACTING PATHS...\n\n";
    for(auto m_tup : g_LIST_FoundE){
        Node* m_ptr = std::get<0>(m_tup);
        STYPE m_scr = std::get<1>(m_tup);
        if(m_scr > minSCORE){
            std::cout << "\n\t: skipping as low score of " << m_scr;
        }
        else{
            std::list<MTYPE*> ls;
            extract_path(m_ptr, ls);
            g_path_ptrs.push_back(ls);
        }
    }

    do_union();

    // check and add S_pos if not there
    
    // check and add E_pos if not there

    return 0;
}
