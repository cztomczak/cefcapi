OUT = ./Release

SELF_ED = extract_deps.mk
CEF_EXTRACTED = cef_extracted
WHICH_CEF = Debug
CEF_BINARY_EXTRACTED = $(CEF_EXTRACTED)/$(WHICH_CEF)/libcef.so

$(CEF_EXTRACTED): $(SELF_ED)
	mkdir -p $(CEF_EXTRACTED)

$(CEF_BINARY_EXTRACTED): third_party/*.tar.bz2 $(CEF_EXTRACTED) $(SELF_ED)
	rm -rf $(CEF_EXTRACTED)/*
	for archive in third_party/*.tar.bz2; do \
		tar -xf "$$archive" -C $(CEF_EXTRACTED) --strip-components=1; \
		break; \
	done
	if test -e $(CEF_BINARY_EXTRACTED); then touch $(CEF_BINARY_EXTRACTED); fi

$(OUT)/libcef.so: $(CEF_BINARY_EXTRACTED) $(SELF_ED)
	mkdir -p $(OUT)
	cp -r $(CEF_EXTRACTED)/$(WHICH_CEF)/* $(OUT)
	cp -r $(CEF_EXTRACTED)/Resources/* $(OUT)
