from collections import defaultdict

class PosType:
    m_x=-1
    m_y=-1
    def __init__(self, x, y):
        self.m_x=x
        self.m_y=y


class AreaType:
    m_char=''
    m_pos_list=[]
    def insert_pos(self, pos:PosType):
        m_pos_list.append(pos)
    def view_char(self):
        return self.m_char
    def view_count(self):
        return len(m_pos_list)
    def is_within_area(self, posObj):
        pass

def populate_char_pos_map(char_pos_map, lines):
    pos_y=0
    for line in lines:
        #print("Processing line ", pos_y)
        pos_x=0
        for c in line:
            if c !='\n':
                char_pos_map[c].append(PosType(pos_x,pos_y))
            pos_x +=1
        pos_y+=1


def get_perimeter_value_for_pos(pos:PosType, pos_list):
    has_up=0
    has_dn=0
    has_rt=0
    has_lt=0
    # take the x,y for the given element
    pos_x=pos.m_x
    pos_y=pos.m_y
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
    for a in pos_list:
        if not has_up and up_x==a.m_x and up_y==a.m_y:
            has_up = 1
        if not has_dn and dn_x==a.m_x and dn_y==a.m_y:
            has_dn =1
        if not has_rt and rt_x==a.m_x and rt_y==a.m_y:
            has_rt =1
        if not has_lt and lt_x==a.m_x and lt_y==a.m_y:
            has_lt=1
    # calculate
    n_perimeter_val=0
    if not has_up:
        n_perimeter_val+=1
    if not has_dn:
        n_perimeter_val+=1
    if not has_rt:
        n_perimeter_val+=1
    if not has_lt:
        n_perimeter_val+=1
    return n_perimeter_val

def pos_connected_to_area(pos:PosType, areaAsPosList):
    is_connected=0
    # take the x,y for the given element
    pos_x=pos.m_x
    pos_y=pos.m_y
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
    for a in areaAsPosList:
        has_up = up_x==a.m_x and up_y==a.m_y
        has_dn = dn_x==a.m_x and dn_y==a.m_y
        has_rt = rt_x==a.m_x and rt_y==a.m_y
        has_lt = lt_x==a.m_x and lt_y==a.m_y
        is_connected = has_up or has_dn or has_rt or has_lt
        if is_connected:
            break
    return is_connected

def create_areas_from_char_pos_map(char, pos_list, areas):
    merge_1="-"
    merge_2="-"
    merge_p=[]
    print("Processing map of ", char)
    for cPos in pos_list:
        is_added=0
        if areas:
            for areaC in areas:
                if pos_connected_to_area(cPos, areas[areaC]):
                    if is_added: # connected but previously added to some other area
                        print("Found another connection in ", areaC)
                        merge_1=areaC
                    else: # not added
                        print("\tAdding to ", areaC)
                        areas[areaC].append(cPos)
                        merge_2=areaC
                        is_added=1
                if merge_1!="-" and merge_2!="-":
                    print("\tMerging ", merge_1, " and ", merge_2)
                    for mPos in areas[merge_2]:
                        areas[merge_1].append(mPos)
                    areas[merge_2]=[]
                    merge_p.append(merge_2)
                    merge_1="-"
                    merge_2="-"
        if not is_added:
            key_str = str(len(areas)+1)
            print("\tAdding to ", key_str)
            areas[key_str].append(cPos)
    for n in merge_p:
        areas.pop(n)


def cost_for_areas(dict_areas):
    cost = 0
    for area_c in dict_areas:
        p_list = dict_areas[area_c]
        for pos in p_list:
            get_perimeter_value_for_pos(pos, p_list)

if __name__=='__main__':

    fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    #fileP ="sample_input1.txt"
    fH = open(fileP, "r")
    lines = fH.readlines()
    fH.close()

    map_char_pos = defaultdict(list)
    populate_char_pos_map(map_char_pos, lines)
    for m in map_char_pos:
        print(m,": ", len(map_char_pos[m]))

    str_res=[]
    n_area_cost=0

    for m in map_char_pos:
        areas=defaultdict(list)
        create_areas_from_char_pos_map(m, map_char_pos[m], areas)
        for a in areas:
            area_count = len(areas[a])
            p=0
            peri=0
            for c in areas[a]:
                peri = get_perimeter_value_for_pos(c, areas[a])
                #print("perimeter for pos ", c.m_x, ",",c.m_y," is ", peri)
                p+=peri
            i_area_cost = area_count*p
            str_res.append("Region " + m + " " + str(area_count) + "*" + str(p) + "=" + str(i_area_cost))
            n_area_cost += i_area_cost

    print("\n")
    for str1 in str_res:
        print(str1)
    
    print("\n\nTOTAL COST: ", n_area_cost)
