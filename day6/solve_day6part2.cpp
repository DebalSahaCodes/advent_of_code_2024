#include<iostream>
#include<fstream>
#include<list>
#include<string>
#include<functional>
#include<algorithm>
#include<execution>


enum class DirEnumType
{
    UNDEFINED=-1,
    NORTH=1,
    EAST,
    SOUTH,
    WEST
};

struct DirType
{
    DirEnumType _data;

    static std::string read_dir(const DirEnumType& d)
    {
        switch(d)
        {
            case DirEnumType::NORTH: return "North";
            case DirEnumType::EAST: return "East";
            case DirEnumType::SOUTH: return "South";
            case DirEnumType::WEST: return "West";
            default:std::cout << "ERROR"; return "";
        }
    }

    DirType(const DirEnumType& dirVal):_data(dirVal){}

    DirType():_data(DirEnumType::UNDEFINED){}

    const std::string str_dir() const
    {
        return DirType::read_dir(_data);
    }

    bool operator==(const DirType& dir) const
    {
        return dir._data==this->_data;
    }

    static void turn_right(DirEnumType& d)
    {
        switch(d)
        {
            case DirEnumType::NORTH: d=DirEnumType::EAST; break;
            case DirEnumType::EAST: d=DirEnumType::SOUTH; break;
            case DirEnumType::SOUTH: d=DirEnumType::WEST; break;
            case DirEnumType::WEST: d=DirEnumType::NORTH; break;
            default:std::cout << "ERROR in turn right";
        }
    }
};

struct PosType
{
    int _x=-9999;
    int _y=-9999;

    PosType(int x, int y):_x(x), _y(y){}

    PosType(){}

    std::string str_pos() const
    {
        return std::to_string(this->_x) + "," + std::to_string(this->_y);
    }

    bool operator==(const PosType& posGiven) const
    {
        return posGiven._x==this->_x &&  posGiven._y==this->_y;
    }

    bool is_valid() const
    {
        return _x!=-9999 && _y!=-9999;
    }

    static const PosType get_next_pos(const int x, const int y, const DirEnumType& d)
    {
        PosType pos(x,y);
        switch(d)
        {
            case DirEnumType::NORTH: --pos._y; break;
            case DirEnumType::EAST: ++pos._x; break;
            case DirEnumType::SOUTH: ++pos._y; break;
            case DirEnumType::WEST: --pos._x; break;
            default:std::cout << "ERROR in pos dir";
        }
        return pos;
    }
};

struct MoveType
{
    PosType _pos;
    DirType _dir;

    MoveType(const int x, const int y, const DirEnumType& dirEnum) : _dir(DirType(dirEnum))
    {
        _pos = PosType(x,y);
    }

    MoveType(const int x, const int y, const DirType& dir)
    {
        _pos = PosType(x,y);
        _dir = dir;
    }

    MoveType() = delete;

    std::string str_move() const
    {
        return "pos (" + _pos.str_pos() + "); dir: " + _dir.str_dir();
    }

    bool operator==(const MoveType& move) const
    {
        return move._pos==this->_pos && move._dir==this->_dir;
    }

    void turn_right()
    {
        DirType::turn_right(this->_dir._data);
    }

    int x() const{
        return _pos._x;
    }
    int y() const{
        return _pos._y;
    }
    DirEnumType d() const{
        return _dir._data;
    }
};




// Lambda : add_uinque
void add_unique (const int x, const int y, std::list<PosType>& total_positions)
{
    PosType pos(x,y);
    for(const auto& p : total_positions)
    {
        if(p == pos)
        {
            //std::cout << "Adding skipped for " << x << "," << y << "\n";
            return;
        }
    }
    total_positions.push_back(pos);
    //std::cout << "Added position " << x << "," << y << "\n";
};



using MapReader = std::function<const char(const int, const int)>;


using MOVFREQTYPE = std::pair<MoveType, int>;
using MOVFREQLIST = std::list<MOVFREQTYPE>;
using MOVFREQITER = std::list<MOVFREQTYPE>::iterator;

// add_movesfreq
MOVFREQITER add_movesfreq(const int x, const int y, const DirEnumType& d, MOVFREQLIST& moves_vs_freq_list)
{
    MoveType mov(x,y,d);

    MOVFREQITER iter = std::find_if(moves_vs_freq_list.begin(),
                                    moves_vs_freq_list.end(),
                                    [&mov](const MOVFREQTYPE& moveT){return moveT.first==mov;});

    if(iter != moves_vs_freq_list.end())
    {
        //std::cout << "Adding multiple for \"" << iter->first.str_move() << "\"";
        iter->second += 1;
        return iter;
    }
    else
    {
        //std::cout << "Adding first-occur for " << x << "," << y << "\n";
        moves_vs_freq_list.emplace_back(MOVFREQTYPE({mov,1}));
        return std::prev(moves_vs_freq_list.end());
    }
}

using ObsPosBoolType=std::pair<PosType, bool>;
using ObsPosBoolList=std::list<ObsPosBoolType>;

int traverse_map_for_new_obs_pos(
    std::string& map_lines,
    const int map_offset,
    const int line_count,
    const MoveType& start_m,
    MapReader read_next_char,
    bool is_muted,
    ObsPosBoolType& newObsBoolPos = ObsPosBoolType())
{
    PosType newObsPos = newObsBoolPos.first;

    const size_t looping_check_pos_size = 100; // after how many unique-positions do we need to check the looping-condition
    const int pos_freq_for_looping = 3; // frequency of a position in list that will be used in looping-condition to catch if looping
    bool is_solve2 = newObsPos.is_valid(); // flag set if part-2 solution is desired

    std::list<PosType> total_guard_pos;
    MOVFREQLIST moves_vs_freq_list;


    int xMax = map_offset;
    int yMax = line_count;
    // Lambda : is_exit
    auto is_exit = [&](const int x, const int y) -> bool
    {
        return x<0 || x>xMax || y<0 || y>yMax;
    };

    auto peek_next_char = [&](const int x, const int y, const DirEnumType& d) -> const char
    {
        PosType pNext = PosType::get_next_pos(x, y, d);

        if(is_exit(pNext._x, pNext._y))
        {
            if(!is_muted) std::cout << "\n\nBAD POSITION TO PEEK: " << x << "," << y << " !!\n\n";
            return '&';
        }
        else
        {
            // check the set pos-x and pos-y; if its same as new-obs pos then return '#' else call function to read-using-pos
            return (pNext == newObsPos) ? '#' : read_next_char(pNext._x, pNext._y);
        }
    };

    int pos_x = start_m.x();
    int pos_y = start_m.y();
    DirEnumType pos_d = start_m.d();

    add_unique(pos_x, pos_y, total_guard_pos);

    if(!is_muted) std::cout << "\n starting the map traversal ....\n";

    bool isLooping = false;
    while(!is_exit(pos_x, pos_y))
    {
        const char c = peek_next_char(pos_x, pos_y, pos_d);
        if(c=='&')
        {
            if(!is_muted) std::cout << "Exit criteria met";
            break;
        }
        else if(c=='#')
        {
            DirType::turn_right(pos_d);
            if(!is_muted) std::cout << "Turning to " << " direction " << DirType::read_dir(pos_d) << " ...";
        }
        else
        {
            PosType pos = PosType::get_next_pos(pos_x, pos_y, pos_d);
            pos_x=pos._x;
            pos_y=pos._y;
            if(!is_muted) std::cout << "\nAdvanced to " << pos.str_pos() << " in direction " << DirType::read_dir(pos_d) << " ...";
            add_unique(pos_x, pos_y, total_guard_pos);

            auto ret_iter = add_movesfreq(pos_x, pos_y, pos_d, moves_vs_freq_list);
            if(!is_muted) std::cout << "... added to the pos-freq list with freq: " << ret_iter->second;
            if(ret_iter->second >= pos_freq_for_looping)
            {
                isLooping = true;
                break;
            }
        }
    };

    // looping check
    if(isLooping)
    {
        if(!is_muted) std::cout << "\n\n Guard is Looping!!! \n\n";
        total_guard_pos.clear();
        newObsBoolPos.second = true;
    }

    if(!is_muted) std::cout << "\n Total pos: " << total_guard_pos.size() << "\n";
    return total_guard_pos.size();
}


int traverse_map_for_pos(
    std::string& map_lines,
    const int map_offset,
    const int line_count,
    const MoveType& start_m,
    MapReader read_next_char,
    bool is_muted,
    const PosType& newObsPos = PosType())
{
    if(newObsPos.is_valid())
    {
        ObsPosBoolType new_obs_data;
        new_obs_data.first = newObsPos;
        return traverse_map_for_new_obs_pos(map_lines, map_offset, line_count, start_m, read_next_char, is_muted, new_obs_data);
    }
    else
    {
        return traverse_map_for_new_obs_pos(map_lines, map_offset, line_count, start_m, read_next_char, is_muted);
    }
}



void solve_day2_par(
    std::string& map_lines,
    const int map_offset,
    const int line_count,
    const MoveType& start_m,
    MapReader read_next_char,
    bool is_muted = false)
{
    
    int xMax = map_offset;
    int yMax = line_count;

    int iLoopObs = 0;
    ObsPosBoolType new_obs_pos_bool;
    ObsPosBoolList new_obs_pos_bool_list;

    // check for all positions possible for new obstacle
    for(int x=0; x<xMax; ++x)
    {
        for(int y=0; y<yMax; ++y)
        {
            if(x==start_m.x() && y==start_m.y())
            {
                if(!is_muted) std::cout << "\n skipping test for the start pos at :" << x << "," << y << " ...\n\n";
                continue;
            }
            new_obs_pos_bool.first = PosType(x,y);
            new_obs_pos_bool.second = false;
            new_obs_pos_bool_list.push_back(new_obs_pos_bool);
        }
    }

    std::for_each(
        std::execution::seq,//std::execution::par,
        new_obs_pos_bool_list.begin(),
        new_obs_pos_bool_list.end(),
        [&](ObsPosBoolType& obs_posbool)
        {
            if(0 == traverse_map_for_new_obs_pos(map_lines, map_offset, line_count, start_m, read_next_char, is_muted, obs_posbool))
            {
                int x=obs_posbool.first._x;
                int y=obs_posbool.first._y;
                if(true) std::cout << "\n Adding Looping-Obs at (" << x << "," << y << ")\n";
            }
        });

    for(const auto& o : new_obs_pos_bool_list)
    {
        if(o.second)
            ++iLoopObs;
    }

    std::cout << "\n Total Looping Obs: " << iLoopObs << "\n";
}

void solve_day2_nonpar(
    std::string& map_lines,
    const int map_offset,
    const int line_count,
    const MoveType& start_m,
    MapReader read_next_char,
    bool is_muted = false)
{
    int xMax = map_offset;
    int yMax = line_count;
    int iLoopObs = 0;
    // check for all positions possible for new obstacle
    for(int x=0; x<xMax; ++x)
    {
        for(int y=0; y<yMax; ++y)
        {
            if(x==start_m.x() && y==start_m.y())
            {
                if(!is_muted) std::cout << "\n skipping test for the start pos at :" << x << "," << y << " ...\n\n";
                continue;
            }
            if(!is_muted) std::cout << "\n\n Trying with obstacle at pos: " << x << "," << y << " ...\n";
            if(0 == traverse_map_for_pos(map_lines, map_offset,line_count, start_m, read_next_char, is_muted, PosType(x,y)))
            {
                std::cout << "\n Adding Looping-Obs \"" << ++iLoopObs << "\" at (" << x << "," << y << ")\n";
            }
        }
    }
    std::cout << "\n Total Looping Obs: " << iLoopObs << "\n";
}

void solve_day1(
    std::string& map_lines,
    const int map_offset,
    const int line_count,
    const MoveType& start_m,
    MapReader read_next_char,
    bool is_muted = false)
{
    int pos = traverse_map_for_pos(map_lines, map_offset,line_count, start_m, read_next_char, is_muted);
}

int main(int argc, char* argv[])
{
    bool is_muted = true;
    bool is_nonpar = false;
    bool is_solve1 = false;
    std::string file_name = "puzzle_input.txt";

    if(argc>=2)
    {
        if(argc==2 && std::string(argv[1])=="3")
        {
            is_nonpar = true;
            std::cout << "running part-2 in non-par and guard position logs, messages muted...";
        }
        if(argc==3 && std::string(argv[1])=="3" && std::string(argv[2]).find(".txt") != std::string::npos)
        {
            is_nonpar = true;
            std::cout << "running part-2 in non-par and guard position logs, messages muted...";
            file_name = argv[2];
            std::cout << "\n Running input file as " << file_name << "\n";
        }
        else if(argc==2 && std::string(argv[1])=="0")
        {
            std::cout << "running part-2 and guard position logs, messages muted...";
        }
        else if(argc==3 && std::string(argv[1])=="0" && std::string(argv[2]).find(".txt") != std::string::npos)
        {
            std::cout << "guard position logs, messages muted...";
            file_name = argv[2];
            std::cout << "\n Running input file as " << file_name << "\n";
        }
        // if only 2 rgs; and arg-2 is file-name
        else if(argc==2 && std::string(argv[1]).find(".txt") != std::string::npos)
        {
            file_name = argv[1];
            std::cout << "\n Running input file as " << file_name << "\n";
        }
        else if(argc==2 && std::string(argv[1]) == "1")
        {
            is_solve1=true;
            std::cout << "\n Running part-1\n";
        }
        // if 3 args; arg-2 is "1" for part-1 (or something else like 0 for part-2)
        // and arg-3 is filename
        else if(argc==3)
        {
            if(std::string(argv[2]).find(".txt") != std::string::npos)
            {
                file_name = argv[2];
                std::cout << "\n Running input file as " << file_name << "\n";
            }
            if(std::string(argv[1]) == "1")
            {
                is_solve1=true;
                std::cout << "\n Running part-1\n";
            }
        }
    }
    else
    {
        std::cout <<"\n Solving day-6 part-2 with " << file_name << "\n";
    }

    // declare input file stream
    std::ifstream istream_file(file_name);

    if(!istream_file.is_open()) // check if OPEN, return if NOT
    {
        std::cout<<"\nFailed to opne file: " << file_name << "\n";
        return -1;
    }

    std::string temp_line; // line to read from each stream
    std::string map_lines; // storage for all map-lines

    int map_line_offset=0; // used for getting char from maplines
    int map_size=0;

    while(istream_file>>temp_line)
    {
        if(temp_line.size()>1)
        {
            // if last character is new-line then remvoe
            if(temp_line[temp_line.size() -1] == '\n')
            {
                temp_line = temp_line.substr(0, temp_line.size() -1);
            }

            map_lines += temp_line; // add to map-lines
            
            map_line_offset = temp_line.size();

            ++map_size; // increment offset (used to read the char at position x,y)
        }
    }
    istream_file.close();

    // lambda to get char in map-lines using pos as (x,y)
    MapReader get_map_char = [&](const int x, const int y) -> const char
    {
        char c='&';
        // first char is at (0,0)
        if (y>=0 && y<map_size && x>=0 && x<map_line_offset)
            c = map_lines[y*map_line_offset + x];
        return c;
    };
    auto get_map_char_at_pos = [&](const PosType& pos) -> char
    {
        return get_map_char(pos._x, pos._y);
    };

    std::cout << "\n Inputs count: " << argc;
    std::cout << "\n second input: " << ((argc==2) ? argv[1] : " ");
    std::cout << "\n Map Lines offset: " << map_line_offset;
    std::cout << "\n Map Size (n lines): " << map_size;
    std::cout << "\n Char at pos (7,9): " << get_map_char(7,9) << "\n";

    int start_x=0;
    int start_y=0;
    for(const auto line : map_lines)
    {
        const char c = get_map_char(start_x, start_y);
        if(c=='^')
            break;
        if(start_x>=map_line_offset-1)
        {
            // increment y
            ++start_y;
            // reset x
            start_x = 0;
        }
        else
        {
            ++start_x;
        }
    }
    const MoveType start_move(start_x, start_y, DirEnumType::NORTH);
    std::cout << "\n Star position : " << start_move.str_move() << "\n";

   if(is_solve1)
   {
       solve_day1(map_lines, map_line_offset, map_size, start_move, get_map_char, is_muted);
   }
   else
   {
       if(is_nonpar)
       {
           solve_day2_nonpar(map_lines, map_line_offset, map_size, start_move, get_map_char, is_muted);
       }
       else
       {
           solve_day2_par(map_lines, map_line_offset, map_size, start_move, get_map_char, is_muted);
       }
   }

    return 0;
}
