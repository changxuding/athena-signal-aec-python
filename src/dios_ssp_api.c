/* Copyright (C) 2017 Beijing Didi Infinity Technology and Development Co.,Ltd.
All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Description: This is an illustration of how you can use the modules of 
athena-signal to create your own signal processing algorithms. A complete 
framework includes high-pass filter(HPF), acoustic echo cancellation(AEC),
voice activity detection(VAD), beamforming using MVDR, noise supression(NS)
and automatic gain control(AGC). You can enable/disable each module according
to your needs by setting KEYs to 1/0 in SSP_PARAM struct.

Other modules such as source localization, blind source separation, 
de-reverberation will be added soon. We are working on them!
==============================================================================*/

#include "dios_ssp_api.h"

objDios_ssp* dios_ssp_init_api(int frame_size, int nb_mic, int nb_speakers, int mode)
{
    int i;
	void* ptr = NULL;
	ptr = (void*)calloc(1, sizeof(objDios_ssp));
	objDios_ssp* srv = (objDios_ssp*)ptr;
    
    // params init
    srv->cfg_frame_len = frame_size;
    srv->cfg_mic_num = nb_mic;
    srv->cfg_ref_num = nb_speakers;

    // init aec module
    srv->ptr_aec = dios_ssp_aec_init_api(srv->cfg_mic_num, srv->cfg_ref_num, srv->cfg_frame_len);
    
    if (mode==0) dios_ssp_aec_config_api(srv->ptr_aec, 0);  // 0: communication mode; 1: asr mode
    // allocate memory
    srv->ptr_mic_buf = (float*)calloc(srv->cfg_mic_num * srv->cfg_frame_len, sizeof(float));
    srv->ptr_ref_buf = (float*)calloc(srv->cfg_ref_num * srv->cfg_frame_len, sizeof(float));
    srv->ptr_data_buf = (float*)calloc(srv->cfg_frame_len, sizeof(float));

    // variables init 
    srv->dt_st = 1;

	return ptr;
}

int dios_ssp_reset_api(void* ptr)
{
    if(ptr == NULL)
    {
        return ERROR_AUDIO_PROCESS;
    }

    objDios_ssp* srv = (objDios_ssp*)ptr;
    int ret;

    // variables reset
    srv->dt_st = 1;
    ret = dios_ssp_aec_reset_api(srv->ptr_aec);
    if(ret != 0)
    {
        return ERROR_AEC;
    }
    

	return 0;
}

int dios_ssp_process_api(void* ptr, const short* mic_buf, const short* ref_buf, short* out_buf)
{
    if(ptr == NULL)
    {
        return ERROR_AUDIO_PROCESS;
    }

    objDios_ssp* srv = (objDios_ssp*)ptr;
    int ret;
    int i, j;
   
    // get input data, single-channel or multi-channel
    for(i = 0; i < srv->cfg_mic_num; i++)
    {
        for(j = 0; j < srv->cfg_frame_len; j++)
        {
            srv->ptr_mic_buf[i * srv->cfg_frame_len + j] = (float)(mic_buf[i * srv->cfg_frame_len + j]);
        }
    }
    if(ref_buf != NULL)
    {
        for(i = 0; i < srv->cfg_ref_num; i++)
        {
            for(j = 0; j < srv->cfg_frame_len; j++)
            {
                srv->ptr_ref_buf[i * srv->cfg_frame_len + j] = (float)(ref_buf[i * srv->cfg_frame_len + j]);
            }
        }
    }

    // aec process
    if(ref_buf != NULL)
    {
        ret = dios_ssp_aec_process_api(srv->ptr_aec, srv->ptr_mic_buf, srv->ptr_ref_buf, &srv->dt_st);
        if(ret != 0)
        {
            return ERROR_AEC;
        }
    }

    for(i = 0; i < srv->cfg_mic_num; i++)
    {
        for(j = 0; j < srv->cfg_frame_len; j++)
        {
            out_buf[i * srv->cfg_frame_len + j] = (short)(srv->ptr_mic_buf[i * srv->cfg_frame_len + j]);
        }
    }

    return 0;
}

int dios_ssp_uninit_api(void* ptr)
{
    if(ptr == NULL)
    {
        return ERROR_AUDIO_PROCESS;
    }
    objDios_ssp* srv = (objDios_ssp*)ptr;
    int ret;

    /* free memory */
    if(srv->ptr_mic_buf != NULL)
    {
        free(srv->ptr_mic_buf);
        srv->ptr_mic_buf = NULL;
    }
    if(srv->ptr_ref_buf != NULL)
    {
        free(srv->ptr_ref_buf);
        srv->ptr_ref_buf = NULL;
    }
    if(srv->ptr_data_buf != NULL)
    {
        free(srv->ptr_data_buf);
        srv->ptr_data_buf = NULL;
    }
 
    ret = dios_ssp_aec_uninit_api(srv->ptr_aec);
    if(ret != 0)
    {
        return ERROR_AEC;
    }
    free(srv);
	return OK_AUDIO_PROCESS;
}

