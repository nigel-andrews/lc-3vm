; Simple I/O
.ORIG x3000
READ_LOOP
    TRAP x20
    ADD R1, R0, #0
    TRAP x21

    ADD R2, R1, #-10
    BRz END

    BRnzp READ_LOOP

END
    TRAP x25
.END
