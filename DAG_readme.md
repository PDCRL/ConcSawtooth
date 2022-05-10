**Important:**  Details of the specific package versions that work with Sawtooth 1.2.6

Ubuntu :18.04
docker:  20.10.7
docker-compose:  1.29.2
python3:  3.6.9
G++: 7.5.0


Commands to compile DAG codes:
location: Sawtooth-DAG/validator/DAG

```bash
$ sudo g++ -c -fPIC DAG_module.cpp -pthread
$ sudo g++ -shared -Wl,-soname,libgeek.so  -o libgeek.so  DAG_module.o
```

**Important:** Chainging sawtooth serial to parallel can be done by:

Line 106 in sawtooth-DAG/docker-compose.yaml file

For Parallel scedular:
    sawtooth-validator --scheduler parallel -vv 

For serial scedular:
    sawtooth-validator --scheduler serial -vv 

**Important:** Chainging sawtooth treescheduler to DAG scheduer:

location: Sawtooth-DAG/validator/sawtooth_validator/execution

scheduler_parallel_1 : DAG scheduler

scheduler_parallel_2: tree scheduler

**Remane the desired scheduler as scheduler_parallel

**Important:** Using Simplewalle transaction family:

setting up transaction processor:
```bash
$   docker exec -it sawtooth-shell-local bash
$   ./pyprocessor/simplewallet-tp
```


setting up client:
```bash
$   docker exec -it sawtooth-shell-local bash
$   sawtooth keygen client1
$   ./puclient/simplewallet
```
Note: sawtooth key generation has to be done for all clients
The transactions list should be given in "batches.txt" file in pyclient folder.


