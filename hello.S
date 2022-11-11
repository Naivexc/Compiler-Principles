  .text
  .globl add
add:
  addi sp, sp, -32
//store
  sw a0, 0(sp)
//store
  sw a1, 4(sp)
//load
  lw t0, 4(sp)
  sw t0, 8(sp)
//load
  lw t0, 0(sp)
  sw t0, 12(sp)
//binary
  lw t0, 12(sp)
  lw t1, 8(sp)
  add t0, t0, t1
  sw t0, 16(sp)
//ret binary
  lw a0, 16(sp)
  addi sp, sp, 32
  ret

  .text
  .globl sub
sub:
  addi sp, sp, -32
//store
  sw a0, 0(sp)
//store
  sw a1, 4(sp)
//load
  lw t0, 4(sp)
  sw t0, 8(sp)
//load
  lw t0, 0(sp)
  sw t0, 12(sp)
//binary
  lw t0, 12(sp)
  lw t1, 8(sp)
  sub t0, t0, t1
  sw t0, 16(sp)
//ret binary
  lw a0, 16(sp)
  addi sp, sp, 32
  ret

  .text
  .globl mul
mul:
  addi sp, sp, -32
//store
  sw a0, 0(sp)
//store
  sw a1, 4(sp)
//load
  lw t0, 4(sp)
  sw t0, 8(sp)
//load
  lw t0, 0(sp)
  sw t0, 12(sp)
//binary
  lw t0, 12(sp)
  lw t1, 8(sp)
  mul t0, t0, t1
  sw t0, 16(sp)
//ret binary
  lw a0, 16(sp)
  addi sp, sp, 32
  ret

  .text
  .globl div
div:
  addi sp, sp, -32
//store
  sw a0, 0(sp)
//store
  sw a1, 4(sp)
//load
  lw t0, 4(sp)
  sw t0, 8(sp)
//load
  lw t0, 0(sp)
  sw t0, 12(sp)
//binary
  lw t0, 12(sp)
  lw t1, 8(sp)
  div t0, t0, t1
  sw t0, 16(sp)
//ret binary
  lw a0, 16(sp)
  addi sp, sp, 32
  ret

  .text
  .globl main
main:
  addi sp, sp, -144
  sw ra, 140(sp)
//call
  li a0, 1
  li a1, 2
  call sub
  sw a0, -24(sp)
//call
  li a0, 4
  li a1, 5
  call div
  sw a0, -20(sp)
//call
  li a0, 3
  lw a1, -20(sp)
  call mul
  sw a0, -16(sp)
//call
  lw a0, -24(sp)
  lw a1, -16(sp)
  call add
  sw a0, -12(sp)
//store
  lw t0, -12(sp)
  sw t0, -8(sp)
//branch
  li t0, 1
  la t5, then_0
  beqz t0, label_j_0
  jalr x0, 0(t5)
label_j_0:
  la t5, else_1
  jalr x0, 0(t5)
then_0:
//store
  li t0, 1
  sw t0, -4(sp)
//jump
  la t5, end_2
  jalr x0, 0(t5)
else_1:
//binary
  li t0, 1
  li t1, 0
  or t0, t0, t1
  snez t0, t0
  sw t0, 0(sp)
//binary
  lw t0, 0(sp)
  li t1, 0
  xor t0, t0, t1
  snez t0, t0
  sw t0, 4(sp)
//store
  lw t0, 4(sp)
  sw t0, -4(sp)
//jump
  la t5, end_2
  jalr x0, 0(t5)
end_2:
//load
  lw t0, -4(sp)
  sw t0, 8(sp)
//branch
  li t0, 0
  la t5, then_3
  beqz t0, label_j_1
  jalr x0, 0(t5)
label_j_1:
  la t5, else_4
  jalr x0, 0(t5)
then_3:
//load
  lw t0, -8(sp)
  sw t0, 20(sp)
//call
  li a0, 1
  lw a1, 20(sp)
  call sub
  sw a0, 24(sp)
//binary
  li t0, 0
  li t1, 0
  xor t0, t0, t1
  snez t0, t0
  sw t0, 28(sp)
//binary
  lw t0, 24(sp)
  li t1, 0
  xor t0, t0, t1
  snez t0, t0
  sw t0, 32(sp)
//binary
  lw t0, 32(sp)
  lw t1, 28(sp)
  snez t0, t0
  snez t1, t1
  and t0, t0, t1
  sw t0, 36(sp)
//store
  lw t0, 36(sp)
  sw t0, 16(sp)
//jump
  la t5, end_5
  jalr x0, 0(t5)
else_4:
//store
  li t0, 0
  sw t0, 16(sp)
//jump
  la t5, end_5
  jalr x0, 0(t5)
end_5:
//load
  lw t0, 16(sp)
  sw t0, 40(sp)
//branch
  lw t0, 40(sp)
  la t5, then_6
  beqz t0, label_j_2
  jalr x0, 0(t5)
label_j_2:
  la t5, else_7
  jalr x0, 0(t5)
then_6:
//store
  li t0, 1
  sw t0, 12(sp)
//jump
  la t5, end_8
  jalr x0, 0(t5)
else_7:
//load
  lw t0, -8(sp)
  sw t0, 48(sp)
//branch
  lw t0, 48(sp)
  la t5, then_9
  beqz t0, label_j_3
  jalr x0, 0(t5)
label_j_3:
  la t5, else_10
  jalr x0, 0(t5)
end_8:
//load
  lw t0, 12(sp)
  sw t0, 52(sp)
//call
  lw a0, 8(sp)
  lw a1, 52(sp)
  call add
  sw a0, 56(sp)
//store
  lw t0, 56(sp)
  sw t0, 60(sp)
//load
  lw t0, 60(sp)
  sw t0, 64(sp)
//load
  lw t0, -8(sp)
  sw t0, 68(sp)
//binary
  lw t0, 68(sp)
  lw t1, 64(sp)
  add t0, t0, t1
  sw t0, 72(sp)
//ret binary
  lw a0, 72(sp)
  lw ra, 140(sp)
  addi sp, sp, 144
  ret
then_9:
//store
  li t0, 1
  sw t0, 44(sp)
//jump
  la t5, end_11
  jalr x0, 0(t5)
else_10:
//call
  li a0, 1
  li a1, 2
  call add
  sw a0, 76(sp)
//binary
  lw t0, 76(sp)
  li t1, 10
  slt t0, t1, t0
  sw t0, 80(sp)
//binary
  lw t0, 48(sp)
  lw t1, 80(sp)
  or t0, t0, t1
  snez t0, t0
  sw t0, 84(sp)
//binary
  lw t0, 84(sp)
  li t1, 0
  xor t0, t0, t1
  snez t0, t0
  sw t0, 88(sp)
//store
  lw t0, 88(sp)
  sw t0, 44(sp)
//jump
  la t5, end_11
  jalr x0, 0(t5)
end_11:
//load
  lw t0, 44(sp)
  sw t0, 92(sp)
//call
  lw a0, 92(sp)
  li a1, 5
  call div
  sw a0, 96(sp)
//call
  li a0, 3
  lw a1, 96(sp)
  call mul
  sw a0, 100(sp)
//binary
  lw t0, 40(sp)
  lw t1, 100(sp)
  or t0, t0, t1
  snez t0, t0
  sw t0, 104(sp)
//binary
  lw t0, 104(sp)
  li t1, 0
  xor t0, t0, t1
  snez t0, t0
  sw t0, 108(sp)
//store
  lw t0, 108(sp)
  sw t0, 12(sp)
//jump
  la t5, end_8
  jalr x0, 0(t5)

