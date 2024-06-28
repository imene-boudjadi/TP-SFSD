**TP-SFSD**

This project aims to compare the access counts (reads and writes) and the load factor between TOF and LOF models, as well as among four combinations of LOF. 
The project involves generating CSV-formatted text files detailing access counts and load factors, and creating corresponding graphs.


Implementation Details : 

- Abstract Machine Implementation :
Implemented abstract machine models for both TOF and LOF.

- INSERTION 1 :
Handles normal insertions and allocates new blocks if the threshold is exceeded.
Ensures no inter-block shifts unless necessary, optimizing insertion performance.

- INSERTION 2 :
Manages insertions by distributing data among blocks when the target block is full.
Ensures minimal inter-block gaps for efficient storage.

- SUPPRESSION 1 :
Manages physical deletions by shifting data within the block.
Adjusts block chaining to maintain integrity and potentially frees blocks if empty.

- SUPPRESSION 2 :
After deletion, redistributes data within adjacent blocks to optimize storage.
Utilizes block spaces efficiently by redistributing surplus data.

- Initial Setup and Data Handling : 
Initially created a text file containing 100,000 randomly generated keys, sorted and annotated with 'I' (insertion) or 'S' (suppression).
Used this file for TOF and LOF loading.
