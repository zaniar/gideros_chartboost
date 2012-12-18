package com.nightspade.gideros.android.plugins.chartboost;

import java.lang.ref.WeakReference;

import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;

import android.app.Activity;
import android.view.KeyEvent;

public class GChartBoost {
	private static WeakReference<Activity> sActivity;
	private static GChartBoost sInstance;
	private static long sData;
	private static Chartboost sChartboost;
	private static ChartboostDelegate sChartBoostDelegate;
	
	public static void onCreate(Activity activity)
	{		
		sActivity = new WeakReference<Activity>(activity);;
	}
	
	public static void onStart()
	{
		sChartboost.onStart(sActivity.get());
	}
	
	public static void onStop()
	{
		sChartboost.onStop(sActivity.get());
	}
	
	static public void init(long data)
	{	
		sData = data;
		
		sInstance = new GChartBoost(sActivity.get());
	}
	
	static public void cleanup()
	{
		if (sInstance != null)
		{
			sData = 0;
			sInstance = null;
		}
	}
	
	public GChartBoost(Activity activity)
	{
	}
	
	static public void onDestroy()
	{
		cleanup();
	}
	
	static public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (sChartboost.onBackPressed())
			// If a Chartboost view exists, close it and return
			return true;
		else
			// If no Chartboost view exists, continue on as normal
			return false;
	}
	
	static public void startSession(String appId, String appSignature)
	{	
		sChartBoostDelegate = new ChartboostDelegate() {

			@Override
			public void didCacheInterstitial(String location) {
				onCacheInterstitial(location, sData);
			}

			@Override
			public void didCacheMoreApps() {
				onCacheMoreApps(sData);
			}

			@Override
			public void didClickInterstitial(String location) {
				onClickInterstitial(location, sData);
			}

			@Override
			public void didClickMoreApps() {
				onClickMoreApps(sData);
			}

			@Override
			public void didCloseInterstitial(String location) {
				onCloseInterstitial(location, sData);
			}

			@Override
			public void didCloseMoreApps() {
				onCloseMoreApps(sData);
			}

			@Override
			public void didDismissInterstitial(String location) {
				onDismissInterstitial(location, sData);
			}

			@Override
			public void didDismissMoreApps() {
				onDismissMoreApps(sData);
			}

			@Override
			public void didFailToLoadInterstitial(String location) {
				onFailToLoadInterstitial(location, sData);
			}

			@Override
			public void didFailToLoadMoreApps() {
				onFailToLoadMoreApps(sData);
			}

			@Override
			public void didShowInterstitial(String location) {
				onShowInterstitial(location, sData);
			}

			@Override
			public void didShowMoreApps() {
				onShowMoreApps(sData);
			}

			@Override
			public boolean shouldDisplayInterstitial(String location) {
				return true;
			}

			@Override
			public boolean shouldDisplayLoadingViewForMoreApps() {
				return true;
			}

			@Override
			public boolean shouldDisplayMoreApps() {
				return true;
			}

			@Override
			public boolean shouldRequestInterstitial(String location) {
				return true;
			}

			@Override
			public boolean shouldRequestInterstitialsInFirstSession() {
				return true;
			}

			@Override
			public boolean shouldRequestMoreApps() {
				return true;
			}
			
		};
		
		sChartboost = Chartboost.sharedChartboost();
		sChartboost.onCreate(sActivity.get(), appId, appSignature, sChartBoostDelegate);
		
		try
		{
			sActivity.get().runOnUiThread(new Runnable() {
 
				@Override
				public void run() {
					sChartboost.startSession();
				}
			});
 
		}
		catch(Exception ex)	{}
	}
	
	static public void showInterstitial()
	{
		sChartboost.showInterstitial();
	}
	
	static public void showMoreApps()
	{
		sChartboost.showMoreApps();
	}
	
	static public void cacheInterstitial()
	{
		sChartboost.cacheInterstitial();
	}
	
	static public void cacheMoreApps()
	{
		sChartboost.cacheMoreApps();
	}
	
	static public void clearCache()
	{
		sChartboost.clearCache();
	}
	
	private static native void onCacheInterstitial(String location, long data);
	private static native void onCacheMoreApps(long data);
	private static native void onClickInterstitial(String location, long data);
	private static native void onClickMoreApps(long data);
	private static native void onCloseInterstitial(String location, long data);
	private static native void onCloseMoreApps(long data);
	private static native void onDismissInterstitial(String location, long data);
	private static native void onDismissMoreApps(long data);
	private static native void onFailToLoadInterstitial(String location, long data);
	private static native void onFailToLoadMoreApps(long data);
	private static native void onShowInterstitial(String location, long data);
	private static native void onShowMoreApps(long data);
}
