# LAB 1, resistor network code
#
# "Numerical Recipes" ludcmp() C code on page 46 translated into Python
# I make it as simple as possible, disregard efficiency.
# Here a is a list of list, n is integer (size of the matrix)
# index is a list, and d is also a list of size 1
# Python list index starts from 0.  So matrix index is from 0 to n-1.
#
import math

def ludcmp(a, n, indx, d):
    d[0] = 1.0
    # looking for the largest a in each row and store it in vv as inverse
    # We need a new list same size as indx, for this we use .copy() 
    vv=indx.copy()
    for i in range(0,n):
        big = 0.0
        for j in range(0,n):
            temp = math.fabs(a[i][j])
            if (temp > big):
                big = temp
        vv[i]=1.0/big
    #
    # run Crout's algorithm
    for j in range(0,n):
        # top half & bottom part are combined
        # but the upper limit l for k sum is different
        big = 0.0
        for i in range(0,n):           
            if(i<j):
                l = i
            else:
                l = j
            sum = a[i][j]    
            for k in range(0,l):
                sum -= a[i][k]*a[k][j]
            # end for k
            a[i][j] = sum
            # for bottom half, we keep track which row is larger
            if(i>=j):
                dum = vv[i]*math.fabs(sum)
                if (dum >= big):
                    big = dum
                    imax = i
            # end if (i>= ...)
        # end for i
        # pivoting part, swap row j with row imax, a[j] is a whole row
        if (j != imax):
            dum = a[imax]
            a[imax] = a[j]
            a[j] = dum
            d[0] = - d[0]
            vv[imax] = vv[j]
        # end if (j != ...)
        # divide by the beta diagonal value
        indx[j] = imax
        dum = 1.0/a[j][j]
        for i in range(j+1,n):
            a[i][j] *= dum
        # end for i 
    # end for j 
# end of def ludcmp

# We do backward substitution in lubksb() take the row swapped LU decomposed
# a, size n, and swapping indx, and b vector as input.  The output is
# in b after calling.
def lubksb(a, n, indx, b):
    ii = -1
    # forward
    for i in range(0,n):
        ip = indx[i]
        sum = b[ip]
        b[ip] = b[i]
        if(ii != -1):
            for j in range(ii,i):
                sum -= a[i][j]*b[j]
        elif(sum != 0):
            ii = i
        b[i] = sum
    # bote alpha_{ii} is 1 above
    #  backward
    for i in range(n-1,-1,-1):
        sum = b[i]
        for j in range(i+1,n):
            sum -= a[i][j]*b[j]
        b[i] = sum/a[i][i]
# end lubksb()

            
# unfortunately a is destroyed (become swapped LU)
def linearsolver(a,n,b):
    indx = list(range(n))
    d =[1]
    ludcmp(a,n,indx,d)
    x = b.copy()
    lubksb(a,n,indx,x)
    return x
# end linearsolver


# for the matrix A
def genmatrx(L):
    M=L+1
    M2=M*M
    # initialize a M by M zero marix
    rowzero=[0]*M2
    A = []
    for i in range(0,M2):
        row = rowzero.copy()
        A.append(row)
    # site A is i = 0, site B is M2-1
    A[0][0]=1
    A[M2-1][M2-1]=1

    # buid a list of neighbors of i
    # use it to set the nonzero entries of A
    for i in range(1,M2-1):
        neighbor=[]
        if i%M != 0:
            neighbor.append(i-1)
        if (i+1)%M != 0:
            neighbor.append(i+1)
        if (i-M)>= 0:
            neighbor.append(i-M)
        if (i+M)< M2:
            neighbor.append(i+M)
        A[i][i] = len(neighbor)
        for j in neighbor:
            A[i][j] = -1
    return A

# current calculation
def current(L):
    M=L+1
    M2=M*M
    # initialize the b vector
    b=[0]*M2
    b[M2-1]=1
    A=genmatrx(L)
    V=linearsolver(A,M2,b)
    return (V[1] + V[M])


    
    


        
        

    




    
