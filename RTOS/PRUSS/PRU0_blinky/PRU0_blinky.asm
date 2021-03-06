
; PRUSS program to flash two LEDs connected to enhanced GPIO pins alternately
; at a rate determined by a value stored in the shared memory location, at offset 0.
;
; This program runs on PRU0.
;
; The PRUSS program halts if it detects that the next word in the shared memory location,
; at offset +4, is not equal to zero. This halt condition is generated by the other PRU,
; when the user presses both buttons simultaneously.
;
; author: Takis Zourntos (takis.zourntos@emads.org)

        .cdecls "main.c"
        .clink
        .global BEGIN
        
        .asg    0x00010000,    SM_ba
        .asg    0x0001000c,    HALT_ba
        .asg    0xffffffff,    HALT_val
        
BEGIN:
        LDI32   r0, SM_ba           ; place shared memory address in register r0, where delay is
        LDI32   r3, HALT_ba         ; place shared memory address in register r3, where halt state is stored

LEDOFF:
        CLR     r30, r30.t7         ; clear (turn on) the red LED
        SET     r30, r30.t5         ; set (turn off) the blue LED
        LBBO    &r1, r0, 0, 4       ; load r1 with 32-bit word stored at SM_ba, offset 0

DELAYOFF:
        SUB     r1, r1, 1           ; count down the delay
        QBNE    DELAYOFF, r1, 0     ; do the 2-instruction delay loop thing until r1=0

LEDON:
        SET     r30, r30.t7         ; set red LED
        CLR     r30, r30.t5         ; clear blue LED
        LBBO    &r1, r0, 0, 4       ; load r1 with the potentially updated delay count

DELAYON:
        SUB     r1, r1, 1           ; count down the delay     
        QBNE    DELAYON, r1, 0      ; do the 2-instruction delay loop thing until r1=0

CHKHALT:   
        LBBO    &r2, r3, 0, 4       ; load r2 with the 32-bit word stored at SM_ba, offset 0x10
        QBEQ    LEDOFF, r2, 0       ; if the status=0, we can keep running

END:
        HALT                        ; if we get here, two buttons were pressed simultaneously by user
