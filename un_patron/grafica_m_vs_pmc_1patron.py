import numpy as np
import matplotlib.pyplot as plt

#temperaturas = [0.0001, 0.002, 0.004, 0.006, 0.008, 0.01, 0.012, 0.014, 0.016, 0.018, 0.02, 0.022, 0.024, 0.026]

temperaturas = [0.0001, 0.006, 0.016, 0.02, 0.026]



plt.figure(figsize=(10, 6))

for T in temperaturas:
    nombre_archivo = f'un_patron/con_deformacion/m_T_{T:.6f}.txt'

    data = np.loadtxt(nombre_archivo)
    pasos = data[:, 0]
    m_valores = data[:, 1]
        
    plt.plot(pasos, m_valores, label=f'T = {T:.4f}')
  

plt.xlabel('Pasos Monte Carlo')
plt.ylabel('Solapamiento (m)')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(loc='lower right')
plt.ylim(-1.1, 1.1)
plt.show()