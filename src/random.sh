
# generates a random 31 bit number (between 0 and around 2147483648), stores it
# in $random. This should be fine if the dictionaries don't get exceedingly
# gigantic
function getRandom() {
	local num=$(od -A n -t d -N 4 /dev/urandom | tr -d ' ');

	# make sure only positive integers are generated
	# this gives us just 31 bits of randomness thought :(
	if (( "$num" < 0 )); then
		num=$((num*(-1)));
	fi;

	random=$num;
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

# chooses a random dictionary
function chooseDict() {
	getRandom

	randomDict=$((random%dictCount));
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

# get $1 random words, separated by $2
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
