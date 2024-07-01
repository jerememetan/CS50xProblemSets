from cs50 import get_float


def main():
    change = round(get_float("Change: "), 2) * 100
    if change < 0:
        print("Change must be a valid interger")
        return main()
    amount = calcamount(change)
    print(f"{amount}")


def calcamount(n):
    quarter = 25
    dime = 10
    nickel = 5
    penny = 1
    counter = 0
    while n >= 0:
        if n >= quarter:
            counter += 1
            n -= quarter
        elif n >= dime:
            counter += 1
            n -= dime
        elif n >= nickel:
            counter += 1
            n -= nickel
        elif n >= penny:
            counter += 1
            n -= penny
        else:
            return counter


main()
