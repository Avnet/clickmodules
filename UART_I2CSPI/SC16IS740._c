
#include <SC16IS740.h>

static uint8_t (*_rdReg)(uint8_t reg_addr);
static void    (*_wrReg)(uint8_t reg_addr, uint8_t val);

open_sc16is740( uint8_t protocol, uint8_t addr, uint32_t baud, uint8_t (*rdreg)(uint8_t), void (*init)(void), void (*wrreg)(uint8_t,uint8_t) ) 
{ 
    protocol = prtcl;
    device_address_sspin = addr_sspin;
    init(); //call users initialization function to setup any necessary pins

    _rdReg = rdreg;
    _wrReg= wrreg;

    ResetDevice();
    FIFOEnable(1);
    SetBaud(baud);
    SetLine(SC16IS740_CS8,0,0);  //default is 8-bits, no parity, 1 stop
}

void sc16is740_reset(void)
{
    uint8_t reg = _rdReg(SC16IS740_REG_IOCONTROL) | 0x08;
    _wrReg(SC16IS740_REG_IOCONTROL, reg);
}

void sc16is740_baud(uint32_t baudrate_divisor) 
{
    uint8_t temp_lcr   = _rdReg(SC16IS740_REG_LCR);

    _wrReg(SC16IS740_REG_LCR,(temp_lcr|0x80));         //enable br generator latch
    _wrReg(SC16IS740_REG_DLL,(uint8_t)divisor_divisor);        
    _wrReg(SC16IS740_REG_DLH,(uint8_t)(divisor_divisor>>8));   
    _wrReg(SC16IS740_REG_LCR,temp_lcr);                //start br generator
}

void sc16is740_format(uint8_t data_length, uint8_t parity_select, uint8_t stop_length )
{
    uint8_t temp_lcr = _rdReg(SC16IS740_REG_LCR) & 0xC0;    //Clear the lower six bit of LCR (LCR[0] to LCR[5]
    temp_lcr |= (data_length | parity | stop_length);
    _wrReg(SC16IS740_REG_LCR,temp_lcr);
}


void sc16is740_enable_output(uint8_t tx_enable)
{
    uint8_t temp_efcr;
    temp_efcr = _rdReg(SC16IS740_REG_EFCR);
    if ( tx_enable == 0) 
        temp_efcr |= 0x04;
    else 
        temp_efcr &= 0xFB;
    _wrReg(SC16IS740_REG_EFCR,temp_efcr);
}

void InterruptControl(uint8_t int_ena)
{
    _wrReg(SC16IS740_REG_IER, int_ena);
}

uint8_t InterruptPendingTest(void)
{
    return (_rdReg(SC16IS740_REG_IIR) & 0x01);
}

void    sc16is740_isr_attach( void (*f)(uint8_t s) )          //> Attach a user ISR, provides IIR status at time of interrupt
{
    usr_isr = f;
}

void __isr(void)
{
    uint8_t irq_src = _rdReg(SC16IS740_REG_IIR) >> 1;
    irq_src &= 0x3F;
    if( usr_isr )
        usr_isr(irq_src);
    else
        switch (irq_src) {
            case 0x06:               //Receiver Line Status Error
            case 0x0c:               //Receiver time-out interrupt
            case 0x04:               //RHR interrupt
            case 0x02:               //THR interrupt
            case 0x00:               //modem interrupt;
            case 0x30:               //input pin change of state
            case 0x10:               //XOFF
            case 0x20:               //CTS,RTS
            default:
                break;
            }
}

//
// FIFO control...
//

void FIFOReset(uint8_t rx_fifo)
{
    uint8_t temp_fcr = _rdReg(SC16IS740_REG_FCR);
    temp_fcr |= (rx_fifo==0)? 0x04:0x02;
    _wrReg(SC16IS740_REG_FCR,temp_fcr);
}

void FIFOEnable(uint8_t fifo_enable)
{
    uint8_t temp_fcr=_rdReg(SC16IS740_REG_FCR);

    if (fifo_enable == 0)
        temp_fcr &= 0xFE;
    else 
        temp_fcr |= 0x01;
    _wrReg(SC16IS740_REG_FCR,temp_fcr);
}

uint8_t FIFO_readable(void)
{
   return _rdReg(SC16IS740_REG_RXLVL);
}

uint8_t FIFO_writable(void)
{
   return _rdReg(SC16IS740_REG_TXLVL);
}

int sc16is740_getc(void)
{
    volatile uint8_t val = -1;
    if (FIFO_readable() != 0) 
        val = _rdReg(SC16IS740_REG_RHR);
    return val;
}

size_t sc16is740_putc(uint8_t val)
{
    uint8_t tmp_lsr;
    do {
        tmp_lsr = _rdReg(SC16IS740_REG_LSR);
        } 
    while ((tmp_lsr&0x20) ==0);
    _wrReg(SC16IS740_REG_THR,val);
}

size_t  sc16is740_puts(const char *s)
{
    while( *s != 0x00 )
        sc16is740_putc(*s++);
    sc16is740_putc('\r');
    sc16is740_putc('\n');
}

size_t sc16is740_printf( const char fmt, ... ) 
{
    char buff[255];
    va_list ap;
    va_start(ap, fmt);
    vsprintf (buff,fmt, ap);
    for( size_t i=0; i<strlen(buff); i++ )
        _MCP23S17_putchar(buff[i]);
    va_end(ap);
    return strlen(buff);
}

void sc16is740_flush()
{
    uint8_t tmp_lsr;

    do {
        tmp_lsr = _rdReg(SC16IS740_REG_LSR);
        } 
    while ((tmp_lsr&0x20) == 0);
}

int  sc16is740_read (uint8_t *buffer, int length)
{
    int len = 0;
    while( FIFO_readable() && len < length)
        buffer[len++] = _rdReg(SC16IS740_REG_RHR);
    return len;
}

int  sc16is740_write (const uint8_t *buffer, int length)
{
    int len = 0;
    while( FIFO_writeable() && len < length )
        putc(buffer[len]);
    return len;
}

int sc16is740_readable(void)
{
    return FIFO_readable();
}

