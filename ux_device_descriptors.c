/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_descriptors.c
  * @author  MCD Application Team
  * @brief   USBX Device descriptor header file
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
#include "ux_device_descriptors.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
USBD_DevClassHandleTypeDef  USBD_Device_FS, USBD_Device_HS;

uint8_t UserClassInstance[USBD_MAX_CLASS_INTERFACES] = {
  CLASS_TYPE_CDC_ACM,
};

/* The generic device descriptor buffer that will be filled by builder
   Size of the buffer is the maximum possible device FS descriptor size. */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t DevFrameWorkDesc_FS[USBD_FRAMEWORK_MAX_DESC_SZ] __ALIGN_END = {0};

/* The generic device descriptor buffer that will be filled by builder
   Size of the buffer is the maximum possible device HS descriptor size. */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static uint8_t DevFrameWorkDesc_HS[USBD_FRAMEWORK_MAX_DESC_SZ] __ALIGN_END = {0};

static uint8_t *pDevFrameWorkDesc_FS = DevFrameWorkDesc_FS;

static uint8_t *pDevFrameWorkDesc_HS = DevFrameWorkDesc_HS;
/* USER CODE BEGIN PV0 */

/* USER CODE END PV0 */

/* String Device Framework :
 Byte 0 and 1 : Word containing the language ID : 0x0904 for US
 Byte 2       : Byte containing the index of the descriptor
 Byte 3       : Byte containing the length of the descriptor string
*/
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN UCHAR USBD_string_framework[USBD_STRING_FRAMEWORK_MAX_LENGTH]
__ALIGN_END = {0};

/* Multiple languages are supported on the device, to add
   a language besides English, the Unicode language code must
   be appended to the language_id_framework array and the length
   adjusted accordingly. */

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN UCHAR USBD_language_id_framework[LANGUAGE_ID_MAX_LENGTH]
__ALIGN_END = {0};

/* USER CODE BEGIN PV1 */

/* USER CODE END PV1 */

/* Private function prototypes -----------------------------------------------*/
static void USBD_Desc_GetString(uint8_t *desc, uint8_t *Buffer, uint16_t *len);
static uint8_t USBD_Desc_GetLen(uint8_t *buf);

static uint8_t *USBD_Device_Framework_Builder(USBD_DevClassHandleTypeDef *pdev,
                                              uint8_t *pDevFrameWorkDesc,
                                              uint8_t *UserClassInstance,
                                              uint8_t Speed);

static uint8_t USBD_FrameWork_AddToConfDesc(USBD_DevClassHandleTypeDef *pdev,
                                            uint8_t Speed,
                                            uint8_t *pCmpstConfDesc);

static uint8_t USBD_FrameWork_AddClass(USBD_DevClassHandleTypeDef *pdev,
                                       USBD_CompositeClassTypeDef class,
                                       uint8_t cfgidx, uint8_t Speed,
                                       uint8_t *pCmpstConfDesc);

static uint8_t USBD_FrameWork_FindFreeIFNbr(USBD_DevClassHandleTypeDef *pdev);

static void USBD_FrameWork_AddConfDesc(uint32_t Conf, uint32_t *pSze);

static void USBD_FrameWork_AssignEp(USBD_DevClassHandleTypeDef *pdev, uint8_t Add,
                                    uint8_t Type, uint32_t Sze);

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
static void USBD_FrameWork_CDCDesc(USBD_DevClassHandleTypeDef *pdev,
                                   uint32_t pConf, uint32_t *Sze);
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED == 1U */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_Get_Device_Framework_Speed
  *         Return the device speed descriptor
  * @param  Speed : HIGH or FULL SPEED flag
  * @param  length : length of HIGH or FULL SPEED array
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_Get_Device_Framework_Speed(uint8_t Speed, ULONG *Length)
{
  uint8_t *pFrameWork = NULL;
  /* USER CODE BEGIN Device_Framework0 */

  /* USER TAG BEGIN Device_Framework0 */

  if (USBD_FULL_SPEED == Speed)
  {
    USBD_Device_Framework_Builder(&USBD_Device_FS, pDevFrameWorkDesc_FS,
                                  UserClassInstance, Speed);

    /* Get the length of USBD_device_framework_full_speed */
    *Length = (ULONG)(USBD_Device_FS.CurrDevDescSz + USBD_Device_FS.CurrConfDescSz);

    pFrameWork = pDevFrameWorkDesc_FS;
  }
  else
  {
    USBD_Device_Framework_Builder(&USBD_Device_HS, pDevFrameWorkDesc_HS,
                                  UserClassInstance, Speed);

    /* Get the length of USBD_device_framework_high_speed */
    *Length = (ULONG)(USBD_Device_HS.CurrDevDescSz + USBD_Device_HS.CurrConfDescSz);

    pFrameWork = pDevFrameWorkDesc_HS;
  }
  /* USER CODE Device_Framework1 */

  /* USER CODE Device_Framework1 */
  return pFrameWork;
}

/**
  * @brief  USBD_Get_String_Framework
  *         Return the language_id_framework
  * @param  Length : Length of String_Framework
  * @retval Pointer to language_id_framework buffer
  */
uint8_t *USBD_Get_String_Framework(ULONG *Length)
{
  uint16_t len = 0U;
  uint8_t count = 0U;

  /* USER CODE String_Framework0 */

  /* USER CODE String_Framework0 */

  /* Set the Manufacturer language Id and index in USBD_string_framework */
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_MFC_STR;

  /* Set the Manufacturer string in string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_string_framework + count, &len);

  /* Set the Product language Id and index in USBD_string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_PRODUCT_STR;

  /* Set the Product string in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_string_framework + count, &len);

  /* Set Serial language Id and index in string_framework */
  count += len + 1;
  USBD_string_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_string_framework[count++] = USBD_LANGID_STRING >> 8;
  USBD_string_framework[count++] = USBD_IDX_SERIAL_STR;

  /* Set the Serial number in USBD_string_framework */
  USBD_Desc_GetString((uint8_t *)USBD_SERIAL_NUMBER, USBD_string_framework + count, &len);

  /* USER CODE String_Framework1 */

  /* USER CODE String_Framework1 */

  /* Get the length of USBD_string_framework */
  *Length = strlen((const char *)USBD_string_framework);

  return USBD_string_framework;
}

/**
  * @brief  USBD_Get_Language_Id_Framework
  *         Return the language_id_framework
  * @param  Length : Length of Language_Id_Framework
  * @retval Pointer to language_id_framework buffer
  */
uint8_t *USBD_Get_Language_Id_Framework(ULONG *Length)
{
  uint8_t count = 0U;

  /* Set the language Id in USBD_language_id_framework */
  USBD_language_id_framework[count++] = USBD_LANGID_STRING & 0xFF;
  USBD_language_id_framework[count++] = USBD_LANGID_STRING >> 8;

  /* Get the length of USBD_language_id_framework */
  *Length = strlen((const char *)USBD_language_id_framework);

  return USBD_language_id_framework;
}

/**
  * @brief  USBD_Get_Interface_Number
  *         Return interface number
  * @param  class_type : Device class type
  * @param  interface_type : Device interface type
  * @retval interface number
  */
uint16_t USBD_Get_Interface_Number(uint8_t class_type, uint8_t interface_type)
{
  uint8_t itf_num = 0U;
  uint8_t idx = 0U;

  /* USER CODE BEGIN USBD_Get_Interface_Number0 */

  /* USER CODE BEGIN USBD_Get_Interface_Number0 */

  for(idx = 0; idx < USBD_MAX_SUPPORTED_CLASS; idx++)
  {
    if ((USBD_Device_FS.tclasslist[idx].ClassType == class_type) &&
        (USBD_Device_FS.tclasslist[idx].InterfaceType == interface_type))
    {
      itf_num = USBD_Device_FS.tclasslist[idx].Ifs[0];
    }
  }

  /* USER CODE BEGIN USBD_Get_Interface_Number1 */

  /* USER CODE BEGIN USBD_Get_Interface_Number1 */

  return itf_num;
}

/**
  * @brief  USBD_Get_Configuration_Number
  *         Return configuration number
  * @param  class_type : Device class type
  * @param  interface_type : Device interface type
  * @retval configuration number
  */
uint16_t USBD_Get_Configuration_Number(uint8_t class_type, uint8_t interface_type)
{
  uint8_t cfg_num = 1U;

  /* USER CODE BEGIN USBD_Get_CONFIGURATION_Number0 */

  /* USER CODE BEGIN USBD_Get_CONFIGURATION_Number0 */

  /* USER CODE BEGIN USBD_Get_CONFIGURATION_Number1 */

  /* USER CODE BEGIN USBD_Get_CONFIGURATION_Number1 */

  return cfg_num;
}

/**
  * @brief  USBD_Desc_GetString
  *         Convert ASCII string into Unicode one
  * @param  desc : descriptor buffer
  * @param  Unicode : Formatted string buffer (Unicode)
  * @param  len : descriptor length
  * @retval None
  */
static void USBD_Desc_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
  uint8_t idx = 0U;
  uint8_t *pdesc;

  if (desc == NULL)
  {
    return;
  }

  pdesc = desc;
  *len = (uint16_t)USBD_Desc_GetLen(pdesc);

  unicode[idx++] = *(uint8_t *)len;

  while (*pdesc != (uint8_t)'\0')
  {
    unicode[idx++] = *pdesc;
    pdesc++;
  }
}

/**
  * @brief  USBD_Desc_GetLen
  *         return the string length
  * @param  buf : pointer to the ASCII string buffer
  * @retval string length
  */
static uint8_t USBD_Desc_GetLen(uint8_t *buf)
{
  uint8_t  len = 0U;
  uint8_t *pbuff = buf;

  while (*pbuff != (uint8_t)'\0')
  {
    len++;
    pbuff++;
  }

  return len;
}

/**
  * @brief  USBD_Device_Framework_Builder
  *         Device Framework builder
  * @param  pdev: device instance
  * @param  pDevFrameWorkDesc: Pointer to the device framework descriptor
  * @param  UserClassInstance: type of the class to be added
  * @param  Speed: Speed parameter HS or FS
  * @retval status
  */
static uint8_t *USBD_Device_Framework_Builder(USBD_DevClassHandleTypeDef *pdev,
                                              uint8_t *pDevFrameWorkDesc,
                                              uint8_t *UserClassInstance,
                                              uint8_t Speed)
{
  static USBD_DeviceDescTypedef   *pDevDesc;
  static USBD_DevQualiDescTypedef *pDevQualDesc;
  uint8_t Idx_Instance = 0U;

  /* Set Dev and conf descriptors size to 0 */
  pdev->CurrConfDescSz = 0U;
  pdev->CurrDevDescSz = 0U;

  /* Set the pointer to the device descriptor area*/
  pDevDesc = (USBD_DeviceDescTypedef *)pDevFrameWorkDesc;

  /* Start building the generic device descriptor common part */
  pDevDesc->bLength = (uint8_t)sizeof(USBD_DeviceDescTypedef);
  pDevDesc->bDescriptorType = UX_DEVICE_DESCRIPTOR_ITEM;
  pDevDesc->bcdUSB = USB_BCDUSB;
  pDevDesc->bDeviceClass = 0x00;
  pDevDesc->bDeviceSubClass = 0x00;
  pDevDesc->bDeviceProtocol = 0x00;
  pDevDesc->bMaxPacketSize = USBD_MAX_EP0_SIZE;
  pDevDesc->idVendor = USBD_VID;
  pDevDesc->idProduct = USBD_PID;
  pDevDesc->bcdDevice = 0x0200;
  pDevDesc->iManufacturer = USBD_IDX_MFC_STR;
  pDevDesc->iProduct = USBD_IDX_PRODUCT_STR;
  pDevDesc->iSerialNumber = USBD_IDX_SERIAL_STR;
  pDevDesc->bNumConfigurations = USBD_MAX_NUM_CONFIGURATION;
  pdev->CurrDevDescSz += (uint32_t)sizeof(USBD_DeviceDescTypedef);

  /* Check if USBx is in high speed mode to add qualifier descriptor */
  if (Speed == USBD_HIGH_SPEED)
  {
    pDevQualDesc = (USBD_DevQualiDescTypedef *)(pDevFrameWorkDesc + pdev->CurrDevDescSz);
    pDevQualDesc->bLength = (uint8_t)sizeof(USBD_DevQualiDescTypedef);
    pDevQualDesc->bDescriptorType = UX_DEVICE_QUALIFIER_DESCRIPTOR_ITEM;
    pDevQualDesc->bcdDevice = 0x0200;
    pDevQualDesc->Class = 0x00;
    pDevQualDesc->SubClass = 0x00;
    pDevQualDesc->Protocol = 0x00;
    pDevQualDesc->bMaxPacketSize = 0x40;
    pDevQualDesc->bNumConfigurations = 0x01;
    pDevQualDesc->bReserved = 0x00;
    pdev->CurrDevDescSz += (uint32_t)sizeof(USBD_DevQualiDescTypedef);
  }

  /* Build the device framework */
  while (Idx_Instance < USBD_MAX_SUPPORTED_CLASS)
  {
    if ((pdev->classId < USBD_MAX_SUPPORTED_CLASS) &&
        (pdev->NumClasses < USBD_MAX_SUPPORTED_CLASS) &&
        (UserClassInstance[Idx_Instance] != CLASS_TYPE_NONE))
    {
      /* Call the composite class builder */
      (void)USBD_FrameWork_AddClass(pdev,
                                    (USBD_CompositeClassTypeDef)UserClassInstance[Idx_Instance],
                                    0, Speed,
                                    (pDevFrameWorkDesc + pdev->CurrDevDescSz));

      /* Increment the ClassId for the next occurrence */
      pdev->classId ++;
      pdev->NumClasses ++;
    }

    Idx_Instance++;
  }

  /* Check if there is a composite class and update device class */
  if (pdev->NumClasses > 1)
  {
    pDevDesc->bDeviceClass = 0xEF;
    pDevDesc->bDeviceSubClass = 0x02;
    pDevDesc->bDeviceProtocol = 0x01;
  }
  else
  {
    /* Check if the CDC ACM class is set and update device class */
    if (UserClassInstance[0] == CLASS_TYPE_CDC_ACM)
    {
      pDevDesc->bDeviceClass = 0x02;
      pDevDesc->bDeviceSubClass = 0x02;
      pDevDesc->bDeviceProtocol = 0x00;
    }
  }

  return pDevFrameWorkDesc;
}

/**
  * @brief  USBD_FrameWork_AddClass
  *         Register a class in the class builder
  * @param  pdev: device instance
  * @param  class: type of the class to be added (from USBD_CompositeClassTypeDef)
  * @param  cfgidx: configuration index
  * @param  speed: device speed
  * @param  pCmpstConfDesc: to composite device configuration descriptor
  * @retval status
  */
uint8_t  USBD_FrameWork_AddClass(USBD_DevClassHandleTypeDef *pdev,
                                 USBD_CompositeClassTypeDef class,
                                 uint8_t cfgidx, uint8_t Speed,
                                 uint8_t *pCmpstConfDesc)
{

  if ((pdev->classId < USBD_MAX_SUPPORTED_CLASS) &&
      (pdev->tclasslist[pdev->classId].Active == 0U))
  {
    /* Store the class parameters in the global tab */
    pdev->tclasslist[pdev->classId].ClassId = pdev->classId;
    pdev->tclasslist[pdev->classId].Active = 1U;
    pdev->tclasslist[pdev->classId].ClassType = class;

    /* Call configuration descriptor builder and endpoint configuration builder */
    if (USBD_FrameWork_AddToConfDesc(pdev, Speed, pCmpstConfDesc) != UX_SUCCESS)
    {
      return UX_ERROR;
    }
  }

  UNUSED(cfgidx);

  return UX_SUCCESS;
}

/**
  * @brief  USBD_FrameWork_AddToConfDesc
  *         Add a new class to the configuration descriptor
  * @param  pdev: device instance
  * @param  Speed: device speed
  * @param  pCmpstConfDesc: to composite device configuration descriptor
  * @retval status
  */
uint8_t  USBD_FrameWork_AddToConfDesc(USBD_DevClassHandleTypeDef *pdev, uint8_t Speed,
                                      uint8_t *pCmpstConfDesc)
{
  uint8_t interface = 0U;

  /* USER CODE FrameWork_AddToConfDesc_0 */

  /* USER CODE FrameWork_AddToConfDesc_0 */

  /* The USB drivers do not set the speed value, so set it here before starting */
  pdev->Speed = Speed;

  /* start building the config descriptor common part */
  if (pdev->classId == 0U)
  {
    /* Add configuration and IAD descriptors */
    USBD_FrameWork_AddConfDesc((uint32_t)pCmpstConfDesc, &pdev->CurrConfDescSz);
  }

  switch (pdev->tclasslist[pdev->classId].ClassType)
  {

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1

    case CLASS_TYPE_CDC_ACM:

      /* Find the first available interface slot and Assign number of interfaces */
      interface = USBD_FrameWork_FindFreeIFNbr(pdev);
      pdev->tclasslist[pdev->classId].NumIf = 2U;
      pdev->tclasslist[pdev->classId].Ifs[0] = interface;
      pdev->tclasslist[pdev->classId].Ifs[1] = (uint8_t)(interface + 1U);

      /* Assign endpoint numbers */
      pdev->tclasslist[pdev->classId].NumEps = 3U;  /* EP_IN, EP_OUT, CMD_EP */

      /* Check the current speed to assign endpoints */
      if (Speed == USBD_HIGH_SPEED)
      {
        /* Assign OUT Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPOUT_ADDR,
                                USBD_EP_TYPE_BULK, USBD_CDCACM_EPOUT_HS_MPS);

        /* Assign IN Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPIN_ADDR,
                                USBD_EP_TYPE_BULK, USBD_CDCACM_EPIN_HS_MPS);

        /* Assign CMD Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPINCMD_ADDR,
                                USBD_EP_TYPE_INTR, USBD_CDCACM_EPINCMD_HS_MPS);
      }
      else
      {
        /* Assign OUT Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPOUT_ADDR,
                                USBD_EP_TYPE_BULK, USBD_CDCACM_EPOUT_FS_MPS);

        /* Assign IN Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPIN_ADDR,
                                USBD_EP_TYPE_BULK, USBD_CDCACM_EPIN_FS_MPS);

        /* Assign CMD Endpoint */
        USBD_FrameWork_AssignEp(pdev, USBD_CDCACM_EPINCMD_ADDR,
                                USBD_EP_TYPE_INTR, USBD_CDCACM_EPINCMD_FS_MPS);
      }

      /* Configure and Append the Descriptor */
      USBD_FrameWork_CDCDesc(pdev, (uint32_t)pCmpstConfDesc, &pdev->CurrConfDescSz);

      break;

#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */
    /* USER CODE FrameWork_AddToConfDesc_1 */

    /* USER CODE FrameWork_AddToConfDesc_1 */

    default:
      /* USER CODE FrameWork_AddToConfDesc_2 */

      /* USER CODE FrameWork_AddToConfDesc_2 */
      break;
  }

  return UX_SUCCESS;
}

/**
  * @brief  USBD_FrameWork_FindFreeIFNbr
  *         Find the first interface available slot
  * @param  pdev: device instance
  * @retval The interface number to be used
  */
static uint8_t USBD_FrameWork_FindFreeIFNbr(USBD_DevClassHandleTypeDef *pdev)
{
  uint32_t idx = 0U;

  /* Unroll all already activated classes */
  for (uint32_t i = 0U; i < pdev->NumClasses; i++)
  {
    /* Unroll each class interfaces */
    for (uint32_t j = 0U; j < pdev->tclasslist[i].NumIf; j++)
    {
      /* Increment the interface counter index */
      idx++;
    }
  }

  /* Return the first available interface slot */
  return (uint8_t)idx;
}

/**
  * @brief  USBD_FrameWork_AddConfDesc
  *         Add a new class to the configuration descriptor
  * @param  Conf: configuration descriptor
  * @param  pSze: pointer to the configuration descriptor size
  * @retval none
  */
static void  USBD_FrameWork_AddConfDesc(uint32_t Conf, uint32_t *pSze)
{
  /* Intermediate variable to comply with MISRA-C Rule 11.3 */
  USBD_ConfigDescTypedef *ptr = (USBD_ConfigDescTypedef *)Conf;

  ptr->bLength = (uint8_t)sizeof(USBD_ConfigDescTypedef);
  ptr->bDescriptorType = USB_DESC_TYPE_CONFIGURATION;
  ptr->wDescriptorLength = 0U;
  ptr->bNumInterfaces = 0U;
  ptr->bConfigurationValue = 1U;
  ptr->iConfiguration = USBD_CONFIG_STR_DESC_IDX;
  ptr->bmAttributes = USBD_CONFIG_BMATTRIBUTES;
  ptr->bMaxPower = USBD_CONFIG_MAXPOWER;
  *pSze += sizeof(USBD_ConfigDescTypedef);
}

/**
  * @brief  USBD_FrameWork_AssignEp
  *         Assign and endpoint
  * @param  pdev: device instance
  * @param  Add: Endpoint address
  * @param  Type: Endpoint type
  * @param  Sze: Endpoint max packet size
  * @retval none
  */
static void  USBD_FrameWork_AssignEp(USBD_DevClassHandleTypeDef *pdev,
                                     uint8_t Add, uint8_t Type, uint32_t Sze)
{
  uint32_t idx = 0U;

  /* Find the first available endpoint slot */
  while (((idx < (pdev->tclasslist[pdev->classId]).NumEps) && \
          ((pdev->tclasslist[pdev->classId].Eps[idx].is_used) != 0U)))
  {
    /* Increment the index */
    idx++;
  }

  /* Configure the endpoint */
  pdev->tclasslist[pdev->classId].Eps[idx].add = Add;
  pdev->tclasslist[pdev->classId].Eps[idx].type = Type;
  pdev->tclasslist[pdev->classId].Eps[idx].size = (uint16_t) Sze;
  pdev->tclasslist[pdev->classId].Eps[idx].is_used = 1U;
}

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1
/**
  * @brief  USBD_FrameWork_CDCDesc
  *         Configure and Append the CDC Descriptor
  * @param  pdev: device instance
  * @param  pConf: Configuration descriptor pointer
  * @param  Sze: pointer to the current configuration descriptor size
  * @retval None
  */
static void USBD_FrameWork_CDCDesc(USBD_DevClassHandleTypeDef *pdev,
                                   uint32_t pConf, uint32_t *Sze)
{
  static USBD_IfDescTypedef               *pIfDesc;
  static USBD_EpDescTypedef               *pEpDesc;
  static USBD_CDCHeaderFuncDescTypedef    *pHeadDesc;
  static USBD_CDCCallMgmFuncDescTypedef   *pCallMgmDesc;
  static USBD_CDCACMFuncDescTypedef       *pACMDesc;
  static USBD_CDCUnionFuncDescTypedef     *pUnionDesc;
#if USBD_COMPOSITE_USE_IAD == 1
  static USBD_IadDescTypedef              *pIadDesc;
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

#if USBD_COMPOSITE_USE_IAD == 1
  pIadDesc = ((USBD_IadDescTypedef *)(pConf + *Sze));
  pIadDesc->bLength = (uint8_t)sizeof(USBD_IadDescTypedef);
  pIadDesc->bDescriptorType = USB_DESC_TYPE_IAD; /* IAD descriptor */
  pIadDesc->bFirstInterface = pdev->tclasslist[pdev->classId].Ifs[0];
  pIadDesc->bInterfaceCount = 2U;    /* 2 interfaces */
  pIadDesc->bFunctionClass = 0x02U;
  pIadDesc->bFunctionSubClass = 0x02U;
  pIadDesc->bFunctionProtocol = 0x01U;
  pIadDesc->iFunction = 0; /* String Index */
  *Sze += (uint32_t)sizeof(USBD_IadDescTypedef);
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

  /* Control Interface Descriptor */
  __USBD_FRAMEWORK_SET_IF(pdev->tclasslist[pdev->classId].Ifs[0], 0U, 1U, 0x02,
                          0x02U, 0x01U, 0U);

  /* Control interface headers */
  pHeadDesc = ((USBD_CDCHeaderFuncDescTypedef *)((uint32_t)pConf + *Sze));
  /* Header Functional Descriptor*/
  pHeadDesc->bLength = 0x05U;
  pHeadDesc->bDescriptorType = 0x24U;
  pHeadDesc->bDescriptorSubtype = 0x00U;
  pHeadDesc->bcdCDC = 0x0110;
  *Sze += (uint32_t)sizeof(USBD_CDCHeaderFuncDescTypedef);

  /* Call Management Functional Descriptor*/
  pCallMgmDesc = ((USBD_CDCCallMgmFuncDescTypedef *)((uint32_t)pConf + *Sze));
  pCallMgmDesc->bLength = 0x05U;
  pCallMgmDesc->bDescriptorType = 0x24U;
  pCallMgmDesc->bDescriptorSubtype = 0x01U;
  pCallMgmDesc->bmCapabilities = 0x00U;
  pCallMgmDesc->bDataInterface = pdev->tclasslist[pdev->classId].Ifs[1];
  *Sze += (uint32_t)sizeof(USBD_CDCCallMgmFuncDescTypedef);

  /* ACM Functional Descriptor*/
  pACMDesc = ((USBD_CDCACMFuncDescTypedef *)((uint32_t)pConf + *Sze));
  pACMDesc->bLength = 0x04U;
  pACMDesc->bDescriptorType = 0x24U;
  pACMDesc->bDescriptorSubtype = 0x02U;
  pACMDesc->bmCapabilities = 0x02;
  *Sze += (uint32_t)sizeof(USBD_CDCACMFuncDescTypedef);

  /* Union Functional Descriptor*/
  pUnionDesc = ((USBD_CDCUnionFuncDescTypedef *)((uint32_t)pConf + *Sze));
  pUnionDesc->bLength = 0x05U;
  pUnionDesc->bDescriptorType = 0x24U;
  pUnionDesc->bDescriptorSubtype = 0x06U;
  pUnionDesc->bMasterInterface = pdev->tclasslist[pdev->classId].Ifs[0];
  pUnionDesc->bSlaveInterface = pdev->tclasslist[pdev->classId].Ifs[1];
  *Sze += (uint32_t)sizeof(USBD_CDCUnionFuncDescTypedef);

  /* Append Endpoint descriptor to Configuration descriptor */
  __USBD_FRAMEWORK_SET_EP(pdev->tclasslist[pdev->classId].Eps[2].add, \
                          USBD_EP_TYPE_INTR,
                          (uint16_t)pdev->tclasslist[pdev->classId].Eps[2].size,
                          USBD_CDCACM_EPINCMD_HS_BINTERVAL,
                          USBD_CDCACM_EPINCMD_FS_BINTERVAL);

  /* Data Interface Descriptor */
  __USBD_FRAMEWORK_SET_IF(pdev->tclasslist[pdev->classId].Ifs[1], 0U, 2U, 0x0A,
                          0U, 0U, 0U);

  /* Append Endpoint descriptor to Configuration descriptor */
  __USBD_FRAMEWORK_SET_EP((pdev->tclasslist[pdev->classId].Eps[0].add), \
                          (USBD_EP_TYPE_BULK),
                          (uint16_t)(pdev->tclasslist[pdev->classId].Eps[0].size),
                          (0x00U), (0x00U));

  /* Append Endpoint descriptor to Configuration descriptor */
  __USBD_FRAMEWORK_SET_EP((pdev->tclasslist[pdev->classId].Eps[1].add), \
                          (USBD_EP_TYPE_BULK),
                          (uint16_t)(pdev->tclasslist[pdev->classId].Eps[1].size),
                          (0x00U), (0x00U));

  /* Update Config Descriptor and IAD descriptor */
  ((USBD_ConfigDescTypedef *)pConf)->bNumInterfaces += 2U;
  ((USBD_ConfigDescTypedef *)pConf)->wDescriptorLength = *Sze;
}
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED == 1 */

/* USER CODE BEGIN 1 */

#define BYTE uint8_t
#define UINT16 uint16_t

#define EP0         0
#define EP0_SIZE    64

#define EP1         1
#define EP1_SIZE    64

#define EP2         2
#define EP2_SIZE    8

/* Descriptor Types */
#define DSC_DEV     0x01
#define DSC_CFG     0x02
#define DSC_STR     0x03
#define DSC_INTF    0x04
#define DSC_EP      0x05

/* Functional Descriptors */
/* Type Values for the bDscType Field */
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25

/* bDscSubType in Functional Descriptors */
#define DSC_FN_HEADER               0x00
#define DSC_FN_CALL_MGT             0x01
#define DSC_FN_ACM                  0x02    // ACM - Abstract Control Management
#define DSC_FN_DLM                  0x03    // DLM - Direct Line Managment
#define DSC_FN_TELEPHONE_RINGER     0x04
#define DSC_FN_RPT_CAPABILITIES     0x05
#define DSC_FN_UNION                0x06
#define DSC_FN_COUNTRY_SELECTION    0x07
#define DSC_FN_TEL_OP_MODES         0x08
#define DSC_FN_USB_TERMINAL         0x09

/* Endpoint definitions */
#define _EP01_OUT   0x01
#define _EP01_IN    0x81
#define _EP02_OUT   0x02
#define _EP02_IN    0x82
#define _EP03_OUT   0x03
#define _EP03_IN    0x83
#define _EP04_OUT   0x04
#define _EP04_IN    0x84
#define _EP05_OUT   0x05
#define _EP05_IN    0x85
#define _EP06_OUT   0x06
#define _EP06_IN    0x86
#define _EP07_OUT   0x07
#define _EP07_IN    0x87
#define _EP08_OUT   0x08
#define _EP08_IN    0x88
#define _EP09_OUT   0x09
#define _EP09_IN    0x89
#define _EP10_OUT   0x0A
#define _EP10_IN    0x8A
#define _EP11_OUT   0x0B
#define _EP11_IN    0x8B
#define _EP12_OUT   0x0C
#define _EP12_IN    0x8C
#define _EP13_OUT   0x0D
#define _EP13_IN    0x8D
#define _EP14_OUT   0x0E
#define _EP14_IN    0x8E
#define _EP15_OUT   0x0F
#define _EP15_IN    0x8F

/*
Affectation des endpoints physiques pour le device CDC

  EP0               EP0 est toujours le endpoint de contrôle
  CDC_INT_EP        endpoint d'interruption
  CDC_BULK_IN_EP    endpoint Bulk de transport des données du device vers l'hôte
  CDC_BULK_OUT_EP   endpoint Bulk de transport des données de l'hôte vers le device
*/

#define CDC_INT_EP              EP2
#define CDC_BULK_IN_EP          EP1
#define CDC_BULK_OUT_EP         EP1

/* Endpoint Transfer Type */
#define _CTRL       0x00            //Control Transfer
#define _ISO        0x01            //Isochronous Transfer
#define _BULK       0x02            //Bulk Transfer
#define _INT        0x03            //Interrupt Transfer

/*
  Même chose, exprimée dans le descripteur de device USB
*/
#define CDC_INT_IN_EP           _EP02_IN
#define CDC_INT_EP_SIZE         EP2_SIZE

#define CDC_BULK_IN_SPEC        _EP01_IN
#define CDC_BULK_IN_EP_SIZE     EP1_SIZE

#define CDC_BULK_OUT_SPEC       _EP01_OUT
#define CDC_BULK_OUT_EP_SIZE    EP1_SIZE

typedef struct
{
    BYTE bLength;       BYTE bDscType;      UINT16 bcdUSB;
    BYTE bDevCls;       BYTE bDevSubCls;    BYTE bDevProtocol;
    BYTE bMaxPktSize0;  UINT16 idVendor;    UINT16 idProduct;
    UINT16 bcdDevice;   BYTE iMFR;          BYTE iProduct;
    BYTE iSerialNum;    BYTE bNumCfg;
} __packed USB_DEV_DSC;

static const USB_DEV_DSC device_dsc =
{
    sizeof(USB_DEV_DSC),    // Size of this descriptor in bytes
    DSC_DEV,                // DEVICE descriptor type
    0x00110,                // USB Spec Release Number in BCD format
    0x02,                   // Class Code = CDC device
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    EP0_SIZE,               // Max packet size for EP0
    0x20BB,                 // Vendor ID
    0x0001,                 // Product ID
    0x0001,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index    : sd001
    0x02,                   // Product string index         : sd002
    0x00,                   // Device serial number string index : !sd003 cf. commentaire ci-dessous
    0x01                    // Number of possible configurations
};

typedef struct
{
    BYTE bLength;       BYTE bDscType;      UINT16 wTotalLength;
    BYTE bNumIntf;      BYTE bCfgValue;     BYTE iCfg;
    BYTE bmAttributes;  BYTE bMaxPower;
} __packed USB_CFG_DSC;

typedef struct
{
    BYTE bLength;       BYTE bDscType;      BYTE bIntfNum;
    BYTE bAltSetting;   BYTE bNumEPs;       BYTE bIntfCls;
    BYTE bIntfSubCls;   BYTE bIntfProtocol; BYTE iIntf;
} __packed USB_INTF_DSC;

typedef struct
{
    BYTE bLength;       BYTE bDscType;      BYTE bEPAdr;
    BYTE bmAttributes;  UINT16 wMaxPktSize; BYTE bInterval;
} __packed USB_EP_DSC;

/* Header Functional Descriptor */
typedef struct
{
    BYTE bFNLength;
    BYTE bDscType;
    BYTE bDscSubType;
    UINT16 bcdCDC;
} __packed USB_CDC_HEADER_FN_DSC;

/* Abstract Control Management Functional Descriptor */
typedef struct
{
    BYTE bFNLength;
    BYTE bDscType;
    BYTE bDscSubType;
    BYTE bmCapabilities;
} __packed USB_CDC_ACM_FN_DSC;

/* Union Functional Descriptor */
typedef struct
{
    BYTE bFNLength;
    BYTE bDscType;
    BYTE bDscSubType;
    BYTE bMasterIntf;
    BYTE bSaveIntf0;
} __packed USB_CDC_UNION_FN_DSC;

/* Call Management Functional Descriptor */
typedef struct
{
    BYTE bFNLength;
    BYTE bDscType;
    BYTE bDscSubType;
    BYTE bmCapabilities;
    BYTE bDataInterface;
} __packed USB_CDC_CALL_MGT_FN_DSC;

#define CDC_COMM_INTF_ID        0x00
#define CDC_DATA_INTF_ID        0x01

static const struct Cfg01
{   USB_CFG_DSC             cd01;
    USB_INTF_DSC            i01a00;
    USB_CDC_HEADER_FN_DSC   cdc_header_fn_i01a00;
    USB_CDC_ACM_FN_DSC      cdc_acm_fn_i01a00;
    USB_CDC_UNION_FN_DSC    cdc_union_fn_i01a00;
    USB_CDC_CALL_MGT_FN_DSC cdc_call_mgt_fn_i01a00;
    USB_EP_DSC              ep02i_i01a00;
    USB_INTF_DSC            i02a00;
    USB_EP_DSC              ep03o_i02a00;
    USB_EP_DSC              ep03i_i02a00;
} __packed cfg01 =
{
    /* Configuration Descriptor */
    {sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    DSC_CFG,                // CONFIGURATION descriptor type
    sizeof(cfg01),          // Total length of data for this cfg
    2,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    (1 << 7),	            // bus powered
    50},                     // Max power consumption (2X mA)

    /* Interface Descriptor */
    {sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    DSC_INTF,               // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    1,                      // Number of endpoints in this intf
    0x02,                   // Class code = Communication Interface
    0x02,                   // Subclass code = Abstract Control Model
    0x00,                   // No class-specific protocol
    0},                      // Interface string index

    /* CDC Class-Specific Descriptors */
    {sizeof(USB_CDC_HEADER_FN_DSC),
    CS_INTERFACE,           // INTERFACE Functional Descriptor
    DSC_FN_HEADER,          // Header Functional Descriptor
    0x0110},                 // USB release number

    {sizeof(USB_CDC_ACM_FN_DSC),
    CS_INTERFACE,           // INTERFACE Functional Descriptor
    DSC_FN_ACM,             // Abstract Control Management
    0x00},                   // Capabilities

    {sizeof(USB_CDC_UNION_FN_DSC),
    CS_INTERFACE,           // INTERFACE Functional Descriptor
    DSC_FN_UNION,           // Union Functional Descriptor
    CDC_COMM_INTF_ID,       // number of the controlling interface
    CDC_DATA_INTF_ID},       // interface number of the first subordinate

    {sizeof(USB_CDC_CALL_MGT_FN_DSC),
    CS_INTERFACE,           // INTERFACE Functional Descriptor
    DSC_FN_CALL_MGT,        // Call Management
    0x00,                   // Capabilities
    CDC_DATA_INTF_ID},       // Interface number

    /* Endpoint Descriptor */
    {sizeof(USB_EP_DSC),     // Size of this descriptor
    DSC_EP,                 // ENDPOINT descriptor type
    CDC_INT_IN_EP,          // endpoint address and direction
    _INT,                   // endpoint transfer type
    CDC_INT_EP_SIZE,        // Maximum packet size
    0xff},                   // Maximum polling interval

    /* Interface Descriptor */
    {sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    DSC_INTF,               // INTERFACE descriptor type
    1,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    0x0a,                   // Class code
    0,                      // Subclass code
    0x00,                   // Protocol code
    0},                      // Interface string index

    /* Endpoint Descriptors */
    {sizeof(USB_EP_DSC),     // Size of this descriptor
    DSC_EP,                 // ENDPOINT descriptor type
    CDC_BULK_OUT_SPEC,        // endpoint address and direction
    _BULK,                  // endpoint transfer type
    CDC_BULK_OUT_EP_SIZE,   // Maximum packet size
    0x00},                   // Maximum polling interval

    {sizeof(USB_EP_DSC),     // Size of this descriptor
    DSC_EP,                 // ENDPOINT descriptor type
    CDC_BULK_IN_SPEC,       // endpoint address and direction
    _BULK,                  // endpoint transfer type
    CDC_BULK_IN_EP_SIZE,    // Maximum packet size
    0x00}                    // Maximum polling interval
};

static const struct {
	USB_DEV_DSC dev_desc;
	struct Cfg01 dev_cfg01;
} __packed device_framework = {

	device_dsc,
	cfg01
};

uint8_t *USBD_Get_Device_Framework_Speed(uint8_t Speed, ULONG *Length)
{
	*Length = sizeof(device_framework);
	return (uint8_t *)&device_framework;
}

static const struct Sd000
{BYTE bLength; BYTE bDscType; UINT16 string[1];} __packed sd000 =
{
    sizeof(sd000),
    DSC_STR,
    {0x0409}
};

static const struct Sd001
{BYTE bLength;BYTE bDscType;UINT16 string[29];} __packed sd001 =
{
    sizeof(sd001),
    DSC_STR,
	{'T', 'h', 'a', 'l', 'e', 's', ' ', 'T', 'r', 'a', 'n', 's', 'p', 'o', 'r', 't', 'a', 't', 'i', 'o', 'n', ' ', 'S', 'y', 's', 't', 'e', 'm', 's'}
};

static const struct Sd002
{BYTE bLength;BYTE bDscType;UINT16 string[13];} __packed sd002 =
{
    sizeof(sd002),
    DSC_STR,
	{'C', 'S', 'C', ' ', 'R', 'e', 'a', 'd', 'e', 'r', ' ', 'V', '4'}
};

static const struct Sd003
{BYTE bLength;BYTE bDscType;UINT16 string[12 * 2];} __packed sd003 =
{
    sizeof(sd003),
    DSC_STR,
    // Le champ string de <sd003> est initialisé avec le numéro de série du
    // micro-contrôleur dans la fonction <SysUsbCdc_Install>.
	{'0', '1', '2', '3', '4', '5', '6', '0', '1', '2', '3', '4', '5', '6'}
};

/*static const struct {
	struct Sd000 s0;
	struct Sd001 s1;
	struct Sd002 s2;
	struct Sd003 s3;
} __packed string_framework = {
		sd000,
		sd001,
		sd002,
		sd003
};*/

static const uint8_t string_framework[] = {
		0x09, 0x04, 1, 3, 'R', 'C', 'S',
		0x09, 0x04, 2, 13, 'C', 'S', 'C', ' ', 'R', 'e', 'a', 'd', 'e', 'r', ' ', 'V', '4',
		0x09, 0x04, 3, 12, '0', '1', '2', '3', '4', '5', '6', '0', '1', '2', '3', '4', '5', '6'
};

uint8_t *USBD_Get_String_Framework(ULONG *Length)
{
	*Length = sizeof(string_framework);
	return (uint8_t *)&string_framework;
}

BYTE language_id_framework[2] = {
    /* English. */
    0x09, 0x04
};

uint8_t *USBD_Get_Language_Id_Framework(ULONG *Length)
{
	*Length = sizeof(language_id_framework);
	return (uint8_t *)language_id_framework;
}

/* USER CODE END 1 */
