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
    is_inserted=0
    is_merging=0
    x_a_char='' # to be removed post merge
    n_char=0 # default index added to character when mapping to area-dict
    for ad in area_dict:
        # the key in dict is of format CHAR_NUM e.g. "C_10"
        # split the key with "_" as delimiter;so 
        # - CHAR="C" 
        # - NUM="11"
        ad_str=ad.split('_')
        if len(ad_str)<=1:
            print("BAD DICT ELEMENT CHAR", ad,"\nEXITING...")
            exir()
        a_chr=ad_str[0]
        a_num=int(ad_str[1])
        # if the CHAR part matches the current character in AreaElement
        if a_chr==area_c:
            # if area to which current element is connected, is found, then insert
            if area_list_connected_to_e(area_dict[ad], area_e):
                if not is_inserted:
                    print("\tAdding to ", ad)
                    area_dict[ad].append(area_e)
                    is_inserted = 1
                    # store the index in a local dict for next iteration
                    dict[area_c] = ad#int(ad_str[1])
                    continue
                else:
                    is_merging=1 # already inserted and found connected to current area
                    print("\tMerging ", ad, " into ", dict[area_c])
                    for t in area_dict[ad]:
                        area_dict[dict[area_c]].append(t)
                    x_a_char = ad # set the merged list for removal
            else: #if not connected
                n_char+=1

    if not is_inserted:
        print("\tInsert ",area_c + '_' + str(n_char))
        area_dict[area_c + '_' + str(n_char)].append(area_e)

    if x_a_char and is_merging: # if non empty list remaining that was merged then pop from area_dict
        print("\tRemoving merged list ", x_a_char)
        area_dict.pop(x_a_char)



def get_elemnent(element, pos_x, pos_y, lines):
    str1=""
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
    #if has_up:
    #    str1 +=" has_up"
    #if has_dn:
    #    str1 +=" has_dn"
    #if has_rt:
    #    str1 +=" has_rt"
    #if has_lt:
    #    str1 +=" has_lt"
    #print(element, str1)
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

    fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    #fileP ="sample_input1.txt"
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
                e = get_elemnent(c, pos_x, pos_y, lines)
                add_to_area_dict(areas, c, e)
            pos_x +=1
        pos_y+=1


    total_cost=0
    for area_l in areas:
        area_c = area_l
        area_p = areas[area_l]
        area_v = len(area_p)
        #print("Area for \"",area_c,"\" is ->", area_v)
        peri=0
        for e in area_p:
            peri += get_perimeter_value_for_element(e)
        print("Region ", area_c," with price ",area_v,"*", peri," = ",area_v*peri)
        total_cost += area_v*peri
    print("\n\nTotal_cost: ", total_cost)
