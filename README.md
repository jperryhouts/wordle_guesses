# Wordle Solver

Identify the best first wordle guess based on how many options it would eliminate.

Depending on the answer to any particular game, the first guess will eliminate a different number of options. E.g. if the answer is `GREYS`, then plaing `GREET` would leave you with only 3 possible second guesses. But if the answer is `SLATE` then `GREET` would not have ruled out many options. This program considers, for each possible guess, how many options it would eliminate *on average* given lots of different possible answers.

See [results.txt](results.txt) for all guesses relative to all possible answers.

## Basic usage

Compile:
```bash
g++ -o wordle wordle.cc
```

Run
```bash
./wordle dictionary.txt > results.txt
```

## Only consider answers that have actually been used in previous wordle games

```bash
./wordle --answers answers.txt
```

## Speed up by parallelizing the processing

Use separate terminals to execute 6 batch processes with 1500 rows each:
```bash
time ./wordle --start 0 --end 1500 dictionary.txt > 'res_0_1500.txt'
time ./wordle --start 1500 --end 3000 dictionary.txt > 'res_1500_3000.txt'
time ./wordle --start 3000 --end 4500 dictionary.txt > 'res_3000_4500.txt'
time ./wordle --start 4500 --end 6000 dictionary.txt > 'res_4500_6000.txt'
time ./wordle --start 6000 --end 7500 dictionary.txt > 'res_6000_7500.txt'
time ./wordle --start 7500 --end 9000 dictionary.txt > 'res_7500_9000.txt'
```

When all processes have finished, combine batched results:
```bash
sort -g -k2 -r res_* > results.txt
```
