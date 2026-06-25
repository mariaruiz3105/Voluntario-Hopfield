# Voluntario modelo de Hopfield de red neuronal


Este repositorio contiene el código fuente, los datos de salida y las animaciones correspondientes. Está organizado en tres carpetas correspondientes a cada apartado de resultados del informe: un_patron, tres_patrones y muchos_patrones. Fuera de esas carpetas se encuentra animacion.py, usado para animar la evolución del sistema para varias temperaturas, y los 3 archivos que empiezan por im1, im2, im3 son los patrones utilizados.

En las dos primeras carpetas un_patron y tres_patrones están los códigos correspondientes en C++ del modelo de Hopfield, con el cual se obtuvieron los datos s_historial_T_[temperatura].txt y m_T_[temperatura].txt para hacer la animación de la evolución del sistema y para realizar la gráfica del solapamiento frente a los pasos Monte Carlo. El .cpp que empieza por guardar_m_T es una variación del anterior para obtener el solapamiento a muchas temperaturas y no tener que guardar la matriz s en cada paso, los datos obtenidos están en m_vs_T.txt. Los datos obtenidos y animaciones se encuentran en las carpetas aleatorio y con_deformacion dependiendo de cuál era el patrón inicial. Para obtener las gráficas del informe del solapamiento frente a los pMc o frente a la temperatura T basta con ejecutar los archivos .py.

En la carpeta muchos_patrones se encuentra el código en C++ utilizado junto a los datos obtenidos y al código en python para realizar la gráfica.
