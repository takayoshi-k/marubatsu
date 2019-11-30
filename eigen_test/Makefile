TARGET = ttt

# CSRC = test.cxx
CSRC = NNUtil.cxx NNLayers.cxx NeuralNetwork.cxx main.cxx
CFLAGS = -I.. -lm

all: $(TARGET)

$(TARGET): $(CSRC)
	g++ -g -O0 -o $@ $(CFLAGS) $(CSRC)

clean:
	rm -f $(TARGET)

