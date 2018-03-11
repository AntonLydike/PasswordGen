# generates a random 31 bit number (between 0 and around 2147483648), stores it
# in $random. This should be fine if the dictionaries don't get exceedingly
# gigantic
function getRandom() {
  # get an 8 byte (64 bit) unsigned int from /dev/urandom, remove leading whitespace
	random=$(od -A n -t u8 -N 8 /dev/urandom | tr -d ' ');
}

# gets a random word from dictionary with number in arg 1
function getRandomWordFromDict() {
	local dict=$1;
	local dictSize=${dicts[$dict,1]};
	# number of lines - 1
	dictSize=$((dictSize));

	# get a random line number
	getRandom;
	local line=$((random%dictSize));
	# sed starts counting at 1. Ugh.
	line=$((line+1));

	getNthWord $dict $line;
}


# get a random word from a random dictionary (can be overwritten by $forceDict)
function getRandomWord() {
	if [[ "$forceDict" == 'FALSE' ]]; then
		chooseDict
	else
		randomDict=$forceDict;
	fi;

	getRandomWordFromDict $randomDict
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
