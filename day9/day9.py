
def get_checksum(filesys):
    chksum=0
    for idx in range(len(filesys)-1):
        c_char=filesys[idx]
        if c_char==".":
            break
        else:
            i_char=int(c_char)
            chksum += idx*i_char
    return chksum

def rearrange_filesys(filesys_str):
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
    filesys=""
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
            filesys += t
        idx += 1
    return filesys


if __name__=='__main__':
    #file_name="sample_input.txt"
    file_name="puzzle_input.txt"
    f_handle=open(file_name,'r')
    lines=[]
    lines=f_handle.readlines()
    if not lines:
        print("\n\nFailed to read file :\"" + file_name + "\"\n")
        exit()
    # remove the new-line char from the end of the lines being read from file
    filesys=create_filesys(lines[0])
    print("After reading lines: \"" + filesys + "\"")
    filesys=rearrange_filesys(filesys)
    print("After rearranging file and space: \"" + filesys + "\"")
    chksum=get_checksum(filesys)
    print("Checksum is: " + str(chksum))
