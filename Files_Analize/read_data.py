import os
import sys
from pathlib import Path


import numpy as np
import matplotlib.pyplot as plt

#file_name = "Pos_Dist_Data_1"

def plot_and_calc_data(file):
    file_name = file
    #file_to_open = f'C:\QT_Apps\Datafiles\{file_name}'
    file_to_open = file


    #reading file with column titles at first line
    with open(file_to_open,'r') as f:
        line1 = f.readline()
        label_x = [line1.split()[0] ]
        label_y = [line1.split()[1] ]
        print (label_x)
        print (label_y)

        datas = f.readlines()
        pos = [p.split()[0] for p in datas]
        dist = [d.split()[1] for d in datas]
        
        
    pos_arr =[]
    dist_arr = []
    for p in pos:
        pos_arr.append(float(p))
    for d in dist:
        dist_arr.append(float(d))
    
    print (pos_arr)
    print (dist_arr)

    from calc_surf_area import calc_area 

    #call function calculating area and round precission to 2
    result = round(calc_area(pos_arr, dist_arr), 2)
    print(result)


    #plot 
    fig = plt.figure()

    fig.suptitle(f"Measured surface area = {result} cm2")
    ax1 = fig.add_subplot(111)

    unit_x = "deg"
    lab_x = f"{label_x[0]} [{unit_x}]"

    unit_y = "cm"
    lab_y = f"{label_y[0]} [{unit_y}]"

    font1 = {'family':'serif','color':'blue','size':20}
    font2 = {'family':'serif','color':'darkred','size':15}

    ax1.set_title("Rotary Scanner data", fontdict = font1)    
    ax1.set_xlabel(lab_x, fontdict = font2)
    ax1.set_ylabel(lab_y, fontdict = font2)



    ax1.plot(pos_arr,dist_arr, c='r', label='measured data')

    leg = ax1.legend()

    max_y = max(dist_arr)+10
    plt.axis([min(pos_arr), max(pos_arr), 0, max_y])

    plt.show()

def main(argv):
    plot_and_calc_data(argv)

if __name__ == '__main__':
    main(sys.argv[1])

