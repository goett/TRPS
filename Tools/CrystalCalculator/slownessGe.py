import math
import numpy as np
import continuum as dyn

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib import cm

#plt.rcParams['image.cmap'] = 'viridis';

numt = 40;
nump = 40;

t = np.linspace(0,math.pi,numt)
p = np.linspace(0,math.pi*2.0,nump)

print(t)
print(p)

pm,tm = np.meshgrid(p,t);

#vectors holding results for phase velocity
L = np.zeros(numt*nump);
FT = np.zeros(numt*nump);
ST = np.zeros(numt*nump);

rho = dyn.density('Ge')
C = dyn.elasticity('Ge')

# placeholders for 

i = 0

for tt in t:
    for pp in p:
        Vp,pol = dyn.CalcPhaseVelocityPol(tt,pp,C,rho);
        print(Vp)
	L[i] = Vp[0];
	FT[i] = Vp[1];
	ST[i] = Vp[2];
        i+=1


#Save resulting data
np.savetxt('GeslowL.dat',L);
np.savetxt('GeslowFT.dat',FT);
np.savetxt('GeslowST.dat',ST);

Lm = L.reshape(numt,nump);
Lmc = Lm/np.amax(L);
FTm = FT.reshape(numt,nump);
FTmc = FTm/np.amax(FT);
STm = ST.reshape(numt,nump);
STmc = STm/np.amax(ST);

xl=np.sin(tm)*np.cos(pm)/Lm;
yl=np.sin(tm)*np.sin(pm)/Lm;
zl=np.cos(tm)/Lm;

xft=np.sin(tm)*np.cos(pm)/FTm;
yft=np.sin(tm)*np.sin(pm)/FTm;
zft=np.cos(tm)/FTm;

xst=np.sin(tm)*np.cos(pm)/STm;
yst=np.sin(tm)*np.sin(pm)/STm;
zst=np.cos(tm)/STm;

fig1 = plt.figure(figsize=(8.5,8.5));
#f,(ax, bx, cx) = plt.subplots(1,3);
ax = fig1.add_subplot(111,projection='3d');
#ax.plot_surface(xl,yl,zl,facecolors=Lmc,cmap='PuBuGn');
ax.plot_wireframe(xl,yl,zl,color='k');
cset = ax.contour(xl, yl, zl, zdir='z', offset=1.2*np.amin(zl), cmap=cm.coolwarm)
cset = ax.contour(xl, yl, zl, zdir='y', offset=1.2*np.amax(yl), cmap=cm.coolwarm)
cset = ax.contour(xl, yl, zl, zdir='x', offset=1.2*np.amin(xl), cmap=cm.coolwarm)
ax.set_xlim((1.2*np.amin(xl),1.2*np.amax(xl)));
ax.set_ylim((1.2*np.amin(yl),1.2*np.amax(yl)));
ax.set_zlim((1.2*np.amin(zl),1.2*np.amax(zl)));
fig2 = plt.figure(figsize=(8.5,8.5));
bx = fig2.add_subplot(111,projection='3d');
bx.plot_wireframe(xft,yft,zft,color='k');
cset = bx.contour(xft, yft, zft, zdir='z', offset=1.2*np.amin(zft), cmap=cm.coolwarm)
cset = bx.contour(xft, yft, zft, zdir='y', offset=1.2*np.amax(yft), cmap=cm.coolwarm)
cset = bx.contour(xft, yft, zft, zdir='x', offset=1.2*np.amin(xft), cmap=cm.coolwarm)
bx.set_xlim((1.2*np.amin(xft),1.2*np.amax(xft)));
bx.set_ylim((1.2*np.amin(yft),1.2*np.amax(yft)));
bx.set_zlim((1.2*np.amin(zft),1.2*np.amax(zft)));
fig3 = plt.figure(figsize=(8.5,8.5));
cx = fig3.add_subplot(111,projection='3d');
cx.plot_wireframe(xst,yst,zst,color='k');
cset = cx.contour(xst, yst, zst, zdir='z', offset=1.2*np.amin(zst), cmap=cm.coolwarm)
cset = cx.contour(xst, yst, zst, zdir='y', offset=1.2*np.amax(yst), cmap=cm.coolwarm)
cset = cx.contour(xst, yst, zst, zdir='x', offset=1.2*np.amin(xst), cmap=cm.coolwarm)
cx.set_xlim((1.2*np.amin(xst),1.2*np.amax(xst)));
cx.set_ylim((1.2*np.amin(yst),1.2*np.amax(yst)));
cx.set_zlim((1.2*np.amin(zst),1.2*np.amax(zst)));
#ax.set_axis_off()
plt.show()
