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

#define PAI 3.1415
int draw_time(egx_widget_t *widget)
{
	int x0 = 160,y0 = 120;
	int x_sec = 0,y_sec = 0;
	int radius_sec = 60;
	double angle;
	sfpr_time_t time;
	sfpr_tm_t tm;

	sfpr_time_now(&time);
	sfpr_time_localtime(&time,&tm);
	
	angle = ((tm.sec * 6)*PAI)/180;
	x_sec = x0 + (int)((double)radius_sec*sin(angle));
	y_sec = y0 - (int)((double)radius_sec*cos(angle));
	
	egx_paint_begin((egx_frame_t*)widget);

	egx_draw_ellipse((egx_frame_t*)widget,x0-70,y0-70,140,140);
	egx_draw_line((egx_frame_t*)widget,x0,y0,x_sec,y_sec);
	
	egx_paint_end((egx_frame_t*)widget);
	return 0;
}

int egx_window_timer1_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("egx_window_timer_cb %s\n",widget->id);	
}

int egx_window_timer2_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
//	printf("egx_window1_timer1_cb %s\n",widget->id);	
	egx_widget_update(widget,1);
}

int egx_window_paint_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
//	printf("egx_window1_paint_cb %s\n",widget->id);	
	draw_time(widget);
	return 0;
}

int egx_window_keydown_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	egx_uint32_t key = param1;
	printf("key down 0x%02x <%s>\n",key,egx_keycode_desc((unsigned char)key));	
	return 0;
}

int egx_window_keyup_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	egx_uint32_t key = param1;
	printf("key up 0x%02x <%s>\n",key,egx_keycode_desc((unsigned char)key));	
	return 0;
}

int egx_window_lbuttondown_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	egx_uint32_t x = (param1 & 0xffff); 
	egx_uint32_t y = (param1 & 0xffff0000) >> 16; 
	printf("left button down x:%d y:%d\n",x,y);	
	return 0;
}

int egx_window_lbuttonup_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("left button up\n");	
	return 0;
}

int egx_window_lbuttondbclk_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("left button db clk\n");	
	return 0;
}

int egx_window_mbuttondown_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("midle button down\n");	
	return 0;
}

int egx_window_mbuttonup_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	printf("midle button up\n");	
	return 0;
}

int egx_window_mousewheel_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	egx_uint32_t x = (param1 & 0xffff); 
	egx_uint32_t y = (param1 & 0xffff0000) >> 16; 
	printf("left button down x:%d y:%d delta:%d\n",x,y,param2);
	printf("mouse wheel\n");	
	return 0;
}


int egx_window_edit_cb(egx_widget_t *widget,egx_uint32_t msg_id,egx_uint32_t param1,egx_uint32_t param2,void *userdata)
{
	char buffer[64];
	char *button_id = (char*)userdata;
	memset(buffer,0,sizeof(buffer));
	egx_edit_get_text(button_id,buffer,sizeof(buffer));
	printf("edit: %s  text:%s\n",button_id,buffer);	
	return 0;
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
	egx_message_reg_timer("window_test1",6,4000,egx_window_timer1_cb,NULL);
	egx_pushbutton_create("pushbutton_test11",0,"window2",0,50,80,100,60,"window_test1");
	egx_message_pushbutton_jump("pushbutton_test11","window_test2",NULL);

	egx_window_create("window_test2",egx_window_type_general,"test2222",0,0,0,640,480);

	egx_view_create("view_test21",egx_view_type_general,"aaaaaaaa","window_test2");
	egx_label_create("label_test21",0,"test label1",0,20,170,150,30,"view_test21");

	egx_view_create("view_test23",egx_view_type_general,"ccccccccc","window_test2");
	egx_label_create("label_test23",0,"test label3",0,60,100,200,30,"view_test23");
	egx_edit_create("edit_test23",0,"View aaaaa",0,60,160,200,30,"view_test23");
	egx_pushbutton_create("pushbutton_test23",301,"Trans Data",0,60,200,100,30,"view_test23");
	egx_message_reg_notification("pushbutton_test23",egx_notification_pushbutton_click,egx_window_edit_cb,NULL,(char*)"edit_test23");


	frame_id = "view_test21";
	egx_message_reg_timer(frame_id,5,1000,egx_window_timer2_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_paint,egx_window_paint_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_keydown,egx_window_keydown_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_keyup,egx_window_keyup_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_lbuttondown,egx_window_lbuttondown_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_lbuttonup,egx_window_lbuttonup_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_lbuttondbclk,egx_window_lbuttondbclk_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_mbuttondown,egx_window_mbuttondown_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_mbuttonup,egx_window_mbuttonup_cb,NULL);
	egx_message_reg_public_msg(frame_id,egx_message_mousewheel,egx_window_mousewheel_cb,NULL);

	egx_window_show("window_test1");

	easygx_run();
	easygx_quit();
	return 0;
}
