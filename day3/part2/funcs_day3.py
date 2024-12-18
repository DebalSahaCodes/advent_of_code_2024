

def last_word_in_str_is_mulb(str1):
    return str1[-4:] == "mul("


def last_word_in_str_is_dont(str1):
    return str1[-5:] == "don't"

def is_numeral(c):
    if c in ['0','1','2','3','4','5','6','7','8','9']:
        return 1
    else:
        return 0

def is_all_num(numg):
    isOK = 1
    for c in numg:
        isOK = isOK and is_numeral(c)
    return isOK

def is_acceptable(ch):
    if ch in [',', ')'] or is_numeral(ch):
        return 1;
    else:
        return 0

def prod_from_mul_str(mul_str):
    prod = 0
    if "mul(" == mul_str[:4] and ")" == mul_str[-1:]:
        mul_str = (mul_str.split("mul(")[1])[:-1] # remove "mul(" and ")"
        prt = mul_str.split(',')
        #print("parts: ")
        print(prt)
        if len(prt) == 2 and is_all_num(prt[0]) and is_all_num(prt[1]):
            prod = int(prt[0]) * int(prt[1])
    return prod

