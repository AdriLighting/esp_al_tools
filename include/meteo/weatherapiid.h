#ifndef WEATHERAPIID_H
  #define WEATHERAPIID_H
    #include <Arduino.h>
    // #define WEATHERBIT_APP_ID  Sting("abb4137bd9474b30ab29f9d286a268e6")

    // #define WEATHER_LOCATION_ID Sting("2972214")
    // #define WEATHER_LANGUAGE    Sting("fr")
    // 
    //IS_METRIC ? "metric" : "imperial";
static const char  WEATHERBIT_APPID   [] PROGMEM = "4b771a18fe0640a9939fd9492e162e00"; // abb4137bd9474b30ab29f9d286a268e6
static const char  OPENWEATHERMAP_APPID [] PROGMEM = "603ef62761eb23b76ac8c8af7711776f";

static const char  WEATHER_LOCATIONID   [] PROGMEM = "2972214";
static const char  WEATHER_LANGUAGE     [] PROGMEM = "fr";


// http://api.openweathermap.org/data/2.5/weather?id=
static const char  OPENWEATHERMAP_URL     [] PROGMEM = "http://api.openweathermap.org/data/2.5/weather/";
// http://api.weatherbit.io/v2.0/current?city_id=
static const char  WEATHERBIT_URL         [] PROGMEM = "http://api.weatherbit.io/v2.0/";
// http://api.weatherbit.io/v2.0/forecast/daily?city_id=
// static const char  WEATHERBITFORECAST_URL [] PROGMEM = "http://api.weatherbit.io/v2.0/forecast/";
class alt_weatherApi
{
public:
  alt_weatherApi();
  ~alt_weatherApi(){};
  
  char * _wb_id       = nullptr;
  char * _ow_id       = nullptr;
  char * _wb_location = nullptr;
  char * _ow_location = nullptr;
  char * _lang        = nullptr;
};
extern alt_weatherApi _alt_weatherApi; 

static const char ALMWBF_KEY_VALID_DATE            [] PROGMEM = "valid_date";
static const char ALMWBF_KEY_TS                    [] PROGMEM = "ts";
static const char ALMWBF_KEY_WIND_GUST_SPD         [] PROGMEM = "wind_gust_spd";
static const char ALMWBF_KEY_MAX_TEMP              [] PROGMEM = "max_temp";
static const char ALMWBF_KEY_MIN_TEMP              [] PROGMEM = "min_temp";
static const char ALMWBF_KEY_HIGH_TEMP             [] PROGMEM = "high_temp";
static const char ALMWBF_KEY_LOW_TEMP              [] PROGMEM = "low_temp";
static const char ALMWBF_KEY_APP_MAX_TEMP          [] PROGMEM = "app_max_temp";
static const char ALMWBF_KEY_APP_MIN_TEMP          [] PROGMEM = "app_min_temp";
static const char ALMWBF_KEY_POP                   [] PROGMEM = "pop";
static const char ALMWBF_KEY_SNOW_DEPTH            [] PROGMEM = "snow_depth";
static const char ALMWBF_KEY_CLOUDS_LOW            [] PROGMEM = "clouds_low";
static const char ALMWBF_KEY_CLOUDS_MID            [] PROGMEM = "clouds_mid";
static const char ALMWBF_KEY_CLOUDS_HI             [] PROGMEM = "clouds_hi";
static const char ALMWBF_KEY_MAX_DHI               [] PROGMEM = "max_dhi";
static const char ALMWBF_KEY_OZONE                 [] PROGMEM = "ozone";
static const char ALMWBF_KEY_MOON_PHASE            [] PROGMEM = "moon_phase";
static const char ALMWBF_KEY_MOONRISE_TS           [] PROGMEM = "moonrise_ts";
static const char ALMWBF_KEY_MOONSET_TS            [] PROGMEM = "moonset_ts";
static const char ALMWBF_KEY_SUNRISE_TS            [] PROGMEM = "sunrise_ts";
static const char ALMWBF_KEY_SUNSET_TS             [] PROGMEM = "sunset_ts";

static const char ALMWBF_DESC_VALID_DATE        [] PROGMEM = "Date the forecast is valid for in format YYYY-MM-DD. [Midnight to midnight local time]";
static const char ALMWBF_DESC_TS                [] PROGMEM = "Forecast period start unix timestamp (UTC).";
static const char ALMWBF_DESC_WIND_GUST_SPD     [] PROGMEM = "Wind gust speed (Default m/s).";
static const char ALMWBF_DESC_WIND_CDIR_FULL    [] PROGMEM = "Verbal wind direction.";
static const char ALMWBF_DESC_MAX_TEMP          [] PROGMEM = "Maximum Temperature (default Celcius).";
static const char ALMWBF_DESC_MIN_TEMP          [] PROGMEM = "Minimum Temperature (default Celcius).";
static const char ALMWBF_DESC_HIGH_TEMP         [] PROGMEM = "High Temperature - Calculated from 6AM to 6AM local time (default Celcius).";
static const char ALMWBF_DESC_LOW_TEMP          [] PROGMEM = "Low Temperature - Calculated from 6AM to 6AM local (default Celcius).";
static const char ALMWBF_DESC_APP_MAX_TEMP      [] PROGMEM = "Apparent/Feels Like temperature at max_temp time (default Celcius).";
static const char ALMWBF_DESC_APP_MIN_TEMP      [] PROGMEM = "Apparent/Feels Like temperature at min_temp time (default Celcius).";
static const char ALMWBF_DESC_POP               [] PROGMEM = "Probability of Precipitation (%).";
static const char ALMWBF_DESC_SNOW_DEPTH        [] PROGMEM = "Snow Depth (default mm).";
static const char ALMWBF_DESC_CLOUDS_LOW        [] PROGMEM = "Low-level (~0-3km AGL) cloud coverage (%).";
static const char ALMWBF_DESC_CLOUDS_MID        [] PROGMEM = "Mid-level (~3-5km AGL) cloud coverage (%).";
static const char ALMWBF_DESC_CLOUDS_HI         [] PROGMEM = "High-level (>5km AGL) cloud coverage (%).";
static const char ALMWBF_DESC_MAX_DHI           [] PROGMEM = "[DEPRECATED] Maximum direct component of solar radiation (W/m^2).";
static const char ALMWBF_DESC_OZONE             [] PROGMEM = "Average Ozone (Dobson units).";
static const char ALMWBF_DESC_MOON_PHASE        [] PROGMEM = "Moon phase fraction (0-1).";
static const char ALMWBF_DESC_MOONRISE_TS       [] PROGMEM = "Moonrise time unix timestamp (UTC).";
static const char ALMWBF_DESC_MOONSET_TS        [] PROGMEM = "Moonset time unix timestamp (UTC).";
static const char ALMWBF_DESC_SUNRISE_TS        [] PROGMEM = "Sunrise time unix timestamp (UTC).";
static const char ALMWBF_DESC_SUNSET_TS         [] PROGMEM = "Sunset time unix timestamp (UTC) .";

static const char ALMWB_KEY_LAT            [] PROGMEM = "lat";
static const char ALMWB_KEY_LON            [] PROGMEM = "lon";
static const char ALMWB_KEY_SUNRISE        [] PROGMEM = "sunrise";
static const char ALMWB_KEY_SUNSET         [] PROGMEM = "sunset";
static const char ALMWB_KEY_TIMEZONE       [] PROGMEM = "timezone";
static const char ALMWB_KEY_STATION        [] PROGMEM = "station";
static const char ALMWB_KEY_OB_TIME        [] PROGMEM = "ob_time";
static const char ALMWB_KEY_DATETIME       [] PROGMEM = "datetime";
static const char ALMWB_KEY_TS             [] PROGMEM = "ts";
static const char ALMWB_KEY_CITY_NAME      [] PROGMEM = "city_name";
static const char ALMWB_KEY_COUNTRY_CODE   [] PROGMEM = "country_code";
static const char ALMWB_KEY_STATE_CODE     [] PROGMEM = "state_code";
static const char ALMWB_KEY_PRES           [] PROGMEM = "pres";
static const char ALMWB_KEY_SLP            [] PROGMEM = "slp";
static const char ALMWB_KEY_WIND_SPD       [] PROGMEM = "wind_spd";
static const char ALMWB_KEY_WIND_DIR       [] PROGMEM = "wind_dir";
static const char ALMWB_KEY_WIND_CDIR      [] PROGMEM = "wind_cdir";
static const char ALMWB_KEY_WIND_CDIR_FULL [] PROGMEM = "wind_cdir_full";
static const char ALMWB_KEY_TEMP           [] PROGMEM = "temp";
static const char ALMWB_KEY_APP_TEMP       [] PROGMEM = "app_temp";
static const char ALMWB_KEY_RH             [] PROGMEM = "rh";
static const char ALMWB_KEY_DEWPT          [] PROGMEM = "dewpt";
static const char ALMWB_KEY_CLOUDS         [] PROGMEM = "clouds";
static const char ALMWB_KEY_POD            [] PROGMEM = "pod";
static const char ALMWB_KEY_ICON           [] PROGMEM = "icon";
static const char ALMWB_KEY_CODE           [] PROGMEM = "code";
static const char ALMWB_KEY_DESCRIPTION    [] PROGMEM = "description";
static const char ALMWB_KEY_VIS            [] PROGMEM = "vis";
static const char ALMWB_KEY_PRECIP         [] PROGMEM = "precip";
static const char ALMWB_KEY_SNOW           [] PROGMEM = "snow";
static const char ALMWB_KEY_UV             [] PROGMEM = "uv";
static const char ALMWB_KEY_AQI            [] PROGMEM = "aqi";
static const char ALMWB_KEY_DHI            [] PROGMEM = "dhi";
static const char ALMWB_KEY_DNI            [] PROGMEM = "dni";
static const char ALMWB_KEY_GHI            [] PROGMEM = "ghi";
static const char ALMWB_KEY_SOLAR_RAD      [] PROGMEM = "solar_rad";
static const char ALMWB_KEY_ELEV_ANGLE     [] PROGMEM = "elev_angle";
static const char ALMWB_KEY_H_ANGLE        [] PROGMEM = "h_angle";

static const char ALMWB_DESC_LAT            [] PROGMEM = "Latitude (Degrees).";
static const char ALMWB_DESC_LON            [] PROGMEM = "Longitude (Degrees).";
static const char ALMWB_DESC_SUNRISE        [] PROGMEM = "Sunrise time (HH:MM).";
static const char ALMWB_DESC_SUNSET         [] PROGMEM = "Sunset time (HH:MM).";
static const char ALMWB_DESC_TIMEZONE       [] PROGMEM = "Local IANA Timezone.";
static const char ALMWB_DESC_STATION        [] PROGMEM = "Source station ID.";
static const char ALMWB_DESC_OB_TIME        [] PROGMEM = "Last observation time (YYYY-MM-DD HH:MM).";
static const char ALMWB_DESC_DATETIME       [] PROGMEM = "Current cycle hour (YYYY-MM-DD:HH).";
static const char ALMWB_DESC_TS             [] PROGMEM = "Last observation time (Unix timestamp).";
static const char ALMWB_DESC_CITY_NAME      [] PROGMEM = "City name.";
static const char ALMWB_DESC_COUNTRY_CODE   [] PROGMEM = "Country abbreviation.";
static const char ALMWB_DESC_STATE_CODE     [] PROGMEM = "State abbreviation/code.";
static const char ALMWB_DESC_PRES           [] PROGMEM = "Pressure (mb).";
static const char ALMWB_DESC_SLP            [] PROGMEM = "Sea level pressure (mb).";
static const char ALMWB_DESC_WIND_SPD       [] PROGMEM = "Wind speed (Default m/s).";
static const char ALMWB_DESC_WIND_DIR       [] PROGMEM = "Wind direction (degrees).";
static const char ALMWB_DESC_WIND_CDIR      [] PROGMEM = "Abbreviated wind direction.";
static const char ALMWB_DESC_WIND_CDIR_FULL [] PROGMEM = "Verbal wind direction.";
static const char ALMWB_DESC_TEMP           [] PROGMEM = "Temperature (default Celcius).";
static const char ALMWB_DESC_APP_TEMP       [] PROGMEM = "Apparent/Feels Like temperature (default Celcius).";
static const char ALMWB_DESC_RH             [] PROGMEM = "Relative humidity (%).";
static const char ALMWB_DESC_DEWPT          [] PROGMEM = "Dew point (default Celcius).";
static const char ALMWB_DESC_CLOUDS         [] PROGMEM = "Cloud coverage (%).";
static const char ALMWB_DESC_POD            [] PROGMEM = "Part of the day (d = day / n = night).";
static const char ALMWB_DESC_ICON           [] PROGMEM = "Weather icon code.";
static const char ALMWB_DESC_CODE           [] PROGMEM = "Weather code.";
static const char ALMWB_DESC_DESCRIPTION    [] PROGMEM = "Text weather description.";
static const char ALMWB_DESC_VIS            [] PROGMEM = "Visibility (default KM).";
static const char ALMWB_DESC_PRECIP         [] PROGMEM = "Liquid equivalent precipitation rate (default mm/hr).";
static const char ALMWB_DESC_SNOW           [] PROGMEM = "Snowfall (default mm/hr).";
static const char ALMWB_DESC_UV             [] PROGMEM = "UV Index (0-11+).";
static const char ALMWB_DESC_AQI            [] PROGMEM = "Air Quality Index [US - EPA standard 0 - +500]";
static const char ALMWB_DESC_DHI            [] PROGMEM = "Diffuse horizontal solar irradiance (W/m^2) [Clear Sky]";
static const char ALMWB_DESC_DNI            [] PROGMEM = "Direct normal solar irradiance (W/m^2) [Clear Sky]";
static const char ALMWB_DESC_GHI            [] PROGMEM = "Global horizontal solar irradiance (W/m^2) [Clear Sky]";
static const char ALMWB_DESC_SOLAR_RAD      [] PROGMEM = "Estimated Solar Radiation (W/m^2).";
static const char ALMWB_DESC_ELEV_ANGLE     [] PROGMEM = "Solar elevation angle (degrees).";
static const char ALMWB_DESC_H_ANGLE        [] PROGMEM = "Solar hour angle (degrees).";




#endif // WEATHERAPIID_H