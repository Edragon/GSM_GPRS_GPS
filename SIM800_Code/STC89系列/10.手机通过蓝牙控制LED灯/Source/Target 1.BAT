SET C51INC=d:\Keil_v5\C51\Inc;d:\Keil_v5\C51\Inc\Atmel
SET C51LIB=d:\Keil_v5\C51\Lib
SET CPU_TYPE=AT89C52
SET CPU_VENDOR=Atmel
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x016E3600
"d:\Keil_v5\C51\BIN\A51.EXE" @"..\Output\STARTUP._ia"
"d:\Keil_v5\C51\BIN\C51.EXE" @"..\Output\delay.__i"
"d:\Keil_v5\C51\BIN\C51.EXE" @"..\Output\main.__i"
"d:\Keil_v5\C51\BIN\C51.EXE" @"..\Output\uart.__i"
"d:\Keil_v5\C51\BIN\BL51.EXE" @"..\Output\project.lnp"
"d:\Keil_v5\C51\BIN\OH51.EXE" "..\Output\project" 
