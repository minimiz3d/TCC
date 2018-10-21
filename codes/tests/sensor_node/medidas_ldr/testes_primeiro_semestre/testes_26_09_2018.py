# import pandas as pd
# import matplotlib.pyplot as plt

# B = 450
# C = 500
# D = 550
# F = 650
# occupied_morning = (0.8 * 50 * C + 0.2 * 50 * D)/50
# occupied_afternoon = (0.5 * 50 * B + 0.4 * 50 * D + 0.1 * 50 * C)/50

# if __name__ == '__main__':
#     df = pd.DataFrame(
#         [
#             ['Livre','Manhã',F],
#             ['Livre','Tarde',F],
#             ['Ocupada','Manhã',occupied_morning],
#             ['Ocupada','Tarde',occupied_afternoon],], 
#         columns=['Estado','Turno','Threshold']
#     )

#     df.pivot("Turno", "Estado", "Threshold").plot()

#     print(occupied_morning)
#     print(occupied_afternoon)

#     plt.show()

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

bar_width = .6
time = np.arange(start=8, stop=18, step=1)

# Dados
data = pd.DataFrame({
 'Vaga ocupada' : [60,70,65,70,70,60,50,45,45,45,50],
 'Vaga disponível' : [210,100,100,70,70,75,70,60,65,60,10],
 'Taxa de acerto' : [90,40,30,30,30,25,25,20,15,10,12],
})

# data[['Vaga ocupada','Vaga disponível']].plot(kind='bar', bar_width=bar_width)
data['Taxa de acerto'].plot(secondary_y=True, color='green', legend='Taxa de acerto')

# ax = plt.gca()
# plt.xlim([-bar_width, len(data['Vaga ocupada'])-bar_width])
# ax.set_xticklabels(time)
# ax.set_xlabel("Tempo (h)")
# ax.set_ylabel("Índice de luminosidade")

# plt.show()