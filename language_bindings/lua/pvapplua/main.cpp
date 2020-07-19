//***************************************************************************
//                          main.cpp  -  description
//                             -------------------
//  begin            : So. Jan 23 10:25:55 2011
//  generated by     : pvdevelop (C) Lehrig Software Engineering
//  email            : lehrig@t-online.de
//***************************************************************************
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "processviewserver.h"

int trace = 0;

// Include the Lua API header files
#ifdef __cplusplus
extern "C" {
#endif
//#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
extern int luaopen_pv(lua_State* L);    // declare the wrapped module
extern int luaopen_rllib(lua_State* L); // declare the wrapped module
#ifdef __cplusplus
}
#endif

char pvarg0[1024];

int pvMain(PARAM *dummy)
{
  if(dummy == NULL) return -1;
  return 0;
}

int luaMain()
{
  int ret, status;
  lua_State* L;

  // initialize Lua
  if(trace) printf("lua_open\n");
  // L = lua_open();
  L = luaL_newstate();
  luaL_openlibs(L);

  // load our custom libs
  ret = luaopen_pv(L);
  if(trace) printf("luaopen_pv ret=%d\n", ret);
  ret = luaopen_rllib(L);
  if(trace) printf("luaopen_rllib ret=%d\n", ret);

  // load the script
  if(trace) printf("Loading '%s'\n", "main.lua");
  status = luaL_dofile(L, "main.lua");
  if(status)
  {
    printf("Couldn't load file: %s\n", lua_tostring(L, -1));
    lua_close(L);
    return -1;
  }

  // call luaMain
  lua_getglobal(L, "luaMain");
  lua_pushnumber(L, trace);
  status = lua_pcall(L, 1, 1, 0); 
  if(status)
  {
    printf("Exception in running lua: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  
  lua_close(L);
  return 0;
}

int main(int ac, char **av)
{
  if(ac != 1)
  {
    if(strcmp(av[1],"-trace") == 0)
    {
      trace = 1;
    }
    else
    {
      printf("usage: %s <-trace>\n", av[0]);
      printf("%s will run Lua script main.lua\n", av[0]);
      printf("The main function is luaMain(trace)\n");
      printf("You may use functions from rllib there\n");
    }  
  }
  if(getcwd(pvarg0, sizeof(pvarg0)-20) == NULL) return -1;
  strcat(pvarg0,"\\main.lua");

  luaMain();
  return 0;
}
