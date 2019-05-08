startup: startup.o
	gcc -fpic -S startup.c -o startup.s -ldl
	rm startup.o
	# remove the last two lines
	sed -i '/.ident/d' startup.s
	sed -i '/.note.GNU-stack/d' startup.s
