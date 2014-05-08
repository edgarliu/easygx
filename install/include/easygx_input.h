
#ifndef EASYGX_INPUT_H
#define EASYGX_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

//����ASCIIֵ
#define EASYGX_KEY_BACKSPACE         8         /* �˸��: BACKSPACE Crtl-H */
#define EASYGX_KEY_TAB               9         /* TAB��: TAB       Crtl-I */
#define EASYGX_KEY_LF                10        /* ���м�: LF line feed,new line Crtl- */
#define EASYGX_KEY_ENTER             13        /* �س���: ENTER     Crtl-M */
#define EASYGX_KEY_SHIFT             16   //Shift��
#define EASYGX_KEY_CONTROL           17   //Ctrl��
#define EASYGX_KEY_ALT               18   //Alt��
#define EASYGX_KEY_PAUSE             19   //Pause Break��
#define EASYGX_KEY_CAPITAL           20   //Caps Lock��
#define EASYGX_KEY_ESCAPE            27        /* ESC��: ESCAPE    0x1b   */
#define EASYGX_KEY_SPACE             32   //�ո��
#define EASYGX_KEY_PGUP              33   //Page Up
#define EASYGX_KEY_PGDOWN            34   //Page Down
#define EASYGX_KEY_END               35   //End��
#define EASYGX_KEY_HOME              36   //Home��
#define EASYGX_KEY_LEFT              37   //�����(��)
#define EASYGX_KEY_UP                38   //�����(��)
#define EASYGX_KEY_RIGHT             39   //�����(��)
#define EASYGX_KEY_DOWN              40   //�����(��)
#define EASYGX_KEY_PRTSC             44   //Print Screen��
#define EASYGX_KEY_INSERT            45   //Insert��
#define EASYGX_KEY_DELETE            46   //Delete��

#define EASYGX_KEY_LWIN              91   //��ձ��
#define EASYGX_KEY_RWIN              92   //�һձ��
#define EASYGX_KEY_APPS              93   //����Ҽ���ݼ�

#define EASYGX_KEY_F1                112  //F1��
#define EASYGX_KEY_F2                113  //F2��
#define EASYGX_KEY_F3				 114  //F3��
#define EASYGX_KEY_F4                115  //F4��
#define EASYGX_KEY_F5				 116  //F5��
#define EASYGX_KEY_F6				 117  //F6��
#define EASYGX_KEY_F7				 118  //F7��
#define EASYGX_KEY_F8				 119  //F8��
#define EASYGX_KEY_F9				 120  //F9��
#define EASYGX_KEY_F10				 121  //F10��
#define EASYGX_KEY_F11				 122  //F11��
#define EASYGX_KEY_F12				 123  //F12��
#define EASYGX_KEY_NUMLOCK           144  //Num Lock��
#define EASYGX_KEY_SCROLL            145  //Scroll Lock��

#define EASYGX_KEY_FN				 255  //Fn��

/*
С����0��VK_NUMPAD0 (96)
С����1��VK_NUMPAD1 (97)
С����2��VK_NUMPAD2 (98)
С����3��VK_NUMPAD3 (99)
С����4��VK_NUMPAD4 (100)
С����5��VK_NUMPAD5 (101)
С����6��VK_NUMPAD6 (102)
С����7��VK_NUMPAD7 (103)
С����8��VK_NUMPAD8 (104)
С����9��VK_NUMPAD9 (105)
С���̡���VK_DECIMAL (110)
С����*��VK_MULTIPLY (106)
С����+��VK_ADD (107)
С����-��VK_SUBTRACT (109)
С����/��VK_DIVIDE (111)
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
