# HINT on Steroids: Batch Query Processing for Interval Data

Source code for the "HINT on Steroids: Batch Query Processing for Interval Data" paper

## Dependencies
- g++/gcc
- Boost Library 


## Essentials

### Inputs
All real and synthetic datasets used in the paper are available on https://drive.google.com/drive/folders/1RY1v6Au7k_IFKNr6UE8y4djVfkvpO4Vb?usp=sharing.

Download ```real.tar.gz``` and ```synthetic.tar.gz``` archives and extract them inside the ```inputs``` directory.
A ```real``` and ```synthetic``` sub-directory will be automatically created to include each dataset. 

### Queries
Directory ```queries``` includes all queries used in the experiments for both real and synthetic datasets.

### Outputs
Directory ```outputs``` includes the reports after executing the bash scripts that reproduce the experiments in the paper.

## Usage

### To execute an evaluation strategy use the ```query_batch.exec``` executable:

### Parameters
| Parameter | Description | Comment |
| ------ | ------ | ------ |
| -m | set the number of bits for HINT | in experiments: 10 for BOOKS, 12 for WEBKIT, 17 for TAXIS, GREEND and all synthetics|
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

To reproduce all experiments follow the instructions below.
Reports will be written in the ```outputs``` directory; one file for each experiment.

### Real datasets
Execute experiments: 
- vary query extent, and
- vary # queries (batch size)

Repeat each test 10 times.
```
$ bash run_all_real.sh 10
```

### Synthetic datasets
Execute experiments:
- vary domain size
- vary dataset cardinality
- vary α (interval length)
- vary σ (interval position)
- vary extent
- vary # queries (batch size)
```
$ bash run_all_sythntic.sh 10
```
Repeat each test 10 times.
