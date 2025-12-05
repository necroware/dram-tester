# Project settings
BOARD = arduino:avr:nano
PORT  = /dev/ttyUSB0

# Default target
all: compile

compile:
	arduino-cli compile --fqbn $(BOARD)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(BOARD)

clean:
	rm -rf build
