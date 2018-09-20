#ifndef TrinketBoardConstants_H
#define TrinketBoardConstants_H

/* Below are registers for the specific board we are compiling for. A 3V Atmel ATtiny85 */

/* SPI Status Register */
#define    SPIF         7
#define    WCOL         6

/* SPI Control Register */
#define    SPIE         7
#define    SPE          6
#define    DORD         5
#define    MSTR         4
#define    CPOL         3
#define    CPHA         2
#define    SPR1         1
#define    SPR0         0

/* Other constants. Need to test all of these. Especially the ones I can't even begin to test out what they do */
#define RXC0 7
#define RXCIE0 7
#define RXEN0 4
#define SPCR _SFR_IO8(0x2C)
#define SPSR _SFR_IO8(0x2D)
#define TXEN0 3
#define UBRR0 _SFR_MEM16(0xC4)
#define UCSR0A _SFR_MEM8(0xC0)
#define UCSR0B _SFR_MEM8(0xC1)
#define UDR0 _SFR_MEM8(0xC6)
#define UDRE0 5


#define SPDR _SFR_IO8(0x2E)
#define SPDR0 0
#define SPDR1 1
#define SPDR2 2
#define SPDR3 3
#define SPDR4 4
#define SPDR5 5
#define SPDR6 6
#define SPDR7 7

#endif