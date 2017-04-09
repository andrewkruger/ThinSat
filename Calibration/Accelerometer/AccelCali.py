# Written in Python 2.7
# Used to calibrate an LSM303 accelerometer
# by fitting a 3-axis ellipsoid to accel data

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from scipy import optimize

# Get accelerometer data
x3, y3, z3 = np.genfromtxt('/Users/andrew/Desktop/AccelCaliData.txt', unpack=True)


plt.clf()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', axisbg = 'white')

ax.set_aspect('equal')
plt.xlabel('X')
plt.ylabel('Y')
ax.autoscale_view(tight=True)
#ax.plot_surface(xs, ys, zs, rstride=1, cstride=1, color='b', alpha=0.08, edgecolors='lightgrey', linewidth=0.5)
ax.scatter(x3, y3, z3, s=1, color='b')



 # 3-axis hard iron and gain calibration
def calc_params3(xc, yc, zc, xm, ym, zm):
    return (((x3-xc)/xm)**2 + ((y3-yc)/ym)**2 + ((z3-zc)/zm)**2)    
    
def lq_function3(c):
    fit = calc_params3(*c)
    return fit - 1

# parameter estimates
x3_cen = (x3.max()+x3.min())/2.
y3_cen = (y3.max()+y3.min())/2.
z3_cen = (z3.max()+z3.min())/2.
x3_mag = (x3.max()-x3.min())/2.
y3_mag = (y3.max()-y3.min())/2.
z3_mag = (z3.max()-z3.min())/2.

# fit data
params3_estimate = x3_cen, y3_cen, z3_cen, x3_mag, y3_mag, z3_mag
params3, ier = optimize.leastsq(lq_function3, params3_estimate)
xc_3, yc_3, zc_3, xm_3, ym_3, zm_3 = params3
print params3
# calibrate data
x3_hcal = (x3-xc_3)/xm_3
y3_hcal = (y3-yc_3)/ym_3
z3_hcal = (z3-zc_3)/zm_3

u = np.linspace(0, 2 * np.pi, 21)
v = np.linspace(0, np.pi, 15)
xs = np.outer(np.cos(u), np.sin(v))
ys = np.outer(np.sin(u), np.sin(v))
zs = np.outer(np.ones(np.size(u)), np.cos(v))


plt.clf()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', axisbg = 'white')

ax.set_aspect('equal')
ax.autoscale_view(tight=True)
ax.plot_surface(xs, ys, zs, rstride=1, cstride=1, color='b', alpha=0.08, edgecolors='lightgrey', linewidth=0.5)
ax.scatter(x3_hcal, y3_hcal, z3_hcal, s=1, color='b')
