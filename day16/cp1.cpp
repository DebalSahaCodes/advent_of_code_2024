#include <iostream>
#include <fstream>
#include "cp1.h"

std::string g_iFileName="puzzle.txt";

//auto pos_cmp =[](const PostType& p1, const PostType& p2) -> bool{

std::map<PosType,char> g_maze;

auto S_pos=PosType(-1,-1);
auto E_pos=PosType(-1,-1);

int g_xMax=-1;
int g_yMax=-1;

MVDICT g_DICT;
std::list<MVLIST> g_lstmoves_toE;
std::list<std::string> g_keys_to_del;
std::list<NUMTYPE> g_scores;
std::map<KEYTYPE,NUMTYPE> g_key_vs_score;

void initialize()
{
    std::ifstream fHandle(g_iFileName, std::ios_base::in);

    int idx=0;
    int idy=0;
    int itr=0;
    char ch;
    while(fHandle.get(ch))
    {
        if(ch=='\n')
        {
            idx=0;
            ++idy;
            continue;
        }
        const PosType pos(idx, idy);
        g_maze.insert({pos, ch});
        //std::cout << "\n" << itr++ << ": " << pos.m_x << "," << pos.m_y << " : " << ch;
        if(ch=='S'){
            S_pos=PosType(idx,idy);
        }
        else if(ch=='E'){
            E_pos=PosType(idx,idy);
        }
        ++idx;
    }
    fHandle.close();

    std::cout << "\n Size of g-MAZE: " << g_maze.size() << "\n";

    g_xMax=idx-1;
    g_yMax=idy;

    std::cout<<"xMax:" << g_xMax << "\n";
    std::cout<<"yMax:" << g_yMax << "\n";
    std::cout<<"S-Pos:" << S_pos.get_str() << "\n";
    std::cout<<"E-Pos:" << E_pos.get_str() << "\n";

}

void print_maze(){
    for(int y=0; y<=g_yMax; ++y){
        for(int x=0; x<= g_xMax; ++x){
            std::cout << g_maze[PosType(x,y)];
        }
        std::cout << "\n";
    }
}

std::string get_new_key(NUMTYPE iterIDX)
{
    std::string key_str;
    if(!g_keys_to_del.empty()) {
        key_str=g_keys_to_del.front();
        g_keys_to_del.pop_front();
    }
    else{
        key_str = std::to_string(iterIDX);
    }
    return key_str;
}


bool all_end_GDICT(){
    bool res=true;
    for(const auto& d : g_DICT){
        const auto& l_pos = d.second.back().m_pos;
        const char& l_chr = g_maze[l_pos];
        res = res && (l_chr=='#' || l_chr=='E');
        if(!res){
            res = false;
            break;
        }
    }
     const auto res_str = (res) ? "1" : "0";
    //std::cout <<"RESULT: " << res_str << "\n";
    return res;
}


void chop_off_more_than_100_and_store_score(const KEYTYPE& key){
    std::string print_str ="\n\n CHOPPING OFF 100 elements from KEY:" + key;
    int idx=0;
    int ndx=100;
    std::list<MovType>& gLIST = g_DICT[key];
    std::list<MovType> c_lst;
    for(;idx < ndx; ++idx){
        c_lst.push_back(gLIST.front());
        gLIST.pop_front();
    }
    NUMTYPE n_score=calculate_score_of_moves_list(c_lst);
    auto i_key = g_key_vs_score.find(key);
    if(i_key != g_key_vs_score.end()){
        g_key_vs_score[key] += n_score;
    } else{
        g_key_vs_score.insert({key, n_score});
    }
    print_str += " SCORE: " + std::to_string(n_score)  + "\n";
    std::cout << print_str;
}

int main()
{
    initialize();

    //print_maze();
    //return -1;

    NUMTYPE i_iter=0;
    // start from the s-pos: it is direction-move in RT direction
    g_DICT.insert({"0", {MovType(S_pos,DIRTYPE::RT)}});

    while(!g_DICT.empty() && !all_end_GDICT())
    {
        KEYSLST currentKeysList;
        for(const auto& dict_item : g_DICT){
            currentKeysList.push_back(dict_item.first);
        }

        for(const KEYTYPE& cur_iter_key : currentKeysList) {
            //----------------------------
            const KEYTYPE& d_key = cur_iter_key;
            //----------------------------
            // chop off list till 5 if n_elements > 105
            if(105<g_DICT[d_key].size()){
                chop_off_more_than_100_and_store_score(d_key);
            }
            //----------------------------
            MVLIST& d_val = g_DICT[d_key];
            const MovType& d_bck = d_val.back();
            //----------------------------
            //std::cout << "Looking at 4 possible move-values for \"" + d_bck.get_str() + "\" for KEY:" << cur_iter_key << "\n";
            std::list<MovType> nx_moves;
            std::string msg;
            //----------------------------
            // get_next_moves(d_bck, d_val, nx_moves, msg);
            const auto& currPos = d_bck.m_pos;
            MVLIST candidates;
            candidates.push_back(MovType(PosType(currPos.m_x, currPos.m_y-1), DIRTYPE::UP));
            candidates.push_back(MovType(PosType(currPos.m_x+1, currPos.m_y), DIRTYPE::RT));
            candidates.push_back(MovType(PosType(currPos.m_x, currPos.m_y+1), DIRTYPE::DN));
            candidates.push_back(MovType(PosType(currPos.m_x-1, currPos.m_y), DIRTYPE::LT));
            for(const auto& c : candidates){
            //---------------
                bool is_valid_candidate = false;
                if(c.m_pos.m_x >=0 && c.m_pos.m_x <= g_xMax && c.m_pos.m_y >=0 && c.m_pos.m_y <= g_yMax){
                    is_valid_candidate = true;
                    for(const MovType& mv : d_val){
                        int px=mv.m_pos.m_x;
                        int py=mv.m_pos.m_y;
                        if(c.m_pos.m_x==px && c.m_pos.m_y==py) {
                            is_valid_candidate=false;
                            break;
                        }
                    }
                }
                if(is_valid_candidate){
                    nx_moves.emplace_back(c);
                }
                //---------------
            }
            //----------------------------
            size_t n_is_added = 0;
            //bool found_E = false;
            //bool found_H = false;
            for(const MovType& nx : nx_moves){
                //----------------------------
                KEYTYPE current_key=d_key;
                //----------------------------
                const char maze_d=g_maze[nx.m_pos];
                //----------------------------
                const bool found_E=maze_d=='E';
                const bool found_H=maze_d=='#';
                //----------------------------
                if(found_E)    std::cout << "\t:  has found symbol \""<<maze_d<<"\" for KEY:" << current_key << "\n";
                //if(found_H) std::cout << "\t:  has hit symbol \"#\"" << "\n";
                //----------------------------
                if(0==n_is_added){
                    if(found_H){
                        // do nothing
                    }else{
                        //std::cout << "\t: added \"" + nx.get_str() + "\" to cur-KEY:" + current_key + "\n";
                        d_val.emplace_back(nx);
                        ++n_is_added;
                    }
                }
                else{
                    if(found_H){
                        // do nothing
                    }else{
                        //const auto newKey = get_new_key(i_iter);
                        const std::string newKey = std::to_string(++i_iter);
                        current_key = newKey;
                        // ------------------------------------------
                        std::list<MovType> c_movlist;
                        // ------------------------------------------
                        //std::cout << "\t: copied KEY:" << d_key <<" data to new-KEY:" + newKey + "\n";
                        std::copy(d_val.begin(), d_val.end(), std::back_inserter(c_movlist)); // copy existing
                        auto i_key = g_key_vs_score.find(current_key); // also copy chopped off score
                        if(i_key != g_key_vs_score.end()){
                            g_key_vs_score.insert({newKey, g_key_vs_score[current_key]});
                        }
                        /*for(const auto& mv : d_val){
                            std::cout << "\t: copied \"" + mv.get_str() + "\" from KEY:" << d_key <<" to new-KEY:" + newKey + "\n";
                            c_movlist.push_back(mv);
                        }*/
                        const auto& last_item = d_val.back();
                        if(false == (d_bck == last_item)){
                            c_movlist.pop_back(); // last element was added for another list when n_is_added==0
                        }
                        // ------------------------------------------
                        c_movlist.emplace_back(nx); // add new
                        //std::cout << "\t: added \"" + nx.get_str() + "\" to new-KEY:" + newKey + "\n";
                        g_DICT.insert({newKey, c_movlist});
                        // ------------------------------------------
                        ++n_is_added;
                    }
                }
                //----------------------------
                if(found_E){// if "E" is found 
                    const MVLIST& cMvLst = g_DICT[current_key];
                    MVLIST e_List; MovType o_mv; int i=0;
                    std::string str_err="";
                    str_err="\nPOST_FOUND E FOR KEY:"+current_key+"\n";
                    for(const auto& c_mv : cMvLst){
                        //std::cout << "\n\tAdding " << c_mv.get_str() << " after " << o_mv.get_str();
                        if(i != 0 && o_mv.m_pos.m_x != c_mv.m_pos.m_x && o_mv.m_pos.m_y != c_mv.m_pos.m_y){
                            str_err += "\n: BAD BAD!!!\n\n";
                            std::cout << str_err;
                            return -1;
                            //continue;
                        }
                        else{
                            e_List.push_back(c_mv);
                        }
                        o_mv = c_mv;
                        ++i;
                    }
                    //-------------------------------------------
                    //g_lstmoves_toE.push_back(e_List);
                    NUMTYPE moves_score = calculate_score_of_moves_list(cMvLst, true);
                    // add score from the chopped off map if anything mapped to key there
                    auto i_key = g_key_vs_score.find(current_key);
                    if(i_key != g_key_vs_score.end()){
                        moves_score += g_key_vs_score[current_key];
                    }
                    std::cout << "\n Score of key: " << current_key << " is " << moves_score <<"\n";
                    g_scores.push_back(moves_score);
                    g_keys_to_del.push_back(d_key);
                    //-------------------------------------------
                }
            }
            //std::cout << str_m;
            //----------------------------
            std::string str_k;
            //----------------------------
            if(n_is_added == 0 || nx_moves.empty()){
                // if no moves are found for the current position
                //std::cout << "\t:  added the top-cur KEY:" + d_key + " for deletion ...\n";
                g_keys_to_del.push_back(d_key);
            }
        }

        for(const KEYTYPE& dK : g_keys_to_del){
            //std::cout << "\t:  deleting KEY: " << dK << " ...\n";
            g_DICT.erase(dK);
        }
        g_keys_to_del.clear();

    }

    /*for(const auto& cs_list: g_lstmoves_toE){
        const NUMTYPE moves_score = calculate_score_of_moves_list(cs_list);
        std::cout << "\n LEN: " << cs_list.size() << " Score:" << moves_score <<"\n";
        g_scores.push_back(moves_score);
    }*/

    g_scores.sort();
    int idx=0;
    for(const auto s : g_scores){
        std::cout << "\nScore-" << idx++ << ": " << s ;
    }
    std::cout << "\n\nMIN SCORE: " << g_scores.front() << "\n";

    return 0;
}
