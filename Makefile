

all: tokyoping

tokyoping: alg.o flow_manager.o network_service.o packet.o
	g++ alg.o flow_manager.o network_service.o packet.o -o tokyoping

intermed:
	g++ alg.cpp flow_manager.cpp network_service.cpp packet.cpp -o tokyoping -g

alg.o: alg.cpp
	g++ -c alg.cpp

flow_manager.o: flow_manager.cpp
	g++ -c flow_manager.cpp

network_service.o: network_service.cpp
	g++ -c network_service.cpp

packet.o: packet.cpp
	g++ -c packet.cpp

clean:
	rm -rf *o tokyoping