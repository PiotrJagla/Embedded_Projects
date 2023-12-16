.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def overflows=r18
.def buttonIn = r17
.def temp = r16
.def hexNum = r19

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


  ldi temp, 0b00100011
  out DDRB, temp

  ldi temp, 0b00000100
  out PortB, temp

  ldi temp, 0b11111110
  out DDRD, temp

  ldi r20, 0b00000100
  out TCCR0B, r20

  ldi temp, 0b00000001
  sts TIMSK0, temp

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

  rcall Delay

  in buttonIn, PINB 
  sbrc buttonIn, PB2 
  rcall ButtonPressed 

  rcall ButtonNotPressed 

  rjmp Start


ButtonPressed:
  sbi PortB, 5
  rjmp Start


ButtonNotPressed:
  cbi PortB, 5

Increment:
  inc hexNum
  
Light:
  cpi hexNum, 10
  brne PC+2
  clr hexNum

  cpi hexNum, 0
  brne PC+2
  out PortD, r0

  cpi hexNum, 1
  brne PC+2
  out PortD, r1

  cpi hexNum, 2
  brne PC+2
  out PortD, r2

  cpi hexNum, 3
  brne PC+2
  out PortD, r3

  cpi hexNum, 4
  brne PC+2
  out PortD, r4

  cpi hexNum, 5
  brne PC+2
  out PortD, r5

  cpi hexNum, 6
  brne PC+2
  out PortD, r6

  cpi hexNum, 7
  brne PC+2
  out PortD, r7

  cpi hexNum, 8
  brne PC+2
  out PortD, r8

  cpi hexNum, 9
  brne PC+2
  out PortD, r9
  
Delay:
  clr overflows
  sec_count:
    cpi overflows,10
  brne sec_count
  ret
  

overflow_handler:
  inc overflows
  cpi overflows, 61
  brne PC+2
  clr overflows
  reti

