; Testing Hello World but with OUT instead of PUTS
.ORIG x3000
    LEA R1, HELLO_MSG  ; Load the address of the first character of HELLO_MSG into R1

PRINT_LOOP
    LDR R0, R1, #0     ; Load the character at address R1 into R0
    BRz END            ; If it's a null character (end of string), branch to END
    TRAP x21           ; Output the character in R0
    ADD R1, R1, #1     ; Increment R1 to point to the next character
    BRnzp PRINT_LOOP   ; Loop back to print the next character

END
    TRAP x25           ; Halt the program

HELLO_MSG .STRINGZ "Hello, World!" ; The string to print
.END
