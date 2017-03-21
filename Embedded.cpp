#include <stdio.h>
#include <stdlib.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <linux/spi/spidev.h>

//#include "gz_clk.h"
//#include "gpio-sysfs.h"

int a0;
int a1;
int a2;
int b0;
int b1;
int b2;

int col_num;
int row_num;

static const char *device = "/dev/spidev0.0";
static uint8_t mode = SPI_CPHA | SPI_CPOL;;
static uint8_t bits = 8;
static uint32_t speed = 50000;
static uint16_t delay = 10;
static int drdy_GPIO = 22;


/*static void writeReset(int fd)
{
  int ret;
  uint8_t tx1[5] = {0xff,0xff,0xff,0xff,0xff};
  uint8_t rx1[5] = {0};
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx1;
  tr.rx_buf = (unsigned long)rx1;
  tr.len = ARRAY_SIZE(tx1);
  tr.delay_usecs = delay;
  tr.speed_hz = speed;
  tr.bits_per_word = bits;
  
  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");
}


static void writeReg(int fd, uint8_t v)
{
  int ret;
  uint8_t tx1[1];
  tx1[0] = v;
  uint8_t rx1[1] = {0};
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx1;
  tr.rx_buf = (unsigned long)rx1;
  tr.len = ARRAY_SIZE(tx1);
  tr.delay_usecs = delay;
  tr.speed_hz = speed;
  tr.bits_per_word = bits;

  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");
}


static uint8_t readReg(int fd)
{
	int ret;
	uint8_t tx1[1];
	tx1[0] = 0;
	uint8_t rx1[1] = {0};
	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = ARRAY_SIZE(tx1);
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = 8;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	  pabort("can't send spi message");
	  
	return rx1[0];
}


static int readData(int fd)
{
	int ret;
	uint8_t tx1[2] = {0,0};
	uint8_t rx1[2] = {0,0};
	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx1;
	tr.rx_buf = (unsigned long)rx1;
	tr.len = ARRAY_SIZE(tx1);
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = 8;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	  pabort("can't send spi message");
	  
	return (rx1[0]<<8)|(rx1[1]);
}*/


void mux(int row, int col)
{
	a0 = col & 0x01;
	a1 = (col & 0x02) >> 1;
	a2 = (col & 0x04) >> 2;

	b0 = row & 0x01;
	b1 = (row & 0x02) >> 1;
	b2 = (row & 0x04) >> 2;

	//printf("b2: %d  b1: %d  b0: %d  a2: %d  a1: %d  a0: %d\n", b2, b1, b0, a2, a1, a0);
}


int main()
{

//int ret = 0;
//int fd;
//fd = open(device, O_RDWR);

//ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
//ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
//ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
//ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
//ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
//ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);

// resets the AD7705 so that it expects a write to the communication register
//writeReset(fd);

// tell the AD7705 that the next write will be to the clock register
//writeReg(fd,0x20);
// write 00001100 : CLOCKDIV=1,CLK=1,expects 4.9152MHz input clock
//writeReg(fd,0x0C);

// tell the AD7705 that the next write will be the setup register
//writeReg(fd,0x10);
// intiates a self calibration and then after that starts converting
//writeReg(fd,0x40);

while(1)
{
	for (row_num = 0; row_num < 6; row_num++) {

		for (col_num = 0; col_num < 5; col_num++) {
			
			mux(row_num, col_num);
			// tell the AD7705 to read the data register (16 bits)
	  		//writeReg(fd,0x38);
	  		// read the data register by performing two 8 bit reads
	  		//int value = readData(fd)-0x8000;
		}
	}
}
}
