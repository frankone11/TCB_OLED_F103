/*
 * arial.h
 *
 *  Created on: Nov 25, 2023
 *      Author: paco
 */

#ifndef INC_ARIAL_H_
#define INC_ARIAL_H_

 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
 typedef struct {
     long int code;
     const tImage *image;
     const int image_left;
     const int image_top;
     const int char_width;
     const int char_height;
     } tChar;
 typedef struct {
     int length;
     const tChar *chars;
     } tFont;

#ifndef _NO_EXT_
extern const tFont Arial;
#endif //_NO_EXT_


#endif /* INC_ARIAL_H_ */
