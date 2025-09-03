# generate documentation
doc:
	pandoc ./README.md -so ./doc/htag.html -d ./doc/html.yaml --embed-resources
	pandoc ./README.md -so ./doc/htag.1 -d ./doc/man.yaml -t man

# build the program
build:
	[ ! -d ./bin/ ] && mkdir ./bin/ || true
	gcc ./src/htag.c -o ./bin/htag
