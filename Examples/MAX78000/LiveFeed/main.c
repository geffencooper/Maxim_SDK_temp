/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
*
* This software is protected by copyright laws of the United States and
* of foreign countries. This material may also be protected by patent laws
* and technology transfer regulations of the United States and of foreign
* countries. This software is furnished under a license agreement and/or a
* nondisclosure agreement and may only be used or reproduced in accordance
* with the terms of those agreements. Dissemination of this information to
* any party or parties not specified in the license agreement and/or
* nondisclosure agreement is expressly prohibited.
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/

/**
 * @file    main.c
 * @brief   Camera Interface
 * @details experimenting/understanding the camera interface
 */

/***** Includes *****/
// standard C libraries
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// helper libraries for peripherals
#include "mxc_device.h"
#include "mxc_sys.h"
#include "bbfc_regs.h"
#include "fcr_regs.h"
#include "icc.h"
#include "led.h"
#include "pb.h"
#include "mxc_delay.h"
#include "camera.h"
#include "camera_tft_funcs.h"
#include "dma.h"
#include "tft_fthr.h"
#include "capture_button.h"
#include "sd_card_funcs.h"

/***** Definitions *****/
#define CAMERA_FREQ   (10 * 1000 * 1000)

/******************************************************************************/
int main(void)
{
  // Enable cache
  MXC_ICC_Enable(MXC_ICC0);

  // Switch to 100 MHz clock
  MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
  SystemCoreClockUpdate();

  // Initialize DMA for camera interface
	MXC_DMA_Init();
	int dma_channel = MXC_DMA_AcquireChannel();

  // Initialize TFT display.
  init_LCD();
  MXC_TFT_ClearScreen();

  // Initialize camera.
  printf("Init Camera.\n");
  camera_init(CAMERA_FREQ*2);
  
  set_image_dimensions(128, 128);

  /* Set the screen rotation because camera flipped*/
	MXC_TFT_SetRotation(ROTATE_180);

  // Setup the camera image dimensions, pixel format and data acquiring details.
  // four bytes because each pixel is 2 bytes, can get 2 pixels at a time
	int ret = camera_setup(get_image_x(), get_image_y(), PIXFORMAT_RGB565, FIFO_FOUR_BYTE, USE_DMA, dma_channel);
  if (ret != STATUS_OK) 
  {
		printf("Error returned from setting up camera. Error %d\n", ret);
		return -1;
	}
  
  MXC_Delay(1000000);
  MXC_TFT_SetBackGroundColor(4);

  // test_drive();
  // return 0;


  init_capture_button();
  init_class_button();

    // int img_len_words = (img_dim*img_dim)/4;
    // uint32_t input_0_camera[1024];
    // uint32_t input_1_camera[1024];
    // uint32_t input_2_camera[1024];
   
    MXC_TFT_SetForeGroundColor(YELLOW);
    // TFT_Print(buff,0,0,font_1,sprintf(buff,"option 1"));
    // cd("sorting_imgs");

    // uint16_t num = 0;
    // num_to_file("CLASS1/num_imgs.txt",&num);
    // num_to_file("CLASS2/num_imgs.txt",&num);
    // num_to_file("CLASS3/num_imgs.txt",&num);
    // num_to_file("CLASS4/num_imgs.txt",&num);
    // num_to_file("CLASS5/num_imgs.txt",&num);
    // get_num_from_file("CLASS1/num_imgs.txt",&num);
    // get_num_from_file("CLASS2/num_imgs.txt",&num);
    // get_num_from_file("CLASS3/num_imgs.txt",&num);
    // get_num_from_file("CLASS4/num_imgs.txt",&num);
    // get_num_from_file("CLASS5/num_imgs.txt",&num);
    // return 0;
    
    while (1) 
    {
        
        //err = 0;
        //printf("i: %i\n",i);
        capture_camera_img();
        //i+=1;
        display_RGB565_img(56,96);
        //process_RGB888_img(input_0_camera,input_1_camera,input_2_camera);
        //display_RGB888_img(input_0_camera,input_1_camera,input_2_camera,1024,0,0);
    }
    
    printf("End of example, please try to read the card.\n");
    return 0;
}