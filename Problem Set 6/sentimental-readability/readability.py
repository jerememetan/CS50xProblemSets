from cs50 import get_string


def main():
    text = get_string("Text: ")
    grade = calc(text)
    print(f"{grade}")
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def calc(string):
    letters = 0
    words = 1
    sentences = 0
    length = len(string)  # includes the space
    for l in string:
        if l.isalpha() == True:
            letters += 1

        elif l.isspace() == True:
            words += 1
        elif l in ("!", ".", "?"):
            sentences += 1
    if words == 1 and letters < 1:
        words = 0
    print(f"Letters: {letters}")
    print(f"Words: {words}")
    print(f"Sentences: {sentences}")
    index = 0.0588 * (letters/words * 100) - 0.296 * (sentences/words * 100) - 15.8
    return round(index)


main()
