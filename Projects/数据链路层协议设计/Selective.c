#include <stdio.h>
#include <string.h>

#include "datalink.h"
#include "protocol.h"

#define DATA_TIMER 3000
#define ACK_TIMER 300
#define MAX_SEQ 15

#define NR_BUFS ((MAX_SEQ + 1) / 2)
#define inc(x) x = (x + 1) % (MAX_SEQ + 1)

typedef enum
{
	false,
	true
} bool;
typedef unsigned char seq_nr;
typedef unsigned char packet[PKT_LEN];
typedef struct
{
	unsigned char kind;
	seq_nr ack;
	seq_nr seq;
	packet data;
	unsigned int crc;
} frame;

/*
	DATA Frame
	+=========+========+========+===============+========+
	| KIND(1) | ACK(1) | SEQ(1) | DATA(240~256) | CRC(4) |
	+=========+========+========+===============+========+

	ACK Frame
	+=========+========+========+
	| KIND(1) | ACK(1) | CRC(4) |
	+=========+========+========+

	NAK Frame
	+=========+========+========+
	| KIND(1) | ACK(1) | CRC(4) |
	+=========+========+========+
*/

static int phl_ready = 0;
bool no_nak = true;

static int between(seq_nr a, seq_nr b, seq_nr c)
{
	return ((a <= b && b < c) || (c < a && a <= b) || (b < c && c < a));
}

static void put_frame(unsigned char *frame, int len)
{
	*(unsigned int *)(frame + len) = crc32(frame, len);
	send_frame(frame, len + 4);
	phl_ready = 0;
}

static void send_data(unsigned char fk, seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{
	frame s;
	s.kind = fk;
	s.seq = frame_nr;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	if (fk == FRAME_DATA)
	{
		memcpy(s.data, buffer[frame_nr % NR_BUFS], PKT_LEN);
		put_frame((unsigned char *)&s, 3 + PKT_LEN);
		start_timer(frame_nr % NR_BUFS, DATA_TIMER);
		dbg_frame("Send DATA %d %d, ID: %d\n", s.seq, s.ack, *(short *)&(s.data));
	}
	if (fk == FRAME_NAK)
	{
		put_frame((unsigned char *)&s, 2);
		no_nak = false;
		dbg_frame("Send NAK %d\n", s.ack);
	}
	if (fk == FRAME_ACK)
	{
		put_frame((unsigned char *)&s, 2);
		dbg_frame("Send ACK %d\n", s.ack);
	}
	stop_ack_timer();
}

int main(int argc, char **argv)
{
	seq_nr next_frame_to_send = 0,
		   ack_expected = 0,
		   frame_expected = 0,
		   too_far = NR_BUFS,
		   nbuffered = 0,
		   i;

	packet out_buf[NR_BUFS],
		in_buf[NR_BUFS];

	bool arrived[NR_BUFS];

	int event, arg, len = 0;

	frame r;

	for (i = 0; i < NR_BUFS; i++)
		arrived[i] = false;

	protocol_init(argc, argv);
	enable_network_layer();

	for (;;)
	{
		event = wait_for_event(&arg);

		switch (event)
		{
		case NETWORK_LAYER_READY:
			get_packet(out_buf[next_frame_to_send % NR_BUFS]);
			nbuffered++;
			send_data(FRAME_DATA, next_frame_to_send, frame_expected, out_buf);
			inc(next_frame_to_send);
			break;

		case PHYSICAL_LAYER_READY:
			phl_ready = 1;
			break;

		case FRAME_RECEIVED:
			len = recv_frame((unsigned char *)&r, sizeof r);
			if (len < 5 || crc32((unsigned char *)&r, len) != 0)
			{
				if (no_nak)
					send_data(FRAME_NAK, 0, frame_expected, out_buf);
				dbg_event("**** Receiver Error, Bad CRC ******\n");
				break;
			}
			if (r.kind == FRAME_DATA)
			{
				if ((r.seq != frame_expected) && no_nak)
					send_data(FRAME_NAK, 0, frame_expected, out_buf);
				else
					start_ack_timer(ACK_TIMER);
				if (between(frame_expected, r.seq, too_far) && (arrived[r.seq % NR_BUFS] == false))
				{
					arrived[r.seq % NR_BUFS] = true;
					memcpy(in_buf[r.seq % NR_BUFS], r.data, PKT_LEN);

					while (arrived[frame_expected % NR_BUFS])
					{
						put_packet(in_buf[frame_expected % NR_BUFS], len - 7);
						no_nak = true;
						arrived[frame_expected % NR_BUFS] = false;
						inc(frame_expected);
						inc(too_far);
						start_ack_timer(ACK_TIMER);
					}
					dbg_frame("Recv DATA %d %d, ID: %d \n", r.seq, r.ack, *(short *)&(r.data));
				}
			}
			else if ((r.kind == FRAME_NAK) && between(ack_expected, (r.ack + 1) % (MAX_SEQ + 1), next_frame_to_send))
			{
				send_data(FRAME_DATA, (r.ack + 1) % (MAX_SEQ + 1), frame_expected, out_buf);
				dbg_frame("Recv NAK %d\n", r.ack);
			}

			while (between(ack_expected, r.ack, next_frame_to_send))
			{
				stop_timer(ack_expected % NR_BUFS);
				nbuffered--;
				inc(ack_expected);
			}
			break;

		case DATA_TIMEOUT:
			send_data(FRAME_DATA, ack_expected, frame_expected, out_buf);
			dbg_event("******** DATA %d timeout **********\n", arg);
			break;

		case ACK_TIMEOUT:
			send_data(FRAME_ACK, 0, frame_expected, out_buf);
			dbg_event("******** ACK %d timeout *********\n", arg);
			break;
		}

		if (nbuffered < NR_BUFS && phl_ready)
			enable_network_layer();
		else
			disable_network_layer();
	}
}