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


#import "UnsignedCharVector_proxy.h"
#import "VectorTileDecoder_proxy.h"
#import "MBVectorTileStyleSet_proxy.h"

/**
 * Decoder for vector tiles in MapBox format.
 */
__attribute__ ((visibility("default"))) @interface NTMBVectorTileDecoder : NTVectorTileDecoder
-(void*)getCptr;
-(id)initWithCptr: (void*)cptr swigOwnCObject: (BOOL)ownCObject;
/**
 * Constructs decoder for MapBox vector tiles based on specified style set.<br>
 * If the style set contains multiple styles, the first one (based on lexicographical order) from the root folder is chosen.<br>
 * @param styleSet Style set for the tiles.
 */
-(id)initWithStyleSet: (NTMBVectorTileStyleSet*)styleSet;
/**
 * Constructs decoder for MapBox vector tiles based on specified style set.<br>
 * Specified style is selected as the current style.<br>
 * @param styleSet Style set for the tiles.<br>
 * @param styleName Style to select.
 */
-(id)initWithStyleSet: (NTMBVectorTileStyleSet*)styleSet styleName: (NSString*)styleName;
/**
 * Returns the current style name.<br>
 * @return The current style name.
 */
-(NSString*)getCurrentStyleName;
/**
 * Select current style by style name. The style must exist in the style set container specified in the constructor as an xml file.<br>
 * This call will also reset style-related parameters, like geometry and billboard scales of the decoder.<br>
 * @param styleName style to use
 */
-(void)setCurrentStyle: (NSString*)styleName;
/**
 * Returns the relative scale of geometry elements.<br>
 * @return The current geometry scale.
 */
-(float)getGeometryScale;
/**
 * Updates relative scale of geometry elements (lines). Default is 1.0f.<br>
 * @param scale New relative scale (1.0f is the default).
 */
-(void)setGeometryScale: (float)scale;
/**
 * Returns the relative scale of billboard elements.<br>
 * @return The current billboard scale.
 */
-(float)getBillboardScale;
/**
 * Updates the relative scale of billboard and text elements. Default is 1.0f.<br>
 * @param scale New relative scale (1.0f is the default).
 */
-(void)setBillboardScale: (float)scale;
-(NTColor*)getBackgroundColor;
-(int)getMinZoom;
-(int)getMaxZoom;

-(void)dealloc;

@end


#ifdef __cplusplus
}
#endif
