
/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/un.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include <hwlib/hwlib.h>

#include "max30102.h"
#include "algorithm_by_RF.h"

#define PROXIMITY_THRESHOLD  32000
#define delay(x)             (usleep(x*1000))   //macro to provide ms pauses

i2c_handle_t  i2c_handle = (i2c_handle_t)NULL;

#define INTPIN_SOCKET1    GPIO_PIN_94
#define INTPIN_SOCKET2    GPIO_PIN_7

gpio_handle_t intPin; 

void init(void) {
    gpio_init(INTPIN_SOCKET2, &intPin);
    gpio_dir(intPin, GPIO_DIR_INPUT);
    i2c_bus_init(I2C_BUS_I, &i2c_handle);
    }

int read_i2c( uint8_t addr, uint16_t count, uint8_t* ptr ) 
{
    int err=i2c_write(i2c_handle, MAX30101_SAD, &addr, 1, I2C_NO_STOP);
    if( !err )
        err =i2c_read(i2c_handle, MAX30101_SAD, ptr, count);
    return err;
}

void write_i2c( uint8_t addr, uint16_t count, uint8_t* ptr)
{
    uint8_t* buff = malloc(count+1);

    if( buff == NULL )
        return;

    buff[0] = addr;
    memcpy(&buff[1], ptr, count);

    i2c_write(i2c_handle, MAX30101_SAD, buff, count+1, I2C_STOP);

    free(buff);
}

void usage (void)
{
    printf(" The 'heartrate_demo' program can be started with several options:\n");
    printf(" -r X: Set the run-time for 'X' seconds\n");
    printf(" -?  : Display usage info\n");
}

int main(int argc, char *argv[]) 
{
    float    n_spo2, ratio, correl;                                       //SPO2 value
    int8_t   ch_spo2_valid;                                               //indicator to show if the SPO2 calculation is valid
    int32_t  n_heart_rate;                                                //heart rate value
    int8_t   ch_hr_valid;                                                 //indicator to show if the heart rate calculation is valid
    uint32_t aun_ir_buffer[BUFFER_SIZE];                                  //infrared LED sensor data
    uint32_t aun_red_buffer[BUFFER_SIZE];                                 //red LED sensor data
    int32_t  i;
    int32_t  average_hr;
    float    average_spo2;
    int32_t  nbr_readings;

    int            run_time = 30;  //default to 30 second run time
    gpio_level_t   intVal;
    struct timeval time_start, time_now;

    while((i=getopt(argc,argv,"tr:?")) != -1 )

        switch(i) {
           case 'r':
               sscanf(optarg,"%x",&run_time);
               printf(">> run-time set to %d seconds ",run_time);
               break;
           case '?':
               usage();
               exit(EXIT_SUCCESS);
           default:
               fprintf (stderr, ">> unknown option character `\\x%x'.\n", optopt);
               exit(EXIT_FAILURE);
           }

    printf("\n\n");
    printf("     ****\r\n");
    printf("    **  **     SW reuse using C example\r\n");
    printf("   **    **    for the Heart Rate Click\r\n");
    printf("  ** ==== **\r\n");
    printf("\r\n");

    init();
    maxim_max30102_i2c_setup( read_i2c, write_i2c);
    maxim_max30102_init();

    printf("\nHeartRate Click Revision: 0x%02X\n", max30102_get_revision());
    printf("HeartRate Click Part ID:  0x%02X\n\n", max30102_get_part_id());
    printf("Begin ... Place your finger on the sensor\n\n");

    gettimeofday(&time_start, NULL);
    time_now = time_start;
    average_hr = nbr_readings = 0;
    average_spo2 = 0.0;

    while( difftime(time_now.tv_sec, time_start.tv_sec) < run_time ) {
        //buffer length of BUFFER_SIZE stores ST seconds of samples running at FS sps
        //read BUFFER_SIZE samples, and determine the signal range
        for(i=0;i<BUFFER_SIZE;i++) {
            do {
                gpio_read(intPin,&intVal);
                }
            while( intVal == GPIO_LEVEL_HIGH  );                               //wait until the interrupt pin asserts
            maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));   //read from MAX30102 FIFO
            }

        //calculate heart rate and SpO2 after BUFFER_SIZE samples (ST seconds of samples) using Robert's method
        rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer, &n_spo2, 
                   &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl); 

        if(ch_hr_valid && ch_spo2_valid) {
             printf("Blood Oxygen Level (SpO2)=%.2f%% [normal is 95-100%%], Heart Rate=%d BPM [normal resting for adults is 60-100 BPM]\n",n_spo2, n_heart_rate);
             average_hr += n_heart_rate;
             average_spo2 += n_spo2;
             nbr_readings++;
             }

        gettimeofday(&time_now, NULL);
        }

    printf("\n\nAverage Blood Oxygen Level = %.2f%%\n",average_spo2/nbr_readings);
    printf("        Average Heart Rate = %d BPM\n",average_hr/nbr_readings);
    max301024_shut_down(1);
    gpio_deinit(&intPin);
    i2c_bus_deinit(&i2c_handle);
    printf("\r \nDONE...\n");
    exit(EXIT_SUCCESS);
}

