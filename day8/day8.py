import math

class Position:
    _x=0
    _y=0
    def __init__(self, x, y):
        self._x=x
        self._y=y
    def pos_str(self):
        return "(" + str(self._x) + "," + str(self._y) + ")"

class Antenna:
    _name=":"
    _pos=Position(0,0)
    _is_done=0
    def __init__(self, char, x, y):
        self._name=char
        self._pos=Position(x,y)
    def name(self):
        return self._name
    def position(self):
        return self._pos
    def pos_str(self):
        return self._pos.pos_str()
    def set_is_done(self):
        self._is_done=1
    def view_is_done(self):
        return self._is_done

def are_same_antennas(a1, a2):
    return a1.name()==a2.name() and a1._pos._x==a2._pos._x and a1._pos._y==a2._pos._y

def get_same_antennas(anns, a_given):
    return [a for a in anns if a.name()==a_given.name() and not are_same_positions(a.position(), a_given.position()) and not a.view_is_done()]

def mark_pair_done_in_antennas(a1, a2, anns):
    for a in anns:
        if are_same_antennas(a, a1) or are_same_antennas(a, a2):
            a.set_is_done()

def distance_pos1_pos2(pos1, pos2):
    x_d=pos1._x - pos2._x
    y_d=pos1._y - pos2._y
    x_2=x_d*x_d
    y_2=y_d*y_d
    return math.sqrt(x_2+y_2)

def are_same_positions(pos1, pos2):
    return pos1._x==pos2._x and pos1._y==pos2._y

def is_antinode_from_antennas_pair_to_solve_part1(pos0, ant1, ant2):
    d1=distance_pos1_pos2(pos0, ant1.position())
    d2=distance_pos1_pos2(pos0, ant2.position())
    return d1==d2*2 or 2*d1==d2

def is_not_in_pos_list(pos, pos_list):
    return len([k for k in pos_list if are_same_positions(k,pos)])==0

def is_clear_space_in_map(char):
    return char=="."

def get_slope(pos1, pos2):
    if pos2._x==pos1._x:
        return -999 # this is not the correct slop value but is ggo enough to avoid "div by zero" and be used for checking collinearity
    else:
        return (pos2._y-pos1._y)/(pos2._x-pos1._x)

def are_collinear(pos1, pos2, pos3):
    return get_slope(pos1, pos2)==get_slope(pos2, pos3)


def solve_day_8_part1(n_antennas, all_positions):
    # find antinodes for all antennas where antinodes are positions that are collinear with 2 antennas
    # and distnace from one anetenna is twice the distance from the other
    i_antinode=0
    a_antinodes=[]
    for a1 in n_antennas:
        a_found = get_same_antennas(n_antennas, a1)
        for a2 in a_found:
            y_p=0
            for line in all_positions:
                x_p=0
                for c in line:
                    pos=Position(x_p,y_p)
                    if not are_same_positions(pos,a1.position()) and not are_same_positions(pos,a2.position()) and are_collinear(pos, a1.position(), a2.position()):
                    #if is_clear_space_in_map(c):
                        if is_antinode_from_antennas_pair_to_solve_part1(pos, a1, a2) and is_not_in_pos_list(pos, a_antinodes):
                            i_antinode += 1
                            dist1=distance_pos1_pos2(pos, a1.position())
                            dist2=distance_pos1_pos2(pos, a2.position())
                            print(" Result " + str(i_antinode) + ": adding pos " + pos.pos_str() + " for dist:" + str(dist1) + " from " + a1.name() + " at " + a1.pos_str() + " and  dist:" + str(dist2)  + " from " + a2.name() + " at " + a2.pos_str())
                            a_antinodes.append(pos)
                    x_p+=1
                y_p+=1


def solve_day_8_part2(n_antennas, all_positions):
    # find antinodes for all antennas considering the "effects of resonant harmonics" so the antinodes are positions that are collinear 
    # with "at least" 2 antennas "regardless of the distnace from the anetennas"
    # That means, use almost the same algorithms as part-1 and get rid of the distance-check for antinode, only keep collinearity check
    # find antinodes for all antennas where antinodes are positions that are collinear with 2 antennas
    # and distnace from one anetenna is twice the distance from the other
    i_antinode=0
    a_antinodes=[]
    for a1 in n_antennas:
            a_found = get_same_antennas(n_antennas, a1)
            for a2 in a_found:
                y_p=0
                for line in all_positions:
                    x_p=0
                    for c in line:
                        pos=Position(x_p,y_p)
                        if is_not_in_pos_list(pos, a_antinodes):
                            # find the slopes explicitly here, to print them with reult
                            slope_1=get_slope(pos,a1.position())
                            slope_2=get_slope(pos,a2.position())
                            if slope_1==slope_2:
                                i_antinode += 1
                                print(str(i_antinode) + " Adding pos " + pos.pos_str() + " with slope " + str(slope_1) + " form " + a1.name() + " at " + a1.pos_str() + " and at slope " + str(slope_2) + " from " + a2.name() + " at " + a2.pos_str())
                                a_antinodes.append(pos)
                            elif are_same_positions(a1.position(), pos):
                                i_antinode += 1
                                print(str(i_antinode) + " Adding pos " + pos.pos_str() + " co-existing with " + a1.name() + " at " + a1.pos_str() + " and at slope " + str(slope_2) + " from " + a2.name() + " at " + a2.pos_str())
                                a_antinodes.append(pos)
                            elif are_same_positions(a2.position(), pos):
                                i_antinode += 1
                                print(str(i_antinode) + " Adding pos " + pos.pos_str() + " with slope " + str(slope_1) + " form " + a1.name() + " at " + a1.pos_str() + " and co-existing with " + a2.name() + " at " + a2.pos_str())
                                a_antinodes.append(pos)
                            #else:
                            #    result_str = str(i_antinode) + " Skipping pos " + pos.pos_str() + " with slope " + str(slope_1) + " form " + a1.name() + " at " + a1.pos_str() + " and at slope " + str(slope_2) + " from " + a2.name() + " at " + a2.pos_str())
                        x_p+=1
                    y_p+=1


if __name__=='__main__':
    #file_name="sample_input.txt"
    #file_name="sample_input_2.txt"
    file_name="puzzle_input.txt"
    f_handle=open(file_name,'r')
    lines=[]
    lines=f_handle.readlines()
    if not lines:
        print("\n\nFailed to read file :\"" + file_name + "\"\n")
        exit()
    # remove the new-line char from the end of the lines being read from file
    nLines=[]
    for line in lines:
        nLines.append(line.strip())
    x_max=len(nLines[0]) # no. of characters in each row
    y_max=len(nLines)    # no. of total rows
    #####################################
    # read all the antennas from the lines read from the file
    # create a container with all the antenna-types vs. their positions
    antennas=[]
    y_pos=0
    for line in nLines:
        x_pos=0 # increment x-coordinate counter
        for character in line:
            if character != '.':
                a = Antenna(character, x_pos, y_pos)
                antennas.append(a)
            x_pos+=1
        y_pos+=1 # increment y-coordinate counter
    #####################################
    # solve day-8 part1
    #solve_day_8_part1(antennas, nLines)
    #####################################
    # solve day-8 part2
    solve_day_8_part2(antennas, nLines)
