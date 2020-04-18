# kill receiver which migh already run
pkill receiver
sleep 0.2

# start receiver, first the last in the chain
./build/receiver/receiver 20000 &
sleep 0.2

# start the other receivers, chained together
./build/receiver/receiver 20001 20000 &
sleep 0.2
./build/receiver/receiver 20002 20001 &
sleep 0.2
./build/receiver/receiver 20003 20002 &
sleep 0.2
./build/receiver/receiver 20004 20003 &
sleep 0.2
./build/receiver/receiver 20005 20004 &
sleep 0.2
./build/receiver/receiver 20006 20005 &

# start sender, send to last receiver
#./build/sender/sender 20006

