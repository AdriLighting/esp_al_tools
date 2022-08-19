#include "../include/aldatestring.h"
#include "../include/altools.h"

#ifndef ARRAY_SIZE
	#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#endif


const char monthStr0_en 	[] PROGMEM  = "Err";
const char monthStr1_en 	[] PROGMEM  = "January";
const char monthStr2_en 	[] PROGMEM  = "February";
const char monthStr3_en 	[] PROGMEM  = "March";
const char monthStr4_en 	[] PROGMEM  = "April";
const char monthStr5_en 	[] PROGMEM  = "May";
const char monthStr6_en 	[] PROGMEM  = "June";
const char monthStr7_en 	[] PROGMEM  = "July";
const char monthStr8_en 	[] PROGMEM  = "August";
const char monthStr9_en 	[] PROGMEM  = "September";
const char monthStr10_en 	[] PROGMEM  = "October";
const char monthStr11_en 	[] PROGMEM  = "November";
const char monthStr12_en 	[] PROGMEM  = "December";

const char monthShortStr0_en 	[] PROGMEM  = "Err";
const char monthShortStr1_en 	[] PROGMEM  = "Jan";
const char monthShortStr2_en 	[] PROGMEM  = "Feb";
const char monthShortStr3_en 	[] PROGMEM  = "Mar";
const char monthShortStr4_en 	[] PROGMEM  = "Apr";
const char monthShortStr5_en 	[] PROGMEM  = "May";
const char monthShortStr6_en 	[] PROGMEM  = "Jun";
const char monthShortStr7_en 	[] PROGMEM  = "Jul";
const char monthShortStr8_en 	[] PROGMEM  = "Aug";
const char monthShortStr9_en 	[] PROGMEM  = "Sep";
const char monthShortStr10_en 	[] PROGMEM  = "Oct";
const char monthShortStr11_en 	[] PROGMEM  = "Nov";
const char monthShortStr12_en 	[] PROGMEM  = "Dec";

const char monthStr0_fr 	[] PROGMEM  = "Err";
const char monthStr1_fr 	[] PROGMEM  = "Janvier";
const char monthStr2_fr 	[] PROGMEM  = "Fevrier";
const char monthStr3_fr 	[] PROGMEM  = "Mars";
const char monthStr4_fr 	[] PROGMEM  = "Avril";
const char monthStr5_fr 	[] PROGMEM  = "Mai";
const char monthStr6_fr 	[] PROGMEM  = "Juin";
const char monthStr7_fr 	[] PROGMEM  = "Juillet";
const char monthStr8_fr 	[] PROGMEM  = "Aout";
const char monthStr9_fr 	[] PROGMEM  = "Septembre";
const char monthStr10_fr 	[] PROGMEM  = "Octobre";
const char monthStr11_fr 	[] PROGMEM  = "Novembre";
const char monthStr12_fr 	[] PROGMEM  = "Decembre";

const char monthShortStr0_fr	[] PROGMEM  = "Err";
const char monthShortStr1_fr	[] PROGMEM  = "Jan";
const char monthShortStr2_fr	[] PROGMEM  = "Fev";
const char monthShortStr3_fr	[] PROGMEM  = "Mar";
const char monthShortStr4_fr	[] PROGMEM  = "Avr";
const char monthShortStr5_fr	[] PROGMEM  = "Mai";
const char monthShortStr6_fr	[] PROGMEM  = "Jun";
const char monthShortStr7_fr	[] PROGMEM  = "Jui";
const char monthShortStr8_fr	[] PROGMEM  = "Aou";
const char monthShortStr9_fr	[] PROGMEM  = "Sep";
const char monthShortStr10_fr 	[] PROGMEM  = "Oct";
const char monthShortStr11_fr 	[] PROGMEM  = "Nov";
const char monthShortStr12_fr 	[] PROGMEM  = "Dec";

const char dayStr0_en [] PROGMEM = "Err";
const char dayStr1_en [] PROGMEM = "Sunday";
const char dayStr2_en [] PROGMEM = "Monday";
const char dayStr3_en [] PROGMEM = "Tuesday";
const char dayStr4_en [] PROGMEM = "Wednesday";
const char dayStr5_en [] PROGMEM = "Thursday";
const char dayStr6_en [] PROGMEM = "Friday";
const char dayStr7_en [] PROGMEM = "Saturday";

const char dayShortStr0_en [] PROGMEM = "Err";
const char dayShortStr1_en [] PROGMEM = "Sun";
const char dayShortStr2_en [] PROGMEM = "Mon";
const char dayShortStr3_en [] PROGMEM = "Tue";
const char dayShortStr4_en [] PROGMEM = "Wed";
const char dayShortStr5_en [] PROGMEM = "Thu";
const char dayShortStr6_en [] PROGMEM = "Fri";
const char dayShortStr7_en [] PROGMEM = "Sat";

const char dayStr0_fr [] PROGMEM = "Err";
const char dayStr1_fr [] PROGMEM = "Dimanche";
const char dayStr2_fr [] PROGMEM = "Lundi";
const char dayStr3_fr [] PROGMEM = "Mardi";
const char dayStr4_fr [] PROGMEM = "Mercredi";
const char dayStr5_fr [] PROGMEM = "Jeudi";
const char dayStr6_fr [] PROGMEM = "Vendredi";
const char dayStr7_fr [] PROGMEM = "Samedi";

const char dayShortStr0_fr [] PROGMEM = "Err";
const char dayShortStr1_fr [] PROGMEM = "Dim";
const char dayShortStr2_fr [] PROGMEM = "Lun";
const char dayShortStr3_fr [] PROGMEM = "Mar";
const char dayShortStr4_fr [] PROGMEM = "Mer";
const char dayShortStr5_fr [] PROGMEM = "Jeu";
const char dayShortStr6_fr [] PROGMEM = "Ven";
const char dayShortStr7_fr [] PROGMEM = "Sam";

const al_dateString_days al_dateString_days_t [] PROGMEM = { 
	{dayStr1_en, dayStr1_fr, dayShortStr1_en, dayShortStr1_fr},
	{dayStr2_en, dayStr2_fr, dayShortStr2_en, dayShortStr2_fr},
	{dayStr3_en, dayStr3_fr, dayShortStr3_en, dayShortStr3_fr},
	{dayStr4_en, dayStr4_fr, dayShortStr4_en, dayShortStr4_fr},
	{dayStr5_en, dayStr5_fr, dayShortStr5_en, dayShortStr5_fr},
	{dayStr6_en, dayStr6_fr, dayShortStr6_en, dayShortStr6_fr},
	{dayStr7_en, dayStr7_fr, dayShortStr7_en, dayShortStr7_fr},
};
uint8_t calendar_dateCount = ARRAY_SIZE(al_dateString_days_t);	

const al_dateString_month al_dateString_month_t [] PROGMEM = { 
	// {monthStr0_en, monthStr0_fr, (const char*)"err"},
	{monthStr1_en, 	monthStr1_fr, 	monthShortStr1_en, 	monthShortStr1_fr	},
	{monthStr2_en, 	monthStr2_fr, 	monthShortStr2_en, 	monthShortStr2_fr	},
	{monthStr3_en, 	monthStr3_fr, 	monthShortStr3_en, 	monthShortStr3_fr	},
	{monthStr4_en, 	monthStr4_fr, 	monthShortStr4_en, 	monthShortStr4_fr	},
	{monthStr5_en, 	monthStr5_fr, 	monthShortStr5_en, 	monthShortStr5_fr	},
	{monthStr6_en, 	monthStr6_fr, 	monthShortStr6_en, 	monthShortStr6_fr	},
	{monthStr7_en, 	monthStr7_fr, 	monthShortStr7_en, 	monthShortStr7_fr	},
	{monthStr8_en, 	monthStr8_fr, 	monthShortStr8_en, 	monthShortStr8_fr	},
	{monthStr9_en, 	monthStr9_fr, 	monthShortStr9_en, 	monthShortStr9_fr	},
	{monthStr10_en, monthStr10_fr, 	monthShortStr10_en,	monthShortStr10_fr	},
	{monthStr11_en, monthStr11_fr, 	monthShortStr11_en,	monthShortStr11_fr	},
	{monthStr12_en, monthStr12_fr, 	monthShortStr12_en,	monthShortStr12_fr	},
};	
uint8_t calendar_monthCount = ARRAY_SIZE(al_dateString_month_t);

namespace al_datestr {
	int8_t get_dow(const String & country, const String & day) {
		int8_t ret = -1;
		String iDay;
		for (uint8_t i = 0; i < calendar_dateCount; ++i) {
			if (country == "fr") iDay = al_tools::ch_toString(al_dateString_days_t[i].fr);
			if (country == "en") iDay = al_tools::ch_toString(al_dateString_days_t[i].en);

			if ( iDay == day) {
				ret = i;
				break;
			}
			
		}
		return ret;
	}	
	int8_t get_dow(const String & country, const char * day) {
		int8_t ret = -1;
		String iDay;
		for (uint8_t i = 0; i < calendar_dateCount; ++i) {
			if (country == "fr") iDay = al_tools::ch_toString(al_dateString_days_t[i].fr);
			if (country == "en") iDay = al_tools::ch_toString(al_dateString_days_t[i].en);

			if ( iDay == al_tools::ch_toString(day)) {
				ret = i;
				break;
			}
			
		}
		return ret;
	}		
	void get_dowStr(const String & country, const String & day, String & result) {
	    	// Serial.printf("\n[calendar_date_get_dowStr] search_day: %s\n", day.c_str());
		
		String ret = "";
		String iDay;
		for (int i = 0; i < calendar_dateCount; ++i) {
			if (country == "fr") iDay = al_tools::ch_toString(al_dateString_days_t[i].fr);
			if (country == "en") iDay = al_tools::ch_toString(al_dateString_days_t[i].en);

			if ( iDay == day) {
				if (country == "en") ret = al_tools::ch_toString(al_dateString_days_t[i].fr);
				if (country == "fr") ret = al_tools::ch_toString(al_dateString_days_t[i].en);
				break;
			}
			
		}
		result = ret;
	}

	int8_t get_month(const String & country, const String & search) {
		int8_t ret = -1;
		String iMon = "";
		for (uint8_t i = 0; i < calendar_monthCount; ++i) {
			if (country == "fr") iMon = al_tools::ch_toString(al_dateString_month_t[i].fr);
			if (country == "en") iMon = al_tools::ch_toString(al_dateString_month_t[i].en);

			if ( iMon == search) {
				ret = i;
				break;
			}
			
		}
		return ret;
	}
	void get_monyhStr(const String & country, const String & search, String & result) {
		String ret = "";
		String iMon;
		for (uint8_t i = 0; i < calendar_monthCount; ++i) {
			if (country == "fr") iMon = al_tools::ch_toString(al_dateString_month_t[i].fr);
			if (country == "en") iMon = al_tools::ch_toString(al_dateString_month_t[i].en);

			if ( iMon == search) {
				if (country == "en") ret = al_tools::ch_toString(al_dateString_month_t[i].fr);
				if (country == "fr") ret = al_tools::ch_toString(al_dateString_month_t[i].en);
				break;
			}
			
		}
		result = ret;
	}
	int8_t get_month(const String & country, const char * search) {
		int ret = -1;
		String iMon;
		for (uint8_t i = 0; i < calendar_monthCount; ++i) {
			if (country == "fr") iMon = al_tools::ch_toString(al_dateString_month_t[i].fr);
			if (country == "en") iMon = al_tools::ch_toString(al_dateString_month_t[i].en);

			if ( iMon == al_tools::ch_toString(search) ) {
				ret = i;
				break;
			}
			
		}
		return ret;
	}
}
