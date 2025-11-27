#include <iostream>
#include <fstream>
#include "cp1.h"

std::string g_iFileName="sample1.txt";

//auto pos_cmp =[](const PostType& p1, const PostType& p2) -> bool{

std::map<PosType,char> g_maze;

auto S_pos=PosType(-1,-1);
auto E_pos=PosType(-1,-1);

int g_xMax=-1;
int g_yMax=-1;

MVDICT g_DICT;
std::list<MVLIST> g_lstmoves_toE;

std::list<std::string> g_keys_to_del;

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

int main()
{
    initialize();

    //print_maze();
    //return -1;

    std::list<MovType> g_movs_queue;
    std::vector<NodeType> g_paths;

    auto is_valid_candidate = [](const PosType& cPos, const MVLIST& gMovs, int xMax, int yMax, std::string& err){
        bool is_valid=true;
        if(cPos.m_x <0 || cPos.m_x > xMax){
            is_valid = false;
            err += " out-of-range x-pos; ";
        }
        else if(cPos.m_y <0 || cPos.m_y > yMax){
            is_valid = false;
            err += " out-of-range Y-pos; ";
        }
        else if(g_maze[cPos] == '#'){
            is_valid = false;
            err += " is HASH at (x,y) in maze; ";
        }
        else{
            for(const MovType& mv : gMovs){
                int px=mv.m_pos.m_x;
                int py=mv.m_pos.m_y;
                if(cPos.m_x==px && cPos.m_y==py) {
                    is_valid=false;
                    err += "is present already; ";
                    break;
                }
            }
        }
        return is_valid;
    };

    auto get_next_moves = [&](const MovType& currMov, const MVLIST& gVAL, MVLIST& canL, std::string& val_res){
        
        const auto& currPos = currMov.m_pos;
        MVLIST candidates;
        candidates.push_back(MovType(PosType(currPos.m_x, currPos.m_y-1), DIRTYPE::UP));
        candidates.push_back(MovType(PosType(currPos.m_x+1, currPos.m_y), DIRTYPE::RT));
        candidates.push_back(MovType(PosType(currPos.m_x, currPos.m_y+1), DIRTYPE::DN));
        candidates.push_back(MovType(PosType(currPos.m_x-1, currPos.m_y), DIRTYPE::LT));
        
        for(const auto& c : candidates){
            std::string err_str;
            if(is_valid_candidate(c.m_pos, gVAL, g_xMax, g_yMax, err_str)){
                val_res += "\n\t  valid: " + c.get_str();
                canL.emplace_back(c);
            }
            else{
                val_res += "\n\tINVALID: " + c.get_str() + ": " + err_str;
            }
        }
    };

    // start from the s-pos
    NUMTYPE i_iter=0;
    g_DICT.insert({get_new_key(i_iter), {MovType(S_pos,DIRTYPE::NL)}});

    while(!g_DICT.empty())
    {
        //for(MVDICT_ITR& d_iter=g_DICT.begin(); d_iter!=g_DICT.end(); ++d_iter) {
        KEYSLST currentKeysList;
        for(const auto& dict_item : g_DICT){
            currentKeysList.push_back(dict_item.first);
        }
        //std::cout<<"\n\nKeys found for this iter: "<< currentKeysList.size() << "\n";

        for(const KEYTYPE& cur_iter_key : currentKeysList) {
            // Fetch all the relevant data from current dictionary item
            const KEYTYPE& d_key = cur_iter_key;
            MVLIST& d_val =g_DICT[d_key];
            const MovType& d_bck = d_val.back();
            //----------------------------
            //std::string str_m = "\nFor current move-value \"" + d_bck.get_str() + "\"\n";
            std::cout << "\nLooking at 4 possible move-values for \"" + d_bck.get_str() + "\"\n";
            //----------------------------
            // obtain next set of moves
            std::list<MovType> nx_moves;
            bool found_E=false;// flag is SET when "E" is found
            bool found_Hash=false;// flag is SET when "#" is found
            bool is_curr_key_marked_for_del=false;
            KEYSLST clist_keys2del;
            //----------------------------
            NUMTYPE n_iter=0;
            std::string validity_res;
            get_next_moves(d_bck, d_val, nx_moves, validity_res);
            //str_m += validity_res;
            //std::cout << validity_res << "\n";
            //std::cout<<"\tNew valid positions found: "<< nx_moves.size() << "\n";
            //----------------------------
            for(const MovType& nx : nx_moves){
                const char maze_d=g_maze[nx.m_pos];
                found_E=maze_d=='E';
                found_Hash=maze_d=='#';
                if(found_Hash) std::cout << "\t:  has hit symbol \"#\"" << "\n";
                auto current_key=d_key;
                //----------------------------
                // str_m += "\t : added \"" + nx.get_str() + "\" to KEY:" + current_key + "\n";
                //----------------------------
                if(0==n_iter){
                    d_val.emplace_back(nx); // is equivalent to doing g_DICT[current_key].emplace_back(nx)
                    //str_m += "\t : added \"" + nx.get_str() + "\" to cur-KEY:" + current_key + "\n";
                    std::cout << "\t: added \"" + nx.get_str() + "\" to cur-KEY:" + current_key + "\n";
                    if(found_E || found_Hash){
                        is_curr_key_marked_for_del=true;
                    }
                }
                else{
                    i_iter += 1;
                    const auto newKey = get_new_key(i_iter);
                    current_key = newKey;
                    std::list<MovType> c_movlist;
                    //std::cout << "\t:  copying values from current list to generate new entry to dict...\n";
                    // copy the existing iteration key values
                    for(const auto& v : d_val){
                        c_movlist.emplace_back(v);
                    }
                    // add the new valid position
                    c_movlist.emplace_back(nx);
                    //str_m += "\t : added \"" + nx.get_str() + "\" to new-KEY:" + current_key + "\n";
                    std::cout << "\t: added \"" + nx.get_str() + "\" to new-KEY:" + current_key + "\n";
                    // insert the new moves list mapped against the new KEY
                    g_DICT.insert({newKey, c_movlist});
                    // if the new value inserted is "E" or "#" mark key for deletion
                    if(found_E || found_Hash){
                        clist_keys2del.push_back(newKey);
                    }
                }
                ++n_iter;
                //----------------------------
                if(found_E){
                    // if "E" is found then create a copy of the list
                    // that is mapped to the current key from the iteration
                    MVLIST newMvLst;
                    const MVLIST& cMvLst = g_DICT[current_key];
                    //std::cout << "\t:  cp/mv-ing values KEY:" + current_key + " as it found \"E\"...\n";
                    for(const auto& mv : cMvLst){
                        newMvLst.push_back(mv);
                    }
                    // push that list to the global list of move to reach  "E"
                    g_lstmoves_toE.emplace_back(newMvLst);
                    std::cout << "\t: " << g_lstmoves_toE.size() << " E ending list with " << newMvLst.size() << " elements added. ...\n"; 
                }
            }
            //std::cout << str_m;
            //----------------------------
            std::string str_k;
            //----------------------------
            if(nx_moves.empty() || is_curr_key_marked_for_del){
                // if no moves are found for the current position
                // OR if the current position is marked for deletion
                // then add it to the keys to be deleted
                // NOTE: current key can be marked for deletion at it
                // might have been moved to the global-list of moves
                // to reach "E"
                //std::cout << "\t:  added the top-cur KEY:" + d_key + " for deletion ...\n";
                clist_keys2del.push_back(d_key);
            }
            //----------------------------
            for(const auto& dK : clist_keys2del){
                // copy key strings inside the local list to the global
                // list of keys-marked-for-deletion so that these keys
                // can be used for overwriting related mempry mapped in
                // the g_DICT object in the upcoming iterations
                //std::cout << "\t:  marking KEY: " << dK << " for deletion ...\n";
                // str_k += " " + dK + ",";
                g_keys_to_del.emplace_back(dK);
            }
            clist_keys2del.clear();
            //if(!str_k.empty())
            //{
            //    str_k = "\tKeys to be deleted: " + str_k + "\n";
            //    std::cout << str_k;
            //    str_k = "";
            //}
        }
        for(const KEYTYPE& dK : g_keys_to_del){
            std::cout << "\t:  deleting KEY: " << dK << " ...\n";
            g_DICT.erase(dK);
        }
        g_keys_to_del.clear();
        //if(i_iter>125) return -1;
    }

    std::cout << "\n\nMIN SCORE: " << get_min_score(g_lstmoves_toE) << "\n";

    return 0;
}
