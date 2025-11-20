puzzle_lines=[]

# couting GPS for board in file
fName="final_sample1.txt"
fH=open(fName)
puzzle_lines=fH.readlines()
fH.close()

xMax=len(puzzle_lines[0])-1
if puzzle_lines[0][-1:]=="\n":
    xMax-=1
yMax=len(puzzle_lines)-1

print("xMax:",xMax)
print("yMax:",yMax)

gps_sum=0
pos_x=-1
pos_y=-1
for line in puzzle_lines:
    pos_y+=1
    pos_x=0
    if line[-1:]=='\n':
        line=line[:-1]
    for c in line:
        d_from_up=0
        d_from_dn=0
        d_from_rt=0
        d_from_lt=0
        if c=="[":
            d_from_up=pos_y
            #d_from_dn=yMax-pos_y
            #c_y_wise=min(d_from_up, d_from_dn)
            c_y_wise=d_from_up
            d_from_lt=pos_x
            #d_from_rt=xMax-pos_x-1
            #c_x_wise=min(d_from_lt, d_from_rt)
            c_x_wise=d_from_lt
            c_gps=100*c_y_wise + c_x_wise
            print("for pos (",str(pos_x),",",str(pos_y),") GPS X_WISE:",c_x_wise, " and Y_WISE:",c_y_wise, " , gps result:",c_gps)
            gps_sum += c_gps
        pos_x+=1

print("TOTAL GPS SUM:", gps_sum)
