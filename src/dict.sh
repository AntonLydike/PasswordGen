# stores if the dictionaries have already been imported
declare -i dictImported=0;
# 2d array with dictionary metadata
declare -A dicts;
# number of dictionaries
declare -i dictCount=0;

# imports dictionary metadata from the list file
function importDicts() {
	local i
	local j
	local ifs_old="$IFS";

	i=0;
	while read line; do
		if [ "$line" == "" ]; then
			continue;
		fi;

		IFS=':';
		j=0;
		for attr in $line; do
			dicts[$i,$j]="$attr";
			j=$((j+1));
		done;
		i=$((i+1));
		IFS="$ifs_old";
	done < $dictPath/list

	dictCount=$i;
	dictImported=1;
}

# checks checksums and updates metadata if necessary
# used sha256 for validation
function refreshDicts() {
	if (( $dictImported == 0 )); then
		return 0;
	fi;

	if (( "$verbose" == 1 )); then
		echo "checking dicts";
	fi;


	local i;
	local refreshed=0;
	for ((i=0;i<dictCount;i++)); do
		sum=$(sha256sum "$dictPath/${dicts[$i,0]}" | head -c 64);

		if (( "$verbose" == 1 )); then
			printf "  [$i] ${dicts[$i,0]}(${dicts[$i,1]}): $sum";
		fi;

		if [ "$sum" == "${dicts[$i,2]}" ]; then
			if (( "$verbose" == 1 )); then
				echo " [OK]";
			fi;
		else

			if (( "$verbose" == 1 )); then
				echo " [FAILED]";
			fi;

			refreshDict $i;
			refreshed=1;
		fi;
	done;

	if (( "$refreshed" == "1" )); then
		writeDicts
	fi;

}

# refreshes line count and checksum of dictionary
function refreshDict() {
	if (( $dictImported == 0 )); then
		return 0;
	fi;

	local dictNum="$1";

	if (( "$verbose" == 1 )); then
		echo "      Updating...";
	fi;


	path="$dictPath/${dicts[$dictNum,0]}";

	lines=$(wc -l < "$path");

	sha=$(sha256sum "$path" | head -c 64);

	dicts[$dictNum,1]="$lines";
	dicts[$dictNum,2]="$sha";
}

# writes new metadata to list file
function writeDicts() {
	if (( $dictImported == 0 )); then
		return 0;
	fi;

	outstr="";

	local i;
	for ((i=0;i<dictCount;i++)); do
		outstr="$outstr${dicts[$i,0]}:${dicts[$i,1]}:${dicts[$i,2]}\n";
	done;

	printf "$outstr" > $dictPath/list;

	if (( "$verbose" == 1 )); then
		echo "writing dicts";
	fi;

}

# gets the $2th word from the dictionary $1
function getNthWord() {
	local dict=$1;
	local line=$2;

	if (( "$verbose" == 1 )); then
		echo "Getting line $line from ${dicts[$dict,0]}";
	fi;

	local path="$dictPath/${dicts[$dict,0]}"
	word=$(sed "${line}q;d" "$path" | tr -d '\r')
}

function getTotalWordCount() {
	local count=0;
	local i;
	for ((i=0;i<dictCount;i++)); do
		count=$((count+${dicts[$i,1]}));
	done;

	totalWordCount=$count;
}

function addDictionary() {
	local file=$1;
	local dictName=$(basename $file);
	local path="$dictPath/$dictName";

	# copy file over
	cp $file $path;

	# strip newlines
	sed -i '/^\s*$/d' $path

	# count lines and compute hash
	local lines=$(wc -l <  "$path");
	local sha=$(sha256sum "$path" | head -c 64);

	echo "$dictName:$lines:$sha" >> "$dictPath/list"

	echo "Added $dictName with $lines words [$sha]"
}

function removeDictionary() {
	local dictid=$1;
	local name=${dicts[$dictid,0]};
	local hash=${dicts[$dictid,2]};

	rm $dictPath/$name;

	sed -i "/$dictName:*:$hash/d" $dictPath/list

	echo "Removed dictionary $name [$hash]".
}
