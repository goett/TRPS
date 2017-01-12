from math import cos, sin, pi
import numpy as np
import numpy.linalg as la

def dircosines(theta,phi):
	""" 
	    Returns tensor of direction cosines for polar angle theta and azimuthal 
	    angle phi in radians.
	"""

	kx = np.sin(theta)*np.cos(phi);
	ky = np.sin(theta)*np.sin(phi);
	kz = np.cos(theta);

	J = np.array([[kx,0.0,0.0,0.0,kz,ky],
		     [0.0,ky,0.0,kz,0.0,kx],
                     [0.0,0.0,kz,ky,kx,0.0]]);

	return J


def VoightVec(k):
	""" 
	    Returns voight representation of a vector
	"""

	kx = k[0];
	ky = k[1];
	kz = k[2];

	K = np.array([[kx,0.0,0.0,0.0,kz,ky],
		     [0.0,ky,0.0,kz,0.0,kx],
                     [0.0,0.0,kz,ky,kx,0.0]]);

	return K


def VoightDot(k,e):
	"""
		Returns voigt contracted tensor product of two 3 vectors
	"""

	vd = np.array([ k[0]*e[0], k[1]*e[1], k[2]*e[2], k[2]*e[1]+k[1]*e[2], k[2]*e[0]+k[0]*e[2], k[1]*e[0]+k[0]*e[1]]);
	vd.reshape((6,1));

	return vd


def CalcPhaseVelocityPol(theta,phi,C,rho):
	""" Calculate phase velocity for a material with elasticity tensor
	    C and density rho in the direction of polar angle theta and
	    azimuthal angle phi in radians. 
	    Vp gives the phase velocities sorted in order L, FT, ST
	    Pol gives the polarization vectors as columns in order L, FT, ST
	"""

	# define tensor of direction cosines
	J = dircosines(theta,phi);
	
	# define christoffel matrix and calculate eigenvalues and eigenvectors
	CRIS = np.dot(np.dot(J,C),J.T);
        w,v = la.eig(CRIS);

	# array is eigenvalues is w**2/k**2
	# factor density and return km/s
	E = 10.0*(np.sqrt(w/rho));
	#print(theta,phi);
	#print('\t',E);

	# determine which eigenvalue corresponds to each mode
	# by convention the fastest mode is logitudinal and the slowest
	# is slow transverse
	Lindex = np.argmax(E);
	STindex = np.argmin(E);
	# check that STindex and Lindex are not degenerate
	if STindex==Lindex:
		print('The min and max phase velocity are degenerate');
		Lindex = np.argmax(E);
		STindex = np.argmin(E);
		print(w);
		print(v);
		print(E);
		print(Lindex);
		print(STindex);

	FTindex = np.setdiff1d([0,1,2],[STindex,Lindex]);

	pol = np.zeros((3,3));
	pol[:,0]=v[:,Lindex];
	pol[:,2]=v[:,STindex];
	pol[:,1]=v[:,FTindex[0]];
	Vp = np.array([E[Lindex],E[FTindex],E[STindex]]);
	#print('\t',Vp)	

	return Vp,pol


def CalcVelocityPol3(theta,phi,C,rho):
	""" Calculate phase velocity for a material with elasticity tensor
	    C and density rho in the direction of polar angle theta and
	    azimuthal angle phi in radians. 
	    Vp gives the phase velocities sorted in order L, FT, ST
	    Pol gives the polarization vectors as columns in order L, FT, ST
	"""

	# define tensor of direction cosines
	J = dircosines(theta,phi);
	
	# define christoffel matrix and calculate eigenvalues and eigenvectors
	CRIS = np.dot(np.dot(J,C),J.T)/rho;
        w,v = la.eig(CRIS);

	# array is eigenvalues is w**2/k**2
	# factor density and return km/s
	E = np.sqrt(w);

	# determine which eigenvalue corresponds to each mode
	# by convention the fastest mode is logitudinal and the slowest
	# is slow transverse
	Lindex = np.argmax(w);
	STindex = np.argmin(w);
	# check that STindex and Lindex are not degenerate
	if STindex==Lindex:
		print('The min and max phase velocity are degenerate');

	FTindex = np.setdiff1d([0,1,2],[STindex,Lindex]);

	pol = np.zeros((3,3));
	pol[:,0]=v[:,Lindex];
	pol[:,2]=v[:,STindex];
	pol[:,1]=v[:,FTindex[0]];
	Vp = np.array([E[Lindex],E[FTindex],E[STindex]]);

	return Vp,pol


def CalcGroupVelocityPol(theta,phi,C,rho):
	""" Calculate group velocity for a material with elasticity tensor
	    C and density rho in the direction of polar angle theta and
	    azimuthal angle phi in radians. 
	    Vg gives the group velocity magnitudes sorted in order L, FT, ST
	    Vv gives the velocity vectors as columns in order L, FT, ST
	"""

	# define tensor of direction cosines
	J = dircosines(theta,phi);
	K = np.array([sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta)*cos(phi)]);
	
	# define christoffel matrix and calculate eigenvalues and eigenvectors
	CRIS = np.dot(np.dot(J,C),J.T);
        w,v = la.eig(CRIS);

	# array is eigenvalues is w**2/k**2
	# factor density and return km/s
	E = 10.0*(np.sqrt(w/rho));

	# determine which eigenvalue corresponds to each mode
	# by convention the fastest mode is logitudinal and the slowest
	# is slow transverse
	Lindex = np.argmax(w);
	STindex = np.argmin(w);
	# check that STindex and Lindex are not degenerate
	if STindex==Lindex:
		print('The min and max phase velocity are degenerate');

	FTindex = np.setdiff1d([0,1,2],[STindex,Lindex]);

	pol = np.zeros((3,3));
	pol[:,0]=v[:,Lindex];
	pol[:,2]=v[:,STindex];
	pol[:,1]=v[:,FTindex[0]];
	Vp = np.array([E[Lindex],E[FTindex],E[STindex]]);
	#print('\t',Vp)	
	#print(Vp);
	#print(pol);

	#With phase velocities and polarizations determined, 
	#calculate group velocities as V = e*C*K*e/pw = e*C*n*e/Vp*p
 	#multiply by 1.0E5 to get in units of m/s
	Vv = np.zeros((3,3));
	Vg = np.zeros(3);
	#Longitudinal
	Sl = K/Vp[0]; 
	Sel = VoightDot(Sl,pol[:,0])/rho;
	eVl = VoightVec(pol[:,0]); 
	Vv[:,0] = np.dot(eVl,np.dot(C,Sel))*1.0E5;
	Vg[0] = np.linalg.norm(Vv[:,0]);
	
	#Fast Transverse
	Sft = K/Vp[1]; 
	Seft = VoightDot(Sft,pol[:,1])/rho;
	eVft = VoightVec(pol[:,1]); 
	Vv[:,1] = np.dot(eVft,np.dot(C,Seft))*1.0E5;
	Vg[1] = np.linalg.norm(Vv[:,1]);
	
	#Slow Transverse
	Sst = K/Vp[2]; 
	Sest = VoightDot(Sst,pol[:,2])/rho;
	eVst = VoightVec(pol[:,2]); 
	Vv[:,2] = np.dot(eVst,np.dot(C,Sest))*1.0E5;
	Vg[2] = np.linalg.norm(Vv[:,2]);

	return Vg,Vv


def density(xtal):
	""" 
	    Returns density of select materials in g/cc.
	"""
	if xtal=='TeO2':
		rho = 6.020;
	elif xtal =='Ge':
		rho = 5.3267;
	elif xtal =='ZnSe':
		rho = 5.27;

	return rho


def elasticity(xtal):
	""" 
	    Returns elasticity tensor of select materials in 10**12 dyn.
	"""
	if xtal=='Ge':
		C = np.array([[1.30, 0.49, 0.49, 0.0, 0.0, 0.0],
			[0.49, 1.30, 0.49, 0.0, 0.0, 0.0],
			[0.49, 0.49, 1.30, 0.0, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.67, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.67, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.0, 0.67]]);
	elif xtal=='TeO2':
		"""
		    Extrapolated from Ledbetter et. al Journal of Applied Physics 96, 6201 (2004)
		"""
		C = np.array([[0.559, 0.513, 0.217, 0.0, 0.0, 0.0],
			[0.513, 0.559, 0.217, 0.0, 0.0, 0.0],
			[0.217, 0.217, 1.056, 0.0, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.267, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.267, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.0, 0.662]]);
	elif xtal=='ZnSe':
		"""
		    Extrapolated from B.H. Lee (1970), Journal Appl. Physics v41 #7
		"""
		C = np.array([[0.888, 0.527, 0.527, 0.0, 0.0, 0.0],
			[0.527, 0.888, 0.527, 0.0, 0.0, 0.0],
			[0.527, 0.527, 0.888, 0.0, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.414, 0.0, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.414, 0.0],
			[0.0, 0.0, 0.0, 0.0, 0.0, 0.414]]);


	return C


 

 
