
#ifndef EASYGX_INPUT_H
#define EASYGX_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

//ÐéÄâASCIIÖµ
#define EASYGX_KEY_BACKSPACE         8         /* ÍË¸ñ¼ü: BACKSPACE Crtl-H */
#define EASYGX_KEY_TAB               9         /* TAB¼ü: TAB       Crtl-I */
#define EASYGX_KEY_LF                10        /* »»ÐÐ¼ü: LF line feed,new line Crtl- */
#define EASYGX_KEY_ENTER             13        /* »Ø³µ¼ü: ENTER     Crtl-M */
#define EASYGX_KEY_SHIFT             16   //Shift¼ü
#define EASYGX_KEY_CONTROL           17   //Ctrl¼ü
#define EASYGX_KEY_ALT               18   //Alt¼ü
#define EASYGX_KEY_PAUSE             19   //Pause Break¼ü
#define EASYGX_KEY_CAPITAL           20   //Caps Lock¼ü
#define EASYGX_KEY_ESCAPE            27        /* ESC¼ü: ESCAPE    0x1b   */
#define EASYGX_KEY_SPACE             32   //¿Õ¸ñ¼ü
#define EASYGX_KEY_PGUP              33   //Page Up
#define EASYGX_KEY_PGDOWN            34   //Page Down
#define EASYGX_KEY_END               35   //End¼ü
#define EASYGX_KEY_HOME              36   //Home¼ü
#define EASYGX_KEY_LEFT              37   //·½Ïò¼ü(¡û)
#define EASYGX_KEY_UP                38   //·½Ïò¼ü(¡ü)
#define EASYGX_KEY_RIGHT             39   //·½Ïò¼ü(¡ú)
#define EASYGX_KEY_DOWN              40   //·½Ïò¼ü(¡ý)
#define EASYGX_KEY_PRTSC             44   //Print Screen¼ü
#define EASYGX_KEY_INSERT            45   //Insert¼ü
#define EASYGX_KEY_DELETE            46   //Delete¼ü

#define EASYGX_KEY_LWIN              91   //×ó»Õ±ê¼ü
#define EASYGX_KEY_RWIN              92   //ÓÒ»Õ±ê¼ü
#define EASYGX_KEY_APPS              93   //Êó±êÓÒ¼ü¿ì½Ý¼ü

#define EASYGX_KEY_F1                112  //F1¼ü
#define EASYGX_KEY_F2                113  //F2¼ü
#define EASYGX_KEY_F3				 114  //F3¼ü
#define EASYGX_KEY_F4                115  //F4¼ü
#define EASYGX_KEY_F5				 116  //F5¼ü
#define EASYGX_KEY_F6				 117  //F6¼ü
#define EASYGX_KEY_F7				 118  //F7¼ü
#define EASYGX_KEY_F8				 119  //F8¼ü
#define EASYGX_KEY_F9				 120  //F9¼ü
#define EASYGX_KEY_F10				 121  //F10¼ü
#define EASYGX_KEY_F11				 122  //F11¼ü
#define EASYGX_KEY_F12				 123  //F12¼ü
#define EASYGX_KEY_NUMLOCK           144  //Num Lock¼ü
#define EASYGX_KEY_SCROLL            145  //Scroll Lock¼ü

#define EASYGX_KEY_FN				 255  //Fn¼ü

/*
Ð¡¼üÅÌ0£ºVK_NUMPAD0 (96)
Ð¡¼üÅÌ1£ºVK_NUMPAD1 (97)
Ð¡¼üÅÌ2£ºVK_NUMPAD2 (98)
Ð¡¼üÅÌ3£ºVK_NUMPAD3 (99)
Ð¡¼üÅÌ4£ºVK_NUMPAD4 (100)
Ð¡¼üÅÌ5£ºVK_NUMPAD5 (101)
Ð¡¼üÅÌ6£ºVK_NUMPAD6 (102)
Ð¡¼üÅÌ7£ºVK_NUMPAD7 (103)
Ð¡¼üÅÌ8£ºVK_NUMPAD8 (104)
Ð¡¼üÅÌ9£ºVK_NUMPAD9 (105)
Ð¡¼üÅÌ¡££ºVK_DECIMAL (110)
Ð¡¼üÅÌ*£ºVK_MULTIPLY (106)
Ð¡¼üÅÌ+£ºVK_ADD (107)
Ð¡¼üÅÌ-£ºVK_SUBTRACT (109)
Ð¡¼üÅÌ/£ºVK_DIVIDE (111)
*/

typedef struct{
	int code;
	char *desc;
}egx_keycode_t;

const char *egx_keycode_desc(unsigned char keycode);

#ifdef __cplusplus
}
#endif

#endif
