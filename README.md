# HINT on Steroids: Batch Query Processing for Interval Data

Source code for the "HINT on Steroids: Batch Query Processing for Interval Data" paper

A wide range of applications manage interval data. HINT was re- cently proposed to hierarchically index intervals in main memory. The index outperforms competitive structures by a wide margin, but under its current setup, HINT is able to service only single query requests. In practice however, real systems receive a large number of queries at the same time and so, our focus in this paper is on batch query processing. We propose two novel evaluation strategies termed level-based and partition-based, which both work in a per-level fashion, i.e., all queries for an index level are computed before moving to the next level. The new strategies op- erate in a cache-aware fashion to reduce the cache misses caused by climbing the index hierarchy or accessing multiple partitions per level, and to decrease the total execution time for a query batch. Our experimental analysis with both real and synthetic datasets showed that our batch processing strategies always out- perform a baseline that executes queries in a serial fashion, and that partition-based is overall the most efficient strategy.

## Dependencies
- g++/gcc
- Boost Library 

## Compile
Compile the source code using ```make all``` or simple ```make```.

## Essentials

### Inputs

Download ```real.tar.gz``` and ```synthetic.tar.gz``` archives and extract them inside the ```inputs``` directory.
A ```real``` and ```synthetic``` sub-directory will be automatically created to include each dataset. 

- Real datasets, https://drive.google.com/file/d/1MRe19YCCY99bc8F6YC-U9bjpU5s_h4d7/view?usp=share_link
- Synthetic datasets, https://drive.google.com/file/d/1wlSYbY7nsbGoQziUTNx9W2J0kkZD0GwS/view?usp=sharing

### Queries
Directory ```queries``` includes all queries used in the experiments for both real and synthetic datasets.

### Outputs
Directory ```outputs``` includes the reports after executing the bash scripts that reproduce the experiments in the paper.

## Usage

To execute an evaluation strategy use the ```query_batch.exec``` executable.

### Parameters
| Parameter | Description | Comment |
| ------ | ------ | ------ |
| -m | set the number of bits for HINT | in experiments: 10 for BOOKS, 12 for WEBKIT, 17 for TAXIS, 17 for GREEND and 17 for all synthetics|
| -b | select the batch evaluation strategy | "query" or "level" or "partition"; by default "query" |
| -s | sort queries by their start | mandatory for the level-based and the partition-based strategies |
| -r | set number of runs per query | by default 1 |

### Examples

QUERY-based:

    $ ./query_batch.exec -m 10 -b query -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry

QUERY-based with sorting:
    
    $ ./query_batch.exec -m 10 -b query -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry
    
LEVEL-based with sorting:

    $ ./query_batch.exec -m 10 -b level -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry

PARTITION-based with sorting

    $ ./query_batch.exec -m 10 -b partition -s -r 10 inputs/real/BOOKS.inp queries/real/BOOKS_qe0.1%_qn10K.qry

## Experiments

To reproduce all experiments in the paper follow the instructions below.
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
- vary query extent
- vary # queries (batch size)
```
$ bash run_all_synthetic.sh 10
```
Repeat each test 10 times.
