#ifndef CURSO_IOCTL_H
#define CURSO_IOCTL_H

#define HSMKM_GETCASIZE_CMD 1
#define HSMKM_GETCA_CMD 2

#define IOCTL_HSMKM_GETCASIZE _IOR('h', HSMKM_GETCASIZE_CMD, int *)
#define IOCTL_HSMKM_GETCA _IOR('h', HSMKM_GETCA_CMD, unsigned char *)

#endif 
