from enum import Enum
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
    def str_pos(self):
        return str(self.m_x) + "," + str(self.m_y)
    def is_same(self, pos):
        return self.m_x==pos.m_x and self.m_y==pos.m_y

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
    m_pos=PosType(-1,-1)
    m_has_boundary_up=-1
    m_has_boundary_dn=-1
    m_has_boundary_rt=-1
    m_has_boundary_lt=-1
    def __init__(self,pos,up,dn,lt,rt):
        self.m_pos=pos
        self.m_has_boundary_up=up
        self.m_has_boundary_dn=dn
        self.m_has_boundary_rt=rt
        self.m_has_boundary_lt=lt
    def is_same_boundary_vertex(self, b_pos):
        return self.m_pos.is_same(b_pos.m_pos) and self.m_has_boundary_up == b_pos.m_has_boundary_up and self.m_has_boundary_dn == b_pos.m_has_boundary_dn and self.m_has_boundary_rt == b_pos.m_has_boundary_rt and self.m_has_boundary_lt == b_pos.m_has_boundary_lt
    def is_valid(self):
        return self.m_pos.is_valid()
    # are_pos1_pos2_diagonal_of_concave
    # (pos1:BoundaryType, pos2:BoundaryType):
    
    #                 ||||||||        ||||||||| 
    #                 ||||||||        |||||||||   
    #                 ||||||||        |||||||||   
    #             V1 +-------        -------+ V2
    #    =========== |                       | =========== 
    #    =========== |                       | =========== 
    #    =========== |                       | =========== 
    #    =========== |                       | =========== 
    #
    #
    #
    #   =========== |                        | =========== 
    #   =========== |                        | =========== 
    #   =========== |                        | =========== 
    #   =========== |                        | =========== 
    #               +-------       ---------+  V3
    #            V4  ||||||||        ||||||||| 
    #                ||||||||        ||||||||| 
    #                ||||||||        ||||||||| 
    #
    #
    def is_pos_making_boundary_up_lt(self, b_pos):
        #
        #         case-1                       case-2
        #                             
        #                self                  b_pos   
        #            +-------    OR          +------- 
        #     b_pos |                   self |         
        #           |                        |         
        #
        pos_case_1 = self.m_pos.m_x-1==b_pos.m_pos.m_x and self.m_pos.m_y+1==b_pos.m_pos.m_y
        has_case_1 = self.m_has_boundary_dn and b_pos.m_has_boundary_rt
        case_1 = pos_case_1 and has_case_1
        #
        pos_case_2 = b_pos.m_pos.m_x-1==self.m_pos.m_x and b_pos.m_pos.m_y+1==self.m_pos.m_y
        has_case_2 = self.m_has_boundary_rt and b_pos.m_has_boundary_dn
        case_2 = pos_case_2 and has_case_2
        return case_1 or case_2
    def is_pos_making_boundary_up_rt(self, b_pos):
        #            CASE 1                   CASE 2
        #
        #  b_pos                     self
        # ------- +          OR     -------+
        #         | self                  | b_pos
        #         |                       |
        #
        pos_case_1 = b_pos.m_pos.m_x+1==self.m_pos.m_x and b_pos.m_pos.m_y+1==self.m_pos.m_y
        has_case_1 = b_pos.m_has_boundary_dn and self.m_has_boundary_lt
        pos_case_2 = self.m_pos.m_x+1==b_pos.m_pos.m_x and self.m_pos.m_y+1==b_pos.m_pos.m_y
        has_case_2 = self.m_has_boundary_dn and b_pos.m_has_boundary_lt
        return (pos_case_1 and has_case_1) or (pos_case_2 and has_case_2)
    def is_pos_making_boundary_dn_rt(self, pos):
        #            CASE 1                CASE 2
        #
        #         | self      OR             | pos 
        #         |                          |      
        # ------- +                  ------- +      
        #     pos                       self 
        #
        pos_case_1 = pos.m_pos.m_x+1==self.m_pos.m_x and pos.m_pos.m_y-1==self.m_pos.m_y
        has_case_1 = pos.m_has_boundary_up and self.m_has_boundary_lt
        pos_case_2 = self.m_pos.m_x+1==pos.m_pos.m_x and self.m_pos.m_y-1==pos.m_pos.m_y
        has_case_2 = self.m_has_boundary_up and pos.m_has_boundary_lt
        return (pos_case_1 and has_case_1) or (pos_case_2 and has_case_2)
    def is_pos_making_boundary_dn_lt(self, pos):
        #            CASE 1                CASE 2
        #
        #         |           OR          |       
        #   self  |                  pos  |       
        #         +-------                +-------
        #            pos                   self  
        #
        pos_case_1 = pos.m_pos.m_x-1==self.m_pos.m_x and pos.m_pos.m_y-1==self.m_pos.m_y
        has_case_1 = pos.m_has_boundary_up and self.m_has_boundary_rt
        pos_case_2 = self.m_pos.m_x-1==pos.m_pos.m_x and self.m_pos.m_y-1==pos.m_pos.m_y
        has_case_2 = self.m_has_boundary_up and pos.m_has_boundary_rt
        return pos_case_1 and has_case_1 or pos_case_2 and has_case_2





def str_print_boundary_pos(p:BoundaryType):
    str1 = "\tPoint " + p.m_pos.m_c + " at " + str(p.m_pos.m_x) + "," + str(p.m_pos.m_y) + " "
    val=0
    if p.m_has_boundary_up:
        val+=1
        str1 += " has up,"
    if p.m_has_boundary_dn:
        val+=1
        str1 += " has down,"
    if p.m_has_boundary_rt:
        val+=1
        str1 += " has right,"
    if p.m_has_boundary_lt:
        val+=1
        str1 += " has left,"
    # handle no edges case
    if val==0:
        str1 += " has no edges and is all surrounded"
    return str1


def check_diag_corner(v, str_corner):
    if len(v)>=2:
        if len(v)>2:
            print(" BAD COMPUTATION OF ", str_corner," DIAG CORNER; Found ", len(v), " elements")
            exit()
        else:
            print("\tMakes ", str_corner, " with count=", len(v), " with ", v[0].m_pos.str_pos())


def get_concave_vertices_from_boundary_points1(boundary_points):
    concave_vertex = 0 # two diagonal elements make one concave side vertex
    print(" check for concave shape vertices ")
    for b in boundary_points:
        print("\t" + str_print_boundary_pos(b))
        #
        # Check if making UP LEFT corner
        v = [cxpos for cxpos in boundary_points if b.is_pos_making_boundary_up_lt(cxpos)]
        if check_diag_corner(v, "UP LEFT"):
            concave_vertex += 0.5
        # Check for any UP RIGHT element
        v = [cxpos for cxpos in boundary_points if b.is_pos_making_boundary_up_rt(cxpos)]
        if check_diag_corner(v, "UP RIGHT"):
            concave_vertex += 0.5
        # Check for any DOWN LEFT element
        v = [cxpos for cxpos in boundary_points if b.is_pos_making_boundary_dn_lt(cxpos)]
        if check_diag_corner(v, "DOWN LEFT"):
            concave_vertex += 0.5
        # Check for any DOWN RIGHT element
        v = [cxpos for cxpos in boundary_points if b.is_pos_making_boundary_dn_rt(cxpos)]
        if check_diag_corner(v, "DOWN RIGHT"):
            concave_vertex += 0.5
    #if 0!=concave_vertex%2:
    #    print("BAD NO. FOR CONCAVE VERTICES: ", concave_vertex)
    #    exit()
    return concave_vertex


def str_print_diag_corner(vPos:BoundaryType, str_corner):
    return "\t\tMakes " + str_corner + " with " + vPos.m_pos.str_pos()


class ConcavePair:
    m_p1:BoundaryType(PosType(-1,-1),-1,-1,-1,-1)
    m_p2:BoundaryType(PosType(-1,-1),-1,-1,-1,-1)
    def __init__(self, p1:BoundaryType, p2:BoundaryType):
        self.m_p1=p1
        self.m_p2=p2


def add_to_v_pair_list(c_pair, v_pair_list):
    is_there=0
    for v in v_pair_list:
        c1 = v.m_p1.m_pos.is_same(c_pair.m_p1.m_pos) and v.m_p2.m_pos.is_same(c_pair.m_p2.m_pos)
        c2 = v.m_p2.m_pos.is_same(c_pair.m_p1.m_pos) and v.m_p1.m_pos.is_same(c_pair.m_p2.m_pos)
        if c1 or c2:
            is_there = 1
            break
    if not is_there:
        v_pair_list.append(c_pair)
    return 0==is_there


def filter_inner_convex_pos_from_dict(bdry_pos_dict):
    # handle cases of internal vertex <-- exclude items
    #                          *    *                            *    *                            *    *                       
    #        case 1            *    *       case 2               *    *       case 3               *    *       case 4         
    #                          *    *                            *    *                            *    *                       
    #  ||||  V |||             *    *               ||||  V |||  *    *                            *    *  
    #  |||||||||||             *    *               |||||||||||  *    *                            * OR *                                  
    #  ----------+             * OR *               +----------  * OR *   |||| B ||||              *    *                    ||||||||||||  
    #            +----------+  *    *  +----------+              *    *   |||||||||||              *    *                    ||||| B  |||  
    #            ||||||||||||  *    *  ||||||||||||              *    *     ----------+            *    *                    +----------- 
    #            ||||| B  |||  *    *  ||||| B  |||              *    *               +----------+ *    *       -----------+          
    #                          *    *                            *    *               |||||||||||| *    *       ||||  V |||         
    #                          *    *                            *    *               ||||| V |||| *    *       ||||||||||| 
    # 
    if len(bdry_pos_dict)>=4:
        for entry in bdry_pos_dict:
            if len(bdry_pos_dict[entry])<=0:
                break
            b = bdry_pos_dict[entry][0]
            for inner_e in bdry_pos_dict:
                if len(bdry_pos_dict[inner_e])<=0:
                    break
                v = bdry_pos_dict[inner_e][0]
                is_diag_case_1 = v.m_pos.m_x+1==b.m_pos.m_x and v.m_pos.m_y+1==b.m_pos.m_y and v.m_has_boundary_dn and v.m_has_boundary_rt and b.m_has_boundary_lt and b.m_has_boundary_up
                is_diag_case_2 = v.m_pos.m_x-1==b.m_pos.m_x and v.m_pos.m_y+1==b.m_pos.m_y and v.m_has_boundary_dn and v.m_has_boundary_lt and b.m_has_boundary_rt and b.m_has_boundary_up
                is_diag_case_3 = v.m_pos.m_x-1==b.m_pos.m_x and v.m_pos.m_y-1==b.m_pos.m_y and v.m_has_boundary_up and v.m_has_boundary_lt and b.m_has_boundary_dn and b.m_has_boundary_rt
                is_diag_case_4 = v.m_pos.m_x+1==b.m_pos.m_x and v.m_pos.m_y-1==b.m_pos.m_y and v.m_has_boundary_up and v.m_has_boundary_rt and b.m_has_boundary_dn and b.m_has_boundary_lt
                if is_diag_case_1 or is_diag_case_2 or is_diag_case_3 or is_diag_case_4:
                    print("\n\t REMOVING ", v.m_pos.str_pos())
                    bdry_pos_dict[inner_e] = []


class VertexType(Enum):
        NO_TP=-1
        UP_RT=0,
        UP_LT=1,
        DN_LT=2,
        DN_RT=3

def str_vertex_type(v:VertexType) -> str:
    if v==VertexType.UP_RT:
        return "UP_RT"
    if v==VertexType.UP_LT:
        return "UP_LT"
    if v==VertexType.DN_LT:
        return "DN_LT"
    if v==VertexType.DN_RT:
        return "DN_RT"

def add_to_convex_dict(b:BoundaryType, convex_pos_dict, type_val:VertexType):
    is_added=1
    #for pos in convex_pos_dict:
    #    v1 = convex_pos_dict[pos]
    #    if len(v1)<= 0:
    #        break
    #    v = v1[0]
    #    is_same = v.is_same_boundary_vertex(b) 
    #    if is_same:
    #        is_added = 0
    #        break
    #if 1:
    #    convex_pos_dict[b.m_pos.str_pos].append(b)
    convex_pos_dict[b.m_pos.str_pos() + "_" + str_vertex_type(type_val)].append(b)
    return is_added


def get_concave_vertices_from_boundary_points(boundary_points):
    concave_vertex = 0 # two diagonal elements make one concave side vertex
    print(" check for concave shape vertices ")
    v_concave_pair=[]
    for b in boundary_points:
        has_concave_up_lt=0
        has_concave_up_rt=0
        has_concave_dn_lt=0
        has_concave_dn_rt=0
        #print("\t" + str_print_boundary_pos(b))
        #
        for cxpos in boundary_points:
            if has_concave_up_lt and has_concave_up_rt and has_concave_dn_lt and has_concave_dn_rt:
                break
            # Check if making UP LEFT corner
            if not has_concave_up_lt and b.is_pos_making_boundary_up_lt(cxpos):
                has_concave_up_lt = 1
                if add_to_v_pair_list(ConcavePair(cxpos,b), v_concave_pair):
                    print(str_print_diag_corner(cxpos, "UP LEFT"))
            # Check for any UP RIGHT element
            if not has_concave_up_rt and b.is_pos_making_boundary_up_rt(cxpos):
                has_concave_up_rt = 1
                if add_to_v_pair_list(ConcavePair(cxpos,b), v_concave_pair):
                    print(str_print_diag_corner(cxpos, "UP RIGHT"))
            # Check for any DOWN LEFT element
            if not has_concave_dn_lt and b.is_pos_making_boundary_dn_lt(cxpos):
                has_concave_dn_lt = 1
                if add_to_v_pair_list(ConcavePair(cxpos,b), v_concave_pair):
                    print(str_print_diag_corner(cxpos, "DOWN LEFT"))
            # Check for any DOWN RIGHT element
            if not has_concave_dn_rt and b.is_pos_making_boundary_dn_rt(cxpos):
                has_concave_dn_rt = 1
                if add_to_v_pair_list(ConcavePair(cxpos,b), v_concave_pair):
                    print(str_print_diag_corner(cxpos, "DOWN RIGHT"))
    #if 0!=concave_vertex%2:
    #    print("BAD NO. FOR CONCAVE VERTICES: ", concave_vertex)
    #    exit()
    #concave_vertex = 0.25 * len(v)
    concave_vertex = len(v_concave_pair)
    print(" Concave vertices found: ", concave_vertex)
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
    convex_vertices = 0
    convex_vertices_dict = defaultdict(list)
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
            # vertex top left
            if has_up and has_lt:
                if add_to_convex_dict(b, convex_vertices_dict, VertexType.UP_LT):
                    print("convex vertex: ",str_print_boundary_pos(b))
            # vertex top right
            if has_up and has_rt:
                if add_to_convex_dict(b, convex_vertices_dict, VertexType.UP_RT):
                    print("convex vertex: ",str_print_boundary_pos(b))
             # vertex bottom left
            if has_dn and has_lt:
                if add_to_convex_dict(b, convex_vertices_dict, VertexType.DN_LT):
                    print("convex vertex: ",str_print_boundary_pos(b))
            # vertex bottom right
            if has_dn and has_rt:
                if add_to_convex_dict(b, convex_vertices_dict, VertexType.DN_RT):
                    print("convex vertex: ",str_print_boundary_pos(b))
    concave_count_vertex = get_concave_vertices_from_boundary_points(boundary_points)
    #
    # Filter Inner Convex Vertices
    filter_inner_convex_pos_from_dict(convex_vertices_dict)
    #
    for v in convex_vertices_dict:
        if len(convex_vertices_dict[v])>0:
            convex_vertices += 1
    print("COnvex VErtices found: ", convex_vertices)
    return convex_vertices + concave_count_vertex



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

    #fileP ="sample_input.txt"
    fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    #fileP ="sample_input1.txt"
    #fileP ="sample_input2.txt"
    #fileP ="sample_input3.txt"
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

            n_vertices  = get_vertex_count_from_area_pos_list(areas[a])
            i_area_cost = area_count*n_vertices
            str_res.append("Region " + m + " " + str(area_count) + "*" + str(n_vertices) + "=" + str(i_area_cost))
            n_area_cost += i_area_cost

    print("\n")
    for str1 in str_res:
        print(str1)
    
    print("\n\nTOTAL COST: ", n_area_cost)
