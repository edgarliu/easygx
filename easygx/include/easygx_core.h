
#ifndef EASYGX_CORE_H
#define EASYGX_CORE_H

#include "sfpr_stack.h"
#include "sfpr_hash.h"

#include "sfel.h"

#include "easygx.h"
#include "easygx_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

struct easygx_s
{
	char name[32];
	int quit_flag;
	void *application;
	sfel_instance_t *sfel_inst;
	int width,height;
	char *firstwnd;
	sfpr_hash_t *widget_hash;
	sfpr_stack_t *showed_frame;
};

#ifdef __cplusplus
}
#endif

#endif
