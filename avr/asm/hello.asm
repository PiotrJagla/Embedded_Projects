.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def overflows=r18

.org 0x0000
jmp Reset
.org 0x0020
jmp overflow_handler

Reset:
  ldi r20, 0b01111110 
  MOV r0, r20
  ldi r20, 0b00001100 
  MOV r1, r20
  ldi r20, 0b10110110 
  MOV r2, r20
  ldi r20, 0b10011110 
  MOV r3, r20
  ldi r20, 0b11001100 
  MOV r4, r20
  ldi r20, 0b11011010 
  MOV r5, r20
  ldi r20, 0b11111010 
  MOV r6, r20
  ldi r20, 0b00001110 
  MOV r7, r20
  ldi r20, 0b11111110 
  MOV r8, r20
  ldi r20, 0b11011110 
  MOV r9, r20


  ldi r16, 0b00100011
  out DDRB, r16

  ldi r16, 0b11111110
  out DDRD, r16

  out PortD, r0

  ldi r30, 0b00000000
  ldi r31, 0b00000000

  ldi r20, 0b00000100
  out TCCR0B, r20

  ldi r16, 0b00000001
  sts TIMSK0, r16

  sei

  clr r20
  out TCNT0, r20
  MOV r25, r5

  rjmp Start



Start:
  sbi PortB, 1
  cbi PortB, 0
  out PortD, r25
  rcall Delay

  sbi PortB, 0
  cbi PortB, 1
  rcall Light
  rcall Delay

  rjmp Start


Increment:
  inc r30
  
Light:
  cpi r30, 10
  brne PC+2
  clr r30

  cpi r30, 0
  brne PC+2
  out PortD, r0

  cpi r30, 1
  brne PC+2
  out PortD, r1

  cpi r30, 2
  brne PC+2
  out PortD, r2

  cpi r30, 3
  brne PC+2
  out PortD, r3

  cpi r30, 4
  brne PC+2
  out PortD, r4

  cpi r30, 5
  brne PC+2
  out PortD, r5

  cpi r30, 6
  brne PC+2
  out PortD, r6

  cpi r30, 7
  brne PC+2
  out PortD, r7

  cpi r30, 8
  brne PC+2
  out PortD, r8

  cpi r30, 9
  brne PC+2
  out PortD, r9
  
Delay:
  clr overflows
  sec_count:
    cpi overflows,1
  brne sec_count
  ret
  

overflow_handler:
  inc overflows
  cpi overflows, 61
  brne PC+2
  clr overflows
  reti

