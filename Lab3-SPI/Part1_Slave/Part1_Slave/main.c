/*
 * Part1_Slave.c
 *
 * Created: 10/11/2019 7:14:31 PM
 * Author : Owner
 */ 

#include <avr/io.h>

void SPI_ServantInit(void);
char SPI_ServantReceive(void);

int main(void)
{
<<<<<<< HEAD
    SPI_ServantInit();
    
    while (1) 
    {

=======
    while (1) 
    {
		
>>>>>>> 35e4c43a3e05c339e138c9f3444a28de006c95bf
    }
}

void SPI_ServantInit(void) {
    /* Set MISO output, all others input */
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x40; PORTB = 0xBF;//B4=~SS | B5=MOSI | B6=MISO | B7=SCLK

    /* Enable SPI */
    DDR_SPI = (1<<DD_MISO);
    SPCR = (1<<SPE);
}

char SPI_ServantReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR & (1<<SPIF)))
    ;
    /* Return Data Register */
    return SPDR;
}

