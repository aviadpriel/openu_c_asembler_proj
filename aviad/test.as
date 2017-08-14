MAIN:   mov M1[r2][r7],LENGTH
        ;;;;aviad priel
LOOP:   jmp END
        prn #-5
        jmp r1
        sub r1,r4
        inc K
        mov M1[r3][r3],r3
STR:    .string "ab  cdef"
K:      .data   22,1,2,3,43,99
M1:     .mat  [2][2]  