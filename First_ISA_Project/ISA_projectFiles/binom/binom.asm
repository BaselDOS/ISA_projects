.word 0x100 5 #inisialize n
.word 0x101 10 # inisialize k
Binom:
add $sp, $sp, $imm, -4 # adjust stack for 3 items
sw s1, $sp, $imm, 3 #save $s1
sw a0, $sp, $imm, 2 #save $s0
sw a1, $sp, $imm, 1 #save $a0
sw ra, $sp, $imm, 0 #save $ra
bgt $imm, $a1, $zero,L1 # if k>0 go to L1
add $v0, $zero, $imm,1 #set $v0 to be 1
beq $imm, $a0, $a1, L2 #if k=n jump to L2
beq $imm, $a1, $zero,L2 # if K=0 jump to L2
L1:
sub $a0, $a0,1 #n=n-1
jal $imm, $zero, $zero, Binom # calculate Binom(n-1,k)
add $s0, $v0,$zero,0 ##s0=$v0
sub $a1,$a1,$imm, 1 #k=k-1
jal $imm, $zero, $zero, Binom #calculate Binom (n-1,k-1)
add $v0, $v0, $s0, 0 #$s0=Binom(n-1,k-1)+Binom(n-1,k)
lw s1, $sp, $imm, 3 #restore $s1
lw a0, $sp, $imm, 2 #restore $s0
lw a1, $sp, $imm, 1 #restore $a1
lw ra, $sp, $imm, 0 #restore $ra
L2:
add $sp, $sp,$imm, 4 #pop 4 items from stack
beq $ra, $zero, $zero #jump to $ra