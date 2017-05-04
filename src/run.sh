cp -rf /tmp/hello_world/host/hello_world /bin/hello_world
cp -rf /tmp/hello_world/ta/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta /lib/optee_armtz/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta

cp -rf /tmp/myPrograms/output/reseed/reseed /bin/reseed

cp -rf /tmp/myPrograms/output/ta/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta /lib/optee_armtz/8aaaf200-2450-11e4-abe2-0002a5d5c51b.ta
cp -rf /tmp/myPrograms/output/libinvctrl/libinvctrl.so /lib

cp -rf /tmp/myPrograms/output/modbus/lib/libmodbus** /lib/

cp -rf /tmp/myPrograms/output/ModbusServerClient/server /bin/
cp -rf /tmp/myPrograms/output/ModbusServerClient/client /bin/

cd /tmp/myPrograms/output/ModbusServerClient

# server > serverOutput 2>/dev/null &
# client > clientOutput 2>/dev/null

# server 1 > serverOutput 2>/dev/null &
# client 1 > clientOutput 2>/dev/null

# server 2 > serverOutput 2>/dev/null &
# client 2 > clientOutput 2>/dev/null

# reseed

# server > /tmp/myPrograms/clientServerOutputs/serverOutput 2>/dev/null &
# client > /tmp/myPrograms/clientServerOutputs/clientOutput 2>/dev/null


# reseed

# server 1 > /tmp/myPrograms/clientServerOutputs/serverOutput1 2>/dev/null &
# client 1 > /tmp/myPrograms/clientServerOutputs/clientOutput1 2>/dev/null


# reseed

# server 2 > /tmp/myPrograms/clientServerOutputs/serverOutput2 2>/dev/null &
# client 2 > /tmp/myPrograms/clientServerOutputs/clientOutput2 2>/dev/null


mkdir -p /tmp/myPrograms/clientServerOutputs/

reseed

server 3 > /tmp/myPrograms/clientServerOutputs/serverOutput3 2>/dev/null &
client 3 > /tmp/myPrograms/clientServerOutputs/clientOutput3 2>/dev/null

echo
echo
echo "****************** Server Output ******************"
cat /tmp/myPrograms/clientServerOutputs/serverOutput3
echo
echo
echo "****************** Client Output ******************"
cat /tmp/myPrograms/clientServerOutputs/clientOutput3

# reseed