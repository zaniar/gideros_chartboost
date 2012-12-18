gideros_chartboost
==================

ChartBoost plugin for Gideros

Simple example:
```
require "chartboost"

chartboost:startSession("APP_ID", "APP_SIGNATURE")

chartboost:addEventListener(Event.CLICK_INTERSTITIAL, function(event)
	print("Interstitial Clicked", event.location)
end)

chartboost:addEventListener(Event.CLOSE_INTERSTITIAL, function(event)
	print("Interstitial Closed", event.location)
end)

chartboost:addEventListener(Event.DISMISS_INTERSTITIAL, function(event)
	print("Interstitial Dismissed", event.location)
end)

stage:addEventListener(Event.KEY_UP, function(event)
	if event.keyCode == KeyCode.BACK then
		application:exit()
	elseif event.keyCode == KeyCode.MENU then
		chartboost:showInterstitial()
	end
end)
```