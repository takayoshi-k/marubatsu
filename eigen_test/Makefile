TARGET = ttt

# CSRC = test.cxx
CSRC = nn2.cxx
CFLAGS = -I.. -lm

all: $(TARGET)

$(TARGET): $(CSRC)
	g++ -o $@ $(CFLAGS) $<

clean:
	rm -f $(TARGET)

