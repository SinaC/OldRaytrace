#ifndef __TYPES_INTEGER_H__
#define __TYPES_INTEGER_H__

//! Integer representation of a floating-point value.
#define IR(x)	((u32&)x)
//! Integer-based comparison of a floating point value.
//! Don't use it blindly, it can be faster or slower than the FPU comparison, depends on the context.
#define IS_NEGATIVE_FLOAT(x)	(IR(x)&0x80000000)

#endif
