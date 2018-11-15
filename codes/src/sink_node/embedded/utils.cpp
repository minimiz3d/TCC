#include "utils.h"

String uart_buffer;

// #define MAX_NULL_RECEIVED 5

void float_to_string(double number, char *res, int afterpoint) {
  // parte inteira em aux
  int32_t aux = (int32_t) number;
  // parte fracionÃ¡ria em aux2
  float aux2 = number - (float) aux;

  // salva a parte inteira do nÃºmero na string e jÃ¡ coloca o ponto
  sprintf(res, "%d.", aux);

  uint8_t loop;
  for (loop = 0; loop < afterpoint; loop++)
  {
    aux2 = aux2 * 10;
    printf("%f\n", aux2 );
    aux = (int32_t) aux2;
    char strnumber[2];
    sprintf(strnumber, "%d", abs(aux) );
    strcat(res, strnumber);

    aux2 = aux2 - aux;
  }
}

