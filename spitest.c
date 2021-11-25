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

int main(int argc, char **argv)
{
	char *name;
	int fd;
	struct spi_ioc_transfer xfer[2];
	unsigned char inbuf[32], outbuf[32], *bp;
	int len, status;
	unsigned int speed_hz = 2000000;

	name = argv[1];
	fd = open(name, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	memset(xfer, 0, sizeof xfer);
	memset(inbuf, 0, sizeof inbuf);
	memset(outbuf, 0, sizeof outbuf);

	/*
	 * Send a GetID command
	 */
	inbuf[0] = 0x9f;
	xfer[0].tx_buf = (unsigned long)inbuf;
	xfer[0].len = 1;

	xfer[1].rx_buf = (unsigned long) outbuf;
	xfer[1].len = 6;
	len = 6;

	status = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz);
	if (status < 0) {
		perror("SPI_IOC_WR_MAX_SPEED_HZ");
		return -1;
	}

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return -1;
	}

	printf("response(%d): ", status);
	for (bp = outbuf; len; len--)
		printf("%02x ", *bp++);
	printf("\n");

	return 0;
}
