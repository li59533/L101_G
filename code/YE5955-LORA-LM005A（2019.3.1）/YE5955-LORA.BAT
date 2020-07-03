SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;F:\Program Files (x86)\Embarcadero\Studio\18.0\bin;C:\Users\Public\Documents\Embarcadero\Studio\18.0\Bpl;F:\Program Files (x86)\Embarcadero\Studio\18.0\bin64;C:\Users\Public\Documents\Embarcadero\Studio\18.0\Bpl\Win64;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;F:\Program Files\MATLAB\R2014b\runtime\win64;F:\Program Files\MATLAB\R2014b\bin;F:\Program Files\MATLAB\R2014b\polyspace\bin;C:\Users\ococ\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=MKL16Z64xxx4
SET CPU_VENDOR=NXP
SET UV2_TARGET=YE5955-LORA
SET CPU_CLOCK=0x00B71B00
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_adc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_gpio.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_spi.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_tpm.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_uart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_pit.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_lptmr.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mkl_lpo_rtc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\retarget_mdk.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\mcg.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\arm_cm0.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_mkl25z4._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\system8m.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\modbus.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\w25qxx.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\flash.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\YE5955-LORA.lnp"
"D:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\YE5955-LORA.axf" --i32combined --output ".\YE5955-LORA.hex"
