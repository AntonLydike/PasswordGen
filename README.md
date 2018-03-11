# PasswordGen

Generate passwords from dictionaries.

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
    - bash: `od -A n -t u8 -N 8 /dev/urandom | tr -d ' '` (unsigned 64 bit integer from /dev/urandom)
    - C:
  2. calculate rand % totalWordCount
  3. find out which dictionary has the word in it
  4. get that word from that dictionary

steps 2 and 3 are different if a specific dictionary is forced.
