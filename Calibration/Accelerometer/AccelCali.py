# Written in Python 2.7
# Used to calibrate an LSM303 accelerometer
# by fitting a 3-axis ellipsoid to accel data

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from scipy import optimize

# Get accelerometer data
x, y, z = np.genfromtxt('https://raw.githubusercontent.com/andrewkruger/ThinSat/master/Calibration/Accelerometer/AccelCaliData.txt', unpack=True)

# Plot uncalibrated data
plt.clf()
fig = plt.figure(1)
ax = fig.add_subplot(111, projection='3d', axisbg = 'white')
ax.set_aspect('equal')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Uncalibrated Data')
ax.autoscale_view(tight=True)
ax.scatter(x, y, z, s=1, color='b')
plt.show()


# Create least square functions for fit
def calc_params(xcen, ycen, zcen, xmag, ymag, zmag):
    return (((x-xcen)/xmag)**2 + ((y-ycen)/ymag)**2 + ((z-zcen)/zmag)**2)    
    
def lq_function(c):
    fit = calc_params(*c)
    return fit - 1

# parameter estimates
xc_est = (x.max()+x.min())/2.
yc_est = (y.max()+y.min())/2.
zc_est = (z.max()+z.min())/2.
xm_est = (x.max()-x.min())/2.
ym_est = (y.max()-y.min())/2.
zm_est = (z.max()-z.min())/2.

# fit data
params_estimate = xc_est, yc_est, zc_est, xm_est, ym_est, zm_est
params, ier = optimize.leastsq(lq_function, params_estimate)
xc, yc, zc, xm, ym, zm = params
print 'Centroids:',params[0:3]
print 'Magnitudes:',params[3:]

# calibrate data
xcal = (x-xc)/xm
ycal = (y-yc)/ym
zcal = (z-zc)/zm

# create unit sphere to view calibration
u = np.linspace(0, 2 * np.pi, 21)
v = np.linspace(0, np.pi, 15)
xs = np.outer(np.cos(u), np.sin(v))
ys = np.outer(np.sin(u), np.sin(v))
zs = np.outer(np.ones(np.size(u)), np.cos(v))

# plot calibrated data and unit sphere
fig = plt.figure(2)
ax = fig.add_subplot(111, projection='3d', axisbg = 'white')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Calibrated Data')
ax.set_aspect('equal')
ax.autoscale_view(tight=True)
ax.plot_surface(xs, ys, zs, rstride=1, cstride=1, color='b', alpha=0.08, edgecolors='lightgrey', linewidth=0.5)
ax.scatter(xcal, ycal, zcal, s=1, color='b')
