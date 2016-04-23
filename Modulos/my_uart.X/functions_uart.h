#include <stdio.h>         /* C standard IO */
#include <stdlib.h>         /* C standard IO */
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define INPUT_LENGTH 64        // Size of user input string.
bool getchar_active=false;
char entrada[INPUT_LENGTH];

// --- Hardware ---
#define led1    RB0

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
unsigned char getch(){
    getchar_active = true;
    /* retrieve one byte */
    while(getchar_active) /* set when register is not empty */
        continue;
    return RCREG;
}

unsigned char getche(void){
    unsigned char c;
    putch(c = getch());
    return c;
}
// Subscreve o putch chamdo pelo printf
void putch(unsigned char byte){
    while (!TXSTAbits.TRMT);
    TXREG = byte;
    if ('\n' == byte) {
        while (!TXSTAbits.TRMT);
        TXREG = '\r';
    }
    return;
}

//Função para pisca o numero de vezes que o numero teclado representa
void pulse(char number){
    char i;
    
    for(i=0;i<number;i++){
        led1=0x01;
        __delay_ms(500);
        led1=0x00;
        __delay_ms(500);
    }
}

// função personalizada que permite verificar se uma
// string contém apenas dígitos de 0 a 9
int is_numeric(const char *str){
 int res = 1; // passou no teste

  // vamos percorrer todos os caracteres da string
  // e testar se cada um é um dígito. Se não
  // for nós marcamos res como 0 e retornamos
  while(*str){
    if(!isdigit(*str)){
      res = 0;
      break;
    }
    // vamos passar para o próximo caractere
    str++;
  }

  return res;
}

//Manipula os dados recebidos do PC
void manipula_entrada(){
        if(is_numeric(entrada)){
            int number=atoi(entrada);
            pulse(number);
        }else printf("Alerta\n");
}


