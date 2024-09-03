/*
 * http_server.h
 *
 *  Created on: Sep 2, 2024
 *      Author: yaouledamer
 */

#ifndef INC_HTTP_SERVER_H_
#define INC_HTTP_SERVER_H_

#include "lwip/api.h"

void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn);


#endif /* INC_HTTP_SERVER_H_ */
