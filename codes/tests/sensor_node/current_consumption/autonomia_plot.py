import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    tempos = [5, 10, 15, 20, 25, 30]
    autonomias = [
        2.8920526245133575,
        5.494462264991534,
        8.074016411621201,
        10.631014838367781,
        13.165752099547111,
        15.678517642938447
    ]
    
    fig, ax = plt.subplots()
    # ax.set_title("Autonomia de um nó sensor x tempo em modo low power")
    ax.set_xlabel("Tempo em modo de baixo consumo (s)")
    ax.set_ylabel("Autonomia (meses)")
    ax.plot(
        tempos, 
        autonomias,
        marker='o',
        label="Capacidade da bateria = 2348mAh"
    )
    debug = [
        3.510370519532823,
        6.669172681101309,
        9.800232867598137,
        12.90391494435612,
        15.980576441102757,
        19.030568689256633,
    ]
    ax.plot(
        tempos, 
        debug, 
        marker='o',
        label="Capacidade da bateria = 2850mAh"
    )
    ax.legend(loc='upper left')
    ax.grid()
    plt.show()