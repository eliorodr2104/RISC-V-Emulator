# template

.globl _start

.data
	# variables here
	x: .word 10
	y: .word 12

.text

_start:
    addi t0, zero, 10
    addi t1, zero, 5
    addi t2, zero, 3


    add  t0, t0, t1
    sub  t0, t0, t2
    sll  t0, t1, t2
    sra  t0, t1, t2
    srl  t0, t1, t2

    addi t0, t0, 10
    slli t0, t0, 3
    srai t0, t0, 3
    srli t0, t0, 3

    addi t2, zero, -16
    and  t0, t0, t2
    or   t0, t0, t2
    xor  t0, t0, t2
    xori t0, t0, -1

    addi t1, zero, 80
    addi t2, zero, 12
    jal  ra, CODE

    addi t0, zero, 20
    addi t1, zero, 5

    ecall

CODE:
    add  t0, t0, t1

    addi t2, zero, 10
    add  t0, t0, t2

    ret

#exit:
#    addi x17, x0, 10  # call number 10 = exit
#    ecall
