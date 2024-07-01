from cs50 import get_int


def main():
    height = get_int("Height: ")
    if height < 1 or height > 8:
        print("Only Accepts value 1 to 8")
        return main()
    makepyramid(height, 1)


def makepyramid(n, x):
    n -= 1
    if n < 0:
        return
    space = 2
    for blank in range(n):
        print(" ", end="")
    for brick1 in range(x):
        print("#", end="")
    print(" " * space, end="")
    for brick2 in range(x):
        print("#", end="")
    print()
    makepyramid(n, x+1)


main()
