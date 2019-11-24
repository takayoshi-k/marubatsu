TARGET = marubatsu

CSRC = marubatsu_main.cxx

all: $(TARGET)

$(TARGET): $(CSRC)
	g++ -o $@ $<

clean:
	rm -f $(TARGET)

