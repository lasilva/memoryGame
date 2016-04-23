#include <stdio.h>         /* C standard IO */
#include <string.h>
#include <stdbool.h>


void UART_init(){
    BRGH = 1; //seleção de baud rate assincrono high
    SYNC = 0; //modo assincrono
    TX9  = 0; //8-bit de transmissão
    CREN = 1; //habilita recepção continua 
    SPBRG = 129;//9600 baud @ 20MHz com BRGH = 1
    RCIE  = 1; //Habilita Interrupção de recebimento da USART
    SPEN = 1; // habilita porta serial
    TXEN = 1; // habilita transmissão de dados
}
/*
A função espera até que o  USART Receive Data Register(RCREG) esteja cheio e lê dados de 8 bits. 
*/
unsigned char getch(bool getchar_active){
    /* retrieve one byte */
    while(getchar_active) /* set when register is not empty */
        continue;
    return RCREG;
}
/*
char UART_Read()
{
  while(!RCIF);
  return RCREG;
} 
*/

/*
 
*/
unsigned char getche(void){
    unsigned char c;
    putch(c = getch());
    return c;
}


// Subscreve o putch chamdo pelo printf
void putch(unsigned char byte)
{
    while (!TXSTAbits.TRMT);
    TXREG = byte;
    if ('\n' == byte) {
        while (!TXSTAbits.TRMT);
        TXREG = '\r';
    }
    return;
}

//Manipula os dados recebidos do PC
void manipula_entrada(){
        if(strcmp (entrada, "y") == 0)RB0 = 0x01; 
        else if(strcmp (entrada, "t") == 0)RB0 = 0x00; 
        else if(strcmp (entrada, "r") == 0)RB1 = 0x01;
        else if(strcmp (entrada, "e") == 0)RB1 = 0x00;
        else if(strcmp (entrada, "g") == 0)RB2 = 0x01;
        else if(strcmp (entrada, "f") == 0)RB2 = 0x00;
        else printf("Alerta\n");
}

