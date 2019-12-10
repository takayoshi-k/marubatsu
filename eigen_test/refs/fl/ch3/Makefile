all: new_nn

ttt: nn_org.c
	gcc -o ttt nn_org.c -lm

rrr: nn_xor.c
	gcc -o rrr nn_xor.c -lm

new_nn: new_nn.c
	gcc -o new_nn new_nn.c -lm

clean:
	rm -f ttt rrr new_nn
