#include "adcreader.h"
#include <Qdebug>

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


static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void writeReset(int fd)
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

static void writeReg(int fd, uint8_t v)
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

static uint8_t readReg(int fd)
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

static int readData(int fd)
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


void mux(int sel)
{

	int a0, a1, a2, a3, a4;

	a0 = sel & 0x01;
	a1 = (sel & 0x02) >> 1;
	a2 = (sel & 0x04) >> 2;
	a3 = (sel & 0x08) >> 3;
	a4 = (sel & 0x10) >> 4;

	gpio_set_value(s1,a0);
	gpio_set_value(s2,a1);
	gpio_set_value(s3,a2);
	gpio_set_value(s4,a3);
	gpio_set_value(s5,a4);
}


int main(int argc, char *argv[])
{

	static const char *device = "/dev/spidev0.0";
	static uint8_t mode = SPI_CPHA | SPI_CPOL;;
	static uint8_t bits = 8;
	static int drdy_GPIO = 22;

	static int s1 = 2;
	static int s2 = 3;
	static int s3 = 17;
	static int s4 = 6;
	static int s5 = 13;


	int ret = 0;
	int fd;
	int sysfs_fd;

	int s1_fd;
	int s2_fd;
	int s3_fd;
	int s4_fd;
	int s5_fd;

	int no_tty = !isatty( fileno(stdout) );

	int col;
	int row;
	int sel_num = 0;

	int t = 0;

	uint16_t value[5][6];

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

	// enables sysfs entry for the GPIO pin

	gpio_export(drdy_GPIO);
	gpio_export(s1);
	gpio_export(s2);
	gpio_export(s3);
	gpio_export(s4);
	gpio_export(s5);

	// set to input
	gpio_set_dir(drdy_GPIO,0);
	gpio_set_dir(s1,1);
	gpio_set_dir(s2,1);
	gpio_set_dir(s3,1);
	gpio_set_dir(s4,1);
	gpio_set_dir(s5,1);

	// set interrupt detection to falling edge
	gpio_set_edge(drdy_GPIO,"falling");

	// get a file descriptor for the GPIO pin
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

	// we read data in an endless loop and display it
	// this needs to run in a thread ideally
	while (1) {

		col = 0;
		row = 0;

		for (sel_num = 0; sel_num < 30; sel_num++) {
		          mux(sel_num);


		          // let's wait for data for max one second
		          ret = gpio_poll(sysfs_fd,1000);
		          if (ret<1) {
		            fprintf(stderr,"Poll error %d\n",ret);
			  }

		          // tell the AD7705 to read the data register (16 bits)
		          writeReg(fd,0x38);
		          // read the data register by performing two 8 bit reads
			  int col2 = col - 1;
			  if (col2 < 0){
				col2 = 5;
			  }
		          value[row][col2] = readData(fd)-0x8000;
			  //if (row != 1){
			//	value[row][col] = 0;
			 // }
		          if( no_tty )
		          {
      	                    printf("%d\n", value);
		            fflush(stdout);
	        	  }
			  col++;

			  if (col > 5) {
			    col = 0;
			    row++;
			  }

			  for (t = 0; t < 1000000; t++) {
			  }

		}



		for (int row_num = 0; row_num < 5; row_num++) {

			for (int col_num = 0; col_num < 6; col_num++) {
				value[row_num][col_num] = (value[row_num][col_num] - 1000) / 50.78125;
				if (value[row_num][col_num] > 255) {
					value[row_num][col_num] = 255;
				}
				printf("%u ", value[row_num][col_num]);
			}
			printf("\n");
		}
		printf("\n");

		//Print out the matrix of values
		//for (int row_num = 0; row_num < 5; row_num++) {

		//	for (int col_num = 0; col_num < 6; col_num++) {
		//		printf("%u ", value[row_num][col_num]);
		//	}
		//		printf("\n");
		//}
		//printf("\n");

	}

	close(fd);
	gpio_fd_close(sysfs_fd);
	gpio_fd_close(s1_fd);
	gpio_fd_close(s2_fd);
	gpio_fd_close(s3_fd);
	gpio_fd_close(s4_fd);
	gpio_fd_close(s5_fd);

	return ret;
}
