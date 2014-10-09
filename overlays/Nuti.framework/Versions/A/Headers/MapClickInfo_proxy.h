/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.6
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#import <Foundation/Foundation.h>


#ifdef __cplusplus
extern "C" {
#endif


#import "MapPos_proxy.h"
#import "MapEventListener_proxy.h"

/**
 * A container class that provies information about a click performed on<br>
 * an empty area of the map.
 */
@interface NTMapClickInfo : NSObject
{
	void *swigCPtr;
	BOOL swigCMemOwn;
}
-(void*)getCptr;
-(id)initWithCptr: (void*)cptr swigOwnCObject: (BOOL)ownCObject;
/**
 * Returns the click type.<br>
 * @return The type of the click performed.
 */
-(enum NTClickType)getClickType;
/**
 * Returns the click position.<br>
 * @return The click position in the coordinate system of the base projection.
 */
-(NTMapPos*)getClickPos;

-(void)dealloc;

@end


#ifdef __cplusplus
}
#endif
