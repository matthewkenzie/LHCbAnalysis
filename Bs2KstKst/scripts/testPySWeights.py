import numpy as np
from scipy.stats import rv_continuous
from scipy.stats import norm
from scipy.special import kv
from scipy.special import gamma
#from ROOT.Math import cyl_bessel_k
#from ROOT.TMath import Gamma
class ipatia_gen(rv_continuous):
  "Ipatia distribution"

  def low_x_BK(self, nu, x):
    return gamma(nu)*(2**(nu-1.))*(x**(-nu))

  def low_x_lnBK(self, nu, x):
    return np.log( gamma(nu) + (nu-1.)*np.log(2.) - nu*np.log(x))

  def BK(self, ni, x):
    nu = np.abs(ni)
    if ( x < 1.e-6 and nu > 0. ):
      return self.low_x_BK(nu,x)
    if ( x < 1.e-4 and nu > 0. and nu < 55. ):
      return self.low_x_BK(nu,x)
    if ( x < 0.1 and nu > 55.):
      return self.low_x_BK(nu,x)
    return kv(nu,x)

  def LnBK(self, ni, x):
    nu = np.abs(ni)
    if ( x < 1.e-6 and nu > 0. ):
      return self.low_x_lnBK(nu,x)
    if ( x < 1.e-4 and nu > 0. and nu < 55. ):
      return self.low_x_lnBK(nu,x)
    if ( x < 0.1 and nu > 55.):
      return self.low_x_lnBK(nu,x)
    return np.log(kv(nu,x))


  def LogEval(self, d, l, alpha, beta, delta):
    gamma = alpha
    dg = delta*gamma
    thing = delta*delta + d*d
    logno = l*np.log(gamma/delta) - np.log( (2*np.pi)**0.5 ) - self.LnBK(l, dg)
    return np.exp( logno + beta*d + (0.5-l)*(np.log(alpha)-0.5*np.log(thing)) + self.LnBK(l-0.5,alpha*(thing**0.5)))

  def diff_eval(self, d, l, alpha, beta, delta):
    gamma = alpha
    dg = delta*gamma
    thing = delta*delta + d*d
    sqthing = thing**0.5
    alphasq = alpha*sqthing
    no = ((gamma/delta)**l)/self.BK(l,dg)*((2*pi)**(-0.5))
    ns1 = 0.5-l
    return no*(alpha**ns1)*(thing**(l/2.-1.25))*(-d*alphasq*(self.BK(l-1.5,alphasq) + self.BK(l+0.5, alphasq)) + (2.*(beta*thing + d*L) - d)*self.BK(ns1,alphasq))*np.exp(beta*d)/2.

  def _pdf(self,x,l,zeta,fb,a,n,a2,n2):
    d = x-loc
    cons0 = zeta**0.5
    asigma = a*scale
    a2sigma = a2*scale
    out = 0.
    if (zeta!=0):
      phi = self.BK(l+1.,zeta)/self.BK(l,zeta)
      cons1 = scale/(phi**0.5)
      alpha = cons0/cons1
      beta = fb
      delta = cons0*cons1

      if ( d < -asigma ):
        k1 = self.LogEval(-asigma,l,alpha,beta,delta)
        k2 = self.diff_eval(-asigma,l,alpha,beta,delta)
        B = -asigma + n*k1/k2
        A = k1*((B+asigma)**n)
        out = A*((B-d)**(-n))
      elif ( d > a2sigma ):
        k1 = self.LogEval(a2sigma,l,alpha,beta,delta)
        k2 = self.diff_eval(a2sigma,l,alpha,beta,delta)
        B = -a2sigma - n2*k1/k2
        A = k1*((B+a2sigma)**n2)
        out = A*((B+d)**(-n2))
      else:
        out = self.LogEval(d,l,alpha,beta,delta)

    elif ( l < 0. ):
      beta = fb
      cons1 = -2.*l
      if ( l<=-1.0 ):
        delta = scale * (-2+cons1)**0.5
      else:
        print "WARNING: zeta==0 and l>-1 ===> not defined rms. Changing the meaning of sigma"
        delta = scale

      delta2 = delta**2
      if ( d < -asigma):
        cons1 = np.exp( -beta*asigma )
        phi = 1. + asigma*asigma/delta2
        k1 = cons1*(phi**(l-0.5))
        k2 = beta*k1 - cons1*(l-0.5)*(phi**(l-1.5))*2*asigma/delta2
        B = -asigma + n1*k1/k2
        A = k1*((B+asigma)**n)
        out = A*((B-d)**(-n))
      elif ( d > a2sigma ):
        cons1 = np.exp( beta*a2sigma )
        phi = 1. + a2sigma*a2sigma/delta2
        k1 = cons1*(phi**(l-0.5))
        k2 = beta*k1 + cons1*(l-0.5)*(phi**(l-1.5))*2*a2sigma/delta2
        B = -a2sigma - n2*k1/k2
        A = k1*((B+a2sigma)**n2)
        out = A*((B+d)**(-n2))
      else:
        out = np.exp(beta*d)*((1.+d*d/delta2)**(l-0.5))
    else:
      print "zeta=0 only supported for l<0"

    return out
ipatia = ipatia_gen(name='ipatia', a=0, b=1)

import matplotlib.pyplot as plt
x = np.linspace(5000,5800,100)

plt.plot( x, ipatia.pdf(x, l=-5.3715, zeta=0, fb=0, a=2.5, n=2.5, a2=2.575, n2=2.526, loc=5200, scale=10), label='frozen pdf')
#plt.plot( x, ipatia.pdf(x, l=-5.3715, zeta=0, fb=0, sigma=14.744, mu=5368.43, a=2.5, n=2.5, a2=2.575, n2=2.526), label='frozen pdf')
#plt.plot( x, norm.pdf(x, loc=5368.43, scale=14.744), label='gaus' )
plt.legend()
plt.show()



