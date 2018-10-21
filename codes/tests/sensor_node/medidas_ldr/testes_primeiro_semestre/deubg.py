import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
rc('mathtext', default='regular')

pi = np.pi

# fake data
time = np.linspace(0, 25, 50)
temp = 50 / np.sqrt(2 * pi * 3**2) \
    * np.exp(-((time - 13)**2 / (3**2))**2) + 15
Swdown = 400 / np.sqrt(2 * pi * 3**2) * np.exp(-((time - 13)**2 / (3**2))**2)
Rn = Swdown - 10

fig = plt.figure()
ax = fig.add_subplot(111)

ax.plot(time, Swdown, '-', label='Swdown')
ax.plot(time, Rn, '-', label='Rn')
ax2 = ax.twinx()
ax2.plot(time, temp, '-r', label='temp')

# ask matplotlib for the plotted objects and their labels
lines, labels = ax.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax2.legend(lines + lines2, labels + labels2, loc=0)

ax.grid()
ax.set_xlabel("Time (h)")
ax.set_ylabel(r"Radiation ($MJ\,m^{-2}\,d^{-1}$)")
ax2.set_ylabel(r"Temperature ($^\circ$C)")
ax2.set_ylim(0, 35)
ax.set_ylim(-20, 100)
plt.show()
