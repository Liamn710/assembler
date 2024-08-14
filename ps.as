
mov r1, r2  
add r3, r4  
macro zero_reg
        clr r1
        clr r2
        clr r3
endmacr
zero_reg
macro increment_reg
        inc r1
        inc r2
endmacr
increment_reg
        sub r1, r3  
        jmp end     

nop         
