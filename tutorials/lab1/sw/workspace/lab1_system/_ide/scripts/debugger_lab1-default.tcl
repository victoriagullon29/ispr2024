# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/ispr/ispr2024/tutorials/lab1/sw/workspace/lab1_system/_ide/scripts/debugger_lab1-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source /home/ispr/ispr2024/tutorials/lab1/sw/workspace/lab1_system/_ide/scripts/debugger_lab1-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo 210279787097A" && level==0 && jtag_device_ctx=="jsn-Zybo-210279787097A-13722093-0"}
fpga -file /home/ispr/ispr2024/tutorials/lab1/sw/workspace/lab1/_ide/bitstream/system_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw /home/ispr/ispr2024/tutorials/lab1/sw/workspace/system_wrapper/export/system_wrapper/hw/system_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source /home/ispr/ispr2024/tutorials/lab1/sw/workspace/lab1/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow /home/ispr/ispr2024/tutorials/lab1/sw/workspace/lab1/Debug/lab1.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
