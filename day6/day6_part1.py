from day6_helper import ORIENTATION, MOVEXY
from day6_helper import is_obstacle
from day6_helper import get_obs_x_y
from day6_helper import get_start_x_y
from day6_helper import find_next_x_y
from day6_helper import move_right_x_y
from day6_helper import add_x_y_to_moveL_if_not_there
from day6_helper import get_orn_str


def solve_day6_part1(lines):
    # decide boundaries X,Y
    xMax = len(lines[0])-2# exclude "\n"
    yMax = len(lines)-1
    print("xMax: " + str(xMax))
    print("yMax: " + str(yMax))
    # get all obstacles list
    obs_x, obs_y = get_obs_x_y(lines)
    #container for all positions occupied by guard
    allGuardPos = []
    # get the start pos
    x,y = get_start_x_y(lines)
    add_x_y_to_moveL_if_not_there(x, y, allGuardPos)
    print("Start pos: " + str(x) + "," + str(y) + "\n")
    print("First obs at: " + str(obs_x[0]) + "," + str(obs_y[0]) + "\n")
    print("Total obs : " + str(len(obs_x)) + "\n")
    #exit()
    
    #current position
    c_cur_x=x
    c_cur_y=y
    #current orientation
    c_orn = ORIENTATION.NORT
    # start traversing
    notDone = 1

    str1 = ""
    isTurn=0
    while notDone:

        next_x, next_y=find_next_x_y(c_cur_x, c_cur_y, c_orn, lines)

        if is_obstacle(next_x, next_y, obs_x, obs_y):
            str1="\nFound obstacle at " + str(next_x) + "," + str(next_y)
            # move to the right (find new pos_x, pos_y and orientation)
            str_r = "Turning from "  + get_orn_str(c_orn)
            _, _, c_orn = move_right_x_y(c_cur_x, c_cur_y, c_orn, lines)
            str_r += " to "  + get_orn_str(c_orn)
            print(str1 + "\n" + str_r + "\n")
            isTurn=1
        else:
            c_cur_x=next_x
            c_cur_y=next_y
            isTurn=0

        notDone = c_cur_x>=0 and c_cur_x<=xMax and c_cur_y>=0 and c_cur_y<=yMax

        if notDone and not isTurn:
            isAdded = add_x_y_to_moveL_if_not_there(c_cur_x, c_cur_y, allGuardPos)
            str0 = str(c_cur_x) + "," + str(c_cur_y)
            print("Go ahead in " + get_orn_str(c_orn) + " direction to x,y : " + str0)
            strA = "Added .."
            if not isAdded:
                strA="already there.."
            strA += "No. of moves: " + str(len(allGuardPos))
            print(strA)
            str1=""
        if not notDone:
            print("\nExiting at x,y : " + str0)
        # exited the map

    print("Total Moves: " + str(len(allGuardPos)) + "\n\n")
    exit()

    for m in allGuardPos:
        str0 = str(m._x) + "," + str(m._y)
        iOccur=0
        for k in allGuardPos:
            if k._x==m._x and k._y==m._y:
                iOccur+=1
        str0 += ": " + str(iOccur)
        print(str0)


if __name__=='__main__':

    fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    fH = open(fileP, "r")
    lines = fH.readlines()
    fH.close()

    solve_day6_part1(lines)
