#ifndef _DIOS_SSP_API_H_
#define _DIOS_SSP_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dios_ssp_aec_api.h"
#include "dios_ssp_return_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    /* handle of each module */
	void* ptr_aec;

    /* necessary buffer definition */
	float* ptr_mic_buf;
	float* ptr_ref_buf;
	float* ptr_data_buf;

    /* necessary variables */
    int cfg_frame_len;
    int cfg_mic_num;
    int cfg_ref_num;

    /* necessary variables */
    int dt_st;
}objDios_ssp;

objDios_ssp* dios_ssp_init_api(int frame_size, int nb_mic, int nb_speakers, int mode);
	
int dios_ssp_reset_api(void* ptr);

int dios_ssp_process_api(void* ptr, const short* mic_buf, const short* ref_buf, short* out_buf);

int dios_ssp_uninit_api(void* ptr);

#ifdef __cplusplus
}
#endif
#endif  /* _DIOS_SSP_API_H_ */

