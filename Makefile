TARGET    := lisp.out
SRC_DIR   := src
BUILD_DIR := build
SRCS      := $(shell find $(SRC_DIR) -name *.cpp)
OBJS      := $(SRCS:$(SRC_DIR)%.cpp=$(BUILD_DIR)%.o)
DEPS      := $(OBJS:%.o=%.dpp)

CXX       := g++-9
CXXFLAGS  := --std=c++11 -O2 -Wall -MMD -MP
LIBS      := -ledit

MAKEDIR_P     := mkdir -p


$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(LIBS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MAKEDIR_P) $(BUILD_DIR) && $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -MF $(BUILD_DIR)/$*.dpp

clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: clean
-include $(DEPS)
