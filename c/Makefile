all: saxpy saxpy_input_sender saxpy_input_receiver

saxpy: saxpy.c
	gcc saxpy.c -o saxpy -lm

saxpy_input_sender: saxpy_input_sender.c
	gcc saxpy_input_sender.c -o saxpy_input_sender -lm

saxpy_input_receiver: saxpy_input_receiver.c
	gcc saxpy_input_receiver.c -o saxpy_input_receiver -lm
