#ifndef IMAGE_SCALING_C
#define IMAGE_SCALING_C

//#define IGNORE_EDGE_SAMPLING

void scale_image (
 /* PROC PARAM */
 unsigned char * dst, int xsize_dst, int ysize_dst, int xstart_dstrect, int ystart_dstrect, int xsize_dstrect, int ysize_dstrect,
 unsigned char * src, int xsize_src, int ysize_src, int xstart_srcrect, int ystart_srcrect, int xsize_srcrect, int ysize_srcrect
) {
 /* PROC VAR */
 float channel [ 4 ];
 float intensity;
 float xscale;
 float yscale;
#ifndef IGNORE_EDGE_SAMPLING
 float xedge;
 float yedge;
 float average;
#endif
 int xstart;
 int ystart;
 int xsize;
 int ysize;
 int xadjust;
 int yadjust;
 unsigned char * start;
 unsigned char * end;
 unsigned char * dstpos;
 unsigned char * srcpos;
 /* INITIAL CHECKS */
 if ( ! dst ) return;
 if ( xsize_dst <= 0 ) return;
 if ( ysize_dst <= 0 ) return;
 if ( ! src ) return;
 if ( xsize_src <= 0 ) return;
 if ( ysize_src <= 0 ) return;
 if ( xstart_dstrect >= xsize_dst ) return;
 if ( ystart_dstrect >= ysize_dst ) return;
 if ( xstart_dstrect < 0 ) {
  xsize_dstrect += xstart_dstrect;
  xstart_dstrect = 0;
 }
 if ( xstart_dstrect + xsize_dstrect > xsize_dst ) xsize_dstrect = xsize_dst - xstart_dstrect;
 if ( xsize_dstrect <= 0 ) return;
 if ( ystart_dstrect < 0 ) {
  ysize_dstrect += ystart_dstrect;
  ystart_dstrect = 0;
 }
 if ( ystart_dstrect + ysize_dstrect > ysize_dst ) ysize_dstrect = ysize_dst - ystart_dstrect;
 if ( ysize_dstrect <= 0 ) return;

 if ( xstart_srcrect < 0 || ystart_srcrect < 0 || xstart_srcrect >= xsize_src || ystart_srcrect >= ysize_src ) return;
 if ( xsize_srcrect <= 0 || ysize_srcrect <= 0 ) return;
 /* INITIAL VAR SET */
 xscale = ( float ) xsize_srcrect / ( float ) xsize_dstrect;
 yscale = ( float ) ysize_srcrect / ( float ) ysize_dstrect;
 xadjust = ( int ) ( ( ( ( xscale < 1.0f ) ? ( 1.0f / xscale ) : xscale ) - 1.0f ) / 2.0f );
 yadjust = ( int ) ( ( ( ( yscale < 1.0f ) ? ( 1.0f / yscale ) : yscale ) - 1.0f ) / 2.0f );
#ifndef IGNORE_EDGE_SAMPLING
 xedge = ( ( ( ( xscale < 1.0f ) ? ( 1.0f / xscale ) : xscale ) - 1.0f ) - ( int ) ( ( ( ( xscale < 1.0f ) ? ( 1.0f / xscale ) : xscale ) - 1.0f ) ) ) / 2.0;
 yedge = ( ( ( ( yscale < 1.0f ) ? ( 1.0f / yscale ) : yscale ) - 1.0f ) - ( int ) ( ( ( ( yscale < 1.0f ) ? ( 1.0f / yscale ) : yscale ) - 1.0f ) ) ) / 2.0;
 average = ( yedge && xedge ) ? ( ( yedge + xedge ) / 2.0f ) : 0.0f;
#endif
 for ( int ycntdst = 0; ycntdst < ysize_dstrect; ycntdst ++ ) {
  for ( int xcntdst = 0; xcntdst < xsize_dstrect; xcntdst ++ ) {
   channel [ 0 ] = channel [ 1 ] = channel [ 2 ] = channel [ 3 ] = intensity = 0.0f;
   xstart = xstart_srcrect + ( int ) ( ( float ) xcntdst * xscale );
   xsize = 1;
   if ( xadjust ) {
    xstart -= xadjust;
    xsize += 2 * xadjust;
   }
   if ( xstart < 0 ) {
    xsize += xstart;
    xstart = 0;
   }
   if ( xstart + xsize > xsize_src ) xsize = xsize_src - xstart;
   if ( xsize <= 0 ) continue;
   ystart = ystart_srcrect + ( int ) ( ( float ) ycntdst * yscale );
   ysize = 1;
   if ( yadjust ) {
    ystart -= yadjust;
    ysize += 2 * yadjust;
   }
   if ( ystart < 0 ) {
    ysize += ystart;
    ystart = 0;
   }
   if ( ystart + ysize > ysize_src ) ysize = ysize_src - ystart;
   if ( ysize <= 0 ) continue;
   srcpos = & src [ ( xstart + xsize_src * ystart ) << 2 ];
   if ( xstart < xsize_src && ystart < ysize_src ) {
    intensity += ( float ) ( xsize * ysize );
    for ( int ycnt = 0; ycnt < ysize; ycnt ++ ) {
     start = srcpos + ( ( xsize_src * ycnt ) << 2 );
     end = start + ( xsize << 2 );
     while ( start < end ) {
      channel [ 0 ] += * start;
      start ++;
      channel [ 1 ] += * start;
      start ++;
      channel [ 2 ] += * start;
      start ++;
      channel [ 3 ] += * start;
      start ++;
     }
    }
   }

#ifndef IGNORE_EDGE_SAMPLING
   if ( yedge ) {
    if ( ystart - 1 >= 0 ) {
     intensity += xsize * yedge;
     start = srcpos - ( xsize_src << 2 );
     end = start + ( xsize << 2 );
     while ( start < end ) {
      channel [ 0 ] += * start * yedge;
      start ++;
      channel [ 1 ] += * start * yedge;;
      start ++;
      channel [ 2 ] += * start * yedge;;
      start ++;
      channel [ 3 ] += * start * yedge;;
      start ++;
     }
    }
    if ( ystart + 1 < ysize_src ) {
     intensity += xsize * yedge;
     start = srcpos + ( xsize_src << 2 );
     end = start + ( xsize << 2 );
     while ( start < end ) {
      channel [ 0 ] += * start * yedge;
      start ++;
      channel [ 1 ] += * start * yedge;;
      start ++;
      channel [ 2 ] += * start * yedge;;
      start ++;
      channel [ 3 ] += * start * yedge;;
      start ++;
     }
    }
   }

   if ( xedge ) {
    if ( xstart - 1 >= 0 ) {
     intensity += ysize * xedge;
     start = srcpos - 4;
     end = start + ( ( xsize_src * ysize ) << 2 );
     while ( start < end ) {
      channel [ 0 ] += * start * xedge;
      start ++;
      channel [ 1 ] += * start * xedge;
      start ++;
      channel [ 2 ] += * start * xedge;
      start ++;
      channel [ 3 ] += * start * xedge;
      start ++;
      start += ( ( xsize_src - 1 ) << 2 );
     }
    }
    if ( xstart + 1 < xsize_src ) {
     intensity += ysize * xedge;
     start = srcpos + 4;
     end = start + ( ( xsize_src * ysize ) << 2 );
     while ( start < end ) {
      channel [ 0 ] += * start * xedge;
      start ++;
      channel [ 1 ] += * start * xedge;
      start ++;
      channel [ 2 ] += * start * xedge;
      start ++;
      channel [ 3 ] += * start * xedge;
      start ++;
      start += ( ( xsize_src - 1 ) << 2 );
     }
    }
   }

   if ( average ) {
    if ( xstart - 1 >= 0 ) {
     if ( ystart - 1 >= 0 ) {
      start = srcpos - ( xsize_src << 4 ) - 4;
      intensity += average;
      channel [ 0 ] += * start * average;
      start ++;
      channel [ 1 ] += * start * average;
      start ++;
      channel [ 2 ] += * start * average;
      start ++;
      channel [ 3 ] += * start * average;
     }
     if ( ystart + 1 < ysize_src ) {
      start = srcpos + ( xsize_src << 4 ) - 4;
      intensity += average;
      channel [ 0 ] += * start * average;
      start ++;
      channel [ 1 ] += * start * average;
      start ++;
      channel [ 2 ] += * start * average;
      start ++;
      channel [ 3 ] += * start * average;
     }
    }
    if ( xstart + 1 < xsize_src ) {
     if ( ystart - 1 >= 0 ) {
      start = srcpos - ( xsize_src << 4 ) + 4;
      intensity += average;
      channel [ 0 ] += * start * average;
      start ++;
      channel [ 1 ] += * start * average;
      start ++;
      channel [ 2 ] += * start * average;
      start ++;
      channel [ 3 ] += * start * average;
     }
     if ( ystart + 1 < ysize_src ) {
      start = srcpos + ( xsize_src << 4 ) + 4;
      intensity += average;
      channel [ 0 ] += * start * average;
      start ++;
      channel [ 1 ] += * start * average;
      start ++;
      channel [ 2 ] += * start * average;
      start ++;
      channel [ 3 ] += * start * average;
     }
    }
   }
#endif

   if ( intensity ) {
    dstpos = & dst [ ( ( xstart_dstrect + xcntdst + xsize_dst * ( ystart_dstrect + ycntdst ) ) << 2 ) ];
    * dstpos ++ = channel [ 0 ] / intensity;
    * dstpos ++ = channel [ 1 ] / intensity;
    * dstpos ++ = channel [ 2 ] / intensity;
    * dstpos = channel [ 3 ] / intensity;
   }
  }
 }
}

#endif