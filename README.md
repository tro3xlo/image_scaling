# image_scaling
Image scaling written in C.
- dst and src are source and destination buffers respectively,
- xsize_dst, ysize_dst, xsize_src and ysize_src are above buffers width and height
- xstart_dstrect, ystart_dstrect, xsize_dstrect, ysize_dstrect, xstart_srcrect, ystart_srcrect, xsize_srcrect and ysize_srcrect are destination and source rectangles
# Notes
- buffers need to represent 4 byte sized channels, 32 bits in total
- rectangle size ( xsize_dstrect, ysize_dstrect, xsize_srcrect and ysize_srcrect ) is calculated from the start component
  ( xstart_dstrect, ystart_dstrect, xstart_srcrect and ystart_srcrect ), not from the coordinate start, so
  start 10 and size 20 is ( 10, 20 ) not ( 10, 30 )
- scale is determined by the ratio of source rectangle size and destination rectangle size, so
  xsize_srcrect 10 and xsize_dstrect 20 will horizontally strech the source rectangle to destination rectangle,
  ysize_srcrect 20 and ysize_dstrect 10 will vertically skew the source rectangle to destination rectangle
- edge sampling increases computation time by at least 2 per axis if scale is not a whole number or the reciprocal value of scale is not a whole number.
