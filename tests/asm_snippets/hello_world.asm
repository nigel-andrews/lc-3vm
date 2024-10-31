; Hello World
.ORIG x3000
    LEA R0, HELLO_MSG
    PUTS
    TRAP x25

HELLO_MSG .STRINGZ "Hello, World!"
.END
