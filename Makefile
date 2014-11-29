

all: tokyoping

tokyoping: alg.o flow_manager.o network_service.o packet.o
	g++ -std=c++11 alg.o flow_manager.o network_service.o packet.o -o tokyoping

intermed:
	g++ -std=c++11 alg.cpp flow_manager.cpp network_service.cpp packet.cpp -o tokyoping -g

alg.o: alg.cpp
	g++ -std=c++11 -c alg.cpp

flow_manager.o: flow_manager.cpp
	g++ -std=c++11 -c flow_manager.cpp

network_service.o: network_service.cpp
	g++ -std=c++11 -c network_service.cpp

packet.o: packet.cpp
	g++ -std=c++11 -c packet.cpp

clean:
	rm -rf *o tokyoping
