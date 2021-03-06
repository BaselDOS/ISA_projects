.word 0x100 33
.word 0x101 0
.word 0x102 0
.word 0x103 0
.word 0x104 0
.word 0x105 33
.word 0x106 0
.word 0x107 0
.word 0x108 0
.word 0x109 0
.word 0x10A 33
.word 0x10B 0
.word 0x10C 0
.word 0x10D 0
.word 0x10E 0
.word 0x10F 33
.word 0x110 33
.word 0x111 0
.word 0x112 0
.word 0x113 0
.word 0x114 0
.word 0x115 33
.word 0x116 0
.word 0x117 0
.word 0x118 0
.word 0x119 0
.word 0x11A 33
.word 0x11B 0
.word 0x11C 0
.word 0x11D 0
.word 0x11E 0
.word 0x11F 33
              add $sp, $zero, $imm, 1              # set $sp = 1
	sll $sp, $sp, $imm, 11               # set $sp = 1 << 11 = 2048
	jal $imm, $zero, $zero, mul          # call mul 
	halt $zero, $zero, $zero, 0	    # halt 
mul:
	add $sp, $sp, $imm, -3                 # adjust stack for 3 items 
	sw $s0, $sp, $imm, 2                   # save $s0
	sw $s1, $sp, $imm, 1                   # save $s1
	sw $s2, $sp, $imm, 0                   # save $s2
	add $t0, $zero, $imm, 4	       # $t0 = N = 4
	add $s0, $zero, $imm, -1               # $s0 = i = -1 
For1:
	add $s0, $s0, $imm, 1                  # i++ 
	beq $imm, $s0, $t0, end                # for(i=0; i<N; i++)
	add $s1, $zero, $imm, -1               # set $s1 as  j = -1 
For2:
	blt $imm, $s1, $zero, Else             # if the loop didnt started yet 
	add $t1, $s0, $imm, 0	    # $t1 = i
	mul $t1, $t1, $t0, 0                   # $t1 = i*N
	add $t1, $t1, $s1, 0                   # $t1 = i*N + j
	sw $t3, $t1, $imm, 0x120               # C[i][j] = $t3
Else:
	add $s1, $s1, $imm, 1                  # j++ 
	beq $imm, $s1, $t0, For1              # for(j=0; j<N; j++)
	add $s2, $zero, $imm, -1               # $s2 = k = -1
	add $t3, $zero, $zero, 0               # $t3 = 0
	add $t1, $zero, $zero, 0               # $t1 = 0 
For3:
	add $s2, $s2, $imm, 1                  # k++ 
	beq $imm, $s2, $t0, For2              # for(k=0; k<N; k++)
	add $t1, $s0, $imm, 0	    # $t1 = i
	mul $t1, $t1, $t0, 0                   # $t1 = i*N
	add $t1, $t1, $s2, 0                   # $t1 = i*N + k
	lw $t1, $t1, $imm, 0x100               # $t1 = A[i][k]
	add $t2, $s2, $imm, 0	 # $t2 = k
	mul $t2, $t2, $t0, 0                   # $t2 = k*N
	add $t2, $t2, $s1, 0                   # $t2 = k*N + j
	lw $t2, $t2, $imm, 0x110               # $t1 = B[k][j]
	mul $t1, $t1, $t2, 0                   # $t1 = A[i][k] * B[k][j]
	add $t3, $t3, $t1, 0                   # $t3 =$t3+ A[i][k] * B[k][j]
	add $t1, $zero, $zero, 0               # $t1 = 0
	add $t2, $zero, $zero, 0               # $t2 = 0
	beq $imm, $zero, $zero, For3          # jump to For3
end:
	lw $s2, $sp, $imm, 0                   # restore $s2
	lw $s1, $sp, $imm, 1                   # restore $s1
	lw $s0, $sp, $imm, 2                   # restore $s0
	add $sp, $sp, $imm, 3                  # pop 3 items from stack
	beq $ra, $zero, $zero, 0               # return 

