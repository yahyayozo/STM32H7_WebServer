/*
 * rest_api.h
 *
 *  Created on: Sep 4, 2024
 *      Author: yaouledamer
 */

#ifndef INC_REST_API_H_
#define INC_REST_API_H_

#include "stdint.h"

void rest_api_parse_request(uint8_t* request_buf,uint32_t request_buf_len);

#endif /* INC_REST_API_H_ */
