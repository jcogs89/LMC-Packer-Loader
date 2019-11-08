import argparse
import os


# ToDo
def pack_one_to_one(filepath, password, opt_dir):
    # Compress with 7zip, encrypt with AES?
    pass


# ToDo
def pack_many_to_one(filepaths, password, opt_dir):
    # Compress with 7zip, encrypt with AES?
    pass


def get_dir_filepaths(dir):
    pathlist = []
    for root, dirs, filenames in os.walk(dir):
        for filename in filenames:
            pathlist.append(os.path.join(root, filename))
    return pathlist


def main(args):
    inp_dir = os.path.join(os.pardir, "packer_input")
    opt_dir = os.path.join(os.pardir, "packer_output")

    if args.input_directory:
        inp_dir = args.input_directory

    if args.output_dir:
        if os.path.exists(args.output_dir):
           opt_dir = args.output_dir
        else:
            print("Specified output directory does not exist; will output to default.")  # ToDo switch to logger

    if args.input_file:
        pack_one_to_one(args.input_file, args.password, opt_dir)
    else:  # input is a directory
        filepaths = get_dir_filepaths(inp_dir)
        if args.compress_output:
            pack_many_to_one(filepaths, args.password, opt_dir)
        else:
            for filepath in filepaths:
                pack_one_to_one(filepath, args.password, opt_dir)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--input_file', help='specify input as one file', type=str)
    parser.add_argument('-d', '--input_directory', help='specify input as all files in a directory', type=str)
    parser.add_argument('-o', '--output_dir', help='specify an output directory', type=str)
    parser.add_argument('-c', '--compress_output', help='put all input files into one output file', action='store_true')
    parser.add_argument('-p', '--password', help='specify a password to encrypt with', type=str)
    main(parser.parse_args())
