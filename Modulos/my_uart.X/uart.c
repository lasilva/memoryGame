#include "header.h"
#include "functions_uart.h"

//Método de interrupções
void interrupt interrupcao(void){
    //RCIF:bit Flag de interrupção de recebimento da USART
    //1 = O buffer de recebimento esta cheio.
    //0 = O buffer de recebimento esta vazio.
    if (RCIF){
        getchar_active = false;//limpa flag
        RCREG;      // Lê RCREG para limpar o RCIF
    }
} //end interrupt


// --- Função Principal
void main(){
    TRISC0 = 0;                        //Entrada em RC0 como saida
    PORTC = 0;
    UART_init();  // initialize the UART module
    ADCON0 = 0x00;
    ADCON1 = 0x06;
    CMCON = 0x07;
    GIE=0x01;// habilita interrupção global
    PEIE=0x01;//habilita interrupção por perifericos
    
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