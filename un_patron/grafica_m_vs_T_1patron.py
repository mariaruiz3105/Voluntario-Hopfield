import numpy as np
import matplotlib.pyplot as plt

archivo_def = 'un_patron/con_deformacion/m_vs_T.txt'
archivo_sin_def = 'un_patron/aleatorio/m_vs_T.txt'


data_con = np.loadtxt(archivo_def)
data_sin = np.loadtxt(archivo_sin_def)
T_con = data_con[:, 0]
m_con = data_con[:, 1]
T_sin = data_sin[:, 0]
m_sin = data_sin[:, 1]

plt.figure(figsize=(8, 6))

plt.plot(T_con, m_con, marker='o', linestyle='-', lw=1, color='blue', markersize=4, alpha=0.8, label='Con deformación inicial del 30%')
plt.plot(T_sin, m_sin, marker='s', linestyle='-', lw=1, color='red', markersize=4, alpha=0.8, label='Patrón inicial aleatorio')

plt.xlabel('T')
plt.ylabel('|m|')
plt.grid(True, linestyle='dotted', alpha=0.7)
plt.legend()
plt.show()
