#include <pru/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_virtqueue.h>
#include <pru_rpmsg.h>

#define _ENABLE_RPMSG 1

#if _ENABLE_RPMSG == 1
	#ifdef __AM335X_PRU0__
		#include "resource_table_0.h"
		#define HOST_INT	  ((uint32_t) 1 << 30)
		#define TO_ARM_HOST	  16
		#define FROM_ARM_HOST 17
		#define CHAN_NAME	  "rpmsg-pru"
		#define CHAN_DESC	  "Channel 30"
		#define CHAN_PORT	  30
	#else
		#include "resource_table_1.h"
		#define HOST_INT      ((uint32_t) 1 << 31)
		#define TO_ARM_HOST   18
		#define FROM_ARM_HOST 19   
		#define CHAN_NAME     "rpmsg-pru"
		#define CHAN_DESC     "Channel 31"
		#define CHAN_PORT     31  
	#endif

	#define VIRTIO_CONFIG_S_DRIVER_OK 4

	char payload[RPMSG_BUF_SIZE];

	static uint16_t src, dst, len; 
	static struct pru_rpmsg_transport transport;
	static volatile uint8_t *status;

	void init_rpmsg()
	{
		CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
		CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

		status = &resourceTable.rpmsg_vdev.status;
		while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

		/* Initialize the RPMsg transport structure */
		pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);

		/* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
		while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
		while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) != PRU_RPMSG_SUCCESS);
	}

	void send_rpmsg(int data)
	{
		memset(payload, '\0', RPMSG_BUF_SIZE);

		itoa(data, payload, 10);
		strcat(payload, "\n");

		pru_rpmsg_send(&transport, dst, src, payload, len);
	}

	int receive_rpmsg()
	{
		while (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) != PRU_RPMSG_SUCCESS);

		return atoi(payload);
	}
#else
	#include "resource_table.h"
#endif

int main()
{
	init_rpmsg();
	send_rpmsg(-111);

	int count = receive_rpmsg();
	// int count = 0;

	while (1) {
		send_rpmsg(count);
		
		count++;
		__delay_cycles(200000000);
	}

	return 0;
}
