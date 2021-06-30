#include <stdio.h>
#include <string.h>

#include "datalink.h"
#include "protocol.h"

#define DATA_TIMER 2000
#define ACK_TIMER 300
#define MAX_SEQ 7
#define inc(n) n = (n + 1) % (MAX_SEQ + 1)

typedef unsigned char seq_nr;
typedef unsigned char packet[PKT_LEN];
typedef struct
{
    seq_nr kind;
    seq_nr ack;
    seq_nr seq;
    packet data;
    unsigned int crc;
} FRAME;

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

seq_nr frame_nr = 0, ack_expected = 0;
seq_nr nbuffered = 0, frame_expected = 0;
packet buffer[MAX_SEQ + 1];

static int phl_ready = 0;
static int no_nak = 1;

/* 将frame做CRC校验后附校验码一起发送到物理层 */
static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *)(frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    phl_ready = 0;
}

static int between(unsigned char a, unsigned char b, unsigned char c)
{
    return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

static void send_data_frame(unsigned char frame_nr, unsigned char frame_expected, unsigned char buffer[MAX_SEQ + 1][PKT_LEN])
{
    FRAME s;
    s.kind = FRAME_DATA;
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
    memcpy(s.data, buffer[frame_nr], PKT_LEN);

    put_frame((unsigned char *)&s, 3 + PKT_LEN);
    dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
    start_timer(frame_nr, DATA_TIMER);

    stop_ack_timer();
}

/* 发送单个ACK帧 */
static void send_ack_frame()
{
    FRAME s;
    s.kind = FRAME_ACK;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    put_frame((unsigned char *)&s, 2);
    dbg_frame("Send ACK  %d\n", s.ack);

    stop_ack_timer();
}

/* 发送单个NAK帧 */
static void send_nak_frame()
{
    FRAME s;
    s.kind = FRAME_NAK;
    s.ack = frame_expected % (MAX_SEQ + 1);

    put_frame((unsigned char *)&s, 2);
    dbg_frame("Send NAK  %d\n", s.ack);
    no_nak = 0;

    stop_ack_timer();
}

int main(int argc, char **argv)
{
    int len = 0, arg, event;
    FRAME r;
    protocol_init(argc, argv);
    disable_network_layer(); /* 等待物理层 */

    for (;;)
    {
        event = wait_for_event(&arg);
        switch (event)
        {
        case NETWORK_LAYER_READY:
            nbuffered++;
            get_packet(buffer[frame_nr]);
            send_data_frame(frame_nr, frame_expected, buffer);
            inc(frame_nr);
            break;

        case PHYSICAL_LAYER_READY:
            phl_ready = 1;
            break;

        case FRAME_RECEIVED:
            len = recv_frame((unsigned char *)&r, sizeof r);

            /* 校验得出接收帧错误 */
            if (len < 5 || crc32((unsigned char *)&r, len) != 0)
            {
                if (no_nak && r.seq == frame_expected)
                    send_nak_frame();
                dbg_event("**** Receiver Error, Bad CRC Checksum\n");
                break;
            }

            if (r.kind == FRAME_DATA && r.seq == frame_expected)
            {
                dbg_frame("Recv DATA %d %d, ID %d\n", r.seq, r.ack, *(short *)r.data);
                put_packet(r.data, len - 7);
                start_ack_timer(ACK_TIMER);
                no_nak = 1;
                inc(frame_expected);
            }

            else if (r.kind == FRAME_ACK)
                dbg_frame("Recv ACK %d \n", r.ack);

            else if (r.kind == FRAME_NAK)
            {
                dbg_frame("Recv NAK %d\n", r.ack);
                frame_nr = r.ack;
                r.ack = (r.ack + MAX_SEQ) % (MAX_SEQ + 1);

                /* 先累积确认 */
                while (between(ack_expected, r.ack, frame_nr))
                {
                    nbuffered--;
                    stop_timer(ack_expected);
                    inc(ack_expected);
                }

                /* 然后重发 */
                for (int i = 0; i < nbuffered; i++)
                {
                    send_data_frame(frame_nr, frame_expected, buffer);
                    inc(frame_nr);
                }
                break;
            }

            /* 累积确认 */
            while (between(ack_expected, r.ack, frame_nr))
            {
                stop_timer(ack_expected);
                nbuffered--;
                inc(ack_expected);
            }
            break;

        /* ACK计时器超时，发送独立的ACK帧 */
        case ACK_TIMEOUT:
            dbg_event("---- ACK %d timeout\n", arg);
            send_ack_frame();
            break;

        /* 未收到接收方的ACK帧，则重发缓冲区所有帧 */
        case DATA_TIMEOUT:
            dbg_event("---- DATA %d timeout\n", arg);
            frame_nr = ack_expected;
            for (int i = 0; i < nbuffered; i++)
            {
                send_data_frame(frame_nr, frame_expected, buffer);
                inc(frame_nr);
            }
            break;
        }

        if (nbuffered < MAX_SEQ && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
    }
}
