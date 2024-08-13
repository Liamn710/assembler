
macro MY_MACRO
    mov r0, #5
    add r1, r2
endmacr

macro OTHER_MACRO
    sub r3, r4
    cmp r5, #10
endmacr

.data
    var1: .data 5
    var2: .data 10
    message: .string "Hello, World!"


.entry main


main: 
    mov r0, #0     ; Initialize register r0 to 0
    MY_MACRO       ; Use the macro defined earlier
    other_label:   ; A label for jumps
    prn r0         ; Print the value in register r0
    jmp other_label ; Jump to other_label
    stop           ; Stop the program

