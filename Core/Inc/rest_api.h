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

#define HTTP_GET	"GET"
#define HTTP_POST	"POST"
#define HTTP_UPDATE	"UPDATE"
#define HTTP_DELETE	"DELETE"

typedef enum {
	HTTP_METHOD_GET, HTTP_METHOD_POST, HTTP_METHOD_UPDATE, HTTP_METHOD_DELETE,
} HTTP_MethodType;

typedef struct {
	HTTP_MethodType method;
	char URI[256];
} HTTP_Request;

typedef struct {
	char *body;
	uint32_t bodyLen;
} HTTP_Response;


typedef enum {
	REST_API_OK,
	REST_API_UNCORRECT_METHOD,
	REST_API_NO_SPACE,
	REST_API_NO_REGISTERED_ENDPOINT,
	REST_API_NULL_CALLBACK
} REST_API_Error;

typedef REST_API_Error (*rest_api_request_clb)(HTTP_Request *http_request,
		HTTP_Response *http_response);

typedef struct {
	HTTP_MethodType methodType;
	char URI[256];
	rest_api_request_clb callback;
} Rest_API_Endpoint;


REST_API_Error rest_api_parse_request(uint8_t *request_buf,
		uint32_t request_buf_len, HTTP_Request *http_request);

REST_API_Error rest_api_register_endpoint_callback(HTTP_MethodType method,
		char *uri, rest_api_request_clb request_clb, uint16_t *entryIndex);

REST_API_Error rest_api_call_endpoint(const HTTP_Request *http_request,
		HTTP_Response *http_response);

#endif /* INC_REST_API_H_ */
