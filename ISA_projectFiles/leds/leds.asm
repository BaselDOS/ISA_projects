add $t0, $zero, $imm, 6 #set $t0 to be 6 (irqhandle in IOregister)
out, $imm, $t0, $zero, Ledo # set irq handlet to be Ledo so when the irq2in is 1 we will go and light a led so we can make a 1 second delay by using irq2in
halt $zero, $zero, $zero,0 #halt
Ledo:
in $t1, $zero, $imm, 9 #get the leds status
srl $t1, $t1, $imm, 1 #shift right the leds status
or $t1, $t1, $imm, 0x80000000 #light up the first led 
out $t1, $zero, $imm, 9 # update the leds status
reti $zero, $zero, $zero,0 #return from interrupt

