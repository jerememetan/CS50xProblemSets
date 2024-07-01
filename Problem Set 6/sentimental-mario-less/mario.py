from cs50 import get_int


def main():
    height = get_int("Height: ")
    if height < 1 or height > 8:
        print("Only Accepts value 1 to 8")
        return main()
    makepyramid(height)


def makepyramid(n):
    for i in range(n):
        for j in range(n-i-1):
            print(" ", end="")
        for k in range(i+1):
            print("#", end="")
        print()


main()
