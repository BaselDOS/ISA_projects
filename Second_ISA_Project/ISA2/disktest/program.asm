Disk:
out $imm2, $zero, $imm1, 14, 1                                   # set Diskcmd to reading case
add $t0, $zero, $zero, 0, 0                                      #$t0=0
add $t1, $zero, $imm1, 0x100, 0                                  #set $t1 to adrress 0x100 to be the first sector buffer copy 
add $t2, $zero, $imm1, 0x200, 0                                  #set $t2 to adrress 0x200 to be the second sector buffer copy 
sll $sp, $imm1, $imm2, 1, 11                                     # set stack position to be address 2048
add $sp, $sp, $imm1, -2, 0                                       #adjust stackk for 2 items
sw $s0, $sp, $imm1, 0, 0                                         #save $s0 value
sw $s1, $sp, $imm1, 0, 0                                         #save $s1 value
ReadSectors:
out $imm2,$zero,$imm1, 16, 0x100                                 #set the current disk buffer to the first sector copy address at the data memory
out $imm2,$zero,$imm1, 15, 0                                     #set disksector to be the first one
out $imm2,$zero,$imm1, 14, 1                                     # read the first sector
out $imm2,$zero,$imm1, 16, 0x200                                 #set the current disk buffer to the second sector copy address at the data memory
out $imm2,$zero,$imm1, 15, 1                                     #set disksector to be the second one
out $imm2,$zero,$imm1, 14, 1                                     # read the second sector
Check:
lw $s0, $t1, $t0, 0, 0                                           # load the first sector data at position 0x100+$t0
lw $s2, $t2, $t0, 0, 0                                           # load the second sector data at position 0x100+$t0
bne $imm2, $s0, $t1, 0, LightRight                               # if the elements are diffirent then light the left leds and finish the job.
add $t0, $t0, $imm1, 1, 0 # $t0++
blt $imm2, $t0, $imm1, 128, Check                                #if we didn't reach the last secor element jump back to Check
beq $imm2, $zero, $zero, 0, LightLeft                           # if we the two sectors are the same light the right leds and finish the job.
LightRight:
out $imm2, $imm1, $zero, 9, 0x0000FFFF
beq $imm2, $zero, $zero, 0, EXIT
LightLeft:
out $imm2, $imm1, $zero, 9,0xFFFF0000
beq $imm2, $zero, $zero, 0, EXIT
EXIT:
lw $s0, $sp, $imm1, 0, 0                                         #restore $s0 value 
lw $s1, $sp, $imm1, 0, 0                                         #restore $s1 value
add $sp, $sp, $imm1, 2, 0                                        #pop 2 items from the stack
halt $zero, $zero, $zero, 0, 0                                   #exit the simulation.