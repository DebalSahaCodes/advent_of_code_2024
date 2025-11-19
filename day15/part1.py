puzzle_lines=[]

c_robot='@'
c_mvbox='O'
c_stopc='#'

xMax=-1
yMax=-1

file_name="puzzle.txt"
tempL=[]
fH=open(file_name, 'r')
tempL=fH.readlines()
fH.close()


curr_pos_x=-1
curr_pos_y=-1

print_lines=[]

oLine=""
for idx,line in enumerate(tempL):
    # first row is boundary
    # first col is boundary
    # last col is boundary
    # last row is boundary
    if "###" in oLine and line=='\n':
        #print("found end!!!")
        if xMax==-1:
            xMax=len(oLine)-1
        if yMax==-1:
            yMax=idx-1
    if line[-1:] == '\n':
        line=line[:-1]
    if '@' in line:
        curr_pos_x=idx
        #t_idx=0
        for t_idx,c in enumerate(line):
            if c=='@':
                curr_pos_y=t_idx
    c_line=[]
    for c in line:
        c_line.append(c)
    puzzle_lines.append(c_line)
    oLine=line

print("xMax:", xMax)
print("yMax:", yMax)

print("curr_pos_x:", curr_pos_x)
print("curr_pos_y:", curr_pos_y)


def update_board(i_move, c_move, rpos_x, rpos_y):
    inc_x=0
    inc_y=0
    if c_move=='<':
        inc_x=-1
        inc_y=0
    elif c_move=='>':
        inc_x=1
        inc_y=0
    elif c_move=='v':
        inc_x=0
        inc_y=1
    elif c_move=='^':
        inc_x=0
        inc_y=-1
    else:
        print("Bad char given for move:", c_move)
        exit()
    is_move=0
    new_posx=rpos_x
    new_posy=rpos_y
    # depending on x or y increment, traverse the row or col
    # and keep stoing the symbols encountered in the positions
    # till the symbol is '#' or '.'
    # If the symbol is '.', then update the map with each symbol
    # in the next x (or next y position)
    # if symbol is '#' then break the loop
    if inc_x != 0:
        p_line = puzzle_lines[rpos_y]
        curr_pos=rpos_x
        while p_line[curr_pos] != '#' and curr_pos<xMax and curr_pos>0:
            sym=p_line[curr_pos]
            if sym=='#':
                break
            elif sym=='.':
                is_move=1
                break
            curr_pos = curr_pos + inc_x
        #print("\t line is:", p_line)
        if is_move:
            #print("Moving robot using move ", c_move, " as \".\" found in (",curr_pos,",",rpos_y,"}")
            while curr_pos != rpos_x and curr_pos<xMax and curr_pos>0:
                #print("\t replacing pos (",curr_pos,",",rpos_y,")", puzzle_lines[rpos_y][curr_pos], "with pos (", curr_pos-inc_x,",",rpos_y,") ",puzzle_lines[rpos_y][curr_pos - inc_x])
                puzzle_lines[rpos_y][curr_pos]=puzzle_lines[rpos_y][curr_pos - inc_x]
                curr_pos = curr_pos - inc_x
            puzzle_lines[rpos_y][rpos_x]='.'
            new_posx=rpos_x + inc_x
            new_posy=rpos_y
    elif inc_y!=0:
        # compute if move is possible and set the flag "is_move" if so
        curr_pos=rpos_y
        while puzzle_lines[curr_pos][rpos_x] != '#' and curr_pos<yMax and curr_pos>0:
            sym=puzzle_lines[curr_pos][rpos_x]
            if sym=='#':
                break
            elif sym=='.':
                is_move=1
                break
            curr_pos = curr_pos + inc_y
        if is_move:
            #print("Moving robot using move ", c_move, " as \".\" found in (",rpos_x,",",curr_pos,"}")
            while curr_pos != rpos_y and curr_pos<yMax and curr_pos>0:
                #print("\t replacing pos (",rpos_x,",",curr_pos,")", puzzle_lines[curr_pos][rpos_x], "with pos (", rpos_x,",",curr_pos-inc_y,") ",puzzle_lines[curr_pos-inc_y][rpos_x])
                puzzle_lines[curr_pos][rpos_x]=puzzle_lines[curr_pos-inc_y][rpos_x]
                curr_pos -= inc_y
            puzzle_lines[rpos_y][rpos_x]='.'
            new_posx=rpos_x
            new_posy=rpos_y + inc_y
    else:
        print("Bad inc of x or y")
        exit()

    strR=""
    if 0==is_move:
        strR+="NO MOVE WITH " + str(i_move) + " move i.e. \"" + c_move + "\""
    else:
        strR += "MOVED ROBOT WITH " + str(i_move) + " move i.e. \"" + c_move + "\"" 
    print(strR + " AND ROBOT POS NOW (", new_posx, ",", new_posy,") \n")
    # new pos of robot to be determined if moved
    return new_posx, new_posy

def print_board(c_name='no_move'):
    file_name='file_'+c_name+'.txt'
    print("\n\n.. PRINTING FILE: ", file_name)
    fH=open(file_name, 'w')
    c_line=""
    for iy in range(len(puzzle_lines)):
        if iy > yMax:
            break
        for ix in range(len(puzzle_lines[iy])):
            c_line+=puzzle_lines[iy][ix]
        c_line+="\n"
    fH.write(c_line)
    fH.close()

def add_board_to_print_lines(i_move, c_move):
    p_line="\n\nAdding for move " + str(i_move) + ":\"" + c_move + "\"\n"
    for iy in range(len(puzzle_lines)):
        if iy > yMax:
            break
        for ix in range(len(puzzle_lines[iy])):
            p_line+=puzzle_lines[iy][ix]
        p_line+="\n"
    print_lines.append(p_line)

def print_final_board_print_lines():
    file_name='file_final_board.txt'
    print("\n\n.. PRINTING FILE: ", file_name)
    fH=open(file_name, 'w')
    for p_lines in print_lines:
        fH.writelines(p_lines)
    fH.close()


def compute_board_gps():
    gps_all=0
    for iy in range(yMax+1):
        for ix in range(xMax+1):
            sym=puzzle_lines[iy][ix]
            if sym==c_mvbox:
                gps_sym=100*iy + ix
                gps_all += gps_sym
    return gps_all


idx_file_min=yMax+2
idx_file_max=len(puzzle_lines)-1
print("idx_file_max:",idx_file_max)
print("idx_file_min:",idx_file_min)

c_x=curr_pos_x
c_y=curr_pos_y
print("Processing moves...")

range_file_lines=range(idx_file_min, idx_file_max+1)
if idx_file_min >= idx_file_max:
    range_file_lines=range(idx_file_min+1)

all_moves=[]
for idx_file in range_file_lines:
    moves_line=puzzle_lines[idx_file]
    #print("moves: ", moves_line,"\n\n")
    for c in moves_line:
        if c!='\n':
            all_moves.append(c)

i_move=0
add_board_to_print_lines(0, "-initial-no_move-")
for c in all_moves:
    i_move +=1
    c_x, c_y = update_board(i_move, c, c_x, c_y)
    #print("new robot pos after move ", c," is: (", c_x,",",c_y,")")
    #print_board('_prev')
    add_board_to_print_lines(i_move, c)

print("Computing the GPS of board: ", compute_board_gps())
#print_board(str(i_move))
print_final_board_print_lines()
