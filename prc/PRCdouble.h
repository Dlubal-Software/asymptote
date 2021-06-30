#ifndef __PRC_DOUBLE_H
#define __PRC_DOUBLE_H

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace prc {

// some mumbo-jumbo follows
// we use PRC_BIG_ENDIAN to identify endiannes in PRC code
// if WORDS_BIGENDIAN was not set by configure/cmake due to
// system being little endian or code being compiled without configuration stage
// try predefined macros

#ifdef WORDS_BIGENDIAN
# define PRC_BIG_ENDIAN 1
#else
# if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
#  if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#   define PRC_BIG_ENDIAN 1
#  endif
# else   
#  if defined(__BIG_ENDIAN__)
#   define PRC_BIG_ENDIAN 1
#  endif
# endif
#endif

// from Adobe's documentation

union ieee754_double
{
 double d;
 /* This is the IEEE 754 double-precision format. */
 struct
 {
#ifdef PRC_BIG_ENDIAN
  unsigned int negative:1;
  unsigned int exponent:11;
  /* Together these comprise the mantissa.  */
  unsigned int mantissa0:20;
  unsigned int mantissa1:32;
#else
  /* Together these comprise the mantissa.  */
  unsigned int mantissa1:32;
  unsigned int mantissa0:20;
  unsigned int exponent:11;
  unsigned int negative:1;
#endif
 } ieee;
};

union ieee754_float
{
 float f;
 /* This is the IEEE 754 float-precision format. */
 struct {
#ifdef PRC_BIG_ENDIAN
 unsigned int negative:1;
 unsigned int exponent:8;
 unsigned int mantissa:23;
#else
 unsigned int mantissa:23;
 unsigned int exponent:8;
 unsigned int negative:1;
#endif
 } ieee;
};

enum ValueType {VT_double,VT_exponent};

struct sCodageOfFrequentDoubleOrExponent
{
  short Type;
  short NumberOfBits;
  unsigned Bits;
  union {
    unsigned ul[2];
    double Value;
  } u2uod;
};

#ifdef PRC_BIG_ENDIAN
#       define DOUBLEWITHTWODWORD(upper,lower)  upper,lower
#       define UPPERPOWER       (0)
#       define LOWERPOWER       (!UPPERPOWER)

#       define NEXTBYTE(pbd)                            ((pbd)++)
#       define PREVIOUSBYTE(pbd)                ((pbd)--)
#       define MOREBYTE(pbd,pbend)              ((pbd)<=(pbend))
#       define OFFSETBYTE(pbd,offset)   ((pbd)+=offset)
#       define BEFOREBYTE(pbd)                  ((pbd)-1)
#       define DIFFPOINTERS(p1,p2)              ((p1)-(p2))
#       define SEARCHBYTE(pbstart,b,nb) (unsigned char *)memrchr((pbstart),(b),(nb))
#       define BYTEAT(pb,i)     *((pb)-(i))
#else
#       define DOUBLEWITHTWODWORD(upper,lower)  lower,upper
#       define UPPERPOWER       (1)
#       define LOWERPOWER       (!UPPERPOWER)

#       define NEXTBYTE(pbd)                            ((pbd)--)
#       define PREVIOUSBYTE(pbd)                ((pbd)++)
#       define MOREBYTE(pbd,pbend)              ((pbd)>=(pbend))
#       define OFFSETBYTE(pbd,offset)   ((pbd)-=offset)
#       define BEFOREBYTE(pbd)                  ((pbd)+1)
#       define DIFFPOINTERS(p1,p2)              ((unsigned)((p2)-(p1)))
#       define SEARCHBYTE(pbstart,b,nb) (unsigned char *)memchr((pbstart),(b),(nb))
#       define BYTEAT(pb,i)     *((pb)+(i))
#endif

#define MAXLENGTHFORCOMPRESSEDTYPE      ((22+1+1+4+6*(1+8))+7)/8

#define NEGATIVE(d)     (((union ieee754_double *)&(d))->ieee.negative)
#define EXPONENT(d)     (((union ieee754_double *)&(d))->ieee.exponent)
#define MANTISSA0(d)    (((union ieee754_double *)&(d))->ieee.mantissa0)
#define MANTISSA1(d)    (((union ieee754_double *)&(d))->ieee.mantissa1)

typedef unsigned char PRCbyte;
typedef unsigned short PRCword;
typedef unsigned PRCdword;

extern PRCdword stadwZero[2],stadwNegativeZero[2];

#define NUMBEROFELEMENTINACOFDOE   (2077)

#ifdef PRC_BIG_ENDIAN
#       define DOUBLEWITHTWODWORDINTREE(upper,lower)    {upper,lower} 
#else
#       define DOUBLEWITHTWODWORDINTREE(upper,lower)    {lower,upper}
#endif
extern sCodageOfFrequentDoubleOrExponent acofdoe[NUMBEROFELEMENTINACOFDOE];
/*
struct sCodageOfFrequentDoubleOrExponent* getcofdoe(unsigned,short);
*/
#define STAT_V
#define STAT_DOUBLE

int stCOFDOECompare(const void*,const void*);

#ifdef PRC_BIG_ENDIAN
#ifndef HAVE_MEMRCHR
void *memrchr(const void *,int,size_t);
#endif
#endif

} // namespace prc

#endif // __PRC_DOUBLE_H
