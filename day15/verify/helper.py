
str_look="Adding for move"

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


def str_num_move(str_g):
    if str_look in str_g and len(line)>len(str_look):
        return str_g.split(str_look)[1]

def get_str_move(str_g):
    if len(line)>3:
        return str_g[-2:]

def get_verify_str_cxcy_move_oxoy(cx,cy,move,ox,oy):
    str1="n__ok"
    nx,ny=get_next_pos_from_move(ox,oy,move)
    is_ok = nx==cx and ny==cy
    if is_ok:
        str1="is_ok"
    return str1
