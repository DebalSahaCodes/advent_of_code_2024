import time

def clean_leading_zeros(cnum):
    strn=""
    lidx=0
    for c in cnum:
        if c!='0':
            break
        else:
            lidx += 1
    for idx in range(lidx, len(cnum)):
        strn += cnum[idx]
    return strn


def get_post_apply_rules(c_list):
    idx=0
    lst=[]
    for c in c_list:
        #print("Processing ", c)
        if c=="0":
            #print(".. applying rule 1...")
            lst.append("1")
        elif len(c)%2==0:
            #print(".. applying rule 2...")
            cmid=int(len(c)/2)
            lst.append(c[0:cmid])
            lst.append(str(int(c[cmid:len(c)])))#remove leading zero by converting to int then back to str
            sr1=lst[len(lst)-2:len(lst)]
            #print("adding",sr1[0]," and ", sr1[1])
        else:
            #print(".. applying rule 3...")
            lst.append(str(int(c)*2024))
    c_list = lst
    return c_list

def print_stones(c_list):
    pstr="\nTotal " + str(len(c_list)) + " stones:\n"
    for s in c_list:
        pstr += s + " "
    print(pstr)


def get_num_list(file_path):
    num_list=[]
    fH=open(file_path)
    lines=fH.readlines()
    fH.close()
    if len(lines)>0:
        num_list=lines[0].split(' ')
    return num_list


if __name__=='__main__':
    file_p="puzzle_input.txt"
    #file_p="sample_2.txt"
    startT=time.time()
    c_list=get_num_list(file_p)
    if not c_list:
        print("Failed to parse file \"" + file_path + "\"")
        exit()
    print("starting list of stones")
    print_stones(c_list)
    #apply rule n times:
    n_rules=25
    for i in range(n_rules):
        c_list = get_post_apply_rules(c_list)
        #print("After blink ", i)
        #print_stones(c_list)
    print("After blink ", n_rules)
    print_stones(c_list)
    t2=time.time() - startT
    print("\n\nTime taken: ", t2," sec")
