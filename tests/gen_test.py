import sys
from random import randint
import random


userCount= int(sys.argv[1])
itemCount= int(sys.argv[2])

itemList = []
for i in range(itemCount):
    itemList.append(i)




file = open(str(userCount) + '-users.txt' ,'w')
print >> file, str(userCount)
print >> file, str(itemCount)

for i in range(userCount):
    random.shuffle(itemList)
    tempList = []
    for j in range(itemCount):
        tempList.append(99)

    for j in range(itemCount):
        if j < itemCount/2:
            num = randint(1, 10)
            tempList[itemList[j]] = num
        else:
            tempList[itemList[j]] = 99

    for j in range(itemCount):
        file.write(str(tempList[j]) + ' ')

    print >> file, '\t'
