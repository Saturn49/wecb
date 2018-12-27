
#ifndef __CMS_TMS_H__
#define __CMS_TMS_H__


/*!\file cms_tms.h
 * \brief Header file for the CMS Timestamp API.
 *  This is in the cms_util library.
 *
 */

#include "ctl.h"

/** Number of nanoseconds in 1 second. */
#define NSECS_IN_SEC 1000000000

/** Number of nanoseconds in 1 milli-second. */
#define NSECS_IN_MSEC 1000000

/** Number of nanoseconds in 1 micro-second. */
#define NSECS_IN_USEC 1000

/** Number of micro-seconds in 1 second. */
#define USECS_IN_SEC  1000000

/** Number of micro-seconds in a milli-second. */
#define USECS_IN_MSEC 1000

/** Number of milliseconds in 1 second */
#define MSECS_IN_SEC  1000

/** Number of seconds in a minute */
#define SECS_IN_MINUTE   60

/** Number of seconds in an hour */
#define SECS_IN_HOUR     (SECS_IN_MINUTE * 60)

/** Number of seconds in a day */
#define SECS_IN_DAY      (SECS_IN_HOUR * 24)

/** Base format of the XSI datetime string */
#define UNKNOWN_DATETIME_STRING "0001-01-01T00:00:00Z"


/** OS independent timestamp structure.
 */
typedef struct
{
   tsl_u32_t sec;   /**< Number of seconds since some arbitrary point. */
   tsl_u32_t nsec;  /**< Number of nanoseconds since some arbitrary point. */
} CmsTimestamp;



/** Get the current timestamp.
 * 
 * The timestamps are supposed to be unaffected by changes in the system
 * time.  Even though timestamps have a nanosecond field, the resolution
 * of the timetime is probably around 2.5 to 10ms.
 *
 *@param tms (IN) This structure is filled with the current timestamp.
 */   
void ctl_timer_get(CmsTimestamp *tms);


/** Calculate the difference between two timestamps.
 *
 * This function correctly handles rollover of the timestamps.
 * That is, if newTms={0.0} and oldTms={4294967295,999999000} (1000ns before
 * rollover) then deltaTms={0,1000}.
 * Therefore,  caller must be careful to specify the two timestamps for
 * this function in the correct order.  Otherwise, the function will
 * think that a rollover has occured and return a surpringly large delta.
 *
 * In the more common, non-rollover case, example would be
 * newTms={10,500000000} and oldTms={3,100000000} so
 * deltaTms={7,400000000}.
 *
 * @param newTms    (IN) The timestamp that was obtained more recently.
 * @param oldTms    (IN) The timestamp that was obtained in the past.
 * @param deltaTms (OUT) The difference between newTms and oldTms.
 */   
void ctl_timer_delta(const CmsTimestamp *newTms,
                  const CmsTimestamp *oldTms,
                  CmsTimestamp *deltaTms);


/** Calculate the difference between two timestamps and return their
 *  difference in milliseconds.
 *
 * This function uses ctl_timer_delta(), so the comments for that function
 * applies to this function as well.
 *
 * @param newTms    (IN) The timestamp that was obtained more recently.
 * @param oldTms    (IN) The timestamp that was obtained in the past.
 * @return delta in milli-seconds.  If the delta in milliseconds is too
 *         large for an tsl_u32_t, then MAX_tsl_u32_t is returned.
 */   
tsl_u32_t ctl_timer_deltea_ms(const CmsTimestamp *newTms,
                                  const CmsTimestamp *oldTms);


/** Add the specified number of milliseconds to the given timestamp.
 *
 * @param tms    (IN/OUT) The timestamp to operate on.
 * @param ms     (IN)     The number of millisconds to add to tms.
 */
void ctl_timer_add_ms(CmsTimestamp *tms, tsl_u32_t ms);


/** Format the specified time in XSI format for TR69.
 *
 * @param t      (IN) Number of seconds since Jan 1 1970.  If 0, then this
 *                    function will use the current time.
 * @param buf   (OUT) Buffer to hold the formatted time.
 * @param bufLen (IN) Length of buffer.
 *
 * @return tsl_rv_t enum, specifically, if the buffer is not big enough,
 *                      CMSRET_RESOURCE_EXCEEDED will be returned.
 */
tsl_rv_t ctl_timer_get_time(tsl_u32_t t, char *buf, tsl_u32_t bufLen);


#endif  /* __CMS_TMS_H__ */
