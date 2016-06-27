#ifndef __PUBLIC_TYPES_H
#define __PUBLIC_TYPES_H

// ͨ�����Ͷ���(ע����Ϊ����ǰ׺)
// Ϊ��֤�ڲ�ͬ���������ֲ, �����ܲ�ʹ��int(������32λARM��intΪ32λ, 8/16λ��Ϊ16λ)
typedef unsigned char   u8;         // uc
typedef unsigned short  u16;        // us
typedef unsigned long   u32;        // ul

typedef signed char     s8;         // sc
typedef signed short    s16;        // ss
typedef signed long     s32;        // sl

typedef unsigned char   bool;       // b

// �����ָ��
#ifndef NULL
#define NULL            ((void *)0)
#endif

// ����bool��ֵ
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


