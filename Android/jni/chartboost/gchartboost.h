#ifndef GCHARTBOOST_H
#define GCHARTBOOST_H

#include <gglobal.h>
#include <gevent.h>

enum
{
	GCHARTBOOST_CACHE_INTERSTITIAL_EVENT,
	GCHARTBOOST_CACHE_MORE_APPS_EVENT,
	GCHARTBOOST_CLICK_INTERSTITIAL_EVENT,
	GCHARTBOOST_CLICK_MORE_APPS_EVENT,
	GCHARTBOOST_CLOSE_INTERSTITIAL_EVENT,
	GCHARTBOOST_CLOSE_MORE_APPS_EVENT,
	GCHARTBOOST_DISMISS_INTERSTITIAL_EVENT,
	GCHARTBOOST_DISMISS_MORE_APPS_EVENT,
	GCHARTBOOST_FAIL_TO_LOAD_INTERSTITIAL_EVENT,
	GCHARTBOOST_FAIL_TO_LOAD_MORE_APPS_EVENT,
	GCHARTBOOST_SHOW_INTERSTITIAL_EVENT,
	GCHARTBOOST_SHOW_MORE_APPS_EVENT
};

typedef struct gchartboost_InterstitialEvent
{
	const char *location;
} gchartboost_InterstitialEvent;

#ifdef __cplusplus
extern "C" {
#endif

G_API int gchartboost_isAvailable();

G_API void gchartboost_init();
G_API void gchartboost_cleanup();

G_API void gchartboost_startSession(const char* appId, const char* appSignature);
G_API void gchartboost_showInterstitial();
G_API void gchartboost_showMoreApps();
G_API void gchartboost_cacheInterstitial();
G_API void gchartboost_cacheMoreApps();
G_API void gchartboost_clearCache();

G_API g_id gchartboost_addCallback(gevent_Callback callback, void *udata);
G_API void gchartboost_removeCallback(gevent_Callback callback, void *udata);
G_API void gchartboost_removeCallbackWithGid(g_id gid);

#ifdef __cplusplus
}
#endif

#endif
