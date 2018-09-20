#ifndef SPI_h
#define SPI_h

#ifdef __cplusplus
extern "C" {
#endif


#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
	#define SPI_DDR   DDRB
	#define MISO         4
    #define MOSI         3
    #define SCK          5
    #define SS           2

#elif defined (__AVR_ATmega64__)
    #define SPI_DDR   DDRB
    #define MISO         3
    #define MOSI         2
    #define SCK          1
    #define SS           0
#endif

void spi_init();
void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len);
void spi_transmit_sync (uint8_t * dataout, uint8_t len);
uint8_t spi_fast_shift (uint8_t data);

#ifdef __cplusplus
}
#endif

#endif