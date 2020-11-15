
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project( libclick C )

azsphere_configure_tools(TOOLS_REVISION "20.04") 
azsphere_configure_api(TARGET_API_SET "7") 

set( clickmodules_inc "LIGHTRANGER/core/inc" "LIGHTRANGER/platform/inc" "AIRQUALITY5" "BAROMETER"  )
set( clickmodules_inc ${clickmodules_inc} "FLAME" "HEARTRATE4" "LCDMINI" "LSM6DSL" "OLED-B" "RELAY" "TEMP_HUM" )

set( clickmodules_src "AIRQUALITY5/ads1015.c" "AIRQUALITY5/air_quality_5.c" )
set( clickmodules_src ${clickmodules_src} "BAROMETER/barometer.c" )
set( clickmodules_src ${clickmodules_src} "FLAME/flame.c" )
set( clickmodules_src ${clickmodules_src} "HEARTRATE4/algorithm_by_RF.c" "HEARTRATE4/max30102.c" )
set( clickmodules_src ${clickmodules_src} "LCDMINI/lcdmini.c" )
set( clickmodules_src ${clickmodules_src} "LSM6DSL/LSM6DSL_ACC_GYRO_Driver.c" "LSM6DSL/lsm6dsl.c" "LSM6DSL/LSM6DSLSensor.c" )
set( clickmodules_src ${clickmodules_src} "OLED-B/Avnet_GFX.c" "OLED-B/glcdfont.c" "OLED-B/oledb_ssd1306.c" )
set( clickmodules_src ${clickmodules_src} "RELAY/relay.c" )
set( clickmodules_src ${clickmodules_src} "TEMP_HUM/hts221.c" )
set( clickmodules_src ${clickmodules_src} "LIGHTRANGER/platform/src/vl53l0x_platform.c" )
set( clickmodules_src ${clickmodules_src} "LIGHTRANGER/core/src/vl53l0x_api.c" "LIGHTRANGER/core/src/vl53l0x_api_calibration.c" )
set( clickmodules_src ${clickmodules_src} "LIGHTRANGER/core/src/vl53l0x_api_core.c" "LIGHTRANGER/core/src/vl53l0x_api_ranging.c" )
set( clickmodules_src ${clickmodules_src} "LIGHTRANGER/core/src/vl53l0x_api_strings.c" )

include_directories( ${clickmodules_inc} )

add_library( click STATIC ${clickmodules_src} )

target_compile_options(click PRIVATE -w)
