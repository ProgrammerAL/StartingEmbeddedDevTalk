#include <avr/io.h>
#include "spi.h"

void spi_init()
// Initialize pins for spi communication
{
    SPI_DDR &= ~((1<<MOSI)|(1<<MISO)|(1<<SS)|(1<<SCK));
    // Define the following pins as output
    SPI_DDR |= ((1<<MOSI)|(1<<SS)|(1<<SCK));

    
    SPCR = ((1<<SPE)|               // SPI Enable
            (0<<SPIE)|              // SPI Interupt Enable
            (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|              // Master/Slave select
            (0<<SPR1)|(0<<SPR0)|    // SPI Clock Rate
            (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

    SPSR = (1<<SPI2X);              // Double Clock Rate
    
    // SCK frequency 4MHz with above parameters. Deails in datasheet
    
}

void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while((SPSR & (1<<SPIF))==0);
        datain[i] = SPDR;
    }
}

void spi_transmit_sync (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
    uint8_t i;
    for (i = 0; i < len; i++) {
        SPDR = dataout[i];
        while((SPSR & (1<<SPIF))==0);
    }
}

uint8_t spi_fast_shift (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
    SPDR = data;
    while((SPSR & (1<<SPIF))==0);
    return SPDR;
}
