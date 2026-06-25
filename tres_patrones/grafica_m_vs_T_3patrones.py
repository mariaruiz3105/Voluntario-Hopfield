import matplotlib.pyplot as plt
import numpy as np

datos = np.loadtxt("varios_patrones/con_deformacion/m_vs_T.txt")
T = datos[:, 0]
m1 = datos[:, 1]
m2 = datos[:, 2]
m3 = datos[:, 3]

plt.figure(figsize=(8, 6))

plt.plot(T, m1, color='blue', marker='.', label='Patrón 1')
plt.plot(T, m2, color='red', marker='.', label='Patrón 2')
plt.plot(T, m3, color='green', marker='.', label='Patrón 3')

plt.xlabel('T')
plt.ylabel('|m|')
plt.grid(True, linestyle='dotted', alpha=0.7)
plt.legend()
plt.show()