# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/ispr/ispr2024/tutorials/lab1/sw/workspace/system_wrapper/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/ispr/ispr2024/tutorials/lab1/sw/workspace/system_wrapper/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {system_wrapper}\
-hw {/home/ispr/ispr2024/ispr2/ispr2024/tutorials/lab1/hw/system_wrapper.xsa}\
-no-boot-bsp -out {/home/ispr/ispr2024/tutorials/lab1/sw/workspace}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {system_wrapper}
platform generate -quick
platform generate
platform generate
platform active {system_wrapper}
platform generate -domains 
