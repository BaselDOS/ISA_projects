.word 0x100 1
.word 0x101 8
.word 0x102 4
.word 0x103 2
.word 0x104 8
.word 0x105 1
.word 0x106 0
.word 0x107 11
.word 0x108 5
.word 0x109 9
.word 0x10A 1
.word 0x10B 2
.word 0x10C 9
.word 0x10D 1
.word 0x10E 2
.word 0x10F 0
.word 0x110 0
.word 0x110 3
.word 0x111 8
.word 0x112 10
.word 0x113 9
.word 0x114 5
.word 0x115 7
.word 0x116 8
.word 0x117 4
.word 0x118 3
.word 0x119 8
.word 0x11A 2
.word 0x11B 8
.word 0x11C 7
.word 0x11D 1
.word 0x11E 0
Summat:
add $t0, $zero, $zero, 0, 0                                   # set $t0=0
add $t1, $zero, $imm1, 0x100, 0                               # set $t1 to 0x100 to the first element of the first matrix
add $t2, $zero, $imm1, 0x120, 0                               # set $t1 to 0x120 to the second element of the first matrix
sll $sp, $imm1, $imm2, 1, 11                                  # set $sp to be 2048
add $sp, $sp, $imm1, -3, 0                                    # adjust the stack for 3 elements
sw $s0, $sp, $zero, 0, 0                                      # save the first value of $s0
sw $s1, $sp, $imm1, 1, 0                                      # save the first value of $s1
sw $v0, $sp, $imm1, 2, 0                                      # save the first value of $v0
Load:
lw $s0, $t1, $zero, 0, 0                                      # load the elements number $t1 at the first matrix
lw $s1, $t1, $imm1, 16, 0                                     # load the elements number $t1+16 at the second matrix
Sum:
add $v0, $s0, $s1, 0, 0                                       # $v0=$s0=$s1
Store:
sw $v0, $t2, $zero, 0, 0                                      # save the sum resault at the $t2 address
step:
add $t0, $t0, $imm1, 1, 0                                     # $t0++
add $t1, $t1, $imm1, 1, 0                                     # $t1++
add $t2, $t2, $imm1, 1, 0                                     # $t2++
Check:
blt $imm2, $t0, $imm1, 16, Load                               # if we didnt reach the last element of the matrices jump back to Load
lw $s0, $sp, $zero, 0, 0                                      # return the first $s0 value
lw $s1, $sp, $imm1, 1, 0                                      # return the first $s1 value
lw $v0, $sp, $imm1, 2, 0                                      # return the first $v0 value
add $sp, $sp, $imm1, 3, 0                                     # pop 3 items from the stack
halt $zero, $zero, $zero, 0, 0                                # exit the simulation







