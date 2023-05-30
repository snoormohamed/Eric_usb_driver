/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_cdc_acm.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "ux_device_cdc_acm.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "app_usbx_device.h"

#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_RX_DATA_SIZE                2048
#define APP_TX_DATA_SIZE                2048
#define VCP_WORDLENGTH8                 8
#define VCP_WORDLENGTH9                 9
#define MIN_BAUDRATE                    9600
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER CDC_VCP_LineCoding =
{
  115200, /* baud rate */
  0x00,   /* stop bits-1 */
  0x00,   /* parity - none */
  0x08    /* nb. of bits 8 */
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void Error_Handler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @param  cdc Instance
  * @retval none
  */
void CDC_Init_FS(void *cdc_acm)
{
  /* Status */
  UINT ux_status = UX_SUCCESS;

  /* USER CODE BEGIN 3 */

  printf("Callback CDC_Init_FS\n");

  /* Set device_class_cdc_acm with default parameters */
  ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                            UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                            &CDC_VCP_LineCoding);
  /* Check Status */
  if (ux_status != UX_SUCCESS)
  {
    Error_Handler();
  }

  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
void CDC_DeInit_FS(void *cdc_acm)
{
  /* USER CODE BEGIN 4 */
  printf("Callback CDC_DeInit_FS\n");
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cdc Instance
  * @retval none
  */
VOID ux_app_parameters_change(VOID *cdc_acm)
{
  UX_SLAVE_TRANSFER *transfer_request;
  UX_SLAVE_DEVICE   *device;
  ULONG             request;
  UINT ux_status = UX_SUCCESS;

  printf("Callback ux_app_parameters_change\n");

  /* Get the pointer to the device.  */
  device = &_ux_system_slave -> ux_system_slave_device;

  /* Get the pointer to the transfer request associated with the control endpoint. */
  transfer_request = &device -> ux_slave_device_control_endpoint.
                     ux_slave_endpoint_transfer_request;

  /* Extract all necessary fields of the request. */
  request = *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);

  /* Here we proceed only the standard request we know of at the device level.  */
  switch (request)
  {
    /* Set Line Coding Command */
    case UX_SLAVE_CLASS_CDC_ACM_SET_LINE_CODING :
    {
      /* Get the Line Coding parameters */
      ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                                UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING,
                                                &CDC_VCP_LineCoding);
      /* Check Status */
      if (ux_status != UX_SUCCESS)
      {
        Error_Handler();
      }
      break;
    }

    /* Get Line Coding Command */
    case UX_SLAVE_CLASS_CDC_ACM_GET_LINE_CODING :
    {
      ux_status = ux_device_class_cdc_acm_ioctl(cdc_acm,
                                                UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                                &CDC_VCP_LineCoding);

      /* Check Status */
      if (ux_status != UX_SUCCESS)
      {
        Error_Handler();
      }
      break;
    }

    /* Set the the control line state */
    case UX_SLAVE_CLASS_CDC_ACM_SET_CONTROL_LINE_STATE :
    default :
      break;
  }
}

/* USER CODE END 0 */

/**
  * @brief  USBD_CDC_ACM_Activate
  *         This function is called when insertion of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Activate */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);
  /* USER CODE END USBD_CDC_ACM_Activate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_Deactivate
  *         This function is called when extraction of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Deactivate */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);
  /* USER CODE END USBD_CDC_ACM_Deactivate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_ParameterChange
  *         This function is invoked to manage the CDC ACM class requests.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_ParameterChange(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_ParameterChange */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);
  /* USER CODE END USBD_CDC_ACM_ParameterChange */

  return;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
