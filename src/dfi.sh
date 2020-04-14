if [ $# -ne 1 ]
then
  echo "Usage: $0 <file>"
  exit 1
fi

ls -la $1

if [ $? -ne 0 ]
then
  exit
fi

filetype=`file -b $1`
printf "Type:\t%s\n" "$filetype"

if command -v exiftool &> /dev/null
then
  out=`exiftool $1`
  if [ $? -eq 0 ]
  then
    echo "$out"
  fi
fi
