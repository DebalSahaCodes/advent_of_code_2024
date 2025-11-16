
bad_val=0.01

n_tokens_A=3
n_tokens_B=1

def get_vals_from_line(line_g):
    lineP0=line_g.split(': ')[1]
    lineP1=lineP0.split(',')
    # Depending on the string sent decide the char 
    # that can be used to split and obtain the two
    # values
    # for Button use '+'
    # for Prize use '='
    sign_split='+'
    if "Prize:" in line_g:
        sign_split='='
    vals=[]
    for p in lineP1:
        i_str=p.split(sign_split)[1]
        i_val=int(i_str)
        #print("Value from i_str: ", i_val)
        vals.append(i_val)
    return vals[0],vals[1]


def get_coeff_from_lines(lines):
    # initialize with a bad value
    a1=bad_val
    a2=bad_val
    b1=bad_val
    b2=bad_val
    p1=bad_val
    p2=bad_val
    #---------------------------------
    if len(lines) !=3:
        print("Bad data :\n")
        for idx,line in enumerate(lines) :
            print(idx, ": ", line)
        exit()
    # get Button A value from line-0
    #----------------------------------
    if "Button A:" not in lines[0]:
        print("Line-0 is not Button A, given line: ", lines[0])
        exit()
    else:
        a1,a2 = get_vals_from_line(lines[0])
    #----------------------------------
    # get Button B value from line-1
    #----------------------------------
    if "Button B:" not in lines[1]:
        print("Line-1 is not Button B, given line: ", lines[1])
        exit()
    else:
        b1,b2 = get_vals_from_line(lines[1])
    #----------------------------------
    # get the prize's X and Y 
    #----------------------------------
    if "Prize:" not in lines[2]:
        print("Line-2 is not \"Prize:\", given line: ", lines[2])
        exit()
    else:
        p1,p2 = get_vals_from_line(lines[2])
    
    return a1,a2,b1,b2,p1,p2

def is_integer(num):
    return int(num) == num


def solve_for_buttons_A_B(a1,a2,b1,b2,p1,p2):
    # Equations from lines:
    # a1*X and a2*Y from line 0  
    # b1*X and b2*Y from line 1 
    # p1=X and p2=Y from line 2
    # 
    # Solving for X and Y from the 3 lines:
    # a1*X + b1*X = p1
    # and
    # a2*Y + b2*Y = p2
    #
    # Now a1=a2 and b1=b2 because a1 & both represent "Price of Button A"
    # and similary both b1 and b2 represent prices for Button B
    # 
    # Table of the following format :
    #
    # -----------------------------------
    #   X  |  Y  |
    # -----------------------------------
    #  a1  |  a2 |
    #  b1  |  b2 |
    # ----------------
    #  p1 |  p2  |
    # -----------------------------------
    #
    # Button A: X+94, Y+34
    # Button B: X+22, Y+67
    # Prize: X=8400, Y=5400
    # -----------------------------------
    #   X    |    Y  |
    # -----------------------------------
    #  94*a  |  34*a |
    #  22*b  |  67*b |
    # ----------------
    #  8400 |  5400  |
    # -----------------------------------
    # And this gives us equation:
    #  94*a + 22*b = 8400
    #  34*a + 67*b = 5400
    # From the above two eq.s, we can derive a and b:
    # a = (8400 - 22*b)/94 = (5400 - 67*b)/34
    # i.e.
    # a = (p1 - b1*b)/a1 = (p2 - b2*b)/a2
    # solving for b, gives:
    # b = (p1*a2 - p2*a1)/(a2*b1 - a1*b2)
    
    B = (p1*a2 - p2*a1)/(a2*b1 - a1*b2)
    A = (p1 - b1*B)/a1 #= (p2 - b2*B)/a2
    #print("Type of A i.e.", A,": ", type(A))
    #print("Type of B i.e.", B,": ", type(B))
    return A,B

n_tokens_prize=0
n_numbers_prize=0

if __name__=='__main__':
    flieLines=[]

    file_name="puzzle.txt"
    #file_name="sample1.txt"

    fH = open(file_name, "r")
    flieLines = fH.readlines()
    fH.close()

    machines=[]
    t_lines_in_3=[]
    for line in flieLines:
        #print("\tProcessing line: ", line)
        if "Button A" in line or "Button B" in line or "Prize:" in line:
            #print("\tAdding line: ", line)
            t_lines_in_3.append(line)
        if len(t_lines_in_3)==3:
            machines.append(t_lines_in_3)
            t_lines_in_3=[] # reset
        if line=="\n":
            continue

    lines_in_3=[]
    for lines_in_3 in machines:
        str1=""
        a1,a2,b1,b2,p1,p2 = get_coeff_from_lines(lines_in_3)
        A,B = solve_for_buttons_A_B(a1,a2,b1,b2,p1,p2)
        str1="\n Found coefficients: A:" + str(A) + "\tB:" + str(B)
        if is_integer(A) and is_integer(B):
            i_tokens_prize = n_tokens_A*A + n_tokens_B*B
            str1+=".. use tokens:" + str(i_tokens_prize)
            n_tokens_prize += i_tokens_prize
            n_numbers_prize += 1
        else:
            str1=""#+="..skipping..."
        if str1:
            print(str1)

    print("\n\n")
    print("TOTAL PRIZES: ", n_numbers_prize)
    print("TOTAL TOKENS: ", n_tokens_prize) 
