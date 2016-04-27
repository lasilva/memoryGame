/*
    Curso de PIC WR Kits
*/
#include "header.h"


// --- Hardware ---
#define led1    RC0
//Linhas e colunas de varredura
#define col1    RB0
#define col2    RB1
#define col3    RB2
#define col4    RB3
#define row1    RB4
#define row2    RB5
#define row3    RB6
#define row4    RB7
//controle de varredura
unsigned char controle = 0x01;
bit flag=0;

//Funções
void pulse(char number);
void keyPressed();

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
void main()
{
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
     while(1)                         //Loop Infinito
     {
         if(flag){
             keyPressed();
         }
     } //end while

} //end main

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

//Faz Varredura nas linhas e colunas
void keyPressed(){
    
        if(col1 && controle==0x01){//Varre a coluna 1 a cada estouro de 20ms
            controle=0x02;//muda para no proximo estouro verificar a coluna 2
            col1=0x00;
            col2=0x01;
            col3=0x01;
            col4=0x01;
            if(!row1)pulse(1);
            else if(!row2)pulse(4);
            else if(!row3)pulse(7);
            else if(!row4)pulse(10);
        }else if(col2 && controle==0x02){//Varre a coluna 2 a cada estouro de 20ms
            controle=0x03;//muda para no proximo estouro verificar a coluna 3
            col1=0x01;
            col2=0x00;
            col3=0x01;
            col4=0x01;
            if(!row1)pulse(2);
            else if(!row2)pulse(5);
            else if(!row3)pulse(8);
            else if(!row4)pulse(11);
        }else if(col3 && controle==0x03){//Varre a coluna 3 a cada estouro de 20ms
            controle=0x04;//muda para no proximo estouro verificar a coluna 1
            col1=0x01;
            col2=0x01;
            col3=0x00;
            col4=0x01;
            if(!row1)pulse(3);
            else if(!row2)pulse(6);
            else if(!row3)pulse(9);
            else if(!row4)pulse(12);
        }else if(col4 && controle==0x04){//Varre a coluna 4 a cada estouro de 20ms
            controle=0x01;//muda para no proximo estouro verificar a coluna 1
            col1=0x01;
            col2=0x01;
            col3=0x01;
            col4=0x00;
            if(!row1)pulse(13);
            else if(!row2)pulse(14);
            else if(!row3)pulse(15);
            else if(!row4)pulse(16);
        }
        flag=0;
}
//tempo=cicloDeMaquina*prescaler*contagem(256-TMR0)