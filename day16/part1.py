
s_pos=(-1,-1)
e_pos=(-1,-1)

g_maz={}
fH=open("sample2.txt",'r')
lines=fH.readlines()
fH.close()

idy=-1
for line in lines:
    if line[-1:]=='\n':
        line=line[:-1]
    idy+=1
    idx=-1
    for c in line:
        idx+=1
        g_maz[(idx,idy)]=c
        if c=='S':
            s_pos=(idx,idy)
        elif c=='E':
            e_pos=(idx,idy)

xMax=idx
yMax=idy

print("xMax:",xMax)
print("yMax:",yMax)
print("s_pos:",s_pos[0],",",s_pos[1])
print("e_pos:",e_pos[0],",",e_pos[1])


result_moves_lst=[]

class MovType:
    #m_pos=(-1,-1)
    #m_dir='-'
    #def __init__(self,x,y,d):
    #    self.m_pos=(x,y)
    #    self.m_dir=d
    def __init__(self,p,d):
        self.m_pos=p
        self.m_dir=d
    #def str_mov(self):
    #    return str(self.m_pos[0]) + "," + str(self.m_pos[1]) + "," + self.m_dir


def is_valid_next_mov(mov:MovType, c_m_lst):
    px=mov.m_pos[0]
    py=mov.m_pos[1]
    pD=mov.m_dir
    if px>=0 and px<=xMax and py>=0 and py<=yMax and g_maz[(px,py)]!='#':
        # value found is within limits and is not a HURDLE positions
        # now check if position is forward or not 
        for p in c_m_lst:
            if p.m_pos[0]==px and p.m_pos[1]==py:
                return 0# not a new pos so no need to add i.e. is invalis
        return 1
    else:
        return 0

g_dict={}
is_empty=[]
def are_all_end_of_g_dict_E():
    res=0
    str1=""
    for _,vals in g_dict.items():
        last_mov=vals[len(vals)-1]
        last_pos=last_mov.m_pos
        last_c=g_maz[last_pos]
        if 'E'==last_c:
            str1+="Last char: " + last_c
            res=1
        else:
            res=0
            break
    print(str1 + " Result:", res)
    return res

i_iter=0
n_is_empty=0
while 0 == are_all_end_of_g_dict_E():
    if 0 == i_iter:
        i_iter+=1
        g_dict[i_iter]=[]
        g_dict[i_iter].append(MovType(s_pos,'S'))
        print("Added the start element...")
        continue
    if len(g_dict)==0:
        n_is_empty+=1
    if n_is_empty>=5:
        break
    # this the first iteration starting from the base position
    #-----------------------------------------------------------
    k_del_list=[]
    #print("CURRENT DICT:")
    #for k,val in g_dict.items():
    #    print("\t",k,": ")
    #    for v in val:
    #        print("\t\t",v.m_pos[0],",",v.m_pos[1],",",v.m_dir)
    #-----------------------------------------------------------
    all_keys = list(g_dict)
    for k in all_keys:
        v=g_dict[k]
        o_list=[]
        for val in v:
            o_list.append(val)
        # get the last entry in the list for the key
        c_last=v[len(v)-1]
        last_p=c_last.m_pos
        # get all the possible 4 positions surrounding the current position
        next_4=[]
        print("Looking at the 4 positions for: ", last_p[0],",",last_p[1])
        move_4=[[0,-1,'up'],[1,0,'rt'],[0,1,'dn'],[-1,0,'lt']] # up, rt, dn, lt
        for m in move_4:
            px=last_p[0]+m[0]
            py=last_p[1]+m[1]
            pD=m[2]
            nx = MovType((px,py),pD)
            if is_valid_next_mov(nx, v):
                next_4.append(nx)
        if len(next_4)>0:
            for i,n in enumerate(next_4):
                str_P="\tadding " + str(n.m_pos[0]) + "," + str(n.m_pos[1])  + "," + n.m_dir
                if i==0:
                    str_P += " to same key:" + str(k)
                    g_dict[k].append(n)
                else:
                    str_P += " to new key:" + str(i_iter+1)
                    i_iter+=1
                    g_dict[i_iter]=[]
                    for val in o_list:
                        #print("copying from key:", k," val:", val.m_pos[0],",", val.m_pos[1],",", val.m_dir)
                        g_dict[i_iter].append(val)
                    g_dict[i_iter].append(n)
                print(str_P)
                # chcek if the newly added moves leads to eh 'E' character in maze
                m_char=g_maz[n.m_pos]
                if m_char=='E':
                    # print this dictonary & add to results list
                    print("GOOD MOVES DICT:")
                    e_values=[]
                    result_moves=[]
                    if i==0:
                        e_values=g_dict[k]
                    else:
                        e_values=g_dict[i_iter]
                    for v in e_values:
                        print("\t\t",v.m_pos[0],",",v.m_pos[1],",",v.m_dir)
                        result_moves.append(v)
                    result_moves_lst.append(result_moves)
        else:
            k_del_list.append(k)
    for key in k_del_list:
        print("\tDELETING ", key," ...")
        del g_dict[key]

min_score=0
for idx,result_moves in enumerate(result_moves_lst):
    print("FOUND RESULT MOVES..\nCounting scores now:")
    o_mov=MovType((-1,-1),'_')
    score=0
    for mov in result_moves:
        if mov.m_dir=='S':
            o_mov=mov
            continue
        else:
            if mov.m_pos[0]!=o_mov.m_pos[0]:
                #print("1 for moving in x dir")
                score +=1
            elif mov.m_pos[1]!=o_mov.m_pos[1]:
                #print("1 for moving in y dir")
                score +=1
            if o_mov.m_dir != mov.m_dir:
                #print("1000 for chaning from ",o_mov.m_dir, " to ", mov.m_dir)
                score+=1000
            o_mov=mov
    print("SCORE ", idx,": ", score)
    if score < min_score or min_score==0:
        min_score=score


print("\n\nFINAL SCORE: ", min_score)
