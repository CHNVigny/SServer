objects = server.o webDeal.o webUrl.o recoContact.o jsoncpp.o
server: $(objects)
	g++ -o server $(objects) -lpthread
clean:
	rm server $(objects)
