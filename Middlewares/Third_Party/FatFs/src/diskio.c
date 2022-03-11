/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2017        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2017, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "ff_gen_drv.h"
#include "spi_sdcard.h"

#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern Disk_drvTypeDef  disk;
#define SD_CARD     0       /* SD卡,卷标为0 */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
//  DSTATUS stat;
//
//  stat = disk.drv[pdrv]->disk_status(disk.lun[pdrv]);
//  return stat;
    return RES_OK;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS stat = RES_OK;
  uint8_t res = 0;

  if(disk.is_initialized[pdrv] == 0)
  {
    disk.is_initialized[pdrv] = 1;
    stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
  }

  res = sd_init();

  if(res)
      return STA_NOINIT;
  else
      return 0;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
//  DRESULT res;
//
//  res = disk.drv[pdrv]->disk_read(disk.lun[pdrv], buff, sector, count);
//  return res;

    uint8_t res = 0;
    if (!count)return RES_PARERR;   /* count不能等于0，否则返回参数错误 */

    res = sd_read_disk(buff, sector, count);
    while (res)   /* 读出错 */
    {
        if (res != 2)sd_init(); /* 重新初始化SD卡 */
        res = sd_read_disk(buff, sector, count);    //重新读取数据
//        UART_printf(&hspi1,"SD read error!");
    }

    if(res == 0x00)
        return RES_OK;
    else
        return RES_ERROR;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
  DRESULT res;

  res = disk.drv[pdrv]->disk_write(disk.lun[pdrv], buff, sector, count);
  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
//  DRESULT res;
//
//  res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
//  return res;

    DRESULT res;

    if (pdrv == SD_CARD)    /* SD卡 */
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;

            case GET_SECTOR_SIZE:
                *(DWORD *)buff = 512;
                res = RES_OK;
                break;

            case GET_BLOCK_SIZE:
                *(WORD *)buff = 8;
                res = RES_OK;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)buff = sd_get_sector_count();
                res = RES_OK;
                break;

            default:
                res = RES_PARERR;
                break;
        }
    }
    else
    {
        res = RES_ERROR;    /* 其他的不支持 */
    }

    return res;
}
#endif /* _USE_IOCTL == 1 */

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
__weak DWORD get_fattime (void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

