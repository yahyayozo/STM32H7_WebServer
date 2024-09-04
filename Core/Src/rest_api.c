/*
 * rest_api.c
 *
 *  Created on: Sep 4, 2024
 *      Author: yaouledamer
 */


#include "rest_api.h"
#include "stdio.h"

char request_method[10];
char uri[256];

void rest_api_parse_request(uint8_t* request_buf,uint32_t request_buf_len){
	sscanf(request_buf,"%s %s",request_method,uri);
}
