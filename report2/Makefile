#Makefile

#Inside Macro
#	$@:ターゲット名
#	$<:依存ファイルの先頭のファイル名
#	$^:依存ファイルのリスト
#Ver.0.1 (2016/12/13)

CC := gcc
CFLAGS := -Wall -g -lm -o 
#optimeze option ...実行時にO="-O0", "-O1", "-O2"などを指定
O = -O2

all:

%:
	$(CC) $(CFLAGS) $* $*.c $(O)
%.gc:
	$(CC) $(CFLAGS) $* $*.c $(O)
	

#	gcc -Wall -g -o kadai01 kadai01.c -lm
.PHONY: %.c
%.c: OUTPUT = $*
%.c: 
	$(CC) $(CFLAGS) $(OUTPUT) $< $@ $(O)
	
	
%.gc:
	$(CC) $(CFLAGS) $* $*.c $(O)
	

	

	
%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $@ $<