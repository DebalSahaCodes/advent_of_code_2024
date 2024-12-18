from funcs_day3 import is_numeral, is_all_num
from funcs_day3 import is_acceptable
from funcs_day3 import last_word_in_str_is_mulb, prod_from_mul_str
from funcs_day3 import last_word_in_str_is_dont


def process_lines(lines):
    isOK = 1
    isMUL = 0
    str_p = ""
    str_g = ""
    list_all= []
    for line in lines:
        for c in line:
            if c == '\n':
                continue
            else:
                str_p += c
            # process the "isOK" flag
            if "don't" == str_p[-5:]:
                #print("Found \"" + str_p[-5:] + "\" so NOT adding anymore ...")
                list_all.append("don't")
            elif "do" in str_p[-5:][0:2]:
                print("Restarting as found \"" + str_p[-5:] + "\"")
                list_all.append("do")
            # check for valid "mul(" sub-strings ELSE reset str_g
            if isMUL and str_g:
                #print("checking char: \"" + c + "\"")
                str_g += c
                if c == ')':
                    #print("Found: " + str_g)
                    list_all.append(str_g)
                    str_g = ""
            elif last_word_in_str_is_mulb(str_p):
                isMUL = 1
                str_g = "mul("
    if not list_all:
        print("String processing for MUL failed")
    else:
        ans = 0
        for m_str in list_all:
            if not isOK and m_str == "do":
                isOK = 1
                continue
            elif m_str == "don't":
                isOK  = 0
                continue
            ans += prod_from_mul_str(m_str) * isOK
        print("Answer: " + str(ans))
    return list_all


if __name__=='__main__':

    fileP ="../corrupt_mem.txt"
    fH = open(fileP, "r")
    lines = fH.readlines()
    fH.close()

    if lines:
        listA = process_lines(lines)
        for a in listA:
            print(a)
    else:
        print("Couldn't get data from file")
