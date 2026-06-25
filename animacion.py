import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

#temperaturas = np.linspace(1.0, 5.0, 10)
temperaturas = [0.0001, 0.002, 0.004, 0.006, 0.008, 0.01, 0.012, 0.014, 0.016, 0.018, 0.02, 0.022, 0.024, 0.026]
tiempo_inicio = time.time()
N=128

for T in temperaturas:
    print(f"Generando GIF para T={T:.4f}")
    nombre_archivo = f'varios_patrones/con_deformacion/s_historial_T_{T:.6f}.txt'
    
    archivo = np.loadtxt(nombre_archivo)
    num_frames = archivo.shape[0] // N
    datos = archivo.reshape((num_frames, N, N))
    datos = np.flip(datos, axis=1)
    

    fig, ax = plt.subplots(figsize=(8, 8))
    img = ax.imshow(datos[0], cmap='gray', vmin=0, vmax=1, origin='lower')
    ax.set_title(f"T = {T:.4f}")

    def animate(i):
        img.set_data(datos[i]) 
        return [img]

    ani=animation.FuncAnimation(fig, animate, frames=num_frames, interval=50, blit=True, repeat=False)
    
    ani.save(f'varios_patrones/con_deformacion/GIFS30/varios_T{T:.4f}.gif', writer='pillow')
    plt.close(fig) 


tiempo_fin= time.time()
print(f"\nGifs terminados en {(tiempo_fin-tiempo_inicio)/60:.2f} minutos")
