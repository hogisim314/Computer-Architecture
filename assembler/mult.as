        lw 0 2 mplier
        lw 0 3 mcand
        lw 0 4 maxbit
        lw 0 5 maskb
loop    nor 2 5 6     bit examination
        beq 0 6 adder means lsb 1
        beq 0 0 go    means lsb 0
adder   add 1 3 1     lsb 1 gets adder
go      lw 0 6 minus1
        add 4 6 4
        beq 0 4 done 
        add 3 3 3     mcand left shift
        add 5 5 5     maskb left shift
        lw 0 6 plus1
        add 5 6 5     maskb manipulation complete        
        beq 0 0 loop
done    halt            
mcand   .fill   32766
mplier  .fill   12328
maskb   .fill   -2      for mask bit ~ ~ 1111 1110 (using nor) and shift left + 1
plus1   .fill   1
minus1  .fill   -1
maxbit  .fill   15      for only 15bit calculation