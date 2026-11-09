CODEDIRS := intro/code filesystem/code processes/code signals/code events/code threads/code synchro/code scheduling/code pipe/code mqueue/code

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
