#!/bin/bash

# exit this script if any commmand fails
set -e

# read user.cfg if it exists and is readable

_CFG_FILE=$(dirname "$0")"/user.cfg"
if [ -f "$_CFG_FILE" ]
then
    [ -r "$_CFG_FILE" ] || die "Fatal Error: $_CFG_FILE exists but is unreadable"
    . "$_CFG_FILE"
fi

# paths

if [ -z "${NDK_ROOT+aaa}" ]; then
# ... if NDK_ROOT is not set, use "$HOME/bin/android-ndk"
    NDK_ROOT="$HOME/bin/android-ndk"
fi

if [ -z "${CLANG_ROOT+aaa}" ]; then
# ... if CLANG_ROOT is not set, use "$HOME/bin/clang+llvm-3.1"
    CLANG_ROOT="$HOME/bin/clang+llvm-3.1"
fi

if [ -z "${PYTHON_BIN+aaa}" ]; then
# ... if PYTHON_BIN is not set, use "/usr/bin/python2.7"
    PYTHON_BIN="/usr/bin/python2.7"
fi

# find current dir
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# paths with defaults hardcoded to relative paths

if [ -z "${CROSSAPP_ROOT+aaa}" ]; then
    CROSSAPP_ROOT="$DIR/../../"
fi

if [ -z "${CXX_GENERATOR_ROOT+aaa}" ]; then
    CXX_GENERATOR_ROOT="$CROSSAPP_ROOT/tools/bindings-generator"
fi

if [ -z "${TOJS_ROOT+aaa}" ]; then
    TO_JS_ROOT="$CROSSAPP_ROOT/tools/tojs"
fi

echo "Paths"
echo "    NDK_ROOT: $NDK_ROOT"
echo "    CLANG_ROOT: $CLANG_ROOT"
echo "    PYTHON_BIN: $PYTHON_BIN"
echo "    CROSSAPP_ROOT: $CROSSAPP_ROOT"
echo "    CXX_GENERATOR_ROOT: $CXX_GENERATOR_ROOT"
echo "    TO_JS_ROOT: $TO_JS_ROOT"

# write userconf.ini

_CONF_INI_FILE="$PWD/userconf.ini"
if [ -f "$_CONF_INI_FILE" ]
then
    rm "$_CONF_INI_FILE"
fi

_CONTENTS=""
_CONTENTS+="[DEFAULT]"'\n'
_CONTENTS+="androidndkdir=$NDK_ROOT"'\n'
_CONTENTS+="clangllvmdir=$CLANG_ROOT"'\n'
_CONTENTS+="cocosdir=$CROSSAPP_ROOT"'\n'
_CONTENTS+="cxxgeneratordir=$CXX_GENERATOR_ROOT"'\n'
_CONTENTS+="extra_flags="'\n'

echo 
echo "generating userconf.ini..."
echo ---
echo -e "$_CONTENTS"
echo -e "$_CONTENTS" > "$_CONF_INI_FILE"
echo ---

# Generate bindings for CrossApp
echo "Generating bindings for CrossApp..."
set -x
LD_LIBRARY_PATH=${CLANG_ROOT}/lib $PYTHON_BIN ${CXX_GENERATOR_ROOT}/generator.py ${TO_JS_ROOT}/CrossApp.ini -s CrossApp -o ${CROSSAPP_ROOT}/scripting/javascript/bindings/generated -n jsb_CrossApp_auto

