#include "header.h"
#include "functions_uart.h"

// Program parameters
void UART_write(unsigned char c);

//M�todo de interrup��es
void interrupt interrupcao(void){
    //RCIF:bit Flag de interrup��o de recebimento da USART
    //1 = O buffer de recebimento esta cheio.
    //0 = O buffer de recebimento esta vazio.
    if (RCIE && RCIF){
        getchar_active = false;//limpa flag
        RCREG;      // L� RCREG para limpar o RCIF
    }
} //end interrupt


// --- Fun��o Principal
void main(){
    
    UART_init();  // initialize the UART module
    ADCON0 = 0x00;
    ADCON1 = 0x06;
    CMCON = 0x07;
    TRISB = 0;                        //Entrada em RC0 como saida
    PORTB = 0;                        //RC0  iniciam em high e RA2, RA3 como saida
    GIE=0x01;// habilita interrup��o global
    PEIE=0x01;//habilita interrup��o por perifericos
    
    printf("*** Iniciando UART ***\n");
    while(1){                         //Loop Infinito       
        printf("Digite um numero:\n");
        cgets(entrada);
        printf("\nO numero digitado foi : ");
        printf("%s\n",entrada);
        manipula_entrada();
    } //end while
} //end main


//tempo=cicloDeMaquina*prescaler*contagem(256-TMR0)