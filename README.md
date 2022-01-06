## About NameGen:
NameGen uses a Markov model for learning real datasets, extracting information, and generating close-to-real names randomly. 
It enables end-users to tune the name characteristic (e.g., number and length of components). 

It removes the need to rely on available small datasets or to generate huge datasets of fully random names for network evaluation.
Without loss of generality, we focus on NDN names as an important use-case for future Internet architectures.

reference:
1. [Chavoosh Ghasemi*, Hamed Yousefi*, Kang G. Shin, and Beichuan Zhang (*co-first authors), _On the Granularity of Trie-Based Data Structures for Name Lookups and Updates_, in IEEE/ACM Transactions on Networking, vol. 27, no. 2, pp. 777-789, April 2019.](https://ieeexplore.ieee.org/document/8673766/)
2. [chavoosh/TrieGranularity](https://github.com/chavoosh/TrieGranularity)

## About O3-NameGen

This work use order-3 markov model instead of order-2 markov model to achieve more human-readable names. The main difference between O3-NameGen and NameGen is:
1. use order-3 markov model.
2. compress the char space from 128 to 64. Because o3 markov matrix need much more space than o2 matrix.
3. import a random transport factor to generate series which haven't seen in input dataset.
4. solve the `fatal bug` of Roulette algorithm in NameGen.
5. O3-NameGen print more specific information.
6. other small changes...

## How to run the program:

Go to directory `src` and compile the program by running:
    
    $make

In directory `src`, use the following command to run NameGen

    $./ng -i <input file> -n <number of records/URLs to learn> -m <number of new names to be generated> -o <optional characteristics (number and length of components) as input>

For example, one may run `$./ng -i ../dataset/output_urls.out -n 300000 -m 1000000`

Then, NameGen is trained by 300000 URLs from dataset `output_urls.out` and generates a new dataset of 1000000 NDN-like names.


