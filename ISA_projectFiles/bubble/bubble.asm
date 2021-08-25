.word 1024 15 # inisialize
.word 1025 1 # inisialize
.word 1026 33 # inisialize
.word 1027 8 # inisialize
.word 1028 49 # inisialize
.word 1029 2 # inisialize
.word 1030 11 # inisialize
.word 1031 114 # inisialize
.word 1032 14 # inisialize
.word 1033 18 # inisialize
.word 1034 15 # inisialize
.word 1035 19 # inisialize
.word 1036 20 # inisialize
.word 1037 82 # inisialize
.word 1038 86 # inisialize
.word 1039 87 # inisialize
add $t0, $zero, $zero, 0 #j=0
add $t1, $zero, $zero, 0 #i=0
For1:
add $t2, $zero, $imm, 15 # 
bgt $imm, $t1,$t2, EXIT #if i>=15 go to EXIT
add $t0, $t0, $imm, 1 #j=1
For2:
bgt $imm, $t1, $t2, For1 #if j>=15 go to For1
lw $t2, $t1, $imm 1024 #$t2=Dmem[j]
lw $t3, $t1, $imm, 1025 #$t3=Dmem[j+1]
add $t1, $t1, $imm, 1 #i++
blt $imm, $t2, %t3,For2 #if Dmem[j]<Dmem[j+1] go to For2
sw $t3, $t1, $imm, 1023 #Dmem[j+1] move to Dmem[j]
sw $t2, $t1, $imm, 1024 #Dmem[j]= past Dmem[j+1]
beq $imm, $zero, $zero, For2 #jump to For2
EXIT:
halt $zero, $zero, $zero, 0 #halt