// **********************************************************************************
// Driver definition for HopeRF RFM69W/RFM69HW/RFM69CW/RFM69HCW, Semtech SX1231/1231H

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
#include <avr/interrupt.h>
#include "spi.h"
#include "RFM69registers.h"
#include "RFM69.h"
//#include "get_millis.h"
#include <stdint.h>

volatile uint8_t DATALEN;
volatile uint8_t SENDERID;
volatile uint8_t TARGETID;                 // should match _address
volatile uint8_t PAYLOADLEN;
volatile uint8_t ACK_REQUESTED;
volatile uint8_t ACK_RECEIVED;             // should be polled immediately after sending a packet with ACK request
volatile int16_t RSSI;                     // most accurate RSSI during reception (closest to the reception)
volatile uint8_t mode = RF69_MODE_STANDBY; // should be protected?
volatile uint8_t inISR = 0;
uint8_t isRFM69HW = 1;                     // if RFM69HW model matches high power enable possible
uint8_t address;                           //nodeID
uint8_t powerLevel = 31;
uint8_t promiscuousMode = 0;
uint32_t millis_current;
uint32_t(*millisFunc)(void);

// freqBand must be selected from 315, 433, 868, 915
void rfm69_init(uint16_t freqBand, uint8_t nodeID, uint8_t networkID, uint32_t(*millisFunction)(void))
{
	millisFunc = millisFunction;
	const uint8_t CONFIG[][2] =
	{
		/* 0x01 */ { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
		/* 0x02 */ { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
		/* 0x03 */ { REG_BITRATEMSB, RF_BITRATEMSB_55555}, // default: 4.8 KBPS
		/* 0x04 */ { REG_BITRATELSB, RF_BITRATELSB_55555},
		/* 0x05 */ { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
		/* 0x06 */ { REG_FDEVLSB, RF_FDEVLSB_50000},

		//* 0x07 */ { REG_FRFMSB, RF_FRFMSB_433},
		//* 0x08 */ { REG_FRFMID, RF_FRFMID_433},
		//* 0x09 */ { REG_FRFLSB, RF_FRFLSB_433},

		/* 0x07 */ { REG_FRFMSB, (uint8_t)(freqBand == RF_315MHZ ? RF_FRFMSB_315 : (freqBand == RF_433MHZ ? RF_FRFMSB_433 : (freqBand == RF_868MHZ ? RF_FRFMSB_868 : RF_FRFMSB_915))) },
		/* 0x08 */ { REG_FRFMID, (uint8_t)(freqBand == RF_315MHZ ? RF_FRFMID_315 : (freqBand == RF_433MHZ ? RF_FRFMID_433 : (freqBand == RF_868MHZ ? RF_FRFMID_868 : RF_FRFMID_915))) },
		/* 0x09 */ { REG_FRFLSB, (uint8_t)(freqBand == RF_315MHZ ? RF_FRFLSB_315 : (freqBand == RF_433MHZ ? RF_FRFLSB_433 : (freqBand == RF_868MHZ ? RF_FRFLSB_868 : RF_FRFLSB_915))) },


		// looks like PA1 and PA2 are not implemented on RFM69W, hence the max output power is 13dBm
		// +17dBm and +20dBm are possible on RFM69HW
		// +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
		// +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
		// +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high power PA settings (section 3.3.7 in datasheet)
		///* 0x11 */ { REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},
		///* 0x13 */ { REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)

		// RXBW defaults are { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5} (RxBw: 10.4KHz)
		/* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
		//for BR-19200: /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3 },
		/* 0x25 */ { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
		/* 0x26 */ { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
		/* 0x28 */ { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
		/* 0x29 */ { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
		///* 0x2D */ { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE } // default 3 preamble bytes 0xAAAAAA
		/* 0x2E */ { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
		/* 0x2F */ { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
		/* 0x30 */ { REG_SYNCVALUE2, networkID }, // NETWORK ID
		/* 0x37 */ { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
		/* 0x38 */ { REG_PAYLOADLENGTH, 66 }, // in variable length mode: the max frame size, not used in TX
		///* 0x39 */ { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
		/* 0x3C */ { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
		/* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
		//for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
		/* 0x6F */ { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
		{255, 0}
	};

	spi_init();                // spi initialize
	//DDRC |= 1<<PC6;          // temporary for testing LED output
	SS_DDR |= 1 << SS_PIN;       // setting SS as output
	SS_PORT |= 1 << SS_PIN;      // setting slave select high
	INT_DDR &= ~(1 << INT_PIN_n);  // setting interrupt pin input. no problem if not given
	INT_PORT &= ~(1 << INT_PIN_n); // setting pull down. because rising will cause interrupt. external pull down is needed.
								   
	//millis_init();                  // to get miliseconds

	uint32_t start = (*millisFunc)();
	uint8_t timeout = 50;
	do writeReg(REG_SYNCVALUE1, 0xAA); while (readReg(REG_SYNCVALUE1) != 0xaa && (*millisFunc)() - start < timeout);
	start = (*millisFunc)();
	do writeReg(REG_SYNCVALUE1, 0x55); while (readReg(REG_SYNCVALUE1) != 0x55 && (*millisFunc)() - start < timeout);

	//while (readReg(REG_SYNCVALUE1) != 0xaa)
	//{
	//	writeReg(REG_SYNCVALUE1, 0xaa);
	//}

	//while (readReg(REG_SYNCVALUE1) != 0x55)
	//{
	//	writeReg(REG_SYNCVALUE1, 0x55);
	//}

	for (uint8_t i = 0; CONFIG[i][0] != 255; i++)
		writeReg(CONFIG[i][0], CONFIG[i][1]);

	// Encryption is persistent between resets and can trip you up during debugging.
	// Disable it during initialization so we always start from a known state.
	encrypt(0);

	setHighPower(isRFM69HW);        // called regardless if it's a RFM69W or RFM69HW
	setMode(RF69_MODE_STANDBY);
	while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00);

	EICRn |= (1 << ISCn1) | (1 << ISCn0); // setting INTn rising. details datasheet p91. must change with interrupt pin.
	EIMSK |= 1 << INTn;               // enable INTn
	inISR = 0;
	//sei();                        //not needed because sei() called in millis_init() :)
	//millis_init();                  // to get miliseconds

	address = nodeID;
	setAddress(address);            // setting this node id
	setNetwork(networkID);
}

// set this node's address
void setAddress(uint8_t addr)
{
	writeReg(REG_NODEADRS, addr);
}

// set network address
void setNetwork(uint8_t networkID)
{
	writeReg(REG_SYNCVALUE2, networkID);
}

uint8_t canSend()
{
	if (mode == RF69_MODE_RX && PAYLOADLEN == 0 && readRSSI(0) < CSMA_LIMIT) // if signal stronger than -100dBm is detected assume channel activity
	{
		setMode(RF69_MODE_STANDBY);
		return 1;
	}
	return 0;
}

// Transmit data
void send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK)
{
	writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
	millis_current = (*millisFunc)();
	while (!canSend() && (*millisFunc)() - millis_current < RF69_CSMA_LIMIT_MS) receiveDone();
	sendFrame(toAddress, buffer, bufferSize, requestACK, 0);
}

// check whether an ACK was requested in the last received packet (non-broadcasted packet)
uint8_t ACKRequested()
{
	return ACK_REQUESTED && (TARGETID != RF69_BROADCAST_ADDR);
}

// should be called immediately after reception in case sender wants ACK
void sendACK(const void* buffer, uint8_t bufferSize)
{
	ACK_REQUESTED = 0;   // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
	uint8_t sender = SENDERID;
	int16_t _RSSI = RSSI; // save payload received RSSI value
	writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
	millis_current = (*millisFunc)();
	while (!canSend() && (*millisFunc)() - millis_current < RF69_CSMA_LIMIT_MS) receiveDone();
	SENDERID = sender;    // TWS: Restore SenderID after it gets wiped out by receiveDone() n.b. actually now there is no receiveDone() :D
	sendFrame(sender, buffer, bufferSize, 0, 1);
	RSSI = _RSSI; // restore payload RSSI
}

// set *transmit/TX* output power: 0=min, 31=max
// this results in a "weaker" transmitted signal, and directly results in a lower RSSI at the receiver
// the power configurations are explained in the SX1231H datasheet (Table 10 on p21; RegPaLevel p66): http://www.semtech.com/images/datasheet/sx1231h.pdf
// valid powerLevel parameter values are 0-31 and result in a directly proportional effect on the output/transmission power
// this function implements 2 modes as follows:
//       - for RFM69W the range is from 0-31 [-18dBm to 13dBm] (PA0 only on RFIO pin)
//       - for RFM69HW the range is from 0-31 [5dBm to 20dBm]  (PA1 & PA2 on PA_BOOST pin & high Power PA settings - see section 3.3.7 in datasheet, p22)
void setPowerLevel(uint8_t powerLevel)
{
	uint8_t _powerLevel = powerLevel;
	if (isRFM69HW == 1) _powerLevel /= 2;
	writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0xE0) | _powerLevel);
}

//put transceiver in sleep mode to save battery - to wake or resume receiving just call receiveDone()
void sleep()
{
	setMode(RF69_MODE_SLEEP);
}

uint8_t readTemperature(uint8_t calFactor) // returns centigrade
{
	setMode(RF69_MODE_STANDBY);
	writeReg(REG_TEMP1, RF_TEMP1_MEAS_START);
	while ((readReg(REG_TEMP1) & RF_TEMP1_MEAS_RUNNING));
	return ~readReg(REG_TEMP2) + COURSE_TEMP_COEF + calFactor; // 'complement' corrects the slope, rising temp = rising val
} // COURSE_TEMP_COEF puts reading in the ballpark, user can add additional correction

// return the frequency (in Hz)
uint32_t getFrequency()
{
	return RF69_FSTEP * (((uint32_t)readReg(REG_FRFMSB) << 16) + ((uint16_t)readReg(REG_FRFMID) << 8) + readReg(REG_FRFLSB));
}

// set the frequency (in Hz)
void setFrequency(uint32_t freqHz)
{
	uint8_t oldMode = mode;
	if (oldMode == RF69_MODE_TX) {
		setMode(RF69_MODE_RX);
	}
	freqHz /= RF69_FSTEP; // divide down by FSTEP to get FRF
	writeReg(REG_FRFMSB, freqHz >> 16);
	writeReg(REG_FRFMID, freqHz >> 8);
	writeReg(REG_FRFLSB, freqHz);
	if (oldMode == RF69_MODE_RX) {
		setMode(RF69_MODE_SYNTH);
	}
	setMode(oldMode);
}

// Read byte from register
uint8_t readReg(uint8_t addr)
{
	select();
	spi_fast_shift(addr & 0x7F);
	uint8_t regval = spi_fast_shift(0);
	unselect();
	return regval;
}

// Write byte to register
void writeReg(uint8_t addr, uint8_t value)
{
	select();
	spi_fast_shift(addr | 0x80);
	spi_fast_shift(value);
	unselect();
}

// To enable encryption: radio.encrypt("ABCDEFGHIJKLMNOP");
// To disable encryption: encrypt(null) or encrypt(0)
// KEY HAS TO BE 16 bytes !!!
void encrypt(const char* key)
{
	setMode(RF69_MODE_STANDBY);
	if (key != 0)
	{
		select();
		spi_fast_shift(REG_AESKEY1 | 0x80);
		for (uint8_t i = 0; i < 16; i++)
			spi_fast_shift(key[i]);
		unselect();
	}
	else
		writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFE) | 0x00);
}

void setMode(uint8_t newMode)
{
	if (newMode == mode)
		return;

	switch (newMode)
	{
	case RF69_MODE_TX:
		writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
		if (isRFM69HW) setHighPowerRegs(1);
		break;
	case RF69_MODE_RX:
		writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
		if (isRFM69HW) setHighPowerRegs(0);
		break;
	case RF69_MODE_SYNTH:
		writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
		break;
	case RF69_MODE_STANDBY:
		writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
		break;
	case RF69_MODE_SLEEP:
		writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
		break;
	default:
		return;
	}
	// we are using packet mode, so this check is not really needed
	// but waiting for mode ready is necessary when going from sleep because the FIFO may not be immediately available from previous mode
	while (mode == RF69_MODE_SLEEP && (readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
	mode = newMode;
}

// internal function
void setHighPowerRegs(uint8_t onOff)
{
	if (onOff == 1)
	{
		writeReg(REG_TESTPA1, 0x5D);
		writeReg(REG_TESTPA2, 0x7C);
	}
	else
	{
		writeReg(REG_TESTPA1, 0x55);
		writeReg(REG_TESTPA2, 0x70);
	}
}

// for RFM69HW only: you must call setHighPower(1) after rfm69_init() or else transmission won't work
void setHighPower(uint8_t onOff)
{
	isRFM69HW = onOff;
	writeReg(REG_OCP, isRFM69HW ? RF_OCP_OFF : RF_OCP_ON);

	if (isRFM69HW == 1) // turning ON
		writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON); // enable P1 & P2 amplifier stages
	else
		writeReg(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | powerLevel); // enable P0 only
}

// get the received signal strength indicator (RSSI)
int16_t readRSSI(uint8_t forceTrigger)
{
	int16_t rssi = 0;
	if (forceTrigger == 1)
	{
		// RSSI trigger not needed if DAGC is in continuous mode
		writeReg(REG_RSSICONFIG, RF_RSSI_START);
		while ((readReg(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00); // wait for RSSI_Ready
	}
	rssi = -readReg(REG_RSSIVALUE);
	rssi >>= 1;
	return rssi;
}

// internal function
void sendFrame(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t requestACK, uint8_t sendACK)
{
	setMode(RF69_MODE_STANDBY); // turn off receiver to prevent reception while filling fifo
	while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
	writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
	if (bufferSize > RF69_MAX_DATA_LEN)
		bufferSize = RF69_MAX_DATA_LEN;

	// control byte
	uint8_t CTLbyte = 0x00;
	if (sendACK == 1)
		CTLbyte = RFM69_CTL_SENDACK;
	else if (requestACK == 1)
		CTLbyte = RFM69_CTL_REQACK;

	// write to FIFO
	select(); //enable data transfer
	spi_fast_shift(REG_FIFO | 0x80);
	spi_fast_shift(bufferSize + 3);
	spi_fast_shift(toAddress);
	spi_fast_shift(address);
	spi_fast_shift(CTLbyte);

	for (uint8_t i = 0; i < bufferSize; i++)
		spi_fast_shift(((uint8_t*)buffer)[i]);

	unselect();

	// no need to wait for transmit mode to be ready since its handled by the radio
	setMode(RF69_MODE_TX);
	millis_current = (*millisFunc)();
	//_delay_ms(500);
	// wait for DIO to high
	// for PINE5
	while (bit_is_clear(INT_PIN, INT_pin_num) && (*millisFunc)() - millis_current < RF69_TX_LIMIT_MS); // must change with interrupt pin change
	setMode(RF69_MODE_STANDBY);
}

// Calibrate RC
void rcCalibration()
{
	writeReg(REG_OSC1, RF_OSC1_RCCAL_START);
	while ((readReg(REG_OSC1) & RF_OSC1_RCCAL_DONE) == 0x00);
}

uint8_t sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime)
{
	for (uint8_t i = 0; i <= retries; i++)
	{
		send(toAddress, buffer, bufferSize, 1);
		millis_current = (*millisFunc)();
		while ((*millisFunc)() - millis_current < retryWaitTime)
		{
			if (ACKReceived(toAddress))
			{
				return 1;
			}
		}
	}
	return 0;
}

// should be polled immediately after sending a packet with ACK request
uint8_t ACKReceived(uint8_t fromNodeID)
{
	if (receiveDone())
		return (SENDERID == fromNodeID || fromNodeID == RF69_BROADCAST_ADDR) && ACK_RECEIVED;
	return 0;
}

// checks if a packet was received and/or puts transceiver in receive (ie RX or listen) mode
uint8_t receiveDone()
{
	cli();

	if (mode == RF69_MODE_RX && PAYLOADLEN > 0)
	{
		setMode(RF69_MODE_STANDBY); // enables interrupts
		return 1;
	}
	else if (mode == RF69_MODE_RX) // already in RX no payload yet
	{
		sei(); // explicitly re-enable interrupts
		return 0;
	}
	receiveBegin();
	sei();
	return 0;
}

// internal function
void receiveBegin()
{
	DATALEN = 0;
	SENDERID = 0;
	TARGETID = 0;
	PAYLOADLEN = 0;
	ACK_REQUESTED = 0;
	ACK_RECEIVED = 0;
	RSSI = 0;
	if (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY)
		writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
	writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01); // set DIO0 to "PAYLOADREADY" in receive mode
	setMode(RF69_MODE_RX);
}

// 1  = disable filtering to capture all frames on network
// 0 = enable node/broadcast filtering to capture only frames sent to this/broadcast address
void promiscuous(uint8_t onOff)
{
	promiscuousMode = onOff;
	if (promiscuousMode == 0)
		writeReg(REG_PACKETCONFIG1, (readReg(REG_PACKETCONFIG1) & 0xF9) | RF_PACKET1_ADRSFILTERING_NODEBROADCAST);
	else
		writeReg(REG_PACKETCONFIG1, (readReg(REG_PACKETCONFIG1) & 0xF9) | RF_PACKET1_ADRSFILTERING_OFF);
}

// Only reenable interrupts if we're not being called from the ISR
void maybeInterrupts()
{
	if (!inISR) sei();
}

// Enable SPI transfer
void select()
{
	SS_PORT &= ~(1 << SS_PIN);
	cli();
}

// Disable SPI transfer
void unselect()
{
	SS_PORT |= 1 << SS_PIN;
	maybeInterrupts();
}

// Interrupt Service Routine
ISR(INT_VECT)
{
	inISR = 1;
	if (mode == RF69_MODE_RX && (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY))
	{
		setMode(RF69_MODE_STANDBY);
		select();
		spi_fast_shift(REG_FIFO & 0x7F);
		PAYLOADLEN = spi_fast_shift(0);
		if (PAYLOADLEN > 66) PAYLOADLEN = 66;
		TARGETID = spi_fast_shift(0);
		if (!(promiscuousMode || TARGETID == address || TARGETID == RF69_BROADCAST_ADDR) // match this node's address, or broadcast address or anything in promiscuous mode
			|| PAYLOADLEN < 3) // address situation could receive packets that are malformed and don't fit this libraries extra fields
		{
			PAYLOADLEN = 0;
			unselect();
			receiveBegin();
			return;
		}

		DATALEN = PAYLOADLEN - 3;
		SENDERID = spi_fast_shift(0);
		uint8_t CTLbyte = spi_fast_shift(0);

		ACK_RECEIVED = CTLbyte & RFM69_CTL_SENDACK; // extract ACK-received flag
		ACK_REQUESTED = CTLbyte & RFM69_CTL_REQACK; // extract ACK-requested flag

		//interruptHook(CTLbyte);                   // TWS: hook to derived class interrupt function

		for (uint8_t i = 0; i < DATALEN; i++)
		{
			DATA[i] = spi_fast_shift(0);
		}
		if (DATALEN < RF69_MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string
		unselect();
		setMode(RF69_MODE_RX);
	}
	RSSI = readRSSI(0);
	inISR = 0;
}
