import sys
file = sys.argv[1]
count = 0
with open(file) as open_file:
    for line in open_file:
        count += 1
print count
