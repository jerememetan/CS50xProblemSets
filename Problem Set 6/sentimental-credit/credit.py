from cs50 import get_int


def main():
    numberarray = []
    number = get_int("Number: ")
    converttoarray(number, numberarray)
    if checksum(numberarray) % 10 != 0:
        print("INVALID")
        return 1
    else:
        if checkcard(numberarray) == 1:
            print("INVALID")

def converttoarray(n, array):
    if n == 0:
        return
    arrayvalue = n % 10
    n /= 10
    array.insert(0, int(arrayvalue))
    converttoarray(int(n), array)


def checksum(array):
    evennum = len(array) - 2
    oddnum = len(array) - 1
    evenvalues = 0
    oddvalues = 0
    while evennum >= 0:
        value = array[evennum] * 2
        if value >= 10:
            evenvalues += (value % 10) + 1
            evennum -= 2
        else:
            evenvalues += value
            evennum -= 2
    while oddnum >= 0:
        oddvalues += array[oddnum]
        oddnum -= 2

    totalvalue = evenvalues + oddvalues
    return totalvalue


def checkcard(array):
    visalength = 13
    mastercardlength = 16
    americanlength = 15
    if len(array) == visalength:
        if array[0] == 4:
            print("VISA")
            return 0
        else:
            return 1
    if len(array) == mastercardlength:
        if array[0] == 4:
            print("VISA")
            return 0
        elif array[0] == 5 and array[1] >= 1 and array[1] <= 5:
            print("MASTERCARD")
            return 0
        else:
            return 1
    if len(array) == americanlength:
        if array[0] == 3 and array[1] in (4, 7):
            print("AMEX")
            return 0
        else:
            return 1
    else:
        return 1


main()
