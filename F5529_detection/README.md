# MSP430F5529 LaunchPad 扩展板V2.1/V2.2案例工程
扩展板链接：[MSP430F5529 LaunchPad 扩展板](https://github.com/422lab/F5529_Expansion_board)  
目前master分支基于V2.1/V2.2版本的扩展板  
## 工程源码支持
* GPIO
* UART串口
* 片内ADC12
* PWM/SPWM(其中包含定时器)
* SSD1351
* SSD1306
* LCD12864
* ADS1256
* ADS1118
* DAC8571
* INA260
* INA226
* 矩阵键盘/按键
## IDE
该工程使用了[Code Composer Studio 11](https://www.ti.com.cn/tool/cn/CCSTUDIO)作为IDE，使用这个版本及以上均可，别在那儿瞎捣鼓CCSv5了，用新版本他不香吗
## 如何使用工程
在git clone完工程后  
在CCS中通过Project -> Import CCS Project...导入工程  
若提示导入失败，则勾选Copy projects into workspace  
在成功导入工程后，编译烧录即可  
## 注意事项
目前只测试了各个模块能否单独使用，联合使用可能会或多或少存在bug，若能自行修复请修复后提交Pull，若不能自行修复请及时提交Issues，有看到会进行测试解决并提出相应方案，前提是我手边还有F5529和扩展板  
## 如果扩展板升级。。
如果有对扩展板进行升级，且相应的引脚等不再适用于该案例工程，请自行创建分支测试，测试通过后提交Pull，因为有极大概率我(Zuoge)对于扩展板的硬件只负责到V2.2版本，注意升级后的扩展板硬件请同步更新到[MSP430F5529 LaunchPad 扩展板](https://github.com/422lab/F5529_Expansion_board)  