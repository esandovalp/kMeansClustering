# Proyecto de K-means con OpenMP

Elaborado por: Emiliano Sandoval Peláez, 195557

### Definición del experimento:

Este experimento evalúa el rendimiento del algoritmo de clustering K-Means en los modos de ejecución serie y paralelo. El objetivo es cuantificar las ganancias de eficiencia de la paralelización del algoritmo utilizando OpenMP a través de varios hilos. El rendimiento se evalúa para conjuntos de datos que van de 100.000 a 1.000.000 de puntos para comprender el impacto del tamaño de los datos en la escalabilidad y el aumento de velocidad.

### Documentación del equipo experimental:

- Hardware: Los experimentos se realizaron en un MacBook Pro M3 equipado con 18 GB de RAM. Esta moderna configuración de hardware proporciona un contexto relevante para evaluar el rendimiento de las tareas de computación paralela.
- Software: La compilación y ejecución del algoritmo K-Means se gestionó utilizando Zed (un entorno de desarrollo similar a Visual Studio Code) y GCC versión 13 con soporte OpenMP habilitado mediante la bandera `fopenmp`. Esta configuración de software permite aprovechar al máximo las capacidades paralelas de OpenMP.

### Interpretación y análisis de los resultados:

- **Ejecución en serie frente a ejecución en paralelo**: En todos los tamaños de conjuntos de datos, la ejecución paralela con OpenMP muestra mejoras significativas de rendimiento con respecto a la ejecución en serie, especialmente a medida que aumenta el número de subprocesos. El aumento del rendimiento es especialmente pronunciado con 5, 11 y 22 subprocesos, lo que demuestra las ventajas del cálculo paralelo.
- **Impacto del número de subprocesos**: Los datos indican un rango de rendimiento óptimo entre 5 y 11 subprocesos, donde se observan los aumentos de velocidad más sustanciales. Sin embargo, también se vieron importantes aumentos de velocidad con un alto número de procesos, por ejemplo, un aumento de velocidad de 6,15 veces con 22 subprocesos para 100.000 puntos y de 7,27 veces con 11 subprocesos para 600.000 puntos ponen de manifiesto la eficacia del procesamiento paralelo para el análisis de datos a gran escala.
- **Escalabilidad con el tamaño de los datos**: Los conjuntos de datos más grandes se benefician sistemáticamente más de la paralelización, lo que pone de relieve la escalabilidad de la versión OpenMP del algoritmo. Esta tendencia es crucial para las aplicaciones de big data en las que la eficiencia del procesamiento es vital.
- **Gastos generales y rendimiento decreciente**: La ligera disminución de la velocidad a mayor número de hilos en algunos conjuntos de datos sugiere la presencia de rendimientos decrecientes debidos a la sobrecarga de la gestión de hilos. Esta observación subraya la importancia de ajustar el número de subprocesos al tamaño específico del conjunto de datos y a las capacidades del hardware para maximizar la eficiencia.

### Gráfica y tabla de resultados:

Las representaciones gráficas del aumento de velocidad conseguido con distintos números de subprocesos en función del tamaño de los conjuntos de datos resumirían visualmente las ventajas de la paralelización en términos de rendimiento. 

![Untitled](Proyecto%20de%20K-means%20con%20OpenMP%2086bff2474da54afca7d4367ffaa7418f/Untitled.png)

| Number of Points | Serial Time (ms) | 1 Thread (ms) | 5 Threads (ms) | 11 Threads (ms) | 22 Threads (ms) | Speedup (1 Thread) | Speedup (5 Threads) | Speedup (11 Threads) | Speedup (22 Threads) |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 100,000 | 190.6 | 244.9 | 42.3 | 32.8 | 31 | 0.78 | 4.51 | 5.81 | 6.15 |
| 200,000 | 406.5 | 443.7 | 171.5 | 152.7 | 66.3 | 0.92 | 2.37 | 2.66 | 6.13 |
| 300,000 | 552.7 | 445.6 | 126.6 | 101.6 | 95.1 | 1.24 | 4.37 | 5.44 | 5.81 |
| 400,000 | 867.5 | 683.3 | 132.5 | 128.8 | 189.2 | 1.27 | 6.55 | 6.74 | 4.59 |
| 600,000 | 1408.1 | 1270.1 | 249.7 | 193.7 | 284.4 | 1.11 | 5.64 | 7.27 | 4.95 |
| 800,000 | 1416.9 | 1732.1 | 291.4 | 453.9 | 213.5 | 0.82 | 4.86 | 3.12 | 6.64 |
| 1,000,000 | 1799.8 | 1985.8 | 382 | 375.5 | 276.9 | 0.91 | 4.71 | 4.79 | 6.50 |

### Archivo de datos:

Adjunto el link de mi repositorio de github donde están todos los archivos (exceptuando archivos .csv): [https://github.com/esandovalp/kMeansClustering](https://github.com/esandovalp/kMeansClustering)

### En busca de un aumento de velocidad, es posible escanear más o menos hilos.

Usando la clase [testThreads.py](http://testThreads.py) hice un dataFrame en el que guarde todos los resultados de tiempo para cada cantidad de threads utilizados {1,2,…,20), para encontrar el menor tiempo para cada cantidad de puntos, estos fueron los resultados:

| No. of points | Threads | Time (ms) | Best time w/threads tested | Speed up (1 threads/n threads) | Previous speed up w/best time |
| --- | --- | --- | --- | --- | --- |
| 100,000 | 20 | 15.1 | 31 (22 threads) | 16.21x | 6.15x |
| 200,000 | 12 | 48.6 | 66.3 (22 threads) | 8.36x | 6.13x |
| 300,000 | 17 | 61.4 | 95.1 (22 threads) | 9x | 5.81x |
| 400,000 | 13 | 74.4 | 128.8 (11 threads) | 11.65x | 6.74x |
| 600,000 | 13 | 118 | 193.7 (11 threads) | 11.93x | 7.27x |
| 800,000 | 21 | 170.9 | 213.5 (22 threads) | 8.29x | 6.64x |
| 1,000,000 | 18 | 194.7 | 276.9 (22 threads) | 9.24x | 6.50x |

![Untitled](Proyecto%20de%20K-means%20con%20OpenMP%2086bff2474da54afca7d4367ffaa7418f/Untitled%201.png)

### Conclusión:

El análisis adicional del aumento de velocidad conseguido mediante el número óptimo de hilos subraya aún más la relación matizada entre paralelismo y rendimiento. En concreto, la comparación entre el aumento general de velocidad con el uso de hilos óptimos y los mejores tiempos anteriores muestra un patrón claro: aunque un mayor número de hilos suele mejorar el rendimiento, hay un punto en el que éste disminuye o incluso se degrada. Esto puede atribuirse a los gastos generales asociados a la gestión de un mayor número de subprocesos, incluidos, entre otros, la creación de subprocesos y los costes de sincronización, así como la posibilidad de que se produzcan más pérdidas de caché.

Además, el examen detallado de los aumentos de velocidad en función del tamaño de los conjuntos de datos pone de manifiesto la escalabilidad del algoritmo K-Means paralelizado. Es evidente que, a medida que aumenta el tamaño del conjunto de datos, las ventajas del procesamiento paralelo son más pronunciadas, lo que demuestra la capacidad del algoritmo para aprovechar eficazmente los recursos informáticos adicionales. Sin embargo, la variación en las tasas de aceleración también sugiere que el número "óptimo" de subprocesos no es un valor único, sino que depende de las características específicas del conjunto de datos y del entorno informático.

Los resultados también refuerzan el concepto de que para lograr un rendimiento óptimo en computación paralela es necesario un equilibrio entre la carga de trabajo computacional y la sobrecarga introducida por el paralelismo. Este equilibrio es fundamental para maximizar la utilización de los recursos sin incurrir en sobrecargas contraproducentes. Por lo tanto, los desarrolladores e investigadores deben realizar evaluaciones empíricas del rendimiento, como la presentada, para identificar la configuración óptima para su contexto específico.

En conclusión, esta evaluación experimental no sólo demuestra la eficacia de la computación paralela para mejorar el rendimiento del algoritmo de agrupamiento K-Means, sino que también proporciona valiosos conocimientos sobre la dinámica de la ejecución paralela. Estos conocimientos son cruciales para el continuo desarrollo y optimización de algoritmos paralelos, allanando el camino para soluciones más eficientes y escalables en el campo del análisis de datos y más allá. A medida que el hardware de computación paralela sea cada vez más accesible, comprender y explotar estos principios será esencial para aprovechar todo el potencial de las infraestructuras informáticas modernas.