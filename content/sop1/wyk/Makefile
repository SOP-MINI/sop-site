CODEDIRS := w1/code w2/code w4/code w5/code events/code w6/code w7/code scheduling/code pipe/code mqueue/code

.PHONY: all clean $(CODEDIRS)

all: $(CODEDIRS)

$(CODEDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(CODEDIRS); do \
		if [ -f "$$dir/Makefile" ]; then \
			$(MAKE) -C $$dir clean; \
		fi \
	done
