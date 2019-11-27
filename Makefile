TARGET = marubatsu

CSRC = marubatsu_main.cxx
CFLAGS = -I.

all: $(TARGET)

$(TARGET): $(CSRC)
	g++ -o $@ $(CFLAGS) $<

clean:
	rm -f $(TARGET)

