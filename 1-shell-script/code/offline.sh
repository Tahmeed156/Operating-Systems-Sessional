#!/bin/bash

ROOT=`pwd`

# ===== Helpers =====

display_usage() {
  echo -e "\nUsage:\n offline.sh file [workdir]"
  echo " file      - contains the type of files to ignore, each in separate lines"
  echo " workdir   - name of the root directory (defaults to '/')"
}

display_output() {
  echo -e "Done! Output:"
  echo " o/ - contains organized files"
  echo " count.csv - contains file types and counts"
}

# ===== Reading command line arguments =====

if [ "$#" -eq 2 ]; then
  input_file=$1
  work_dir=$2
elif [ "$#" -eq 1 ]; then
  work_dir=./
  input_file=$1
else
  echo "Error: Invalid command-line arguments"
  display_usage
  exit 1
fi

# Checking existence of file and directory

if [ ! -f "$input_file" ]; then
  while :
  do
    echo "Error: File '$input_file' not found. Please input a valid file."
    echo -n "File: "; read input_file
    [ -f "$input_file" ] && break
  done
else
  echo "File location: '$input_file'"
fi

if [ ! -d "$work_dir" ]; then
  while :
  do
    echo "Error: Invalid directory '$work_dir'. Please input a valid directory."
    echo -n "Workdir: "; read work_dir
    [ -d "$work_dir" ] && break
  done
else
  echo "Workding directory: '$work_dir'"
fi


# ===== Read from file =====

n=0
declare -A ignored_files

while read -r line || [ -n "$line" ]; do
  line=`echo $line | sed -e 's/\r//g'`
  n=$((n+1))
  ignored_files[$line]=1
done < $input_file

# ===== Finding & collecting files =====

declare -A file_num
output_dir="$ROOT/o"
mkdir -p "$output_dir"

list_files() {
  cd "$1"
  
  for f in *
  do
    if [ -d "$f" ]; then
      # Recursive call if directory
      list_files "$f"
    elif [ -f "$f" ]; then
      # Get file extension
      file_ext=`echo $f | rev | cut -f 1 -d '.' | rev`

      # Check whether extension is ignored
      if [ -n "${ignored_files[$file_ext]}" ]; then
        file_num[ignored]=$((file_num[ignored]+1))
        continue
      fi

      # For files with no extensions
      if [ "$file_ext" = "$f" ]; then
        file_ext=others
      fi

      # Create directory and copy files
      mkdir -p "$output_dir/$file_ext/"
      cp "$f" "$output_dir/$file_ext/$f"
    fi
  done

  cd ../
}

list_files "$work_dir"

# ====== Counting and listing files ======

for d in `ls $output_dir`
do
  file_num[$d]=`ls $output_dir/$d | wc -l`
  for f in `find $work_dir -type f -name "*.$d"`
  do
    echo "$f" >> "$output_dir/$d/desc_$d.txt"
  done
done
# Special case handling file without extensions
for f in `find $work_dir -type f ! -name "*.*"`
do
  echo "$f" >> "$output_dir/others/desc_others.txt"
done

# ====== Storing counts in CSV =====

echo "file_type,no_of_files" > count.csv
for x in "${!file_num[@]}"; 
do
  printf "%s,%s\n" "$x" "${file_num[$x]}" >> count.csv
done

display_output
exit 0
