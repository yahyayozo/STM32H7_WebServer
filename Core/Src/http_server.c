/*
 * http_server.c
 *
 *  Created on: Sep 2, 2024
 *      Author: yaouledamer
 */


/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c
  * @author  MCD Application Team
  * @brief   Basic http server implementation using LwIP netconn API
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include "http_server.h"
#include "cmsis_os.h"

#include <stdio.h>
#include "rest_api.h"

char indexPage[] = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 17\r\n"
        "\r\n"
        "<h2>Hi</h2>";

HTTP_Request httpRequest = {0};
HTTP_Response httpResponse = {0};

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
struct fs_file file;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief serve tcp connection
  * @param conn: pointer on connection structure
  * @retval None
  */


char headers[1024];
char contentLength[30] = {0};

static void http_server_serve(struct netconn *conn)
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;


  /* Read the data from the port, blocking if nothing yet there.
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn, &inbuf);

  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK)
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);


      /* Parse the HTTP request */
      rest_api_parse_request(buf,buflen,&httpRequest);

      if (REST_API_OK == rest_api_call_endpoint(&httpRequest,&httpResponse)){
		 /* netconn_write(conn, (const unsigned char*) indexPage,
									(size_t) sizeof(indexPage), NETCONN_COPY);*/

		  netconn_write(conn, (const unsigned char*) httpResponse.headers,
										(size_t) httpResponse.headersLen, NETCONN_COPY);

		  netconn_write(conn, (const unsigned char*) httpResponse.body,
							(size_t) httpResponse.bodyLen, NETCONN_COPY);
      }


    }
  }

  // Small delay before closing the connection
  osDelay(100);
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);

  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}


/**
  * @brief  http server thread
  * @param arg: pointer on argument(not used here)
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;

  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);

  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);

    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);

      while(1)
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_serve(newconn);

          /* delete connection */
          netconn_delete(newconn);
        }
        osDelay(1);
      }
    }
    else if(err == ERR_USE)
    {
        /* https://lwip.fandom.com/wiki/Netconn_bind
         * ...if you try to bind the same address and/or port you might get an error (ERR_USE, address in use),
         * even if you delete the netconn. Only after some time (minutes) the resources are completely cleared
         * in the underlying stack due to the need to follow the TCP specification and go through the TCP timewait
         * state.  */
        osDelay(30000);
    }
  }
  while(1)
  {
	  osDelay(100);
  }
}

REST_API_Error http_server_handle_index (HTTP_Request* http_request,HTTP_Response* http_response){

	memset(http_response->headers, 0, sizeof(http_response->headers));

	fs_open(&file, "/index.html");
	strcpy(http_response->headers, "HTTP/1.1 200 OK\r\n");
	uint32_t index = strlen(http_response->headers);
	sprintf(&http_response->headers[index], "Content-Length: %d\r\n"
			"Content-Encoding: gzip\r\n"
			"Content-Type: text/html\r\n\r\n", file.len);
	http_response->headersLen = strlen(http_response->headers);
	http_response->body = file.data;
	http_response->bodyLen = file.len;

	fs_close(&file);

	return REST_API_OK;
}

/**
  * @brief  Initialize the HTTP server (start its thread)
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{
  rest_api_register_endpoint_callback(HTTP_METHOD_GET, "/", http_server_handle_index,NULL);

  sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}
