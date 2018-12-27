#ifndef __FLASH_UPDATE_H__
#define __FLASH_UPDATE_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <unistd.h>

#include "aeiTag.h"
#include "flash_define.h"

#define INACTIVE 0
#define ACTIVE 1


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        flash_update_from_file
 *
 *	[DESCRIPTION]:
 *		update from RAM file, and active it
 *
 *	[PARAMETER]:
 *	        filename[in] 	: RAM filename
 *	        tag[in]		: tag write to flash
 *
 *	[RETURN]
 *              >= 0	 	:         SUCCESS
 *              other		:         FAIL
 *
 *	[History]
 *			@2011.9.15  jerome.zhao: create this function
 **************************************************************************/
int flash_update_from_file(const char *filename, AEI_IMG_HEADER *tag);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        flash_read_header
 *
 *	[DESCRIPTION]:
 *		read ACTIVE/INACTIVE system's header
 *
 *	[PARAMETER]:
 *	        tag[in] 	: buf for tag
 *	        flag[in] 	: INACTIVE/ACTIVE
 *
 *	[RETURN]
 *              >= 0	 	:         SUCCESS
 *              other		:         FAIL
 *
 *	[History]
 *			@2011.9.15  jerome.zhao: create this function
 **************************************************************************/
int flash_read_header(void *tag, int flag);

/**************************************************************************
 *	[FUNCTION NAME]:
 *	        flash_update_state
 *
 *	[DESCRIPTION]:
 *		return update state
 *
 *	[RETURN]
 *              >= 0	 	:         update state
 *              other		:         FAIL
 *
 *	[History]
 *			@2011.9.21  jerome.zhao: create this function
 **************************************************************************/
int flash_update_state(void);


/**************************************************************************
 *	[FUNCTION NAME]:
 *	        flash_update_active_img
 *
 *	[DESCRIPTION]:
 *		active the which point by img_num
 *
 *	[RETURN]
 *              >= 0	 	:         SUCCESS
 *              other		:         FAIL
 *
 *	[History]
 *			@2012.2.21  jerome.zhao: create this function
 **************************************************************************/
int flash_update_active_img(int img_num);


#endif
