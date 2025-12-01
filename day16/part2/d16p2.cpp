#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <list>
#include <map>
#include <algorithm>
#include <set>

std::string g_iFileName="sample2.txt";

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
    bool found_MIN = !g_scores.empty() && score > g_scores.front();
    if(found_MIN==false) // also check if the maze_pos has a better score
    {
        auto& mz_score = maze_obj.second;
        found_MIN = mz_score!=0 && mz_score < score;
        if(found_MIN){
            std::cout << "\n curr-score " << score << " EXCEEDS maze_pos-score " << mz_score;
        }else if(mz_score > score){
            mz_score = score;
        }
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


std::string get_new_map_key(const std::string& curr_key){
    static int idx=0;
    return std::to_string(std::stoi(curr_key) + idx++);
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
            map_key = get_new_map_key(map_key);
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



void do_union_method1(){
    int minSCORE =  g_scores.front();
    // -----------------------------
    //   DO INTERSECTIOM 
    // -----------------------------
    using MAZEPOS=std::pair<int,int>;
    using LstMAZEPOS=std::list<MAZEPOS>;

    std::list<LstMAZEPOS> m_selected;

    int vpIDX=0;
    for(auto& [kp, vp] : g_paths)
    {
        LstMAZEPOS s_selected;
        std::map<std::pair<int,int>,int> u_selected; // unique keys

        std::list<MTYPE>& mp = std::get<0>(vp);
        const auto& currSCORE = std::get<1>(vp);
        std::cout << "\n\t: score for path-" << vpIDX++ << " :" << currSCORE;
        if(minSCORE < currSCORE){
            std::cout << " .. skipping";
            continue;
        }
        std::cout << " .. adding " << mp.size() << " positions";

        auto found_key = [&u_selected](const std::pair<int,int>& pkey) -> bool {
            bool found = false;
            for(const auto& [k,v] : u_selected){
                if(k.first == pkey.first && k.second == pkey.second){
                    found = true;
                    break;
                }
            }
            return found;
        };

        for(MTYPE& p : mp){
            MAZEPOS p1;
            p1.first  = std::get<0>(p);
            p1.second = std::get<1>(p);
            if(false == found_key(p1)){
                u_selected.insert({p1,0});
            }else{
                u_selected[p1] += 1;
            }
        }
        for(const auto& [kPos, v_notused] : u_selected){
            s_selected.push_back(kPos);
        }
        if(s_selected.empty() == false){
            m_selected.push_back(s_selected);
        }
    }

    //m_selected.sort();
    LstMAZEPOS m_posvalues;

    if(m_selected.size()>1){

        std::list<LstMAZEPOS>::iterator iter = m_selected.begin();

        // copy the smallest list (sorted so assuming smallest at front)
        std::copy(iter->begin(), iter->end(), std::back_inserter(m_posvalues));

        ++iter; // now filter common positions iterating from SECOND in the list

        for(;iter != m_selected.end(); ++iter){
            LstMAZEPOS c_posLST;
            for(const auto& mp : m_posvalues){
                for(const auto& ip : *iter){
                    if(mp.first==ip.first && mp.second==ip.second) {
                        std::cout << "\n\t\t: common maze tile = (" << mp.first << "," << mp.second << ")";
                        c_posLST.push_back(mp); // found common so add
                    }
                }
            }

            m_posvalues.clear();
            std::copy(c_posLST.begin(), c_posLST.end(), std::back_inserter(m_posvalues));
        }
    } else{
        std::copy(m_selected.front().begin(), m_selected.front().end(), std::back_inserter(m_posvalues));
    }

    std::cout << "\nGood common Tiles in MAZE: " << m_posvalues.size();
}





void do_union_method2(){
    int minSCORE =  g_scores.front();
    // -----------------------------
    //   DO INTERSECTIOM 
    // -----------------------------
    using MAZEPOS=std::pair<int,int>;
    using LstMAZEPOS=std::list<MAZEPOS>;

    std::list<std::list<MTYPE>> m_selected;

    int vpIDX=0;
    for(auto& [kp, vp] : g_paths)
    {
        std::list<MTYPE> s_selected;
        std::map<MTYPE,int> u_selected; // unique keys

        std::list<MTYPE>& mp = std::get<0>(vp);
        const auto& currSCORE = std::get<1>(vp);
        std::cout << "\n\t: score for path-" << vpIDX++ << " :" << currSCORE;
        if(minSCORE < currSCORE){
            std::cout << " .. skipping";
            continue;
        }
        std::cout << " .. adding " << mp.size() << " positions";

        auto found_key = [&u_selected](const MTYPE& pkey) -> bool {
            bool found = false;
            for(const auto& [k,v] : u_selected){
                bool is_same = true;
                is_same = is_same && std::get<0>(pkey) == std::get<0>(k);
                is_same = is_same && std::get<1>(pkey) == std::get<1>(k);
                is_same = is_same && std::get<2>(pkey) == std::get<2>(k);
                if(is_same){
                    found = true;
                    break;
                }
            }
            return found;
        };

        for(MTYPE& p : mp){
            if(false == found_key(p)){
                u_selected.insert({p,0});
            }else{
                u_selected[p] += 1;
            }
        }
        for(const auto& [kPos, v__] : u_selected){
            s_selected.push_back(kPos);
        }
        if(s_selected.empty() == false){
            m_selected.push_back(s_selected);
        }
    }

    std::list<MTYPE> m_posvalues;
    bool has_SPOS=false;
    bool has_EPOS=false;

    if(m_selected.size()>1){

        std::list<std::list<MTYPE>>::iterator iter = m_selected.begin();

        // copy the smallest list (sorted so assuming smallest at front)
        std::copy(iter->begin(), iter->end(), std::back_inserter(m_posvalues));

        ++iter; // now filter common positions iterating from SECOND in the list

        for(;iter != m_selected.end(); ++iter){
            std::list<MTYPE> c_posLST;
            std::cout << "\n\t: this iteration ....";
            for(const auto& mp : m_posvalues){
                for(const auto& ip : *iter){
                    bool is_same = true;
                    is_same = is_same && std::get<0>(mp)==std::get<0>(ip);
                    is_same = is_same && std::get<1>(mp)==std::get<1>(ip);
                    is_same = is_same && std::get<2>(mp)==std::get<2>(ip);
                    if(is_same) {
                        std::cout << "\n\t: common maze tile = " << get_str(mp);
                        if(!has_SPOS){
                            has_SPOS = std::get<0>(mp) == std::get<0>(S_pos);
                            has_SPOS = has_SPOS && std::get<1>(mp) == std::get<1>(S_pos);
                            has_SPOS = has_SPOS && std::get<2>(mp) == std::get<2>(S_pos);
                        }
                        if(!has_EPOS){
                            has_EPOS = std::get<0>(mp) == std::get<0>(E_pos);
                            has_EPOS = has_EPOS && std::get<1>(mp) == std::get<1>(E_pos);
                            //has_EPOS = has_EPOS && std::get<2>(mp) == std::get<2>(S_pos);
                        }
                        c_posLST.push_back(mp); // found common so add
                    }
                }
            }

            m_posvalues.clear();
            std::copy(c_posLST.begin(), c_posLST.end(), std::back_inserter(m_posvalues));
        }
    } else{
        std::copy(m_selected.front().begin(), m_selected.front().end(), std::back_inserter(m_posvalues));
    }

    auto total = m_posvalues.size();
    if(!has_SPOS){ total += 1;}
    if(!has_EPOS){ total += 1;}
    std::cout << "\nGood common Tiles in MAZE: " << total;
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

    if(g_scores.empty()){
        std::cout << "\n\nFAILED TO OBTAIN MIN. SCORE!!!\n\n";
        return -1;
    }

    int minSCORE =  g_scores.front();
    std::cout << "\n\n\n MIN SCORE: " << minSCORE;
    std::cout << "\nTOTAL G_PATH size: " << g_paths.size();

    //auto print_moves = [](const VALTYPE=std::tuple<std::list<MTYPE>, int>;
    auto print_moves = [](const VALTYPE& pathV){
        const std::list<MTYPE>& values = std::get<0>(pathV);
        for(const auto& v : values){
            std::cout << "\n\t: " << get_str(v);  //(" << val.first << "," << val.second << ")";
        }
    };

/*
    // -----------------------------
    //   DO UNION 
    // -----------------------------
    using MazePosType=std::pair<int,int>;
    std::list<MazePosType> m_posvalues;
    int vpIDX=0;
    for(auto& [kp, vp] : g_paths){
        std::list<MTYPE>& mp = std::get<0>(vp);
        const auto& currSCORE = std::get<1>(vp);
        std::cout << "\n\t: score for path-" << vpIDX++ << " :" << currSCORE;
        if(minSCORE < currSCORE){
            std::cout << " .. skipping";
            continue;
        }
        std::cout << " .. adding " << mp.size() << " positions";
        for(MTYPE& p : mp){
            MazePosType p1;
            p1.first  = std::get<0>(p);
            p1.second = std::get<1>(p);
            bool is_present = false;
            for(const MazePosType& po : m_posvalues){
                if(po.first == p1.first && po.second == p1.second){
                    is_present = true;
                    break;
                }
            }
            if(!is_present){
                m_posvalues.push_back(p1);
            }
        }
    }

    m_posvalues.sort();
    //int idxN=0;
    //for(const auto& val : m_posvalues){
    //    std::cout << "\n" << idxN++ << ": (" << val.first << "," << val.second << ")";
    //}

    std::string new_MAZE,str1;
    std::ifstream iHdl(g_iFileName);
    while(iHdl >> str1){
        new_MAZE = str1;
    }
    iHdl.close();
    //std::ofstream iHdl = open("sol_" + g_iFileName, std::);
*/


    do_union_method2();

    //int sidx=0;
    //for(const auto& msel = m_selected.begin(); msel != m_selected.end(); ++msel, ++sidx){
    //    if(sidx > 0){
    //        LstMAZEPOS c_temp;
    //        for(const auto& s : *msel){
    //            for(const auto& p : m_posvalues){
    //                if(s.first==p.first && 
    //        }
    //    }
    //}

    return 0;
}
