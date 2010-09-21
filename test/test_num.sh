#!/bin/sh
SBF2C_OPTS=""

BIN_PREFIX="./"
DICT_FILE="-d ../sfrth2bf/dict.4th"

usage() {
	echo "Usage: $0 [--bin-prefix|-p <prefix>] [--dict-file|-d <dict_file>] [--help|-h] [--system|-s]"
	exit 0
}
while [ -n "$1" ]; do
	case "$1" in
		--bin-prefix|-p)
			BIN_PREFIX="$2"
			shift 2
			;;
		--dict-file|-d)
			if [ -n "$2" ]; then
				DICT_FILE="-d $2"
			else
				DICT_FILE=""
			fi
			shift
			;;
		--help|-h)
			usage
			;;
		--system|-s)
			BIN_PREFIX=""
			DICT_FILE=""
			;;
		--*|-*)
			echo "Invalid option -- '$1'"
			usage
			;;
		*)
			;;
	esac
	shift
done
if [ -n "${BIN_PREFIX}" ]; then
	BF2C="${BIN_PREFIX}sbf2c/sbf2c"
	FRTH2BF="${BIN_PREFIX}sfrth2bf/sfrth2bf"
else
	BF2C="`which sbf2c`"
	FRTH2BF="`which sfrth2bf`"
fi
echo "BIN_PREFIX = ${BIN_PREFIX}"
echo "BF2C = ${BF2C}"
echo "FRTH2BF = ${FRTH2BF}"
echo "DICT_FILE = ${DICT_FILE}"
if [ ! -e "$BF2C" ]; then
	echo "Can't find sbf2c" >&2
	exit 1
fi
if [ ! -e "$FRTH2BF" ]; then
	echo "Can't find sfrth2bf" >&2
	exit 1
fi


			
#if [ "$1" == "test" ]; then
#	SBF2C_OPTS=" -D 0 -L 3"
#fi
gen_incs () 
{
	local num="$1"
	while [ "$num" -gt 0 ]; do
		echo -n '+'
		num=$(( $num - 1 ))
	done
}
gen_code ()
{
	local a="$1"
	local b="$2"
	local c="$3"
	local n="$4"
	local m="$5"

	echo "`gen_incs $a`> `gen_incs $b`>  `gen_incs $c` < [<`gen_incs $n`>-] > [<<`gen_incs $m`>>-]"
}

gen_code_test ()
{
	local a="$1"
	local b="$2"
	local c="$3"
	local n="$4"
	local m="$5"

	local bf="`gen_code $a $b $c $n $m`"
	echo "$bf"
	echo -n "$a + $n*$b + $m*$c = "
	echo "$bf" | ./sbf2c/sbf2c $SBF2C_OPTS | gcc -O2 -x c - && ./a.out
	echo
}
#gen_code_test 11 12 12 12 12
test_nums () {
	#local src_len=0
	local test_i
	for i in `seq 0 255`; do
		echo "$i" | "${FRTH2BF}" $DICT_FILE -o test.4th
		"${BF2C}" -D 1 -L -1 -o test.c test.4th
		#src_len=$(( $src_len + `wc -c src.4th | awk '{ print $1; }'` ))
		test_i=`gcc -O2 test.c && ./a.out 2>&1`
		#echo "$test_i"
		if [ -z "$test_i" ] ; then
			echo
			echo "Test failed: \`$test_i', but expected $i"
			#echo "frh_src: `cat src.4th`"
			#echo "c_src: $c_src"
		elif [ "$i" -ne "$test_i" ]; then
			echo
			echo "Test failed: \`$test_i', but expected $i"
			echo "frh_src: $frth_src"
		else
			echo -n "."
		fi
	
		#echo $i | ./sfrth2bf/sfrth2bf -d ../sfrth2bf/dict.4th #| ./sbf2c/sbf2c -D 0 | gcc -O2 -x c - && ./a.out
	done
	echo
	#echo "avg bf-code length = "$(( $src_len / 256))
}
test_div10 () {
	for i in `seq 0 255`; do
		echo "$i /10" | "${FRTH2BF}" $DICT_FILE -o test.bf
		"${BF2C}" -D 1 -L -1 -o test.c test.bf
		test_i="`gcc -O2 test.c && ./a.out 2>&1`"
		if [ -z "$test_i" ]; then
			echo
			echo "Test $i failed: \`$test_i', but expected $(( $i / 10 ))"
		elif [ "$test_i" -ne "$(( $i / 10))" ]; then
			echo
			echo "Test $i failed: \`$test_i', but expected $(( $i / 10 ))"
			gcc -O2 
		else
			echo -n "."
		fi
	done
	echo
}

test_ints() {
	for i in `seq 0 255`; do
		echo "$i .INT" | "${FRTH2BF}" $DICT_FILE -o test.bf
		"${BF2C}" -o test.c test.bf
		test_i="`gcc -O2 test.c && ./a.out`"
		if [ -z "$test_i" ]; then
			echo
			echo "Test failed: \`$test_i', but expected $i"
		elif [ "$test_i" -ne "$i" ]; then
			echo
			echo "Test failed: \`$test_i', but expected $i"
		else
			echo -n "."
		fi
	done
	echo
}
#test_div10
test_ints
