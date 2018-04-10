# some functions to work with big numbers, uses bc
function mod() {
  mod_result=$(echo "$1%$2" | bc)
}
function minus() {
  minus_result=$(echo "$1-$2" | bc)
}
function lowerThan() {
  return $(echo "$1>=$2" | bc)
}

# generates a random 64 bit number and stores it in $random.
function getRandom() {
  # get an 8 byte (64 bit) unsigned int from /dev/urandom, remove leading whitespace
	random=$(od -A n -t u8 -N 8 /dev/urandom | tr -d ' ');
}

# get a random word from a random dictionary (can be overwritten by $forceDict)
function getRandomWord() {
  local remainder
  local dict

  getRandom

  if [[ "$forceDict" == 'FALSE' ]]; then
    # if no dictionary was forced, calc mod totalWordCount
    mod $random $totalWordCount
    random=$mod_result
    remainder=$random

    # find dict containing that word
    dict=0
    while ( lowerThan ${dicts[$dict,1]} $remainder ); do
      minus $remainder ${dicts[$dict,1]}
      remainder=$minus_result
      dict=$((dict + 1))
    done

    random=$remainder
	else
		dict=$forceDict
    random=$((random % ${dicts[$dict,1]}))
	fi;

	getNthWordFromDict $dict $random
}

# get $1 random words, separated by $2 (needs totalWordCount from getTotalWordCount())
function getRandomWords() {
	local count=$1
	local delimiter=$2
	local str="";

	local i
	for ((i=0;i<count;i++)); do
		getRandomWord;
		str="${str}${word}";
		if (( "$i" < $((count-1)) )); then
			str="${str}${delimiter}";
		fi;
	done;

	randomWords=$str;
}
