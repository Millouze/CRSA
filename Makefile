mong:
	gcc -Wall -static -o mong maintest.c rsa_keygen.c rsa_enc_dec_mong.c

slow:
	gcc -Wall -static -o slow maintest.c rsa_keygen.c rsa_enc_dec.c

clean:
	rm slow mong
