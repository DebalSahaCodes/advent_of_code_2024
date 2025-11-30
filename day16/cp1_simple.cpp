#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <list>
#include <map>
#include <algorithm>



std::string g_iFileName="puzzle.txt";

std::map<std::pair<int,int>,std::pair<char,int>> g_maze;

std::tuple<int, int, char> S_pos;
std::tuple<int, int, char> E_pos;

int g_xMax=-1;
int g_yMax=-1;

std::list<int> g_scores;

//std::map<std::pait<int,int>,char> g_DICT;

std::string get_str(const std::tuple<int, int, char>& tup){
    return "(" + std::to_string(std::get<0>(tup)) + ","
           + std::to_string(std::get<1>(tup)) + ","
+ std::get<2>(tup) + ")";
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

using MTYPE = std::tuple<int,int,char>;
using RTYPE = std::tuple<std::pair<MTYPE, MTYPE>, int>;
using LTYPE = std::list<RTYPE>;

LTYPE get_next(const RTYPE& given_v)
{
    LTYPE v_list;

    const auto& pos01 = std::get<0>(given_v);
    const std::tuple<int, int, char>& p0 = pos01.first;
    const std::tuple<int, int, char>& p1 = pos01.second;
    //std::cout << "\n\t: processing p1 " << get_str(p1) << "\n";

    const int score = std::get<1>(given_v);
    const int min_score = (g_scores.empty()) ? -9999 : g_scores.front();
    //std::cout << "\n\t score: " << score << " and min. " << min_score;

    const char c_p1 = g_maze[std::make_pair(std::get<0>(p1),std::get<1>(p1))].first;
    const bool found_E = c_p1=='E';
    const bool found_H = c_p1=='#';
    bool found_MIN = !g_scores.empty() && score >= g_scores.front();
    // do another current min. score for the position in MAZE based check
    // if for the given position in maze there exists a smaller score that means
    // the current path is not worth traversing
    int& score_p1 = g_maze[std::make_pair(std::get<0>(p1),std::get<1>(p1))].second;
    if(score_p1 ==0 || score < score_p1){
        // this branch updates the MAZE position score with the current min. score
        score_p1=score;
    } else if(score_p1 !=0 && score >= score_p1){
        // this branch sets FLAG when current min. is smaller than the this iteration score
        found_MIN=true;
    }

    const bool is_end_condition = found_E || found_H || found_MIN;
    if(found_E) {
        std::cout << "\n FOUND \"E\".. adding score-" << g_scores.size() << " -> " << score << "\n";
        g_scores.push_back(score);
        g_scores.sort();
    }
    else if(found_MIN){
        std::cout << " not better than current min. " << g_scores.front() << "\n";
    }

    if(false == is_end_condition){
        std::list<std::tuple<std::tuple<int, int, char>, int>> nxs_list;
        const char nxd=std::get<2>(p1);
        if(nxd=='n'){ // don't add SOUTH to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)+1,std::get<1>(p1),'e'), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)-1,'n'), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)-1,std::get<1>(p1),'w'), score+1001));
        }
        else if(nxd=='e' || nxd=='S'){ // don't add WEST to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)+1,'s'), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)+1,std::get<1>(p1),'e'), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)-1,'n'), score+1001));
        }
        else if(nxd=='s'){ // don't add NORTH to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)-1,std::get<1>(p1),'w'), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)+1,'s'), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)+1,std::get<1>(p1),'e'), score+1001));
        }
        else if(nxd=='w'){ // don't add EAST to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)-1,'n'), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1)-1,std::get<1>(p1),'w'), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(std::get<0>(p1),std::get<1>(p1)+1,'s'), score+1001));
        } else{
            std::cout << "BAD BAD CHOISE FOR NEXT!!!";
            nxs_list.clear();
        }
        // validate elements in list and filter invalids; then recurse for each
        for(const auto& possible_n : nxs_list){
            auto& p_next = std::get<0>(possible_n);
            int  p_score = std::get<1>(possible_n);
            if(std::get<0>(p_next) == std::get<0>(p0) && 
               std::get<1>(p_next) == std::get<1>(p0) &&
               std::get<2>(p_next) == std::get<2>(p0)){
                std::cout << "\n Reversal FOund!!" << get_str(p0) << " vs. " << get_str(p_next);
                continue;
            }else if(std::get<0>(p_next)<0 || std::get<0>(p_next)>g_xMax){
                std::cout << "\n\t: X out of bound\n";
                continue;
            }else if(std::get<1>(p_next)<0 || std::get<1>(p_next)>g_yMax){
                std::cout << "\n\t: Y out of bpund!!\n";
                continue;
            }else{
                const auto& p_old = p1;
                //std::cout << "\n\t: adding " << get_str(p_old)  << " -> " << get_str(p_next) << " score: " << p_score <<"\n";
                v_list.push_back({std::make_pair(p_old, p_next), p_score});
            }
        }
    }

    //std::cout << "\33[2K" << "\r";
    return v_list;
}

int main(){
    initialize();

    LTYPE u_List;
    u_List.push_back({{S_pos, S_pos}, 0});
    while(true){
        LTYPE c_List;
        for(const RTYPE& u : u_List){
            const LTYPE& x_List = get_next(u);
            if(!x_List.empty()){
                std::copy(x_List.begin(), x_List.end(), std::back_inserter(c_List));
            } else{
                //std::cout << "\n RETURNED EMPTY!!";
            }
        }
        if(c_List.empty())
            break;
        u_List.clear();
        std::copy(c_List.begin(), c_List.end(), std::back_inserter(u_List));
    }

    std::cout << "\n\n\n MIN SCORE: " << (g_scores.empty() ? -9999 : g_scores.front()) ;

    return 0;
}
