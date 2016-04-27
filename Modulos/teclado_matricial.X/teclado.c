/*
    Curso de PIC WR Kits
*/
#include "header.h"
#include "teclado_functions.h"

//Método de interrupções
void interrupt interrupcao(void){
    
    //IF TIMER0
    if(TMR0IF){//Houve estouro do Timer0?        
        flag=1;
        TMR0IF = 0x00; //Limpa a flag
        TMR0 = 0x89; //Reinicia o timer0 em 137
    }

} //end interrupt


// --- Função Principal
void main(){
    //Configurando o configurando TMR0 prescaler
    OPTION_REG = 0b10000000;//desabilita pull-up interno NOT_RBPU
    T0CS = 0;// seleciona o clock interno, pois TOCS = 0;
    T0SE = 0;// borda de subida
    PSA = 0;// seleciona o prescaler pra o timer0
    PS2 = 1;// Prescaler 1:256
    PS1 = 1;// Prequencia de estouro 76Hz
    PS0 = 1;// Periodo de estouro 13,11 ms 
    
    //Configuração do ITCON overflow do TMR0
    GIE=1;// habilita interrupção global
    PEIE=1;//habilita interrupção por perifericos
    TMR0IE=1;//habilita interrupção por estouro do tmr0
    TMR0 = 0x89;//inicia o timer0 em 137

    TRISC = 0;                        //Entrada em RC0 como saida
    PORTC = 0;                        //RC0  iniciam em high e RA2, RA3 como saida
    TRISB = 0b11110000;//bit 7 a 4 como entrada(linhas) bit 3 a 0 como saida(colunas)
    PORTB=1;
     while(1)//Loop Infinito
     {
         if(flag){//Se o TMR estourou
             keyPressed();//faz varredura
         }
     } //end while

} //end main

//tempo=cicloDeMaquina*prescaler*contagem(256-TMR0)