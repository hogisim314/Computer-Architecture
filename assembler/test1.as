        lw 0 1 one		load 1 to reg1 (symbolic)
	lw 0 2 ten		load 10 to reg2 (symbolic)
start	add 1 2 3		reg3 = reg1 + reg2
	add 0 2 4	
	beq 2 4 0
done halt
ten .fill 10 
one .fill 1
