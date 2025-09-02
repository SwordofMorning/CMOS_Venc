/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * IOCTL OS adaptation layer
 * 
 * Copyright (c) 2021 Shanghai Visinex Technologies Co., Ltd.
 */
#ifndef __VS_OS_IOCTL_H__
#define __VS_OS_IOCTL_H__

#define _VS_OS_IOC_NRBITS	8
#define _VS_OS_IOC_TYPEBITS	8

#ifndef _VS_OS_IOC_SIZEBITS
#define _VS_OS_IOC_SIZEBITS	14
#endif

#ifndef _VS_OS_IOC_DIRBITS
#define _VS_OS_IOC_DIRBITS	2
#endif

#define _VS_OS_IOC_NRMASK	((1 << _VS_OS_IOC_NRBITS)-1)
#define _VS_OS_IOC_TYPEMASK	((1 << _VS_OS_IOC_TYPEBITS)-1)
#define _VS_OS_IOC_SIZEMASK	((1 << _VS_OS_IOC_SIZEBITS)-1)
#define _VS_OS_IOC_DIRMASK	((1 << _VS_OS_IOC_DIRBITS)-1)

#define _VS_OS_IOC_NRSHIFT	0
#define _VS_OS_IOC_TYPESHIFT	(_VS_OS_IOC_NRSHIFT+_VS_OS_IOC_NRBITS)
#define _VS_OS_IOC_SIZESHIFT	(_VS_OS_IOC_TYPESHIFT+_VS_OS_IOC_TYPEBITS)
#define _VS_OS_IOC_DIRSHIFT	(_VS_OS_IOC_SIZESHIFT+_VS_OS_IOC_SIZEBITS)

#ifndef _VS_OS_IOC_NONE
#define _VS_OS_IOC_NONE	0U
#endif

#ifndef _VS_OS_IOC_WRITE
#define _VS_OS_IOC_WRITE	1U
#endif

#ifndef _VS_OS_IOC_READ
#define _VS_OS_IOC_READ	2U
#endif

#ifdef __CHECKER__
#define _VS_OS_IOC_TYPECHECK(t) (sizeof(t))
#else
	extern unsigned int __invalid_sz_arg_for_VS_OS_IOC;
#define _VS_OS_IOC_TYPECHECK(t) \
		((sizeof(t) == sizeof(t[1]) && \
		  sizeof(t) < (1 << _VS_OS_IOC_SIZEBITS)) ? \
		  sizeof(t) : __invalid_sz_arg_for_VS_OS_IOC)
#endif

#define _VS_OS_IOC(dir,type,nr,size) \
	(((dir)  << _VS_OS_IOC_DIRSHIFT) | \
	 ((type) << _VS_OS_IOC_TYPESHIFT) | \
	 ((nr)   << _VS_OS_IOC_NRSHIFT) | \
	 ((size) << _VS_OS_IOC_SIZESHIFT))

#define _VS_OS_IO(type,nr)		_VS_OS_IOC(_VS_OS_IOC_NONE,(type),(nr),0)
#define _VS_OS_IOR(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_READ,(type),(nr),(_VS_OS_IOC_TYPECHECK(size)))
#define _VS_OS_IOW(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_WRITE,(type),(nr),(_VS_OS_IOC_TYPECHECK(size)))
#define _VS_OS_IOWR(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_READ|_VS_OS_IOC_WRITE,(type),(nr),(_VS_OS_IOC_TYPECHECK(size)))
#define _VS_OS_IOR_BAD(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_READ,(type),(nr),sizeof(size))
#define _VS_OS_IOW_BAD(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_WRITE,(type),(nr),sizeof(size))
#define _VS_OS_IOWR_BAD(type,nr,size)	_VS_OS_IOC(_VS_OS_IOC_READ|_VS_OS_IOC_WRITE,(type),(nr),sizeof(size))

#define _VS_OS_IOC_DIR(nr)		(((nr) >> _VS_OS_IOC_DIRSHIFT) & _VS_OS_IOC_DIRMASK)
#define _VS_OS_IOC_TYPE(nr)		(((nr) >> _VS_OS_IOC_TYPESHIFT) & _VS_OS_IOC_TYPEMASK)
#define _VS_OS_IOC_NR(nr)		(((nr) >> _VS_OS_IOC_NRSHIFT) & _VS_OS_IOC_NRMASK)
#define _VS_OS_IOC_SIZE(nr)		(((nr) >> _VS_OS_IOC_SIZESHIFT) & _VS_OS_IOC_SIZEMASK)

#define VS_OS_IOC_IN		(_VS_OS_IOC_WRITE << _VS_OS_IOC_DIRSHIFT)
#define VS_OS_IOC_OUT		(_VS_OS_IOC_READ << _VS_OS_IOC_DIRSHIFT)
#define VS_OS_IOC_INOUT	((_VS_OS_IOC_WRITE|_VS_OS_IOC_READ) << _VS_OS_IOC_DIRSHIFT)
#define VS_OS_IOCSIZE_MASK	(_VS_OS_IOC_SIZEMASK << _VS_OS_IOC_SIZESHIFT)
#define VS_OS_IOCSIZE_SHIFT	(_VS_OS_IOC_SIZESHIFT)


#endif /* #ifndef __VS_OS_IOCTL_H__ */

