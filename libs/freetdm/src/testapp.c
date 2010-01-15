#include "freetdm.h"

int main(int argc, char *argv[])
{
	ftdm_global_set_default_logger(FTDM_LOG_LEVEL_DEBUG);
	ftdm_channel_t *chan;
	unsigned ms = 20;
	ftdm_codec_t codec = FTDM_CODEC_SLIN;
	unsigned runs = 1;


	if (ftdm_global_init() != FTDM_SUCCESS) {
		fprintf(stderr, "Error loading OpenFTDM\n");
		exit(-1);
	}

	printf("OpenFTDM loaded\n");

 top:
	//if (ftdm_channel_open_any("wanpipe", 0, FTDM_TOP_DOWN, &chan) == FTDM_SUCCESS) {
	if (ftdm_channel_open(1, 1, &chan) == FTDM_SUCCESS) {
		int x = 0;
		printf("opened channel %d:%d\n", chan->span_id, chan->chan_id);

#if 1
		if (ftdm_channel_command(chan, FTDM_COMMAND_SET_INTERVAL, &ms) == FTDM_SUCCESS) {
			ms = 0;
			ftdm_channel_command(chan, FTDM_COMMAND_GET_INTERVAL, &ms);
			printf("interval set to %u\n", ms);
		} else {
			printf("set interval failed [%s]\n", chan->last_error);
		}
#endif
		if (ftdm_channel_command(chan, FTDM_COMMAND_SET_CODEC, &codec) == FTDM_SUCCESS) {
			codec = 1;
			ftdm_channel_command(chan, FTDM_COMMAND_GET_CODEC, &codec);
			printf("codec set to %u\n", codec);
		} else {
			printf("set codec failed [%s]\n", chan->last_error);
		}

		for(x = 0; x < 25; x++) {
			unsigned char buf[2048];
			ftdm_size_t len = sizeof(buf);
			ftdm_wait_flag_t flags = FTDM_READ;

			if (ftdm_channel_wait(chan, &flags, -1) == FTDM_FAIL) {
				printf("wait FAIL! %u [%s]\n", (unsigned)len, chan->last_error);
			}
			if (flags & FTDM_READ) {
				if (ftdm_channel_read(chan, buf, &len) == FTDM_SUCCESS) {
					printf("READ: %u\n", (unsigned)len); 
				} else {
					printf("READ FAIL! %u [%s]\n", (unsigned)len, chan->last_error);
					break;
				}
			} else {
				printf("wait fail [%s]\n", chan->last_error);
			}
		}
		ftdm_channel_close(&chan);
	} else {
		printf("open fail [%s]\n", chan->last_error);
	}

	if(--runs) {
		goto top;
	}

	ftdm_global_destroy();
	return 0;
}

/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4:
 */
