// **********************************************************************************
// Driver definition for HopeRF RFM69W/RFM69HW/RFM69CW/RFM69HCW, Semtech SX1231/1231H
// **********************************************************************************

// **********************************************************************************
// converted to avr environment by Zulkar Nayem

// microcontroller : atmega64
// default pins:
// MOSI -> PB2
// MISO -> PB3
// SS -> PB0
// DIO0 -> PE5 that is INT5, an interrupt enabled pin
// **********************************************************************************

#include <avr/io.h>

#ifndef RFM69_h
#define RFM69_h

#ifdef __cplusplus
extern "C" {
#endif

// Definitions
#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#   define SS_DDR                DDRB
#   define SS_PORT              PORTB
#   define SS_PIN                 PB2

#   define INT_DDR               DDRD
#   define INT_PORT             PORTD
#   define INT_PIN               PIND
#   define INT_pin_num              2
#   define INT_PIN_n              PD2
#   define INTn                  INT0
#   define ISCn0                ISC00
#   define ISCn1                ISC01
#   define INT_VECT         INT0_vect

#   define EICRn               EICRA

#elif defined (__AVR_ATmega64__)
#   define SS_DDR                DDRB
#   define SS_PORT              PORTB
#   define SS_PIN                 PB0

#   define INT_DDR               DDRE
#   define INT_PORT             PORTE
#   define INT_PIN               PINE
#   define INT_pin_num              5
#   define INT_PIN_n              PE5
#   define INTn                  INT5
#   define ISCn0                ISC50
#   define ISCn1                ISC51
#   define INT_VECT         INT5_vect

#   define EICRn               EICRB
#else
	Error: Unknown target board for AVR
#endif


#define RF69_MAX_DATA_LEN       61  // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define CSMA_LIMIT             -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0   // XTAL OFF
#define RF69_MODE_STANDBY       1   // XTAL ON
#define RF69_MODE_SYNTH         2   // PLL ON
#define RF69_MODE_RX            3   // RX MODE
#define RF69_MODE_TX            4   // TX MODE
#define null                  0
#define COURSE_TEMP_COEF    -90     // puts the temperature reading in the ballpark, user can fine tune the returned value
#define RF69_BROADCAST_ADDR 255
#define RF69_CSMA_LIMIT_MS 1000
#define RF69_TX_LIMIT_MS   1000
#define RF69_FSTEP    61.035156     // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet) FXOSC = module crystal oscillator frequency 
// TWS: define CTLbyte bits
#define RFM69_CTL_SENDACK   0x80
#define RFM69_CTL_REQACK    0x40

// Global Variables
volatile uint8_t DATA[RF69_MAX_DATA_LEN];  // recv/xmit buf, including header & crc bytes

// Function Declerations
void rfm69_init(uint16_t freqBand, uint8_t nodeID, uint8_t networkID, uint32_t(*millisFunction)(void));
void setAddress(uint8_t addr);
void setNetwork(uint8_t networkID);
uint8_t canSend();
void send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK);
uint8_t sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime);
uint8_t ACKRequested();
uint8_t ACKReceived(uint8_t fromNodeID);
void receiveBegin();
uint8_t receiveDone();
void sendACK(const void* buffer, uint8_t bufferSize);
uint32_t getFrequency();
void setFrequency(uint32_t freqHz);
void encrypt(const char* key);
int16_t readRSSI(uint8_t forceTrigger);
void setHighPower(uint8_t onOFF);           // has to be called after initialize() for RFM69HW
void setPowerLevel(uint8_t level);            // reduce/increase transmit power level
void sleep();
uint8_t readTemperature(uint8_t calFactor); // get CMOS temperature (8bit)
void rcCalibration();                         // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]
uint8_t readReg(uint8_t addr);
void writeReg(uint8_t addr, uint8_t val);
void sendFrame(uint8_t toAddress, const void* buffer, uint8_t size, uint8_t requestACK, uint8_t sendACK);
void setMode(uint8_t mode);
void setHighPowerRegs(uint8_t onOff);
void promiscuous(uint8_t onOff);
void maybeInterrupts();
void select();
void unselect();
uint8_t receiveDone();

#ifdef __cplusplus
}
#endif

#endif

