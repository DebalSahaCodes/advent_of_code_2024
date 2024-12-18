from funcs_day3 import is_numeral, is_all_num
from funcs_day3 import is_acceptable

def test1():
    # Test : 
    if is_numeral('4'):
        print("Is a num")
    else:
        print("Is NOT a num")

    str1 = "HEJWQELWEJ 000 25"
    last2 = str1[-2:]
    if is_all_num(last2):
        print("All num")
    else:
        print("\"" + last2 + "\" : nope not all num")

def test2():
    # Test
    str1 = "mul(263*,25)"
    isOK = 1
    for c in str1:
        if not is_acceptable:
            print("Faulty Test")
            isOK = 0
            break
    if isOK:
        print("Good string")



if __name__=='__main__':
    test2()
