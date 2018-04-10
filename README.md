# PasswordGen

Generate passwords from dictionaries.

The Bash implementation requires bc, The C one needs openssl (`libssl-dev` for example) to compile

## Usage

Flags:

 - `-l|--length <length>` Number of words
 - `-d|--del|--delimiter <delimiter>` delimiter
 - `-c|--clip` copy password to clipboard (requires [xclip](https://wiki.ubuntuusers.de/xclip/))
 - `--dictionary <dict#>` Only use one dictionary (find ID by passing `--verbose`)
  **WARNING** This could lead to insecure passwords due to a low number of possible words
 - `--dictpath <path>` Path to another dictionary directory (with list file)
 - `--verbose` See some useful output (dictionary overview, total number of words available, etc...)
 - **Manipulating Dictionaries**
    - `--add-dictionary <path-to-dict>` add dictionary at path
    - `--remove-dictionary <dict#>` remove dictionary at number `dict#` (can be identified with --verbose flag)


## lists
At the moment, there are two dictionaries in the `/dictionaries` folder

 - german.dic: https://sourceforge.net/projects/germandict/
 - english.dic: https://github.com/dwyl/english-words

## How this program chooses a random word from the Dictionaries

  1. Generate a random integer
    - bash: `od -A n -t u8 -N 8 /dev/urandom | tr -d ' '`, then calculate `rand % totalWordCount` (unsigned 64 bit integer from /dev/urandom)
    - C: Generate a random number between 0 and totalWordCount with `randombytes_uniform(uint64_t totalWordCount)`
  2. find out which dictionary has the word in it
  3. get that word from that dictionary

steps 2 and 3 are different if a specific dictionary is forced.
