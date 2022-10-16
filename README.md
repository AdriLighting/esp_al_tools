# esp_al_tools

## DEFINE




<details>
	
<summary>ALT_DEFINE_DEFAULT</summary>

```
if ALT_DEFINE_DEFAULT isnt defined la definatination par default est active
	#define
		FSOK
		USE_LITTLEF
		FILESYSTEM LittleFS 

		ALT_DEBUG_TARCE
		DEBUG_KEYBOARD
		ALSI_ENABLED

		OPENWEATHMAP_ENABLED
		WEATHERBIT_ENABLED
		WEATHERBITFORECAST_ENABLED

		ALWC_WS_OTA
		ALWC_WS_API
```

</details>

<details>

<summary>filesystem</summary>

- specifiy filessytem framewrok with USE_LITTLEFS || USE_SPIFFS
- specifiy the filessytem pointer with FILESYSTEM 
</details>


<details>
	
<summary>ALT_DEBUG_TARCE</summary>

```
if ALT_DEBUG_TARCE is defined  
	_DebugPrintList 
	ALT_TRACE	
```

</details>

<details>
	
<summary>DEBUG_KEYBOARD</summary>

```
if DEBUG_KEYBOARD is defined  

```

</details>

<details>
	
<summary>ALSI_ENABLED</summary>

```
if ALSI_ENABLED is defined  

```

</details>

</details>

<details>
	
<summary>ALTIMESYNC_ENABLED</summary>

```
if ALTIMESYNC_ENABLED is defined  

```

</details>

<details>
	
<summary>ALWORLDCLOCK_ENABLED</summary>

```
if ALWORLDCLOCK_ENABLED is defined  

```

</details>

<details>
	
<summary>weather</summary>

```
    -DOPENWEATHMAP_ENABLED
    -DWEATHERBIT_ENABLED
    -DWEATHERBITFORECAST_ENABLED

```

</details>




