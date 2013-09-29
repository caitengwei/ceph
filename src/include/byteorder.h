/*
 * byteorder.h
 *
 * LGPL 2
 */

#ifndef CEPH_BYTEORDER_H
#define CEPH_BYTEORDER_H

#include "acconfig.h"

#include "include/int_types.h"

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

static __inline__ __u16 swab16(__u16 val) 
{
  return (val >> 8) | (val << 8);
}
static __inline__ __u32 swab32(__u32 val) 
{
  return (( val >> 24) |
	  ((val >> 8)  & 0xff00) |
	  ((val << 8)  & 0xff0000) | 
	  ((val << 24)));
}
static __inline__ uint64_t swab64(uint64_t val) 
{
  return (( val >> 56) |
	  ((val >> 40) & 0xff00ull) |
	  ((val >> 24) & 0xff0000ull) |
	  ((val >> 8)  & 0xff000000ull) |
	  ((val << 8)  & 0xff00000000ull) |
	  ((val << 24) & 0xff0000000000ull) |
	  ((val << 40) & 0xff000000000000ull) |
	  ((val << 56)));
}

#ifndef BYTE_ORDER
# ifdef WORDS_BIGENDIAN
#  define BYTE_ORDER BIG_ENDIAN
# else
#  define BYTE_ORDER LITTLE_ENDIAN
# endif
#endif

// mswab == maybe swab (if not LE)
#if BYTE_ORDER == BIG_ENDIAN
# define mswab64(a) swab64(a)
# define mswab32(a) swab32(a)
# define mswab16(a) swab16(a)
#else
# if BYTE_ORDER != LITTLE_ENDIAN
#  warning BYTE_ORDER is not defined, assuming little endian
# endif
# define mswab64(a) (a)
# define mswab32(a) (a)
# define mswab16(a) (a)
#endif


#define MAKE_LE_CLASS(bits)						\
  struct ceph_le##bits {							\
    __u##bits v;							\
    ceph_le##bits &operator=(__u##bits nv) {				\
      v = mswab##bits(nv);						\
      return *this;							\
    }									\
    operator __u##bits() const { return mswab##bits(v); }		\
  } __attribute__ ((packed));						\
  static inline bool operator==(ceph_le##bits a, ceph_le##bits b) {		\
    return a.v == b.v;							\
  }
  
MAKE_LE_CLASS(64)
MAKE_LE_CLASS(32)
MAKE_LE_CLASS(16)
#undef MAKE_LE_CLASS

#define init_le64(x) { (__u64)mswab64(x) }
#define init_le32(x) { (__u32)mswab32(x) }
#define init_le16(x) { (__u16)mswab16(x) }

  /*
#define cpu_to_le64(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)
  */
#define le64_to_cpu(x) ((uint64_t)x)
#define le32_to_cpu(x) ((__u32)x)
#define le16_to_cpu(x) ((__u16)x)

#endif
