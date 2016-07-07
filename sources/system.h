#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define SW_VER_MAJOR  1
#define SW_VER_MINOR  0
#define SW_VER_BUILD  0

typedef struct tagBuildDate
{
    u16 m_usBuildYear;
    u8  m_ucBuildMonth;
    u8  m_ucBuildDate;
}BUILD_DATE_ST;

typedef struct tagVerInfo
{
    u8  m_ucSoftVerMajor;       // 软件主版本(大版本)
    u8  m_ucSoftVerMinor;       // 软件次版本(小版本)
    u16 m_usSoftVerBuild;       // 软件B版本(构建/编译版本)
    BUILD_DATE_ST m_stBuildDate;// 软件构建日期(yyyy-mm-dd)
    s8  m_scHardVerion;         // 硬件版本('A', 'B', ...)[不支持硬件版本号的单板, 固定为'X']
    s8  m_scBomVersion;         // BOM版本.
}VER_INFO_ST;



void System_Init(void);
extern u8 g_ucSysInitFault;
extern u8 g_ucBoardAddr; // TODO: 增加单板地址配置.

extern VER_INFO_ST g_stVerInfo;


#endif
// end of file.
