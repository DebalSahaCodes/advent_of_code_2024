#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <list>
#include <map>
#include <algorithm>

std::string g_iFileName="sample1old.txt";

std::map<std::pair<int,int>,std::pair<char,int>> g_maze;

//using MTYPE = std::tuple<int,int,char>;
using MTYPE = std::tuple<int,int,char,std::string>;
using RTYPE = std::tuple<std::pair<MTYPE,MTYPE>,int>;
using LTYPE = std::list<RTYPE>;

// global variable mapping key to paths
// cleared when found not the best path
using KEYTYPE=std::string;
using VALTYPE=std::tuple<std::list<MTYPE>, int>;
// std::string, std::tuple<std::list<MTYPE>, int>
using PTYPE=std::map<KEYTYPE, VALTYPE>;
PTYPE g_paths;

MTYPE S_pos;
MTYPE E_pos;

int g_xMax=-1;
int g_yMax=-1;

std::list<int> g_scores;

//std::map<std::pait<int,int>,char> g_DICT;

std::string get_str(const MTYPE& tup){
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
            S_pos=std::make_tuple(idx,idy,'S', "0");
        }
        else if(ch=='E'){
            E_pos=std::make_tuple(idx,idy,'E',"0xFFFF");
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


LTYPE get_next(const RTYPE& given_v)
{
    LTYPE v_list;

    const auto& pos01 = std::get<0>(given_v);
    const MTYPE& p0 = pos01.first;
    const MTYPE& p1 = pos01.second;
    //std::cout << "\n\t: processing p1 " << get_str(p1) << "\n";

    const int score = std::get<1>(given_v);
    const int min_score = (g_scores.empty()) ? -9999 : g_scores.front();
    //std::cout << "\n\t score: " << score << " and min. " << min_score;

    auto& maze_obj =  g_maze[std::make_pair(std::get<0>(p1),std::get<1>(p1))];
    const char& c_p1 = maze_obj.first;
    const bool found_E = c_p1=='E';
    const bool found_H = c_p1=='#';
    bool found_MIN = !g_scores.empty() && score >= g_scores.front();
    // do another current min. score for the position in MAZE based check
    // if for the given position in maze there exists a smaller score that means
    // the current path is not worth traversing
    int& score_p1 = maze_obj.second;
    if(score_p1 ==0 || score < score_p1){
        // this branch updates the MAZE position score with the current min. score
        score_p1=score;
    } else if(score_p1 !=0 && score > score_p1){
        // this branch sets FLAG when current min. is smaller than the this iteration score
        found_MIN=true;
    }

    const bool is_end_condition = found_E || found_H || found_MIN;
    if(found_E) {
        std::cout << "\n FOUND \"E\".. adding score-" << g_scores.size() << " -> " << score << "\n";
        g_scores.push_back(score);
        g_scores.sort();
    }
    else if(found_MIN || found_H){
        const std::string& key_p1 = std::get<3>(p1) ;
        if(found_MIN){
            std::cout << " \n no better than curr. min. "
            << g_scores.front() << ".. erasing " << key_p1
            << " from map ...\n";
        } else if(found_H){
            std::cout << " \n Found '#' so erasing "<< key_p1
            << " from map ...\n";
        }
        g_paths.erase(key_p1);
    }

    if(false == is_end_condition){
        std::list<std::tuple<MTYPE, int>> nxs_list;
        const auto& nxk = std::get<3>(p1); // p1 key
        const auto& nxx = std::get<0>(p1); // p1 x-position
        const auto& nxy = std::get<1>(p1); // p1 y-position
        const auto& nxd = std::get<2>(p1); // p1 direction
        if(nxd=='n'){ // don't add SOUTH to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx+1,nxy,'e',nxk), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy-1,'n',nxk), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx-1,nxy,'w',nxk), score+1001));
        }
        else if(nxd=='e' || nxd=='S'){ // don't add WEST to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy+1,'s',nxk), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx+1,nxy,'e',nxk), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy-1,'n',nxk), score+1001));
        }
        else if(nxd=='s'){ // don't add NORTH to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx-1,nxy,'w',nxk), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy+1,'s',nxk), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx+1,nxy,'e',nxk), score+1001));
        }
        else if(nxd=='w'){ // don't add EAST to avoid reversal
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy-1,'n',nxk), score+1001));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx-1,nxy,'w',nxk), score+1));
            nxs_list.push_back(std::make_tuple(std::make_tuple(nxx,nxy+1,'s',nxk), score+1001));
        } else{
            std::cout << "BAD BAD CHOISE FOR NEXT!!!" << "\"" << nxd << "\"";
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
                std::cout << "\n\t: adding " << get_str(p_old)  << " -> " << get_str(p_next) << " score: " << p_score <<"\n";
                v_list.push_back({std::make_pair(p_old, p_next), p_score});
            }
        }
    }

    //std::cout << "\33[2K" << "\r";
    return v_list;
}


std::string get_new_map_key(const std::string& curr_key, const int idx){
    return std::to_string(std::stoi(curr_key)+idx);
}

using VALUNIT_T=std::tuple<MTYPE, int>;
using VALUNIT_L=std::list<VALUNIT_T>;

void insert_into_GPATHS(const VALUNIT_L& nLIST, const KEYTYPE& origKey)
{
    for(const VALUNIT_T& nIter: nLIST){
        //------------------------------------------------------
        const int nScore   = std::get<1>(nIter);
        const MTYPE& nItem = std::get<0>(nIter);
        const std::string& n_key  = std::get<3>(nItem);
        //------------------------------------------------------
        if(g_paths.find(n_key) == g_paths.end()){
            // create a New Map-Entry with new-key and new list 
            // the new key should already be the one assigned to the iteration-item
            // In case
            VALTYPE newMapEntry;
            std::list<MTYPE>& nMap_mvList =  std::get<0>(newMapEntry);
            std::get<1>(newMapEntry) = nScore;
            // -------------------------------------------
            const VALTYPE& vDATA = g_paths[origKey];
            const std::list<MTYPE>& vdataList = std::get<0>(vDATA);
            //const int& vDataScore =  std::get<1>(vDATA);
            // -------------------------------------------
            std::copy(vdataList.cbegin(), vdataList.cend(), std::back_inserter(nMap_mvList));
            std::cout << "\n\t: creating new entry in GPATH for key: " << n_key;
            g_paths.insert({n_key, newMapEntry});
        }
        //------------------------------------------------------
        std::cout << "\n\t: inserting to the GPATH map with key: " << n_key;
        std::get<0>(g_paths[n_key]).push_back(nItem);
        std::get<1>(g_paths[n_key]) = nScore;
        //------------------------------------------------------
    }
}


void extract_g_paths_from_return(LTYPE& xLIST, int& idxITER)
{
    VALUNIT_L m_DATA; // list with score for the paths made by this list's elements
    KEYTYPE orig_key; // will be used later when calling lamda that will map pos-list and score
    //------------------------------------------------------
    LTYPE::iterator itr = xLIST.begin();
    std::string map_key = std::get<3>(std::get<0>(*itr).second);
    for(; itr != xLIST.end(); ++itr){
        RTYPE& itrV =  *itr;
        auto& pos_01 = std::get<0>(itrV);
        MTYPE& posn1  = pos_01.second;
        auto&  scor1 = std::get<1>(itrV);
        if(itr == xLIST.cbegin()){
            orig_key = map_key;
        }else{
            std::cout << "\n\t: added new key : " << map_key;
            map_key = get_new_map_key(map_key, ++idxITER);
        }
        //------ Update the KEY for the pos1 to be sent for G_PATHS processing ---
        std::get<3>(posn1) = map_key;
        //--------------------------------------
        VALUNIT_T dat1 = std::make_tuple(posn1, scor1);
        m_DATA.push_back(dat1);
    }
    //------------------------------------------------------
    // call lambda that inserts the positions with scores into the G_PATH
    insert_into_GPATHS(m_DATA, orig_key);
    //------------------------------------------------------
}

int main(){
    initialize();

    LTYPE u_List;
    u_List.push_back({{S_pos, S_pos}, 0});

    // intiialize the G_PATHS
    std::list<MTYPE> iList;
    iList.push_back(S_pos);
    std::string iKey = std::get<3>(S_pos);
    g_paths.insert({iKey, std::make_tuple(iList,0)});

    int i_iter=0;

    while(true){
        LTYPE c_List;
        for(const RTYPE& u : u_List){
            LTYPE& x_List = get_next(u);
            if(!x_List.empty()){
                extract_g_paths_from_return(x_List, i_iter);
                std::copy(x_List.cbegin(), x_List.cend(), std::back_inserter(c_List));
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
    std::cout << "\nTOTAL G_PATH size: " << g_paths.size();

    using MazePosType=std::pair<int,int>;
    std::map<MazePosType, int> m_selected;
    for(auto& [kp, vp] : g_paths){
        std::list<MTYPE>& mp = std::get<0>(vp);
        for(MTYPE& p : mp){
            MazePosType p1;
            p1.first  = std::get<0>(p);
            p1.second = std::get<1>(p);
            if(m_selected.find(p1) == m_selected.end()){
                m_selected.insert({p1,0});
            } else {
                m_selected[p1]+=1;
            }
        }
    }

    int idxN=0;
    for(const auto& [key, val] : m_selected){
        std::cout << "\n" << idxN << ": (" << key.first << "," << key.second << ")";
        ++idxN;
    }
    std::cout << "\nTOTAL Tiles in MAZE: " << m_selected.size();
    return 0;
}
