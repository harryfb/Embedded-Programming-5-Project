#include "adcreader.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "gz_clk.h"
#include "gpio-sysfs.h"
#include <assert.h>

#include <QDebug>

uint16_t ADCValue[5][6] = {0};

//ADCReader Constructor
ADCreader::ADCreader()
{
    static const char *device = "/dev/spidev0.0";
    mode = SPI_CPHA | SPI_CPOL;;
    bits = 8;
    drdy_GPIO = 22;
	
    //Multiplexer Select Lines
    s1 = 2;
    s2 = 3;
    s3 = 17;
    s4 = 6;
    s5 = 13;


    int ret = 0;

    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    fprintf(stderr, "spi mode: %d\n", mode);
    fprintf(stderr, "bits per word: %d\n", bits);

    // enable master clock for the AD
    // divisor results in roughly 4.9MHz
    // this also inits the general purpose IO
    gz_clock_ena(GZ_CLK_5MHz,5);

    // enables sysfs entry for the GPIO pin and Multiplexer Select Lines
    gpio_export(drdy_GPIO);
    gpio_export(s1);
    gpio_export(s2);
    gpio_export(s3);
    gpio_export(s4);
    gpio_export(s5);

    // set drdy to input & select pins to output
    gpio_set_dir(drdy_GPIO,0);
    gpio_set_dir(s1,1);
    gpio_set_dir(s2,1);
    gpio_set_dir(s3,1);
    gpio_set_dir(s4,1);
    gpio_set_dir(s5,1);

    // set interrupt detection to falling edge
    gpio_set_edge(drdy_GPIO,"falling");

    // get a file descriptor for the GPIO pin and MUX Select Lines
    sysfs_fd = gpio_fd_open(drdy_GPIO);
    s1_fd = gpio_fd_open(s1);
    s2_fd = gpio_fd_open(s2);
    s3_fd = gpio_fd_open(s3);
    s4_fd = gpio_fd_open(s4);
    s5_fd = gpio_fd_open(s5);


    // resets the AD7705 so that it expects a write to the communication register
    printf("sending reset\n");
    writeReset(fd);

    // tell the AD7705 that the next write will be to the clock register
    writeReg(fd,0x20);

    // write 00001111 : CLOCKDIV=1,CLK=1,expects 4.9152MHz input clock, FS0=1, FS1=1
    writeReg(fd,0x0F);

    // tell the AD7705 that the next write will be the setup register
    writeReg(fd,0x10);
    // intiates a self calibration and then after that starts converting
    writeReg(fd,0x40);

}


void ADCreader::pabort(const char *s)
{
    perror(s);
    abort();
}


void ADCreader::writeReset(int fd)
{
  int ret;
  uint8_t tx1[5] = {0xff,0xff,0xff,0xff,0xff};
  uint8_t rx1[5] = {0};
  struct spi_ioc_transfer tr;

  memset(&tr,0,sizeof(struct spi_ioc_transfer));
  tr.tx_buf = (unsigned long)tx1;
  tr.rx_buf = (unsigned long)rx1;
  tr.len = sizeof(tx1);

  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1) {
    printf("\nerr=%d when trying to reset. \n",ret);
    pabort("Can't send spi message");
  }
}

void ADCreader::writeReg(int fd, uint8_t v)
{
  int ret;
  uint8_t tx1[1];
  tx1[0] = v;
  uint8_t rx1[1] = {0};
  struct spi_ioc_transfer tr;

  memset(&tr,0,sizeof(struct spi_ioc_transfer));
  tr.tx_buf = (unsigned long)tx1;
  tr.rx_buf = (unsigned long)rx1;
  tr.len = sizeof(tx1);

  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");
}

uint8_t ADCreader::readReg(int fd)
{
    int ret;
    uint8_t tx1[1];
    tx1[0] = 0;
    uint8_t rx1[1] = {0};
    struct spi_ioc_transfer tr;

    memset(&tr,0,sizeof(struct spi_ioc_transfer));
    tr.tx_buf = (unsigned long)tx1;
    tr.rx_buf = (unsigned long)rx1;
    tr.len = sizeof(tx1);

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
      pabort("can't send spi message");

    return rx1[0];
}

int ADCreader::readData(int fd)
{
    int ret;
    uint8_t tx1[2] = {0,0};
    uint8_t rx1[2] = {0,0};
    struct spi_ioc_transfer tr;

    memset(&tr,0,sizeof(struct spi_ioc_transfer));
    tr.tx_buf = (unsigned long)tx1;
    tr.rx_buf = (unsigned long)rx1;
    tr.len = sizeof(tx1);

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
          {
      printf("\n can't send spi message, ret = %d\n",ret);
          exit(1);
          }

    return (rx1[0]<<8)|(rx1[1]);
}

//This function selects the correct multiplexer lines for the next pixel to be read into the ADC
void ADCreader::mux(int sel)
{

    int a0, a1, a2, a3, a4;

    //Split sel into 5 output bits
    a0 = sel & 0x01;
    a1 = (sel & 0x02) >> 1;
    a2 = (sel & 0x04) >> 2;
    a3 = (sel & 0x08) >> 3;
    a4 = (sel & 0x10) >> 4;

    //Assign bit values to select pins
    gpio_set_value(s1,a0);
    gpio_set_value(s2,a1);
    gpio_set_value(s3,a2);
    gpio_set_value(s4,a3);
    gpio_set_value(s5,a4);
}

//The ADCReader Thread started by adcreader->start(); in the window.cpp file
void ADCreader::run()
{
    running = true;

    while (running) {

        int col = 0;
        int row = 0;
	uint16_t rawvalue[5][6];
	uint16_t value[5][6];
	int sel_num;
	int ret;

        for (sel_num = 0; sel_num < 30; sel_num++) {
                  mux(sel_num); //Select the correct pixel
		  msleep(10); //wait 10ms

                  // let's wait for data for max one second
                  ret = gpio_poll(sysfs_fd,1000);
                  if (ret<1) {
                    fprintf(stderr,"Poll error %d\n",ret);
              }

                  // tell the AD7705 to read the data register (16 bits)
                  writeReg(fd,0x38);

                  int col2 = col;
	          int row2 = row;

                  // read the data register by performing two 8 bit reads
                  rawvalue[row2][col2] = readData(fd)-0x8000;
                  value[row2][col2] = (rawvalue[row2][col2] - 1000) / 50.78125;

                  if (value[row2][col2] > 255) { //Make sure the greyscale value cannot be greater than 255
                      value[row2][col2] = 255;
                  }
		  //Store the sample from the ADC into the ADCValue Matrix
                  ADCValue[row2][col2] = value[row2][col2];

              col++;
              if (col > 5) {
                col = 0;
                row++;
              }
        }
    }

    //Close gpio
    close(fd);
    gpio_fd_close(sysfs_fd);
    gpio_fd_close(s1_fd);
    gpio_fd_close(s2_fd);
    gpio_fd_close(s3_fd);
    gpio_fd_close(s4_fd);
    gpio_fd_close(s5_fd);

}
//Stop the ADCReader thread
void ADCreader::quit()
{
    running = false;
    exit(0);
}
//Get the ADC Sample for the pixel indicated by valueRow and valueCol
int ADCreader::getSample(int valueRow, int valueCol)
{
  return ADCValue[valueRow][valueCol];
}
