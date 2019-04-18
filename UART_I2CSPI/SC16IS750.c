
#include <SC16IS750.h>

uint8_t (*ReadRegister)(uint8_t reg_addr);
void    (*WriteRegister)(uint8_t reg_addr, uint8_t val);

open_SC16IS750( uint8_t protocol, uint8_t addr, uint32_t baud, uint8_t (*rdreg)(uint8_t), void (*init)(void), void (*wrreg)(uint8_t,uint8_t) ) 
{ 
    protocol = prtcl;
    device_address_sspin = addr_sspin;
    init(); //call users initialization function to setup any necessary pins

    ReadRegister = rdreg;
    WriteRegister= wrreg;

    ResetDevice();
    FIFOEnable(1);
    SetBaud(baud);
    SetLine(8,0,1);  //8-bits, no parity, 1 stop
}

void ResetDevice(void)
{
    uint8_t reg = ReadRegister(SC16IS750_REG_IOCONTROL) | 0x08;
    WriteRegister(SC16IS750_REG_IOCONTROL, reg);
}

int16_t SetBaud(uint32_t baudrate) //return error of baudrate parts per thousand
{
    uint16_t divisor;
    uint8_t  prescaler;
    uint32_t actual_baudrate;
    int16_t  error;
    uint8_t  temp_lcr;

    prescaler = (ReadRegister(SC16IS750_REG_MCR)&0x80)? 4:1; 
    divisor   = (SC16IS750_CRYSTCAL_FREQ/prescaler)/(baudrate*16);
    temp_lcr  = ReadRegister(SC16IS750_REG_LCR) | 0x80;
    WriteRegister(SC16IS750_REG_LCR,temp_lcr);

    //write to DLL
    WriteRegister(SC16IS750_REG_DLL,(uint8_t)divisor);

    //write to DLH
    WriteRegister(SC16IS750_REG_DLH,(uint8_t)(divisor>>8));

    temp_lcr &= 0x7F;
    WriteRegister(SC16IS750_REG_LCR,temp_lcr);

    actual_baudrate = (SC16IS750_CRYSTCAL_FREQ/prescaler)/(16*divisor);
    error = ((float)actual_baudrate-baudrate)*1000/baudrate;

    return error;
}

void SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length )
{
    uint8_t temp_lcr = ReadRegister(SC16IS750_REG_LCR) & 0xC0; /Clear the lower six bit of LCR (LCR[0] to LCR[5]
    switch (data_length) {            //data length settings
        case 6:
            temp_lcr |= 0x01;
            break;
        case 7:
            temp_lcr |= 0x02;
            break;
        default:
        case 8:
            temp_lcr |= 0x03;
        case 5:
            break;
    }

    if ( stop_length == 2 ) 
        temp_lcr |= 0x04;

    switch (parity_select) {            //parity selection length settings
        case 1:                         //odd parity
            temp_lcr |= 0x08;
            break;
        case 2:                         //even parity
            temp_lcr |= 0x18;
            break;
        case 3:                         //force '1' parity
            temp_lcr |= 0x03;
        case 0:                         //no parity
        case 4:                         //force '0' parity
        default:
            break;
        }
    WriteRegister(SC16IS750_REG_LCR,temp_lcr);
}


void EnableTransmit(uint8_t tx_enable)
{
    uint8_t temp_efcr;
    temp_efcr = ReadRegister(SC16IS750_REG_EFCR);
    if ( tx_enable == 0) 
        temp_efcr |= 0x04;
    else 
        temp_efcr &= 0xFB;
    WriteRegister(SC16IS750_REG_EFCR,temp_efcr);
}

void SetPinInterrupt(uint8_t io_int_ena)
{
    WriteRegister(SC16IS750_REG_IOINTENA, io_int_ena);
}

void InterruptControl(uint8_t int_ena)
{
    WriteRegister(SC16IS750_REG_IER, int_ena);
}

uint8_t InterruptPendingTest(void)
{
    return (ReadRegister(SC16IS750_REG_IIR) & 0x01);
}

void __isr(void)
{
    uint8_t irq_src = ReadRegister(SC16IS750_REG_IIR) >> 1;
    irq_src &= 0x3F;

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
    uint8_t temp_fcr = ReadRegister(SC16IS750_REG_FCR);
    temp_fcr |= (rx_fifo==0)? 0x04:0x02;
    WriteRegister(SC16IS750_REG_FCR,temp_fcr);
}

void FIFOEnable(uint8_t fifo_enable)
{
    uint8_t temp_fcr=ReadRegister(SC16IS750_REG_FCR);

    if (fifo_enable == 0)
        temp_fcr &= 0xFE;
    else 
        temp_fcr |= 0x01;
    WriteRegister(SC16IS750_REG_FCR,temp_fcr);
}

void FIFOSetTriggerLevel(uint8_t rx_fifo, uint8_t length)
{
    uint8_t temp_reg;

    temp_reg = ReadRegister(SC16IS750_REG_MCR);
    temp_reg |= 0x04;
    WriteRegister(SC16IS750_REG_MCR,temp_reg);       //SET MCR[2] to '1', use TLR register in FCR register

    temp_reg = ReadRegister(SC16IS750_REG_EFR);
    WriteRegister(SC16IS750_REG_EFR, temp_reg|0x10); //set ERF[4] to '1' to use enhanced features
    if (rx_fifo == 0) 
        length <<= 4;
    WriteRegister(SC16IS750_REG_TLR, length);        //Rx FIFO Trigger level setting
    WriteRegister(SC16IS750_REG_EFR, temp_reg);      //restore EFR register
}

int readable(void)
{
    return FIFO_readable();
}

uint8_t FIFO_readable(void)
{
   return ReadRegister(SC16IS750_REG_RXLVL);
}

uint8_t FIFO_writeable(void)
{
   return ReadRegister(SC16IS750_REG_TXLVL);
}

int sc16is750_getc(void)
{
    volatile uint8_t val = -1;
    if (FIFO_readable() != 0) 
        val = ReadRegister(SC16IS750_REG_RHR);
    return val;
}

size_t sc16is750_putc(uint8_t val)
{
    uint8_t tmp_lsr;
    do {
        tmp_lsr = ReadRegister(SC16IS750_REG_LSR);
        } 
    while ((tmp_lsr&0x20) ==0);
    WriteRegister(SC16IS750_REG_THR,val);
}

void sc16is750_flush()
{
    uint8_t tmp_lsr;

    do {
        tmp_lsr = ReadRegister(SC16IS750_REG_LSR);
        } 
    while ((tmp_lsr&0x20) == 0);
}


//
// GPIO control functions...
//

void ModemPin(uint8_t gpio) //gpio == 0, gpio[7:4] are modem pins, gpio == 1 gpio[7:4] are gpios
{
    uint8_t temp_iocontrol = ReadRegister(SC16IS750_REG_IOCONTROL);
    if ( gpio == 0 ) 
        temp_iocontrol |= 0x02;
    else 
        temp_iocontrol &= 0xFD;
    WriteRegister(SC16IS750_REG_IOCONTROL, temp_iocontrol);
}

void GPIOSetPinMode(uint8_t pin_number, uint8_t i_o)
{
    uint8_t temp_iodir = ReadRegister(SC16IS750_REG_IODIR);
    if ( i_o == OUTPUT ) 
        temp_iodir |= (0x01 << pin_number);
    else
        temp_iodir &= (uint8_t)~(0x01 << pin_number);
    WriteRegister(SC16IS750_REG_IODIR, temp_iodir);
}

void GPIOSetPinState(uint8_t pin_number, uint8_t pin_state)
{
    uint8_t temp_iostate = ReadRegister(SC16IS750_REG_IOSTATE);
    if ( pin_state == 1 ) 
      temp_iostate |= (0x01 << pin_number);
    else 
      temp_iostate &= (uint8_t)~(0x01 << pin_number);
    WriteRegister(SC16IS750_REG_IOSTATE, temp_iostate);
}


uint8_t GPIOGetPinState(uint8_t pin_number)
{
    uint8_t temp_iostate = ReadRegister(SC16IS750_REG_IOSTATE);
    return (temp_iostate & (0x01 << pin_number));
}

uint8_t GPIOGetPortState(void)
{
    return ReadRegister(SC16IS750_REG_IOSTATE);
}

void GPIOSetPortMode(uint8_t port_io)
{
    WriteRegister(SC16IS750_REG_IODIR, port_io);
}

void GPIOSetPortState(uint8_t port_state)
{
    WriteRegister(SC16IS750_REG_IOSTATE, port_state);
}

void pinMode(uint8_t pin, uint8_t i_o)
{
    GPIOSetPinMode(pin, i_o);
}

void GPIOLatch(uint8_t latch)
{
    uint8_t temp_iocontrol = ReadRegister(SC16IS750_REG_IOCONTROL);
    if ( latch == 0 ) 
        temp_iocontrol &= 0xFE;
    else 
        temp_iocontrol |= 0x01;
    WriteRegister(SC16IS750_REG_IOCONTROL, temp_iocontrol);
}

//
// Low-level read/write functions
//

uint8_t platform_ReadRegister(uint8_t reg_addr)
{
    uint8_t result, addr=0x80|(reg_addr<<3);
    if ( protocol == SC16IS750_PROTOCOL_I2C ) 
        // read from reg_address if using i2c bus here
    else if (protocol == SC16IS750_PROTOCOL_SPI) {                                   //register read operation via SPI
        gpio_write( csPin,  GPIO_LEVEL_LOW );
        spi_transfer(myspi, (uint8_t*)&addr, (uint32_t)1, (uint8_t*)&result, (uint32_t)1);
        gpio_write( csPin,  GPIO_LEVEL_LOW );
        }
    return result;
}

void platform_WriteRegister(uint8_t reg_addr, uint8_t val)
{
    if ( protocol == SC16IS750_PROTOCOL_I2C ) 
        // send reg_addr and val via i2c bus here
    else {
        uint8_t buff[2];
        buff[0] = reg_addr<<3;
        buff[1] = val;

        gpio_write( csPin,  GPIO_LEVEL_LOW );
        spi_transfer(myspi, buff, (uint32_t)2, NULL, (uint32_t)0);
        gpio_write( csPin,  GPIO_LEVEL_LOW );
    }
    return ;
}

#define MICROBUS_RST
#define MICROBUS_CS
#define MICROBUS_INT

spi_handle_t  myspi = (spi_handle_t)0;
gpio_handle_t rstPin; 
gpio_handle_t csPin; 
gpio_handle_t intPin; 

platform_init()
{
    spi_bus_init(SPI_BUS_II, &myspi);
    spi_format(myspi, SPIMODE_CPOL_0_CPHA_0, SPI_BPW_8);
    spi_frequency(myspi, 960000);

    gpio_init(MICROBUS_RST, &rstPin);
    gpio_dir(rstPin, GPIO_DIR_OUTPUT);
    gpio_write(rstPin,  GPIO_LEVEL_HIGH );       

    gpio_init(MICROBUS_CS, &csPin);
    gpio_dir(csPin, GPIO_DIR_OUTPUT);
    gpio_write(csPin,  GPIO_LEVEL_HIGH );         

    gpio_init(MICROBUS_INT, &intPin);
    gpio_dir(intPin, GPIO_DIR_INPUT);
}

