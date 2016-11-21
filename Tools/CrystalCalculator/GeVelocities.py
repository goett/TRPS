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

#vectors holding results for phase velocity magnitudes
L = np.zeros(numt*nump);
FT = np.zeros(numt*nump);
ST = np.zeros(numt*nump);
#vectors holding results for phase velocity magnitudes
LVec = np.zeros((numt*nump,3));
FTVec = np.zeros((numt*nump,3));
STVec = np.zeros((numt*nump,3));

rho = dyn.density('Ge')
C = dyn.elasticity('Ge')

# placeholders for 

i = 0

for tt in t:
    for pp in p:
        Vp,pol = dyn.CalcPhaseVelocityPol(tt,pp,C,rho);
	L[i] = Vp[0];
	FT[i] = Vp[1];
	ST[i] = Vp[2];
	LVec[i,:] = pol[:,0];
	FTVec[i,:] = pol[:,1];
	STVec[i,:] = pol[:,2];
        i+=1


#Save resulting data
np.savetxt('GeL.dat',L);
np.savetxt('GeFT.dat',FT);
np.savetxt('GeST.dat',ST);
np.savetxt('GeLVec.dat',LVec);
np.savetxt('GeFTVec.dat',FTVec);
np.savetxt('GeSTVec.dat',STVec);

