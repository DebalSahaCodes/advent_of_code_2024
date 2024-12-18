from funcs_day3 import is_numeral, is_all_num
from funcs_day3 import is_acceptable
from funcs_day3 import last_word_in_str_is_mulb, prod_from_mul_str
from funcs_day3 import last_word_in_str_is_dont



def process_lines(lines):
    isOK = 1
    isMUL = 0
    str_p = ""
    str_g = ""
    list_mul= []
    for line in lines:
        for c in line:
            if c == '\n':
                continue
            else:
                str_p += c
            # process the "isOK" flag
            if isOK:
                isOK = last_word_in_str_is_dont(str_p)
            else:
                isOK = "do" in str_p[:-5] and not last_word_in_str_is_dont(str_p)
            # check for valid "mul(" sub-strings ELSE reset str_g
            if isMUL:
                #print("checking char: \"" + c + "\"")
                str_g += c
                if c == ')':
                    #print("Found: " + str_g)
                    list_mul.append(str_g)
                    str_g = ""
            elif last_word_in_str_is_mulb(str_p):
                isMUL = 1
    if not list_mul:
        print("String processing for MUL failed")
    else:
        ans = 0
        for m_str in list_mul:
            ans += prod_from_mul_str(m_str)
        print("Answer: " + str(ans))



if __name__=='__main__':

    fileP ="C:/dsaha/__Work/_advent_of_code/2024_AOCpp/day3/corrupt_mem.txt"
    fH = open(fileP, "r")
    lines = fH.readlines()
    fH.close()

    if lines:
        process_lines(lines)
    else:
        print("Couldn't get data from file")
