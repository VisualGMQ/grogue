import cpp_header_parser as chp
import os

CodeInfos: list[chp.CodeInfo] = []

def ApplyOnEachFile(dirname: str, ignore_filenames: list[str], func):
    for dirpath, _, filenames in os.walk(dirname):
        for filename in filenames:
            complete_path = (dirpath + '/' + filename).replace('\\', '/')
            if complete_path in ignore_filenames:
                print("ignore file ", complete_path)
                continue
            print('dealing file ', complete_path, '...')
            func(complete_path)

def GetCodeInfo(filename: str):
    global CodeInfos
    CodeInfos.append(chp.ParseHeaderFile(filename))

if __name__ == '__main__':
    ignore_filenames = []
    with open('./utilities/cpp_parser/ignore_files.txt') as f:
        ignore_filenames = list(map(lambda s: s.strip(), f.readlines()))
    ApplyOnEachFile('./include', ignore_filenames, GetCodeInfo)

    class_num = 0
    gfunc_num = 0

    for info in CodeInfos:
        class_num = class_num + len(info.classes)
        gfunc_num = gfunc_num + len(info.global_functions)
    print('{} classes, {} global functions'.format(class_num, gfunc_num))
