#include <string>
#include <vector>
#include <list>
#include <set>
#include <utility>
#include <map>
#include <memory>

using NUMTYPE=int;

enum class DIRTYPE{
    NL=-1,
    UP,
    RT,
    DN,
    LT
};

const std::string get_dir_str(const DIRTYPE& dirV){
    switch(dirV){
        case DIRTYPE::NL: return "-";
        case DIRTYPE::UP: return "UP";
        case DIRTYPE::RT: return "RT";
        case DIRTYPE::DN: return "DN";
        case DIRTYPE::LT: return "LT";
        default: return "-";
    }
}

struct PosType{
    int m_x=-1;
    int m_y=-1;
    PosType(){}
    ~PosType(){}
    PosType(int x, int y):m_x(x), m_y(y){}
    PosType(const PosType& p){
        this->m_x=p.m_x;
        this->m_y=p.m_y;
    }
    PosType& operator=(const PosType& p){
        this->m_x=p.m_x;
        this->m_y=p.m_y;
        return *this;
    }
    bool operator <(const PosType&) const;
    bool operator==(const PosType& p1)const {return m_x==p1.m_x && m_y==p1.m_y;}
    const std::string get_str() const;
};
bool PosType::operator <(const PosType& p) const {
    if(m_y < p.m_y){
        return true;
    }else if(m_y == p.m_y && m_x < p.m_x){
        return true;
    } else {
        return false;
    }
}
const std::string PosType::get_str() const{
    return std::to_string(m_x) + ":" + std::to_string(m_y);
}


struct MovType{
    PosType m_pos;
    DIRTYPE m_dir=DIRTYPE::NL;
    MovType(){}
    ~MovType(){}
    MovType(const PosType& pos, const DIRTYPE& dir):m_pos(pos), m_dir(dir){}
    MovType(const MovType& m){
        this->m_pos=m.m_pos;
        this->m_dir=m.m_dir;
    }
    MovType& operator=(const MovType& m){
        this->m_pos=m.m_pos;
        this->m_dir=m.m_dir;
        return *this;
    }
    bool operator==(const MovType& mv) const {return m_pos==mv.m_pos && m_dir==mv.m_dir;}
    const std::string get_str() const{
        return m_pos.get_str() + ":" + get_dir_str(m_dir);
    }
};

using MovUPtr=std::unique_ptr<MovType>;
using MovSPtr=std::shared_ptr<MovType>;

/*struct KEYTYPE : public std::string{
    
};*/
using KEYTYPE=std::string;
using KEYSLST=std::list<KEYTYPE>;
using MVLIST=std::list<MovType>;
using MVDICT=std::map<KEYTYPE, MVLIST>;
using MVDICT_ITR=MVDICT::iterator;

struct NodeType{
    MovSPtr m_prev;
    MovUPtr m_curr;
    NodeType(const MovType& mP, const MovType& mC){
        m_prev = std::make_shared<MovType>(mP);
        m_curr = std::make_unique<MovType>(mC);
    }
};

void print_moves_list(const MVLIST& mvList){
    std::cout << " \n\nGiven list: ";
    for(const auto& mv : mvList)
        std::cout << "\n\t: " << mv.get_str();
    std::cout << "\n";
}

NUMTYPE calculate_score_of_moves_list(const MVLIST& mvList)
{
    auto score_from_moves = [](const MovType& m0, const MovType& m1)->NUMTYPE{
        NUMTYPE score=0;
        const std::string& dstr0 = m0.m_pos.get_str() + ":" + get_dir_str(m0.m_dir);
        const std::string& dstr1 = m1.m_pos.get_str() + ":" + get_dir_str(m1.m_dir);
        if(dstr0 != "-" && dstr1 != "-" && (m0.m_dir != m1.m_dir)) {
            std::cout << "\n\t: " + dstr0 + " to " + dstr1 << ", score for 90 turn";
            score+=1000;
        }
        if(m0.m_pos.m_x != m1.m_pos.m_x && m0.m_pos.m_y == m1.m_pos.m_y) {
            std::cout << "\n\t: " + dstr0 + " to " + dstr1 << ", score for x move";
            score+=1;
        }
        else if(m0.m_pos.m_y != m1.m_pos.m_y && m0.m_pos.m_x == m1.m_pos.m_x) {
            std::cout << "\n\t: " + dstr0 + " to " + dstr1 << ", score for Y move";
            score+=1;
        }
        return score;
    };

    NUMTYPE i_iter = 0;
    MovType o_mov;
    NUMTYPE score1 = 0;
    for(const MovType& d_mov : mvList){
        if(i_iter == 0){
            o_mov=d_mov;
        }
        else{
            const auto score_c = score_from_moves(o_mov, d_mov);
            if(score_c == -1){
                return -1;
            }
            score1 += score_c;
            o_mov=d_mov;
        }
        ++i_iter;
    }

    std::cout << "\nLEN: " << mvList.size() <<" SCORE: " << score1 << "\n";
    return score1;
}

NUMTYPE get_min_score(const std::list<MVLIST>& given_lists)
{
    NUMTYPE min_score=0;

    for(const MVLIST& c_list : given_lists){
        print_moves_list(c_list);
        const NUMTYPE score = calculate_score_of_moves_list(c_list);
        if(min_score == 0 || min_score > score){
            min_score = score;
        }
    }

    return min_score;
}
