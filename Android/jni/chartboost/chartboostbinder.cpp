#include "gchartboost.h"
#include "gideros.h"
#include <glog.h>

#include <android/log.h>

// some Lua helper functions
#ifndef abs_index
#define abs_index(L, i) ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : lua_gettop(L) + (i) + 1)
#endif

static void luaL_newweaktable(lua_State *L, const char *mode)
{
	lua_newtable(L);			// create table for instance list
	lua_pushstring(L, mode);
	lua_setfield(L, -2, "__mode");	  // set as weak-value table
	lua_pushvalue(L, -1);             // duplicate table
	lua_setmetatable(L, -2);          // set itself as metatable
}

static void luaL_rawgetptr(lua_State *L, int idx, void *ptr)
{
	idx = abs_index(L, idx);
	lua_pushlightuserdata(L, ptr);
	lua_rawget(L, idx);
}

static const char *CACHE_INTERSTITIAL = "cacheInterstitial";
static const char *CACHE_MORE_APPS = "cacheMoreApps";
static const char *CLICK_INTERSTITIAL = "clickInterstitial";
static const char *CLICK_MORE_APPS = "clickMoreApps";
static const char *CLOSE_INTERSTITIAL = "closeInterstitial";
static const char *CLOSE_MORE_APPS = "closeMoreApps";
static const char *DISMISS_INTERSTITIAL = "dismissInterstitial";
static const char *DISMISS_MORE_APPS = "dismissMoreApps";
static const char *FAIL_TO_LOAD_INTERSTITIAL = "failToLoadInterstitial";
static const char *FAIL_TO_LOAD_MORE_APPS = "failToLoadMoreApps";
static const char *SHOW_INTERSTITIAL = "showInterstitial";
static const char *SHOW_MORE_APPS = "showMoreApps";

static void luaL_rawsetptr(lua_State *L, int idx, void *ptr)
{
	idx = abs_index(L, idx);
	lua_pushlightuserdata(L, ptr);
	lua_insert(L, -2);
	lua_rawset(L, idx);
}

static char keyWeak = ' ';

class ChartBoost : public GEventDispatcherProxy
{
public:
	ChartBoost(lua_State *L) : L(L)
	{
		gchartboost_init();
		gchartboost_addCallback(callback_s, this);
	}

	~ChartBoost()
	{
		gchartboost_removeCallback(callback_s, this);
		gchartboost_cleanup();
	}

	void startSession(const char* appId, const char* appSignature)
	{
		gchartboost_startSession(appId, appSignature);
	}

	void showInterstitial()
	{
		gchartboost_showInterstitial();
	}

private:
	static void callback_s(int type, void *event, void *udata)
	{
		static_cast<ChartBoost*>(udata)->callback(type, event);
	}

	void callback(int type, void *event)
	{
		dispatchEvent(type, event);
	}

	void dispatchEvent(int type, void *event)
	{
		luaL_rawgetptr(L, LUA_REGISTRYINDEX, &keyWeak);
		luaL_rawgetptr(L, -1, this);

		if (lua_isnil(L, -1))
		{
			lua_pop(L, 2);
			return;
		}

		lua_getfield(L, -1, "dispatchEvent");

		lua_pushvalue(L, -2);

		lua_getglobal(L, "Event");
		lua_getfield(L, -1, "new");
		lua_remove(L, -2);

		switch (type)
		{
		case GCHARTBOOST_CACHE_INTERSTITIAL_EVENT:
			lua_pushstring(L, CACHE_INTERSTITIAL);
		break;
		case GCHARTBOOST_CACHE_MORE_APPS_EVENT:
			lua_pushstring(L, CACHE_MORE_APPS);
		break;
		case GCHARTBOOST_CLICK_INTERSTITIAL_EVENT:
			lua_pushstring(L, CLICK_INTERSTITIAL);
		break;
		case GCHARTBOOST_CLICK_MORE_APPS_EVENT:
			lua_pushstring(L, CLICK_MORE_APPS);
		break;
		case GCHARTBOOST_CLOSE_INTERSTITIAL_EVENT:
			lua_pushstring(L, CLOSE_INTERSTITIAL);
		break;
		case GCHARTBOOST_CLOSE_MORE_APPS_EVENT:
			lua_pushstring(L, CLOSE_MORE_APPS);
		break;
		case GCHARTBOOST_DISMISS_INTERSTITIAL_EVENT:
			lua_pushstring(L, DISMISS_INTERSTITIAL);
		break;
		case GCHARTBOOST_DISMISS_MORE_APPS_EVENT:
			lua_pushstring(L, DISMISS_MORE_APPS);
		break;
		case GCHARTBOOST_FAIL_TO_LOAD_INTERSTITIAL_EVENT:
			lua_pushstring(L, FAIL_TO_LOAD_INTERSTITIAL);
		break;
		case GCHARTBOOST_FAIL_TO_LOAD_MORE_APPS_EVENT:
			lua_pushstring(L, FAIL_TO_LOAD_MORE_APPS);
		break;
		case GCHARTBOOST_SHOW_INTERSTITIAL_EVENT:
			lua_pushstring(L, SHOW_INTERSTITIAL);
		break;
		case GCHARTBOOST_SHOW_MORE_APPS_EVENT:
			lua_pushstring(L, SHOW_MORE_APPS);
		break;
		}

		lua_call(L, 1, 1);

		if (
			type == GCHARTBOOST_CACHE_INTERSTITIAL_EVENT ||
			type == GCHARTBOOST_CLICK_INTERSTITIAL_EVENT ||
			type == GCHARTBOOST_CLOSE_INTERSTITIAL_EVENT ||
			type == GCHARTBOOST_DISMISS_INTERSTITIAL_EVENT ||
			type == GCHARTBOOST_FAIL_TO_LOAD_INTERSTITIAL_EVENT ||
			type == GCHARTBOOST_SHOW_INTERSTITIAL_EVENT
		)
		{
			gchartboost_InterstitialEvent *event2 = (gchartboost_InterstitialEvent*) event;

			lua_pushstring(L, event2->location);
			lua_setfield(L, -2, "location");
		}

		lua_call(L, 2, 0);

		lua_pop(L, 2);
	}

private:
	lua_State *L;
	bool initialized_;
};

static int destruct(lua_State* L)
{
	void *ptr =*(void**)lua_touserdata(L, 1);
	GReferenced* object = static_cast<GReferenced*>(ptr);
	ChartBoost *chartboost = static_cast<ChartBoost*>(object->proxy());

	chartboost->unref();

	return 0;
}

static ChartBoost *getInstance(lua_State *L, int index)
{
	GReferenced *object = static_cast<GReferenced*>(g_getInstance(L, "ChartBoost", index));
	ChartBoost *chartboost = static_cast<ChartBoost*>(object->proxy());

	return chartboost;
}
static int startSession(lua_State *L)
{
	ChartBoost *chartboost = getInstance(L, 1);

	const char *appId = lua_tostring(L, 2);
	const char *appSignature = lua_tostring(L, 3);

	chartboost->startSession(appId, appSignature);

	return 0;
}

static int showInterstitial(lua_State *L)
{
	ChartBoost *chartboost = getInstance(L, 1);

	chartboost->showInterstitial();

	return 0;
}

static int loader(lua_State *L)
{
	const luaL_Reg functionList[] = {
		{"startSession", startSession},
		{"showInterstitial", showInterstitial},
		{NULL, NULL}
	};

	g_createClass(L, "ChartBoost", "EventDispatcher", NULL, destruct, functionList);

	// create a weak table in LUA_REGISTRYINDEX that can be accessed with the address of keyWeak
	luaL_newweaktable(L, "v");
	luaL_rawsetptr(L, LUA_REGISTRYINDEX, &keyWeak);

	lua_getglobal(L, "Event");
	lua_pushstring(L, CACHE_INTERSTITIAL);
	lua_setfield(L, -2, "CACHE_INTERSTITIAL");
	lua_pushstring(L, CACHE_MORE_APPS);
	lua_setfield(L, -2, "CACHE_MORE_APPS");
	lua_pushstring(L, CLICK_INTERSTITIAL);
	lua_setfield(L, -2, "CLICK_INTERSTITIAL");
	lua_pushstring(L, CLICK_MORE_APPS);
	lua_setfield(L, -2, "CLICK_MORE_APPS");
	lua_pushstring(L, CLOSE_INTERSTITIAL);
	lua_setfield(L, -2, "CLOSE_INTERSTITIAL");
	lua_pushstring(L, CLOSE_MORE_APPS);
	lua_setfield(L, -2, "CLOSE_MORE_APPS");
	lua_pushstring(L, DISMISS_INTERSTITIAL);
	lua_setfield(L, -2, "DISMISS_INTERSTITIAL");
	lua_pushstring(L, DISMISS_MORE_APPS);
	lua_setfield(L, -2, "DISMISS_MORE_APPS");
	lua_pushstring(L, FAIL_TO_LOAD_INTERSTITIAL);
	lua_setfield(L, -2, "FAIL_TO_LOAD_INTERSTITIAL");
	lua_pushstring(L, FAIL_TO_LOAD_MORE_APPS);
	lua_setfield(L, -2, "FAIL_TO_LOAD_MORE_APPS");
	lua_pushstring(L, SHOW_INTERSTITIAL);
	lua_setfield(L, -2, "SHOW_INTERSTITIAL");
	lua_pushstring(L, SHOW_MORE_APPS);
	lua_setfield(L, -2, "SHOW_MORE_APPS");
	lua_pop(L, 1);

	ChartBoost *chartboost = new ChartBoost(L);
	g_pushInstance(L, "ChartBoost", chartboost->object());

	luaL_rawgetptr(L, LUA_REGISTRYINDEX, &keyWeak);
	lua_pushvalue(L, -2);
	luaL_rawsetptr(L, -2, chartboost);
	lua_pop(L, 1);

	lua_pushvalue(L, -1);
	lua_setglobal(L, "chartboost");

    return 1;
}

static void g_initializePlugin(lua_State *L)
{
    lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");

	lua_pushcfunction(L, loader);
	lua_setfield(L, -2, "chartboost");

	lua_pop(L, 2);
}

static void g_deinitializePlugin(lua_State *L)
{

}

REGISTER_PLUGIN("ChartBoost", "2012.12")
