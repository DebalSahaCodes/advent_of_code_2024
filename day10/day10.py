import time

class PosType:
    _x:int
    _y:int
    def __init__(self,x:int,y:int):
        self._x=x
        self._y=y
    def pos_str(self) -> str:
        return "(" + str(self._x) + "," + str(self._y) + ")"


def is_pos_in_poslist(pos0, pos_list):
    is_same=0
    for lpos in pos_list:
        x_same=lpos._x==pos0._x
        y_same=lpos._y==pos0._y
        if x_same and y_same:
            is_same=1
            break
    return is_same

def solve_day10_part1(file_name):
    # read lines
    lines=[]
    fH=open(file_name)
    aL=fH.readlines()
    for line in aL:
        line = line.strip()
        lines.append(line)
    # check xMax, yMax
    xMax=len(lines[0])
    yMax=len(lines)
    for l in lines: # verify the file lines are consistent
        if len(l) != xMax:
            print("Bad file lines extraction")
            exit()
    # print the max X and Y
    print("xMax: ", xMax)
    print("yMax: ", yMax)
    # gather all trail heads
    pos_trail_head=[]
    pos_th_y=-1
    for line in lines:
        pos_th_y += 1#each line change is Y coord change
        pos_th_x=-1
        for c in line:
            pos_th_x += 1# each character in line progress to change X-coord
            if c=='0':
                pos=PosType(pos_th_x,pos_th_y)
                pos_trail_head.append(pos)
                print("Trail head at " + pos.pos_str() + " added ...")
    # iterate over all the trail-head positions
    c_alltrails=0 # rating for trails starting at one given head [one for each iteration]
    n_alltrails=0 # total of all the ratings from all the trails in the map
    n_trailends_score=0
    for pos_th in pos_trail_head:
        # storage and flags for storing trail-ends
        p_trailends=[]
        # flag to be used as while loop
        not_done=1
        trail_pos_list=[] # storage of latest position
        # add the trail ehad as the first element , to act as the seed for the rest of the process
        trail_pos_list.append(pos_th)
        # while-loop till all trails are explored
        print("\n\nStarting trail starting at " + pos_th.pos_str() + " ...")
        while(not_done):
            candidate_pos=[] # list to store all the possible positions leading to full trail
            for old_pos in trail_pos_list:
                old_x=old_pos._x
                old_y=old_pos._y
                try:
                    c_old_num=lines[old_y][old_x]
                except:
                    print("Out of range position at " + old_pos.pos_str())
                    exit()
                print("Searching for trail " + c_old_num + " at " + old_pos.pos_str() + " ...")
                try_direction=[] # temp list to stroe positions in all directions to try
                # gather all possible positions in all 4 directions of this position
                try_direction.append(PosType(old_x,old_y-1))
                try_direction.append(PosType(old_x+1,old_y))
                try_direction.append(PosType(old_x,old_y+1))
                try_direction.append(PosType(old_x-1,old_y))
                # iterate over this 4 possible positions directioned at N,E,S,W and see if expected num found
                for tpos in try_direction:
                    x=tpos._x
                    y=tpos._y
                    if x>=0 and y>=0 and x<xMax and y<yMax:
                        # expected num in this x,y pos
                        c_new_num=lines[y][x]
                        try:
                            new_num=int(c_new_num)
                        except:
                            new_num=-99
                            print("Not a good trail char, using",new_num,"instead...")
                        # compare expected num with the num in the list
                        old_num=int(c_old_num)
                        if new_num==old_num+1:
                            str0 = "Found " + str(c_new_num) + " at (" + str(x) + "," + str(y) + ") !"
                            candidate_pos.append(tpos)
                            if c_new_num=='9':
                                not_done=0
                                str0 += "\nChecking if the position already exists ..."
                                if not is_pos_in_poslist(PosType(x,y),p_trailends):
                                    p_trailends.append(tpos)
                                    str0 +="\tAdding trail end " + tpos.pos_str() + " for trail with head " + pos_th.pos_str()
                                else:
                                    str0 += "\tSkipping as already added...."
                                print(str0)
                                #str1 += " and till now total trails=", n_alltrails)
                                #print(str1)
                        else: #bad path
                            print("No path at " + str(x) + "," + str(y))
                    else:
                        print("Bad position coordinates: (" + str(x) + "," + str(y) + ")")
            # once done checking all 4 directioned positions numbers, then assign this list to pos_list to redo same process
            trail_pos_list = candidate_pos
            str1 = "New possbile count: " + str(len(candidate_pos))
            str1 += " and the possbile positions  are: "
            for p in candidate_pos:
                str1 += p.pos_str() + " "
            print(str1 + "\n")
        c_alltrails = len(candidate_pos)
        print("\n Total trails starting with head at " + pos_th.pos_str() + " :" + str(c_alltrails))
        n_alltrails += c_alltrails
        t_score=len(p_trailends)
        print("\n No. of distinct trails ends at " + pos_th.pos_str() + " :" + str(t_score))
        n_trailends_score += t_score
    print("\n\n\n (solution day10 part1) TOTAL SCORE: " + str(n_trailends_score))
    print("\n\n\n (solution day10 part2) TOTAL RATINGS: " + str(n_alltrails))


if __name__=='__main__':
    start_time = time.time()
    #file_name="sample_input_2.txt"
    #file_name="sample_input_3.txt"
    #file_name="sample_input_4.txt"
    #file_name="sample_input_5.txt"
    #file_name="sample_input.txt"
    file_name="puzzle_input.txt"
    solve_day10_part1(file_name)
    tot_time = time.time() - start_time
    print('\nTotal time take: {:.10}'.format(tot_time), " seconds")
