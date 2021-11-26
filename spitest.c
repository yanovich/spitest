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
	unsigned char buf[32], *bp;
	int len, status;

	name = argv[1];
	fd = open(name, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	memset(xfer, 0, sizeof xfer);
	memset(buf, 0, sizeof buf);
	len = sizeof buf;

	/*
	 * Send a GetID command
	 */
	buf[0] = 0x9f;
	len = 6;
	xfer[0].tx_buf = (unsigned long)buf;
	xfer[0].len = 1;

	xfer[1].rx_buf = (unsigned long) buf;
	xfer[1].len = 6;

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {
		perror("SPI_IOC_MESSAGE");
		return -1;
	}

	printf("response(%d): ", status);
	for (bp = buf; len; len--)
		printf("%02x ", *bp++);
	printf("\n");

	return 0;
}
