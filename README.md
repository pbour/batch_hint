# HINT on Steroids: Batch Query Processing for Interval Data


## Dependencies
- g++/gcc
- Boost Library 


## Essential directories

### Inputs
Download and extract the datasets from irectory  ```inputs``` includes the datasets used in the experiments and ```queries``` the query batches. 
- BOOKS.inp
- AARHUS-BOOKS_2013_20k.qry


## Compile
Compile using ```make all``` or simply ```make```.


## Workloads
The experiments run code supports two types of workload:
- Counting the qualifying records, or
- XOR'ing between their ids

By default the XOR workload is selected. You can switch to counting by commenting out Line 52 for the ``WORKLOAD_XOR`` flag and uncommenting Line 51 for the `WORKLOAD_COUNT` flag in def_global.h; remember to use `make clean` after resetting the flag.
The experiments in the paper ran using the XOR workload.


## Usage

### To execute a batch strategy use the ``query_batch.exec`` executable:

### Parameters
| Parameter | Description | Comment |
| ------ | ------ | ------ |
| -m | set the number of bits for HINT | Real datasets: 10 for BOOKS, 12 for WEBKIT, 17 for TAXIS and GREEND |
| -b | select the batch evaluation strategy | "query" or "level" or "partition"; by default "query" |
| -s | sort queries by their start | mandatory for the level-based and the partition-based strategies |
| -r | set number of runs per query | by default 1

- #### Examples

    ```sh
    $ ./query_batch.exec -m 10 -b query -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```
    ```sh
    $ ./query_batch.exec -m 10 -b query -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```
