import pandas as pd
import matplotlib.pyplot as plt
import os

filenames = []
for root, dirs, files in os.walk("benchmarks"):
    for file in files:
        if file.endswith(".csv"):
            filenames.append(os.path.join(root, file))

def get_types_and_number(filename):
    end = filename.split('/')[-1].split('.')[0]
    type1, type2, number = end.split('_')
    return (type1, type2, int(number))

for filename in filenames:
    frame = pd.read_csv(filename, index_col=0)
    type1, type2, number = get_types_and_number(filename)
    plt.figure()
    frame.plot(kind="bar").set_yscale("log", base=2)
    plt.title(f"Average run time over dictionaries of {type1=}, {type2=}")
    plt.ylabel("time in nanoseconds")
    plt.xticks(rotation=7.5)
    plt.legend()
    plt.show()