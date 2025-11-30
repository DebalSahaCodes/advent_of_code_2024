

b_pos=(-1,-1,'-')
g_maz={}

g_scores=[]
g_next_try=[]

g_xMax=-1
g_yMax=-1

def get_lines_from_file(file_name):
    fH=open(file_name,'r')
    f_lines=fH.readlines()
    fH.close()
    return f_lines


def populate_maze_max_and_SE(lines):
    idy=-1 
    idx=-1
    for line in lines:
        if line[-1:]=='\n':
            line=line[:-1]
        idy+=1
        idx=-1
        for c in line:
            idx+=1
            g_maz[(idx,idy)]=c
            if c=='S':
                s_pos=(idx,idy,'S')
            elif c=='E':
                e_pos=(idx,idy,'E')
    # assign the boundary max values
    xMax=idx
    yMax=idy
    return xMax, yMax, s_pos, e_pos


def get_next_pos_score_list(pos0, pos1, score):
    c_next_pos=[]
    print("\t: checking ",pos1)
    # check if '#' or 'E' is reached
    char1=g_maz[(pos1[0],pos1[1])]
    found_E = 'E'==char1
    found_H = '#'==char1
    found_B = pos1[0] < 0 or pos1[0] > g_xMax or pos1[1] < 0 or pos1[1] > g_yMax
    found_X = 0
    if len(g_scores)>0:
        found_X = score >= g_scores[0]
    #--------------------------------------
    end_consition = found_E or found_H or found_B or found_X
    #--------------------------------------
    if found_H:
        score=0
        print("\t: Found # ")
    elif found_B:
        score=0
        print("\t: Reached Boundary ")
    elif found_X:
        score=0
        print("\t: Score\"", score,"\" doesn't beat current min.\"",g_scores[0],"\"")
    elif found_E:
        print("\t: Added score " , len(g_scores)," : ", score)
        g_scores.append(score)
        #exit()
    else:
        pass
    #--------------------------------------
    if end_consition:
        #return c_next_pos
        pass # let it run to end to return empty list
    #--------------------------------------
    else:
        nx_s=[] # next possible [positions, score]
        pos1_dir=pos1[2]
        #--------------------------------------------------
        if pos1_dir=='S':
            pos1_dir='e'# replace the position of 'S' in s_pos with 'e'
        #--------------------------------------------------
        if pos1_dir=='n': # north  , don't add SOUTH to avoid reversal
            nx_s.append([(pos1[0]-1, pos1[1], 'w'),score+1001])
            nx_s.append([(pos1[0], pos1[1]-1, 'n'),score+1])
            nx_s.append([(pos1[0]+1, pos1[1], 'e'),score+1001])
        elif pos1_dir=='e': # east , don't add WEST to avoid reversal
            nx_s.append([(pos1[0], pos1[1]-1, 'n'),score+1001])
            nx_s.append([(pos1[0]+1, pos1[1], 'e'),score+1])
            nx_s.append([(pos1[0], pos1[1]+1, 's'),score+1001])
        elif pos1_dir=='s': # south, don't add NORTH to avoid reversal
            nx_s.append([(pos1[0]+1, pos1[1], 'e'),score+1001])
            nx_s.append([(pos1[0], pos1[1]+1, 's'),score+1])
            nx_s.append([(pos1[0]-1, pos1[1], 'w'),score+1001])
        elif pos1_dir=='w': # west,  don't add EAST to avoid reversal
            nx_s.append([(pos1[0], pos1[1]+1, 's'),score+1001])
            nx_s.append([(pos1[0]-1, pos1[1], 'w'),score+1])
            nx_s.append([(pos1[0], pos1[1]-1, 'n'),score+1001])
        else:
            print("BAD DIR :",pos0[2])
            exit()
        # recursive call with the new pos
        for nxs in nx_s:
            if nxs[0][0]==pos0[0] and nxs[0][1]==pos0[1] and nxs[0][2]==pos1[2]:
                continue #not be added as it is same as pos0
            elif nxs[0][0]<0 or nxs[0][0] > xMax:
                continue # out-of-bound-X
            elif nxs[0][1]<0 or nxs[0][1] > yMax:
                continue # out-of-bound-Y
            else:
                print("\t: Preparing ",pos1," -> ", nxs[0]," with score \"", nxs[1],"\"")
                c_next_pos.append((pos1, nxs[0], nxs[1]))
    #--------------------------------------
    if not c_next_pos:
        print("\t: returning empty ...")
    if len(g_scores)>0:
        g_scores.sort()
    return c_next_pos


def all_empty_list(given_list):
    is_empty=False
    for e in given_list:
        if len(e)==0:
            is_empty=True
        else:
            print("NOT EMPTY")
            is_empty=False
            break
    if is_empty:
        print(" RETURN 1")
    return is_empty


if __name__=='__main__':

    f_lines=[]
    f_name="sample2.txt"
    f_lines = get_lines_from_file(f_name);
    if not f_lines:
        print("FAILED TO PROCESS FILE: ", f_name)
        exit()

    s_pos=b_pos
    e_pos=b_pos

    xMax, yMax, s_pos, e_pos = populate_maze_max_and_SE(f_lines)
    g_xMax=xMax
    g_yMax=yMax

    print("g_xMax:",g_xMax)
    print("g_yMax:",g_yMax)
    print("s_pos:",s_pos[0],",",s_pos[1])
    print("e_pos:",e_pos[0],",",e_pos[1])


    new_pos=[]
    new_pos.append((s_pos, s_pos, 0))
    idx=0
    while(1):
        r_empty=1
        print("idx: ", idx)
        cur_output_list=[]
        for nx in new_pos:
            r_list = get_next_pos_score_list(nx[0], nx[1], nx[2])
            if r_list:
                cur_output_list.append(r_list)
                r_empty=0
        if r_empty==1:
            print("all came back empty...")
            break
        new_pos=[]
        for cur_pos in cur_output_list:
            for cx in cur_pos:
                #print("Found: (",cx[0],") -> (", cx[1],"): ", cx[2])
                new_pos.append(cx)
        idx += 1
    #get_next_from_last_two_pos(new_s_pos,new_s_pos,0)
    if len(g_scores) > 0:
        g_scores.sort()
        print('Min score:', g_scores[0])
    else:
        print("\n\nNO SCORES ADDED!!!")
