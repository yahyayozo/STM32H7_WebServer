/*
 * rest_api.h
 *
 *  Created on: Sep 4, 2024
 *      Author: yaouledamer
 */

#ifndef INC_REST_API_H_
#define INC_REST_API_H_

#include "stdint.h"

#define REST_API_MAX_ENDPOINTS (10)

typedef enum {
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_UPDATE,
	HTTP_METHOD_DELETE,
}HTTP_MethodType;


typedef struct{
	HTTP_MethodType method;
	char URI[256];
}HTTP_Request;

typedef struct{
	char* body;
	uint32_t bodyLen;
}HTTP_Response;


typedef void (*rest_api_request_clb) (HTTP_Request* http_request,HTTP_Response* http_response);


typedef struct {
	HTTP_MethodType methodType;
	char URI[256];
	rest_api_request_clb callback;
}Rest_API_Endpoint;




void rest_api_parse_request(uint8_t* request_buf,uint32_t request_buf_len, HTTP_Request* http_request);

void rest_api_register_endpoint_callback(HTTP_MethodType method, char* uri,rest_api_request_clb request_clb);

void rest_api_call_endpoint(const HTTP_Request* http_request, HTTP_Response* http_response);

#endif /* INC_REST_API_H_ */
