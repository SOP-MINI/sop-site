.PHONY: all hugo install codes clean-codes

all: hugo codes

hugo:
	hugo --cleanDestinationDir
	rm public/index.html
	cp static/index.html public/index.html

clean-hugo:
	rm -rf public/
	rm -rf resources/
	rm -rf .hugo_build.lock

install: hugo
	rsync -ac --delete public/ sop@ssh.mini.pw.edu.pl:public_html/

codes:
	$(MAKE) -C content/sop2/wyk all

clean-codes:
	$(MAKE) -C content/sop2/wyk clean

clean: clean-hugo clean-codes