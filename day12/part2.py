
board={}
posmap=[]

class PosType:
    x=-1
    y=-1
    def __init__(self, i, j):
        self.x=i
        self.y=j

class WallType:
    slope=-1
    def __init__(self, i, j):
        self.slope_x=i
        self.slope_y=j


wall_dict={}

def add_up_wall(wall_dict):
    pass

def add_dn_wall(wall_dict):
    pass

def add_rt_wall(wall_dict):
    pass

def add_lt_wall(wall_dict):
    pass


def pos_to_str(i,j) -> str:
    return str(i) + ',' + str(j)

def str_to_pos(str):
    parts=str.split(',')
    return int(parts[0]), int(parts[1])

if __name__=='__main__':

    file_name1 ="../sample_some.txt"
    file_name2 ="../puzzle_input.txt"
    file_name3 ="../sample_input.txt"
    file_name4 ="../sample_input1.txt"
    file_name5 ="../sample_input2.txt"
    file_name6 ="../sample_input3.txt"
    fH = open(file_name2, "r")
    lines = fH.readlines()
    fH.close()
    j=-1
    posmap1=[]
    for line in lines:
        j+=1
        i=-1
        tLine=[]
        for c in line:
            if c!='\n':
                i+=1
                tLine.append(c)
        posmap1.append(tLine)

    print("Toltal cols: ", len(posmap1[0]))

    for col in range(len(posmap1[0])):
        loct=[]
        for row in range(len(posmap1)):
            loct.append(posmap1[col][row])
        posmap.append(loct)



    xMax=i+1
    yMax=j+1
    print(xMax)
    print(yMax)



    #for j in range(yMax):
    #    str1=""
    #    for i in range(xMax):
    #        str1+=posmap[i][j]
    #    print(str1)
    #exit()

    dict_c={} # stores the names of the maps
    dict_i={} # map char-name TO list of char-positions
    #print("\n ",board[str(1)+','+str(0)])
    #exit()
    # read board from left to right, top to bottom
    # so if the element is found in up or lt then element existed, else new
    # up, lt <-- EXISTING
    # dn, rt <-- NEW
    x_slope_str='x_'
    y_slope_str='y_'
    for j in range(yMax):
        for i in range(xMax):
            print("starting ", i,',',j)
            c=posmap[i][j]
            has_char_up=i>=0 and j-1>=0 and i<xMax and j-1<yMax and c==posmap[i][j-1]
            has_char_lt=i-1>=0 and j>=0 and i-1<xMax and j<yMax and c==posmap[i-1][j]
            has_char_dn=i>=0 and j+1>=0 and i<xMax and j+1<yMax and c==posmap[i][j+1]
            has_char_rt=i+1>=0 and j>=0 and i+1<xMax and j<yMax and c==posmap[i+1][j]
            is_new=0
            exists=0
            to_finds=[]
            to_add=pos_to_str(i,j)
            if has_char_up:
                print("HAs char up")
                exists=1
                print(" to look into pos ", pos_to_str(i,j-1))
                to_finds.append(pos_to_str(i,j-1))
            else: #else add up wall
                #add_up_wall(i,j-0.5)
                wall_slope=x_slope_str+str(j-0.5) #along X-axis at a pos y_pos-0.5
                if wall_slope in wall_dict:
                    wall_dict[wall_slope]+=1
                else:
                    wall_dict[wall_slope]=1
            if has_char_lt:
                print("HAs char LEFT")
                exists=1
                str_f=pos_to_str(i-1,j)
                print(" to look into pos ", pos_to_str(i-1,j))
                to_finds.append(pos_to_str(i-1,j))
            else: #else add up wall
                #add_lt_wall(i-0.5,j)
                wall_slope=y_slope_str+str(i-0.5) #along Y-axis at a pos x_pos-0.5
                if wall_slope in wall_dict:
                    wall_dict[wall_slope]+=1
                else:
                    wall_dict[wall_slope]=1
            # Since done checking UP and LEFT, so establish if NEW or Exists
            if not exists:
                is_new=1
            # Now check the new positions (no need to find exisiting)
            if has_char_dn: #new char
                print("HAs char DOWN")
            else:
                #add_dn_wall()
                wall_slope=x_slope_str+str(j+0.5) #along X-axis at a pos y_pos+0.5
                if wall_slope in wall_dict:
                    wall_dict[wall_slope]+=1
                else:
                    wall_dict[wall_slope]=1
            if has_char_rt: #new char
                print("HAs char RIGHT")
            else:
                #add_rt_wall()
                wall_slope=y_slope_str+str(i+0.5) #along Y-axis at a pos x_pos+0.5
                if wall_slope in wall_dict:
                    wall_dict[wall_slope]+=1
                else:
                    wall_dict[wall_slope]=1
            n_keys=0
            if exists:
                found_k=[]
                l_idx=len(dict_i.items())
                idx=0
                for to_f in to_finds:
                    for k,vals in dict_i.items():
                        idx+=1
                        strp ="\n Finding " + to_f + " in " + k
                        for v in vals:
                            if v==to_f:
                                strp += ".. found " + to_f + " in key " + k
                                if k not in found_k:
                                    strp += ".. adding to keys list.."
                                    print(strp)
                                    found_k.append(k)
                    print(strp)
                n_keys=len(found_k)
                print("SIZE OF KEYS LISTR FOUND: ", n_keys)
                if n_keys>2:
                    print("MAny keys , more than 2.. no. of keys found: ", n_keys)
                    exit()
                elif n_keys == 2:
                    print("Now will merge two (Add to prev. and delete the later) ...")
                    # time to merge two lists
                    print("Adding to prev the value ", v)
                    for v in dict_i[found_k[1]]:
                        if v not in dict_i[found_k[0]]:
                            dict_i[found_k[0]].append(v)
                    del dict_i[found_k[1]]
                        #exit()
                k=found_k[0]
                dict_i[k].append(to_add)
                print("Added (", to_add, ") to existing ", k)
            if n_keys==0 or is_new: # Add the new character
                print("IS NEW...")
                idx=0
                for k in dict_i.keys():
                    p=k.split('_')
                    if p[0]==c:
                        idx=int(p[1])+1
                cidx=c+'_'+str(idx)
                # add the new element to a new list
                print("\n Adding new key ", cidx, "with pos (", to_add, ")")
                dict_i[cidx]=[]
                dict_i[cidx].append(to_add)
            print("Loop done!\n********\n")




for k,v in dict_i.items():
    print("items in \"", k,"\":", len(v))


print("\n\n")
for k,v in wall_dict.items():
    print("items in \"", k,"\":", v)

print("\n Mapping walls to regions \n")

list_walls=wall_dict.keys()
new_wall_dict={}
for k in dict_i.keys():
    new_wall_dict[k]=0

#for w in list_walls:
#    is_found=0
#    w_pos=float(w.split('_')[1])
#    for k,pos_list in dict_i.items():
#        for pos in pos_list:
#            p_i,p_j=str_to_pos(pos)
#            if p_i-0.5==w_pos or p_i+0.5==w_pos or p_j-0.5==w_pos or p_j+0.5==w_pos:
#                print("Adding wall \"", w, "\" to ", k)
#                new_wall_dict[k]+=1
#                break


# perimter wall prices
tot_price=0
for k,pos_list in dict_i.items():
    n_pos=len(pos_list)
    str_p="\nCounting walls for \"" + k + "\" ..."
    wall_dict1={}
    for pos in pos_list:
        i,j=str_to_pos(pos)
        #print("starting ", i,',',j)
        c=posmap[i][j]
        has_char_up=pos_to_str(i,j-1) in pos_list
        has_char_lt=pos_to_str(i-1,j) in pos_list
        has_char_dn=pos_to_str(i,j+1) in pos_list
        has_char_rt=pos_to_str(i+1,j) in pos_list
        wall_slopes=[]
        if not has_char_up:
            wall_slope1 = x_slope_str+str(j-0.5) #along X-axis at a pos y_pos-0.5
            str_p += "\n\t adding wall for(" + pos + ") -> " + wall_slope1
            if wall_slope1 not in wall_dict1:
                wall_dict1[wall_slope1]=[]
            wall_dict1[wall_slope1].append(i)
        if not has_char_rt:
            wall_slope2 = y_slope_str+str(i+0.5) #along Y-axis at a pos x_pos+0.5
            str_p += "\n\t adding wall for(" + pos + ") -> " + wall_slope2
            if wall_slope2 not in wall_dict1:
                wall_dict1[wall_slope2]=[]
            wall_dict1[wall_slope2].append(j)
        if not has_char_dn:
            wall_slope3 = x_slope_str+str(j+0.5) #along X-axis at a pos y_pos+0.5
            str_p += "\n\t adding wall for(" + pos + ") -> " + wall_slope3
            if wall_slope3 not in wall_dict1:
                wall_dict1[wall_slope3]=[]
            wall_dict1[wall_slope3].append(i)
        if not has_char_lt:
            wall_slope4 = y_slope_str+str(i-0.5) #along Y-axis at a pos x_pos-0.5
            str_p += "\n\t adding wall for(" + pos + ") -> " + wall_slope4
            if wall_slope4 not in wall_dict1:
                wall_dict1[wall_slope4]=[]
            wall_dict1[wall_slope4].append(j)
        #for wall_slope in wall_slopes:
        #    str_p += "\n\t adding wall for(" + pos + ") -> " + wall_slope
        #    if wall_slope in wall_dict1:
        #        wall_dict1[wall_slope]+=1
        #    else:
        #        wall_dict1[wall_slope]=1

    n_walls=0
    for kw, vw in wall_dict1.items():
        n_walls+=1
        vw.sort()
        max_vw=len(vw)
        for idx in range(max_vw):
            if idx+1 < max_vw and vw[idx+1] - vw[idx]>1:
                n_walls+=1
    tot_price+=n_pos*n_walls
    str_p += "\nFor " + k + " : " + str(n_pos) + " * " + str(n_walls) + ": " + str(n_pos*n_walls)
    print(str_p)

print("\n\nTOTAL PRICE: ", tot_price)
