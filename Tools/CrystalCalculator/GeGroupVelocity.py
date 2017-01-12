import math
import numpy as np
import continuum as dyn

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

#plt.rcParams['image.cmap'] = 'viridis';

numt = 161;
nump = 321;

t = np.linspace(0,math.pi,numt)
p = np.linspace(0,math.pi*2.0,nump)

#vectors holding results for phase velocity
L = np.zeros(numt*nump);
FT = np.zeros(numt*nump);
ST = np.zeros(numt*nump);

#vectors holding polarizations
pL = np.zeros((3,numt*nump));
pFT = np.zeros((3,numt*nump));
pST = np.zeros((3,numt*nump));

rho = dyn.density('Ge')
C = dyn.elasticity('Ge')

i = 0

for tt in t:
    for pp in p:
        Vg,Vv = dyn.CalcGroupVelocityPol(tt,pp,C,rho);
	L[i] = Vg[0];
	FT[i] = Vg[1];
	ST[i] = Vg[2];
	pL[:,i] = Vv[:,0]; 
	pFT[:,i] = Vv[:,1]; 
	pST[:,i] = Vv[:,2]; 
        i+=1


#Save polarization data
np.savetxt('GeLVec.dat',pL.T);
np.savetxt('GeFTVec.dat',pFT.T);
np.savetxt('GeSTVec.dat',pST.T);
#Save group velocity data
np.savetxt('GeL.dat',L);
np.savetxt('GeFT.dat',FT);
np.savetxt('GeST.dat',ST);

