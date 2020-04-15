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
    echo "exif:"
    echo "$out"
  fi
fi

if [[ $filetype =~ "image" ]]
then
  if command -v image_histogram &>/dev/null
  then
    executable="image_histogram"
  else
    executable="$(dirname $0)/image_histogram"
  fi

  out=`$executable $1`
  if [ $? -eq 0 ]
  then
    echo "Histogram:"
    echo "$out"
  fi
fi

if [[ $filetype =~ "ISO Media" ]]
then
  if command -v ffprobe &> /dev/null
  then
    echo "ffprobe:"
    ffprobe -hide_banner $1
  else
    echo "For more information, install ffprobe"
  fi
fi
