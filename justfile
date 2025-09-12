build:
	[ -d bin ] || mkdir bin
	gcc src/htag.c -o bin/htag

doc:
	[ -d doc ] || mkdir doc
	corre -i doc/source.md -o README.md
	pandoc README.md -so doc/index.html -d readme -d pandoc.yml

clean:
	[ ! -d bin ] || rm -r bin
	[ ! -d doc ] || rm -r doc

publish: clean doc build
	[ -d bin ]
	[ -f bin/htag ]
	sudo cp bin/htag /usr/local/bin/htag
