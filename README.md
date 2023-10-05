# HINT on Steroids: Batch Query Processing for Interval Data


## Dependencies
- g++/gcc
- Boost Library 


## Essentials

### Inputs
Download and extract the archive from . A ```inputs``` directory will be automatically created which includes all datasets used in the experiments, both real and synthtic. 

### Queries
Directory ```queries``` includes all queries used in the experiments for both real and synthetic datasets.

### Outputs
Directory ```outputs``` includes the reports after executing the bash scripts that reproduce the experiments in the paper.


## Workloads
The experiments run code supports two types of workload:
- Counting the qualifying records, or
- XOR'ing between their ids

By default the XOR workload is selected. You can switch to counting by commenting out Line 52 for the ``WORKLOAD_XOR`` flag and uncommenting Line 51 for the `WORKLOAD_COUNT` flag in def_global.h; remember to use `make clean` after resetting the flag.
The experiments in the paper ran using the XOR workload.


## Usage

### To execute an evaluation strategy use the ```query_batch.exec``` executable:

### Parameters
| Parameter | Description | Comment |
| ------ | ------ | ------ |
| -m | set the number of bits for HINT | Real datasets: 10 for BOOKS, 12 for WEBKIT, 17 for TAXIS, GREEND and all synthetics|
| -b | select the batch evaluation strategy | "query" or "level" or "partition"; by default "query" |
| -s | sort queries by their start | mandatory for the level-based and the partition-based strategies |
| -r | set number of runs per query | by default 1 |

- #### Examples

    QUERY-based:
    ```sh 
    $ ./query_batch.exec -m 10 -b query -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```
    QUERY-based with sorting:
    ```sh 
    $ ./query_batch.exec -m 10 -b query -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```
    LEVEL-based with sorting:
    ```sh
    $ ./query_batch.exec -m 10 -b level -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```
    PARTITION-based with sorting
    ```sh 
    $ ./query_batch.exec -m 10 -b partition -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    ```

## Experiments

To reproduce all experiments in the paper use the following bash scripts:
- run_real_vary-qe.sh
- run_real_vary-qn.sh 

Reports will be writing in the ```outputs``` directory; one file for each experiment.

- ### Real datasets

    Vary query extend experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary batch size or number of queries experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qn.sh WEBKIT 12 query 10
    ```

- ### Synthetic datasets
    Vary domain size experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary dataset cardinality experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary interval length alpha experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary interval position sigma experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary query extent experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```

    Vary batch size or number of queries experiment (10 runs per query):
    ```sh 
    $ bash run_real_vary-qe.sh WEBKIT 12 query 10
    ```
