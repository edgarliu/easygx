
#include <stdarg.h>
#include <stdio.h>

#include "sfpr_string.h"
#include "sfpr_global.h"

#include "sfel.h"

#include "config.h"
#ifdef HAVE_LUA
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int sfel_lua_call_func(void *lstate,sfel_node_t *node,char *errcode,char *fmt,...)
{
	#ifdef HAVE_LUA
	char script[1024],*func = NULL;
	va_list vl;
	int narg = 0,nres = 0;
	int number = 0,count = 0;
	lua_State *ls = NULL;

	char *lang = sfel_xml_attr_get_value(node,"lang");
	if(NULL == lang){
		sprintf(errcode,"cannot find lang attribute");
		return -1;
	}
	if(strcmp(lang,"text/lua")){
		sprintf(errcode,"invalid lang:%s",lang);
		return -1;
	}

	memset(script,0,sizeof(script));
	strncpy(script,sfel_xml_node_get_value(node),sizeof(script)-1);
	
	if(NULL == lstate){
		ls = luaL_newstate();
		luaL_openlibs(ls);
	}else{
		ls = (lua_State*)lstate;
	}

	if(luaL_loadbuffer(ls,script,strlen(script),"line")){
		sprintf(errcode,"cannot load script:%s",lua_tostring(ls,-1));
		return -1;
	}
	if(lua_pcall(ls,0,LUA_MULTRET,0)){
		sprintf(errcode,"cannot run script:%s",lua_tostring(ls,-1));
		return -2;
	}

	func = sfel_xml_attr_get_value(node,"entry");
	if(NULL == func){
		lua_getglobal(ls,"lua_main");		
	}else{
		lua_getglobal(ls,func);
	}
	
	va_start(vl,fmt);
	while(*fmt){
		switch(*fmt++){
		case 'd':
//			lua_pushnumber(ls,va_arg(vl,double));
			lua_pushnumber(ls,va_arg(vl,int));
			break;
		case 'i':
			lua_pushnumber(ls,va_arg(vl,int));
			break;
		case 's':
			lua_pushstring(ls,va_arg(vl,char*));
			break;
		case '>':
			goto endwhile;
		}
		narg++;
	}
	endwhile:

	nres = strlen(fmt);
	if(lua_pcall(ls,narg,nres,0)){
		sprintf(errcode,"cannot call %s:%s",func,lua_tostring(ls,-1));
		return -3;
	}
	#ifdef LOG_NOTICE
	printf("statck number:%d\n",lua_gettop(ls));
	#endif
	nres = -nres;
	number = lua_gettop(ls);
	while(*fmt && count++ < number){
		switch(*fmt++){
		case 'd':
			if(lua_isnumber(ls,nres)){
//				double res = lua_tonumber(ls,nres);
//				memcpy(va_arg(vl,double*),&res,sizeof(double));
				*va_arg(vl,int*) = (int)lua_tonumber(ls,nres);
			}
			break;
		case 'i':
			if(lua_isnumber(ls,nres)){
				*va_arg(vl,int*) = (int)lua_tonumber(ls,nres);
			}
			break;
		case 's':
			if(lua_isstring(ls,nres)){
				strcpy(va_arg(vl,char*),lua_tostring(ls,nres));
			}
			break;
		}
		nres++;
	}
	va_end(vl);
	if(NULL == lstate){
		lua_close(ls);
	}else{
		lua_settop(ls,0);
	}
	strcpy(errcode,"success");
	return 0;
	#else
	strcpy(errcode,"cannot support lua");	
	return -1;
	#endif
}

#ifdef __cplusplus
}
#endif
