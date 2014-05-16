#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sfel.h"
#include "sfpr_time.h"

#include "easygx.h"
#include "easygx_widget.h"
#include "easygx_frame.h"
#include "easygx_control.h"
#include "easygx_message.h"
#include "easygx_input.h"
#include "easygx_draw.h"
#include "easygx_log.h"

#ifdef WIN32
#include "windows.h"
#endif

int egx_pushbutton_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	char buffer[64];
	char *control_id = (char*)userdata;
	int ret = 0;
	
	memset(buffer,0,sizeof(buffer));
	
	ret = egx_button_checked(control_id);
	sprintf(buffer,"%d",ret);
	
	printf("egx_pushbutton_cb() id:%s  ret:%s\n",control_id,buffer);
	return 0;
}

int egx_checkbox_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("egx_checkbox_cb() id:%s\n",widget->id);
	return 0;
}

int egx_radiobutton_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("egx_radiobutton_cb() id:%s\n",widget->id);
	return 0;
}

int egx_window_timer1_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("egx_window_timer_cb %s\n",widget->id);	
}

int main(int argc, char* argv[])
{
	easygx_init_conf_t init_conf;
	char *frame_id = NULL;

	#ifdef WIN32
	LoadLibraryA("exchndl.dll");
	#endif
	
	easygx_log_init("stderr",SFPR_LOG_INFO);	
	easygx_init(&init_conf);

	egx_window_create("window_test1",egx_window_type_general,"test1111",0,0,0,640,480);

	egx_tree_create("tree_test11",0,"Tree Test",0,20,10,300,400,"window_test1");
	egx_tree_insert_item("tree_test11",0,"Tesss");
	egx_tree_insert_item("tree_test11",1,"Edgarliu");
	egx_tree_insert_item("tree_test11",2,"Iss");

// Table
/*
	egx_table_create("table_test11",0,"",0,20,10,300,400,"window_test1");
	egx_table_init_column("table_test11",0,60,"First");
	egx_table_init_column("table_test11",1,80,"Second");
	egx_table_init_column("table_test11",2,80,"Thirst");
	egx_table_init_item("table_test11",10);
	
	{
		char *text[4];
		text[0] = "defe";
		text[1] = "qwsa";
		text[2] = "xxxx";
		egx_table_set_item("tree_test11",3,text);
	}
	{
		char buffer[256];
		memset(buffer,0,sizeof(buffer));
		strcpy(buffer,"aaaa$bbbb$cccc");
		egx_table_set_line("tree_test11",4,buffer,"$");
	}
	egx_table_set_item_data("tree_test11",0,0,"00.00");
	egx_table_set_item_data("tree_test11",6,2,"00.11");
	egx_table_set_item_data("tree_test11",9,2,"11.00");
*/

//ListBox & ComboBox
/*
	egx_listbox_create("listbox_test11",0,"test listbox",0,50,80,100,60,"window_test1");
	egx_listbox_add_item("listbox_test11","aaaaaaa",NULL);
	egx_listbox_add_item("listbox_test11","bbbbbbb",NULL);
	egx_listbox_add_item("listbox_test11","xxxxxx",NULL);
	egx_listbox_add_item("listbox_test11","sssssss",NULL);

	egx_combobox_create("combobox_test11",0,"test combobox",0,200,80,100,90,"window_test1");
	egx_combobox_add_item("combobox_test11","aaaaaaa",NULL);
	egx_combobox_add_item("combobox_test11","bbbbbbb",NULL);
	egx_combobox_add_item("combobox_test11","xxxxxx",NULL);
	egx_combobox_add_item("combobox_test11","sssssss",NULL);
	egx_combobox_set_selected_item("combobox_test11",0);
*/

/*
	egx_message_reg_timer("window_test1",6,4000,egx_window_timer1_cb,NULL);	
	egx_pushbutton_create("pushbutton_test11",0,"test button",0,50,80,100,60,"window_test1");
	egx_message_pushbutton_jump("pushbutton_test11","dialog_test1",NULL);
	egx_pushbutton_create("pushbutton_test12",0,"test button",0,50,200,100,60,"window_test1");
	egx_message_pushbutton_jump("pushbutton_test12","window_test2",NULL);

	egx_dialog_create("dialog_test1",egx_dialog_type_general,"test2222",0,0,0,320,240);
	egx_pushbutton_create("pushbutton_test211",0,"View 2",0,50,20,100,30,"dialog_test1");

	egx_window_create("window_test2",egx_window_type_general,"test2222",0,0,0,640,480);
//	egx_window_set_stack("window_test2");

	egx_view_create("view_test21",egx_view_type_general,"aaaaaaaa","window_test2");
	egx_label_create("label_test21",0,"test label1",0,20,170,150,30,"view_test21");
	egx_pushbutton_create("pushbutton_test211",0,"View 2",0,50,20,100,30,"view_test21");
	egx_message_pushbutton_jump("pushbutton_test211","view_test22",NULL);
	egx_pushbutton_create("pushbutton_test212",0,"View 3",0,50,60,100,30,"view_test21");
	egx_message_pushbutton_jump("pushbutton_test212","view_test23",NULL);

	egx_view_create("view_test22",egx_view_type_general,"bbbbbbb","window_test2");
	egx_label_create("label_test22",0,"test label2",0,60,20,150,30,"view_test22");
	egx_pushbutton_create("pushbutton_test22",0,"Test Control",0,60,100,150,30,"view_test22");
	egx_message_reg_notification("pushbutton_test22",egx_notification_pushbutton_click,egx_pushbutton_cb,NULL,(char*)"radiobutton_test221");	

	egx_checkbox_create("checkbox_test22",0,"This Check Box",0,60,150,100,30,"view_test22");
	egx_message_reg_notification("checkbox_test22",egx_notification_checkbox_check,egx_checkbox_cb,NULL,(char*)"radiobutton_test221");	
	//egx_button_enable("checkbox_test22",0);
	egx_groupbox_create("groupbox_test221",0,"GroupBox",0,230,20,120,90,"view_test22");
	egx_radiobutton_create("radiobutton_test221",0,"Radio1",EGX_STYLE_GROUP,240,40,100,30,"view_test22");
	egx_radiobutton_create("radiobutton_test222",0,"Radio2",0,240,80,100,30,"view_test22");
	egx_message_reg_notification("radiobutton_test221",egx_notification_radiobutton_check,egx_radiobutton_cb,NULL,(char*)"radiobutton_test221");	
	egx_message_reg_notification("radiobutton_test222",egx_notification_radiobutton_check,egx_radiobutton_cb,NULL,(char*)"radiobutton_test221");	

	egx_groupbox_create("groupbox_test222",0,"GroupBox",0,230,120,120,90,"view_test22");
	egx_radiobutton_create("radiobutton_test223",0,"Radio3",EGX_STYLE_GROUP,240,140,100,30,"view_test22");
	egx_radiobutton_create("radiobutton_test224",0,"Radio4",0,240,180,100,30,"view_test22");
	egx_message_reg_notification("radiobutton_test223",egx_notification_radiobutton_check,egx_radiobutton_cb,NULL,(char*)"radiobutton_test221");	
	egx_message_reg_notification("radiobutton_test224",egx_notification_radiobutton_check,egx_radiobutton_cb,NULL,(char*)"radiobutton_test221");	


	egx_view_create("view_test23",egx_view_type_general,"ccccccccc","window_test2");
	egx_label_create("label_test23",0,"test label3",0,60,100,200,30,"view_test23");
	egx_edit_create("edit_test23",0,"View aaaaa",0,60,160,200,30,"view_test23");
	egx_pushbutton_create("pushbutton_test23",301,"Trans Data",0,60,200,100,30,"view_test23");
	egx_message_pushbutton_trans("pushbutton_test23","edit_test23","label_test23",NULL);
*/

	egx_window_show("window_test1");

	easygx_run();
	easygx_quit();
	return 0;
}
