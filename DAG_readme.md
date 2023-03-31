**Important:**  **Details of the specific package versions that work with Sawtooth 1.2.6**

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

**Important:** **Chainging sawtooth serial to parallel can be done by:**

Line 106 in sawtooth-DAG/docker-compose.yaml file

For Parallel scedular:
    sawtooth-validator --scheduler parallel -vv 

For serial scedular:
    sawtooth-validator --scheduler serial -vv 

**Important:** **Chainging sawtooth tree scheduler to DAG scheduer:**

location: Sawtooth-DAG/validator/sawtooth_validator/execution

scheduler_parallel_1 : DAG scheduler

scheduler_parallel_2: tree scheduler

**Rename the desired scheduler as scheduler_parallel

**Important:** **Using Simplewallet transaction family:**

setting up transaction processor:
```bash
$   docker exec -it sawtooth-shell-local bash
$   ./pyprocessor/simplewallet-tp
```


setting up client:
```bash
$   docker exec -it sawtooth-shell-local bash
$   sawtooth keygen client1
$   ./pyclient/simplewallet
```
Note: sawtooth key generation has to be done for all clients
The transactions list should be given in "batches.txt" file in pyclient folder.
**Important:** **Running Insurance Transaction Family**

**To start the insurance transaction family processor:**

a. Open sawtooth local command bash.

b. navigate to "Ins_processor/insuance-handler" folder using command "cd Ins_processor/insuance-handler"

c. Run "apt install nodejs" to install nodejs in the docker container.

d. Run "node Processor.js" commnad to start the insurance transaction family processor.

e. Check the log of sawtooth validator to see if the transaction processor is registered with the validator.
	
	
**To submit transaction to the validator:**

a. The batches.txt file in Ins_client is the input for transactions.

b. Enter each transaction command in a new line in the above file.

c. open sawtooth local command bash and navigate to Ins_client folder.

**Important:** **Using Voting transaction family:**

setting up transaction processor:
```bash
$   docker exec -it sawtooth-shell-local bash
$   python3 ./voting_tp/tp.py
```


setting up client:
```bash
$   docker exec -it sawtooth-shell-local bash
$   python ./voting_client/vote.py
```
The transactions list should be given in "batches.txt" file in voting_client folder.

**Important:** **Using Intkey transaction family:**

setting up transaction processor:
```bash
$   docker exec -it sawtooth-shell-local bash
$   python3 ./intkey_processor/main.py
```


setting up client:
```bash
$   docker exec -it sawtooth-shell-local bash
$   python ./intkey_client/intkey_cli.py
```	
The transactions list should be given in "batches.txt" file in intkey_client folder.

**Important:** The time taken for execution of each block are recorded in the location "Enhanced-sawtooth/validator/DAG/execution-time.txt"
