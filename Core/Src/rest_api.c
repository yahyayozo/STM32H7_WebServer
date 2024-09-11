/*
 * rest_api.c
 *
 *  Created on: Sep 4, 2024
 *      Author: yaouledamer
 */

#include "rest_api.h"
#include "stdio.h"

Rest_API_Endpoint API_Endpoints[REST_API_MAX_ENDPOINTS];

REST_API_Error rest_api_parse_request(uint8_t *request_buf, uint32_t request_buf_len,
		HTTP_Request *http_request) {
	char method[10] = { 0 };
	memset(http_request, 0, sizeof(http_request));
	sscanf(request_buf, "%s %s", method, http_request->URI);

	if (0 == strcmp(method, HTTP_GET)) {
		http_request->method = HTTP_METHOD_GET;
	} else if (0 == strcmp(method, HTTP_POST)) {
		http_request->method = HTTP_METHOD_POST;
	} else if (0 == strcmp(method, HTTP_UPDATE)) {
		http_request->method = HTTP_METHOD_UPDATE;
	} else if (0 == strcmp(method, HTTP_DELETE)) {
		http_request->method = HTTP_METHOD_DELETE;
	} else{
		return REST_API_UNCORRECT_METHOD;
	}

	return REST_API_OK;

}

REST_API_Error rest_api_register_endpoint_callback(HTTP_MethodType method, char *uri,
		rest_api_request_clb request_clb,uint16_t *entryIndex) {

	if (NULL == request_clb) {
		return REST_API_NULL_CALLBACK;
	}

	for (uint16_t i = 0; i < REST_API_MAX_ENDPOINTS; i++) {
		if (NULL == API_Endpoints[i].callback) {
			API_Endpoints[i].methodType = method;
			strcpy(API_Endpoints[i].URI, uri);
			API_Endpoints[i].callback = request_clb;
			if(NULL != entryIndex){
				*entryIndex = i;
			}
			return REST_API_OK;
		}
	}

	return REST_API_NO_SPACE;
}

REST_API_Error rest_api_call_endpoint(const HTTP_Request *http_request,
		HTTP_Response *http_response) {
	for (uint16_t i = 0; i < REST_API_MAX_ENDPOINTS; i++) {
		if (http_request->method == API_Endpoints[i].methodType
				&& 0 == strcmp(http_request->URI, API_Endpoints[i].URI)) {
			if (API_Endpoints[i].callback != NULL) {
				return API_Endpoints[i].callback(http_request, http_response);
			}
		}
	}

	return REST_API_NO_REGISTERED_ENDPOINT;
}

