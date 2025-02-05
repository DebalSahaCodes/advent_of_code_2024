import time

def get_checksum(filesys):
    chksum=0
    for idx in range(len(filesys)-1):
        fileblk=filesys[idx]
        if fileblk==".":
            break
        else:
            i_char=int(fileblk)
            print("Adding ",idx,"*",i_char, " :", idx*i_char) 
            chksum += idx*i_char
    return chksum

def strfromlist(liststr):
    strlist=""
    for c in liststr:
        strlist+=c
    return strlist

def rearrange_filesys(filesys):
    # process as list of blocks where each blockis of file-id or space type
    n_itr=len(filesys)
    n_max=n_itr-1
    itr_lastc=0
    for idx in range(n_itr):
        curr_pos=n_max-idx
        if filesys[curr_pos]=='.':
            continue
        else:
            prn_str = "Moving \"" + filesys[curr_pos] + "\" at pos " + str(curr_pos)
            while itr_lastc<curr_pos:
                if filesys[itr_lastc]=='.':
                    break
                itr_lastc+=1
            # if the first available "." is at a position > current-position; that means all previous positions are file-id so break
            if itr_lastc>=curr_pos-1:
                break
            else:
                prn_str += ".. to replace \"" + filesys[itr_lastc] + "\" at pos " + str(itr_lastc) 
                filesys[itr_lastc]=filesys[curr_pos]
                filesys[curr_pos]="."
                prn_str += ": " + strfromlist(filesys)
                #print(prn_str)
    # now convert the list back to string and return
    #filesys_str=""
    #filesys_str=strfromlist(filesys)
    return filesys

def rearrange_filesys_old(filesys_str):
    filesys=list(filesys_str)
    n_iter=len(filesys)
    for idx in range(n_iter):
        # select the file-char from the end
        itr1=len(filesys)-1
        while itr1>=0:
            if filesys[itr1]!=".":
                break
            itr1-=1
        char_files=filesys[itr1]
        # select the space-char from the beginning
        itr2=0
        while itr2<(len(filesys)-1):
            if filesys[itr2]==".":
                break
            itr2+=1
        char_space=filesys[itr2]
        #print("Rearranging \"" + char_files + "\" at pos " + str(itr2) + " and \"" + char_space + "\" at pos " + str(itr1))
        tmp=filesys[itr2]
        filesys[itr2]=filesys[itr1]
        filesys[itr1]=tmp
    filesys_str=""
    for c in filesys:
        filesys_str+=c
    return filesys_str


def create_filesys(charsline):
    filesys=[]# process as list of blocks where each blockis of file-id or space type
    i_file=0
    idx=0
    for bchar in charsline:
        #print("Found char: " + bchar)
        bcount=int(bchar)
        tmp=[]
        itr=0
        if idx%2==0:# is file, add file-ID
            tmp=[str(i_file)]*bcount
            i_file += 1
        else: # is space, add "."
            tmp=['.']*bcount
        for t in tmp:
            filesys.append(t)
        idx += 1
    return filesys


if __name__=='__main__':
    start_time = time.time()
    #file_name="sample_input_1.txt"
    #file_name="sample_input.txt"
    file_name="puzzle_input.txt"
    f_handle=open(file_name,'r')
    lines=[]
    lines=f_handle.readlines()
    if not lines:
        print("\n\nFailed to read filepath \"" + file_name + "\"\n")
        exit()
    # read all the characters in the line read from file
    filesys_lines=""
    # create a list of strings representing the file-id blocks interspersed with space blocks (".")
    filesys=[]
    filesys=create_filesys(lines[0])
    print("After extracting files and spaces: \"" + strfromlist(filesys) + "\"")
    filesys=rearrange_filesys(filesys)
    tot_time = time.time() - start_time
    print('Total time take: {:.10}'.format(tot_time), " seconds")
    #exit()
    print("After processing files and spaces: \"" + strfromlist(filesys) + "\"")
    chksum=get_checksum(filesys)
    print("Checksum is: " + str(chksum))
    tot_time = time.time() - start_time
    print('Total time take: {:.10}'.format(tot_time), " seconds")
