from collections import defaultdict

class PosType:
    m_c="-"
    m_x=-1
    m_y=-1
    def __init__(self, x, y, c="-"):
        self.m_x=x
        self.m_y=y
        self.m_c=c
    def is_valid(self):
        return self.m_x>=0 and self.m_y>=0 and self.m_c!="-"

def populate_char_pos_map(char_pos_map, lines):
    pos_y=0
    for line in lines:
        #print("Processing line ", pos_y)
        pos_x=0
        for c in line:
            if c !='\n':
                #print("\tAdding ", c," at ",pos_x,",",pos_y)
                char_pos_map[c].append(PosType(pos_x, pos_y, c))
            pos_x +=1
        pos_y+=1

class BoundaryType:
    m_centr_pos=PosType(-1,-1)
    m_up_lt_pos=PosType(-1,-1)
    m_up_rt_pos=PosType(-1,-1)
    m_dn_lt_pos=PosType(-1,-1)
    m_dn_rt_pos=PosType(-1,-1)
    m_has_up=-1
    m_has_dn=-1
    m_has_rt=-1
    m_has_lt=-1
    def __init__(self,pos,up,dn,lt,rt):
        self.m_centr_pos=pos
        self.m_up_lt_pos=PosType(pos.m_c, pos.m_x-1,pos.m_y-1)
        self.m_up_rt_pos=PosType(pos.m_c, pos.m_x+1,pos.m_y-1)
        self.m_dn_lt_pos=PosType(pos.m_c, pos.m_x-1,pos.m_y+1)
        self.m_dn_rt_pos=PosType(pos.m_c, pos.m_x+1,pos.m_y+1)
        self.m_has_up=up
        self.m_has_dn=dn
        self.m_has_rt=rt
        self.m_has_lt=lt
    def is_valid(self):
        return self.m_has_up!=-1 and self.m_has_dn!=-1 and self.m_has_rt!=-1 and self.m_has_lt!=-1 and self.m_posType.m_x!=-1 and self.m_posType.m_y!=-1


def str_print_boundary_pos(p:BoundaryType):
    str1 = "\tPoint " + p.m_centr_pos.m_c + " at " + str(p.m_centr_pos.m_x) + "," + str(p.m_centr_pos.m_y) + " "
    val=0
    if p.m_has_up:
        val+=1
        str1 += " has up,"
    if p.m_has_dn:
        val+=1
        str1 += " has down,"
    if p.m_has_rt:
        val+=1
        str1 += " has right,"
    if p.m_has_lt:
        val+=1
        str1 += " has left,"
    # handle no edges case
    if val==0:
        str1 += " has no edges and is all surrounded"
    return str1

def make_concave_top_left(cpos:BoundaryType, pos:BoundaryType):
    #            CASE 1                   CASE 2
    #
    #           pos                       cpos
    #        +-------    OR          +-------
    #  cpos |                   pos |
    #       |                       |
    #
    pos_case_1 = pos.m_centr_pos.m_x-1==cpos.m_centr_pos.m_x and pos.m_centr_pos.m_y+1==cpos.m_centr_pos.m_y
    has_case_1 = pos.m_has_dn and cpos.m_has_rt
    pos_case_2 = cpos.m_centr_pos.m_x-1==pos.m_centr_pos.m_x and cpos.m_centr_pos.m_y+1==pos.m_centr_pos.m_y
    has_case_2 = cpos.m_has_dn and pos.m_has_rt
    return pos_case_1 and has_case_1 or pos_case_2 and has_case_2



def make_concave_top_right(cpos:BoundaryType, pos:BoundaryType):
    #            CASE 1                   CASE 2
    #
    #     pos                     cpos
    # ------- +          OR     -------+
    #         | cpos                  | pos
    #         |                       |
    #
    pos_case_1 = pos.m_centr_pos.m_x+1==cpos.m_centr_pos.m_x and pos.m_centr_pos.m_y+1==cpos.m_centr_pos.m_y
    has_case_1 = pos.m_has_dn and cpos.m_has_lt
    pos_case_2 = cpos.m_centr_pos.m_x+1==pos.m_centr_pos.m_x and cpos.m_centr_pos.m_y+1==pos.m_centr_pos.m_y
    has_case_2 = cpos.m_has_dn and pos.m_has_lt
    return pos_case_1 and has_case_1 or pos_case_2 and has_case_2


def make_concave_bottom_right(cpos:BoundaryType, pos:BoundaryType):
    #            CASE 1                CASE 2
    #
    #         | cpos      OR             | pos 
    #         |                          |      
    # ------- +                  ------- +      
    #     pos                       cpos 
    #
    pos_case_1 = pos.m_centr_pos.m_x+1==cpos.m_centr_pos.m_x and pos.m_centr_pos.m_y-1==cpos.m_centr_pos.m_y
    has_case_1 = pos.m_has_up and cpos.m_has_lt
    pos_case_2 = cpos.m_centr_pos.m_x+1==pos.m_centr_pos.m_x and cpos.m_centr_pos.m_y-1==pos.m_centr_pos.m_y
    has_case_2 = cpos.m_has_up and pos.m_has_lt
    return pos_case_1 and has_case_1 or pos_case_2 and has_case_2



def make_concave_bottom_left(cpos:BoundaryType, pos:BoundaryType):
    #            CASE 1                CASE 2
    #
    #         |           OR          |       
    #   cpos  |                  pos  |       
    #         +-------                +-------
    #            pos                   cpos  
    #
    pos_case_1 = pos.m_centr_pos.m_x-1==cpos.m_centr_pos.m_x and pos.m_centr_pos.m_y-1==cpos.m_centr_pos.m_y
    has_case_1 = pos.m_has_up and cpos.m_has_rt
    pos_case_2 = cpos.m_centr_pos.m_x-1==pos.m_centr_pos.m_x and cpos.m_centr_pos.m_y-1==pos.m_centr_pos.m_y
    has_case_2 = cpos.m_has_up and pos.m_has_rt
    return pos_case_1 and has_case_1 or pos_case_2 and has_case_2



def  get_concave_vertices_from_boundary_points(boundary_points):
    concave_vertex = 0 # two diagonal elements make one concave side vertex
    print(" check for concave shape vertices ")
    for b in boundary_points:
        print("\t" + str_print_boundary_pos(b))
        v = []
        v = [cxpos for cxpos in boundary_points if make_concave_top_left(cxpos, b)]
        if len(v)>0:
            concave_vertex += 0.5
            print("\tHas UP LEFT counter pos")
        else:
            print("\tHas  no counter pos at Up Left")

        v=[]
        v = v = [cxpos for cxpos in boundary_points if make_concave_top_right(cxpos, b)]
        if len(v)>0:
            concave_vertex += 0.5
            print("\tHas  counter pos at UP RIGHT")
        else:
            print("\tHas  no counter pos at UP RIGHT")

        v=[]
        v = [cxpos for cxpos in boundary_points if make_concave_bottom_left(cxpos, b)]
        if len(v)>0:
            concave_vertex += 0.5
            print("\tHas  counter pos at DOWN LEFT")
        else:
            print("\tHas  no counter pos at DOWN LEFT")

        v=[]
        v = [cxpos for cxpos in boundary_points if make_concave_bottom_right(cxpos, b)]
        if len(v)>0:
            concave_vertex += 0.5
            print("\tHas  counter pos at DOWN RIGHT")
        else:
            print("\tHas  no counter pos at DOWN RIGHT")

    return concave_vertex


def get_vertex_count_from_area_pos_list(area_pos_list):
    count_vertex = 0
    str1 = "Processing area of " + str(len(area_pos_list)) + " points "
    if len(area_pos_list) > 0:
        str1 += " of type character \"" + area_pos_list[0].m_c + "\""
        print(str1)
    else:
        print(str1 + "\nSince zero size, returning ...")
        return


    boundary_points = []
    # iterate over all elements in the list to check if any adjacent element is there
    for cPos in area_pos_list:
        has_up = 1
        has_dn = 1
        has_lt = 1
        has_rt = 1
        str1 = ""
        for aPos in area_pos_list:
            if 0==has_lt and 0==has_rt and 0==has_up and 0==has_dn:
                break
            if cPos.m_x==aPos.m_x and cPos.m_y-1==aPos.m_y:
                # found an element above current pos so make "has_dn" FALSE
                has_up = 0
            if cPos.m_x==aPos.m_x and cPos.m_y+1==aPos.m_y:
                # found an element below current pos so make "has_dn" FALSE
                has_dn = 0
            if cPos.m_x+1==aPos.m_x and cPos.m_y==aPos.m_y:
                # found an element at RIGHT side of current pos so make "has_rt" FALSE
                has_rt = 0
            if cPos.m_x-1==aPos.m_x and cPos.m_y==aPos.m_y:
                # found an element at LEFT side of current pos so make "has_lt" FALSE
                has_lt = 0
        if has_up or has_dn or has_lt or has_rt:
            b = BoundaryType(cPos, has_up, has_dn, has_lt, has_rt)
            boundary_points.append(b)
            #print(str_print_boundary_pos(b))
        # check  convex side vertices
        if has_up and has_lt:
            count_vertex += 1 # vertex top left
        if has_up and has_rt:
            count_vertex += 1 # vertex top right
        if has_dn and has_lt:
            count_vertex += 1 # vertex bottom left
        if has_dn and has_rt:
            count_vertex += 1 # vertex bottom right
    return count_vertex + get_concave_vertices_from_boundary_points(boundary_points)



def pos_connected_to_area(pos:PosType, areaAsPosList):
    is_connected=0
    # compute the expected X,Y values for the up,down,left,right positions for a given X,Y position
    up_x=pos.m_x      # Up X
    up_y=pos.m_y-1    # UP Y
    dn_x=pos.m_x      # Down X
    dn_y=pos.m_y+1    # Down Y
    rt_x=pos.m_x+1    # right X
    rt_y=pos.m_y      # right Y
    lt_x=pos.m_x-1    # Left X
    lt_y=pos.m_y      # Left Y
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
    #print("Processing map of ", char)
    for cPos in pos_list:
        is_added=0
        if areas:
            for areaC in areas:
                if pos_connected_to_area(cPos, areas[areaC]):
                    if is_added: # connected but previously added to some other area
                        print("Found another connection in ", areaC)
                        merge_1=areaC
                    else: # not added
                        #print("\tAdding to ", areaC)
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
            #print("\tAdding to ", key_str)
            areas[key_str].append(cPos)
    for n in merge_p:
        areas.pop(n)




if __name__=='__main__':

    #fileP ="sample_some.txt"
    #fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    #fileP ="sample_input1.txt"
    #fileP ="sample_input2.txt"
    fileP ="sample_input3.txt"
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

            #print("\tFound boundary points: ", len(boundary_pos_list))

            # sort using custom function that sorts using bigger-vertical_pos then bigger-horizontal_pos
            # i.e. from left-to-right and  from top-to-bottom 
            #sorted(boundary_pos_list, cmp=sorting_function_value_for_boundary_pos)

            count_vertex = get_vertex_count_from_area_pos_list(areas[a])

            i_area_cost = area_count*count_vertex
            str_res.append("Region " + m + " " + str(area_count) + "*" + str(count_vertex) + "=" + str(i_area_cost))
            n_area_cost += i_area_cost

    print("\n")
    for str1 in str_res:
        print(str1)
    
    print("\n\nTOTAL COST: ", n_area_cost)
