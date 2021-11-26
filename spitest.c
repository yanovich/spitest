#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int spi_exec (int fd, unsigned char *inbuf, int inlen, unsigned char *outbuf, int outlen) {
	struct spi_ioc_transfer xfer[2];
	memset(xfer, 0, sizeof xfer);
	memset(outbuf, 0, outlen);
	xfer[0].tx_buf = (unsigned long) inbuf;
	xfer[0].len = inlen;

	xfer[1].rx_buf = (unsigned long) outbuf;
	xfer[1].len = outlen;
	return ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
}

void print_bufs (unsigned char *inbuf, int inlen, unsigned char *outbuf, int outlen, int status) {
	unsigned char *bp;
	printf("request: ");
	for (bp = inbuf; inlen; inlen--)
		printf("%02x ", *bp++);
	printf("\n");
	printf("response(%d): ", status);
	for (bp = outbuf; outlen; outlen--)
		printf("%02x ", *bp++);
	printf("\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	char *name;
	int fd;
	unsigned char inbuf[32], outbuf[32];
	int status;
	unsigned int speed_hz = 2000000;

	name = argv[1];
	fd = open(name, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	status = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz);
	if (status < 0) {
		perror("SPI_IOC_WR_MAX_SPEED_HZ");
		return -1;
	}

	/*
	 * Send a GetID command
	 */
	memset(inbuf, 0, sizeof inbuf);
	inbuf[0] = 0x06;
	status = spi_exec(fd, inbuf, 1, outbuf, 0);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return -1;
	}

	/*
	 * Send a GetID command
	 */
	inbuf[0] = 0x11;
	inbuf[1] = 0x60;
	status = spi_exec(fd, inbuf, 2, outbuf, 0);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return -1;
	}

	return 0;
}
