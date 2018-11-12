import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

def plot_lum_level():
    N = 11
    data_vagas_ocupadas = (
        52, 51.25, 51, 
        53, 53.2, 54, 
        53.5, 52, 51, 
        49.5, 49
    )
    desvio_vagas_ocupadas = (
        2, 3, 4, 
        1, 2, 2, 
        3, 4, 1, 
        2, 3
    )

    ind = np.arange(N)  # the x locations for the groups
    width = 0.25       # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(ind, data_vagas_ocupadas, width, 
    color='mediumorchid', 
    # yerr=desvio_vagas_ocupadas
    )

    data_vagas_livres = (
        65, 65, 65, 
        65, 64, 64, 
        65, 65, 63, 
        61, 60
    )
    desvio_vagas_livres = (
        3, 5, 2, 
        3, 3, 3, 
        5, 2, 3, 
        3, 4
    )
    rects2 = ax.bar(ind + width, data_vagas_livres, width, 
    color='black', 
    # yerr=desvio_vagas_livres
    )

    # add some text for labels, title and axes ticks
    ax.set_ylabel('Índice de luminosidade')
    ax.set_xlabel("Tempo (h)")
    # ax.set_title('Avaliação das medições do LDR ao longo do tempo')
    ax.set_xticks(ind + width/2)
    ax.set_xticklabels((
        '8', '9', '10', 
        '11', '12', '13', 
        '14', '15', '16', 
        '17', '18'
    ))
    ax.set_ylim([0, 70])
    # ax.grid(which='major', linestyle='-', linewidth='0.25', color='black')


    ax.legend(
        (rects1[0], rects2[0]), 
        ('Vaga ocupada', 'Vaga disponível'),
        loc='best'    
    )

    plt.show()


def plot_ldr_eff():
    percentages = [
        98, 97, 97, 
        96, 96, 95, 
        95, 96, 94, 
        93, 93
    ]
    mal_estacionado = [
        70, 71, 67, 
        67, 72, 73, 
        69, 70, 70, 
        68, 67
    ]
    p_array = np.array(percentages)               
    fig, ax2 = plt.subplots()
    ax2.set_xlabel("Tempo (h)")

    N = 11
    width = 0.25
    color = 'limegreen'
    ax2.set_ylabel('Taxa de assertividade (%)', color='black')  # we already handled the x-label with ax1
    ax2.plot(
        np.arange(start=0, stop=N, step=1), 
        p_array, 
        color=color, 
        linewidth=1,
        linestyle='-', 
        marker='o',
        label="Bem estacionado"
    )
    ax2.plot(
        np.arange(start=0, stop=N, step=1), 
        np.array(mal_estacionado), 
        color='red', 
        linewidth=1,
        linestyle='-', 
        marker='o',
        label="Mal estacionado"
    )
    ax2.tick_params(axis='y', labelcolor='black')
    ax2.grid(which='major', linestyle='-', linewidth='0.25', color='gray')
    ax2.legend(loc='best')
    ax2.set_xticks(np.arange(N))
    ax2.set_xticklabels(('8', '9', '10', 
        '11', '12', '13', 
        '14', '15', '16', 
        '17', '18'))
    plt.yticks(np.arange(start=65, stop=101, step=5))

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.show()


if __name__ == "__main__":
    plot_ldr_eff()
    # plot_lum_level()