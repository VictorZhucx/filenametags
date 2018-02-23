#! python3
#filenametags.py : Generate filename tags for gvim on windows platform
import sys
import re
import os
import glob

def Usage():
    print('Usage: filenametags.py <dir> <"py|html"> [add] [query]')
    print('       <dir> is your generate path')
    print('       ["py|html"] is your file format')
    print('       [add] means to add or not add. default is not add')
    print('       [query] means to query the format of filenameTags')
    exit()

def getFormatStr(lineStr):
    formRegExp = re.compile(r'(\.\w*$)')
    if formRegExp.search(str(lineStr)):
        ret = formRegExp.search(str(lineStr)).group()
    else:
        ret = ''
    return ret

def queryFormat(fileNamePath):
    fileName = os.path.abspath(fileNamePath) + '\\filenametags'
    if not os.path.exists(fileName):
        print('Error: The filenametags doesnot exist')
        exit()
    with open(fileName, 'r') as tagsFile:
        tagsContext = tagsFile.readlines()
        formatList = [getFormatStr(eachContext) for eachContext in tagsContext]
        print('The format has: ', end='')
        for eachFormat in sorted(set(formatList)):
            if str(eachFormat) != '':
                print(str(eachFormat) + ' ', end='')
        exit()

def progArgs(prar):
    isAddFlag = 0
    formatList = []
    if len(prar) < 3:
        Usage()
    elif len(prar) == 4:
        if prar[3] == 'add':
            isAddFlag = 1
        elif prar[3] == 'query':
            queryFormat(prar[1])
        else:
            Usage()
    else:
        isAddFlag = 0
    for each_format in str(prar[2]).split('|'):
        formatList.append(each_format)
    return prar[1], formatList, isAddFlag

#step 1: progress args
(dirPath, matchExp, isAdd) = progArgs(sys.argv)

#step 2: get the absolute path
theSearchPath = os.path.abspath(dirPath)

#step 3: enter the absolute path
os.chdir(theSearchPath)

#step 4: get the file name
if isAdd == 0:
    fileFlag = 'w+'
else:
    fileFlag = 'a+'
with open('filenametags', fileFlag) as fileNameTags:
    if isAdd == 0:
        print('!_TAG_FILE_SORTED   2   //2=foldcase//', file=fileNameTags)
    for eachFileFormat in matchExp:
        eachExp = "**\*." + str(eachFileFormat)
        findFile = glob.glob(eachExp, recursive=True)
        for eachFile in findFile:
            print(theSearchPath + str(eachFile), file=fileNameTags)
