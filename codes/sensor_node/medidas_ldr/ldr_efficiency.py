import numpy as np
import matplotlib.pyplot as plt

N = 11
unavailable_data = (
    52, 51.25, 51, 
    53, 53.2, 54, 
    53.5, 52, 51, 
    49.5, 49
)
available_std = (2, 3, 4, 1, 2, 2, 3, 4, 1, 2, 3)

ind = np.arange(N)  # the x locations for the groups
width = 0.2       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind, unavailable_data, width, 
color='mediumorchid', 
# yerr=available_std
)

available_data = (65, 65, 65, 
65, 64, 64, 
65, 65, 63, 
61, 60
)
women_std = (3, 5, 2, 3, 3, 3, 5, 2, 3, 3, 4)
rects2 = ax.bar(ind + width, available_data, width, 
color='k', 
# yerr=women_std
)

# add some text for labels, title and axes ticks
ax.set_ylabel('Índice de luminosidade')
ax.set_xlabel("Tempo (h)")
ax.set_title('Avaliação das medições do LDR ao longo do tempo')
ax.set_xticks(ind + width/2)
ax.set_xticklabels(('8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18'))
# ax.set_ylim([45, 70])
# ax.grid(which='major', linestyle='-', linewidth='0.25', color='black')

ax.legend(
    (rects1[0], rects2[0]), 
    ('Vaga ocupada', 'Vaga disponível'),
    loc="upper left"
    # loc=9, 
    # bbox_to_anchor=(0.5, -0.1), 
    # ncol=2
)


def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')

# autolabel(rects1)
# autolabel(rects2)

percentages = [98, 100, 97, 96, 96,
               95, 95, 96, 94, 93, 93]
p_array = np.array(percentages)               
ax2 = ax.twinx()  # instantiate a second axes that shares the same x-axis

color = 'limegreen'
ax2.set_ylabel('Taxa de assertividade (%)', color=color)  # we already handled the x-label with ax1
ax2.plot(
    np.arange(start=width/2, stop=N, step=1), 
    p_array, 
    color=color, 
    linewidth=1,
    linestyle='-', 
    marker='o'
)
ax2.tick_params(axis='y', labelcolor=color)
ax2.grid(which='major', linestyle='-', linewidth='0.25', color='limegreen')
plt.yticks(np.arange(start=90, stop=101, step=1))

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()
