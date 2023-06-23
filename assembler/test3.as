        lw      0       1       five    load reg1 with 5 (symbolic address)
        lw      1       2       4       load reg2 with -1 (numeric address)
        lw      0       4       mouse   load reg4 with 8
start   add     2       4       5       decrement reg1
        jalr    4       3
        add    1       2       3
        noop
done    halt                            end of program
mouse   .fill   7
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start                   will contain the address of start (2)
