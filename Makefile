SRC_DIR     = src
TEST_DIR    = test

.PHONY: all test lib clean

all: test

test: lib
	$(MAKE) -C $(TEST_DIR)

lib:
	$(MAKE) -C $(SRC_DIR)

clean:
	$(MAKE) -C $(TEST_DIR) clean
	$(MAKE) -C $(SRC_DIR) clean
