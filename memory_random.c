/* 
 * File:   comparador.c
 * Author: aluno
 * Created on 15 de Dezembro de 2015, 18:19
 */

#include <xc.h>
#include <stdio.h>
#include <time.h>
#include "config.h"

#define DISPLAY PORTD

#define MEMORIA RC5
#define NIVEL RC4

#define BUTTON_O1 RB0

bit flag = 0;//flag que controla o acender ou apagar do led;
bit flagLED = 0;//flag que le o valor do registrador do led 02 e tem o valor mudado para colocar no pino de saída rb2
bit flagAD = 0;
bit flagWR = 0;
bit display_flag;

unsigned char const digito[] = {0xBF, // 0 Define cada segmento
                        0x86, // 1 dos valores mostrados
                        0xDB, // 2 no display de LEDs
                        0xCF, // 3 em binário para ficar
                        0xE6, // 4 mais fácil de codificar
                        0xED, // 5 cada dígito dos valores
                        0xFD, // 6 mostrados no display.
                        0x87, // 7
                        0xFF, // 8
                        0xE7, // 9
                        0xF7, //A
                        0xFC, //b
                        0xB9,//C
                        0xDE, //d
                        0xF9, //E
                        0xF1, //F
                        };

unsigned char cnt_display = 0;
unsigned char cnt_teclado = 0;



unsigned char ad_data = 0;
unsigned char a = 'X';



/**
 * Em meu código só terá uma função void interrupt. Nesta função nós vamos tratar todas as 
 * interrupções. Observar o diagrama de blocos para relembrar como é o mecanismo de uma interrupção 
 * na arquitetura do PIC. 
 * 
 */
void interrupt tratar(void){
    //verifica se a porta int que compartilha a porta rb0 possui um valor em borda alta, logo a flag foi habilitada
    if(INTF)
    {
        __delay_ms(100);
        if(BUTTON_O1)
        {
            flag = ~flag;
        }
        INTF = 0;
    }
    
    if(TMR0IF)
    {
       //teclado
        if(cnt_display < 2)
        {
            NIVEL = cnt_display;
            MEMORIA = ~cnt_display;
            display_flag = cnt_display;
            cnt_display++;
        }else 
        {
            cnt_display = 0;
        }
        
        TMR0IF = 0;
    }
    
    if(ADIF)
    {
        ad_data = ADRESH;
        
        
        //pega o valor da conversão do A
        CCPR1L = ad_data;
        CCP1X = ADRESL >> 7 ;// pega o mais sgnificativo do adresl
        CCP1Y = (ADRESL & 0b0100000) >> 6; // pega o segundo mais significativo do adresl


        flagAD = 1;
        GO_DONE = 1;
        ADIF = 0;
    }

    if(CMIF)
    {
        if(C1OUT) flag = 1;
        else flag = 0;
      
        CMIF = 0;
    }

    if(RCIF)
    {
       flagWR = 1;
        a = RCREG;
       RCIF = 0;
    }
    

}

int main(int argc, char** argv) {


    TRISB = 0xFF; // usa o tris(number port) para especificar que a porta é ou não saida. [1 - entrada, 0 - saída]
    // Você pode definir qual pino da porta escolhida você deseja definir como entrada ou saída.
   /*
    TRISB0 = 1; //BUTTON 01 (1 - INPUT 0 - OUTPUT)
    TRISB1 = 0; // LED 01
    TRISB2 = 0; // LED 02
    TRISB3 = 0; // LED 03*/

    //Define as saídas D como as portas dos displays de 7 segmentos.
    TRISD = 0X00;// todas são saída
   
    TRISA = 0XFF; // TODOS SÃO SAÍDAS
    TRISA0 = 1; // entrada analogica A0
    TRISA1 = 1; // entrada analogica A1
    TRISA3 = 1;// ENTRADA analogica A3
    TRISA4 = 0;// SAÍDA DO COMPARADOR ANALÓGICO

    TRISC2 = 0; // SAÍDA DE CCP1 - QUE É O CANAL 1 DO PWM

    /*****TESTE DE INICIALIZAÇÃO*/

    T0CS = 0; // seleciona o clock interno, pois TOCS = 0;
    T0SE = 0; // borda de subida
    PSA = 0;// seleciona o prescaler pra o timer0
    PS2 = 1;// Prescaler 101 = 1:64ca
    PS1 = 0;// Prequencia de estouro 304Hz
    PS0 = 1; // Periodo de estouro 3,28 ms


    /**********CONFIG DO AD CONVERSOR************/

    ADCS2 = 0; // CLOCK EM FOSC/32
    ADCS1 = 1;
    ADCS0 = 0;

    CHS2 = 0; // USAR O CANAL DE CONVERSAO 0 - 000 / 1 - 001
    CHS1 = 0;
    CHS0 = 1; // bit menos significativo

    ADFM = 0; // 0 - JUSTIFICATIVA A ESQUEDA - POIS ELE QUER PEGAR OS BITS MAIS SIGNIFICATIVOS NO REG ADRESH COM OS MAIS SIGNIFICATIVOS
               //1 - JUSTIFICATIVA A DIREITA - POIS EU QUERO PEGAR OS 8 BITS - POIS QUERO FAZER O INTERVALO DE 50 À 250

    //AN0, AN1, AN3 - SÃO ENTRADAS ANALOGICAS
    PCFG3 = 0;// APENAS AN0 E ANALOGICO, AN1
    PCFG2 = 1;// VREF+ = VSS
    PCFG1 = 0;//VREF- = VDD
    PCFG0 = 0;


   /***************CONFIG COMPARADOR ANALOGICO***********/

    CM2 = 0;
    CM1 = 0;
    CM0 = 1;

    CIS = 0;

    C1INV = 1;// 0 - NÃO INVERTE O C1OUT - 1 - inverte c1out
    C2INV = 0; // 0 - NÃO INVERTE O C2OUT.

       /************CONFIG O TIMER 2 ********************/

    TOUTPS3 = 0; // CONFIGUREI DIZENDO QUE O POSTSCALER É 1:1 - OU SEJA - NÃO TEM DIVISAO DE FREQ DEPOIS
    TOUTPS2 = 0;
    TOUTPS1 = 0;
    TOUTPS0 = 0;

    T2CKPS1 = 0;
    T2CKPS0 = 0;
    
    /************CONFIG O PWM ********************/

    CCPR1L = 0;// REGISTRADOR DE DUTY CICLE - OS 8 PRIMEIROS BITS MAIS SIGNIFICATIVOS
    CCP1X = 0; // DOIS BITS MENOS SIGNIFICATIVOS DO DUTY CICLE
    CCP1Y = 0;


    CCP1M3 = 1; // CANAL 1 DE CCP É O PWM
    CCP1M2 = 1;
    CCP1M1 = 1;
    CCP1M0 = 1;

    PR2 = 255;

    /**OUTRAS INTERRUPÇÕES */

    INTEDG = 1; // habilita a interrupção na borda de subida
    INTF = 0; // inicializa com 0 para que quando a interrupção acontecer a flag ser 1. Deste modo o tratamento de interrupção vai habilitar o vetor de interrupção na posição 4h como 1
    INTE = 1; // habilita a interrupção externa

    ADIF = 0; // SET A FLAG COM ZERO
    ADIE = 1; // FALA QUE A INTERRUPÇÃO DE CONVERSAO AD ESTA HABILITADA
    CMIF = 0;// INICIALIZA A FLAG DO COMPARADOR ANALOGICO
    CMIE = 1; // INICIALIZA A INTERRUPÇÃO DO COMPARADOR ANALOGICO
    TMR0IF = 0; // habilita a flag de interrupção do timer como 0, pois o timer ainda não estorou
    TMR0IE = 1; // habilita o uso da flag de interrupção.

    RCIF = 0 ; // flag que diz se o registrado RCREG(dados recebidos da USART)
    RCIE = 1;// habilita a interrupção de quando o RC foi preenchido. 
    PEIE = 1; // HABILITA INTERRUPÇÕES DE PERIFERICOS
    GIE = 1; // habilita todas as interrupções
   

    /*** código que verifica se passou da parte de inicialização***    */


    TMR2ON = 1; // LIGA O TIMER 2
    
    ADON = 1; // HABILITA A CONVERSÃO AD -> ENQUANTO ISSO A CONVERSAO VAI SER REALIZADA
    __delay_us(1); // ESPEREMOS EM JESUS.
    GO_DONE = 1; // INICIALIZA A CONVERSAO - SOLICITA A PRIMEIRA LEITURA

   // init_comms();
    __delay_us(1);

   // printf("It's on!\n");
    /******************************************************************/
   // printf("\rDigite: \n");
    while(1)
    {
        if(!display_flag)
        {
            DISPLAY = digito[2];
        }else
        {
           DISPLAY = digito[4];
        }
    }  
}


void output_memory(){
     
}

void output_level(){
    
}
