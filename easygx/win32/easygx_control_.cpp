
#include <string.h>

#include "easygx.h"
#include "easygx_control.h"
#include "easygx_log.h"

#include "easygx_private.h"

#include "Windows.h"
#include "Windowsx.h"
#include "CommCtrl.h"
#include "Shlobj.h"

#ifdef __cplusplus
extern "C" {
#endif

//1. Label
egx_wnd_t egx_label_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	//sfpr_log(easygx_log,SFPR_LOG_INFO,"egx_label_create_() | parent: 0x%x ---> 0x%x\n",parent,GUIWND_TO_HWND(parent));
	hWnd = CreateWindow("STATIC",name,WS_CHILD|WS_VISIBLE,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

int egx_label_set_text_(egx_wnd_t hwnd,char *text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	SetWindowText(hWnd,text);
}


//2. Button
int egx_button_enable_(egx_wnd_t hwnd,int enable)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	Button_Enable(hWnd,enable);
	return 0;
}

int egx_button_checked_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int state = 0;
	state = Button_GetCheck(hWnd);
	return state;
}

egx_wnd_t egx_pushbutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_checkbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|WS_BORDER|BS_AUTOCHECKBOX,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_groupbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

egx_wnd_t egx_radiobutton_begin_(egx_wnd_t parent)
{
	return 0;
}

egx_wnd_t egx_radiobutton_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	if(style & EGX_STYLE_GROUP){
		hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_GROUP,
					x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	}else{
		hWnd = CreateWindow("Button",name,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);		
	}
	return HWND_TO_GUIWND(hWnd);	
}


//3. Edit
egx_wnd_t egx_edit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_BORDER,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, NULL);
	//GetClassName(hWnd,buff,31);
	return HWND_TO_GUIWND(hWnd);
}

int egx_edit_get_text_(egx_wnd_t hwnd,char *text,int length)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	*((LPWORD)text) = length;
	return SendMessage(hWnd,EM_GETLINE,0,(LPARAM)text);
}

int egx_edit_set_text_(egx_wnd_t hwnd,char *text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	SetWindowText(hWnd,text);
	return 0;
}

egx_wnd_t egx_multiedit_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("EDIT","",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_AUTOHSCROLL,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, NULL);
	return HWND_TO_GUIWND(hWnd);
}


//4. ListBox
egx_wnd_t egx_listbox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("LISTBOX","",WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_SORT|LBS_DISABLENOSCROLL|WS_VSCROLL,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, NULL);
	return HWND_TO_GUIWND(hWnd);
}

int egx_listbox_get_count_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	return ListBox_GetCount(hWnd);
}

int egx_listbox_add_item_(egx_wnd_t hwnd,char *name,void* data)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int index = 256;
	index = ListBox_AddString(hWnd,name);
	ListBox_SetItemData(hWnd,index,data);
	return 0;
}

int egx_listbox_get_selected_item_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);	
	return ListBox_GetCurSel(hWnd);
}

int egx_listbox_get_selected_name_(egx_wnd_t hwnd,char *buffer,int length)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int index = 0;
	index = ListBox_GetCurSel(hWnd);
	*((LPWORD)buffer) = length;
	ListBox_GetText(hWnd,index,(LPARAM)buffer);
	return 0;
}

void* egx_listbox_get_selected_data_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int index = 0;
	index = ListBox_GetCurSel(hWnd);	
	return (void*)ListBox_GetItemData(hWnd,index);
}

//5. ComboBox
egx_wnd_t egx_combobox_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("COMBOBOX","",WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|CBS_DROPDOWN,
				x,y,width,height,
				(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, NULL);
	return HWND_TO_GUIWND(hWnd);
}

int egx_combobox_get_count_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	return ComboBox_GetCount(hWnd);
}

int egx_combobox_add_item_(egx_wnd_t hwnd,char *name,void* data)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int index = 256;
	index = ComboBox_AddString(hWnd,name);
	ComboBox_SetItemData(hWnd,index,data);
	return 0;
}

int egx_combobox_set_selected_item_(egx_wnd_t hwnd,int index)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);	
	ComboBox_SetCurSel(hWnd,index);
	return 0;
}

int egx_combobox_get_selected_item_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);	
	return ComboBox_GetCurSel(hWnd);
}

int egx_combobox_get_selected_name_(egx_wnd_t hwnd,char *buffer,int length)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	ComboBox_GetText(hWnd,buffer,length);
	return 0;
}

void* egx_combobox_get_selected_data_(egx_wnd_t hwnd)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int index = 0;
	index = ComboBox_GetCurSel(hWnd);	
	return (void*)ComboBox_GetItemData(hWnd,index);
}

//6. Tree
egx_wnd_t egx_tree_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(WC_TREEVIEW,name,WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	/*
	HIMAGELIST SmallIcons;	
	Shell_GetImageLists(NULL,&SmallIcons);
	TreeView_SetImageList(hWnd,SmallIcons,TVSIL_NORMAL);
	*/
	return HWND_TO_GUIWND(hWnd);
}

int egx_tree_insert_item_(egx_wnd_t hwnd,int level,char* text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
    TVINSERTSTRUCT tvins; 	
	HTREEITEM hPrev;
	static HTREEITEM hParent = TreeView_GetRoot(hWnd);

	memset(&tvins,0,sizeof(TVINSERTSTRUCT));
	tvins.hParent = hParent; 
	tvins.hInsertAfter = NULL;
	tvins.item.mask = TVIF_TEXT  | TVIF_IMAGE;// | TVIF_SELECTEDIMAGE; 
	tvins.item.pszText = text; 
 //   tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
//	tvi.lParam = (LPARAM)level; 
	
	hPrev = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 
        0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
//	printf("=======parent:%x  item:%x   error:%d\n",hParent,hPrev,GetLastError());
    hParent = hPrev;
	return 0;
}

//7. List
egx_wnd_t egx_list_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(WC_LISTVIEW,name,WS_CHILD|WS_VISIBLE|WS_BORDER| LVS_REPORT | LVS_EDITLABELS,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

//8. Table
egx_wnd_t egx_table_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(WC_LISTVIEW,name,WS_CHILD|WS_VISIBLE|WS_BORDER| LVS_REPORT | LVS_EDITLABELS,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

int egx_table_init_column_(egx_wnd_t hwnd,int column,int width,char *name)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = column;
	lvc.cx = width;
	lvc.pszText = name;
	ListView_InsertColumn(hWnd,column, &lvc);
	return 0;
}

int egx_table_init_item_(egx_wnd_t hwnd,int line,int max_column)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	int count;
	LVITEM lvI;
	lvI.mask  = LVIF_TEXT | LVIF_STATE;
	lvI.iItem = line;
	lvI.pszText  = (LPSTR)"";
	lvI.stateMask = 0;
	lvI.state  = 0;
	lvI.iSubItem  = 0;
	ListView_InsertItem(hWnd, &lvI);	
	for(count = 0;count <max_column;count++){
		ListView_SetItemText(hWnd,line,count,(LPSTR)"");
	}
	return 0;
}

int egx_table_set_item_data_(egx_wnd_t hwnd,int row,int column,char *text)
{
	HWND hWnd = GUIWND_TO_HWND(hwnd);
	if(!text){
		return -1;
	}
	if(column <= 0){
		LVITEM lvI;
		lvI.mask  = LVIF_TEXT | LVIF_STATE;
		lvI.iItem = row;
		lvI.pszText   = text;
		lvI.stateMask = 0;
		lvI.state = 0;
		lvI.iSubItem  = 0;
		ListView_SetItem(hWnd, &lvI);
	}else{
		ListView_SetItemText(hWnd,row,column,text);
	}
	return 0;
}

//9. TextArea
#ifndef SS_EDITCONTROL
#define SS_EDITCONTROL 0x2000
#endif
egx_wnd_t egx_textarea_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("STATIC",name,WS_CHILD|WS_VISIBLE|SS_EDITCONTROL,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

//10. Image
egx_wnd_t egx_image_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow("STATIC",name,WS_CHILD|WS_VISIBLE|SS_BITMAP,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

//11. ProgressBar
egx_wnd_t egx_progressbar_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(PROGRESS_CLASS,name,WS_CHILD|WS_VISIBLE,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

//12. DateTime
egx_wnd_t egx_datetime_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(DATETIMEPICK_CLASS,name,WS_CHILD|WS_VISIBLE,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

//13. IpAddr
egx_wnd_t egx_ipaddr_create_(int res_id,char *name,egx_uint32_t style,int x,int y,int width,int height,egx_wnd_t parent)
{
	HWND hWnd;
	hWnd = CreateWindow(WC_IPADDRESS,name,WS_CHILD|WS_VISIBLE,
				x,y,width,height,(parent)?(GUIWND_TO_HWND(parent)):0, (HMENU)res_id, 0, 0);
	return HWND_TO_GUIWND(hWnd);
}

#ifdef __cplusplus
}
#endif
