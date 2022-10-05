.PHONY: all install

all:
	hugo  --cleanDestinationDir
	rm public/index.html
	cp static/index.html public/index.html

install: all
	rsync -ac --delete public/ sop@ssh.mini.pw.edu.pl:public_html/
	scp htpasswd/htpasswd sop@ssh.mini.pw.edu.pl:

