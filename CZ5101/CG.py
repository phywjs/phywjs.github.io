# this is a python code from
# https://researchsfinest.com/2020/04/15/conjugate-gradient-in-python/
import numpy as np
def conjugate_gradient(A, b, x=None, max_iter=512, reltol=1e-2, verbose=False):
    """
    Implements conjugate gradient method to solve Ax=b for a large matrix A that is not
    computed explicitly, but given by the linear function A. 
    """
    if verbose:
        print("Starting conjugate gradient...")
    if x is None:
        x=np.zeros_like(b)
    # cg standard
    r=b-A(x)
    d=r
    rsnew=np.sum(r.conj()*r).real
    rs0=rsnew
    if verbose:
        print("initial residual: {}".format(rsnew))
    ii=0
    while ((ii<max_iter) and (rsnew>(reltol**2*rs0))):
        ii=ii+1
        Ad=A(d)
        alpha=rsnew/(np.sum(d.conj()*Ad))
        x=x+alpha*d
        if ii%50==0:
            #every now and then compute exact residual to mitigate
            # round-off errors
            r=b-A(x)
            d=r
        else:
            r=r-alpha*Ad
        rsold=rsnew
        rsnew=np.sum(r.conj()*r).real
        d=r+rsnew/rsold*d
        if verbose:
            print("{}, residual: {}".format(ii, rsnew))
    return x


# define the matrix
def A(x):
    Amat = np.array([[1,-0.5],[-0.5,2]])
    Ax = Amat.dot(x)
    return Ax

b = np.array([0.3,-0.1])
x = conjugate_gradient(A,b, reltol=1e-04)
print (A(x), b, x)



