#include "gchartboost.h"
#include <jni.h>
#include <stdlib.h>
#include <glog.h>

#include <android/log.h>

extern "C" {
JavaVM *g_getJavaVM();
JNIEnv *g_getJNIEnv();
}

class GChartBoost
{
public:
	GChartBoost()
	{
		gid_ = g_nextgid();

		JNIEnv *env = g_getJNIEnv();

		jclass localClass = env->FindClass("com/nightspade/gideros/android/plugins/chartboost/GChartBoost");
		cls_ = (jclass)env->NewGlobalRef(localClass);
		env->DeleteLocalRef(localClass);

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "init", "(J)V"), (jlong)this);
	}

	~GChartBoost()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "cleanup", "()V"));

		env->DeleteGlobalRef(cls_);

		gevent_removeEventsWithGid(gid_);
	}

	void onCacheInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CACHE_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onCacheMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CACHE_MORE_APPS_EVENT, NULL, 1, this);
	}

	void onClickInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CLICK_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onClickMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CACHE_MORE_APPS_EVENT, NULL, 1, this);
	}

	void onCloseInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CLOSE_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onCloseMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_CLOSE_MORE_APPS_EVENT, NULL, 1, this);
	}

	void onDismissInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_DISMISS_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onDismissMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_DISMISS_MORE_APPS_EVENT, NULL, 1, this);
	}

	void onFailToLoadInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_FAIL_TO_LOAD_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onFailToLoadMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_FAIL_TO_LOAD_MORE_APPS_EVENT, NULL, 1, this);
	}

	void onShowInterstitial(jstring jlocation)
	{
		JNIEnv *env = g_getJNIEnv();

		const char *location = env->GetStringUTFChars(jlocation, NULL);

		gchartboost_InterstitialEvent *event = (gchartboost_InterstitialEvent*)gevent_createEventStruct1(
			sizeof(gchartboost_InterstitialEvent),
			offsetof(gchartboost_InterstitialEvent, location), location
		);

		env->ReleaseStringUTFChars(jlocation, location);

		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_SHOW_INTERSTITIAL_EVENT, event, 1, this);
	}

	void onShowMoreApps()
	{
		gevent_enqueueEvent(gid_, callback_s, GCHARTBOOST_SHOW_MORE_APPS_EVENT, NULL, 1, this);
	}

	void startSession(const char *appId, const char *appSignature)
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "startSession", "(Ljava/lang/String;Ljava/lang/String;)V"), env->NewStringUTF(appId), env->NewStringUTF(appSignature));
	}

	void showInterstitial()
	{
		JNIEnv *env = g_getJNIEnv();

		env->CallStaticVoidMethod(cls_, env->GetStaticMethodID(cls_, "showInterstitial", "()V"));
	}

	g_id addCallback(gevent_Callback callback, void *udata)
	{
		return callbackList_.addCallback(callback, udata);
	}

	void removeCallback(gevent_Callback callback, void *udata)
	{
		callbackList_.removeCallback(callback, udata);
	}

	void removeCallbackWithGid(g_id gid)
	{
		callbackList_.removeCallbackWithGid(gid);
	}

private:
	static void callback_s(int type, void *event, void *udata)
	{
		((GChartBoost*)udata)->callback(type, event);
	}

	void callback(int type, void *event)
	{
		callbackList_.dispatchEvent(type, event);
	}

private:
	gevent_CallbackList callbackList_;

private:
	jclass cls_;
	g_id gid_;
};

extern "C" {
	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onCacheInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onCacheInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onCacheMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onCacheMoreApps();
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onClickInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onClickInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onClickMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onClickMoreApps();
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onCloseInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onCloseInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onCloseMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onCloseMoreApps();
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onDismissInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onDismissInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onDismissMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onDismissMoreApps();
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onFailToLoadInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onFailToLoadInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onFailToLoadMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onFailToLoadMoreApps();
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onShowInterstitial(JNIEnv *env, jclass clz, jstring location, jlong data)
	{
		((GChartBoost*)data)->onShowInterstitial(location);
	}

	void Java_com_nightspade_gideros_android_plugins_chartboost_GChartBoost_onShowMoreApps(JNIEnv *env, jclass clz, jlong data)
	{
		((GChartBoost*)data)->onShowMoreApps();
	}
}

static GChartBoost *s_chartboost = NULL;

extern "C" {

int gchartboost_isAvailable()
{
	return 1;
}

void gchartboost_init()
{
	s_chartboost = new GChartBoost();
}

void gchartboost_cleanup()
{
	delete s_chartboost;
	s_chartboost = NULL;
}

void gchartboost_startSession(const char *appId, const char *appSignature)
{
	s_chartboost->startSession(appId, appSignature);
}

void gchartboost_showInterstitial()
{
	s_chartboost->showInterstitial();
}

g_id gchartboost_addCallback(gevent_Callback callback, void *udata)
{
	return s_chartboost->addCallback(callback, udata);
}

void gchartboost_removeCallback(gevent_Callback callback, void *udata)
{
	s_chartboost->removeCallback(callback, udata);
}

void gchartboost_removeCallbackWithGid(g_id gid)
{
	s_chartboost->removeCallbackWithGid(gid);
}

}

