import timeit


def custom_csv_reader_perf():
    import customcsv

    customcsv.read_csv_file("example.csv")


def csv_reader_perf():
    import csv

    with open("example.csv") as f:
        csv.reader(f)


# Measure the time taken by csv_reader
csv_reader_time = timeit.timeit(custom_csv_reader_perf, number=100000)

print(f"customcsv time: {csv_reader_time} seconds")

csv_reader_time = timeit.timeit(csv_reader_perf, number=100000)

print(f"csv reader time: {csv_reader_time} seconds")
