puzzle_lines=[]

c_robot='@'
c_mvbox='O'
c_stopc='#'

xMax=-1
yMax=-1

file_name="sample3.txt"
ctempL=[]
fH=open(file_name, 'r')
ctempL=fH.readlines()
fH.close()

print(" file lines: ", len(ctempL))


all_moves=[]
for i_line in range(len(ctempL)):
    c_line=ctempL[i_line]
    if c_line=='\n' or c_line[0]=='#':
        continue
    for c in c_line:
        if c!='\n':
            all_moves.append(c)

#for m in all_moves:
#    print("Moves:", m)
#exit()

ntempL=[]
# double_puzzle_lines
oLine=""
nBorder=0
for line in ctempL:
    d_line=""
    k_hash=[k for k in line if k=='#']
    if len(k_hash) >= len(line)-2:
        print("Found border")
        nBorder += 1
    if nBorder<=2:
        print("place a 'D' after each char")
        for c in line:
            if c!='\n':
                d_line+=c+'D'
    c_list=[]
    for char_l in d_line:
        c_list.append(char_l)
    ntempL.append(c_list)
    oLine=line

print("Total lines:", len(ntempL))
print("Total cols:", len(ntempL[0]))

for idx_line in range(len(ntempL)):
    for idx_char in range(len(ntempL[idx_line])):
        if ntempL[idx_line][idx_char]=='D':
            # check the preceding character to fill the 'D' spot
            prev_sym=ntempL[idx_line][idx_char-1]
            if prev_sym=='#':
                ntempL[idx_line][idx_char]='#'
            elif prev_sym=='O':
                ntempL[idx_line][idx_char-1]='['
                ntempL[idx_line][idx_char]=']'
            elif prev_sym=='.':
                ntempL[idx_line][idx_char]='.'
            elif prev_sym=='@':
                ntempL[idx_line][idx_char]='.'


tempL=[]

for idx_line in range(len(ntempL)):
    nLine=""
    for idx_char in range(len(ntempL[idx_line])):
        nLine += ntempL[idx_line][idx_char]
    nLine+='\n'
    tempL.append(nLine)




curr_pos_x=-1
curr_pos_y=-1

print_lines=[]

# FILL UP THE PUZZLE BOARD
oLine=""
for idx,line in enumerate(tempL):
    if line[-1:] == '\n':
        line=line[:-1]
    # first row is boundary
    # first col is boundary
    # last col is boundary
    # last row is boundary
    k_hash=[k for k in line if k=='#']
    print("Len of k_hash:", len(k_hash))
    if "###" in line and len(k_hash)>=len(line)-2:
        #print("found end!!!")
        if xMax!=-1 and yMax==-1:
            yMax=idx
            print("...this is last border iteration..")
        if xMax==-1: 
            xMax=len(line)-1
            print(".... is 1st border iteration..")
    if '@' in line:
        curr_pos_y=idx
        #t_idx=0
        for t_idx,c in enumerate(line):
            if c=='@':
                curr_pos_x=t_idx
    c_line=[]
    for c in line:
        c_line.append(c)
    puzzle_lines.append(c_line)
    oLine=line


print("xMax:", xMax)
print("yMax:", yMax)

print("curr_pos_x:", curr_pos_x)
print("curr_pos_y:", curr_pos_y)

def print_board(c_name='no_move'):
    file_name='file_'+c_name+'3.txt'
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


print_board()



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
    print("Inc from pos (", rpos_x,",",rpos_y,")")
    #----------------------------------------------------------------------
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
    #----------------------------------------------------------------------
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
        #--------------------------
        # check if the found position is just one step ahead for robot
        if curr_pos==rpos_y+inc_y: # yes it is
            puzzle_lines[rpos_y+inc_y][rpos_x]=puzzle_lines[rpos_y][rpos_x]
            puzzle_lines[rpos_y][rpos_x]='.'
            is_move=1
        #--------------------------
        else:
            #--------------------------
            dict_y_vs_xposList={}
            #--------------------------
            if is_move:
                # collect all the obstacles ahead of the robot till the position of found DOT
                index_y=rpos_y+inc_y
                final_y=curr_pos
                if inc_y==-1:
                    final_y=1 # if going UP
                # iterate through each y (i.e. each row)
                #-----------------------------------------------------------
                while index_y != final_y and index_y>=1 and index_y<yMax:
                    print("Checking for index_y=", index_y, " with final_y=", final_y)
                    t_poslist=[]
                    #-----------------------------------------------------------
                    if len(dict_y_vs_xposList.keys())==0: # if empty
                        #first row ahead of robot, so capture the immediate obstacle's
                        #position with the right/left position depending which one completes it
                        t_poslist.append(rpos_x) #add current
                        if puzzle_lines[index_y][rpos_x]==']':
                            t_poslist.append(rpos_x-1) # add left
                        elif puzzle_lines[index_y][rpos_x]=='[':
                            t_poslist.append(rpos_x+1) # or add right
                    else:
                        x_pos_list=dict_y_vs_xposList[index_y-inc_y] # take last entered x-positions
                        for xpos in x_pos_list:
                            c_sym=puzzle_lines[index_y][xpos]
                            o_sym=puzzle_lines[index_y-inc_y][xpos]
                            if c_sym==']' or c_sym=='[':
                                t_poslist.append(xpos) #add current
                                #add the opposit side if curr_sym != old_sym in same column
                                if o_sym!=c_sym: #if not same then adjacent obstacle needs to be added as well
                                    if c_sym==']':
                                        t_poslist.append(xpos-1) # add left
                                    elif c_sym=='[':
                                        t_poslist.append(xpos+1) # or add right
                                else: # if same symbol then the opposite side will be added in next iteration
                                    pass
                                #    if o_sym==']': #i.e. so old has a left; so adjacent left obstacle to be taken
                                #        t_poslist.append(xpos-1) # add right of left obs
                                #        t_poslist.append(xpos-2) # add left of left obs
                                #    elif o_sym=='[': #i.e. so old has a right; so adjacent rightobstacle to be taken
                                #        t_poslist.append(xpos+1) # add right of right obs
                                #        t_poslist.append(xpos+2) # add left of right obs
                    #-----------------------------------------------------------                
                    # add the currently generated list of x-positions to dict against the KEY of current Y-pos
                    if len(t_poslist)>0:
                        dict_y_vs_xposList[index_y]=t_poslist
                        #str1=""
                        #for x in t_poslist:
                        #    str1 += " (" + str(x) + "," + str(index_y) + ")"
                        #print("Added following positions for y=",index_y,":\n", str1, "\n\n")
                    # increment the index-Y iterator
                    index_y += inc_y
                #-----------------------------------------------------------
            #--------------------------
            #str1=""
            #for k,v in dict_y_vs_xposList.items():
            #    for x in v:
            #        str1 += " (" + str(x) + "," + str(k) + ")"
            #    str1+"\n"
            #print("All collected positions:\n", str1)
            #exit()
            #--------------------------
            # Iterate over the keys (y-pos) and value (list of x-pos) for obstacle positions
            # verify that it has a space in front of it to move to
            # if any osbtacle is moved then rest the flag "is_move" to 0
            
            keys=dict_y_vs_xposList.keys()
            #iterate in reverse order, last list of obstacles first
            revers_keys=list(reversed(keys))
            for c_key in revers_keys:
                #c_key=keys[len(keys)-1-i_key]
                x_lst=dict_y_vs_xposList[c_key]
                curr_y=c_key
                for curr_x in x_lst:
                    in_front_y=curr_y+inc_y
                    in_front_x=curr_x
                    #if any obstacle part sees a border/hurdle i.e. "#" then whole thing can't move
                    if puzzle_lines[in_front_y][in_front_x]=='#':
                        is_move=0
            #--------------------------
            if is_move:
                #print("Moving robot using move ", c_move)
                keys=dict_y_vs_xposList.keys()
                #iterate in reverse order, last list of obstacles first
                revers_keys=list(reversed(keys))
                #----------------------------------------------------------
                for c_key in revers_keys:
                    #c_key=keys[len(keys)-1-i_key]
                    x_lst=dict_y_vs_xposList[c_key]
                    curr_y=c_key
                    for curr_x in x_lst:
                        newo_y=curr_y+inc_y
                        newo_x=curr_x
                        print("\t replacing pos (",newo_x,",",newo_y,")", puzzle_lines[newo_y][newo_x], "with pos (", curr_x,",",curr_y,") ",puzzle_lines[curr_y][curr_x])
                        puzzle_lines[newo_y][newo_x]=puzzle_lines[curr_y][curr_x]
                        puzzle_lines[curr_y][curr_x]='.'
                #move the robot from the last position
                puzzle_lines[rpos_y+inc_y][rpos_x]='@'
                puzzle_lines[rpos_y][rpos_x]='.'
                #----------------------------------------------------------
            #--------------------------
        #--------------------------
        if is_move:
            new_posx=rpos_x
            new_posy=rpos_y + inc_y
    #----------------------------------------------------------------------
    else:
        print("Bad inc of x or y")
        exit()
    #----------------------------------------------------------------------

    strR=""
    if 0==is_move:
        strR+="NO MOVE WITH " + str(i_move) + " move i.e. \"" + c_move + "\""
    else:
        strR += "MOVED ROBOT WITH " + str(i_move) + " move i.e. \"" + c_move + "\"" 
    print(strR + " AND ROBOT POS NOW (", new_posx, ",", new_posy,") \n")
    # new pos of robot to be determined if moved
    return new_posx, new_posy


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
