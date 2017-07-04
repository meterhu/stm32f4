//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            文件名称:              
*
*                摘要: 
*
*            创建时间: 2012-05-20  
*
*            修改时间: 2012-05-20 
*            修改原因:
*
*            
*
*                作者: 杨卫华
*
*************************************************************************************************/
//===============================================================================================

#ifndef __MESSAGE_H
#define __MESSAGE_H


//================================================================================
typedef long int       LRESULT;
typedef uint32_t       WPARAM;
typedef long int       LPARAM;

struct _AFX_MSGMAP_ENTRY;
struct _AFX_MSGMAP;

//typedef struct _AFX_MSGMAP
//{
//        struct _AFX_MSGMAP*       pBaseMessageMap;
//        struct _AFX_MSGMAP_ENTRY* lpEntries;
//}AFX_MSGMAP;

enum AfxSig
{
        AfxSig_end = 0,     // [marks end of message map]
        AfxSig_vv,
};

typedef void (*AFX_PMSG)(WM_MESSAGE * pMsg);

typedef struct _AFX_MSGMAP_ENTRY  // MFC 4.0 format
{
    uint32_t nMessage; // windows message
    uint32_t nCode;    // control code or WM_NOTIFY code
    uint32_t nID;      // control ID (or 0 for windows messages)
    uint32_t nLastID;  // used for entries specifying a range of control id's
    uint32_t nSig;     // signature type (action) or pointer to message #
    AFX_PMSG pfn;      // routine to call (or special value)
    
}AFX_MSGMAP_ENTRY;


#define ON_COMMAND(id, memberFxn) \
   { WM_NOTIFY_PARENT, WM_NOTIFICATION_CLICKED, (uint32_t)id, (uint32_t)id, AfxSig_vv, (AFX_PMSG)memberFxn },


#define ON_WM_CREATE(memberFxn) \
   { WM_CREATE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)memberFxn },

#define ON_WM_PAINT(memberFxn) \
   { WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)memberFxn },


#define ON_WM_INIT_DIALOG(memberFxn) \
   { WM_INIT_DIALOG, 0, 0, 0, AfxSig_vv, (AFX_PMSG)memberFxn },

#define ON_WM_KEY(memberFxn) \
   { WM_WM_KEY, 0, 0, 0, AfxSig_vv, (AFX_PMSG)memberFxn },


//ON_NOTIFY_REFLECT

#define DECLARE_MESSAGE_MAP() \
        static AFX_MSGMAP_ENTRY _messageEntries[]; \
        /////AFX_MSGMAP* GetMessageMap();


#define BEGIN_MESSAGE_MAP() \
        AFX_MSGMAP_ENTRY* GetMessageMap(void) \
                { return _messageEntries; } \
        static AFX_MSGMAP_ENTRY _messageEntries[] = \
        {

#define END_MESSAGE_MAP() \
        { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
        };




#define WM_COMMAND     0x0111  // following windows.h
#define WM_CREATE      0x0001
#define WM_PAINT       0x000F
#define WM_NOTIFY      0x004E

#define CN_COMMAND     0x33


extern BOOL WinMsgProc(WM_MESSAGE * pMsg, AFX_MSGMAP_ENTRY*lpEntry) ;


#endif


