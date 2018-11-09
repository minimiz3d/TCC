class Consumo:
    def __init__(self, capacidade_bat=2348, t_off=30):
        self.capacidade_bat = capacidade_bat

        self.i_off = 0.0055
        self.t_off = t_off
        
        # PIOR CASO
        self.i_sensor = 6
        self.t_sensor = 0.055

        # PIOR CASO
        self.i_tx = 13
        self.t_tx = 0.45

    @staticmethod
    def get_consumo_mah(tempo, consumo):
        return (tempo*consumo)/3600

    def get_consumo_total(self):
        consumo_off = self.get_consumo_mah(self.t_off, self.i_off)
        consumo_sensor = self.get_consumo_mah(self.t_sensor, self.i_sensor)
        consumo_tx = self.get_consumo_mah(self.t_tx, self.i_tx)
        total = consumo_off + consumo_sensor + consumo_tx

        # print(
        #     "**** CONSUMOS [mAh] ****\nOFF: %s\nSENSOR: %s\nTX: %s\n> TOTAL: %s" % 
        #     (consumo_off, consumo_sensor, consumo_tx, total)
        # )

        return consumo_off + consumo_sensor + consumo_tx

    def _get_num_ciclos(self):
        num_ciclos = self.capacidade_bat / self.get_consumo_total() 
        # print("CICLOS: %s" % num_ciclos)
        return num_ciclos

    def _get_tempo_total(self):
        return self.t_off + self.t_sensor + self.t_tx

    def _get_num_horas(self):
        num_horas = self._get_num_ciclos()/(3600/self._get_tempo_total())
        # print("HORAS: %s" % num_horas)
        return num_horas

    def get_num_dias(self):
        return self._get_num_horas()/24        


if __name__ == '__main__':
    # consumo = Consumo(capacidade_bat=2348, t_off=10)
    
    # num_dias = consumo.get_num_dias()
    # print("DIAS: %s" % num_dias)  
    # print("MESES: %s" % (num_dias/30))
    # print("ANOS: %s" % ((num_dias/30)/12))

    import re
    resumo = """
    O crescimento do número de veículos em circulação no país gera diversos inconvenientes, como engarrafamentos e altos índices de poluição. Neste contexto, uma das principais causas de lentidão no trânsito é a busca às cegas por vagas de estacionamento, que se dá pela falta de informatização destes locais. Diante disso, o objetivo deste trabalho é elucidar as necessidades atreladas ao desenvolvimento de uma solução para este problema, e também propor uma aplicação baseada em uma rede de sensores sem fio de baixo custo. Para a realização deste projeto, sensores de luminosidade LDR foram escolhidos, por serem componentes baratos e capazes de medir uma das variáveis mais evidentes relacionadas ao estado de uma vaga de estacionamento ao ar livre. O protótipo da rede de sensores foi estruturado em uma topologia de comunicação estrela de estrelas, utilizando módulos de rádio AM nos nós sensores e um nó central LoRa. O projeto aborda também a autonomia dos nós e uma aplicação baseada no protocolo MQTT para exibição da disponibilidade das vagas monitoradas. Foi constatado que o sensor LDR é capaz de distinguir entre a presença e ausência de veículos em diferentes situações, tanto climáticas, quanto luminosas. Além disso, foi estimada uma autonomia dos nós sensores de aproximadamente 19 meses em operação. Entretanto, a comunicação entre os nós apresentou baixa efetividade. 
    """
    count = len(re.findall(r'\w+', resumo))
    print(count)

    tempos = [5, 10, 15, 20, 25, 30]
    for tempo in tempos:
        consumo_atual = Consumo(capacidade_bat=2850, t_off=tempo)
        print(consumo_atual.get_num_dias()/30)
    