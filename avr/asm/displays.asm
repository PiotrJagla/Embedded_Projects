
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def		temp = r16
.def		overflows = r17
.def		numBuf = r18
.def		input = r19
.def		buttonLock = r20


.org 0x0000
rjmp Reset

.org 0x0020
rjmp overflow_handler

Reset:
				ldi temp, 0b01111110 
				MOV r0, temp
				ldi temp, 0b00001100 
				MOV r1, temp
				ldi temp, 0b10110110 
				MOV r2, temp
				ldi temp, 0b10011110 
				MOV r3, temp
				ldi temp, 0b11001100 
				MOV r4, temp
				ldi temp, 0b11011010 
				MOV r5, temp
				ldi temp, 0b11111010 
				MOV r6, temp
				ldi temp, 0b00001110 
				MOV r7, temp
				ldi temp, 0b11111110 
				MOV r8, temp
				ldi temp, 0b11011110 
				MOV r9, temp

				ldi numBuf, 0b000000000

				ldi temp, 0b00000101
				out TCCR0B, temp

				ldi temp, 0b00000001
				sts TIMSK0, temp

				sei

				clr temp
				out TCNT0, temp

				ldi temp, 0b11111110
				out DDRD, temp 
				ldi temp, 0b00100000
				out DDRB, temp

				ldi temp, 0b00000011
				out PortB, temp
				ldi temp, 0b00000000
				out PortD, temp

				ldi numBuf, 0b00000001
				ldi buttonLock, 0b00000011


Main:
				in input, PinB
				sbrs input, 0
				rcall Increment

				sbrs input, 1
				rcall Decrement



				ldi temp, 0b00000001
				AND temp, input
				cpi temp, 1
				brne PC+3
				ldi temp, 0b00000001
				OR buttonLock, temp

				ldi temp, 0b00000010
				AND temp, input
				cpi temp, 2
				brne PC + 3
				ldi temp, 0b00000010
				OR buttonLock, temp

				rcall Light

				rcall Delay
				rcall Main

Increment:
				ldi temp, 0b00000001
				AND temp, buttonLock
				cpi temp, 1
				brne PC+4
				inc numBuf
				ldi temp, 0b11111110
				AND buttonLock, temp

				ret

Decrement:
				ldi temp, 0b00000010
				AND temp, buttonLock
				cpi temp, 2
				brne PC+4
				dec numBuf
				ldi temp, 0b11111101
				AND buttonLock, temp

				ret

Light:
				cpi numBuf, 10
				brne PC+2
				ldi numBuf, 0b00001001

				cpi numBuf, 0
				brne PC+2
				out PortD, r0

				cpi numBuf, 1
				brne PC+2
				out PortD, r1

				cpi numBuf, 2
				brne PC+2
				out PortD, r2

				cpi numBuf, 3
				brne PC+2
				out PortD, r3

				cpi numBuf, 4
				brne PC+2
				out PortD, r4

				cpi numBuf, 5
				brne PC+2
				out PortD, r5

				cpi numBuf, 6
				brne PC+2
				out PortD, r6

				cpi numBuf, 7
				brne PC+2
				out PortD, r7

				cpi numBuf, 8
				brne PC+2
				out PortD, r8

				cpi numBuf, 9
				brne PC+2
				out PortD, r9

				ret

Delay:
				clr overflows
				sec_count:
								cpi overflows, 30
				brne sec_count
				ret

overflow_handler:
				inc overflows
				cpi overflows, 61
				brne PC+2
				clr overflows
				reti
