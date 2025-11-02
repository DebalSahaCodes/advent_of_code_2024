
board={}
posmap=[]

class PosType:
    x=-1
    y=-1
    def __init__(self, i, j):
        self.x=i
        self.y=j

class WallType:
    slope_x=-1
    slope_y=-1
    def __init__(self, i, j):
        self.slope_x=i
        self.slope_y=j


def add_up_wall():
    pass

def add_dn_wall():
    pass

def add_rt_wall():
    pass

def add_lt_wall():
    pass


def pos_to_str(i,j) -> str:
    return str(i) + ',' + str(j)

if __name__=='__main__':

    #fileP ="sample_some.txt"
    #fileP ="puzzle_input.txt"
    #fileP ="sample_input.txt"
    #fileP ="sample_input1.txt"
    #fileP ="sample_input2.txt"
    fileP ="../sample_input3.txt"
    fH = open(fileP, "r")
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
                add_up_wall()
            if has_char_lt:
                print("HAs char LEFT")
                exists=1
                str_f=pos_to_str(i-1,j)
                print(" to look into pos ", pos_to_str(i-1,j))
                to_finds.append(pos_to_str(i-1,j))
            else: #else add up wall
                add_lt_wall()
            # Since done checking UP and LEFT, so establish if NEW or Exists
            if not exists:
                is_new=1
            # Now check the new positions (no need to find exisiting)
            if has_char_dn: #new char
                print("HAs char DOWN")
            else:
                add_dn_wall()
            if has_char_rt: #new char
                print("HAs char RIGHT")
            else:
                add_rt_wall()
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
    for i in v:
        print("item in \"", k,"\":", i)
