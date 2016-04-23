/*
    Curso de PIC WR Kits
*/
#include "header.h"
#include <xc.h>

// --- Hardware ---
#define S2      RA0
#define S1      RA1
#define led2    RA2
#define led1    RA3



void interrupt interrupcao(void){
    
    if(TMR0IF){//Houve estouro do Timer0?
    //Sim...
    
        TMR0IF = 0x00;                    //Limpa a flag
        TMR0 = 0x89;                    //Reinicia o timer0
        
        if(!S1){//S1 pressionado
             led1=0x01;//Ascende led 1
         }else if(!S2){//S2 pressionado apaga led1
             led1=0x00;
         }
        
    }

} //end interrupt


// --- Função Principal
void main()
{
    CMCON = 0b00000111;//Desabilita os comparadores
    ADCON1 = 0b00000111;//Desabilita  
    //Configurando o configurando TMR0 prescaler
    OPTION_REG=0b10000111;//desabilita pull-up interno e prescaler 1:4
    //Configuração do ITCON overflow do TMR0
    GIE=0x01;// habilita interrupção global
    PEIE=0x01;//habilita interrupção por perifericos
    TMR0IE=0x01;//habilita interrupção por estouro do tmr0
    TMR0 = 0x89;                   //Inicia o timer0

    TRISA = 0b00000011;                        //Entrada em RA0 e RA1
    PORTA = 0b00000011;                        //RA0 e RA1 iniciam em high e RA2, RA3 como saida

     while(1)                             //Loop Infinito
     {
         led2=0x00;
         __delay_ms(500);
         led2=0x01;
         __delay_ms(500);     

     } //end while

} //end main
