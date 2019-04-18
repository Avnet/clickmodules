
#ifndef _SC16IS750_H_
#define _SC16IS750_H_

//Device Address
#define     SC16IS750_SAD     (0X92)

//General Registers
#define     SC16IS750_REG_RHR        (0x00)
#define     SC16IS750_REG_THR        (0X00)
#define     SC16IS750_REG_IER        (0X01)
#define     SC16IS750_REG_FCR        (0X02)
#define     SC16IS750_REG_IIR        (0X02)
#define     SC16IS750_REG_LCR        (0X03)
#define     SC16IS750_REG_MCR        (0X04)
#define     SC16IS750_REG_LSR        (0X05)
#define     SC16IS750_REG_MSR        (0X06)
#define     SC16IS750_REG_SPR        (0X07)
#define     SC16IS750_REG_TCR        (0X06)
#define     SC16IS750_REG_TLR        (0X07)
#define     SC16IS750_REG_TXLVL      (0X08)
#define     SC16IS750_REG_RXLVL      (0X09)
#define     SC16IS750_REG_IODIR      (0X0A)
#define     SC16IS750_REG_IOSTATE    (0X0B)
#define     SC16IS750_REG_IOINTENA   (0X0C)
#define     SC16IS750_REG_IOCONTROL  (0X0E)
#define     SC16IS750_REG_EFCR       (0X0F)

//Special Registers
#define     SC16IS750_REG_DLL        (0x00)
#define     SC16IS750_REG_DLH        (0X01)

//Enhanced Registers
#define     SC16IS750_REG_EFR        (0X02)
#define     SC16IS750_REG_XON1       (0X04)
#define     SC16IS750_REG_XON2       (0X05)
#define     SC16IS750_REG_XOFF1      (0X06)
#define     SC16IS750_REG_XOFF2      (0X07)

#define     SC16IS750_INT_CTS        (0X80)
#define     SC16IS750_INT_RTS        (0X40)
#define     SC16IS750_INT_XOFF       (0X20)
#define     SC16IS750_INT_SLEEP      (0X10)
#define     SC16IS750_INT_MODEM      (0X08)
#define     SC16IS750_INT_LINE       (0X04)
#define     SC16IS750_INT_THR        (0X02)
#define     SC16IS750_INT_RHR        (0X01)

//Application Related 

#define     SC16IS750_CRYSTCAL_FREQ (14745600UL) 
#define     SC16IS750_PROTOCOL_I2C  (0)
#define     SC16IS750_PROTOCOL_SPI  (1)

#define     SC16IS750_B9600   // 9600 baud
#define     SC16IS750_B19200  // 19200 baud
#define     SC16IS750_B3400   // 38400 baud
#define     SC16IS750_B19200  // 19,200 buad
#define     SC16IS750_B57600  // 57,600 baud
#define     SC16IS750_B76800  // 76,800 baud
#define     SC16IS750_B115200 // 115,200 baud

#define     SC16IS750_CS5     // 5 data bits
#define     SC16IS750_CS6     // 6 data bits
#define     SC16IS750_CS7     // 7 data bits
#define     SC16IS750_CS8     // 8 data bits

#define     SC16IS750_CSTOPB  // 2 stop bits (1 otherwise)
#define     SC16IS750_PARENB  // Enable parity bit
#define     SC16IS750_PARODD  // Use odd parity instead of even

class SC16IS750 : public Stream
{ 
    public:
        SC16IS750(uint8_t prtcl = SC16IS750_PROTOCOL_I2C, uint8_t addr = SC16IS750_ADDRESS_AD);
        int     read();
        size_t  write(uint8_t val);
        int     available();
        void    pinMode(uint8_t pin, uint8_t io);
        void    digitalWrite(uint8_t pin, uint8_t value);
        uint8_t digitalRead(uint8_t pin);
        uint8_t ping();
        int     peek();
        void    flush();
        uint8_t GPIOGetPortState(void);
        uint8_t InterruptPendingTest(void);
        void    SetPinInterrupt(uint8_t io_int_ena);
        void    InterruptControl(uint8_t int_ena);
        void    ModemPin(uint8_t gpio); //gpio == 0, gpio[7:4] are modem pins, gpio == 1 gpio[7:4] are gpios
        void    GPIOLatch(uint8_t latch);
        
    
    private:
        uint8_t device_address_sspin;
        uint8_t protocol;
        int16_t SetBaudrate(uint32_t baudrate);
        uint8_t ReadRegister(uint8_t reg_addr);
        void    WriteRegister(uint8_t reg_addr, uint8_t val);
        void    SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length );
        void    GPIOSetPinMode(uint8_t pin_number, uint8_t i_o);
        void    GPIOSetPinState(uint8_t pin_number, uint8_t pin_state);
        
        uint8_t GPIOGetPinState(uint8_t pin_number);
        void    GPIOSetPortMode(uint8_t port_io);
        void    GPIOSetPortState(uint8_t port_state);
        void    ResetDevice(void);
        
        void    __isr(void);
        void    FIFOEnable(uint8_t fifo_enable);
        void    FIFOReset(uint8_t rx_fifo);
        void    FIFOSetTriggerLevel(uint8_t rx_fifo, uint8_t length);
        uint8_t FIFOAvailableData(void);
        uint8_t FIFOAvailableSpace(void);
        void    WriteByte(uint8_t val);
        int     ReadByte(void);
        void    EnableTransmit(uint8_t tx_enable);
        int     peek_buf;
        uint8_t peek_flag;
};

#endif





    
    
