//
//  TKStructs.h
//  ThemeKit
//
//  Created by Alexander Zielenski on 6/13/15.
//  Copyright © 2015 Alex Zielenski. All rights reserved.
//

#ifndef TKStructs_h
#define TKStructs_h

/* CSI Format
 csi_header (above)
 
 list of metadata
 
 MAGIC VALUES
 for csi_info in a list
 
 0xE903 - 1001: Slice rects, First 4 bytes length, next num slices rects, next a list of the slice rects
 0xEB03 - 1003: Metrics – First 4 length (including num metrics), next 4 num metrics, next a list of metrics (struct of 3 CGSizes)
 0xEC03 - 1004: Composition - First 4 length, second is the blendmode, third is a float for opacity
 0xED03 - 1005: UTI Type, First 4 length, next 4 length of string, then the string
 0xEE03 - 1006: Image Metadata: First 4 length, next 4 EXIF orientation, (UTI type...?)
 0xF203 - 1010: UNKNOWN. CONTAINS ONE VALUE. I think it's the length of the Internal Link Section, 'INKL'
 
 GRADIENT format documented in TKGradient.h
 SHAPE EFFECT format documented in CUIShapeEffectPreset.h
 
 IMAGES: 'CELM' – Core Element – Header followed by Zipped up raw image data, format coming soon
 RAW DATA: marts 'RAWD' followed by 4 bytes of zero and an unsigned int of the length of the raw data
 INTERNAL LINK: 'INKL' –
 OFFSET   SIZE   DESCRIPTION
 0        4      Magic
 4        4      Padding. Always zero
 8        16     Destination Frame
 24       2      '10'|'20' scale factor X 10?
 26       4      Length of Reference Key
 30       X      Rendition Reference Key List
 
 Reference key being the key of the asset whose pixel data contains this image
 */

// CSI Stands for Core Structured Image
struct csiheader {
    unsigned int magic; // CTSI – Core Theme Structured Image
    unsigned int version; // current known version is 1
    struct {
        unsigned int isHeaderFlaggedFPO:1;
        unsigned int isExcludedFromContrastFilter:1;
        unsigned int isVectorBased:1;
        unsigned int isOpaque:1;
        unsigned int reserved:28;
    } renditionFlags;
    unsigned int width;
    unsigned int height;
    unsigned int scaleFactor; // scale * 100. 100 is 1x, 200 is 2x, etc.
    unsigned int pixelFormat; // 'ARGB' ('BGRA' in little endian), if it is 0x47413820 (GA8) then the colorspace will be gray or 'PDF ' if a pdf
    unsigned int colorspaceID:4; // colorspace ID. 0 for sRGB, all else for generic rgb, used only if pixelFormat 'ARGB'
    unsigned int reserved:28;
    struct _csimetadata {
        unsigned int modDate;  // modification date in seconds since 1970?
        unsigned short layout; // layout/type of the renditoin
        unsigned short reserved; // always zero
        char name[128];
    } metadata;
    unsigned int infolistLength; // size of the list of information after header but before bitmap
    struct _csibitmaplist {
        unsigned int bitmapCount;
        unsigned int reserved;
        unsigned int payloadSize; // size of all the proceeding information listLength + data
    } bitmaps;
};

#pragma mark - Colors

struct rgbquad {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct colorkey {
    unsigned int reserved;
    char name[128];
};

struct colordef {
    unsigned int version;
    unsigned int reserved;
    struct rgbquad value;
};

#pragma mark - Renditions

struct renditionkeyfmt {
    unsigned int magic; // 'kfmt'
    unsigned int reserved;
    unsigned int num_identifiers;
    unsigned int identifier_list[0];
};

struct renditionkeytoken {
    unsigned short identifier;
    unsigned short value;
};

#pragma mark - Types

//!TODO document this
struct csibitmap {
    unsigned int _field1; // magic?
    union {
        unsigned int _field1;
        struct _csibitmapflags {
            unsigned int :1;
            unsigned int :1;
            unsigned int :30;
        } flags;
    } _field2;
    unsigned int _field3;
    unsigned int _field4;
    unsigned char data[0];
};

struct slice {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

//!TODO Document this
struct cuieffectdata {
    unsigned int _field1;
    unsigned int _field2;
    unsigned int _field3;
    unsigned int num_effects;
    struct _cuieffectlist {
        unsigned int _field1;
        unsigned int _field2[0];
    } _field5;
};

struct _psdGradientColor {
    double red;
    double green;
    double blue;
    double alpha;
};

#endif /* TKStructs_h */