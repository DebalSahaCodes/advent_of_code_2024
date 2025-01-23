from enum import Enum

class ORIENTATION(Enum):
        XDIR=-1,
        NORT=0,
        EAST=1,
        SOUT=2,
        WEST=3

def is_same_orientation(dir1, dir2):
    return get_orn_str(dir1) == get_orn_str(dir2) 

class MOVEXY:
    def __init__(self,x=0,y=0):
        self._x=x
        self._y=y
    _x=0
    _y=0
    # check two pos if they are same
    def is_same_pos_as(self, pos1):
        return pos1._x==self._x and pos1._y==self._y
    def is_same_pos_as_xy(self, x, y):
        return x==self._x and y==self._y
    def str_pos(self):
        return str(self._x) + "," + str(self._y)

def get_x_y(x,y,lines):
    line = lines[y]
    return line[x]

def get_start_x_y(lines):
    xMax = len(lines)
    yMax = len(lines[0])-1#exclude "\n"
    for y in range(yMax):
        for x in range(xMax):
            c = get_x_y(x,y,lines)
            if c == "^":
                return x,y

def get_obs_x_y(lines):
    obs_x=[]
    obs_y=[]
    xMax = len(lines)
    yMax = len(lines[0])-1#exclude "\n"
    nLine = 0
    for y in range(yMax):
        for x in range(xMax):
            #print("Checking at pos " + str(x) + " of line " + str(y))
            c = get_x_y(x,y,lines)
            str1="Loc " + str(x) + "," + str(y) + " :" + c
            if c == "#":
                #print("obs at " + str1)
                obs_x.append(x)
                obs_y.append(y)
            else:
                str1=""
        #if str1:
        #    print(str1)
        nLine +=1
    
    return obs_x, obs_y


def get_all_obs_pos_x_y(lines):
    obs_x_y = []
    obs_x = []
    obs_y = []
    obs_x, obs_y = get_obs_x_y(lines)
    for x,y in zip(obs_x, obs_y):
        pos = MOVEXY(x,y)
        obs_x_y.append(pos)
    return obs_x_y


def is_obstacle(x, y, obslist_x, obslist_y):
    is_obs = 0
    for o_x,o_y in zip(obslist_x,obslist_y):
        if o_x==x and o_y==y:
            is_obs = 1
    return is_obs

def find_next_x_y(old_x, old_y, current_orientation, lines):
    if current_orientation==ORIENTATION.NORT:
        return old_x, old_y-1
    elif current_orientation==ORIENTATION.EAST:
        return old_x+1, old_y
    elif current_orientation==ORIENTATION.SOUT:
        return old_x, old_y+1
    elif current_orientation==ORIENTATION.WEST:
        return old_x-1, old_y
    else:
        print("\nBAD BAD INPUT..........\n\n")
        exit()

def move_right_x_y(old_x, old_y, old_r, lines):
    if old_r==ORIENTATION.NORT:
        return old_x+1, old_y, ORIENTATION.EAST   # if NORTH, go east to turn right
    if old_r==ORIENTATION.EAST:
        return old_x, old_y+1, ORIENTATION.SOUT   # if EAST, go south to turn right
    if old_r==ORIENTATION.SOUT:
        return old_x-1, old_y, ORIENTATION.WEST   # if SOUTH, go west to turn right
    if old_r==ORIENTATION.WEST:
        return old_x, old_y-1, ORIENTATION.NORT   # go WEST, go noth  to turn right


def get_orn_str(current_orientation):
    if current_orientation==ORIENTATION.NORT:
        return "north"
    if current_orientation==ORIENTATION.EAST:
        return "east"
    if current_orientation==ORIENTATION.SOUT:
        return "south"
    if current_orientation==ORIENTATION.WEST:
        return "west"


def add_x_y_to_moveL(x,y,moveL):
    moveN=MOVEXY(x,y)
    moveL.append(moveN)

def add_x_y_to_moveL_if_not_there(x,y,moveL):
    isNot=1
    testXY = MOVEXY(x,y)
    for m in moveL:
        #print("testing  " + str(x) + "," + str(y) + "with m: " + str(m._x) + "," + str(m._y))
        if m.is_same_pos_as_xy(x,y):
            #print("already added...skipping")
            isNot=0
    if isNot:
        moveN=MOVEXY(x,y)
        moveL.append(moveN)
    return isNot

