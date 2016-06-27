#ifndef __PUBLIC_TYPES_H
#define __PUBLIC_TYPES_H

// 通用类型定义(注释中为类型前缀)
// 为保证在不同机器间的移植, 尽可能不使用int(典型如32位ARM上int为32位, 8/16位上为16位)
typedef unsigned char   u8;         // uc
typedef unsigned short  u16;        // us
typedef unsigned long   u32;        // ul

typedef signed char     s8;         // sc
typedef signed short    s16;        // ss
typedef signed long     s32;        // sl

typedef unsigned char   bool;       // b

// 定义空指针
#ifndef NULL
#define NULL            ((void *)0)
#endif

// 定义bool型值
//#ifdef FALSE
//#undef FALSE
#define FALSE           0
//#endif
//#ifdef TRUE
//#undef TRUE
#define TRUE            1
//#endif

#define FAST_ISR __attribute__((interrupt, shadow,no_auto_psv))

#endif

// End Of File.


