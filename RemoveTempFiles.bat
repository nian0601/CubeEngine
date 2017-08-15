rd /s /q temp
rd /s /q Solution\ipch

del Solution\Solution.sdf
del /A:H Solution\Solution.v12.suo

del Libs\CommonUtilities_Release.lib
del Libs\Game_Release.lib

del Libs\CommonUtilities_Debug.lib
del Libs\Game_Debug.lib