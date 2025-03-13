from collections import defaultdict

class AreaElement:
    m_c='-'
    m_has_lt=-1
    m_has_rt=-1
    m_has_up=-1
    m_has_dn=-1
    m_pos_x=-1
    m_pos_y=-1
    def __init__(self,c,lt,rt,up,dn,px,py):
        self.m_c=c
        self.m_has_lt=lt
        self.m_has_rt=rt
        self.m_has_up=up
        self.m_has_dn=dn
        self.m_pos_x=px
        self.m_pos_y=py


def area_list_connected_to_e(area_list, e:AreaElement):
    is_connected=0
    # take the x,y for the given element
    pos_x=e.m_pos_x
    pos_y=e.m_pos_y
    # Up
    up_x=pos_x
    up_y=pos_y-1
    # Down
    dn_x=pos_x
    dn_y=pos_y+1
    # right
    rt_x=pos_x+1
    rt_y=pos_y
    # Left
    lt_x=pos_x-1
    lt_y=pos_y
    # ietrate over all elements in the list to check if any adjacent
    for a in area_list:
        is_same_up = up_x==a.m_pos_x and up_y==a.m_pos_y
        is_same_dn = dn_x==a.m_pos_x and dn_y==a.m_pos_y
        is_same_rt = rt_x==a.m_pos_x and rt_y==a.m_pos_y
        is_same_lt = lt_x==a.m_pos_x and lt_y==a.m_pos_y
        is_connected = is_same_up or is_same_dn or is_same_rt or is_same_lt
        if is_connected:
            return 1
    return is_connected

def add_to_area_dict(area_dict, area_c, area_e:AreaElement):
    dict={}
    need_reconnect=0
    x_idx=-1
    x_str=''
    n_char=0 # default index added to character when mapping to area-dict
    for ad in area_dict:
        ad_str=ad.split('_')
        # if the given character is found in the area dict
        if len(ad_str)>1 and ad_str[0]==area_c:
            # if connection found and the index > 0 then need to reconnect
            if area_list_connected_to_e(area_dict[ad], area_e):
                need_reconnect=int(ad_str[1])>0 and area_c in dict and int(ad_str[1])!=dict[area_c]
                if need_reconnect:
                    x_idx = dict[area_c]
                    x_str = ad
                    break
            # if that character is not connected to the found area
            elif not area_list_connected_to_e(area_dict[ad], area_e):
                n_char+=1 # increment the index for mapping to area-dict
        # store the index for next iteration comparison
        dict[area_c] = int(ad_str[1])
    if not need_reconnect:
        print("\tAdding ",area_c + '_' + str(n_char))
        area_dict[area_c + '_' + str(n_char)].append(area_e)
    else:
        print("\tMerging ", x_str, " into ", area_c + '_' + str(x_idx))
        tmp = area_dict[x_str]
        for t in tmp:
            area_dict[area_c + '_' + str(x_idx)].append(t)
        area_dict[area_c + '_' + str(x_idx)].append(area_e)
        # done moving now pop the last created list
        area_dict.pop(x_str)

def get_elemnent(element, pos_x, pos_y, lines):
    min_x=0
    min_y=0
    max_x=len(lines[0])-1 # as there is "\n" as extra character
    max_y=len(lines)
    up_x=pos_x
    up_y=pos_y-1
    dn_x=pos_x
    dn_y=pos_y+1
    rt_x=pos_x+1
    rt_y=pos_y
    lt_x=pos_x-1
    lt_y=pos_y
    has_up=up_x>=0 and up_x<max_x and up_y>=0 and up_y<max_y and lines[up_y][up_x]==element
    has_dn=dn_x>=0 and dn_x<max_x and dn_y>=0 and dn_y<max_y and lines[dn_y][dn_x]==element
    has_rt=rt_x>=0 and rt_x<max_x and rt_y>=0 and rt_y<max_y and lines[rt_y][rt_x]==element
    has_lt=lt_x>=0 and lt_x<max_x and lt_y>=0 and lt_y<max_y and lines[lt_y][lt_x]==element
    return AreaElement(element,has_lt,has_rt,has_up,has_dn,pos_x,pos_y)


def get_perimeter_value_for_element(ar_el:AreaElement):
    n_perimeter_val=0
    if not ar_el.m_has_up:
        n_perimeter_val+=1
    if not ar_el.m_has_dn:
        n_perimeter_val+=1
    if not ar_el.m_has_rt:
        n_perimeter_val+=1
    if not ar_el.m_has_lt:
        n_perimeter_val+=1
    return n_perimeter_val


if __name__=='__main__':

    #fileP ="puzzle_input.txt"
    fileP ="sample_input.txt"
    fH = open(fileP, "r")
    lines = fH.readlines()
    fH.close()

    areas=defaultdict(list)
    pos_y=0
    for line in lines:
        print("Processing line ", pos_y)
        pos_x=0
        for c in line:
            if c !='\n':
                e = get_elemnent(areas, pos_x, pos_y, lines)
                add_to_area_dict(areas, c, e)
            pos_x +=1
        pos_y+=1

    for area_l in areas:
        area_c = area_l
        area_p = areas[area_l]
        area_v = len(area_p)
        #print("Area for \"",area_c,"\" is ->", area_v)
        peri=0
        for e in area_p:
            peri += get_perimeter_value_for_element(e)
        print("For \"", area_c,"\" area:",area_v," peri:", peri," cost=",area_v*peri)
