# Tarea 0 2023-1
# Simulación de Elecciones - Documentación del Proyecto

¡Hola! En este proyecto, he programado una solución para simular elecciones bajo diferentes sistemas de votación y gestionar diversos eventos relacionados con el procesamiento de votos y el cálculo de resultados. A continuación, te proporciono una descripción detallada de cómo funciona la solución que he implementado.

## Funcionamiento

El proyecto se divide en tres partes fundamentales, cada una con objetivos específicos y funcionalidades únicas que interactúan entre sí:

### Procesamiento de Votos

En esta etapa, se registran los votos de los votantes hacia candidatos específicos. Cada voto se almacena adecuadamente para su posterior conteo y análisis. Los votos pueden ser registrados de manera individual, y se calculan conteos parciales y totales de votos por cada candidato. Esta información es crucial para determinar los resultados finales de la elección.

### Resultados por Mayoría Absoluta

Una vez que todos los votos han sido registrados, la solución permite identificar a los ganadores de la elección basándose en la mayoría absoluta de votos recibidos. Se pueden visualizar los resultados dentro de rangos específicos de votos y ordenar los candidatos por la cantidad de votos obtenidos. Esto proporciona una visión clara de los candidatos más respaldados por los votantes.

### Resultados por STV (Single Transferable Vote)

Esta sección implementa un sistema de votación más avanzado. Se eliminan candidatos con menos votos y se transfieren los votos de estos candidatos hacia otros basándose en la cercanía ideológica en el espectro político. Además, se pueden seleccionar candidatos con exceso de votos y redistribuir los votos sobrantes a candidatos cercanos. Este sistema permite una mayor representación y participación de diversos candidatos en el proceso electoral.

## Ejecución

Para ejecutar esta solución:

1. Clona o descarga este repositorio en tu máquina local.
2. Navega a la carpeta del proyecto.
3. Compila el código fuente utilizando el compilador adecuado.
4. Ejecuta el programa resultante.
5. Sigue las instrucciones proporcionadas en pantalla para interactuar con la simulación de elecciones y explorar los diferentes sistemas de votación.

Compilar ```make```

Ejecutar ```./dccars input output```

En caso de querer utilizar utilizar ```valgrind```, ejecutar ```valgrind ./dccars input output```

## Alcance y Objetivos

Durante el desarrollo de este proyecto, logré los siguientes objetivos:

- Programé la solución para procesar votos y calcular conteos parciales y totales de votos por candidato.
- Implementé algoritmos para determinar los ganadores de la elección basados en la mayoría absoluta de votos.
- Creé un sistema de votación STV2 que involucra la eliminación de candidatos y la transferencia de votos basados en la cercanía en el espectro ideológico.
- Desarrollé habilidades en programación, estructuras de datos y algoritmos.
- Practiqué el uso de punteros y el manejo de memoria en el contexto de la programación.
- Gané experiencia en la implementación de algoritmos de ordenamiento y técnicas de manipulación de datos.

A través de este proyecto, he obtenido una comprensión más profunda de los sistemas de votación, la importancia de los algoritmos de conteo y las implicaciones de diferentes métodos en los resultados de una elección. ¡Ha sido un proceso enriquecedor y educativo!