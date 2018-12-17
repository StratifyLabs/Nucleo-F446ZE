/*

Copyright 2011-2018 Stratify Labs, Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 */

#include <sapi/sys/requests.h>

#include "config.h"

#include <arm_dsp_api_declaration.h>


int kernel_request(int req, void * arg){
        return -1;
}

const void * kernel_request_api(u32 request){
	switch(request){
		case SAPI_API_REQUEST_ARM_DSP_Q7: return SOS_BOARD_ARM_DSP_API_Q7;
		case SAPI_API_REQUEST_ARM_DSP_Q15: return SOS_BOARD_ARM_DSP_API_Q15;
		case SAPI_API_REQUEST_ARM_DSP_Q31: return SOS_BOARD_ARM_DSP_API_Q31;
		case SAPI_API_REQUEST_ARM_DSP_F32: return SOS_BOARD_ARM_DSP_API_F32;
		case SAPI_API_REQUEST_ARM_DSP_CONVERSION: return SOS_BOARD_ARM_DSP_CONVERSION_API;
	}
	return 0;
}
