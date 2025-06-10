# template

.globl _start

.data
	# variables here
.text

_start:
    li t0, 10
    li t1, 20

    add t0, t0, t1

exit:
    addi x17, x0, 10  # call number 10 = exit
    ecall
