
#ifndef _SC16IS740_H_
#define _SC16IS740_H_

//Device Address
#define     SC16IS740_SAD     (0X92)

//General Registers
#define     SC16IS740_REG_RHR        (0x00)
#define     SC16IS740_REG_THR        (0X00)
#define     SC16IS740_REG_IER        (0X01)
#define     SC16IS740_REG_FCR        (0X02)
#define     SC16IS740_REG_IIR        (0X02)
#define     SC16IS740_REG_LCR        (0X03)
#define     SC16IS740_REG_MCR        (0X04)
#define     SC16IS740_REG_LSR        (0X05)
#define     SC16IS740_REG_MSR        (0X06)
#define     SC16IS740_REG_SPR        (0X07)
#define     SC16IS740_REG_TCR        (0X06)
#define     SC16IS740_REG_TLR        (0X07)
#define     SC16IS740_REG_TXLVL      (0X08)
#define     SC16IS740_REG_RXLVL      (0X09)
#define     SC16IS740_REG_IODIR      (0X0A)
#define     SC16IS740_REG_IOSTATE    (0X0B)
#define     SC16IS740_REG_IOINTENA   (0X0C)
#define     SC16IS740_REG_IOCONTROL  (0X0E)
#define     SC16IS740_REG_EFCR       (0X0F)

//Special Registers
#define     SC16IS740_REG_DLL        (0x00)
#define     SC16IS740_REG_DLH        (0X01)

//Enhanced Registers
#define     SC16IS740_REG_EFR        (0X02)
#define     SC16IS740_REG_XON1       (0X04)
#define     SC16IS740_REG_XON2       (0X05)
#define     SC16IS740_REG_XOFF1      (0X06)
#define     SC16IS740_REG_XOFF2      (0X07)

#define     SC16IS740_INT_CTS        (0X80)
#define     SC16IS740_INT_RTS        (0X40)
#define     SC16IS740_INT_XOFF       (0X20)
#define     SC16IS740_INT_SLEEP      (0X10)
#define     SC16IS740_INT_MODEM      (0X08)
#define     SC16IS740_INT_LINE       (0X04)
#define     SC16IS740_INT_THR        (0X02)
#define     SC16IS740_INT_RHR        (0X01)

//Application Related 
#define     SC16IS740_CRYSTCAL_FREQ  (1843200UL) 
#define     SC16IS740_PROTOCOL_I2C   (0)
#define     SC16IS740_PROTOCOL_SPI   (1)

//
// Baud Rate generator caculated by:
//     divisor = (SC16IS740_CRYSTCAL_FREQ/prescaler(is 1))/(baudrate*16);
//
#define     SC16IS740_B9600          (12)  // 9600 baud
#define     SC16IS740_B19200         (06)  // 19200 baud
#define     SC16IS740_B38400         (03)  // 38400 baud
#define     SC16IS740_B57600         (02)  // 57,600 baud
#define     SC16IS740_B115200        (01)  // 115,200 baud

#define     SC16IS740_CS5            (0x00) // 5 data bits
#define     SC16IS740_CS6            (0x01) // 6 data bits
#define     SC16IS740_CS7            (0x02) // 7 data bits
#define     SC16IS740_CS8            (0x03) // 8 data bits
#define     SC16IS740_CSMASK         (~(SC16IS740_CS5|SC16IS740_CS6|SC16IS740_CS7|SC16IS740_CS8))

#define     SC16IS740_CSTOPB         (0x04) // 2 stop bits (1 otherwise)
#define     SC16IS740_CSTOPMASK      (~SC16IS740_CSTOPB)
#define     SC16IS740_PARENB         (0x08) // Enable parity bit
#define     SC16IS740_PAREVN         (0x10) // Use even parity (defaults to ODD)
#define     SC16IS740_PARMASK        (~(SC16IS740_PAREVN|SC16IS740_PARENB))

// Public functions
void    open_sc16is740( uint8_t protocol, uint8_t addr, uint32_t baud, uint8_t (*rdreg)(uint8_t), void (*init)(void), void (*wrreg)(uint8_t,uint8_t) ) 
void    sc16is740_reset(void)
int16_t sc16is740_baud(uint32_t baudrate) //return error of baudrate parts per thousand
void    sc16is740_format(uint8_t data_length, uint8_t parity_select, uint8_t stop_length )
void    sc16is740_enable_output(uint8_t tx_enable)
void    sc16is740_isr_attach( void (*f)(uint8_t s) );          //> Attach a user ISR, provides IIR status at time of interrupt
void 	sc16is740_set_flow_control (int yes); // Set the RTS/CTS flow control
int     sc16is740_writable(void)
int     sc16is740_readable(void)

int 	sc16is740_read (uint8_t *buffer, int length); // Begin asynchronous reading using 8bit buffer. More...
int     sc16is740_getc(void)
int 	sc16is740_write (const uint8_t *buffer, int length); // Begin asynchronous write using 8bit buffer. More...
size_t  sc16is740_putc(uint8_t val)
size_t  sc16is740_puts(const char *s)
size_t  sc16is740_printf(const char *fmt,... )
void    sc16is740_flush()



// Private functions
void    SetPinInterrupt(uint8_t io_int_ena)
void    InterruptControl(uint8_t int_ena)
uint8_t InterruptPendingTest(void)
void    __isr(void)

void    FIFOReset(uint8_t rx_fifo)
void    FIFOEnable(uint8_t fifo_enable)
void    FIFOSetTriggerLevel(uint8_t rx_fifo, uint8_t length)

uint8_t FIFO_readable(void)
uint8_t FIFO_writeable(void)

#endif

