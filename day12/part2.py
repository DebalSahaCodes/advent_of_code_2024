
posmap=[]



def pos_to_str(i,j) -> str:
    return str(i) + ',' + str(j)

def str_to_pos(str):
    parts=str.split(',')
    return int(parts[0]), int(parts[1])

if __name__=='__main__':

    #file_name="puzzle_input.txt"
    #file_name="sample_input1.txt"
    #file_name="sample_input2.txt"
    #file_name="sample_input3.txt"
    #file_name="sample_input4.txt"
    #file_name="sample_input5.txt"
    #file_name="sample_extra0.txt"
    #file_name="sample_extra1.txt"
    #file_name="sample_extra3.txt"

    fH = open(file_name, "r")
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

    n_row=len(posmap1)
    n_col=len(posmap1[0])

    print("Toltal rows: ", n_row)
    print("Toltal cols: ", n_col)

    for row in range(n_row):
        loct=[]
        print("adding for row: ", row)
        for col in range(n_col):
            loct.append(posmap1[row][col])
        posmap.append(loct)

    xMax=n_row
    yMax=n_col
    print("\n")
    print("Value of xMax:", xMax)
    print("Value of yMax:", yMax)
    #exit()



    dict_i={} # map char-name TO list of char-positions

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

            if has_char_lt:
                print("HAs char LEFT")
                exists=1
                str_f=pos_to_str(i-1,j)
                print(" to look into pos ", pos_to_str(i-1,j))
                to_finds.append(pos_to_str(i-1,j))

            if not exists:
                is_new=1
            # Now check the new positions (no need to find exisiting)
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

# perimter wall prices
tot_price=0

for k,pos_list in dict_i.items():

    n_pos=len(pos_list)

    str_p="\nEstimating walls by counting corners for \"" + k + "\" ..."

    n_corners = 0

    for pos in pos_list:
        i,j=str_to_pos(pos)
        #print("starting ", i,',',j)
        c=posmap[i][j]
        has_char_up=pos_to_str(i,j-1) in pos_list
        has_char_lt=pos_to_str(i-1,j) in pos_list
        has_char_dn=pos_to_str(i,j+1) in pos_list
        has_char_rt=pos_to_str(i+1,j) in pos_list
        
        # Types of corners:
        out_corner_N_W = not has_char_lt and not has_char_up
        out_corner_N_E = not has_char_rt and not has_char_up
        out_corner_S_W = not has_char_rt and not has_char_dn
        out_corner_S_E = not has_char_lt and not has_char_dn
        if out_corner_N_W:
            n_corners += 1
        if out_corner_N_E:
            n_corners += 1
        if out_corner_S_W:
            n_corners += 1
        if out_corner_S_E:
            n_corners += 1
            
        has_diag_N_W = pos_to_str(i-1,j-1) in pos_list
        has_diag_N_E = pos_to_str(i+1,j-1) in pos_list
        has_diag_S_W = pos_to_str(i-1,j+1) in pos_list
        has_diag_S_E = pos_to_str(i+1,j+1) in pos_list
        in_corner_N_W = has_char_lt and has_char_up and not has_diag_N_W
        in_corner_N_E = has_char_rt and has_char_up and not has_diag_N_E
        in_corner_S_W = has_char_lt and has_char_dn and not has_diag_S_W
        in_corner_S_E = has_char_rt and has_char_dn and not has_diag_S_E
        if in_corner_N_W:
            n_corners += 1
        if in_corner_N_E:
            n_corners += 1
        if in_corner_S_W:
            n_corners += 1
        if in_corner_S_E:
            n_corners += 1

    n_walls=n_corners

    tot_price+=n_pos*n_walls
    str_p += "\nFor " + k + " : " + str(n_pos) + " * " + str(n_walls) + ": " + str(n_pos*n_walls)
    print(str_p)

print("\n\nTOTAL PRICE FOR FILE \"", file_name, "\": ", tot_price)
