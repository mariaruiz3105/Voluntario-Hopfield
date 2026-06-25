import numpy as np
import matplotlib.pyplot as plt


temperaturas = [0.0001, 0.004, 0.006, 0.02,0.026]


for T in temperaturas:
    nombre_archivo = f'varios_patrones/con_deformacion/m_T_{T:.6f}.txt'

    data = np.loadtxt(nombre_archivo)
    pasos = data[:, 0]
    m1 = data[:, 1]
    m2 = data[:, 2]
    m3 = data[:, 3]

    plt.figure(figsize=(8, 6))

    plt.plot(pasos, m1, label='Patrón 1', color='blue')
    plt.plot(pasos, m2, label='Patrón 2', color='red')
    plt.plot(pasos, m3, label='Patrón 3', color='green')
  
    plt.xlabel('Pasos Monte Carlo')
    plt.ylabel('Solapamiento (m)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(loc='upper right') 
    plt.ylim(-1.05, 1.1) 
    
    plt.show()

