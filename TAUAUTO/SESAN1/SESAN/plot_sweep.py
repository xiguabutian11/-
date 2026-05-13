
import sys
import matplotlib.pyplot as plt
import numpy as np

plt.switch_backend('Agg')

data_file = sys.argv[1]
index = int(sys.argv[2])

freq, gain, outpower = np.loadtxt(data_file, unpack=True)

plt.figure(figsize=(8,4))
plt.plot(freq, gain, label='Gain')
plt.plot(freq, outpower, label='Output Power')
plt.title("Point {}".format(index))
plt.xlabel("Frequency")
plt.ylabel("Gain / Power")
plt.grid(True)
plt.legend()

plt.savefig("tiaobian_try/sweep_plot_{}.png".format(index), dpi=150, bbox_inches='tight')
plt.close()