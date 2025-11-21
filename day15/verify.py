puzzle_lines=[]

c_robot='@'
c_stopc='#'
c_let_box='['

xMax=-1
yMax=-1


file_name="puzzle_file_final_board.txt"
ctempL=[]
fH=open(file_name, 'r')
ctempL=fH.readlines()
fH.close()


cstrlook=""
str_look="Adding for move"
str_block="####################################################################################################"
puzzleboard_dict={}
i_flag=0
print("Readig file \"", file_name,"\" to create puzzle boards...")
for idx,line in enumerate(ctempL):
    if line[-1:] == '\n':
        line=line[:-1]
    if str_look in line:
        #print("Starting with line:",str(idx+1)," \"",cstrlook,"\"")
        cstrlook=line
        puzzleboard_dict[cstrlook]=[]
        continue
    if i_flag:
        puzzleboard_dict[cstrlook].append(line)
    if str_block in line:
        if 1==i_flag:
            #print("Done with line:",str(idx+1)," \"",cstrlook,"\"")
            i_flag=0
        else:
            i_flag=1

init_posx=-1
init_posy=-1

def get_next_pos_from_move(currx, curry, move):
    next_x=currx
    next_y=curry
    if move=='<':
        next_x=currx-1
        next_y=curry
    elif move=='>':
        next_x=currx+1
        next_y=curry
    elif move=='^':
        next_x=currx
        next_y=curry-1
    elif move=='v':
        next_x=currx
        next_y=curry+1
    return next_x, next_y


def get_str_xy(cx,cy):
    return "("+str(cx)+","+str(cy)+")"


def get_str_move(str_g):
    if len(line)>3:
        return str_g[-2:]

init_posx=-1
init_posy=-1

def do_verify(d_line, old_posx, old_posy):
    c_posx=-1
    c_posy=-1
    line=puzzleboard_dict[d_line]
    for line in v:
        c_posy+=0
        c_posx=0
        for c in line:
            c_posx+=1
            if c=='@':
                if i_init:
                    init_posx=c_posx
                    init_posy=c_posy
                    print("first init rob pos: ",get_str_xy(c_posx,c_posy))
                    #return 
                else:
                    #check positions: current = last iter + move
                    str_move=get_str_move(k)
                    str_ans=get_verify_str_cxcy_move_oxoy(c_posx,c_posy,str_move,old_posx,old_posy)
                    ny_move="yes move"
                    if old_posx==c_posx and old_posy==c_posy:
                        ny_move="no move"
                    pr_str  = str_ans + ": for "+ get_str_xy(c_posx,c_posy)
                    pr_str += " with move \"" + str_move + " and old pos "
                    pr_str += get_str_xy(old_posx,old_posy) + ": " + ny_move
                    print(pr_str)
                    #exit()
                return c_posx, c_posy
    return -10,-11


def get_verify_str_cxcy_move_oxoy(cx,cy,move,ox,oy):
    str1="n__ok"
    nx,ny=get_next_pos_from_move(ox,oy,move)
    is_ok = nx==cx and ny==cy
    if is_ok:
        str1="is_ok"
    return str1

i_init=0
o_posx=-1
o_posy=-1
for k,v in puzzleboard_dict.items():
    if "initial-no" in k:
        i_init=1
    else:
        i_init=0
    if i_init==1:
        print("found initial board")
    #c_posx=-1
    #c_posy=-1
    #is_done=0
    #for line in v:
    #    posy+=0
    #    posx=0
    #    for c in line:
    #        if c=='@':
    #            c_posx=posx
    #            c_posy=posy
    #            if i_init:
    #                init_posx=c_posx
    #                init_posy=c_posy
    #                print("first init rob pos: ",get_str_xy(c_posx,c_posy))
    #            else:
    #                #check positions: current = last iter + move
    #                str_move=get_str_move(k)
    #                str_ans=get_verify_str_cxcy_move_oxoy(c_posx,c_posy,str_move,o_posx,o_posy)
    #                ny_move="yes move"
    #                if o_posx==c_posx and o_posy==c_posy:
    #                    ny_move="no move"
    #                print(str_ans,": for ",get_str_xy(c_posx,c_posy)," with move \"", str_move," and old pos ",get_str_xy(o_posx,o_posy),": ", ny_move)
    #                #exit()
    #            
    #   o_posx=c_posx
    #   o_posy=c_posy
    c_posx,c_posy=do_verify(k,o_posx,o_posy)
    o_posx=c_posx
    o_posy=c_posy
