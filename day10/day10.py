import time

class PosType:
    _x:int
    _y:int
    def __init__(self,x:int,y:int):
        self._x=x
        self._y=y

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
    pos_th_x=-1
    pos_th_y=-1
    for line in lines:
        pos_th_x += 1
        for c in line:
            pos_th_y += 1
            if c=='0':
                pos_trail_head.append(PosType(pos_th_x,pos_th_y))
    # iterate over all the trail-head positions
    n_alltrails=0
    for pos_th in pos_trail_head:
        not_done=1
        trail_pos_list=[PosType] # storage of latest position
        # add the trail ehad as the first element , to act as the seed for the rest of the process
        trail_pos_list.append(pos_th)
        # while-loop till all trails are explored
        while(not_done):
            candidate_pos=[PosType] # list to store all the possible positions leading to full trail
            try_direction=[PosType] # temp list to stroe positions in all directions to try
            for old_pos in trail_pos_list:
                old_x=old_pos._x
                old_y=old_pos._y
                c_old_num=lines[old_x][old_y]
                # gather all possible positions in all 4 directions of this position
                try_direction.append(PosType(x,y-1))
                try_direction.append(PosType(x+1,y))
                try_direction.append(PosType(x,y+1))
                try_direction.append(PosType(x-1,y))
                # iterate over this 4 possible positions directioned at N,E,S,W and see if expected num found
                for tdir in try_direction:
                    x=tdir._x
                    y=tdir._y
                    if x>=0 and y>=0 and x<=xMax and y<=yMax :
                        # expected num in this x,y pos
                        c_new_num=lines[x][y]
                        new_num=int(c_new_num)
                        # compare expected num with the num in the list
                        old_num=int(c_old_num)
                        if new_num==old_num+1:
                            print("Found ", c_new_num)
                            candidate_pos.append(pos)
                            if c_new_num=='9':
                                not_done=1
                                n_alltrails += 1
                                print("Done for trail head and till now total trails=", n_alltrails)
                # once done checking all 4 directioned positions numbers, then assign this list to pos_list to redo same process
                trail_pos_list = candidate_pos



if __name__=='__main__':
    start_time = time.time()
    #file_name="sample_input_1.txt"
    file_name="sample_input.txt"
    #file_name="puzzle_input.txt"
    solve_day10_part1(file_name)
