import os
import gzip

FS_DATA_LINE_SIZE = 16
FSDATA_FILE_PATH = 'fsdata.c'  # Path to the file

FSDATA_FILE_RAW_DATA_DECLARATION = """#if FSDATA_FILE_ALIGNMENT==1
static const unsigned int dummy_align_{name} = 0;
#endif
static const unsigned char FSDATA_ALIGN_PRE data_{name}[] FSDATA_ALIGN_POST = {{\n"""

FSDATA_STATIC_CONTENT = """#include "lwip/apps/fs.h"
#include "lwip/def.h"

#define file_NULL (struct fsdata_file *) NULL

#ifndef FS_FILE_FLAGS_HEADER_INCLUDED
#define FS_FILE_FLAGS_HEADER_INCLUDED 1
#endif
#ifndef FS_FILE_FLAGS_HEADER_PERSISTENT
#define FS_FILE_FLAGS_HEADER_PERSISTENT 0
#endif
/* FSDATA_FILE_ALIGNMENT: 0=off, 1=by variable, 2=by include */
#ifndef FSDATA_FILE_ALIGNMENT
#define FSDATA_FILE_ALIGNMENT 0
#endif
#ifndef FSDATA_ALIGN_PRE
#define FSDATA_ALIGN_PRE
#endif
#ifndef FSDATA_ALIGN_POST
#define FSDATA_ALIGN_POST
#endif
#if FSDATA_FILE_ALIGNMENT==2
#include "fsdata_alignment.h"
#endif\n\n"""

FSDATA_FILE_STRUCT = """const struct fsdata_file file_{FILE_PATH}[] = {{{{
file_{NEXT_FILE_NAME},
data_{FILE_PATH},
data_{FILE_PATH} + {FILE_PATH_SIZE},
sizeof(data_{FILE_PATH}) - {FILE_PATH_SIZE},
FS_FILE_FLAGS_HEADER_INCLUDED | FS_FILE_FLAGS_HEADER_PERSISTENT,
}}}}; \n\n"""

FSDATA_FILE_END = """#define FS_ROOT file__index_html
#define FS_NUMFILES {NUM_OF_FILES}"""


fsdata_file = open(FSDATA_FILE_PATH, "w+")

# Write static content 
fsdata_file.write(FSDATA_STATIC_CONTENT)

files_structs_declaration = ""

try:
    # Get the absolute path of the directory
    base_dir = os.path.abspath('fs')
    # Loop through the directory
    next_file_name = "NULL"
    number_of_files =  0
    for root,dirs, files in os.walk(base_dir):      
        for file in files:
            number_of_files+= 1
            file_path = os.path.join(root, file)

            relative_path = os.path.relpath(file_path, base_dir)
            relative_path = "/" + relative_path.replace("\\","/")
            print("Converting " + relative_path)
            file_name = (relative_path.replace("/","_")).replace(".","_")
            fsdata_file.write(FSDATA_FILE_RAW_DATA_DECLARATION.format(name = file_name))
            
            hex_array = ""
            for i in range(len(relative_path)):
                hex_array+= "0x{:02x},".format(ord(relative_path[i]))
                if((i+1) % FS_DATA_LINE_SIZE == 0) : 
                    hex_array+="\n"

            hex_array+="0x00,\n"                    
            fsdata_file.write(hex_array)

            hex_array = ""
            with open(file_path, "rb") as file_raw:
                byte_data_raw = file_raw.read()
                #compressed_raw_data = byte_data_raw
                compressed_raw_data = gzip.compress(byte_data_raw,9)
                for i in range(len(compressed_raw_data)): 
                    hex_array+= "0x{:02x}".format(compressed_raw_data[i])
                    if(i != (len(compressed_raw_data)-1)) : hex_array+=","
                    if((i+1) % FS_DATA_LINE_SIZE == 0) : 
                        hex_array+="\n"

            hex_array += "};\n\n"
            fsdata_file.write(hex_array)

            # prepare file structs declaration
            
            files_structs_declaration += FSDATA_FILE_STRUCT.format(NEXT_FILE_NAME = next_file_name,FILE_PATH = file_name, FILE_PATH_SIZE = len(relative_path)+1)
            next_file_name = file_name
           
    fsdata_file.write(files_structs_declaration)

    fsdata_file.write(FSDATA_FILE_END.format(NUM_OF_FILES = number_of_files))

except Exception as e:
    print(f"An error occurred: {e}")    


