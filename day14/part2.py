bad_val = -999 # used to init vals.

class RoboData:
    p_x=bad_val
    p_y=bad_val
    v_x=bad_val
    v_y=bad_val
    def __init__(self, px,py,vx,vy):
        self.p_x=px
        self.p_y=py
        self.v_x=vx
        self.v_y=vy


xMax=bad_val # max. val. for x pos
yMax=bad_val # max. val. for y pos

all_rob_dat=[]
all_rob_pos=[]
quad_bdry_pos=[]

class PosType:
    x=bad_val
    y=bad_val
    def __init__(self, x, y):
        self.x=x
        self.y=y
    def get_str(self):
        return "(" + str(self.x) + "," + str(self.y) + ")"

def is_same_pos(pos1:PosType, pos2:PosType):
    return pos1.x==pos2.x and pos1.y==pos2.y

def get_rob_data_from_line(line:str) -> RoboData:
    prts=line.split(' ')
    p_x=bad_val
    p_y=bad_val
    v_x=bad_val
    v_y=bad_val
    for p in prts:
        s=p.split('=')
        dat=s[0]
        val=s[1].split(',')
        if dat=='p':
            p_x=int(val[0])
            p_y=int(val[1])
        elif dat=='v':
            v_x=int(val[0])
            v_y=int(val[1])
        else:
            print("\n\nBAD DATA: \"",dat,"\"")
    rob_data=RoboData(p_x,p_y,v_x,v_y)
    return rob_data

def inc_pos_using_psmx_velo_time(pos:int, mxps:int, velo:int, time:int) -> int:
    final_pos = pos + velo*time
    # since position wraps around, we need to do mod with max-pos 
    final_pos = final_pos % mxps
    return final_pos


def is_quad_1_pos(pos:PosType, xMax:int, yMax:int):
    return pos.x < xMax/2 and pos.y < yMax/2

def is_quad_2_pos(pos:PosType, xMax:int, yMax:int):
    return pos.x > xMax/2 and pos.x <= xMax and pos.y < yMax/2

def is_quad_3_pos(pos:PosType, xMax:int, yMax:int):
    return pos.x < xMax/2 and pos.y > yMax/2 and pos.y <= yMax

def is_quad_4_pos(pos:PosType, xMax:int, yMax:int):
    return pos.x > xMax/2 and pos.x <= xMax and pos.y > yMax/2 and pos.y <= yMax


quad_1_rob=[]
quad_2_rob=[]
quad_3_rob=[]
quad_4_rob=[]

n_quad_1=0
n_quad_2=0
n_quad_3=0
n_quad_4=0

if __name__=='__main__':
    d_lines=[]
    file_name="puzzle.txt"
    fH = open(file_name, "r")
    d_lines= fH.readlines()
    fH.close()

    lines=[]
    for line in d_lines:
        line=line.split('\n')[0]
        lines.append(line)

    for line in lines:
        line=line.split('\n')[0]
        r_data=get_rob_data_from_line(line)
        all_rob_dat.append(r_data)

    for d in all_rob_dat:
        if d.p_x > xMax:
            xMax=d.p_x
        if d.p_y > yMax:
            yMax=d.p_y
    n_posX = xMax + 1  # no. of tiles in X direction
    n_posY = yMax + 1  # no. of tiles in Y direction
    print("xMax:",xMax,"\nyMax:",yMax)
    
    # decide the quadrant boundaries
    midX = xMax/2
    midY = yMax/2
    # Add the column-quadrant-boundary
    for idx_row in range(n_posY):
        quad_bdry_pos.append(PosType(midX, idx_row))
    # Add the row-quadrant-boundary 
    for idx_col in range(n_posY):
        quad_bdry_pos.append(PosType(idx_col, midY))

    #file_name="sample1.txt"
    n_seconds=100 # total time elapsed

    # get the x-pos and y-pos for all robots
    # after n_seconds have elapsed
    rob_x=bad_val
    rob_y=bad_val
    for rob in all_rob_dat:
        rob_x = inc_pos_using_psmx_velo_time(rob.p_x, n_posX, rob.v_x, n_seconds)
        rob_y = inc_pos_using_psmx_velo_time(rob.p_y, n_posY, rob.v_y, n_seconds)
        r_pos = PosType(rob_x, rob_y)
        is_quad_bdry=len([b_pos for b_pos in quad_bdry_pos if b_pos.x==rob_x and b_pos.y==rob_y]) >= 1
        if not is_quad_bdry:
            all_rob_pos.append(r_pos)
        #print("added robot pos at:", r_pos.get_str())

    #---------------------------------------------
    # print the positions to a file .............
    #---------------------------------------------
    is_print_output=1 # flag to print to file
    #---------------------------------------------
    # Logic here .....
    if is_print_output:
        pos_list=[]
        # print with sots/stars to init
        for x in range(n_posX):
            loc_list=[]
            for y in range(n_posY):
                loc_list.append(bad_val)
            pos_list.append(loc_list)
        print("n.Col for POS_LIST:",len(pos_list[0]))
        print("n.Row for POS_LIST:",len(pos_list))
        print
        #print("CURRENT VAL AT X:",xMax," Y:", yMax," is ->", pos_list[xMax][yMax])
        #exit()
        for rpos in all_rob_pos:
            if rpos.x>xMax:
                print("Bad x val: ", pos.x)
                exit()
            elif rpos.y>yMax:
                print("Bad y val: ", pos.y)
                exit()
            # Now check if the value is valid robot pos
            #print("processing rpos.x:", rpos.x," rpos.y:", rpos.y)
            if pos_list[rpos.x][rpos.y]==bad_val:
                pos_list[rpos.x][rpos.y]=1
            else:
                pos_num = pos_list[rpos.x][rpos.y]
                pos_num += 1
                pos_list[rpos.x][rpos.y] = pos_num
        file_name_out="robots_"+str(n_seconds)+"s.txt"
        file = open(file_name_out, 'w')
        for pos_line in pos_list:
            line=""
            for pos in pos_line:
                if pos==bad_val:
                    line+='*'
                else:
                    line+=str(pos)
            file.write(line +'\n')
        file.close()
        print("Printed the positions grid in file \"", file_name_out,"\"\n")
    #---------------------------------------------

    # count filled pos in each line
    
    
