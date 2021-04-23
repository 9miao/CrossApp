#!/bin/bash
SRC_PATH=""


DIR=$(cd $(dirname $0) && pwd )
JSB_EXEC="$DIR/../../tools/plugin_jscompile/bin/jsbcc"
echo  $DIR

function getdir(){
    
    for element in `ls $1`
      do
        dir_or_file=$1"/"$element
    if [ -d $dir_or_file ]
      then
        getdir $dir_or_file
      else
        FILE_ARRAY=(${dir_or_file[*]}  ${FILE_ARRAY[*]})
    fi
done
}

FILE_ARRAY=()
function help(){
    echo "
    USAGE EXAMPLE: ./js_to_jsc -s srcpath -d dstpath
    HELP
    "
    exit 0
}
rm -rf "$DIR/release"
if [ ! -d "$DIR/release" ]; then
  mkdir "$DIR/release"
  cp -r "$DIR/Resources" "$DIR/release/Resources"
  SRC_PATH="$DIR/release/Resources"
fi

getdir $SRC_PATH
for item in ${FILE_ARRAY[*]}
  do
    SRC_FILE=$item
    SRC=${SRC_PATH#*/}
    if [ "${item##*.}"x = "js"x ];then
      # echo $item
      EXEC_STR=${item/$SRC/$SRC}
      EXEC_STR=${EXEC_STR%.*}
      CMD="$JSB_EXEC $SRC_FILE "$EXEC_STR".cross"

      $CMD
      rm -rf $item
    fi
  done
cd "$DIR/release"
#cd $DIR/"
zip -r "Resources.zip" "./Resources"
#cd ..







