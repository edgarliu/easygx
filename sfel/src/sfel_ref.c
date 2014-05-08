
#include "sfel.h"
#include "sfel_ref.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 * SFEL�������б�
***************************************************************************************************/

/**************************************************************************************************
 * SFEL���Ա�ǩ����ǩ�����б�
***************************************************************************************************/

static sfel_tag_attr_t tag_attr_sfel[] = {
	TAG_ATTR_COMMON
	{"lang",0,0,"��ǰ����"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_script[] = {
	TAG_ATTR_COMMON
	{"lang",0,0,"�ű�����"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_desktop[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"mainwnd",0,0,"������"},
	{"bgcolor",0,0,"����ɫ"},
	{"bgimage",0,0,"����ͼƬ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_welcome[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_window[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"bgcolor",0,0,"����ɫ"},
	{"bgimage",0,0,"����ͼƬ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_dialog[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"bgcolor",0,0,"����ɫ"},
	{"bgimage",0,0,"����ͼƬ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_title[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_view[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"desc",0,0,"��ʾ������"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_status[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_request[] = {
	TAG_ATTR_COMMON
	{"request",0,0,"�ͻ������������������ʱ�� abs_path"},
	{"path_type",0,0,"URL��·��(abs_path)������"},
	{"req_args",0,0,"�������"},
	{"Receiver",0,0,"�����ߵ�ID��window����dialog"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_submit[] = {
	TAG_ATTR_COMMON
	{"request",0,0,"�ͻ������������������ʱ�� abs_path"},
	{"path_type",0,0,"URL��·��(abs_path)������"},
	{"trans_type",0,0,"Server�˺�Client�����ݽ�����ʽ"},
	{"req_args",0,0,"�������"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_button[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"type",0,0,"���������ͣ�push��check��radio"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_checkbox[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_groupbox[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_radiobutton[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_label[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_edit[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"type",0,0,"Password/ text"},
	{"value",0,0,"����ؼ���ֵ"},
	{"maxlength",0,0,"Ϊ�ı������������ָ����������ַ�����"},
	{"readonly",0,0,"��������ؼ�Ϊֻ��"},
	{"min",0,0,"��Сֵ"},
	{"max",0,0,"���ֵ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_textarea[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"value",0,0,"����ؼ���ֵ"},
	{"maxlength",0,0,"Ϊ�ı��������ָ����������ַ�����"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_multiedit[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"value",0,0,"����ؼ���ֵ"},
	{"maxlength",0,0,"Ϊ�ı������������ָ����������ַ�����"},
	{"readonly",0,0,"��������ؼ�Ϊֻ��"},
	{"min",0,0,"��Сֵ"},
	{"max",0,0,"���ֵ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_image[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_listbox[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"min",0,0,"��Сֵ"},
	{"max",0,0,"���ֵ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_combobox[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"min",0,0,"��Сֵ"},
	{"max",0,0,"���ֵ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_option[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_list[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_litem[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"image",0,0,"�б����Ӧ��ͼ��"},
	{"mode",0,0,"��ʾ�ı�����ͼ��"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_table[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"ab",0,0,"��д"},
	{"mode",0,0,"���ģʽ�������ַ�ʽ��Common����ͨģʽ	Smot���򵥱��ģʽ"},
	{"row",0,0,"�������"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_caption[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_tr[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_th[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_tf[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_td[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"isigame",0,0,"�Ƿ���ͼƬ��ʾ"},
	{"rowspan",0,0,"�ϲ�ͬһ�е�Ԫ��"},
	{"colspan",0,0,"�ϲ�ͬһ�е�Ԫ��"},
	{"image",0,0,"��ͼ����ʾ��ͼƬ�ļ���"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_tc[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_tnote[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_font[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"face",0,0,"��������"},
	{"size",0,0,"�����С"},
	{"color",0,0,"������ɫ"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_italic[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_boldfaced[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_progressbar[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_date[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_ipaddress[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

sfel_tag_t sfel_tags_basic[] = {
	/****basic-basic********/
	{"sfel",				"basic",		"basic",			0,0,tag_attr_sfel,"sfel���Ը���ǩ"},
	{"include",				"basic",		"basic",			0,0,NULL,"��������sfel�ļ�"},
	{"script",				"basic",		"basic",			0,0,tag_attr_script,"�ű����Ա�ǩ"},

	/****client-desktop********/
	{"desktop",				"client",		"desktop",			0,0,tag_attr_desktop,"�����ǩ"},
	{"welcome",				"client",		"desktop",			0,0,tag_attr_welcome,"��ӭ��ǩ"},
	/****client-window********/
	{"window",				"client",		"window",			0,0,tag_attr_window,"���ڱ�ǩ"},
	{"dialog",				"client",		"window",			0,0,tag_attr_dialog,"�Ի����ǩ"},
	{"title",				"client",		"window",			0,0,tag_attr_title,"�����ǩ"},
	{"view",				"client",		"window",			0,0,tag_attr_view,"����������ʾ����ǩ"},
	{"status",				"client",		"window",			0,0,tag_attr_status,"״̬����ǩ"},

	/****sftp********/
	{"request",				"sftp",			"request",			0,0,tag_attr_request,"��server��������"},
	{"submit",				"sftp",			"submit",			0,0,tag_attr_submit,"��Ҫ��Server�ύ����ʱʹ��"},

	/****control-basic********/
	{"button",				"control",		"basic",			0,0,tag_attr_button,"��ť"},
	{"checkbox",			"control",		"basic",			0,0,tag_attr_checkbox,"��ѡ��"},
	{"groupbox",			"control",		"basic",			0,0,tag_attr_groupbox,"�ؼ���"},
	{"radiobutton",		"control",		"basic",			0,0,tag_attr_radiobutton,"��ѡ��"},
	{"label",				"control",		"basic",			0,0,tag_attr_label,"�ı���ǩ"},
	{"edit",				"control",		"basic",			0,0,tag_attr_edit,"�༭��"},
	{"textarea",			"control",		"basic",			0,0,tag_attr_textarea,"�ı�����"},
	{"multiedit",			"control",		"basic",			0,0,tag_attr_multiedit,"���б༭��"},
	{"image",				"control",		"basic",			0,0,tag_attr_image,"ͼ��"},
	/****control-combined********/
	{"listbox",				"control",		"combined",			0,0,tag_attr_listbox,"�б��"},
	{"combobox",			"control",		"combined",			0,0,tag_attr_combobox,"���Ͽ�"},
	{"option",				"control",		"combined",			0,0,tag_attr_option,"�б��/���Ͽ��ѡ��"},
	/****control-table********/
	{"list",				"control",		"table",			0,0,tag_attr_list,"�б�"},
	{"litem",				"control",		"table",			0,0,tag_attr_litem,"�б���"},
	{"table",				"control",		"table",			0,0,tag_attr_table,"���"},
	{"caption",				"control",		"table",			0,0,tag_attr_caption,"������"},
	{"th",					"control",		"table",			0,0,tag_attr_th,"��ͷ"},
	{"tr",					"control",		"table",			0,0,tag_attr_tr,"����һ��"},
	{"tf",					"control",		"table",			0,0,tag_attr_tf,"��β"},
	{"td",					"control",		"table",			0,0,tag_attr_td,"��Ԫ��"},
	{"tc",					"control",		"table",			0,0,tag_attr_tc,"��Ԫ����"},
	{"tnote",				"control",		"table",			0,0,tag_attr_tnote,"��ע��"},
	/****control-style********/
	{"font",				"control",		"style",			0,0,tag_attr_font,"�ı�������"},
	{"italic",				"control",		"style",			0,0,tag_attr_italic,"б��"},
	{"boldfaced",			"control",		"style",			0,0,tag_attr_boldfaced,"����"},
	{"crossed",				"control",		"style",			0,0,tag_attr_boldfaced,"�ı�����"},
	/****control-special********/
	{"progressbar",			"control",		"special",			0,0,tag_attr_progressbar,"������"},
	{"date",				"control",		"special",			0,0,tag_attr_date,"ʱ��/����"},
	{"ipaddress",			"control",		"special",			0,0,tag_attr_ipaddress,"IP��ַ"},
	
	{NULL,NULL,NULL}
};

#ifdef __cplusplus
}
#endif

