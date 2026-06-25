import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('muchos_patrones/almacenados_vs_recordados.txt')
P_alma = data[:, 0]
P_rec = data[:, 1]

plt.figure(figsize=(8, 6))
plt.plot(P_alma, P_rec, marker='o', linestyle='-', lw=1, color='crimson', markersize=3)

plt.xlabel('Patrones almacenados')
plt.ylabel('Patrones recordados')
plt.grid(True, linestyle='dotted', alpha=0.7)
plt.show()