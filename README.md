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
    - `--remove-dictionary <dict#>` remove dictionary with ID


## lists
At the moment, there are two dictionaries in the `/dictionaries` folder

 - german.dic: https://sourceforge.net/projects/germandict/
 - english.dic: https://github.com/dwyl/english-words
