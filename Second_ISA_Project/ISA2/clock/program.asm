.word 0x100 0x075955
.word 0x101 0x075956
.word 0x102 0x075957
.word 0x103 0x075958
.word 0x104 0x075959
.word 0x105 0x080000
.word 0x106 0x080001
.word 0x107 0x080002
.word 0x108 0x080003
.word 0x109 0x080004
.word 0x10A 0x080005
Clock:
lw $t0, $zero, $imm1, 0x100, 0                                       #load the first clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the first clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x101, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x102, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x103, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x104, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x105, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x106, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x107, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x108, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x109, 0                                       #load the next clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the next clock time of display7seg by updating it's status.
lw $t0, $zero, $imm1, 0x10A, 0                                       #load the last clock time.
out $t0, $imm1, $imm2, 5,5                                           #display the last clock time of display7seg by updating it's status.
halt $zero, $zero, $zero,0,0