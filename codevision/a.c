#include <mega32.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#include <string.h>


unsigned char lcd_str[17];
char seg[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x20,0X83,0XC6};
char keybord[16] = {'7','8','9','#','4','5','6','*','1','2','3','-','c','0','=','+'};
//{'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
char password[5];
char menu1[]="led";
char menu2[]="buzzer";
char menu3[]="relay";
int menu_count=1;
int ps_count =0;
int seg_count=0;
int inout=0;
int lcd_cl=0;
int t_bounce =0;



interrupt [EXT_INT0] void ext_int0_isr(void)
{if(ps_count==5 && t_bounce==0){
    t_bounce=1;
    if(menu_count==3){
        menu_count=0;}
    menu_count++;
    lcd_cl=1;}
}



// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
if (ps_count==5 && t_bounce==0){
    t_bounce=1;
    lcd_cl=1;
    switch(inout){
        case 0:
            inout=1;
            lcd_cl=1;
            switch(menu_count){
                case 1: 
                    PORTC=0X00;
                    PORTA=seg[10];
                    PORTC.0=1; 
                    break;
                case 2:
                    PORTC=0X00;
                    PORTA=seg[11];
                    PORTC.1=1;
                    delay_ms(1000);
                    PORTC.1=0;
                    break;
                case 3:
                    PORTC=0X00;
                    PORTA=seg[12];
                    PORTC.2=1;
                    break;
            }
            break;
        case 1:
            lcd_cl=1;
            PORTC=0X00;
            inout=0;
            break;
    
        
    }
}

}



// External Interrupt 2 service routine
interrupt [EXT_INT2] void ext_int2_isr(void)
{
if (ps_count!=5){
    lcd_clear();
    ps_count ++;                     
    password[ps_count-1] = keybord[PIND/16];
    sprintf(lcd_str,"%s",password);
    lcd_puts(lcd_str);

    if (ps_count==4){
        ps_count=0;
        
        if(strcmp(password,"*14#")==0){
            lcd_puts("  true");
            delay_ms(1000);
            lcd_clear();
            ps_count = 5;
            }
        else{
            lcd_puts("  wrong");
            delay_ms(1000);
            lcd_clear();
            strncpy(password, "", sizeof(password));}
        }
}}



// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
if (ps_count==5 && inout==0){
    PORTA=0XFF;
    return;}
if(seg_count==301 && ps_count!=5)
    seg_count=0;    
seg_count++;
if (seg_count%30==1 && ps_count!=5){
    PORTA=seg[seg_count/30];}

}


// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
//not used
}



void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRA=(0<<DDA7) | (1<<DDA6) | (1<<DDA5) | (1<<DDA4) | (1<<DDA3) | (1<<DDA2) | (1<<DDA1) | (1<<DDA0);
// State: Bit7=T Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=Out Bit0=Out 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 32.768 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 32.768 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Rising Edge
// INT1: On
// INT1 Mode: Rising Edge
// INT2: On
// INT2 Mode: Rising Edge
GICR|=(1<<INT1) | (1<<INT0) | (1<<INT2);
MCUCR=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
MCUCSR=(1<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (1<<INTF2);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
SFIOR=(0<<ACME);

// ADC initialization
// ADC disabled
ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 3
// D4 - PORTB Bit 4
// D5 - PORTB Bit 5
// D6 - PORTB Bit 6
// D7 - PORTB Bit 7
// Characters/line: 8
lcd_init(16);
// Global enable interrupts
#asm("sei")

lcd_puts("password:");
delay_ms(1000);
lcd_clear();

while (1)
      {if (ps_count==5){
          if(t_bounce==1){
              delay_ms(200);
              t_bounce=0;}
              
          if (lcd_cl==1){
              lcd_clear();
              lcd_cl=0;}
              
          if(menu_count!=3 && inout==0){
              lcd_gotoxy(0,0);
              lcd_puts(menu1);
              lcd_gotoxy(0,1);
              lcd_puts(menu2);}
          else if(menu_count!=3 && inout==1){
              if (menu_count==1){
                  lcd_gotoxy(0,0);
                  lcd_puts(menu1);}
              else{
                  lcd_gotoxy(0,1);
                  lcd_puts(menu2);}} 
          else{ 
              lcd_gotoxy(0,0);
              lcd_puts(menu3);}
            
          
          if(menu_count!=2){
              lcd_gotoxy(7,0);}
          else{
              lcd_gotoxy(7,1);}
                
              
          if (inout==0){ 
              lcd_puts("<==");}
          else{
              lcd_puts("==>");}

      }}
}
