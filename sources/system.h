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
    u8  m_ucSoftVerMajor;       // ������汾(��汾)
    u8  m_ucSoftVerMinor;       // ����ΰ汾(С�汾)
    u16 m_usSoftVerBuild;       // ���B�汾(����/����汾)
    BUILD_DATE_ST m_stBuildDate;// �����������(yyyy-mm-dd)
    s8  m_scHardVerion;         // Ӳ���汾('A', 'B', ...)[��֧��Ӳ���汾�ŵĵ���, �̶�Ϊ'X']
    s8  m_scBomVersion;         // BOM�汾.
}VER_INFO_ST;



void System_Init(void);
extern u8 g_ucSysInitFault;
extern u8 g_ucBoardAddr; // TODO: ���ӵ����ַ����.

extern VER_INFO_ST g_stVerInfo;


#endif
// end of file.
