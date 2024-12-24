# STM32H7 Webserver with LwIP & FreeRTOS

## Overview

This project demonstrates a webserver implementation on the STM32H7 microcontroller using the LwIP stack and FreeRTOS. It features a pre-build script for generating C source files from web assets and includes a simple REST API module.

## Features

- **Webserver**:
  - Uses the LwIP stack for lightweight TCP/IP functionality.
  - Managed with FreeRTOS for task scheduling.

- **Pre-build Script**:
  - A Python script, `makeFSdata.py`, generates C source files from web files placed in the `Middlewares/Third_Party/LwIP/src/apps/http/fs` folder.
  - This script is automatically executed as a pre-build step.

- **REST API Module**:
  - Supports registering callbacks for specific endpoints.
  - Currently, the module registers a callback only for the `/` endpoint (serving `index.html`).

## Getting Started

1. **Add Web Files**
Place your web files into the `Middlewares/Third_Party/LwIP/src/apps/http/fs/` folder.

2. **Pre-build Script**
Ensure the `makeFSdata.py` script is executable and properly configured in your build environment to run as a pre-build step.

3. **Build and Flash**
Compile the project using your STM32 development environment and flash it onto the STM32H7 board.

4. **Access the Webserver**  
Connect to the STM32H7 board's IP address using a web browser to view the hosted content.

## REST API Usage  
The `rest_api.c` module allows you to register callbacks for specific endpoints. Currently, The project includes the following:

- **Endpoint**: `/`  
  - **Callback**: Serves `index.html`.

## Requirements  
- STM32H7 series microcontroller (tested on STM32H743ZI Nucleo board).  
- STM32CubeIDE or other compatible development tools.  
- Python 3.x for running `makeFSdata.py`.

## Notes  
- The `makeFSdata.py` script is essential for converting web files into an embeddable format. Always re-run the script after modifying the contents of the `fs` folder.

## Contributions  
Contributions are welcome! Please submit a pull request or open an issue to discuss potential changes.
