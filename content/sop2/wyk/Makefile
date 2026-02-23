CODEDIRS := shm/code

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
