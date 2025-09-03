build:
	[ -d bin ] || mkdir bin
	gcc src/htag.c -o bin/htag

doc:
	[ -d doc ] || mkdir doc
	pandoc README.md -so doc/index.html -d readme -d pandoc.yml

clean:
	[ ! -d bin ] || rm -r bin
	[ ! -d doc ] || rm -r doc

publish: clean doc build
	[ -d bin ]
	[ -f bin/htag ]
	sudo cp bin/htag /usr/local/bin/htag
