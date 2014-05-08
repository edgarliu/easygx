
#include "sfel.h"
#include "sfel_ref.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 * SFEL语言类列表
***************************************************************************************************/

/**************************************************************************************************
 * SFEL语言标签及标签属性列表
***************************************************************************************************/

static sfel_tag_attr_t tag_attr_sfel[] = {
	TAG_ATTR_COMMON
	{"lang",0,0,"当前语言"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_script[] = {
	TAG_ATTR_COMMON
	{"lang",0,0,"脚本类型"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_desktop[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"mainwnd",0,0,"主窗口"},
	{"bgcolor",0,0,"背景色"},
	{"bgimage",0,0,"背景图片"},
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
	{"bgcolor",0,0,"背景色"},
	{"bgimage",0,0,"背景图片"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_dialog[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"bgcolor",0,0,"背景色"},
	{"bgimage",0,0,"背景图片"},
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
	{"desc",0,0,"显示区描述"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_status[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_request[] = {
	TAG_ATTR_COMMON
	{"request",0,0,"客户端向服务器发送请求时的 abs_path"},
	{"path_type",0,0,"URL中路径(abs_path)的类型"},
	{"req_args",0,0,"请求参数"},
	{"Receiver",0,0,"接收者的ID，window或者dialog"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_submit[] = {
	TAG_ATTR_COMMON
	{"request",0,0,"客户端向服务器发送请求时的 abs_path"},
	{"path_type",0,0,"URL中路径(abs_path)的类型"},
	{"trans_type",0,0,"Server端和Client端数据交互方式"},
	{"req_args",0,0,"请求参数"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_button[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"type",0,0,"有三种类型：push、check、radio"},
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
	{"value",0,0,"输入控件的值"},
	{"maxlength",0,0,"为文本输入框或密码框指定最大允许字符长度"},
	{"readonly",0,0,"设置输入控件为只读"},
	{"min",0,0,"最小值"},
	{"max",0,0,"最大值"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_textarea[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"value",0,0,"输入控件的值"},
	{"maxlength",0,0,"为文本区输入框指定最大允许字符长度"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_multiedit[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"value",0,0,"输入控件的值"},
	{"maxlength",0,0,"为文本输入框或密码框指定最大允许字符长度"},
	{"readonly",0,0,"设置输入控件为只读"},
	{"min",0,0,"最小值"},
	{"max",0,0,"最大值"},
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
	{"min",0,0,"最小值"},
	{"max",0,0,"最大值"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_combobox[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"min",0,0,"最小值"},
	{"max",0,0,"最大值"},
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
	{"image",0,0,"列表项对应的图像"},
	{"mode",0,0,"显示文本还是图像"},
	{NULL,0,0,NULL}
};

static sfel_tag_attr_t tag_attr_table[] = {
	TAG_ATTR_COMMON
	TAG_ATTR_COMMON_WIDGET
	{"ab",0,0,"缩写"},
	{"mode",0,0,"表格模式，分两种方式：Common：普通模式	Smot：简单表格模式"},
	{"row",0,0,"表格列数"},
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
	{"isigame",0,0,"是否用图片显示"},
	{"rowspan",0,0,"合并同一列单元格"},
	{"colspan",0,0,"合并同一行单元格"},
	{"image",0,0,"用图像显示的图片文件名"},
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
	{"face",0,0,"字体名称"},
	{"size",0,0,"字体大小"},
	{"color",0,0,"字体颜色"},
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
	{"sfel",				"basic",		"basic",			0,0,tag_attr_sfel,"sfel语言根标签"},
	{"include",				"basic",		"basic",			0,0,NULL,"引用其他sfel文件"},
	{"script",				"basic",		"basic",			0,0,tag_attr_script,"脚本语言标签"},

	/****client-desktop********/
	{"desktop",				"client",		"desktop",			0,0,tag_attr_desktop,"桌面标签"},
	{"welcome",				"client",		"desktop",			0,0,tag_attr_welcome,"欢迎标签"},
	/****client-window********/
	{"window",				"client",		"window",			0,0,tag_attr_window,"窗口标签"},
	{"dialog",				"client",		"window",			0,0,tag_attr_dialog,"对话框标签"},
	{"title",				"client",		"window",			0,0,tag_attr_title,"标题标签"},
	{"view",				"client",		"window",			0,0,tag_attr_view,"数据内容显示区标签"},
	{"status",				"client",		"window",			0,0,tag_attr_status,"状态栏标签"},

	/****sftp********/
	{"request",				"sftp",			"request",			0,0,tag_attr_request,"向server发送请求"},
	{"submit",				"sftp",			"submit",			0,0,tag_attr_submit,"需要向Server提交数据时使用"},

	/****control-basic********/
	{"button",				"control",		"basic",			0,0,tag_attr_button,"按钮"},
	{"checkbox",			"control",		"basic",			0,0,tag_attr_checkbox,"复选框"},
	{"groupbox",			"control",		"basic",			0,0,tag_attr_groupbox,"控件组"},
	{"radiobutton",		"control",		"basic",			0,0,tag_attr_radiobutton,"单选框"},
	{"label",				"control",		"basic",			0,0,tag_attr_label,"文本标签"},
	{"edit",				"control",		"basic",			0,0,tag_attr_edit,"编辑框"},
	{"textarea",			"control",		"basic",			0,0,tag_attr_textarea,"文本区框"},
	{"multiedit",			"control",		"basic",			0,0,tag_attr_multiedit,"多行编辑框"},
	{"image",				"control",		"basic",			0,0,tag_attr_image,"图像"},
	/****control-combined********/
	{"listbox",				"control",		"combined",			0,0,tag_attr_listbox,"列表框"},
	{"combobox",			"control",		"combined",			0,0,tag_attr_combobox,"复合框"},
	{"option",				"control",		"combined",			0,0,tag_attr_option,"列表框/复合框的选项"},
	/****control-table********/
	{"list",				"control",		"table",			0,0,tag_attr_list,"列表"},
	{"litem",				"control",		"table",			0,0,tag_attr_litem,"列表项"},
	{"table",				"control",		"table",			0,0,tag_attr_table,"表格"},
	{"caption",				"control",		"table",			0,0,tag_attr_caption,"表格标题"},
	{"th",					"control",		"table",			0,0,tag_attr_th,"表头"},
	{"tr",					"control",		"table",			0,0,tag_attr_tr,"表格的一行"},
	{"tf",					"control",		"table",			0,0,tag_attr_tf,"表尾"},
	{"td",					"control",		"table",			0,0,tag_attr_td,"单元格"},
	{"tc",					"control",		"table",			0,0,tag_attr_tc,"单元格列"},
	{"tnote",				"control",		"table",			0,0,tag_attr_tnote,"表单注释"},
	/****control-style********/
	{"font",				"control",		"style",			0,0,tag_attr_font,"文本的字体"},
	{"italic",				"control",		"style",			0,0,tag_attr_italic,"斜体"},
	{"boldfaced",			"control",		"style",			0,0,tag_attr_boldfaced,"粗体"},
	{"crossed",				"control",		"style",			0,0,tag_attr_boldfaced,"文本划线"},
	/****control-special********/
	{"progressbar",			"control",		"special",			0,0,tag_attr_progressbar,"进度条"},
	{"date",				"control",		"special",			0,0,tag_attr_date,"时间/日期"},
	{"ipaddress",			"control",		"special",			0,0,tag_attr_ipaddress,"IP地址"},
	
	{NULL,NULL,NULL}
};

#ifdef __cplusplus
}
#endif

