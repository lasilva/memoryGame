
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
