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


#import "TileDataSource_proxy.h"

/**
 * An online tile data source that connects to Nutiteq tile server.<br>
 * Tile server is currently used to 
 */
__attribute__ ((visibility("default"))) @interface NTNutiteqOnlineTileDataSource : NTTileDataSource
-(void*)getCptr;
-(id)initWithCptr: (void*)cptr swigOwnCObject: (BOOL)ownCObject;
/**
 * Constructs a NutiteqTileDataSource object.<br>
 * @param source Tile source id. Currently only "nutiteq.mbstreets" is supported.
 */
-(id)initWithSource: (NSString*)source;
-(NTTileData*)loadTile: (NTMapTile*)mapTile;

-(void)dealloc;

@end


#ifdef __cplusplus
}
#endif

