import random
import time
import math

dataSize = 2000000 # kiek atsitiktiniu skaiciu sugeneruoti

def countingSort(arr, place):
    count = [0] * 10

    for num in arr:
        count[(num // place) % 10] += 1

    for i in range(1, 10):
        count[i] += count[i - 1]

    output = [0] * dataSize
    i = dataSize - 1
    while i >= 0:
        output[count[(arr[i] // place) % 10] - 1] = arr[i]
        count[(arr[i] // place) % 10] -= 1
        i -= 1

    arr[:] = output

def radixSort(arr, size):
    place = 1
    while size / place >= 1:
        countingSort(arr, place)
        place *= 10



#MAIN

temp_data = []

# random skaiciu rusiavimas
for i in range(3):
    print(f"Rusiuojamu duomenu skaitmenu kiekis: {2 ** (i + 1)}")
    print("-----------------------------------------------------")
    average_radix = 0
    for j in range(6):
        num_count = 2 ** (i + 1)
        file_name = f"data_{2 ** (i + 1)}_{dataSize}.txt"

        with open(file_name, 'r') as input_file:
            temp_data = [int(line.strip()) for line in input_file]

        print(f"{j + 1} testo laikas:")
        print(type(temp_data))
        start1 = time.time()
        radixSort(temp_data, 10 ** (2 ** (i + 1)))
        stop1 = time.time()
        duration_radix = (stop1 - start1) * 1000  # milliseconds
        print(f"{duration_radix:.2f} ms")
        print("----------------------------------------------------")
        average_radix += duration_radix
        del temp_data[:]

    print(f"\nVidutinis laikas: {average_radix / 6:.2f} ms\n")
    print("-----------------------------------------------------\n")