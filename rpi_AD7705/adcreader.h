#ifndef ADCREADER
#define ADCREADER

#include <QThread>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "gz_clk.h"
#include "gpio-sysfs.h"


class ADCreader : public QThread
{
	public:
		ADCreader();


	public:
		//thread functions
		void quit();
		void run();


	protected:
		void writeReset(int fd);
		void writeReg(int fd, uint8_t v);
		uint8_t readReg(int fd);
		int readData(int fd);


	private:
		bool running;
		void pabort(const char *s);


		//spi file descriptor
		int fd;


		//interrupt file descriptor
		int sysfs_fd;


		//select line file descriptors
		int s1_fd;
		int s2_fd;
		int s3_fd;
		int s4_fd;
		int s5_fd;

		//declarations
		uint8_t mode;
		uint8_t bits;
		uint32_t speed;
		uint16_t delay;

		//GPIO pins
		int drdy_GPIO;
		int s1;
		int s2;
		int s3;
		int s4;
		int s5;

};

#endif
