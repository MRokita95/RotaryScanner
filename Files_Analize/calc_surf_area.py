
def calc_area(x, y):

    pi = 3.14 
    result = 0.0
    i = 0

    while i < len(x)-1:
        val_x = x[i + 1] - x[i]
        val_y = y[i]
        result += (pi * val_x) /360 * (val_y * val_y)
        i +=1
    
    return result