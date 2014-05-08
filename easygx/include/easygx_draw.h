/**
 *\file easygx_draw.h
 *\brief easygx¿â»æÍ¼º¯Êý
 *\version 3.0.0
 *\date 2014.04.03
 */
#ifndef EASYGX_DRAW_H
#define EASYGX_DRAW_H

#include "easygx.h"

#ifdef __cplusplus
extern "C" {
#endif

int egx_paint_begin(egx_frame_t *frame);

int egx_paint_end(egx_frame_t *frame);

int egx_draw_line(egx_frame_t *frame,int x0,int y0,int x1,int y1);

int egx_draw_ellipse(egx_frame_t *frame,int x0,int y0,int widget,int width);

#ifdef __cplusplus
}
#endif

#endif
