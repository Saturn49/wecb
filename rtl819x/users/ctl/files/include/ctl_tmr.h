

#ifndef __CMS_TMR_H__
#define __CMS_TMR_H__


/*!\file cms_tmr.h
 * \brief Header file for the CMS Event Timer API.
 *  This is in the cms_util library.
 *
 */

/** Event handler type definition
 */
typedef void (*CmsEventHandler)(void*);


/** Max length (including NULL character) of an event timer name.
 *
 * When an event timer is created, the caller can give it a name
 * to help with debugging and lookup.  Name is optional.
 */
#define CMS_EVENT_TIMER_NAME_LENGTH  32


/** Initialize a timer handle.
 *
 * @param tmrHandle (OUT) On successful return, a handle to be used for
 *                        future handle operation is returned.
 *
 * @return tsl_rv_t enum.
 */
tsl_rv_t ctl_timer_init(void **tmrHandle);


/** Create a new event timer which will expire in the specified number of 
 *  milliseconds.
 *
 * Since lookups are done using a combination of the handler func and
 * context data, there must not be an existing timer event in the handle
 * with the same handler func and context data.  (We could allow 
 * multiple entries with the same func and ctxData, but we will have to
 * clarify what it means to cancel a timer, cancel all or cancel the
 * next timer.)
 *
 * @param tmrHandle (IN/OUT) Pointer to timer handle that was returned by ctl_timer_init().
 * @param func      (IN)     The handler func.
 * @param ctxData   (IN)     Optional data to be passed in with the handler func.
 * @param ms        (IN)     Timer expiration value in milliseconds.
 * @param name      (IN)     Optional name of this timer event.
 *
 * @return tsl_rv_t enum.
 */   
tsl_rv_t ctl_timer_set(void *tmrHandle, CmsEventHandler func, void *ctxData, tsl_u32_t ms, const char *name);


/** Stop an event timer and delete it.
 *
 * The event timer is found by matching the callback func and ctxData.
 *
 * @param tmrHandle (IN/OUT) Pointer to the event timer handle;
 * @param func      (IN) The event handler.
 * @param *handle   (IN) Argument passed to the event handler.
 */   
void ctl_timer_cancel(void *tmrHandle, CmsEventHandler func, void *ctxData);


/** Get the number of milliseconds until the next event is due to expire.
 *
 * @param tmrHandle (IN)  Pointer to timer handle that was returned by ctl_timer_init().
 * @param ms        (OUT) Number of milliseconds until the next event.
 *
 * @return tsl_rv_t enum.  Specifically, TSL_RV_SUC if there is a next event.
 *         If there are no more events in the timer handle, CMSRET_NO_MORE_INSTANCES
 *         will be returned and the parameter ms is set to MAX_tsl_u32_t.
 */
tsl_rv_t ctl_timer_get_time_to_next_event(const void *tmrHandle, tsl_u32_t *ms);


/** Return true if the specified handler func and context data (event) is set.
 *  
 * @param tmrHandle (IN) Pointer to timer handle that was returned by ctl_timer_init().
 * @param func      (IN) The handler func.
 * @param ctxData   (IN) Optional data to be passed in with the handler func.
 *
 * @return TSL_B_TRUE if specified event is present, otherwise, TSL_B_FALSE.
 */   
tsl_bool_t ctl_timer_is_event_present(const void *tmrHandle, CmsEventHandler func, void *ctxData);

#endif  /* __CMS_TMR_H__ */
